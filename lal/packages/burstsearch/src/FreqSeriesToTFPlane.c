/*
 * $Id$
 *
 * Copyright (C) 2007  Kipp Cannon and Flanagan, E.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include <lal/LALRCSID.h>


NRCSID(FREQSERIESTOTFPLANEC, "$Id$");


#include <math.h>
#include <lal/Date.h>
#include <lal/FrequencySeries.h>
#include <lal/LALConstants.h>
#include <lal/LALDatatypes.h>
#include <lal/RealFFT.h>
#include <lal/Sequence.h>
#include <lal/TFTransform.h>
#include <lal/Units.h>


/*
 * Compute the magnitude of the inner product of two arbitrary channel
 * filters.  Note that the sums are done over only the positive frequency
 * components, so this function multiplies by the required factor of 2.
 * The result is the *full* inner product, not the half inner product.  It
 * is safe to pass the same filter as both arguments.
 */


static REAL8 filter_inner_product(
	const COMPLEX8FrequencySeries *filter1,
	const COMPLEX8FrequencySeries *filter2,
	const REAL4Sequence *correlation
)
{
	const int k10 = filter1->f0 / filter1->deltaF;
	const int k20 = filter2->f0 / filter2->deltaF;
	int k1, k2;
	COMPLEX16 sum = {0, 0};

	for(k1 = 0; k1 < (int) filter1->data->length; k1++) {
		const COMPLEX8 *f1data = &filter1->data->data[k1];
		for(k2 = 0; k2 < (int) filter2->data->length; k2++) {
			const COMPLEX8 *f2data = &filter2->data->data[k2];
			const unsigned delta_k = abs(k10 + k1 - k20 - k2);
			const double sksk = (delta_k & 1 ? -1 : +1) * (delta_k < correlation->length ? correlation->data[delta_k] : 0);

			sum.re += sksk * (f1data->re * f2data->re + f1data->im * f2data->im);
			sum.im += sksk * (f1data->im * f2data->re - f1data->re * f2data->im);
		}
	}

	return 2 * sqrt(sum.re * sum.re + sum.im * sum.im);
}


/*
 * Generate the frequency domain channel filter function.  The filter is
 * nominally a Hann window twice the channel's width, centred on the
 * channel's centre frequency.  The filter is normalized so that its sum
 * squares is 1.  If the psd parameter is not NULL, then the filter is
 * divided by the square root of this frequency series prior to
 * normalilization.  This has the effect of de-emphasizing frequency bins
 * with high noise content, and is called "over whitening".
 */


static COMPLEX8FrequencySeries *generate_filter(
	const COMPLEX8FrequencySeries *template,
	REAL8 channel_flow,
	REAL8 channel_width,
	const REAL4FrequencySeries *psd,
	const REAL4Sequence *correlation
)
{
	static const char func[] = "generate_filter";
	char filter_name[100];
	REAL4Window *hann;
	COMPLEX8FrequencySeries *filter;
	unsigned i;
	REAL4 norm;

	sprintf(filter_name, "channel %g +/- %g Hz", channel_flow + channel_width / 2, channel_width / 2);

	/*
	 * Channel filter is a Hann window twice the channel's width,
	 * centred on the channel's centre frequency.  This makes a sum
	 * across channels equivalent to constructing a Tukey window
	 * spanning the same frequency band.  This trick is one of the
	 * ingredients that allows us to accomplish a multi-resolution
	 * tiling using a single frequency channel projection.  Really,
	 * there's no need for the "effective window" resulting from
	 * summing across channels to be something that has a name, any
	 * channel filter at all would do, but this way the code's
	 * behaviour is more easily understood --- it's easy to say "the
	 * channel filter is a Tukey window of variable centre width".
	 *
	 * Note:  the number of samples in the window is odd, being one
	 * more than the number of frequency bins in twice the channel
	 * width.  This gets the Hann windows to super-impose to form a
	 * Tukey window.  (you'll have to draw yourself a picture).
	 */

	filter = XLALCreateCOMPLEX8FrequencySeries(filter_name, &template->epoch, channel_flow - channel_width / 2, template->deltaF, &lalDimensionlessUnit, 2 * channel_width / template->deltaF + 1);
	hann = XLALCreateHannREAL4Window(filter->data->length);
	if(!filter || !hann) {
		XLALDestroyCOMPLEX8FrequencySeries(filter);
		XLALDestroyREAL4Window(hann);
		XLAL_ERROR_NULL(func, XLAL_EFUNC);
	}
	for(i = 0; i < filter->data->length; i++) {
		filter->data->data[i].re = hann->data->data[i];
		filter->data->data[i].im = 0.0;
	}
	XLALDestroyREAL4Window(hann);

	/*
	 * divide by PSD if needed
	 */

	if(psd) {
		REAL4 *pdata = psd->data->data + (int) ((filter->f0 - psd->f0) / psd->deltaF);
		for(i = 0; i < filter->data->length; i++) {
			filter->data->data[i].re /= sqrt(pdata[i]);
			filter->data->data[i].im /= sqrt(pdata[i]);
		}
	}

	/*
	 * normalize the filter.  the filter needs to be normalized so that
	 * it's inner product with itself is 1.
	 */

	norm = sqrt(filter_inner_product(filter, filter, correlation));
	for(i = 0; i < filter->data->length; i++) {
		filter->data->data[i].re /= norm;
		filter->data->data[i].im /= norm;
	}

	/*
	 * success
	 */

	return filter;
}


/*
 * Multiply the data by the filter.
 */


static COMPLEX8Sequence *apply_filter(
	COMPLEX8Sequence *outputseq,
	const COMPLEX8FrequencySeries *inputseries,
	const COMPLEX8FrequencySeries *filterseries
)
{
	static const char func[] = "apply_filter";
	int fstart = (filterseries->f0 - inputseries->f0) / filterseries->deltaF;
	COMPLEX8 *output = outputseq->data + (fstart < 0 ? 0 : fstart);
	const COMPLEX8 *input = inputseries->data->data + (fstart < 0 ? 0 : fstart);
	const COMPLEX8 *filter = filterseries->data->data + (fstart < 0 ? -fstart : 0);
	/* an extra 1 is subtracted to ensure the Nyquist is set to 0 */
	size_t fbins = outputseq->length - (fstart < 0 ? 0 : fstart) - 1;
	if(filterseries->data->length - (fstart < 0 ? -fstart : 0) < fbins)
		fbins = filterseries->data->length - (fstart < 0 ? -fstart : 0);

	if(outputseq->length != inputseries->data->length)
		XLAL_ERROR_NULL(func, XLAL_EBADLEN);

	/* zero the product vector */
	memset(outputseq->data, 0, outputseq->length * sizeof(*outputseq->data));

	/* output = inputseries * conj(filter) */
	for(; fbins--; output++, input++, filter++) {
		output->re = input->re * filter->re + input->im * filter->im;
		output->im = input->im * filter->re - input->re * filter->im;
	}

	return outputseq;
}


/*
 * Compute the mean square for a channel from the PSD and the channel's
 * filter.  PSDs computed by LAL obey the convention that for Gaussian
 * noise, the mean square of a frequency bin is psd[k] / (2 deltaF).
 * Therefore, the mean square of a frequency bin after being multiplied by
 * the channel filter, c[k], is psd[k] |c[k]|^2 / (2 deltaF).  The mean
 * square for the channel is the sum of mean squares for the bins within
 * it, if separate frequency bins are statistically independent so that
 * there are no cross terms.  This is true for stationary noise.
 */


static REAL8 channel_mean_square(
	const REAL4FrequencySeries *psd,
	const COMPLEX8FrequencySeries *filter
)
{
	const REAL4 *pdata = psd->data->data + (int) ((filter->f0 - psd->f0) / psd->deltaF);
	const COMPLEX8 *fdata = filter->data->data;
	double sum = 0.0;
	unsigned i;

	for(i = 0; i < filter->data->length; i++, pdata++, fdata++)
		sum += *pdata * (fdata->re * fdata->re + fdata->im * fdata->im);

	return sum / (2 * psd->deltaF);
}


/******** <lalVerbatim file="FreqSeriesToTFPlaneCP"> ********/
int XLALFreqSeriesToTFPlane(
	REAL4TimeFrequencyPlane *plane,
	const COMPLEX8FrequencySeries *fseries,
	const REAL4FrequencySeries *psd,
	const REAL4FFTPlan *reverseplan,
	INT4 enable_over_whitening
)
/******** </lalVerbatim> ********/
{
	static const char func[] = "XLALFreqSeriesToTFPlane";
	COMPLEX8FrequencySeries **filter;
	COMPLEX8Sequence *fcorr;
	unsigned i;

	/* check input parameters */
	if((fmod(plane->deltaF, fseries->deltaF) != 0.0) ||
	   (fmod(plane->flow - fseries->f0, fseries->deltaF) != 0.0))
		XLAL_ERROR(func, XLAL_EINVAL);

	/* make sure the frequency series spans an appropriate band */
	if((plane->flow < fseries->f0) ||
	   (plane->flow + plane->channels * plane->deltaF > fseries->f0 + fseries->data->length * fseries->deltaF))
		XLAL_ERROR(func, XLAL_EDATA);

	/* create temporary vectors */
	filter = XLALMalloc(plane->channels * sizeof(*filter));
	fcorr = XLALCreateCOMPLEX8Sequence(fseries->data->length);
	if(!filter || !fcorr) {
		XLALFree(filter);
		XLALDestroyCOMPLEX8Sequence(fcorr);
		XLAL_ERROR(func, XLAL_EFUNC);
	}

#if 0
	/* diagnostic code to dump data for the s_{k} histogram */
	{
	unsigned k;
	FILE *f = fopen("sk.dat", "a");
	for(k = plane->flow / fseries->deltaF; k < (plane->flow + plane->channels * plane->deltaF) / fseries->deltaF; k++)
		fprintf(f, "%g\n%g\n", fseries->data->data[k].re, fseries->data->data[k].im);
	fclose(f);
	}
#endif
#if 0
	/* diagnostic code to dump data for the s_{k} s^{*}_{k'} histogram
	 * */
	{
	unsigned k, dk;
	FILE *f = fopen("sksk.dat", "a");
	for(dk = 0; dk < 100; dk++) {
		double avg_r = 0;
		double avg_i = 0;
	for(k = plane->flow / fseries->deltaF; k + dk < (plane->flow + plane->channels * plane->deltaF) / fseries->deltaF; k++) {
		double dr = fseries->data->data[k].re;
		double di = fseries->data->data[k].im;
		double dkr = fseries->data->data[k + dk].re;
		double dki = fseries->data->data[k + dk].im;
		avg_r += dr * dkr + di * dki;
		avg_i += di * dkr - dr * dki;
	}
		avg_r /= k - plane->flow / fseries->deltaF;
		avg_i /= k - plane->flow / fseries->deltaF;
		fprintf(f, "%d %g %g\n", dk, avg_r, avg_i);
	}
	fclose(f);
	}
#endif

	XLALPrintInfo("XLALFreqSeriesToTFPlane(): generating channel filters\n");
	/* generate the frequency domain filter functions */
	for(i = 0; i < plane->channels; i++) {
		filter[i] = generate_filter(fseries, plane->flow + i * plane->deltaF, plane->deltaF, enable_over_whitening ? psd : NULL, plane->two_point_spectral_correlation);
		if(!filter[i]) {
			while(--i)
				XLALDestroyCOMPLEX8FrequencySeries(filter[i]);
			XLALFree(filter);
			XLALDestroyCOMPLEX8Sequence(fcorr);
			XLAL_ERROR(func, XLAL_EFUNC);
		}
	}

	/* compute the channel overlaps */
	for(i = 0; i < plane->channels - 1; i++)
		plane->twice_channel_overlap->data[i] = 2 * filter_inner_product(filter[i], filter[i + 1], plane->two_point_spectral_correlation);

	XLALPrintInfo("XLALFreqSeriesToTFPlane(): projecting data onto time-frequency plane\n");
	/* loop over the time-frequency plane's channels */
	for(i = 0; i < plane->channels; i++) {
		/* cross correlate the input data against the channel
		 * filter by taking their product in the frequency domain
		 * and then inverse transforming to the time domain to
		 * obtain an SNR time series.  Note that
		 * XLALREAL4ReverseFFT() omits the factor of 1/N in the
		 * inverse transform. */
		apply_filter(fcorr, fseries, filter[i]);
		if(XLALREAL4ReverseFFT(plane->channel[i], fcorr, reverseplan)) {
			for(i = 0; i < plane->channels; i++)
				XLALDestroyCOMPLEX8FrequencySeries(filter[i]);
			XLALFree(filter);
			XLALDestroyCOMPLEX8Sequence(fcorr);
			XLAL_ERROR(func, XLAL_EFUNC);
		}

		/* Store the expected root mean square for this channel */
		plane->channel_rms->data[i] = sqrt(channel_mean_square(psd, filter[i]));
	}

	/* clean up */
	for(i = 0; i < plane->channels; i++)
		XLALDestroyCOMPLEX8FrequencySeries(filter[i]);
	XLALFree(filter);
	XLALDestroyCOMPLEX8Sequence(fcorr);

	/* set the name and epoch of the TF plane */
	strncpy(plane->name, fseries->name, LALNameLength);
	plane->epoch = fseries->epoch;

	/* success */
	return 0;
}
