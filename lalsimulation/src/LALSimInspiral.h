/*
 * Copyright (C) 2008 J. Creighton, S. Fairhurst, B. Krishnan, L. Santamaria, E. Ochsner, C. Pankow, 2104 A. Klein
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with with program; see the file COPYING. If not, write to the
 *  Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *  MA  02111-1307  USA
 */

#ifndef _LALSIMINSPIRAL_H
#define _LALSIMINSPIRAL_H

#include <lal/LALDatatypes.h>
#include <lal/LALSimInspiralWaveformFlags.h>
#include <lal/LALSimInspiralTestGRParams.h>
#include <lal/LALSimInspiralSphHarmSeries.h>
#include <lal/TimeSeries.h>
#include <gsl/gsl_matrix.h>

#if defined(__cplusplus)
extern "C" {
#elif 0
} /* so that editors will match preceding brace */
#endif

/**
 * \defgroup LALSimInspiral_h Header LALSimInspiral.h
 * \ingroup lalsimulation_general
 */

#define LAL_PN_MODE_L_MAX 3
/* (2x) Highest available PN order - UPDATE IF NEW ORDERS ADDED!!*/
#define LAL_MAX_PN_ORDER 8

/**
 * Macro procedure for aborting if non-default LALSimInspiralWaveformFlags
 * struct was provided, but that approximant does not use the struct
 * and only has a single default use case.
 *
 * The ChooseWaveform functions will fail in such a case, so the user does not
 * think they are including features that are unavailable.
 *
 * All of the macros below will destroy the LALSimInspiralWaveformFlags struct,
 * print a specific warning and raise a general XLAL_ERROR for invalid argument.
 */
#define ABORT_NONDEFAULT_WAVEFORM_FLAGS(waveFlags)\
do {\
XLALSimInspiralDestroyWaveformFlags(waveFlags);\
XLALPrintError("XLAL Error - %s: Non-default LALSimInspiralWaveformFlags given, but this approximant does not support this case.\n", __func__);\
XLAL_ERROR(XLAL_EINVAL);\
} while (0)

/**
 * Same as above macro, but returns a null pointer rather than XLAL_FAILURE int
 */
#define ABORT_NONDEFAULT_WAVEFORM_FLAGS_NULL(waveFlags)\
do {\
XLALSimInspiralDestroyWaveformFlags(waveFlags);\
XLALPrintError("XLAL Error - %s: Non-default LALSimInspiralWaveformFlags given, but this approximant does not support this case.\n", __func__);\
XLAL_ERROR_NULL(XLAL_EINVAL);\
} while (0)

/**
 * Macro procedure for aborting if non-zero spins
 * given to a non-spinning approximant
 */
#define ABORT_NONZERO_SPINS(waveFlags)\
do {\
XLALSimInspiralDestroyWaveformFlags(waveFlags);\
XLALPrintError("XLAL Error - %s: Non-zero spins were given, but this is a non-spinning approximant.\n", __func__);\
XLAL_ERROR(XLAL_EINVAL);\
} while (0)

/**
 * Macro procedure for aborting if non-zero transverse spin
 * components given to a non-precessing approximant
 */
#define ABORT_NONZERO_TRANSVERSE_SPINS(waveFlags)\
do {\
XLALSimInspiralDestroyWaveformFlags(waveFlags);\
XLALPrintError("XLAL Error - %s: Non-zero transverse spins were given, but this is a non-precessing approximant.\n", __func__);\
XLAL_ERROR(XLAL_EINVAL);\
} while (0)

/**
 * Macro procedure for aborting if non-zero tidal parameters
 * given to an approximant with no tidal corrections
 */
#define ABORT_NONZERO_TIDES(waveFlags)\
do {\
XLALSimInspiralDestroyWaveformFlags(waveFlags);\
XLALPrintError("XLAL Error - %s: Non-zero tidal parameters were given, but this is approximant doe not have tidal corrections.\n", __func__);\
XLAL_ERROR(XLAL_EINVAL);\
} while (0)

/**
 * Macro procedure for aborting if non-default value of
 * LALSimInspiralSpinOrder is given for an approximant
 * which does not use that flag
 */
#define ABORT_NONDEFAULT_SPIN_ORDER(waveFlags)\
do {\
XLALSimInspiralDestroyWaveformFlags(waveFlags);\
XLALPrintError("XLAL Error - %s: Non-default LALSimInspiralSpinOrder provided, but this approximant does not use that flag.\n", __func__);\
XLAL_ERROR(XLAL_EINVAL);\
} while (0)

/**
 * Macro procedure for aborting if non-default value of
 * LALSimInspiralTidalOrder is given for an approximant
 * which does not use that flag
 */
#define ABORT_NONDEFAULT_TIDAL_ORDER(waveFlags)\
do {\
XLALSimInspiralDestroyWaveformFlags(waveFlags);\
XLALPrintError("XLAL Error - %s: Non-default LALSimInspiralTidalOrder provided, but this approximant does not use that flag.\n", __func__);\
XLAL_ERROR(XLAL_EINVAL);\
} while (0)

/**
 * Macro procedure for aborting if non-default value of
 * LALSimInspiralFrameAxis is given for an approximant
 * which does not use that flag
 */
#define ABORT_NONDEFAULT_FRAME_AXIS(waveFlags)\
do {\
XLALSimInspiralDestroyWaveformFlags(waveFlags);\
XLALPrintError("XLAL Error - %s: Non-default LALSimInspiralFrameAxis provided, but this approximant does not use that flag.\n", __func__);\
XLAL_ERROR(XLAL_EINVAL);\
} while (0)

/**
 * Same as above macro, but returns a null pointer rather than XLAL_FAILURE int
 */
#define ABORT_NONDEFAULT_FRAME_AXIS_NULL(waveFlags)\
do {\
XLALSimInspiralDestroyWaveformFlags(waveFlags);\
XLALPrintError("XLAL Error - %s: Non-default LALSimInspiralFrameAxis provided, but this approximant does not use that flag.\n", __func__);\
XLAL_ERROR_NULL(XLAL_EINVAL);\
} while (0)

/**
 * Macro procedure for aborting if non-default value of
 * LALSimInspiralModesChoice is given for an approximant
 * which does not use that flag
 */
#define ABORT_NONDEFAULT_MODES_CHOICE(waveFlags)\
do {\
XLALSimInspiralDestroyWaveformFlags(waveFlags);\
XLALPrintError("XLAL Error - %s: Non-default LALSimInspiralModesChoice provided, but this approximant does not use that flag.\n", __func__);\
XLAL_ERROR(XLAL_EINVAL);\
} while (0)

/**
 * Same as above macro, but returns a null pointer rather than XLAL_FAILURE int
 */
#define ABORT_NONDEFAULT_MODES_CHOICE_NULL(waveFlags)\
do {\
XLALSimInspiralDestroyWaveformFlags(waveFlags);\
XLALPrintError("XLAL Error - %s: Non-default LALSimInspiralModesChoice provided, but this approximant does not use that flag.\n", __func__);\
XLAL_ERROR_NULL(XLAL_EINVAL);\
} while (0)

/* Internal utility function to check all spin components are zero
 returns 1 if all spins zero, otherwise returns 0 */
int checkSpinsZero(REAL8 s1x, REAL8 s1y, REAL8 s1z,
                          REAL8 s2x, REAL8 s2y, REAL8 s2z);

/* Internal utility function to check transverse spins are zero
 returns 1 if x and y components of spins are zero, otherwise returns 0 */
int checkTransverseSpinsZero(REAL8 s1x, REAL8 s1y, REAL8 s2x, REAL8 s2y);

/* Internal utility function to check tidal parameters are zero
 returns 1 if both tidal parameters zero, otherwise returns 0 */
int checkTidesZero(REAL8 lambda1, REAL8 lambda2);

/**
 * Enum that specifies the PN approximant to be used in computing the waveform.
 */
typedef enum {
   TaylorT1, 		/**< Time domain Taylor approximant in which the energy and flux are both kept
                         * as Taylor expansions and a first order ordinary differential equation is solved
                         * or the GW phase as a function of \f$t\f$; Outputs a time-domain wave.
                         */
   TaylorT2,		/**< Time domain Taylor approximant in which the phase evolution \f$\varphi(t)\f$ is
                         * obtained by iteratively solving post-Newtonian expansions \f$\varphi(v)\f$ and \f$t(v)\f$;
                         * Outputs a time-domain wave.
                         */
   TaylorT3,		/**< Time domain Taylor approximant in which phase is explicitly given as a function
                         * of time; outputs a time-domain wave.
                         */
   TaylorF1,		/**< The stationary phase approximation that correctly represents, in the Fourier domain,
                         * the waveform given by \c TaylorT1 approximant (see \cite dis2000 for details);
                         * Outputs a frequency-domain wave. */
   TaylorF2,		/**< The standard stationary phase approximation; Outputs a frequency-domain wave. */
   TaylorR2F4,		/**< A frequency domain model closely related to TaylorT4 */
   TaylorF2RedSpin,		/**< TaylorF2 waveforms for non-precessing spins, defined in terms of a single (reduced-spin) parameter [Ajith_2011ec]*/
   TaylorF2RedSpinTidal,	/**< TaylorF2 waveforms for non-precessing spins, defined in terms of a single (reduced-spin) parameter [Ajith_2011ec] plus tidal terms (http://arxiv.org/abs/1101.1673) */
   PadeT1,		/**< Time-domain P-approximant; Outputs a time-domain wave. */
   PadeF1,		/**< Frequency-domain P-approximant (not yet implemented). */
   EOB,			/**< Effective one-body waveform; Outputs a time-domain wave. */
   BCV,			/**< Detection template family of Buonanno, Chen and Vallisneri \cite BCV03; Outputs a frequency-domain wave. */
   BCVSpin,		/**< Detection template family of Buonanno, Chen and Vallisneri including  spin effects \cite BCV03b; Outputs a frequency-domain wave. */
   SpinTaylorT1,	/**< Spinning case T1 models */
   SpinTaylorT2,	/**< Spinning case T2 models */
   SpinTaylorT3,	/**< Spinning case T3 models */
   SpinTaylorT4,	/**< Spinning case T4 models (lalsimulation's equivalent of SpinTaylorFrameless) */
   SpinTaylorT5,       /**< Spinning case T5. Ref. Sec III of P. Ajith, Phys Rev D (2011)  */
   SpinTaylorF2,	/**< Spinning case F2 models (single spin only) */
   SpinTaylorFrameless,	/**< Spinning case PN models (replace SpinTaylor by removing the coordinate singularity) */
   SpinTaylor,		/**< Spinning case PN models (should replace SpinTaylorT3 in the future) */
   PhenSpinTaylor,      /**< Inspiral part of the PhenSpinTaylorRD. */
   PhenSpinTaylorRD,	/**< Phenomenological waveforms, interpolating between a T4 spin-inspiral and the ringdown. */
   SpinQuadTaylor,	/**< Spinning case PN models with quadrupole-monopole and self-spin interaction. */
   FindChirpSP,		/**< The stationary phase templates implemented by FindChirpSPTemplate in the findchirp package (equivalent to TaylorF2 at twoPN order). */
   FindChirpPTF,	/**< UNDOCUMENTED */
   GeneratePPN,		/**< The time domain templates generated by LALGeneratePPNInspiral() in the inject package (equivalent to TaylorT3 at twoPN order). */
   BCVC,		/**< UNDOCUMENTED */
   FrameFile,		/**< The waveform contains arbitrary data read from a frame file. */
   AmpCorPPN,		/**< UNDOCUMENTED */
   NumRel,		/**< UNDOCUMENTED */
   NumRelNinja2,	/**< The waveform contains REAL8 data generated by lalapps_fr_ninja from a file in the format described in arXiv:0709.0093v3 */
   Eccentricity,	/**< UNDOCUMENTED */
   EOBNR,		/**< UNDOCUMENTED */
   EOBNRv2,		/**< UNDOCUMENTED */
   EOBNRv2HM,		/**< UNDOCUMENTED */
   SEOBNRv1,		/**< Spin-aligned EOBNR model */
   SEOBNRv2,		/**< Spin-aligned EOBNR model v2 */
   SEOBNRv3,		/**< Spin precessing EOBNR model v3 */
   SEOBNRv1_ROM_SingleSpin, /**< Single-spin frequency domain reduced order model of spin-aligned EOBNR model SEOBNRv1 See [Purrer:2014fza] */
   SEOBNRv1_ROM_DoubleSpin, /**< Double-spin frequency domain reduced order model of spin-aligned EOBNR model SEOBNRv1 See [Purrer:2014fza] */
   SEOBNRv2_ROM_SingleSpin, /**< Single-spin frequency domain reduced order model of spin-aligned EOBNR model SEOBNRv2 */
   SEOBNRv2_ROM_DoubleSpin, /**< Double-spin frequency domain reduced order model of spin-aligned EOBNR model SEOBNRv2 */
   IMRPhenomA,		/**< Time domain (non-spinning) inspiral-merger-ringdown waveforms generated from the inverse FFT of IMRPhenomFA  */
   IMRPhenomB,		/**< Time domain (non-precessing spins) inspiral-merger-ringdown waveforms generated from the inverse FFT of IMRPhenomFB */
   IMRPhenomFA,		/**< Frequency domain (non-spinning) inspiral-merger-ringdown templates of Ajith et al [Ajith_2007kx] with phenomenological coefficients defined in the Table I of [Ajith_2007xh]*/
   IMRPhenomFB,		/**< Frequency domain (non-precessing spins) inspiral-merger-ringdown templates of Ajith et al [Ajith_2009bn] */
   IMRPhenomC,		/**< Frequency domain (non-precessing spins) inspiral-merger-ringdown templates of Santamaria et al [Santamaria:2010yb] with phenomenological coefficients defined in the Table II of [Santamaria:2010yb]*/
   IMRPhenomP,		/**< Frequency domain (generic spins) inspiral-merger-ringdown templates of Hannam et al., arXiv:1308.3271 [gr-qc] */
   IMRPhenomFC,		/**< Frequency domain (non-precessing spins) inspiral-merger-ringdown templates of Santamaria et al [Santamaria:2010yb] with phenomenological coefficients defined in the Table II of [Santamaria:2010yb]*/
   TaylorEt,		/**< UNDOCUMENTED */
   TaylorT4,		/**< UNDOCUMENTED */
   TaylorN,		/**< UNDOCUMENTED */
   SpinTaylorT4Fourier, /**< Frequency domain (generic spins) inspiral only waveforms based on TaylorT4, arXiv: 1408.5158 */
   SpinTaylorT2Fourier, /**< Frequency domain (generic spins) inspiral only waveforms based on TaylorT2, arXiv: 1408.5158 */
   SpinDominatedWf,     /**< Time domain, inspiral only, 1 spin, precessing waveform, Tapai et al, arXiv: 1209.1722 */
   NumApproximants	/**< Number of elements in enum, useful for checking bounds */
 } Approximant;

/** Enum of various frequency functions */
typedef enum {
    fSchwarzISCO, /**< Schwarzschild ISCO */
    fIMRPhenomAFinal, /**< Final frequency of IMRPhenomA */
    fIMRPhenomBFinal, /**< Final of IMRPhenomB */
    fIMRPhenomCFinal, /**< Final of IMRPhenomC */
    fEOBNRv2RD, /**< Ringdown frequency of EOBNRv2 */
    fEOBNRv2HMRD, /**< Ringdown frequency of highest harmonic in EOBNRv2HM */
    fSEOBNRv1Peak, /**< Frequency of the peak amplitude in SEOBNRv1 */
    fSEOBNRv1RD, /**< Dominant ringdown frequency in SEOBNRv1 */
    fSEOBNRv2Peak, /**< Frequency of the peak amplitude in SEOBNRv2 */
    fSEOBNRv2RD, /**< Dominant ringdown frequency in SEOBNRv2 */
    NumFreqFunctions /**< Number of elements in the enum */
 } FrequencyFunction;

/** Enum of possible values to use for post-Newtonian order. */
typedef enum {
  LAL_PNORDER_NEWTONIAN,	/**< Newtonain (leading) order */
  LAL_PNORDER_HALF,		/**< 0.5PN <==> O(v) */
  LAL_PNORDER_ONE,		/**< 1PN <==> O(v^2) */
  LAL_PNORDER_ONE_POINT_FIVE,	/**< 1.5PN <==> O(v^3) */
  LAL_PNORDER_TWO,		/**< 2PN <==> O(v^4) */
  LAL_PNORDER_TWO_POINT_FIVE,	/**< 2.5PN <==> O(v^5) */
  LAL_PNORDER_THREE,		/**< 3PN <==> O(v^6) */
  LAL_PNORDER_THREE_POINT_FIVE,	/**< 3.5PN <==> O(v^7)  */
  LAL_PNORDER_PSEUDO_FOUR,	/**< pseudo-4PN tuning coefficients included, true 4PN terms currently unknown */
  LAL_PNORDER_NUM_ORDER		/**< Number of elements in enum, useful for checking bounds */
 } LALPNOrder;

/** Enumeration to specify the tapering method to apply to the waveform */
typedef enum
{
  LAL_SIM_INSPIRAL_TAPER_NONE,		/**< No tapering */
  LAL_SIM_INSPIRAL_TAPER_START,		/**< Taper the start of the waveform */
  LAL_SIM_INSPIRAL_TAPER_END,		/**< Taper the end of the waveform */
  LAL_SIM_INSPIRAL_TAPER_STARTEND,	/**< Taper the start and the end of the waveform */
  LAL_SIM_INSPIRAL_TAPER_NUM_OPTS	/**< Number of elements in enum, useful for checking bounds */
}  LALSimInspiralApplyTaper;

/** Enumeration to specify time or frequency domain */
typedef enum {
  LAL_SIM_DOMAIN_TIME,
  LAL_SIM_DOMAIN_FREQUENCY
 } LALSimulationDomain;

/**
 * Tapers a REAL4 inspiral waveform in the time domain.
 */
int XLALSimInspiralREAL4WaveTaper(
  REAL4Vector              *signalvec,	/**< pointer to waveform vector */
  LALSimInspiralApplyTaper  bookends	/**< taper type enumerator */
  );

/**
 * Tapers a REAL8 inspiral waveform in the time domain.
 */
int XLALSimInspiralREAL8WaveTaper(
  REAL8Vector              *signalvec,	/**< pointer to waveform vector */
  LALSimInspiralApplyTaper  bookends	/**< taper type enumerator */
  );


/**
 * Gives the value of the desired frequency given some physical parameters
 *
 */
double XLALSimInspiralGetFrequency(
    REAL8 m1,                   /**< mass of companion 1 (kg) */
    REAL8 m2,                   /**< mass of companion 2 (kg) */
    const REAL8 S1x,            /**< x-component of the dimensionless spin of object 1 */
    const REAL8 S1y,            /**< y-component of the dimensionless spin of object 1 */
    const REAL8 S1z,            /**< z-component of the dimensionless spin of object 1 */
    const REAL8 S2x,            /**< x-component of the dimensionless spin of object 2 */
    const REAL8 S2y,            /**< y-component of the dimensionless spin of object 2 */
    const REAL8 S2z,            /**< z-component of the dimensionless spin of object 2 */
    FrequencyFunction freqFunc  /**< name of the function to use */
    );

/**
 * Gives the default ending frequencies of the given approximant.
 */
double XLALSimInspiralGetFinalFreq(
    REAL8 m1,                               /**< mass of companion 1 (kg) */
    REAL8 m2,                               /**< mass of companion 2 (kg) */
    REAL8 S1x,                              /**< x-component of the dimensionless spin of object 1 */
    REAL8 S1y,                              /**< y-component of the dimensionless spin of object 1 */
    REAL8 S1z,                              /**< z-component of the dimensionless spin of object 1 */
    REAL8 S2x,                              /**< x-component of the dimensionless spin of object 2 */
    REAL8 S2y,                              /**< y-component of the dimensionless spin of object 2 */
    REAL8 S2z,                              /**< z-component of the dimensionless spin of object 2 */
    Approximant approximant                 /**< post-Newtonian approximant to use for waveform production */
    );

/**
 * Compute the polarizations from all the -2 spin-weighted spherical harmonic
 * modes stored in 'hlms'. Be sure that 'hlms' is the head of the linked list!
 *
 * The computation done is:
 * \f$hp(t) - i hc(t) = \sum_l \sum_m h_lm(t) -2Y_lm(iota,psi)\f$
 *
 * iota and psi are the inclination and polarization angle of the observer
 * relative to the source of GWs.
 */
int XLALSimInspiralPolarizationsFromSphHarmTimeSeries(
    REAL8TimeSeries **hp, /**< Plus polarization time series [returned] */
    REAL8TimeSeries **hc, /**< Cross polarization time series [returned] */
    SphHarmTimeSeries *hlms, /**< Head of linked list of waveform modes */
    REAL8 iota, /**< inclination of viewer to source frame (rad) */
    REAL8 psi /**< polarization angle (rad) */
    );

/**
 * Computes h(2,2) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (79) of:
 * Lawrence E. Kidder, \"Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit\", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode22(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(2,1) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (80) of:
 * Lawrence E. Kidder, \"Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit\", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode21(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(2,0) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (81) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode20(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(3,3) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (82) of:
 * Lawrence E. Kidder, \"Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit\", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode33(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(3,2) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (83) of:
 * Lawrence E. Kidder, \"Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit\", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode32(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(3,1) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (84) of:
 * Lawrence E. Kidder, \"Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit\", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode31(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(3,0) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (85) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode30(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(4,4) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (86) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode44(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(4,3) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (87) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode43(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(4,2) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (88) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode42(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(4,1) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (89) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode41(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(4,0) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (90) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode40(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(5,5) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (91) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode55(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(5,4) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (92) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode54(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(5,3) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (93) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode53(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(5,2) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (94) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode52(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(5,1) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (95) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode51(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(5,0) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * THIS MODE IS ZERO TO THE ORDER CONSIDERED IN:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode50(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(6,6) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (96) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode66(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(6,5) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (97) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode65(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(6,4) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (98) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode64(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(6,3) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (99) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode63(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(6,2) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (100) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode62(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(6,1) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * Implements Equation (101) of:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode61(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Computes h(6,0) mode of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * THIS MODE IS ZERO TO THE ORDER CONSIDERED IN:
 * Lawrence E. Kidder, "Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALSimInspiralPNMode60(
        REAL8TimeSeries *V,   /**< post-Newtonian parameter */
        REAL8TimeSeries *Phi, /**< orbital phase */
        REAL8 v0,             /**< tail gauge parameter (default=1) */
        REAL8 m1,             /**< mass of companion 1 (kg) */
        REAL8 m2,             /**< mass of companion 2 (kg) */
        REAL8 r,              /**< distance of source (m) */
        int O                 /**< twice post-Newtonian order */
        );

/**
 * Multiplies a mode h(l,m) by a spin-2 weighted spherical harmonic
 * to obtain hplus - i hcross, which is added to the time series.
 *
 * Implements the sum of a single term of Eq. (11) of:
 * Lawrence E. Kidder, \"Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit\", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 *
 * If sym is non-zero, symmetrically add the m and -m terms assuming
 * that h(l,-m) = (-1)^l h(l,m)*; see Eq. (78) ibid.
 */
int XLALSimAddMode(
		REAL8TimeSeries *hplus,      /**< +-polarization waveform */
	       	REAL8TimeSeries *hcross,     /**< x-polarization waveform */
	       	COMPLEX16TimeSeries *hmode,  /**< complex mode h(l,m) */
	       	REAL8 theta,                 /**< polar angle (rad) */
	       	REAL8 phi,                   /**< azimuthal angle (rad) */
	       	int l,                       /**< mode number l */
	       	int m,                       /**< mode number m */
	       	int sym                      /**< flag to add -m mode too */
		);

/**
 * Computes h(l,m) mode timeseries of spherical harmonic decomposition of
 * the post-Newtonian inspiral waveform.
 *
 * See Eqns. (79)-(116) of:
 * Lawrence E. Kidder, \"Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit\", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 */
COMPLEX16TimeSeries *XLALCreateSimInspiralPNModeCOMPLEX16TimeSeries(
		REAL8TimeSeries *v,   /**< post-Newtonian parameter */
	       	REAL8TimeSeries *phi, /**< orbital phase */
	       	REAL8 v0,             /**< tail gauge parameter (default = 1) */
	       	REAL8 m1,             /**< mass of companion 1 (kg) */
	       	REAL8 m2,             /**< mass of companion 2 (kg) */
	       	REAL8 r,              /**< distance of source (m) */
	       	int O,                /**< twice post-Newtonain order */
	       	int l,                /**< mode number l */
	       	int m                 /**< mode number m */
		);

/**
 * Given time series for a binary's orbital dynamical variables,
 * construct the waveform polarizations h+ and hx as a sum of
 * -2 spin-weighted spherical harmonic modes, h_lm.
 * NB: Valid only for non-precessing systems!
 *
 * Implements Equation (11) of:
 * Lawrence E. Kidder, \"Using Full Information When Computing Modes of
 * Post-Newtonian Waveforms From Inspiralling Compact Binaries in Circular
 * Orbit\", Physical Review D 77, 044016 (2008), arXiv:0710.0614v1 [gr-qc].
 *
 * FIXME: change the PN variable from \f$x\f$ to \f$v = \sqrt{x}\f$
 */
int XLALSimInspiralPNPolarizationWaveformsFromModes(
		REAL8TimeSeries **hplus,  /**< +-polarization waveform [returned] */
	       	REAL8TimeSeries **hcross, /**< x-polarization waveform [returned] */
	       	REAL8TimeSeries *v,       /**< post-Newtonian parameter */
	       	REAL8TimeSeries *phi,     /**< orbital phase */
	       	REAL8 v0,                 /**< tail-term gauge choice (default = 1) */
	       	REAL8 m1,                 /**< mass of companion 1 */
	       	REAL8 m2,                 /**< mass of companion 2 */
	       	REAL8 r,                  /**< distance of source */
	       	REAL8 i,                  /**< inclination of source (rad) */
	       	int O                     /**< twice post-Newtonian order */
		);

/**
 * Given time series for a binary's orbital dynamical variables,
 * construct the waveform polarizations h+ and hx directly.
 * NB: Valid only for non-precessing binaries!
 *
 * Implements Equations (8.8) - (8.10) of:
 * Luc Blanchet, Guillaume Faye, Bala R. Iyer and Siddhartha Sinha,
 * \"The third post-Newtonian gravitational wave polarisations
 * and associated spherical harmonic modes for inspiralling compact binaries
 * in quasi-circular orbits\", Class. Quant. Grav. 25 165003 (2008);
 * arXiv:0802.1249
 *
 * Note however, that we do not include the constant \"memory\" terms
 */

int XLALSimInspiralPNPolarizationWaveforms(
        REAL8TimeSeries **hplus,  /**< +-polarization waveform [returned] */
        REAL8TimeSeries **hcross, /**< x-polarization waveform [returned] */
        REAL8TimeSeries *V,       /**< post-Newtonian (PN) parameter */
        REAL8TimeSeries *Phi,     /**< orbital phase */
        REAL8 v0,                 /**< tail-term gauge choice (default = 1) */
        REAL8 m1,                 /**< mass of companion 1 (kg) */
        REAL8 m2,                 /**< mass of companion 2 (kg) */
        REAL8 r,                  /**< distance of source (m) */
        REAL8 i,                  /**< inclination of source (rad) */
        int ampO                  /**< twice PN order of the amplitude */
        );

/**
 * Computes polarizations h+ and hx for a spinning, precessing binary
 * when provided time series of all the dynamical quantities.
 * Amplitude can be chosen between 1.5PN and Newtonian orders (inclusive).
 *
 * Based on K.G. Arun, Alesssandra Buonanno, Guillaume Faye and Evan Ochsner
 * \"Higher-order spin effects in the amplitude and phase of gravitational
 * waveforms emitted by inspiraling compact binaries: Ready-to-use
 * gravitational waveforms\", Phys Rev. D 79, 104023 (2009), arXiv:0810.5336
 *
 * HOWEVER, the formulae have been adapted to use the output of the so-called
 * \"Frameless\" convention for evolving precessing binary dynamics,
 * which is not susceptible to hitting coordinate singularities.
 *
 * FIXME: Clean up and commit Mathematica NB Showing correctness. Cite here.
 *
 * NOTE: The vectors MUST be given in the so-called radiation frame where
 * Z is the direction of propagation, X is the principal '+' axis and Y = Z x X
 */
int XLALSimInspiralPrecessingPolarizationWaveforms(
	REAL8TimeSeries **hplus,  /**< +-polarization waveform [returned] */
	REAL8TimeSeries **hcross, /**< x-polarization waveform [returned] */
	REAL8TimeSeries *V,       /**< post-Newtonian parameter */
	REAL8TimeSeries *Phi,     /**< orbital phase */
	REAL8TimeSeries *S1x,	  /**< Spin1 vector x component */
	REAL8TimeSeries *S1y,	  /**< Spin1 vector y component */
	REAL8TimeSeries *S1z,	  /**< Spin1 vector z component */
	REAL8TimeSeries *S2x,	  /**< Spin2 vector x component */
	REAL8TimeSeries *S2y,	  /**< Spin2 vector y component */
	REAL8TimeSeries *S2z,	  /**< Spin2 vector z component */
	REAL8TimeSeries *LNhatx,  /**< unit orbital ang. mom. x comp. */
	REAL8TimeSeries *LNhaty,  /**< unit orbital ang. mom. y comp. */
	REAL8TimeSeries *LNhatz,  /**< unit orbital ang. mom. z comp. */
	REAL8TimeSeries *E1x,	  /**< orbital plane basis vector x comp. */
	REAL8TimeSeries *E1y,	  /**< orbital plane basis vector y comp. */
	REAL8TimeSeries *E1z,	  /**< orbital plane basis vector z comp. */
	REAL8 m1,                 /**< mass of companion 1 (kg) */
	REAL8 m2,                 /**< mass of companion 2 (kg) */
	REAL8 r,                  /**< distance of source (m) */
	REAL8 v0,                 /**< tail-term gauge choice (default = 1) */
	INT4 ampO	 	  /**< twice amp. post-Newtonian order */
	);




/**
 * Computes polarizations h+ and hx for a spinning, precessing binary
 * when provided a single value of all the dynamical quantities.
 * Amplitude can be chosen between 1.5PN and Newtonian orders (inclusive).
 *
 * Based on K.G. Arun, Alesssandra Buonanno, Guillaume Faye and Evan Ochsner
 * \"Higher-order spin effects in the amplitude and phase of gravitational
 * waveforms emitted by inspiraling compact binaries: Ready-to-use
 * gravitational waveforms\", Phys Rev. D 79, 104023 (2009), arXiv:0810.5336
 *
 * HOWEVER, the formulae have been adapted to use the output of the so-called
 * \"Frameless\" convention for evolving precessing binary dynamics,
 * which is not susceptible to hitting coordinate singularities.
 *
 * This has been written to reproduce XLALSimInspiralPrecessingPolarizationWaveforms.
 * If hplus and hcross are the output of XLALSimInspiralPrecessingPolarizationWaveforms,
 * and hp(n) and hc(n) the output of this function for a given harmonic number, then
 *
 * hplus = sum_{n=0}^5 hp(n)*exp(-i*n*Phi) + c.c.
 * hcross = sum_{n=0}^5 hc(n)*exp(-i*n*Phi) + c.c.
 *
 * NOTE: The vectors MUST be given in the so-called radiation frame where
 * Z is the direction of propagation, X is the principal '+' axis and Y = Z x X
 * For different convention (Z is the direction of initial total angular
 * momentum, useful for GRB and comparison to NR, see XLALSimSpinInspiralGenerator())
 */
int XLALSimInspiralPrecessingPolarizationWaveformHarmonic(
        COMPLEX16 *hplus,  /**< +-polarization waveform [returned] */
        COMPLEX16 *hcross, /**< x-polarization waveform [returned] */
        REAL8 v,       /**< post-Newtonian parameter */
        REAL8 s1x,     /**< Spin1 vector x component */
        REAL8 s1y,     /**< Spin1 vector y component */
        REAL8 s1z,     /**< Spin1 vector z component */
        REAL8 s2x,     /**< Spin2 vector x component */
        REAL8 s2y,     /**< Spin2 vector y component */
        REAL8 s2z,     /**< Spin2 vector z component */
        REAL8 lnhx,  /**< unit orbital ang. mom. x comp. */
        REAL8 lnhy,  /**< unit orbital ang. mom. y comp. */
        REAL8 lnhz,  /**< unit orbital ang. mom. z comp. */
        REAL8 e1x,     /**< orbital plane basis vector x comp. */
        REAL8 e1y,     /**< orbital plane basis vector y comp. */
        REAL8 e1z,     /**< orbital plane basis vector z comp. */
        REAL8 dm,                 /**< dimensionless mass difference (m1 - m2)/(m1 + m2) > 0 */
        REAL8 eta,                /**< symmetric mass ratio m1*m2/(m1 + m2)^2 */
        REAL8 v0,                 /**< tail-term gauge choice (default = 1) */
        INT4 n,                   /**< harmonic number */
        INT4 ampO                 /**< twice amp. post-Newtonian order */
        );






/**
 * Compute the physical template family "Q" vectors for a spinning, precessing
 * binary when provided time series of all the dynamical quantities.
 * These vectors always supplied to dominant order.
 *
 * Based on Pan, Buonanno, Chan and Vallisneri PRD69 104017, (see also theses
 * of Diego Fazi and Ian Harry)
 *
 * NOTE: The vectors MUST be given in the so-called radiation frame where
 * Z is the direction of propagation, X is the principal '+' axis and Y = Z x X
 */


int XLALSimInspiralPrecessingPTFQWaveforms(
	REAL8TimeSeries **Q1,     /**< PTF-Q1 waveform [returned] */
	REAL8TimeSeries **Q2,     /**< PTF-Q2 waveform [returned] */
	REAL8TimeSeries **Q3,     /**< PTF-Q2 waveform [returned] */
	REAL8TimeSeries **Q4,     /**< PTF-Q2 waveform [returned] */
	REAL8TimeSeries **Q5,     /**< PTF-Q2 waveform [returned] */
	REAL8TimeSeries *V,       /**< post-Newtonian parameter */
	REAL8TimeSeries *Phi,     /**< orbital phase */
	REAL8TimeSeries *S1x,     /**< Spin1 vector x component */
	REAL8TimeSeries *S1y,     /**< Spin1 vector y component */
	REAL8TimeSeries *S1z,     /**< Spin1 vector z component */
	REAL8TimeSeries *S2x,     /**< Spin2 vector x component */
	REAL8TimeSeries *S2y,     /**< Spin2 vector y component */
	REAL8TimeSeries *S2z,     /**< Spin2 vector z component */
	REAL8TimeSeries *LNhatx,  /**< unit orbital ang. mom. x comp. */
	REAL8TimeSeries *LNhaty,  /**< unit orbital ang. mom. y comp. */
	REAL8TimeSeries *LNhatz,  /**< unit orbital ang. mom. z comp. */
	REAL8TimeSeries *E1x,     /**< orbital plane basis vector x comp. */
	REAL8TimeSeries *E1y,     /**< orbital plane basis vector y comp. */
	REAL8TimeSeries *E1z,     /**< orbital plane basis vector z comp. */
	REAL8 m1,                 /**< mass of companion 1 (kg) */
	REAL8 m2,                 /**< mass of companion 2 (kg) */
	REAL8 r                  /**< distance of source (m) */
	);

/**
 * Compute the length of an inspiral waveform assuming the Taylor dEnergy and Flux equations
 */
REAL8
XLALSimInspiralTaylorLength(
    REAL8 deltaT,   /**< sampling interval */
    REAL8 m1,       /**< mass of companion 1 */
    REAL8 m2,       /**< mass of companion 2 */
    REAL8 f_min,    /**< start frequency */
    int O           /**< twice post-Newtonian order */
    );

/* Waveform switching functions */

/**
 * Checks whether the given approximant is implemented in lalsimulation's XLALSimInspiralChooseTDWaveform().
 *
 * returns 1 if the approximant is implemented, 0 otherwise.
 */
int XLALSimInspiralImplementedTDApproximants(
    Approximant approximant /**< post-Newtonian approximant for use in waveform production */
    );

/**
 * Checks whether the given approximant is implemented in lalsimulation's XLALSimInspiralChooseFDWaveform().
 *
 * returns 1 if the approximant is implemented, 0 otherwise.
 */
int XLALSimInspiralImplementedFDApproximants(
    Approximant approximant /**< post-Newtonian approximant for use in waveform production */
    );

/**
 * XLAL function to determine approximant from a string.  The string need not
 * match exactly, only contain a member of the Approximant enum.
 */
int XLALGetApproximantFromString(const CHAR *inString);

/**
 * XLAL function to determine string from approximant enum.
 * This function needs to be updated when new approximants are added.
 */
char* XLALGetStringFromApproximant(Approximant approximant);

/**
 * XLAL function to determine PN order from a string.  The string need not
 * match exactly, only contain a member of the LALPNOrder enum.
 */
int XLALGetOrderFromString(const CHAR *inString);

/**
 * XLAL function to determine tapering flag from a string.  The string must
 * match exactly with a member of the LALSimInspiralApplyTaper enum.
 */
int XLALGetTaperFromString(const CHAR *inString);

/** XLAL function to determine axis choice flag from a string */
int XLALGetFrameAxisFromString(const CHAR *inString);

/**
 * XLAL function to determine mode flag from a string.
 * Returns one of enum values as name matches case of enum.
 */
int XLALGetHigherModesFromString(const CHAR *inString);

/**
 * DEPRECATED: USE XLALSimInspiralChooseTDWaveform() INSTEAD
 *
 * Chooses between different approximants when requesting a waveform to be generated
 * For spinning waveforms, all known spin effects up to given PN order are included
 *
 * The parameters passed must be in SI units.
 */
int XLALSimInspiralChooseWaveform(
    REAL8TimeSeries **hplus,                    /**< +-polarization waveform */
    REAL8TimeSeries **hcross,                   /**< x-polarization waveform */
    REAL8 phiRef,                               /**< reference orbital phase (rad) */
    REAL8 deltaT,                               /**< sampling interval (s) */
    REAL8 m1,                                   /**< mass of companion 1 (kg) */
    REAL8 m2,                                   /**< mass of companion 2 (kg) */
    REAL8 S1x,                                  /**< x-component of the dimensionless spin of object 1 */
    REAL8 S1y,                                  /**< y-component of the dimensionless spin of object 1 */
    REAL8 S1z,                                  /**< z-component of the dimensionless spin of object 1 */
    REAL8 S2x,                                  /**< x-component of the dimensionless spin of object 2 */
    REAL8 S2y,                                  /**< y-component of the dimensionless spin of object 2 */
    REAL8 S2z,                                  /**< z-component of the dimensionless spin of object 2 */
    REAL8 f_min,                                /**< starting GW frequency (Hz) */
    REAL8 f_ref,                                /**< reference GW frequency (Hz) */
    REAL8 r,                                    /**< distance of source (m) */
    REAL8 i,                                    /**< inclination of source (rad) */
    REAL8 lambda1,                              /**< (tidal deformability of mass 1) / m1^5 (dimensionless) */
    REAL8 lambda2,                              /**< (tidal deformability of mass 2) / m2^5 (dimensionless) */
    LALSimInspiralWaveformFlags *waveFlags,     /**< Set of flags to control special behavior of some waveform families. Pass in NULL (or None in python) for default flags */
    LALSimInspiralTestGRParam *nonGRparams, 	/**< Linked list of non-GR parameters. Pass in NULL (or None in python) for standard GR waveforms */
    int amplitudeO,                             /**< twice post-Newtonian amplitude order */
    int phaseO,                                 /**< twice post-Newtonian order */
    Approximant approximant                     /**< post-Newtonian approximant to use for waveform production */
    );

/**
 * Chooses between different approximants when requesting a waveform to be generated
 * For spinning waveforms, all known spin effects up to given PN order are included
 * Returns the waveform in the time domain.
 *
 * The parameters passed must be in SI units.
 */
int XLALSimInspiralChooseTDWaveform(
    REAL8TimeSeries **hplus,    /**< +-polarization waveform */
    REAL8TimeSeries **hcross,   /**< x-polarization waveform */
    REAL8 phiRef,               /**< reference orbital phase (rad) */
    REAL8 deltaT,               /**< sampling interval (s) */
    REAL8 m1,                   /**< mass of companion 1 (kg) */
    REAL8 m2,                   /**< mass of companion 2 (kg) */
    REAL8 s1x,                  /**< x-component of the dimensionless spin of object 1 */
    REAL8 s1y,                  /**< y-component of the dimensionless spin of object 1 */
    REAL8 s1z,                  /**< z-component of the dimensionless spin of object 1 */
    REAL8 s2x,                  /**< x-component of the dimensionless spin of object 2 */
    REAL8 s2y,                  /**< y-component of the dimensionless spin of object 2 */
    REAL8 s2z,                  /**< z-component of the dimensionless spin of object 2 */
    REAL8 f_min,                /**< starting GW frequency (Hz) */
    REAL8 f_ref,                /**< reference GW frequency (Hz) */
    REAL8 r,                    /**< distance of source (m) */
    REAL8 i,                    /**< inclination of source (rad) */
    REAL8 lambda1,              /**< (tidal deformability of mass 1) / m1^5 (dimensionless) */
    REAL8 lambda2,              /**< (tidal deformability of mass 2) / m2^5 (dimensionless) */
    LALSimInspiralWaveformFlags *waveFlags, /**< Set of flags to control special behavior of some waveform families. Pass in NULL (or None in python) for default flags */
    LALSimInspiralTestGRParam *nonGRparams, /**< Linked list of non-GR parameters. Pass in NULL (or None in python) for standard GR waveforms */
    int amplitudeO,             /**< twice post-Newtonian amplitude order */
    int phaseO,                 /**< twice post-Newtonian phase order */
    Approximant approximant     /**< post-Newtonian approximant to use for waveform production */
    );

/**
 * Chooses between different approximants when requesting a waveform to be generated
 * For spinning waveforms, all known spin effects up to given PN order are included
 * Returns the waveform in the frequency domain.
 *
 * The parameters passed must be in SI units.
 */
int XLALSimInspiralChooseFDWaveform(
    COMPLEX16FrequencySeries **hptilde,         /**< FD plus polarization */
    COMPLEX16FrequencySeries **hctilde,         /**< FD cross polarization */
    REAL8 phiRef,                               /**< reference orbital phase (rad) */
    REAL8 deltaF,                               /**< sampling interval (Hz) */
    REAL8 m1,                                   /**< mass of companion 1 (kg) */
    REAL8 m2,                                   /**< mass of companion 2 (kg) */
    REAL8 S1x,                                  /**< x-component of the dimensionless spin of object 1 */
    REAL8 S1y,                                  /**< y-component of the dimensionless spin of object 1 */
    REAL8 S1z,                                  /**< z-component of the dimensionless spin of object 1 */
    REAL8 S2x,                                  /**< x-component of the dimensionless spin of object 2 */
    REAL8 S2y,                                  /**< y-component of the dimensionless spin of object 2 */
    REAL8 S2z,                                  /**< z-component of the dimensionless spin of object 2 */
    REAL8 f_min,                                /**< starting GW frequency (Hz) */
    REAL8 f_max,                                /**< ending GW frequency (Hz) */
    REAL8 f_ref,                                /**< Reference GW frequency (Hz) */
    REAL8 r,                                    /**< distance of source (m) */
    REAL8 i,                                    /**< inclination of source (rad) */
    REAL8 lambda1,                              /**< (tidal deformability of mass 1) / m1^5 (dimensionless) */
    REAL8 lambda2,                              /**< (tidal deformability of mass 2) / m2^5 (dimensionless) */
    LALSimInspiralWaveformFlags *waveFlags,     /**< Set of flags to control special behavior of some waveform families. Pass in NULL (or None in python) for default flags */
    LALSimInspiralTestGRParam *nonGRparams, 	/**< Linked list of non-GR parameters. Pass in NULL (or None in python) for standard GR waveforms */
    int amplitudeO,                             /**< twice post-Newtonian amplitude order */
    int phaseO,                                 /**< twice post-Newtonian order */
    Approximant approximant                     /**< post-Newtonian approximant to use for waveform production */
    );

/**
 * @brief Generates an time domain inspiral waveform using the specified approximant; the
 * resulting waveform is appropriately conditioned and suitable for injection into data.
 *
 * For spinning waveforms, all known spin effects up to given PN order are included
 *
 * This routine can generate FD approximants and transform them into the time domain.
 * Waveforms are generated beginning at a slightly lower starting frequency and tapers
 * are put in this early region so that the waveform smoothly turns on.  Artifacts at
 * the very end of the waveform are also tapered.  The resulting waveform is high-pass
 * filtered at frequency f_min so that it should have little content at lower frequencies.
 *
 * This routine has one additional parameter relative to XLALSimInspiralChooseTDWaveform.
 * The additional parameter is the redshift, z, of the waveform.  This should be set to
 * zero for sources in the nearby universe (that are nearly at rest relative to the
 * earth).  For sources at cosmological distances, the mass parameters m1 and m2 should
 * be interpreted as the physical (source frame) masses of the bodies and the distance
 * parameter r is the comoving (transverse) distance.  If the calling routine has already
 * applied cosmological "corrections" to m1 and m2 and regards r as a luminosity distance
 * then the redshift factor should again be set to zero.
 * 
 * @note The parameters passed must be in SI units.
 */
int XLALSimInspiralTD(
    REAL8TimeSeries **hplus,                    /**< +-polarization waveform */
    REAL8TimeSeries **hcross,                   /**< x-polarization waveform */
    REAL8 phiRef,                               /**< reference orbital phase (rad) */
    REAL8 deltaT,                               /**< sampling interval (s) */
    REAL8 m1,                                   /**< mass of companion 1 (kg) */
    REAL8 m2,                                   /**< mass of companion 2 (kg) */
    REAL8 S1x,                                  /**< x-component of the dimensionless spin of object 1 */
    REAL8 S1y,                                  /**< y-component of the dimensionless spin of object 1 */
    REAL8 S1z,                                  /**< z-component of the dimensionless spin of object 1 */
    REAL8 S2x,                                  /**< x-component of the dimensionless spin of object 2 */
    REAL8 S2y,                                  /**< y-component of the dimensionless spin of object 2 */
    REAL8 S2z,                                  /**< z-component of the dimensionless spin of object 2 */
    REAL8 f_min,                                /**< starting GW frequency (Hz) */
    REAL8 f_ref,                                /**< reference GW frequency (Hz) */
    REAL8 r,                                    /**< distance of source (m) */
    REAL8 z,                                    /**< redshift of source frame relative to observer frame */
    REAL8 i,                                    /**< inclination of source (rad) */
    REAL8 lambda1,                              /**< (tidal deformability of mass 1) / m1^5 (dimensionless) */
    REAL8 lambda2,                              /**< (tidal deformability of mass 2) / m2^5 (dimensionless) */
    LALSimInspiralWaveformFlags *waveFlags,     /**< Set of flags to control special behavior of some waveform families. Pass in NULL (or None in python) for default flags */
    LALSimInspiralTestGRParam *nonGRparams, 	/**< Linked list of non-GR parameters. Pass in NULL (or None in python) for standard GR waveforms */
    int amplitudeO,                             /**< twice post-Newtonian amplitude order */
    int phaseO,                                 /**< twice post-Newtonian order */
    Approximant approximant                     /**< post-Newtonian approximant to use for waveform production */
    );

/**
 * @brief Generates a frequency domain inspiral waveform using the specified approximant; the
 * resulting waveform is appropriately conditioned and suitable for injection into data.
 *
 * For spinning waveforms, all known spin effects up to given PN order are included.
 *
 * This routine can generate TD approximants and transform them into the frequency domain.
 * Waveforms are generated beginning at a slightly lower starting frequency and tapers
 * are put in this early region so that the waveform smoothly turns on.
 *
 * If an FD approximant is used, this routine applies tapers in the frequency domain
 * between the slightly-lower frequency and the requested f_min.  Also, the phase of the
 * waveform is adjusted to introduce a time shift.  This time shift should allow the
 * resulting waveform to be Fourier transformed into the time domain without wrapping
 * the end of the waveform to the beginning.
 *
 * This routine has a few parameters that differ from XLALSimInspiralChooseFDWaveform.
 * Rather than f_max, this routine takes deltaT, the sampling interval of the corresponding
 * time domain waveform.  The Nyquist frequency, 2/deltaT, thus determines the maximum
 * frequency for the FD waveform.  Also, this routine does not take a deltaF parameter,
 * and instead computes the necessary value of deltaF based on the duration of the
 * corresponding time domain waveform, deltaF <= 1/duration.  The total number of points
 * in the FD waveform is a power of two plus one (the Nyquist frequency).  Thus, the FD
 * waveform returned could be directly Fourier transformed to the time domain without
 * further manipulation.
 *
 * This routine has one additional parameter relative to XLALSimInspiralChooseFDWaveform.
 * The additional parameter is the redshift, z, of the waveform.  This should be set to
 * zero for sources in the nearby universe (that are nearly at rest relative to the
 * earth).  For sources at cosmological distances, the mass parameters m1 and m2 should
 * be interpreted as the physical (source frame) masses of the bodies and the distance
 * parameter r is the comoving (transverse) distance.  If the calling routine has already
 * applied cosmological "corrections" to m1 and m2 and regards r as a luminosity distance
 * then the redshift factor should again be set to zero.
 *
 * 
 * @note The parameters passed must be in SI units.
 */
int XLALSimInspiralFD(
    COMPLEX16FrequencySeries **hptilde,         /**< +-polarization waveform */
    COMPLEX16FrequencySeries **hcross,          /**< x-polarization waveform */
    REAL8 phiRef,                               /**< reference orbital phase (rad) */
    REAL8 deltaT,                               /**< sampling interval (s) */
    REAL8 m1,                                   /**< mass of companion 1 (kg) */
    REAL8 m2,                                   /**< mass of companion 2 (kg) */
    REAL8 S1x,                                  /**< x-component of the dimensionless spin of object 1 */
    REAL8 S1y,                                  /**< y-component of the dimensionless spin of object 1 */
    REAL8 S1z,                                  /**< z-component of the dimensionless spin of object 1 */
    REAL8 S2x,                                  /**< x-component of the dimensionless spin of object 2 */
    REAL8 S2y,                                  /**< y-component of the dimensionless spin of object 2 */
    REAL8 S2z,                                  /**< z-component of the dimensionless spin of object 2 */
    REAL8 f_min,                                /**< starting GW frequency (Hz) */
    REAL8 f_ref,                                /**< reference GW frequency (Hz) */
    REAL8 r,                                    /**< distance of source (m) */
    REAL8 z,                                    /**< redshift of source frame relative to observer frame */
    REAL8 i,                                    /**< inclination of source (rad) */
    REAL8 lambda1,                              /**< (tidal deformability of mass 1) / m1^5 (dimensionless) */
    REAL8 lambda2,                              /**< (tidal deformability of mass 2) / m2^5 (dimensionless) */
    LALSimInspiralWaveformFlags *waveFlags,     /**< Set of flags to control special behavior of some waveform families. Pass in NULL (or None in python) for default flags */
    LALSimInspiralTestGRParam *nonGRparams, 	/**< Linked list of non-GR parameters. Pass in NULL (or None in python) for standard GR waveforms */
    int amplitudeO,                             /**< twice post-Newtonian amplitude order */
    int phaseO,                                 /**< twice post-Newtonian order */
    Approximant approximant                     /**< post-Newtonian approximant to use for waveform production */
    );

/**
 * Interface to compute a set of -2 spin-weighted spherical harmonic modes
 * for a binary inspiral of any available amplitude and phase PN order.
 * The phasing is computed with any of the TaylorT1, T2, T3, T4 methods.
 */
SphHarmTimeSeries *XLALSimInspiralChooseTDModes(
    REAL8 phiRef,                               /**< reference orbital phase (rad) */
    REAL8 deltaT,                               /**< sampling interval (s) */
    REAL8 m1,                                   /**< mass of companion 1 (kg) */
    REAL8 m2,                                   /**< mass of companion 2 (kg) */
    REAL8 f_min,                                /**< starting GW frequency (Hz) */
    REAL8 f_ref,                                /**< reference GW frequency (Hz) */
    REAL8 r,                                    /**< distance of source (m) */
    REAL8 lambda1,                              /**< (tidal deformability of mass 1) / m1^5 (dimensionless) */
    REAL8 lambda2,                              /**< (tidal deformability of mass 2) / m2^5 (dimensionless) */
    LALSimInspiralWaveformFlags *waveFlags,     /**< Set of flags to control special behavior of some waveform families. Pass in NULL (or None in python) for default flags */
    LALSimInspiralTestGRParam *nonGRparams, 	/**< Linked list of non-GR parameters. Pass in NULL (or None in python) for standard GR waveforms */
    int amplitudeO,                             /**< twice post-Newtonian amplitude order */
    int phaseO,                                 /**< twice post-Newtonian order */
    int lmax,                                   /**< generate all modes with l <= lmax */
    Approximant approximant                     /**< post-Newtonian approximant to use for waveform production */
    );

/**
 * Interface to compute a single -2 spin-weighted spherical harmonic mode
 * for a binary inspiral of any available amplitude and phase PN order.
 * The phasing is computed with any of the TaylorT1, T2, T3, T4 methods.
 */
COMPLEX16TimeSeries *XLALSimInspiralChooseTDMode(
    REAL8 phiRef,                               /**< reference orbital phase (rad) */
    REAL8 deltaT,                               /**< sampling interval (s) */
    REAL8 m1,                                   /**< mass of companion 1 (kg) */
    REAL8 m2,                                   /**< mass of companion 2 (kg) */
    REAL8 f_min,                                /**< starting GW frequency (Hz) */
    REAL8 f_ref,                                /**< reference GW frequency (Hz) */
    REAL8 r,                                    /**< distance of source (m) */
    REAL8 lambda1,                              /**< (tidal deformability of mass 1) / m1^5 (dimensionless) */
    REAL8 lambda2,                              /**< (tidal deformability of mass 2) / m2^5 (dimensionless) */
    LALSimInspiralWaveformFlags *waveFlags,     /**< Set of flags to control special behavior of some waveform families. Pass in NULL (or None in python) for default flags */
    LALSimInspiralTestGRParam *nonGRparams, 	/**< Linked list of non-GR parameters. Pass in NULL (or None in python) for standard GR waveforms */
    int amplitudeO,                             /**< twice post-Newtonian amplitude order */
    int phaseO,                                 /**< twice post-Newtonian order */
    int l,                                      /**< l index of mode */
    int m,                                      /**< m index of mode */
    Approximant approximant                     /**< post-Newtonian approximant to use for waveform production */
    );

/* TaylorT4 functions */

/**
 * Evolves a post-Newtonian orbit using the Taylor T4 method.
 *
 * See:
 * Michael Boyle, Duncan A. Brown, Lawrence E. Kidder, Abdul H. Mroue,
 * Harald P. Pfeiﬀer, Mark A. Scheel, Gregory B. Cook, and Saul A. Teukolsky
 * \"High-accuracy comparison of numerical relativity simulations with
 * post-Newtonian expansions\"
 * <a href="http://arxiv.org/abs/0710.0158v2">arXiv:0710.0158v2</a>.
 */
int XLALSimInspiralTaylorT4PNEvolveOrbit(
		REAL8TimeSeries **v,            /**< post-Newtonian parameter [returned] */
		REAL8TimeSeries **phi,          /**< orbital phase [returned] */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< start frequency (Hz) */
		REAL8 fRef,                     /**< reference frequency (Hz) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int O                           /**< twice post-Newtonian order */
		);

/**
 * Driver routine to compute the post-Newtonian inspiral waveform.
 *
 * This routine allows the user to specify different pN orders
 * for phasing calcuation vs. amplitude calculations.
 */
int XLALSimInspiralTaylorT4PNGenerator(
		REAL8TimeSeries **hplus,        /**< +-polarization waveform */
		REAL8TimeSeries **hcross,       /**< x-polarization waveform */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 v0,                       /**< tail-term gauge choice (default = 1) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 i,                        /**< inclination of source (rad) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int amplitudeO,                 /**< twice post-Newtonian amplitude order */
		int phaseO                      /**< twice post-Newtonian phase order */
		);

/**
 * Driver routine to compute the -2 spin-weighted spherical harmonic mode
 * using TaylorT4 phasing.
 */
SphHarmTimeSeries *XLALSimInspiralTaylorT4PNModes(
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 v0,                       /**< tail-term gauge choice (default = 1) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int amplitudeO,                 /**< twice post-Newtonian amplitude order */
		int phaseO,                     /**< twice post-Newtonian phase order */
		int lmax                        /**< generate all modes with l <= lmax */
		);

/**
 * Driver routine to compute the -2 spin-weighted spherical harmonic mode
 * using TaylorT4 phasing.
 */
COMPLEX16TimeSeries *XLALSimInspiralTaylorT4PNMode(
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 v0,                       /**< tail-term gauge choice (default = 1) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int amplitudeO,                 /**< twice post-Newtonian amplitude order */
		int phaseO,                     /**< twice post-Newtonian phase order */
		int l,                          /**< l index of mode */
		int m                           /**< m index of mode */
		);

/**
 * Driver routine to compute the post-Newtonian inspiral waveform.
 *
 * This routine uses the same pN order for phasing and amplitude
 * (unless the order is -1 in which case the highest available
 * order is used for both of these -- which might not be the same).
 *
 * Constant log term in amplitude set to 1.  This is a gauge choice.
 */
int XLALSimInspiralTaylorT4PN(
		REAL8TimeSeries **hplus,        /**< +-polarization waveform */
		REAL8TimeSeries **hcross,       /**< x-polarization waveform */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 deltaT,                   /**< sampling interval (Hz) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< start frequency (Hz) */
		REAL8 fRef,                     /**< reference frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 i,                        /**< inclination of source (rad) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int O                           /**< twice post-Newtonian order */
		);

/**
 * Driver routine to compute the restricted post-Newtonian inspiral waveform.
 *
 * This routine computes the phasing to the specified order, but
 * only computes the amplitudes to the Newtonian (quadrupole) order.
 *
 * Constant log term in amplitude set to 1.  This is a gauge choice.
 */
int XLALSimInspiralTaylorT4PNRestricted(
		REAL8TimeSeries **hplus,        /**< +-polarization waveform */
		REAL8TimeSeries **hcross,       /**< x-polarization waveform */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< start frequency (Hz) */
		REAL8 fRef,                     /**< reference frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 i,                        /**< inclination of source (rad) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int O                           /**< twice post-Newtonian phase order */
		);


/* TaylorT3 functions */

/**
 * Evolves a post-Newtonian orbit using the Taylor T3 method.
 */
int XLALSimInspiralTaylorT3PNEvolveOrbit(
		REAL8TimeSeries **V,            /**< post-Newtonian parameter [returned] */
		REAL8TimeSeries **phi,          /**< orbital phase [returned] */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int O                           /**< twice post-Newtonian order */
		);

/**
 * Driver routine to compute the post-Newtonian inspiral waveform.
 *
 * This routine allows the user to specify different pN orders
 * for phasing calcuation vs. amplitude calculations.
 */
int XLALSimInspiralTaylorT3PNGenerator(
		REAL8TimeSeries **hplus,        /**< +-polarization waveform */
		REAL8TimeSeries **hcross,       /**< x-polarization waveform */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 v0,                       /**< tail-term gauge choice (default = 1) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 i,                        /**< inclination of source (rad) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int amplitudeO,                 /**< twice post-Newtonian amplitude order */
		int phaseO                      /**< twice post-Newtonian phase order */
		);

/**
 * Driver routine to compute the -2 spin-weighted spherical harmonic modes
 * using TaylorT3 phasing.
 */
SphHarmTimeSeries *XLALSimInspiralTaylorT3PNModes(
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 v0,                       /**< tail-term gauge choice (default = 1) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int amplitudeO,                 /**< twice post-Newtonian amplitude order */
		int phaseO,                     /**< twice post-Newtonian phase order */
		int lmax                        /**< generate all modes with l <= lmax */
		);

/**
 * Driver routine to compute the -2 spin-weighted spherical harmonic mode
 * using TaylorT3 phasing.
 */
COMPLEX16TimeSeries *XLALSimInspiralTaylorT3PNMode(
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 v0,                       /**< tail-term gauge choice (default = 1) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int amplitudeO,                 /**< twice post-Newtonian amplitude order */
		int phaseO,                     /**< twice post-Newtonian phase order */
		int l,                          /**< l index of mode */
		int m                           /**< m index of mode */
		);

/**
 * Driver routine to compute the post-Newtonian inspiral waveform.
 *
 * This routine uses the same pN order for phasing and amplitude
 * (unless the order is -1 in which case the highest available
 * order is used for both of these -- which might not be the same).
 *
 * Constant log term in amplitude set to 1.  This is a gauge choice.
 */
int XLALSimInspiralTaylorT3PN(
		REAL8TimeSeries **hplus,        /**< +-polarization waveform */
		REAL8TimeSeries **hcross,       /**< x-polarization waveform */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 i,                        /**< inclination of source (rad) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int O                           /**< twice post-Newtonian order */
		);

/**
 * Driver routine to compute the restricted post-Newtonian inspiral waveform.
 *
 * This routine computes the phasing to the specified order, but
 * only computes the amplitudes to the Newtonian (quadrupole) order.
 *
 * Constant log term in amplitude set to 1.  This is a gauge choice.
 */
int XLALSimInspiralTaylorT3PNRestricted(
		REAL8TimeSeries **hplus,        /**< +-polarization waveform */
		REAL8TimeSeries **hcross,       /**< x-polarization waveform */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m)*/
		REAL8 i,                        /**< inclination of source (rad) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int O                           /**< twice post-Newtonian phase order */
		);


/* TaylorT2 functions */

/**
 * Evolves a post-Newtonian orbit using the Taylor T2 method.
 */
int XLALSimInspiralTaylorT2PNEvolveOrbit(
		REAL8TimeSeries **V,            /**< post-Newtonian parameter [returned] */
		REAL8TimeSeries **phi,          /**< orbital phase [returned] */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int O                           /**< twice post-Newtonian order */
		);

/**
 * Driver routine to compute the post-Newtonian inspiral waveform.
 *
 * This routine allows the user to specify different pN orders
 * for phasing calcuation vs. amplitude calculations.
 */
int XLALSimInspiralTaylorT2PNGenerator(
		REAL8TimeSeries **hplus,        /**< +-polarization waveform */
		REAL8TimeSeries **hcross,       /**< x-polarization waveform */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 v0,                       /**< tail-term gauge choice (default = 1) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 i,                        /**< inclination of source (rad) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int amplitudeO,                 /**< twice post-Newtonian amplitude order */
		int phaseO                      /**< twice post-Newtonian phase order */
		);

/**
 * Driver routine to compute the -2 spin-weighted spherical harmonic modes
 * using TaylorT2 phasing.
 */
SphHarmTimeSeries *XLALSimInspiralTaylorT2PNModes(
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 v0,                       /**< tail-term gauge choice (default = 1) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int amplitudeO,                 /**< twice post-Newtonian amplitude order */
		int phaseO,                     /**< twice post-Newtonian phase order */
		int lmax                        /**< generate all modes with l <= lmax */
		);

/**
 * Driver routine to compute the -2 spin-weighted spherical harmonic mode
 * using TaylorT2 phasing.
 */
COMPLEX16TimeSeries *XLALSimInspiralTaylorT2PNMode(
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 v0,                       /**< tail-term gauge choice (default = 1) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int amplitudeO,                 /**< twice post-Newtonian amplitude order */
		int phaseO,                     /**< twice post-Newtonian phase order */
		int l,                          /**< l index of mode */
		int m                           /**< m index of mode */
		);

/**
 * Driver routine to compute the post-Newtonian inspiral waveform.
 *
 * This routine uses the same pN order for phasing and amplitude
 * (unless the order is -1 in which case the highest available
 * order is used for both of these -- which might not be the same).
 *
 * Constant log term in amplitude set to 1.  This is a gauge choice.
 */
int XLALSimInspiralTaylorT2PN(
		REAL8TimeSeries **hplus,        /**< +-polarization waveform */
		REAL8TimeSeries **hcross,       /**< x-polarization waveform */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz)*/
		REAL8 fRef,                     /**< reference GW frequency (Hz)*/
		REAL8 r,                        /**< distance of source (m) */
		REAL8 i,                        /**< inclination of source (rad) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int O                           /**< twice post-Newtonian order */
		);

/**
 * Driver routine to compute the restricted post-Newtonian inspiral waveform.
 *
 * This routine computes the phasing to the specified order, but
 * only computes the amplitudes to the Newtonian (quadrupole) order.
 *
 * Constant log term in amplitude set to 1.  This is a gauge choice.
 */
int XLALSimInspiralTaylorT2PNRestricted(
		REAL8TimeSeries **hplus,        /**< +-polarization waveform */
		REAL8TimeSeries **hcross,       /**< x-polarization waveform */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 i,                        /**< inclination of source (rad) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int O                           /**< twice post-Newtonian phase order */
		);

/* TaylorT1 functions */

/**
 * Evolves a post-Newtonian orbit using the Taylor T1 method.
 */
int XLALSimInspiralTaylorT1PNEvolveOrbit(
		REAL8TimeSeries **V,            /**< post-Newtonian parameter [returned] */
		REAL8TimeSeries **phi,          /**< orbital phase [returned] */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< start frequency (Hz) */
		REAL8 fRef,                     /**< reference frequency (Hz) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int O                           /**< twice post-Newtonian order */
		);

/**
 * Driver routine to compute the post-Newtonian inspiral waveform.
 *
 * This routine allows the user to specify different pN orders
 * for phasing calcuation vs. amplitude calculations.
 */
int XLALSimInspiralTaylorT1PNGenerator(
		REAL8TimeSeries **hplus,        /**< +-polarization waveform */
		REAL8TimeSeries **hcross,       /**< x-polarization waveform */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 v0,                       /**< tail-term gauge choice (default = 1) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 i,                        /**< inclination of source (rad) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int amplitudeO,                 /**< twice post-Newtonian amplitude order */
		int phaseO                      /**< twice post-Newtonian phase order */
		);

/**
 * Driver routine to compute the -2 spin-weighted spherical harmonic mode
 * using TaylorT1 phasing.
 */
SphHarmTimeSeries *XLALSimInspiralTaylorT1PNModes(
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 v0,                       /**< tail-term gauge choice (default = 1) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int amplitudeO,                 /**< twice post-Newtonian amplitude order */
		int phaseO,                     /**< twice post-Newtonian phase order */
		int lmax                        /**<  generate all modes with l <= lmax */
		);

/**
 * Driver routine to compute the -2 spin-weighted spherical harmonic mode
 * using TaylorT1 phasing.
 */
COMPLEX16TimeSeries *XLALSimInspiralTaylorT1PNMode(
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 v0,                       /**< tail-term gauge choice (default = 1) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int amplitudeO,                 /**< twice post-Newtonian amplitude order */
		int phaseO,                     /**< twice post-Newtonian phase order */
		int l,                          /**< l index of mode */
		int m                           /**< m index of mode */
		);

/**
 * Driver routine to compute the post-Newtonian inspiral waveform.
 *
 * This routine uses the same pN order for phasing and amplitude
 * (unless the order is -1 in which case the highest available
 * order is used for both of these -- which might not be the same).
 *
 * Constant log term in amplitude set to 1.  This is a gauge choice.
 */
int XLALSimInspiralTaylorT1PN(
		REAL8TimeSeries **hplus,        /**< +-polarization waveform */
		REAL8TimeSeries **hcross,       /**< x-polarization waveform */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< start frequency (Hz) */
		REAL8 fRef,                     /**< reference frequency (Hz) */
		REAL8 r,                        /**< distance of source (m) */
		REAL8 i,                        /**< inclination of source (rad) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int O                           /**< twice post-Newtonian order */
		);

/**
 * Driver routine to compute the restricted post-Newtonian inspiral waveform.
 *
 * This routine computes the phasing to the specified order, but
 * only computes the amplitudes to the Newtonian (quadrupole) order.
 *
 * Constant log term in amplitude set to 1.  This is a gauge choice.
 */
int XLALSimInspiralTaylorT1PNRestricted(
		REAL8TimeSeries **hplus,        /**< +-polarization waveform */
		REAL8TimeSeries **hcross,       /**< x-polarization waveform */
		REAL8 phiRef,                   /**< reference orbital phase (rad) */
		REAL8 deltaT,                   /**< sampling interval (s) */
		REAL8 m1,                       /**< mass of companion 1 (kg) */
		REAL8 m2,                       /**< mass of companion 2 (kg) */
		REAL8 f_min,                    /**< starting GW frequency (Hz) */
		REAL8 fRef,                     /**< reference GW frequency (Hz) */
		REAL8 r,                        /**< distance of source (m)*/
		REAL8 i,                        /**< inclination of source (rad) */
		REAL8 lambda1,                  /**< (tidal deformability of body 1)/(mass of body 1)^5 */
		REAL8 lambda2,                  /**< (tidal deformability of body 2)/(mass of body 2)^5 */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		int O                           /**< twice post-Newtonian phase order */
		);


/* TaylorEt functions */

/**
 * Evolves a post-Newtonian orbit using the Taylor T1 method.
 */
int XLALSimInspiralTaylorEtPNEvolveOrbit(
		REAL8TimeSeries **V,   /**< post-Newtonian parameter [returned] */
	       	REAL8TimeSeries **phi, /**< orbital phase [returned] */
	       	REAL8 phic,            /**< orbital phase at end */
	       	REAL8 deltaT,          /**< sampling interval */
		REAL8 m1,              /**< mass of companion 1 */
		REAL8 m2,              /**< mass of companion 2 */
		REAL8 f_min,           /**< start frequency */
		int O                  /**< twice post-Newtonian order */
		);

/**
 * Driver routine to compute the post-Newtonian inspiral waveform.
 *
 * This routine allows the user to specify different pN orders
 * for phasing calcuation vs. amplitude calculations.
 */
int XLALSimInspiralTaylorEtPNGenerator(
		REAL8TimeSeries **hplus,  /**< +-polarization waveform */
	       	REAL8TimeSeries **hcross, /**< x-polarization waveform */
	       	REAL8 phic,               /**< orbital phase at end */
	       	REAL8 x0,                 /**< tail-term gauge choice (if you don't know, just set it to zero) */
	       	REAL8 deltaT,             /**< sampling interval */
	       	REAL8 m1,                 /**< mass of companion 1 */
	       	REAL8 m2,                 /**< mass of companion 2 */
	       	REAL8 f_min,              /**< start frequency */
	       	REAL8 r,                  /**< distance of source */
	       	REAL8 i,                  /**< inclination of source (rad) */
	       	int amplitudeO,           /**< twice post-Newtonian amplitude order */
	       	int phaseO                /**< twice post-Newtonian phase order */
		);

/**
 * Driver routine to compute the post-Newtonian inspiral waveform.
 *
 * This routine uses the same pN order for phasing and amplitude
 * (unless the order is -1 in which case the highest available
 * order is used for both of these -- which might not be the same).
 *
 * Log terms in amplitudes are ignored.  This is a gauge choice.
 */
int XLALSimInspiralTaylorEtPN(
		REAL8TimeSeries **hplus,  /**< +-polarization waveform */
	       	REAL8TimeSeries **hcross, /**< x-polarization waveform */
	       	REAL8 phic,               /**< orbital phase at end */
	       	REAL8 deltaT,             /**< sampling interval */
	       	REAL8 m1,                 /**< mass of companion 1 */
	       	REAL8 m2,                 /**< mass of companion 2 */
	       	REAL8 f_min,              /**< start frequency */
	       	REAL8 r,                  /**< distance of source */
	       	REAL8 i,                  /**< inclination of source (rad) */
	       	int O                     /**< twice post-Newtonian order */
		);

/**
 * Driver routine to compute the restricted post-Newtonian inspiral waveform.
 *
 * This routine computes the phasing to the specified order, but
 * only computes the amplitudes to the Newtonian (quadrupole) order.
 *
 * Log terms in amplitudes are ignored.  This is a gauge choice.
 */
int XLALSimInspiralTaylorEtPNRestricted(
		REAL8TimeSeries **hplus,  /**< +-polarization waveform */
	       	REAL8TimeSeries **hcross, /**< x-polarization waveform */
	       	REAL8 phic,               /**< orbital phase at end */
	       	REAL8 deltaT,             /**< sampling interval */
	       	REAL8 m1,                 /**< mass of companion 1 */
	       	REAL8 m2,                 /**< mass of companion 2 */
	       	REAL8 f_min,              /**< start frequency */
	       	REAL8 r,                  /**< distance of source */
	       	REAL8 i,                  /**< inclination of source (rad) */
	       	int O                     /**< twice post-Newtonian phase order */
		);


/**
 * Structure for passing around PN phasing coefficients.
 * For use with the TaylorF2 waveform.
 */
#define PN_PHASING_SERIES_MAX_ORDER 12
typedef struct tagPNPhasingSeries
{
    REAL8 v[PN_PHASING_SERIES_MAX_ORDER+1];        /**< */ 
    REAL8 vlogv[PN_PHASING_SERIES_MAX_ORDER+1];
    REAL8 vlogvsq[PN_PHASING_SERIES_MAX_ORDER+1];
}
PNPhasingSeries;

int XLALSimInspiralTaylorF2AlignedPhasing(
	PNPhasingSeries **pfa,
	const REAL8 m1,
	const REAL8 m2,
	const REAL8 chi1,
	const REAL8 chi2,
	const REAL8 qm_def1,
	const REAL8 qm_def2,
	const LALSimInspiralSpinOrder spinO
	);

int XLALSimInspiralTaylorF2Core(
        COMPLEX16FrequencySeries **htilde,    /**< FD waveform */
	const REAL8Sequence *freqs,            /**< frequency points at which to evaluate the waveform (Hz) */
//	const INT4 iStart,		       /**< start index for filling waveform data */
        const REAL8 phi_ref,                   /**< reference orbital phase (rad) */
        const REAL8 m1_SI,                     /**< mass of companion 1 (kg) */
        const REAL8 m2_SI,                     /**< mass of companion 2 (kg) */
        const REAL8 S1z,                       /**<  z component of the spin of companion 1 */
        const REAL8 S2z,                       /**<  z component of the spin of companion 2  */
        const REAL8 f_ref,                     /**< Reference GW frequency (Hz) - if 0 reference point is coalescence */
	const REAL8 shft,		       /**< time shift to be applied to frequency-domain phase (sec)*/
        const REAL8 r,                         /**< distance of source (m) */
        const REAL8 quadparam1,                /**< quadrupole deformation parameter of body 1 (dimensionless, 1 for BH) */
        const REAL8 quadparam2,                /**< quadrupole deformation parameter of body 2 (dimensionless, 1 for BH) */
        const REAL8 lambda1,                   /**< (tidal deformation of body 1)/(mass of body 1)^5 */
        const REAL8 lambda2,                   /**< (tidal deformation of body 2)/(mass of body 2)^5 */
        const LALSimInspiralSpinOrder spinO,  /**< twice PN order of spin effects */
        const LALSimInspiralTidalOrder tideO,  /**< flag to control tidal effects */
        const INT4 phaseO,                     /**< twice PN phase order */
        const INT4 amplitudeO                  /**< twice PN amplitude order */
        );

/**
 * Computes the stationary phase approximation to the Fourier transform of
 * a chirp waveform with phase given by \eqref{eq_InspiralFourierPhase_f2}
 * and amplitude given by expanding \f$1/\sqrt{\dot{F}}\f$. If the PN order is
 * set to -1, then the highest implemented order is used.
 */
int XLALSimInspiralTaylorF2(
		COMPLEX16FrequencySeries **htilde, /**< FD waveform */
		const REAL8 phi_ref,            /**< orbital reference phase (rad) */
		const REAL8 deltaF,             /**< frequency resolution */
		const REAL8 m1_SI,              /**< mass of companion 1 (kg) */
		const REAL8 m2_SI,              /**< mass of companion 2 (kg) */
		const REAL8 S1z,                /**<   z component of the spin of companion 1 */
		const REAL8 S2z,                /**<   z component of the spin of companion 2  */
		const REAL8 fStart,             /**< start GW frequency (Hz) */
		const REAL8 fEnd,               /**< highest GW frequency (Hz) of waveform generation - if 0, end at Schwarzschild ISCO */
        const REAL8 f_ref,              /**< Reference GW frequency at which phi_ref is defined */
		const REAL8 r,                  /**< distance of source (m) */
        const REAL8 quadparam1,                /**< quadrupole deformation parameter of body 1 (dimensionless, 1 for BH) */
        const REAL8 quadparam2,                /**< quadrupole deformation parameter of body 2 (dimensionless, 1 for BH) */
		const REAL8 lambda1,            /**< (tidal deformation of body 1)/(mass of body 1)^5 */
		const REAL8 lambda2,            /**< (tidal deformation of body 2)/(mass of body 2)^5 */
		const LALSimInspiralSpinOrder spinO,  /**< twice PN order of spin effects */
		LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
		const INT4 phaseO,              /**< twice PN phase order */
		const INT4 amplitudeO           /**< twice PN amplitude order */
		);



int XLALSimInspiralSpinTaylorF2(
    COMPLEX16FrequencySeries **hplus_out,  /**< FD hplus waveform */
    COMPLEX16FrequencySeries **hcross_out, /**< FD hcross waveform */
    REAL8 phi_ref,                   /**< reference orbital phase (rad) */
	REAL8 deltaF,                   /**< sampling frequency (Hz) */
	REAL8 m1_SI,                    /**< mass of companion 1 (kg) */
	REAL8 m2_SI,                    /**< mass of companion 2 (kg) */
	REAL8 s1x,                      /**< initial value of S1x */
	REAL8 s1y,                      /**< initial value of S1y */
	REAL8 s1z,                      /**< initial value of S1z */
	REAL8 lnhatx,                   /**< initial value of LNhatx */
	REAL8 lnhaty,                   /**< initial value of LNhaty */
	REAL8 lnhatz,                   /**< initial value of LNhatz */
    const REAL8 fStart,             /**< start GW frequency (Hz) */
    const REAL8 fEnd,               /**< highest GW frequency (Hz) of waveform generation - if 0, end at Schwarzschild ISCO */
    const REAL8 f_ref,              /**< Reference GW frequency (Hz) - if 0 reference point is coalescence */
    const REAL8 r,                  /**< distance of source (m) */
    LALSimInspiralTestGRParam *moreParams, /**< Linked list of extra params. Pass in NULL (or None in python) for standard waveform. Set "sideband",m to get a single sideband (m=-2..2) */
    const LALSimInspiralSpinOrder spinO,   /**< twice PN order of spin effects */
    const INT4 phaseO,              /**< twice PN phase order */
    const INT4 amplitudeO           /**< twice PN amplitude order */
	);

/**
 * Functions for generic spinning waveforms.
 * Reproduce and extend old SpinTaylor(Frameless) and SQTPN waveforms
 */

/**
 * This function evolves the orbital equations for a precessing binary using
 * the \"TaylorT1/T2/T4\" approximant for solving the orbital dynamics
 * (see arXiv:0907.0700 for a review of the various PN approximants).
 *
 * It returns time series of the \"orbital velocity\", orbital phase,
 * and components for both individual spin vectors, the \"Newtonian\"
 * orbital angular momentum (which defines the instantaneous plane)
 * and \"E1\", a basis vector in the instantaneous orbital plane.
 * Note that LNhat and E1 completely specify the instantaneous orbital plane.
 * It also returns the time and phase of the final time step
 *
 * For input, the function takes the two masses, the initial orbital phase,
 * Components for S1, S2, LNhat, E1 vectors at starting time,
 * the desired time step size, the starting GW frequency,
 * and PN order at which to evolve the phase,
 *
 * NOTE: All vectors are given in the so-called \"radiation frame\",
 * where the direction of propagation is the z-axis, the principal \"+\"
 * polarization axis is the x-axis, and the y-axis is given by the RH rule.
 * You must give the initial values in this frame, and the time series of the
 * vector components will also be returned in this frame
 */
int XLALSimInspiralSpinTaylorPNEvolveOrbit(
	REAL8TimeSeries **V,      /**< post-Newtonian parameter [returned]*/
	REAL8TimeSeries **Phi,    /**< orbital phase            [returned]*/
	REAL8TimeSeries **S1x,    /**< Spin1 vector x component [returned]*/
	REAL8TimeSeries **S1y,    /**< -- y component [returned]*/
	REAL8TimeSeries **S1z,    /**< -- z component [returned]*/
	REAL8TimeSeries **S2x,    /**< Spin2 vector x component [returned]*/
	REAL8TimeSeries **S2y,    /**< -- y component [returned]*/
	REAL8TimeSeries **S2z,    /**< -- z component [returned]*/
	REAL8TimeSeries **LNhatx, /**< unit orbital ang. mom. x [returned]*/
	REAL8TimeSeries **LNhaty, /**< -- y component [returned]*/
	REAL8TimeSeries **LNhatz, /**< -- z component [returned]*/
	REAL8TimeSeries **E1x,    /**< orb. plane basis vector x[returned]*/
	REAL8TimeSeries **E1y,    /**< -- y component [returned]*/
	REAL8TimeSeries **E1z,    /**< -- z component [returned]*/
	REAL8 deltaT,          	  /**< sampling interval (s) */
	REAL8 m1,              	  /**< mass of companion 1 (kg) */
	REAL8 m2,              	  /**< mass of companion 2 (kg) */
	REAL8 fStart,             /**< starting GW frequency */
	REAL8 fEnd,               /**< ending GW frequency, fEnd=0 means integrate as far forward as possible */
	REAL8 s1x,                /**< initial value of S1x */
	REAL8 s1y,                /**< initial value of S1y */
	REAL8 s1z,                /**< initial value of S1z */
	REAL8 s2x,                /**< initial value of S2x */
	REAL8 s2y,                /**< initial value of S2y */
	REAL8 s2z,                /**< initial value of S2z */
	REAL8 lnhatx,             /**< initial value of LNhatx */
	REAL8 lnhaty,             /**< initial value of LNhaty */
	REAL8 lnhatz,             /**< initial value of LNhatz */
	REAL8 e1x,                /**< initial value of E1x */
	REAL8 e1y,                /**< initial value of E1y */
	REAL8 e1z,                /**< initial value of E1z */
	REAL8 lambda1,            /**< (tidal deformability of mass 1) / (total mass)^5 (dimensionless) */
	REAL8 lambda2,            /**< (tidal deformability of mass 2) / (total mass)^5 (dimensionless) */
	REAL8 quadparam1,               /**< phenom. parameter describing induced quad. moment of body 1 (=1 for BHs, ~2-12 for NSs) */
	REAL8 quadparam2,               /**< phenom. parameter describing induced quad. moment of body 2 (=1 for BHs, ~2-12 for NSs) */
	LALSimInspiralSpinOrder spinO,  /**< twice PN order of spin effects */
	LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
	INT4 phaseO,              /**< twice post-Newtonian order */
	Approximant approx        /**< PN approximant (SpinTaylorT1/T2/T4) */
	);

/**
 * Driver routine to compute a precessing post-Newtonian inspiral waveform
 * with phasing computed from energy balance using the so-called \"T4\" method.
 *
 * This routine allows the user to specify different pN orders
 * for the phasing and amplitude of the waveform.
 *
 * The reference frequency fRef is used as follows:
 * 1) if fRef = 0: The initial values of s1, s2, lnhat and e1 will be the
 * values at frequency fStart. The orbital phase of the last sample is set
 * to phiRef (i.e. phiRef is the "coalescence phase", roughly speaking).
 * THIS IS THE DEFAULT BEHAVIOR CONSISTENT WITH OTHER APPROXIMANTS
 *
 * 2) If fRef = fStart: The initial values of s1, s2, lnhat and e1 will be the
 * values at frequency fStart. phiRef is used to set the orbital phase
 * of the first sample at fStart.
 *
 * 3) If fRef > fStart: The initial values of s1, s2, lnhat and e1 will be the
 * values at frequency fRef. phiRef is used to set the orbital phase at fRef.
 * The code will integrate forwards and backwards from fRef and stitch the
 * two together to create a complete waveform. This allows one to specify
 * the orientation of the binary in-band (or at any arbitrary point).
 * Otherwise, the user can only directly control the initial orientation.
 *
 * 4) fRef < 0 or fRef >= Schwarz. ISCO are forbidden and the code will abort.
 */
int XLALSimInspiralSpinTaylorT4(
	REAL8TimeSeries **hplus,        /**< +-polarization waveform */
	REAL8TimeSeries **hcross,       /**< x-polarization waveform */
	REAL8 phiRef,                   /**< orbital phase at reference pt. */
	REAL8 v0,                       /**< tail gauge term (default = 1) */
	REAL8 deltaT,                   /**< sampling interval (s) */
	REAL8 m1,                       /**< mass of companion 1 (kg) */
	REAL8 m2,                       /**< mass of companion 2 (kg) */
	REAL8 fStart,                   /**< start GW frequency (Hz) */
	REAL8 fRef,                     /**< reference GW frequency (Hz) */
	REAL8 r,                        /**< distance of source (m) */
	REAL8 s1x,                      /**< initial value of S1x */
	REAL8 s1y,                      /**< initial value of S1y */
	REAL8 s1z,                      /**< initial value of S1z */
	REAL8 s2x,                      /**< initial value of S2x */
	REAL8 s2y,                      /**< initial value of S2y */
	REAL8 s2z,                      /**< initial value of S2z */
	REAL8 lnhatx,                   /**< initial value of LNhatx */
	REAL8 lnhaty,                   /**< initial value of LNhaty */
	REAL8 lnhatz,                   /**< initial value of LNhatz */
	REAL8 e1x,                      /**< initial value of E1x */
	REAL8 e1y,                      /**< initial value of E1y */
	REAL8 e1z,                      /**< initial value of E1z */
	REAL8 lambda1,                  /**< (tidal deformability of mass 1) / (total mass)^5 (dimensionless) */
	REAL8 lambda2,                  /**< (tidal deformability of mass 2) / (total mass)^5 (dimensionless) */
	REAL8 quadparam1,               /**< phenom. parameter describing induced quad. moment of body 1 (=1 for BHs, ~2-12 for NSs) */
	REAL8 quadparam2,               /**< phenom. parameter describing induced quad. moment of body 2 (=1 for BHs, ~2-12 for NSs) */
	LALSimInspiralSpinOrder spinO,  /**< twice PN order of spin effects */
	LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
	int phaseO,                     /**< twice PN phase order */
	int amplitudeO                  /**< twice PN amplitude order */
	);

int XLALSimInspiralSpinTaylorT5 (
        REAL8TimeSeries **hplus,        /**< +-polarization waveform */
        REAL8TimeSeries **hcross,       /**< x-polarization waveform */
        REAL8 phiRef,                   /**< orbital phase at reference pt. */
        REAL8 deltaT,                   /**< sampling interval (s) */
        REAL8 m1,                       /**< mass of companion 1 (kg) */
        REAL8 m2,                       /**< mass of companion 2 (kg) */
        REAL8 fStart,                   /**< start GW frequency (Hz) */
        REAL8 r,                        /**< distance of source (m) */
        REAL8 s1x,                      /**< initial value of S1x */
        REAL8 s1y,                      /**< initial value of S1y */
        REAL8 s1z,                      /**< initial value of S1z */
        REAL8 s2x,                      /**< initial value of S2x */
        REAL8 s2y,                      /**< initial value of S2y */
        REAL8 s2z,                      /**< initial value of S2z */
        REAL8 incAngle,                                 /**< inclination angle with J_ini */
        int phaseO,                     /**< twice PN phase order */
        int amplitudeO                  /**< twice PN amplitude order */
        );


int XLALSimInspiralSpinTaylorT2(
	REAL8TimeSeries **hplus,        /**< +-polarization waveform */
	REAL8TimeSeries **hcross,       /**< x-polarization waveform */
	REAL8 phiRef,                   /**< orbital phase at reference pt. */
	REAL8 v0,                       /**< tail gauge term (default = 1) */
	REAL8 deltaT,                   /**< sampling interval (s) */
	REAL8 m1,                       /**< mass of companion 1 (kg) */
	REAL8 m2,                       /**< mass of companion 2 (kg) */
	REAL8 fStart,                   /**< start GW frequency (Hz) */
	REAL8 fRef,                     /**< reference GW frequency (Hz) */
	REAL8 r,                        /**< distance of source (m) */
	REAL8 s1x,                      /**< initial value of S1x */
	REAL8 s1y,                      /**< initial value of S1y */
	REAL8 s1z,                      /**< initial value of S1z */
	REAL8 s2x,                      /**< initial value of S2x */
	REAL8 s2y,                      /**< initial value of S2y */
	REAL8 s2z,                      /**< initial value of S2z */
	REAL8 lnhatx,                   /**< initial value of LNhatx */
	REAL8 lnhaty,                   /**< initial value of LNhaty */
	REAL8 lnhatz,                   /**< initial value of LNhatz */
	REAL8 e1x,                      /**< initial value of E1x */
	REAL8 e1y,                      /**< initial value of E1y */
	REAL8 e1z,                      /**< initial value of E1z */
	REAL8 lambda1,                  /**< (tidal deformability of mass 1) / (total mass)^5 (dimensionless) */
	REAL8 lambda2,                  /**< (tidal deformability of mass 2) / (total mass)^5 (dimensionless) */
	REAL8 quadparam1,               /**< phenom. parameter describing induced quad. moment of body 1 (=1 for BHs, ~2-12 for NSs) */
	REAL8 quadparam2,               /**< phenom. parameter describing induced quad. moment of body 2 (=1 for BHs, ~2-12 for NSs) */
	LALSimInspiralSpinOrder spinO,  /**< twice PN order of spin effects */
	LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
	int phaseO,                     /**< twice PN phase order */
	int amplitudeO                  /**< twice PN amplitude order */
	);





/**
 * Driver routine to compute a precessing post-Newtonian inspiral waveform in the Fourier domain
 * with phasing computed from energy balance using the so-called \"T4\" method.
 *
 * This routine allows the user to specify different pN orders
 * for the phasing and amplitude of the waveform.
 *
 * The reference frequency fRef is used as follows:
 * 1) if fRef = 0: The initial values of s1, s2, lnhat and e1 will be the
 * values at frequency fStart. The orbital phase of the last sample is set
 * to phiRef (i.e. phiRef is the "coalescence phase", roughly speaking).
 * THIS IS THE DEFAULT BEHAVIOR CONSISTENT WITH OTHER APPROXIMANTS
 *
 * 2) If fRef = fStart: The initial values of s1, s2, lnhat and e1 will be the
 * values at frequency fStart. phiRef is used to set the orbital phase
 * of the first sample at fStart.
 *
 * 3) If fRef > fStart: The initial values of s1, s2, lnhat and e1 will be the
 * values at frequency fRef. phiRef is used to set the orbital phase at fRef.
 * The code will integrate forwards and backwards from fRef and stitch the
 * two together to create a complete waveform. This allows one to specify
 * the orientation of the binary in-band (or at any arbitrary point).
 * Otherwise, the user can only directly control the initial orientation.
 *
 * 4) fRef < 0 or fRef >= Schwarz. ISCO are forbidden and the code will abort.
 *
 * It is recommended, but not necessary to set fStart slightly smaller than fMin,
 * e.g. fStart = 9.5 for fMin = 10.
 *
 * The returned Fourier series are set so that the Schwarzschild ISCO frequency
 * corresponds to t = 0 as closely as possible.
 *
 */
int XLALSimInspiralSpinTaylorT4Fourier(
        COMPLEX16FrequencySeries **hplus,        /**< +-polarization waveform */
        COMPLEX16FrequencySeries **hcross,       /**< x-polarization waveform */
        REAL8 fMin,                     /**< minimum frequency of the returned series */
        REAL8 fMax,                     /**< maximum frequency of the returned series */
        REAL8 deltaF,                   /**< frequency interval of the returned series */
        INT4 kMax,                      /**< k_max as described in arXiv: 1408.5158 (min 0, max 10). */
        REAL8 phiRef,                   /**< orbital phase at reference pt. */
        REAL8 v0,                       /**< tail gauge term (default = 1) */
        REAL8 m1,                       /**< mass of companion 1 (kg) */
        REAL8 m2,                       /**< mass of companion 2 (kg) */
        REAL8 fStart,                   /**< start GW frequency (Hz) */
        REAL8 fRef,                     /**< reference GW frequency (Hz) */
        REAL8 r,                        /**< distance of source (m) */
        REAL8 s1x,                      /**< initial value of S1x */
        REAL8 s1y,                      /**< initial value of S1y */
        REAL8 s1z,                      /**< initial value of S1z */
        REAL8 s2x,                      /**< initial value of S2x */
        REAL8 s2y,                      /**< initial value of S2y */
        REAL8 s2z,                      /**< initial value of S2z */
        REAL8 lnhatx,                   /**< initial value of LNhatx */
        REAL8 lnhaty,                   /**< initial value of LNhaty */
        REAL8 lnhatz,                   /**< initial value of LNhatz */
        REAL8 e1x,                      /**< initial value of E1x */
        REAL8 e1y,                      /**< initial value of E1y */
        REAL8 e1z,                      /**< initial value of E1z */
        REAL8 lambda1,                  /**< (tidal deformability of mass 1) / (mass of body 1)^5 (dimensionless) */
        REAL8 lambda2,                  /**< (tidal deformability of mass 2) / (mass of body 2)^5 (dimensionless) */
        REAL8 quadparam1,               /**< phenom. parameter describing induced quad. moment of body 1 (=1 for BHs, ~2-12 for NSs) */
        REAL8 quadparam2,               /**< phenom. parameter describing induced quad. moment of body 2 (=1 for BHs, ~2-12 for NSs) */
        LALSimInspiralSpinOrder spinO,  /**< twice PN order of spin effects */
        LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
        INT4 phaseO,                    /**< twice PN phase order */
        INT4 amplitudeO,                /**< twice PN amplitude order */
        INT4 phiRefAtEnd                /**< whether phiRef corresponds to the end of the inspiral */
        );

/**
 * Driver routine to compute a precessing post-Newtonian inspiral waveform in the Fourier domain
 * with phasing computed from energy balance using the so-called \"T2\" method.
 *
 * This routine allows the user to specify different pN orders
 * for the phasing and amplitude of the waveform.
 *
 * The reference frequency fRef is used as follows:
 * 1) if fRef = 0: The initial values of s1, s2, lnhat and e1 will be the
 * values at frequency fStart. The orbital phase of the last sample is set
 * to phiRef (i.e. phiRef is the "coalescence phase", roughly speaking).
 * THIS IS THE DEFAULT BEHAVIOR CONSISTENT WITH OTHER APPROXIMANTS
 *
 * 2) If fRef = fStart: The initial values of s1, s2, lnhat and e1 will be the
 * values at frequency fStart. phiRef is used to set the orbital phase
 * of the first sample at fStart.
 *
 * 3) If fRef > fStart: The initial values of s1, s2, lnhat and e1 will be the
 * values at frequency fRef. phiRef is used to set the orbital phase at fRef.
 * The code will integrate forwards and backwards from fRef and stitch the
 * two together to create a complete waveform. This allows one to specify
 * the orientation of the binary in-band (or at any arbitrary point).
 * Otherwise, the user can only directly control the initial orientation.
 *
 * 4) fRef < 0 or fRef >= Schwarz. ISCO are forbidden and the code will abort.
 *
 * It is recommended, but not necessary to set fStart slightly smaller than fMin,
 * e.g. fStart = 9.5 for fMin = 10.
 *
 * The returned Fourier series are set so that the Schwarzschild ISCO frequency
 * corresponds to t = 0 as closely as possible.
 *
 */
int XLALSimInspiralSpinTaylorT2Fourier(
        COMPLEX16FrequencySeries **hplus,        /**< +-polarization waveform */
        COMPLEX16FrequencySeries **hcross,       /**< x-polarization waveform */
        REAL8 fMin,                     /**< minimum frequency of the returned series */
        REAL8 fMax,                     /**< maximum frequency of the returned series */
        REAL8 deltaF,                   /**< frequency interval of the returned series */
        INT4 kMax,                      /**< k_max as described in arXiv: 1408.5158 (min 0, max 10). */
        REAL8 phiRef,                   /**< orbital phase at reference pt. */
        REAL8 v0,                       /**< tail gauge term (default = 1) */
        REAL8 m1,                       /**< mass of companion 1 (kg) */
        REAL8 m2,                       /**< mass of companion 2 (kg) */
        REAL8 fStart,                   /**< start GW frequency (Hz) */
        REAL8 fRef,                     /**< reference GW frequency (Hz) */
        REAL8 r,                        /**< distance of source (m) */
        REAL8 s1x,                      /**< initial value of S1x */
        REAL8 s1y,                      /**< initial value of S1y */
        REAL8 s1z,                      /**< initial value of S1z */
        REAL8 s2x,                      /**< initial value of S2x */
        REAL8 s2y,                      /**< initial value of S2y */
        REAL8 s2z,                      /**< initial value of S2z */
        REAL8 lnhatx,                   /**< initial value of LNhatx */
        REAL8 lnhaty,                   /**< initial value of LNhaty */
        REAL8 lnhatz,                   /**< initial value of LNhatz */
        REAL8 e1x,                      /**< initial value of E1x */
        REAL8 e1y,                      /**< initial value of E1y */
        REAL8 e1z,                      /**< initial value of E1z */
        REAL8 lambda1,                  /**< (tidal deformability of mass 1) / (mass of body 1)^5 (dimensionless) */
        REAL8 lambda2,                  /**< (tidal deformability of mass 2) / (mass of body 2)^5 (dimensionless) */
        REAL8 quadparam1,               /**< phenom. parameter describing induced quad. moment of body 1 (=1 for BHs, ~2-12 for NSs) */
        REAL8 quadparam2,               /**< phenom. parameter describing induced quad. moment of body 2 (=1 for BHs, ~2-12 for NSs) */
        LALSimInspiralSpinOrder spinO,  /**< twice PN order of spin effects */
        LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
        INT4 phaseO,                     /**< twice PN phase order */
        INT4 amplitudeO,                /**< twice PN amplitude order */
        INT4 phiRefAtEnd                /**< whether phiRef corresponds to the end of the inspiral */
        );


/**
 * Driver routine to compute the physical template family "Q" vectors using
 * the \"T4\" method. Note that PTF describes single spin systems
 *
 * This routine requires leading-order amplitude dependence
 * but allows the user to specify the phase PN order
 */

int XLALSimInspiralSpinTaylorT4PTFQVecs(
        REAL8TimeSeries **Q1,       /**< Q1 output vector */
        REAL8TimeSeries **Q2,       /**< Q2 output vector */
        REAL8TimeSeries **Q3,       /**< Q3 output vector */
        REAL8TimeSeries **Q4,       /**< Q4 output vector */
        REAL8TimeSeries **Q5,       /**< Q5 output vector */
        REAL8 deltaT,               /**< sampling interval (s) */
        REAL8 m1,                   /**< mass of companion 1 (kg) */
        REAL8 m2,                   /**< mass of companion 2 (kg) */
        REAL8 chi1,                 /**< spin magnitude (|S1|) */
        REAL8 kappa,                /**< L . S (1 if they are aligned) */
        REAL8 fStart,               /**< start GW frequency (Hz) */
        REAL8 lambda1,              /**< (tidal deformability of mass 1) / (total mass)^5 (dimensionless) */
        REAL8 lambda2,              /**< (tidal deformability of mass 2) / (total mass)^5 (dimensionless) */
	LALSimInspiralSpinOrder spinO,  /**< twice PN order of spin effects */
	LALSimInspiralTidalOrder tideO, /**< twice PN order of tidal effects */
        int phaseO                  /**< twice PN phase order */
        );

/**
 * Functions for calculating the  Spin-Dominated waveforms
 * See tables 1 to 5 in the appendix of Arxiv:1209.1722
 * Interface routine, calculating the prefered variables for the Spin-dominated waveforms
 */
int XLALSimInspiralSpinDominatedWaveformInterfaceTD(
	REAL8TimeSeries **hplus,        /**< +-polarization waveform */
	REAL8TimeSeries **hcross,       /**< x-polarization waveform */
	REAL8 deltaT,                   /**< sampling interval (s) */
	REAL8 m1,                       /**< mass of companion 1 (kg) */
	REAL8 m2,                       /**< mass of companion 2 (kg) */
	REAL8 fStart,                   /**< start GW frequency (Hz) */
	REAL8 fRef,			/**< end GW frequency (Hz) */
	REAL8 D,                        /**< distance of source (m) */
	REAL8 s1x,                      /**< initial value of S1x */
	REAL8 s1y,                      /**< initial value of S1y */
	REAL8 s1z,                      /**< initial value of S1z */
	REAL8 lnhatx,                   /**< initial value of LNhatx */
	REAL8 lnhaty,                   /**< initial value of LNhaty */
	REAL8 lnhatz,                   /**< initial value of LNhatz */
	int phaseO,                     /**< twice PN phase order */
	int amplitudeO,                 /**< twice PN amplitude order */
	REAL8 phiRef			/**< Reference phase at the Reference Frequency */
);

/**
 * Function calculating the Spin-Dominated waveforms
 * This waveform is an inspiral only, 1 spin, precessing waveform.
 * For the formulae see the appendix of Arxiv:1209.1722
 */
int XLALSimInspiralSpinDominatedWaveformDriver(
	REAL8TimeSeries **hplus,        /**< +-polarization waveform */
	REAL8TimeSeries **hcross,       /**< x-polarization waveform */
	REAL8 totalmass,		/**< total mass of the binary */
	REAL8 nu,			/**< mass ratio */
	REAL8 chi1,			/**< dimensionless spin paramter */
	REAL8 D,			/**< Distance to the source */
	REAL8 kappa1,			/**< Angle span by S_1 and L */
	REAL8 beta1,			/**< Angle span by J and S_1 */
	REAL8 theta,			/**< Angle span by the line of sight and J */
	REAL8 fStart,			/**< Starting gravitational wave frequency*/
	REAL8 fRef,			/**< Ending gravitational wave frequency*/
	int phaseO,                     /**< twice PN phase order */
	int amplitudeO,                 /**< twice PN amplitude order */
	REAL8 deltaT,			/**< Sampling time interval */
	REAL8 phiRef,			/**< Reference phase at the Reference Frequency */
	REAL8 phin0			/**< Starting value of the \phi_n parameter */
);

/**
 * Function to specify the desired orientation of a precessing binary in terms
 * of several angles and then compute the vector components in the so-called
 * \"radiation frame\" (with the z-axis along the direction of propagation) as
 * needed to specify binary configuration for ChooseTDWaveform.
 *
 * Input:
 * thetaJN is the inclination between total angular momentum (J) and the
 * direction of propagation (N)
 * theta1 and theta2 are the inclinations of S1 and S2
 * measured from the Newtonian orbital angular momentum (L_N)
 * phi12 is the difference in azimuthal angles of S1 and S2.
 * chi1, chi2 are the dimensionless spin magnitudes ( \f$0 \le chi1,2 \le 1\f$)
 * phiJL is the azimuthal angle of L_N on its cone about J.
 * m1, m2, f_ref are the component masses and reference GW frequency,
 * they are needed to compute the magnitude of L_N, and thus J.
 *
 * Output:
 * incl - inclination angle of L_N relative to N
 * x, y, z components of E1 (unit vector in the initial orbital plane)
 * x, y, z components S1 and S2 (unit spin vectors times their
 * dimensionless spin magnitudes - i.e. they have unit magnitude for
 * extremal BHs and smaller magnitude for slower spins).
 *
 * NOTE: Here the \"total\" angular momentum is computed as
 * J = L_N + S1 + S2
 * where L_N is the Newtonian orbital angular momentum. In fact, there are
 * PN corrections to L which contribute to J that are NOT ACCOUNTED FOR
 * in this function. This is done so the function does not need to know about
 * the PN order of the system and to avoid subtleties with spin-orbit
 * contributions to L. Also, it is believed that the difference in Jhat
 * with or without these PN corrections to L is quite small.
 *
 * NOTE: fRef = 0 is not a valid choice. If you will pass fRef=0 into
 * ChooseWaveform, then here pass in f_min, the starting GW frequency
 *
 * The various rotations in this transformation are described in more detail
 * in a Mathematica notebook available here:
 * https://www.lsc-group.phys.uwm.edu/ligovirgo/cbcnote/Waveforms/TransformPrecessingInitialConditions
 */
int XLALSimInspiralTransformPrecessingInitialConditions(
		REAL8 *incl,	/**< Inclination angle of L_N (returned) */
		REAL8 *S1x,	/**< S1 x component (returned) */
		REAL8 *S1y,	/**< S1 y component (returned) */
		REAL8 *S1z,	/**< S1 z component (returned) */
		REAL8 *S2x,	/**< S2 x component (returned) */
		REAL8 *S2y,	/**< S2 y component (returned) */
		REAL8 *S2z,	/**< S2 z component (returned) */
		REAL8 thetaJN, 	/**< zenith angle between J and N (rad) */
		REAL8 phiJL,  	/**< azimuthal angle of L_N on its cone about J (rad) */
		REAL8 theta1,  	/**< zenith angle between S1 and LNhat (rad) */
		REAL8 theta2,  	/**< zenith angle between S2 and LNhat (rad) */
		REAL8 phi12,  	/**< difference in azimuthal angle btwn S1, S2 (rad) */
		REAL8 chi1,	/**< dimensionless spin of body 1 */
		REAL8 chi2,	/**< dimensionless spin of body 2 */
		REAL8 m1,	/**< mass of body 1 (kg) */
		REAL8 m2,	/**< mass of body 2 (kg) */
		REAL8 fRef	/**< reference GW frequency (Hz) */
		);

/**
 * Driver routine to compute a non-precessing post-Newtonian inspiral waveform
 * in the frequency domain, described in http://arxiv.org/abs/1107.1267.
 *
 * The chi parameter should be determined from
 * XLALSimInspiralTaylorF2ReducedSpinComputeChi.
 *
 * A note from Evan Ochsner on differences with respect to TaylorF2:
 *
 * The amplitude-corrected SPA/F2 waveforms are derived and explicitly given in
 * <http://arxiv.org/abs/gr-qc/0607092> Sec. II and Appendix A (non-spinning)
 * and <http://arxiv.org/abs/0810.5336> Sec. VI and Appendix D (spin-aligned).
 *
 * The difference between F2 and F2ReducedSpin is that F2ReducedSpin always
 * keeps only the leading-order TD amplitude multiplying the 2nd harmonic (
 * A_(2,0)(t) in Eq. 2.3 of the first paper OR alpha/beta_2^(0)(t) in Eq. 6.7
 * of the second paper) but expands out the \f$1/\sqrt{\dot{F}}\f$ ( Eq. 5.3 OR Eq.
 * 6.10-6.11 resp.) to whichever order is given as 'ampO' in the code.
 *
 * On the other hand, the F2 model in the papers above will PN expand BOTH the
 * TD amplitude and the factor \f$1/\sqrt{\dot{F}}\f$, take their product, and keep
 * all terms up to the desired amplitude order, as in Eq. 6.13-6.14 of the
 * second paper.
 *
 * In particular, the F2ReducedSpin will always have only the 2nd harmonic, but
 * F2 will have multiple harmonics starting at ampO = 0.5PN. Even if you were
 * to compare just the 2nd harmonic, you would have a difference starting at
 * 1PN ampO, because the F2 has a 1PN TD amp. correction to the 2nd harmonic
 * (alpha/beta_2^(2)(t)) which will not be accounted for by the F2ReducedSpin.
 * So, the two should agree when ampO=0, but will be different in any other
 * case.
 */
int XLALSimInspiralTaylorF2ReducedSpin(
		COMPLEX16FrequencySeries **htilde, /**< FD waveform */
		const REAL8 phic,        /**< orbital coalescence phase (rad) */
		const REAL8 deltaF,      /**< frequency resolution (Hz) */
		const REAL8 m1_SI,       /**< mass of companion 1 (kg) */
		const REAL8 m2_SI,       /**< mass of companion 2 (kg) */
		const REAL8 chi,         /**< dimensionless aligned-spin param */
		const REAL8 fStart,      /**< start GW frequency (Hz) */
		const REAL8 fEnd,        /**< highest GW frequency (Hz) of waveform generation - if 0, end at Schwarzschild ISCO */
		const REAL8 r,           /**< distance of source (m) */
		const INT4 phaseO,       /**< twice PN phase order */
		const INT4 ampO          /**< twice PN amplitude order */
		);

/**
 * Generate the \"reduced-spin templates\" proposed in http://arxiv.org/abs/1107.1267
 * Add the tidal phase terms from http://arxiv.org/abs/1101.1673 (Eqs. 3.9, 3.10)
 * The chi parameter should be determined from XLALSimInspiralTaylorF2ReducedSpinComputeChi.
 */
int XLALSimInspiralTaylorF2ReducedSpinTidal(
		COMPLEX16FrequencySeries **htilde,   /**< FD waveform */
		const REAL8 phic,        /**< orbital coalescence phase (rad) */
		const REAL8 deltaF,      /**< frequency resolution (Hz) */
		const REAL8 m1_SI,       /**< mass of companion 1 (kg) */
		const REAL8 m2_SI,       /**< mass of companion 2 (kg) */
		const REAL8 chi,         /**< dimensionless aligned-spin param */
		const REAL8 lam1,        /**< dimensionless deformability of 1 */
		const REAL8 lam2,        /**< dimensionless deformability of 2 */
		const REAL8 fStart,      /**< start GW frequency (Hz) */
		const REAL8 fEnd,        /**< highest GW frequency (Hz) of waveform generation - if 0, end at Schwarzschild ISCO */
		const REAL8 r,           /**< distance of source (m) */
		const INT4 phaseO,       /**< twice PN phase order */
		const INT4 ampO          /**< twice PN amplitude order */
		);
/**
 * Compute the chirp time of the \"reduced-spin\" templates, described in
 * http://arxiv.org/abs/1107.1267.
 */
REAL8 XLALSimInspiralTaylorF2ReducedSpinChirpTime(
		const REAL8 fStart,  /**< start GW frequency (Hz) */
		const REAL8 m1_SI,   /**< mass of companion 1 (kg) */
		const REAL8 m2_SI,   /**< mass of companion 2 (kg) */
		const REAL8 chi,     /**< dimensionless aligned-spin param */
		const INT4 O        /**< twice PN phase order */
		);

/**
 * Compute the dimensionless, spin-aligned parameter chi as used in the
 * TaylorF2RedSpin waveform. This is different from chi in IMRPhenomB!
 * Reference: http://arxiv.org/abs/1107.1267, paragraph 3.
 */
REAL8 XLALSimInspiralTaylorF2ReducedSpinComputeChi(
    const REAL8 m1,                          /**< mass of companion 1 */
    const REAL8 m2,                          /**< mass of companion 2 */
    const REAL8 s1z,                         /**< dimensionless spin of companion 1 */
    const REAL8 s2z                          /**< dimensionless spin of companion 2 */
);

/**
 * Compute the template-space metric of \"reduced-spin\" PN templates in
 * Mchirp-eta-chi parameter space.
 */
int XLALSimInspiralTaylorF2RedSpinMetricMChirpEtaChi(
    REAL8 *gamma00,  /**< template metric coeff. 00 in mChirp-eta-chi */
    REAL8 *gamma01,  /**< template metric coeff. 01/10 in mChirp-eta-chi */
    REAL8 *gamma02,  /**< template metric coeff. 02/20 in mChirp-eta-chi */
    REAL8 *gamma11,  /**< template metric coeff. 11 in mChirp-eta-chi */
    REAL8 *gamma12,  /**< template metric coeff. 12/21 in mChirp-eta-chi */
    REAL8 *gamma22,  /**< template metric coeff. 22 in mChirp-eta-chi */
    const REAL8 mc,     /**< chirp mass (in solar mass) */
    const REAL8 eta,    /**< symmetric mass ratio */
    const REAL8 chi,    /**< reduced-spin parameter */
    const REAL8 fLow,   /**< low-frequency cutoff (Hz) */
    const REAL8FrequencySeries *Sh
);

/**
 * Compute the Fisher information matrix of "reduced-spin" PN templates in
 * theta0, theta3, theta3s, t0, phi0 parameter space, for an SNR=1/sqrt(2) signal.
 */
gsl_matrix *XLALSimInspiralTaylorF2RedSpinFisherMatrixChirpTimes(
    const REAL8 theta0,     /**< dimensionless parameter related to the chirp time by theta0 = 2 pi fLow tau0 */
    const REAL8 theta3,     /**< dimensionless parameter related to the chirp time by theta3 = -2 pi fLow tau3 */
    const REAL8 theta3s,    /**< dimensionless parameter related to the chirp time by theta3s = 2 pi fLow tau3s */
    const REAL8 fLow,       /**< low-frequency cutoff (Hz) */
    const REAL8 df,         /**< frequency resolution of the noise moment vectors (Hz) */
    REAL8Vector *momI_0,     /**< noise moments: \f$momI_0(f) = \int_{f0}^f (f'/f0)^{(0-17)/3} df'\f$ */
    REAL8Vector *momI_2,     /**< noise moments: \f$momI_2(f) = \int_{f0}^f (f'/f0)^{(2-17)/3} df'\f$ */
    REAL8Vector *momI_3,     /**< noise moments: \f$momI_3(f) = \int_{f0}^f (f'/f0)^{(3-17)/3} df'\f$ */
    REAL8Vector *momI_4,     /**< noise moments: \f$momI_4(f) = \int_{f0}^f (f'/f0)^{(4-17)/3} df'\f$ */
    REAL8Vector *momI_5,     /**< noise moments: \f$momI_5(f) = \int_{f0}^f (f'/f0)^{(5-17)/3} df'\f$ */
    REAL8Vector *momI_6,     /**< noise moments: \f$momI_6(f) = \int_{f0}^f (f'/f0)^{(6-17)/3} df'\f$ */
    REAL8Vector *momI_7,     /**< noise moments: \f$momI_7(f) = \int_{f0}^f (f'/f0)^{(7-17)/3} df'\f$ */
    REAL8Vector *momI_8,     /**< noise moments: \f$momI_8(f) = \int_{f0}^f (f'/f0)^{(8-17)/3} df'\f$ */
    REAL8Vector *momI_9,     /**< noise moments: \f$momI_9(f) = \int_{f0}^f (f'/f0)^{(9-17)/3} df'\f$ */
    REAL8Vector *momI_10,    /**< noise moments: \f$momI_10(f) = \int_{f0}^f (f'/f0)^{(10-17)/3} df'\f$ */
    REAL8Vector *momI_11,    /**< noise moments: \f$momI_11(f) = \int_{f0}^f (f'/f0)^{(11-17)/3} df'\f$ */
    REAL8Vector *momI_12,    /**< noise moments: \f$momI_12(f) = \int_{f0}^f (f'/f0)^{(12-17)/3} df'\f$ */
    REAL8Vector *momI_13,    /**< noise moments: \f$momI_13(f) = \int_{f0}^f (f'/f0)^{(13-17)/3} df'\f$ */
    REAL8Vector *momI_14,    /**< noise moments: \f$momI_14(f) = \int_{f0}^f (f'/f0)^{(14-17)/3} df'\f$ */
    REAL8Vector *momI_15,    /**< noise moments: \f$momI_15(f) = \int_{f0}^f (f'/f0)^{(15-17)/3} df'\f$ */
    REAL8Vector *momI_16,    /**< noise moments: \f$momI_16(f) = \int_{f0}^f (f'/f0)^{(16-17)/3} df'\f$ */
    REAL8Vector *momJ_5,     /**< noise moments: \f$momJ_5(f) = \int_{f0}^f (f'/f0)^{(5-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_6,     /**< noise moments: \f$momJ_6(f) = \int_{f0}^f (f'/f0)^{(6-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_7,     /**< noise moments: \f$momJ_7(f) = \int_{f0}^f (f'/f0)^{(7-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_8,     /**< noise moments: \f$momJ_8(f) = \int_{f0}^f (f'/f0)^{(8-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_9,     /**< noise moments: \f$momJ_9(f) = \int_{f0}^f (f'/f0)^{(9-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_10,    /**< noise moments: \f$momJ_10(f) = \int_{f0}^f (f'/f0)^{(10-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_11,    /**< noise moments: \f$momJ_11(f) = \int_{f0}^f (f'/f0)^{(11-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_12,    /**< noise moments: \f$momJ_12(f) = \int_{f0}^f (f'/f0)^{(12-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_13,    /**< noise moments: \f$momJ_13(f) = \int_{f0}^f (f'/f0)^{(13-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_14,    /**< noise moments: \f$momJ_14(f) = \int_{f0}^f (f'/f0)^{(14-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momK_10,    /**< noise moments: \f$momK_14(f) = \int_{f0}^f (f'/f0)^{(14-17)/3} log(f'/f0) log(f'/f0) df'\f$ */
    REAL8Vector *momK_11,    /**< noise moments: \f$momK_15(f) = \int_{f0}^f (f'/f0)^{(15-17)/3} log(f'/f0) log(f'/f0) df'\f$ */
    REAL8Vector *momK_12     /**< noise moments: \f$momK_16(f) = \int_{f0}^f (f'/f0)^{(16-17)/3} log(f'/f0) log(f'/f0) df'\f$ */
);

/**
 * Compute the template-space metric of "reduced-spin" PN templates in
 * theta0, theta3, theta3s parameter space.
 */
int XLALSimInspiralTaylorF2RedSpinMetricChirpTimes(
    REAL8 *gamma00,         /**< template metric coeff. 00 in theta0-theta3-theta3s*/
    REAL8 *gamma01,         /**< template metric coeff. 01/10 in theta0-theta3-theta3s */
    REAL8 *gamma02,         /**< template metric coeff. 02/20 in theta0-theta3-theta3s */
    REAL8 *gamma11,         /**< template metric coeff. 11 in theta0-theta3-theta3s */
    REAL8 *gamma12,         /**< template metric coeff. 12/21 in theta0-theta3-theta3s */
    REAL8 *gamma22,         /**< template metric coeff. 22 in theta0-theta3-theta3s */
    const REAL8 theta0,     /**< dimensionless parameter related to the chirp time by theta0 = 2 pi fLow tau0 */
    const REAL8 theta3,     /**< dimensionless parameter related to the chirp time by theta3 = -2 pi fLow tau3 */
    const REAL8 theta3s,    /**< dimensionless parameter related to the chirp time by theta3s = 2 pi fLow tau3s */
    const REAL8 fLow,       /**< low-frequency cutoff (Hz) */
    const REAL8 df,         /**< frequency resolution of the noise moment vectors (Hz) */
    REAL8Vector *momI_0,     /**< noise moments: \f$momI_0(f) = \int_{f0}^f (f'/f0)^{(0-17)/3} df'\f$ */
    REAL8Vector *momI_2,     /**< noise moments: \f$momI_2(f) = \int_{f0}^f (f'/f0)^{(2-17)/3} df'\f$ */
    REAL8Vector *momI_3,     /**< noise moments: \f$momI_3(f) = \int_{f0}^f (f'/f0)^{(3-17)/3} df'\f$ */
    REAL8Vector *momI_4,     /**< noise moments: \f$momI_4(f) = \int_{f0}^f (f'/f0)^{(4-17)/3} df'\f$ */
    REAL8Vector *momI_5,     /**< noise moments: \f$momI_5(f) = \int_{f0}^f (f'/f0)^{(5-17)/3} df'\f$ */
    REAL8Vector *momI_6,     /**< noise moments: \f$momI_6(f) = \int_{f0}^f (f'/f0)^{(6-17)/3} df'\f$ */
    REAL8Vector *momI_7,     /**< noise moments: \f$momI_7(f) = \int_{f0}^f (f'/f0)^{(7-17)/3} df'\f$ */
    REAL8Vector *momI_8,     /**< noise moments: \f$momI_8(f) = \int_{f0}^f (f'/f0)^{(8-17)/3} df'\f$ */
    REAL8Vector *momI_9,     /**< noise moments: \f$momI_9(f) = \int_{f0}^f (f'/f0)^{(9-17)/3} df'\f$ */
    REAL8Vector *momI_10,    /**< noise moments: \f$momI_10(f) = \int_{f0}^f (f'/f0)^{(10-17)/3} df'\f$ */
    REAL8Vector *momI_11,    /**< noise moments: \f$momI_11(f) = \int_{f0}^f (f'/f0)^{(11-17)/3} df'\f$ */
    REAL8Vector *momI_12,    /**< noise moments: \f$momI_12(f) = \int_{f0}^f (f'/f0)^{(12-17)/3} df'\f$ */
    REAL8Vector *momI_13,    /**< noise moments: \f$momI_13(f) = \int_{f0}^f (f'/f0)^{(13-17)/3} df'\f$ */
    REAL8Vector *momI_14,    /**< noise moments: \f$momI_14(f) = \int_{f0}^f (f'/f0)^{(14-17)/3} df'\f$ */
    REAL8Vector *momI_15,    /**< noise moments: \f$momI_15(f) = \int_{f0}^f (f'/f0)^{(15-17)/3} df'\f$ */
    REAL8Vector *momI_16,    /**< noise moments: \f$momI_16(f) = \int_{f0}^f (f'/f0)^{(16-17)/3} df'\f$ */
    REAL8Vector *momJ_5,     /**< noise moments: \f$momJ_5(f) = \int_{f0}^f (f'/f0)^{(5-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_6,     /**< noise moments: \f$momJ_6(f) = \int_{f0}^f (f'/f0)^{(6-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_7,     /**< noise moments: \f$momJ_7(f) = \int_{f0}^f (f'/f0)^{(7-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_8,     /**< noise moments: \f$momJ_8(f) = \int_{f0}^f (f'/f0)^{(8-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_9,     /**< noise moments: \f$momJ_9(f) = \int_{f0}^f (f'/f0)^{(9-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_10,    /**< noise moments: \f$momJ_10(f) = \int_{f0}^f (f'/f0)^{(10-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_11,    /**< noise moments: \f$momJ_11(f) = \int_{f0}^f (f'/f0)^{(11-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_12,    /**< noise moments: \f$momJ_12(f) = \int_{f0}^f (f'/f0)^{(12-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_13,    /**< noise moments: \f$momJ_13(f) = \int_{f0}^f (f'/f0)^{(13-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_14,    /**< noise moments: \f$momJ_14(f) = \int_{f0}^f (f'/f0)^{(14-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momK_10,    /**< noise moments: \f$momK_14(f) = \int_{f0}^f (f'/f0)^{(14-17)/3} log(f'/f0) log(f'/f0) df'\f$ */
    REAL8Vector *momK_11,    /**< noise moments: \f$momK_15(f) = \int_{f0}^f (f'/f0)^{(15-17)/3} log(f'/f0) log(f'/f0) df'\f$ */      
    REAL8Vector *momK_12     /**< noise moments: \f$momK_16(f) = \int_{f0}^f (f'/f0)^{(16-17)/3} log(f'/f0) log(f'/f0) df'\f$ */
);

int XLALSimInspiralTaylorF2RedSpinComputeNoiseMoments(
    REAL8Vector *momI_0,    /**< noise moments: \f$momI_0(f) = \int_{f0}^f (f'/f0)^{(0-17)/3} df'\f$ */
    REAL8Vector *momI_2,    /**< noise moments: \f$momI_2(f) = \int_{f0}^f (f'/f0)^{(2-17)/3} df'\f$ */
    REAL8Vector *momI_3,    /**< noise moments: \f$momI_3(f) = \int_{f0}^f (f'/f0)^{(3-17)/3} df'\f$ */
    REAL8Vector *momI_4,    /**< noise moments: \f$momI_4(f) = \int_{f0}^f (f'/f0)^{(4-17)/3} df'\f$ */
    REAL8Vector *momI_5,    /**< noise moments: \f$momI_5(f) = \int_{f0}^f (f'/f0)^{(5-17)/3} df'\f$ */
    REAL8Vector *momI_6,    /**< noise moments: \f$momI_6(f) = \int_{f0}^f (f'/f0)^{(6-17)/3} df'\f$ */
    REAL8Vector *momI_7,    /**< noise moments: \f$momI_7(f) = \int_{f0}^f (f'/f0)^{(7-17)/3} df'\f$ */
    REAL8Vector *momI_8,    /**< noise moments: \f$momI_8(f) = \int_{f0}^f (f'/f0)^{(8-17)/3} df'\f$ */
    REAL8Vector *momI_9,    /**< noise moments: \f$momI_9(f) = \int_{f0}^f (f'/f0)^{(9-17)/3} df'\f$ */
    REAL8Vector *momI_10,   /**< noise moments: \f$momI_10(f) = \int_{f0}^f (f'/f0)^{(10-17)/3} df'\f$ */
    REAL8Vector *momI_11,   /**< noise moments: \f$momI_11(f) = \int_{f0}^f (f'/f0)^{(11-17)/3} df'\f$ */
    REAL8Vector *momI_12,   /**< noise moments: \f$momI_12(f) = \int_{f0}^f (f'/f0)^{(12-17)/3} df'\f$ */
    REAL8Vector *momI_13,   /**< noise moments: \f$momI_13(f) = \int_{f0}^f (f'/f0)^{(13-17)/3} df'\f$ */
    REAL8Vector *momI_14,   /**< noise moments: \f$momI_14(f) = \int_{f0}^f (f'/f0)^{(14-17)/3} df'\f$ */
    REAL8Vector *momI_15,   /**< noise moments: \f$momI_15(f) = \int_{f0}^f (f'/f0)^{(15-17)/3} df'\f$ */
    REAL8Vector *momI_16,   /**< noise moments: \f$momI_16(f) = \int_{f0}^f (f'/f0)^{(16-17)/3} df'\f$ */
    REAL8Vector *momJ_5,    /**< noise moments: \f$momJ_5(f) = \int_{f0}^f (f'/f0)^{(5-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_6,    /**< noise moments: \f$momJ_6(f) = \int_{f0}^f (f'/f0)^{(6-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_7,    /**< noise moments: \f$momJ_7(f) = \int_{f0}^f (f'/f0)^{(7-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_8,    /**< noise moments: \f$momJ_8(f) = \int_{f0}^f (f'/f0)^{(8-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_9,    /**< noise moments: \f$momJ_9(f) = \int_{f0}^f (f'/f0)^{(9-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_10,   /**< noise moments: \f$momJ_10(f) = \int_{f0}^f (f'/f0)^{(10-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_11,   /**< noise moments: \f$momJ_11(f) = \int_{f0}^f (f'/f0)^{(11-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_12,   /**< noise moments: \f$momJ_12(f) = \int_{f0}^f (f'/f0)^{(12-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_13,   /**< noise moments: \f$momJ_13(f) = \int_{f0}^f (f'/f0)^{(13-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momJ_14,   /**< noise moments: \f$momJ_14(f) = \int_{f0}^f (f'/f0)^{(14-17)/3} log(f'/f0) df'\f$ */
    REAL8Vector *momK_10,   /**< noise moments: \f$momK_10(f) = \int_{f0}^f (f'/f0)^{(10-17)/3} log(f'/f0) log(f'/f0) df'\f$ */
    REAL8Vector *momK_11,   /**< noise moments: \f$momK_11(f) = \int_{f0}^f (f'/f0)^{(11-17)/3} log(f'/f0) log(f'/f0) df'\f$ */
    REAL8Vector *momK_12,   /**< noise moments: \f$momK_12(f) = \int_{f0}^f (f'/f0)^{(12-17)/3} log(f'/f0) log(f'/f0) df'\f$ */
    REAL8Vector *Sh,         /**< one sided PSD of the detector noise: Sh(f) for f = [fLow, fNyq] */
    REAL8 fLow,             /**< low frequency cutoff (Hz) */
    REAL8 df
);

/* compute theta0, theta3, theta3s from mc, eta, chi */
void XLALSimInspiralTaylorF2RedSpinChirpTimesFromMchirpEtaChi(
    double *theta0, /**< dimensionless parameter related to the chirp time by theta0 = 2 pi fLow tau0 */
    double *theta3, /**< dimensionless parameter related to the chirp time by theta3 = -2 pi fLow tau3 */
    double *theta3s,/**< dimensionless parameter related to the chirp time by theta3s = 2 pi fLow tau3s */
    double mc,      /**< chirp mass (M_sun) */
    double eta,     /**< symmetric mass ratio  */
    double chi,     /**< reduced-spin parameter */
    double fLow     /**< low-frequency cutoff (Hz) */
);

/* compute mc, eta, chi from theta0, theta3, theta3s */
void XLALSimInspiralTaylorF2RedSpinMchirpEtaChiFromChirpTimes(
    double *mc,     /**< chirp mass (M_sun) */
    double *eta,    /**< symmetric mass ratio  */
    double *chi,    /**< reduced-spin parameter */
    double theta0,  /**< dimensionless parameter related to the chirp time by theta0 = 2 pi fLow tau0 */
    double theta3,  /**< dimensionless parameter related to the chirp time by theta3 = -2 pi fLow tau3 */
    double theta3s, /**< dimensionless parameter related to the chirp time by theta3s = 2 pi fLow tau3s */
    double fLow     /**< low-frequency cutoff (Hz) */
);

typedef enum {

   LAL_SIM_INSPIRAL_SPINLESS, /** These approximants cannot include spin terms */
   LAL_SIM_INSPIRAL_SINGLESPIN, /** These approximants support a signle spin (by default that is the object 1)*/
   LAL_SIM_INSPIRAL_ALIGNEDSPIN, /** These approximants can include spins aligned with L_N */
   LAL_SIM_INSPIRAL_PRECESSINGSPIN, /** These approximant support fully precessing spins */
   LAL_SIM_INSPIRAL_NUMSPINSUPPORT	/**< Number of elements in enum, useful for checking bounds */

 } SpinSupport;

/* check if the given approximant supports precessing spins */

int XLALSimInspiralGetSpinSupportFromApproximant(Approximant approx);


typedef enum {
  LAL_SIM_INSPIRAL_NO_TESTGR_PARAMS,   /** These approximants cannot accept testGR params as input params */
  LAL_SIM_INSPIRAL_TESTGR_PARAMS,      /** These approximants accept testGR params as input params */
  LAL_SIM_INSPIRAL_NUM_TESTGR_ACCEPT  /**< Number of elements in enum, useful for checking bounds */
 } TestGRaccept;

/* check if the given approximant accepts testGRparams */

int XLALSimInspiralApproximantAcceptTestGRParams(Approximant approx);

#if 0
{ /* so that editors will match succeeding brace */
#elif defined(__cplusplus)
}
#endif

#endif /* _LALSIMINSPIRAL_H */
