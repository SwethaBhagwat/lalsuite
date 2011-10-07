/*
 * Copyright (C) 2005 Reinhard Prix
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

/**
 * \author Reinhard Prix, Badri Krishnan
 * \date 2005
 * \file
 * \ingroup SFTfileIO
 * \brief Utility functions for handling of SFTtype and SFTVectors
 *
 * $Id$
 *
 *  The helper functions LALCreateSFTtype(), LALDestroySFTtype(), LALCreateSFTVector()
 * and LALDestroySFTVector() respectively allocate and free SFT-structs and SFT-vectors.
 * Similarly, LALCreateTimestampVector() and LALDestroyTimestampVector() allocate and free
 * a bunch of GPS-timestamps.
 *
 */

#ifndef _SFTUTILS_H  /* Double-include protection. */
#define _SFTUTILS_H

/* remove SWIG interface directives */
#if !defined(SWIG) && !defined(SWIGLAL_STRUCT)
#define SWIGLAL_STRUCT(...)
#endif
#if !defined(SWIG) && !defined(SWIGLAL_DYNAMIC_1DARRAY_BEGIN)
#define SWIGLAL_DYNAMIC_1DARRAY_BEGIN(...)
#define SWIGLAL_DYNAMIC_1DARRAY_END(...)
#endif
#if !defined(SWIG) && !defined(SWIGLAL_DYNAMIC_2DARRAY_BEGIN)
#define SWIGLAL_DYNAMIC_2DARRAY_BEGIN(...)
#define SWIGLAL_DYNAMIC_2DARRAY_END(...)
#endif

/* C++ protection. */
#ifdef  __cplusplus
extern "C" {
#endif

NRCSID( SFTUTILSH, "$Id$" );
/*---------- INCLUDES ----------*/
#include <stdarg.h>

#include <lal/LALDatatypes.h>
#include <lal/DetectorSite.h>
#include <lal/Date.h>
#include <lal/SkyCoordinates.h>
#include <lal/RngMedBias.h>
#include <lal/LALRunningMedian.h>




/*---------- DEFINES ----------*/

/*----- Error-codes -----*/

#define SFTUTILS_ENULL 		1
#define SFTUTILS_ENONULL	2
#define SFTUTILS_EMEM		3
#define SFTUTILS_EINPUT		4
#define SFTUTILS_EFUNC		6

#define SFTUTILS_MSGENULL 	"Arguments contained an unexpected null pointer"
#define SFTUTILS_MSGENONULL	"Output pointer is not NULL"
#define SFTUTILS_MSGEMEM	"Out of memory"
#define SFTUTILS_MSGEINPUT	"Invald input parameter"
#define SFTUTILS_MSGEFUNC	"Sub-routine failed"

/*---------- exported types ----------*/

/** A vector of COMPLEX8FrequencySeries */
typedef struct tagCOMPLEX8FrequencySeriesVector {
  SWIGLAL_STRUCT(COMPLEX8FrequencySeriesVector);
  SWIGLAL_DYNAMIC_1DARRAY_BEGIN(COMPLEX8FrequencySeries, data, length);
  UINT4 			length;		/**< number of SFTs */
  COMPLEX8FrequencySeries 	*data;		/**< array of SFTs */
  SWIGLAL_DYNAMIC_1DARRAY_END(COMPLEX8FrequencySeries, data, length);
} COMPLEX8FrequencySeriesVector;

/** A vector of REAL8FrequencySeries */
typedef struct tagREAL8FrequencySeriesVector {
  SWIGLAL_STRUCT(REAL8FrequencySeriesVector);
  SWIGLAL_DYNAMIC_1DARRAY_BEGIN(REAL8FrequencySeries, data, length);
  UINT4                  length;
  REAL8FrequencySeries   *data;
  SWIGLAL_DYNAMIC_1DARRAY_END(REAL8FrequencySeries, data, length);
} REAL8FrequencySeriesVector;

/** A vector of REAL4FrequencySeries */
typedef struct tagREAL4FrequencySeriesVector {
  SWIGLAL_STRUCT(REAL4FrequencySeriesVector);
  SWIGLAL_DYNAMIC_1DARRAY_BEGIN(REAL4FrequencySeries, data, length);
  UINT4                  length;
  REAL4FrequencySeries   *data;
  SWIGLAL_DYNAMIC_1DARRAY_END(REAL4FrequencySeries, data, length);
} REAL4FrequencySeriesVector;


/** A so-called 'SFT' (short-Fourier-transform) will be stored in a COMPLEX8FrequencySeries */
typedef COMPLEX8FrequencySeries 	SFTtype;


/** The corresponding vector-type to hold a vector of 'SFTs' */
typedef COMPLEX8FrequencySeriesVector 	SFTVector;

/** Special type for holding a PSD vector (over several SFTs) */
typedef REAL8FrequencySeriesVector PSDVector;

/** A collection of SFT vectors -- one for each IFO in a multi-IFO search */
typedef struct tagMultiSFTVector {
  SWIGLAL_STRUCT(MultiSFTVector);
  SWIGLAL_DYNAMIC_1DARRAY_BEGIN(SFTVector*, data, length);
  UINT4      length;  	/**< number of ifos */
  SFTVector  **data; 	/**< sftvector for each ifo */
  SWIGLAL_DYNAMIC_1DARRAY_END(SFTVector*, data, length);
} MultiSFTVector;


/** A collection of PSD vectors -- one for each IFO in a multi-IFO search */
typedef struct tagMultiPSDVector {
  SWIGLAL_STRUCT(MultiPSDVector);
  SWIGLAL_DYNAMIC_1DARRAY_BEGIN(PSDVector*, data, length);
  UINT4      length;  	/**< number of ifos */
  PSDVector  **data; 	/**< sftvector for each ifo */
  SWIGLAL_DYNAMIC_1DARRAY_END(PSDVector*, data, length);
} MultiPSDVector;

/** One noise-weight (number) per SFT (therefore indexed over IFOs and SFTs */
typedef struct tagMultiNoiseWeights {
  SWIGLAL_STRUCT(MultiNoiseWeights);
  SWIGLAL_DYNAMIC_1DARRAY_BEGIN(REAL8Vector*, data, length);
  UINT4 length;		/**< number of ifos */
  REAL8Vector **data;	/**< weights-vector for each SFTs */
  SWIGLAL_DYNAMIC_1DARRAY_END(REAL8Vector*, data, length);
  REAL8 Sinv_Tsft;	/**< normalization factor used: \f$\mathcal{S}^{-1}\,T_\mathrm{SFT}\f$ (using single-sided PSD!) */
} MultiNoiseWeights;

/** A collection of (multi-IFO) time-series */
typedef struct tagMultiREAL4TimeSeries {
  SWIGLAL_STRUCT(MultiREAL4TimeSeries);
  SWIGLAL_DYNAMIC_1DARRAY_BEGIN(REAL4TimeSeries*, data, length);
  UINT4 length;			/**< number of ifos */
  REAL4TimeSeries **data;	/**< vector of REAL4 timeseries */
  SWIGLAL_DYNAMIC_1DARRAY_END(REAL4TimeSeries*, data, length);
} MultiREAL4TimeSeries;

/** A vector of 'timestamps' of type LIGOTimeGPS */
typedef struct tagLIGOTimeGPSVector {
  SWIGLAL_STRUCT(LIGOTimeGPSVector);
  SWIGLAL_DYNAMIC_1DARRAY_BEGIN(LIGOTimeGPS, data, length);
  UINT4 	length;		/**< number of timestamps */
  LIGOTimeGPS 	*data;		/**< array of timestamps */
  SWIGLAL_DYNAMIC_1DARRAY_END(LIGOTimeGPS, data, length);
  REAL8		deltaT;		/**< 'length' of each timestamp (e.g. typically Tsft) */
} LIGOTimeGPSVector;

/** A vector of 'timestamps' of type LIGOTimeGPS */
typedef struct tagMultiLIGOTimeGPSVector {
  SWIGLAL_STRUCT(MultiLIGOTimeGPSVector);
  SWIGLAL_DYNAMIC_1DARRAY_BEGIN(LIGOTimeGPSVector*, data, length);
  UINT4 	        length;	   /**< number of timestamps vectors or ifos */
  LIGOTimeGPSVector 	**data;    /**< timestamps vector for each ifo */
  SWIGLAL_DYNAMIC_1DARRAY_END(LIGOTimeGPSVector*, data, length);
} MultiLIGOTimeGPSVector;

/*---------- Global variables ----------*/
/* empty init-structs for the types defined in here */
extern const SFTtype empty_SFTtype;
extern const SFTVector empty_SFTVector;
extern const PSDVector empty_PSDVector;
extern const MultiSFTVector empty_MultiSFTVector;
extern const MultiPSDVector empty_MultiPSDVector;
extern const MultiNoiseWeights empty_MultiNoiseWeights;
extern const MultiREAL4TimeSeries empty_MultiREAL4TimeSeries;
extern const LIGOTimeGPSVector empty_LIGOTimeGPSVector;
extern const MultiLIGOTimeGPSVector empty_MultiLIGOTimeGPSVector;

/*---------- exported prototypes [API] ----------*/
/* ----------------------------------------------------------------------
 *  some prototypes for general functions handling these data-types
 *----------------------------------------------------------------------*/
void LALCreateSFTtype (LALStatus *status, SFTtype **sft, UINT4 SFTlen);
void LALCreateSFTVector (LALStatus *status, SFTVector **sftvect, UINT4 numSFTs, UINT4 SFTlen);
void LALCreateMultiSFTVector ( LALStatus *status, MultiSFTVector **out, UINT4 length, UINT4Vector *numsft );

SFTVector* XLALCreateSFTVector (UINT4 numSFTs, UINT4 numBins );
SFTtype* XLALCreateSFT ( UINT4 numBins );

void XLALDestroySFTVector (SFTVector *vect);
void XLALDestroySFT (SFTtype *sft);


COMPLEX8Vector *XLALrefineCOMPLEX8Vector (const COMPLEX8Vector *in, UINT4 refineby, UINT4 Dterms);
void upsampleMultiSFTVector (LALStatus *, MultiSFTVector *inout, UINT4 upsample, UINT4 Dterms);
void upsampleSFTVector (LALStatus *, SFTVector *inout, UINT4 upsample, UINT4 Dterms);

void LALDestroySFTtype (LALStatus *status, SFTtype **sft);
void LALDestroySFTVector (LALStatus *status, SFTVector **sftvect);
void LALDestroyPSDVector (LALStatus *status, PSDVector **vect);

void LALDestroyMultiSFTVector (LALStatus *status, MultiSFTVector **multvect);
void LALDestroyMultiPSDVector (LALStatus *status, MultiPSDVector **multvect);

SFTVector* XLALExtractBandfromSFTs ( const SFTVector *sfts, REAL8 fMin, REAL8 fMax );
void LALCopySFT (LALStatus *status, SFTtype *dest, const SFTtype *src);

void LALSubtractSFTVectors (LALStatus *, SFTVector **outVect, const SFTVector *inVect1, const SFTVector *inVect2 );
void LALLinearlyCombineSFTVectors (LALStatus *, SFTVector **outVect, SFTVector **inVects, const COMPLEX16Vector *weights, const CHAR *outName);
void LALAppendSFT2Vector (LALStatus *, SFTVector *vect, const SFTtype *sft );

LIGOTimeGPSVector *XLALCreateTimestampVector (UINT4 len);
void XLALDestroyTimestampVector (LIGOTimeGPSVector *vect);

void LALCreateTimestampVector (LALStatus *status, LIGOTimeGPSVector **vect, UINT4 len);
void LALDestroyTimestampVector (LALStatus *status, LIGOTimeGPSVector **vect);

void LALMakeTimestamps (LALStatus *, LIGOTimeGPSVector **timestamps, const LIGOTimeGPS tStart, REAL8 duration, REAL8 Tsft);

LIGOTimeGPSVector *XLALExtractTimestampsFromSFTs ( const SFTVector *sfts );
MultiLIGOTimeGPSVector *XLALExtractMultiTimestampsFromSFTs ( const MultiSFTVector *multiSFTs );
void XLALDestroyMultiTimestamps ( MultiLIGOTimeGPSVector *multiTS );

CHAR *XLALGetChannelPrefix ( const CHAR *name );
LALDetector *XLALGetSiteInfo ( const CHAR *name );

void LALComputeNoiseWeights  (LALStatus *status, REAL8Vector *weightV, const SFTVector *sftVect,
			      INT4 blkSize, UINT4 excludePercentile);
void LALComputeMultiNoiseWeights  (LALStatus *status, MultiNoiseWeights **weightsV, const MultiPSDVector *multipsd,
				   UINT4 blocksRngMed, UINT4 excludePercentile);
void LALDestroyMultiNoiseWeights  (LALStatus *status, MultiNoiseWeights **weights);


/* ============================================================
 * ===== deprecated LAL interface API ==========
 * ============================================================
 */

void LALGetSFTtimestamps (LALStatus *, LIGOTimeGPSVector **timestamps, const SFTVector *sfts );

#ifdef  __cplusplus
}
#endif
/* C++ protection. */

#endif  /* Double-include protection. */
