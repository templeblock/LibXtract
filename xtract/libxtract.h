/* libxtract feature extraction library
 *  
 * Copyright (C) 2006 Jamie Bullock
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, 
 * USA.
 */

/** \mainpage 
  *
  * LibXtract is a simple, portable, lightweight library of audio feature extraction functions. The purpose of the library is to provide a relatively exhaustive set of feature extraction primatives that are designed to be 'cascaded' to create a extraction hierarchies. 
  * For example, 'variance', 'average deviation', 'skewness' and 'kurtosis', all require the 'mean' of the input vector to be precomputed. However, rather than compute the 'mean' 'inside' each function, it is expected that the 'mean' will be passed in as an argument. This means that if the user wishes to use all of these features, the mean is calculated only once, and then passed to any functions that require it.
  * 
  * This philosophy of 'cascading' features is followed throughout the library, for example with features that operate on the magnitude spectrum of a signal vector (e.g. 'irregularity'), the magnitude spectrum is not calculated 'inside' the respective function, instead, a pointer to the first element in an array containing the magnitude spectrum is passed in as an argument. 
  *
  * Hopefully this not only makes the library more efficient when computing large numbers of features, but also makes it more flexible because extraction functions can be combined arbitrarily (one can take the irregularility of the Mel Frequency Cepstral Coefficients for example). 
  *
  * All feature extraction functions follow the same prototype:
  *
int xtract_function_name(const float *data, const int N, const void *argv, float *result){
  *
  * \param const float *data points to an array of floats representing the input data
  * \param const int N represents the number of elementes from *data to be considered in the calculation
  * \param const void *argv represents an arbitrary list of arguments. Used to pass in values required by the feature calculation
  * \param float *result points to an array of floats, or a single float represnting the result of the calculation
  *
  *
  * It is up to the calling function to allocate enough memory for the *data, *argv, and *result, and to free it when required. Some feature extraction functions may also require an _init() function to be called in order to perform some initialisation. The struct xtract_function_descriptor_t is used to give an indication of recommended default values, and argc for the *argv array.
  *
  * LibXtract can be downloaded from http://www.sf.net/projects/libxtract
  *
  */

#ifndef XTRACT_H
#define XTRACT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file libxtract.h 
 * \brief main header file and API definition 
 */
    
#include "xtract_scalar.h"
#include "xtract_vector.h"
#include "xtract_delta.h"
#include "xtract_types.h"
#include "xtract_macros.h"
#include "xtract_helper.h"

/** \defgroup libxtract API
  *
  * Defines a very simple API that provides access to the functions in the library
  * @{
  */

#define XTRACT_FEATURES 59
    
/** \brief Enumeration of features, elements are used as indixes to an array of pointers to feature extracton functions */
enum xtract_features_ {
    XTRACT_MEAN,
    XTRACT_VARIANCE,
    XTRACT_STANDARD_DEVIATION,
    XTRACT_AVERAGE_DEVIATION,
    XTRACT_SKEWNESS,
    XTRACT_KURTOSIS,
    XTRACT_SPECTRAL_MEAN,
    XTRACT_SPECTRAL_VARIANCE,
    XTRACT_SPECTRAL_STANDARD_DEVIATION,
    XTRACT_SPECTRAL_AVERAGE_DEVIATION,
    XTRACT_SPECTRAL_SKEWNESS,
    XTRACT_SPECTRAL_KURTOSIS,
    XTRACT_SPECTRAL_CENTROID,
    XTRACT_IRREGULARITY_K,
    XTRACT_IRREGULARITY_J,
    XTRACT_TRISTIMULUS_1,
    XTRACT_TRISTIMULUS_2,
    XTRACT_TRISTIMULUS_3,
    XTRACT_SMOOTHNESS,
    XTRACT_SPREAD,
    XTRACT_ZCR,
    XTRACT_ROLLOFF,
    XTRACT_LOUDNESS,
    XTRACT_FLATNESS,
    XTRACT_FLATNESS_DB,
    XTRACT_TONALITY,
    XTRACT_CREST,
    XTRACT_NOISINESS,
    XTRACT_RMS_AMPLITUDE,
    XTRACT_SPECTRAL_INHARMONICITY,
    XTRACT_POWER,
    XTRACT_ODD_EVEN_RATIO,
    XTRACT_SHARPNESS,
    XTRACT_SPECTRAL_SLOPE,
    XTRACT_LOWEST_VALUE,
    XTRACT_HIGHEST_VALUE,
    XTRACT_SUM,
    XTRACT_NONZERO_COUNT,
    XTRACT_HPS,
    XTRACT_F0,
    XTRACT_FAILSAFE_F0,
    XTRACT_LNORM,
    XTRACT_FLUX,
    XTRACT_ATTACK_TIME,
    XTRACT_DECAY_TIME,
    XTRACT_DIFFERENCE_VECTOR,
    XTRACT_AUTOCORRELATION,
    XTRACT_AMDF,
    XTRACT_ASDF,
    XTRACT_BARK_COEFFICIENTS,
    XTRACT_PEAK_SPECTRUM,
    XTRACT_SPECTRUM,
    XTRACT_AUTOCORRELATION_FFT,
    XTRACT_MFCC,
    XTRACT_DCT,
    XTRACT_HARMONIC_SPECTRUM,
    XTRACT_LPC,
    XTRACT_LPCC,
    /* Helper functions */
    XTRACT_WINDOWED
};

/** \brief Enumeration of feature initialisation functions */
enum xtract_feature_init_ {
    XTRACT_INIT_MFCC = 100,
    XTRACT_INIT_BARK,
    XTRACT_INIT_WINDOWED
};

/** \brief Enumeration of feature types */
enum xtract_feature_types_ {
    XTRACT_SCALAR,
    XTRACT_VECTOR,
    XTRACT_DELTA
};

/** \brief Enumeration of mfcc types */
enum xtract_mfcc_types_ {
    XTRACT_EQUAL_GAIN,
    XTRACT_EQUAL_AREA
};

enum xtract_lnorm_filter_types_ {
    XTRACT_NO_LNORM_FILTER,
    XTRACT_POSITIVE_SLOPE,
    XTRACT_NEGATIVE_SLOPE
};

/** \brief Enumeration of return codes */
enum xtract_return_codes_ {
    XTRACT_SUCCESS,
    XTRACT_MALLOC_FAILED,
    XTRACT_BAD_ARGV,
    XTRACT_BAD_VECTOR_SIZE,
    XTRACT_DENORMAL_FOUND,
    XTRACT_NO_RESULT, /* This usually occurs when the correct calculation cannot take place because required data is missing or would result in a NaN or infinity/-infinity. Under these curcumstances 0.f is usually given by *result */
    XTRACT_FEATURE_NOT_IMPLEMENTED
};

/** \brief Enumeration of spectrum types */
enum xtract_spectrum_ {
    XTRACT_MAGNITUDE_SPECTRUM,
    XTRACT_LOG_MAGNITUDE_SPECTRUM,
    XTRACT_POWER_SPECTRUM,
    XTRACT_LOG_POWER_SPECTRUM
};

/** \brief Enumeration of data types*/
typedef enum type_ {
    XTRACT_FLOAT,
    XTRACT_FLOATARRAY,
    XTRACT_INT,
    XTRACT_MEL_FILTER
} xtract_type_t;

/** \brief Enumeration of units*/
typedef enum unit_ {
    /* NONE, ANY */
    XTRACT_HERTZ = 2,
    XTRACT_ANY_AMPLITUDE_HERTZ,
    XTRACT_DBFS,
    XTRACT_DBFS_HERTZ,
    XTRACT_PERCENT,
    XTRACT_SONE
} xtract_unit_t;

/** \brief Boolean */
typedef enum {
    XTRACT_FALSE,
    XTRACT_TRUE
} xtract_bool_t;

/** \brief Window types */
enum xtract_window_types_ {
    XTRACT_GAUSS,
    XTRACT_HAMMING,
    XTRACT_HANN,
    XTRACT_BARTLETT,
    XTRACT_TRIANGULAR,
    XTRACT_BARTLETT_HANN,
    XTRACT_BLACKMAN,
    XTRACT_KAISER,
    XTRACT_BLACKMAN_HARRIS
};

/** \brief Enumeration of vector format types*/
typedef enum xtract_vector_ {
    /* N/2 magnitude/log-magnitude/power/log-power coeffs and N/2 frequencies */
    XTRACT_SPECTRAL,     
    /* N spectral amplitudes */
    XTRACT_SPECTRAL_MAGNITUDES, 
    /* N/2 magnitude/log-magnitude/power/log-power peak coeffs and N/2 
     * frequencies */
    XTRACT_SPECTRAL_PEAKS,
    /* N spectral peak amplitudes */
    XTRACT_SPECTRAL_PEAKS_MAGNITUDES,
    /* N spectral peak frequencies */
    XTRACT_SPECTRAL_PEAKS_FREQUENCIES,
    /* N/2 magnitude/log-magnitude/power/log-power harmonic peak coeffs and N/2 
     * frequencies */
    XTRACT_SPECTRAL_HARMONICS,
    /* N spectral harmonic amplitudes */
    XTRACT_SPECTRAL_HARMONICS_MAGNITUDES,
    /* N spectral harmonic frequencies */
    XTRACT_SPECTRAL_HARMONICS_FREQUENCIES,
    XTRACT_AUTOCORRELATION_COEFFS,
    XTRACT_ARBITRARY_SERIES,
    XTRACT_AUDIO_SAMPLES,
    XTRACT_MEL_COEFFS, 
    XTRACT_LPC_COEFFS, 
    XTRACT_LPCC_COEFFS, 
    XTRACT_BARK_COEFFS,
    XTRACT_SUBFRAMES,
    XTRACT_NO_DATA
} xtract_vector_t;

/** \brief Data structure containing useful information about functions provided by LibXtract. */
typedef struct _xtract_function_descriptor {

    int id;

    struct {
	char name[XTRACT_MAX_NAME_LENGTH];
	char p_name[XTRACT_MAX_NAME_LENGTH]; /* pretty name */
	char desc[XTRACT_MAX_DESC_LENGTH];
	char p_desc[XTRACT_MAX_DESC_LENGTH]; /* pretty description */
	char author[XTRACT_MAX_AUTHOR_LENGTH];
	int year;
    } algo;

    struct {
	xtract_vector_t format;
	xtract_unit_t unit;
    } data;

    int argc;

    struct {
	xtract_type_t type; /* type of the array/value pointed to by argv */
	float min[XTRACT_MAXARGS];
	float max[XTRACT_MAXARGS];
	float def[XTRACT_MAXARGS]; /* defaults */
	xtract_unit_t unit[XTRACT_MAXARGS];
	int donor[XTRACT_MAXARGS]; /* suggested donor functions for argv */
    } argv;

    xtract_bool_t is_scalar;
    xtract_bool_t is_delta; /* features in xtract_delta.h can be scalar or vector */ 

    /* The result.<> entries in descritors.c need to be checked */
    union {

	struct {
	    float min;
	    float max;	   
	    xtract_unit_t unit;
	} scalar;

	struct {
	    xtract_vector_t format;
	    xtract_unit_t unit;
	} vector;

    } result;

} xtract_function_descriptor_t;

/**
 *                                                                          
 * \brief An array of pointers to functions that perform the extraction
 *                                                                          
 * \param *data: a pointer to the start of the input data (usually the first element in an array)                         
 *                                                                          
 * \param N: the number of elements to be processed          
 *                                                                          
 * \param *argv: an abitrary number of additional arguments, used to pass additional parameters to the function being called. All arguments are compulsary!
 *                                                                          
 * \param *result: a pointer to the first element in the result                              
 *                                                                          
 * Each function will iterate over N array elements, the first of which is  
 * pointed to by *data. It is up to the calling function to ensure that the array is in the format expected by the function being called.
 *                                                                          
 * For scalar and delta features, *result will point to a single value.     
 *                                                                          
 * For vector features it will point to the first element in an array.      
 *                                                                          
 * Memory for this array must be allocated and freed by the calling         
 * function.                                                                
 *  
 * All functions return an integer error code as descibed in the enumeration
 * return_codes_
 *
 * The preprocessor macro: XTRACT must be defined before  this  can be used
 * 
 * example:<br>
 * \verbatim
#include <stdio.h>
#define XTRACT
#include "libxtract.h"

main () {
float values[] = {1.0, 2.0, 3.0, 4.0, 5.0};
int N = 5;
float mean;

xtract[MEAN]((void *)values, N, NULL, &mean);

printf("Mean = %.2f\n", mean);
}
\endverbatim
 * The calling function may additionally make some tests against the value returned by xtract
 * 
 */
#ifdef XTRACT_H
extern int(*xtract[XTRACT_FEATURES])(const float *data, const int N, const void *argv, float *result);

#endif

/** \brief A structure to store a set of n_filters Mel filters */
typedef struct xtract_mel_filter_ {
    int n_filters;
    float **filters;
} xtract_mel_filter;

/** \brief A function to initialise a mel filter bank 
 * 
 * It is up to the caller to pass in a pointer to memory allocated for freq_bands arrays of length N. This function populates these arrays with magnitude coefficients representing the mel filterbank on a linear scale 
 */
int xtract_init_mfcc(int N, float nyquist, int style, float freq_min, float freq_max, int freq_bands, float **fft_tables);

/** \brief A function to initialise bark filter bounds
 * 
 * A pointer to an array of BARK_BANDS ints most be passed in, and is populated with BARK_BANDS fft bin numbers representing the limits of each band 
 *
 * \param N: the audio block size
 * \param sr: The sample audio sample rate
 * \param *band_limits: a pointer to an array of BARK_BANDS ints
 */
int xtract_init_bark(int N, float sr, int *band_limits);

/** \brief An initialisation function for functions using FFT
 *
 * This function initialises global data structures used by functions requiring FFT functionality. It can be called multiple times with different feature names. Calling it more than once with the same feature name is not a valid operation and will result in a memory leak.
 *
 * \param N: the size of the FFT
 * \param feature_name: the name of the feature the FFT is being used for, 
 * e.g. XTRACT_DCT
 *
 */
int xtract_init_fft(int N, int feature_name);

/** \brief Free memory used for fft plans
 *
 * This function should be used to explicitly free memory allocated for ffts by xtract_init_fft(). It is primarily intended for use if a new FFT needs to be taken with a different blocksize. If only one fft size is required then there is no need to call this function since it will be called when the program exits. 
 * */
void xtract_free_fft(void);

/** \brief Make a window of a given type and return a pointer to it
 *
 * \param N: the size of the window
 * \param type: the type of the window as given in the enumeration window_types_
 *
 */
float *xtract_init_window(const int N, const int type);

/** \brief Free a window as allocated by xtract_make_window() 
 * 
 * \param *window: a pointer to an array of floats as allocated by xtract_make_window()
 *
 */
void xtract_free_window(float *window);

/* \brief A function to build an array of function descriptors */
xtract_function_descriptor_t *xtract_make_descriptors();

/* \brief A function to free an array of function descriptors */
int xtract_free_descriptors(xtract_function_descriptor_t *fd);
/* Free functions */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
