#ifndef _MOLUP_H
#define _MOLUP_H

/****************************************************************************\
**
** molup.h
**
** These file contains the main API functions that can be accessed directly 
** by the users of Mobile OLUP component.
**
** Copyright (c) 2004 Camo Technologies.
** All Rights Reserved
**
** Authors:
**	Zala VijaySinh Khumansinh
**
**
\****************************************************************************/

#include "molupcodes.h"

/**
 * molpFreeTMobileHandles
 * 
 * This function will Free TMobileHandles structre.
 *
 * @param pMobileHandles (i/o) - pointer of type TMobileHandles .
 *
 * returns - None
 **/
CAMO_DLL_EXPORT void molpFreeTMobileHandles( TObjHandle ohPred);


/**
 * molpGetMissingValue
 *
 * This function gets current representation for "missing" value
 *
 * @return MOLP_OK            - Success\n
 *         MOLPERR_INTERNAL   - Unknown handle specified\n
 *
 * @param pfMissValue [out] - Value indicating "missing"
 *
 */
CAMO_DLL_EXPORT UINT32 molpGetMissingValue( FLOAT32* pfMissValue );


/**
 * molpFree
 *
 * This function frees a memory block allocated by OLUP DLL or OLUP Shared Object
 *
 * @return MOLP_OK            - Success\n
 *         MOLPERR_INTERNAL   - Unknown handle specified\n
 *
 * @param pMem [in]           - Pointer to memory block
 *
 * Comment - The object must have been created with molpOpenPredictor
 */
CAMO_DLL_EXPORT UINT32 molpFree( TPointer pMem );


/**
 * Functions for Prediction management
 */

/* - Initialization & setup
 */

/**
 * molpOpenPredictor
 *
 * Create & initialize a new Predictor object. The Predictor object is used for setting 
 * up prediction parameters and performing the predictions.
 *
 * @returns - a handle for later reference to the Predictor; 0 if failed.
 */
CAMO_DLL_EXPORT  TObjHandle molpOpenPredictor( void );


/**
 * molpClosePredictor
 *
 * This function removes a Predictor object.

 *
 * @return MOLP_OK - Success\n
 *         MOLPERR_BADHANDLE - Unknown handle specified\n
 *
 * @param ohHandle [in]      - Handle of Predictor to remove
 *
 * Comment - The object must have been created with molpOpenPredictor.
 */
CAMO_DLL_EXPORT UINT32 molpClosePredictor( TObjHandle ohHandle );


/**
 * molpSetOptions
 *
 * Sets new calculation options for a Predictor object.
 *
 * @return  MOLP_OK                 - Success\n
 *          MOLPERR_NULL_POINTER    - Parameter pOptions was NULL\n
 *          MOLPERR_INVALID_VALUE   - Bad option value specified\n
 *          MOLPERR_NOT_SUPPORTED   - Pretreatments set to FALSE\n
 *
 * @param ohPred     [in] - Handle of Predictor to modify
 * @param pOptions   [in] - Ptr. to new options
 *
 * Comment - Current version of OLUP always performs pretreatments, and the option cannot be changed.
 */
CAMO_DLL_EXPORT UINT32 molpSetOptions( TObjHandle ohPred, 
                                       TOptions *pOptions );


/**
 * molpGetOptions
 *
 * This function gets current calculation options for a Predictor object.
 *
 * @return MOLP_OK               - Success\n
 *         MOLPERR_BADHANDLE     - Unknown handle specified\n
 *         MOLPERR_NULL_POINTER  - Parameter pOptions was NULL\n
 *
 * @param ohPred     [in]  - Handle of Predictor to modify
 * @param pOptions   [out] - Ptr. to record for returned options
 *
 * Comment - The object must have been created with molpOpenPredictor.
 */
CAMO_DLL_EXPORT UINT32 molpGetOptions( TObjHandle ohPred, 
                                       TOptions *pOptions );


/* - Model selection
 */

/**
 * molpSelectModelFile
 *
 * This function selects model file for prediction
 *
 * @return MOLP_OK                           -  Success\n
 *         MOLPERR_NULL_POINTER              -  Parameter pOptions was NULL\n
 *         MOLPERR_INVALID_VALUE             -  Bad option value specified\n
 *         MOLPERR_NOT_SUPPORTED             -  Pretreatments set to FALSE\n
 *         MOLPERR_BADHANDLE                 -  Unknown handle specified\n
 *         MOLPERR_FILE_TYPE                 -  File extension not recognized (see above)\n
 *         MOLPERR_FILE                      -  Problem while reading file (bad file?)\n
 *         MOLPERR_UNSUPPORTED_PRETREATMENT  -  The PreTreatments apart from Norris, SGolay & MSC, considered as Invalid Type\n
 *         MOLPERR_VALUE_COUNT               -  Wrong number of values received\n
 *         MOLPERR_MODEL_FILE_NOT_SUPPORTED  -  Old Version cant be Supported\n
 *         MOLPERR_MSC_INFO_NOT_AVAILABLE    -  MSC Matrix not Available\n
 *
 * @param ohPred [in] - Handle of Predictor to modify
 * @param ohPred [in] - Filename (incl. path) of model. 
 *                      This must be terminated with an ASCII 0 (null) character.
 *
 * Comment - The object must have been created with molpOpenPredictor.
 */
CAMO_DLL_EXPORT UINT32 molpSelectModelFile( TObjHandle ohPred, 
                                            char* pszModelFileName );

/**
 * molpSetCompsUsed
 *
 * This function sets number of components to use in prediction. <nl>
 * The model must be selected before calling this function.
 *
 * @return MOLP_OK               - Success\n
 *         MOLPERR_BADHANDLE     - Unknown handle specified\n
 *         MOLPERR_INVALID_VALUE -  Bad option value specified\n
 *         MOLPERR_NO_MODEL      - No model has been selected (use molpSelectModel)\n 
 *
 * @param ohPred     [in] - Handle of Predictor to query
 * @param nCompsUsed [in] - Number of PCs to use
 *
 * Comment - Changing this values causes the currently selected model to be re-read from disk
 */
CAMO_DLL_EXPORT UINT32 molpSetCompsUsed( TObjHandle ohPred, 
                                         UINT32 nCompsUsed );


/**
 * molpGetSelectedModelFile
 *
 * This function gets name of current model file used for prediction.
 *
 * @return MOLP_OK            - Success\n
 *         MOLPERR_BADHANDLE  - Unknown handle specified\n
 *         MOLPERR_NO_MODEL   - The model has not yet been selected.\n
 *
 * @param ohPred           [in]  - Handle of Predictor to remove
 * @param pszModelFileName [out] - Filename (incl. path) of model
 *
 * Comment - The ModelFileName buffer must be large enough to hold the largest
 *           possible pathname string.
 */
CAMO_DLL_EXPORT UINT32 molpGetSelectedModelFile( TObjHandle ohPred, 
                                                 char* pszModelFileName );


/**
 * molpGetModelInfo
 *
 * This function gets info about current model used. 
 *
 * @return MOLP_OK               - Success\n
 *         MOLPERR_BADHANDLE     - Unknown handle specified\n
 *         MOLPERR_NO_MODEL      - The model has not yet been selected.\n
 *                                 (Content of TModelInfo buffer is undefined)\n 
 *         MOLPERR_NULL_POINTER  - Parameter pModelInfo was a NULL pointer.
 *
 * @param ohPred     [in]  - Handle of Predictor to remove
 * @param pModelInfo [out] - Ptr.to TModelInfo buffer where info about current model is returned.
 *
 * Comment - The object must have been created with molpOpenPredictor.
 */
CAMO_DLL_EXPORT UINT32 molpGetModelInfo( TObjHandle ohPred, 
                                         TModelInfo* pModelInfo );


/**
 * molpGetXVarName
 *
 * This function gets name of selected X variable no. according to currently selected model.
 *
 * @return MOLP_OK               - Success\n
 *         MOLPERR_BADHANDLE     - Unknown handle specified\n
 *         MOLPERR_TRUNCATED     - Truncated name returned in pszBuf\n
 *         MOLPERR_NULL_POINTER  - Parameter pszBuf was a NULL pointer\n
 *         MOLPERR_NO_MODEL      - No model has been selected\n
 *         MOLPERR_INVALID_VALUE - Invalid nVarNo selected (out of range)\n
 *
 * @param ohPred  [in]   - Handle of Predictor to query
 * @param nVarNo  [in]   - Variable no. to query
 * @param pszBuf  [out]  - Buffer for returned name
 * @param BufSize [in]   - Size of return buffer
 *
 * Comment - If specified buffer size is too small to hold the name, only the
 *           first (nBufSize-1) characters are returned. A special return status
 *           of MOLPERR_TRUNCATED is returned in this case.
 */
CAMO_DLL_EXPORT UINT32 molpGetXVarName( TObjHandle ohPred, 
                                        UINT32 nVarNo, 
                                        char* pszBuf, 
                                        UINT16 nBufSize );

/**
 * molpGetYVarName
 *
 * This function gets name of selected Y variable no. according to currently selected model.
 *
 * @return MOLP_OK               - Success\n
 *         MOLPERR_BADHANDLE     - Unknown handle specified\n
 *         MOLPERR_TRUNCATED     - Truncated name returned in pszBuf\n
 *         MOLPERR_NULL_POINTER  - Parameter pszBuf was a NULL pointer\n
 *         MOLPERR_NO_MODEL      - No model has been selected\n
 *         MOLPERR_INVALID_VALUE - Invalid nVarNo selected (out of range)\n
 *
 * @param ohPred  [in]  - Handle of Predictor to query
 * @param nVarNo  [in]  - Variable no. to query
 * @param pszBuf  [out] - Buffer for returned name
 * @param BufSize [in]  - Size of return buffer
 *
 * Comment - If specified buffer size is too small to hold the name, only the
 *           first (nBufSize-1) characters are returned. A special return status
 *           of MOLPERR_TRUNCATED is returned in this case.
 */
CAMO_DLL_EXPORT UINT32 molpGetYVarName( TObjHandle ohPred, 
                                        UINT32 nVarNo, 
                                        char* pszBuf, 
                                        UINT16 nBufSize );


/* - Prediction result retrieval
 */

/**
 * molpPredict
 *
 * This function performs prediction.
 *
 * @return MOLP_OK                          - Success\n
 *         MOLPERR_BADHANDLE                - Unknown handle specified\n
 *         MOLPERR_NULL_POINTER             - Parameter pDataMatrix was a NULL pointer\n
 *         MOLPERR_NO_MODEL                 - No model has been selected\n
 *         MOLPERR_NUM_PLANES               - Incorrect number of matrix planes\n
 *         MOLPERR_NUM_COLUMNS              - Incorrect number of sample variables (columns);
 *                                            This must correspond to total number of XVars
 *                                            in current model\n
 *         MOLPERR_NUM_ROWS                 - Incorrect number of matrix rows (other than 1 is
 *                                            not supported in current OLUP version)\n
 *         MOLPERR_OUTOFMEMORY              - Ran out of memory during the prediction or
 *                                            preparation for prediction\n
 *         MOLPERR_LESS_NUM_COLUMNS         - Number of columns < 5 to perform Norris derivation
 *         MOLPERR_LESS_SUM_AVG_POINTS      - Order must be less than or equal the sum of averaging points
 *         MOLPERR_DER_ORDER_EQL_POLY_ORDER - Derivation order must be less than or equal the polynomial order
 *         MOLPERR_OUTOFPOLY_ORDER          - The polynomial order must be between 0 and 12
 *         MOLPERR_NOT_LESS_POINTS_ORDER    - The sum of averaging points +1 must be less than this number
 *         MOLPERR_DIVIDE_BY_ZERO           - Divide by the zero
 *         MOLPERR_OUT_MSC_MODEL_TYPE       - The MSC model type must be between 1 and 3 \n
 *         MOLPERR_OUT_MSC_RESULT_TYPE      - The MSC result type must be between 1 and 2
 *         MOLPERR_NO_MSC_SOLUTION          - No MSC solution found
 *
 * @param ohPred        [in] - Handle of Predictor to remove
 * @param pDataMatrix   [in] - Pointer to matrix data record
 *
 * Comment - Currently, only 1 sample (NumRows=1) per prediction is allowed!
 */
CAMO_DLL_EXPORT UINT32 molpPredict( TObjHandle ohPred,
                                    const TMatrix *pDataMatrix );


/**
 * molpSetResultFilter
 *
 * This function sets filter for result matrices.
 *
 * @return MOLP_OK               - Success\n
 *         MOLPERR_BADHANDLE     - Unknown handle specified\n
 *         MOLPERR_NULL_POINTER  - Parameter pTFilter was a NULL pointer\n
 *         MOLPERR_NO_MODEL      - No model has been selected\n
 *         MOLPERR_INVALID_VALUE - Content of TFilter record was not valid
 *                                 according to current model. The new filter was discarded, and the
 *                                 previous filter still applies\n
 *
 * @param ohPred     [in] - Handle of Predictor to remove
 * @param pTFilter   [in] - Pointer to new filter record
 *
 * Comment - The returned pointer must be deallocated using molpFree() after use by the caller.
 */
CAMO_DLL_EXPORT UINT32 molpSetResultFilter( TObjHandle ohPred,
                                            TFilter *pTFilter );


/**
 * molpGetResultFilter
 *
 * This function gets a result filter
 *
 * @return MOLP_OK                - Success\n
 *         MOLPERR_BADHANDLE      - Unknown handle specified\n
 *         MOLPERR_NULL_POINTER   - Parameter ppTFilter was a NULL pointer\n 
 *         MOLPERR_NO_MODEL       - No model has been selected\n
 *         MOLPERR_INVALID_VALUE  - The filter type code requested was not a
 *                                 valid FT_xxx value\n
 *         MOLPERR_OUTOFMEMORY    - Could not allocate necessary memory for the
 *                                 filter record
 *
 * @param ohPred           [in]  - Handle of Predictor to query
 * @param nFilterRequested [in]  - Filter type code
 * @param ppTFilter        [out] - Pointer to filter record
 *
 * Comment - The returned pointer must be deallocated using molpFree() after use by the caller
 */
CAMO_DLL_EXPORT UINT32 molpGetResultFilter( TObjHandle ohPred, 
                                            UINT32 nFilterRequested, 
                                            TPointer *pTFilter );


/**
 * molpGetResultMatrix
 *
 * This function gets a result filter
 *
 * @return MOLP_OK                - Success\n
 *         MOLPERR_BADHANDLE      - Unknown handle specified\n
 *         MOLPERR_NULL_POINTER   - Parameter ppTFilter was a NULL pointer\n 
 *         MOLPERR_NO_MODEL       - No model has been selected\n
 *         MOLPERR_INVALID_VALUE  - The filter type code requested was not a
 *                                 valid FT_xxx value\n
 *         MOLPERR_OUTOFMEMORY    - Could not allocate necessary memory for the
 *                                 filter record
 *
 * @param ohPred     [in]  - Handle of Predictor to query
 * @param nMatrixID  [in]  - Numeric ID of requested matrix
 * @param ppTMatrix  [out] - Pointer to matrix data record
 *
 * Comment - Matrix ID are the Unscrambler/Guideline MID numbers.\n
 *           The returned pointer must be deallocated using molpFree() after use by the caller
 */
CAMO_DLL_EXPORT UINT32 molpGetResultMatrix( TObjHandle ohPred, 
                                            TMID nMatrixID,     
                                            TMatrix* pMatrix );

/**
 * molpGetOutlierList
 *
 * This function gets a copy of the current outlier list
 *
 * @return MOLP_OK               - Success\n
 *         MOLPERR_BADHANDLE     - Unknown handle specified\n
 *         MOLPERR_NULL_POINTER  - Parameter ppTOWL was a NULL pointer\n
 *         MOLPERR_NO_MODEL      - No model has been selected\n
 *         MOLPERR_OUTOFMEMORY   - Could not allocate necessary memory for the TOWL record\n.
 *
 * @param ohPred [in]   - Handle of Predictor to remove
 * @param ppTOWL [out]  - Pointer to object warning list
 *
 * Comment - The returned pointer must be deallocated using molpFree() after use by the caller
 */
/*UINT32 molpGetOutlierList( TObjHandle ohPred, 
                           TPointer *ppTOWL );

*/
/**
 * molpGetNumObjWarnings
 *
 * This function gets number of warnings available in the current outlier list
 *
 * @return MOLP_OK               - Success\n
 *         MOLPERR_BADHANDLE     - Unknown handle specified\n
 *         MOLPERR_NULL_POINTER  - Parameter pnNumWarnings was a NULL pointer\n
 *         MOLPERR_NO_MODEL      - No model has been selected\n
 *
 * @param ohPred        [in]  - Handle of Predictor to remove
 * @param pnNumWarnings [out] - Number of object warnings available (numbered 1..n)
 *
 */
/*UINT32 molpGetNumObjWarnings( TObjHandle ohPred, 
                              UINT32* pnNumWarnings );

*/
/**
 * molpGetObjWarning
 *
 * This function gets one object warning from the current outlier list.\n
 * Valid object warning numbers are in the range 1 to number of
 * warnings in the current object warning (outlier) list.
 *
 * @return MOLP_OK               - Success\n
 *         MOLPERR_BADHANDLE     - Unknown handle specified\n
 *         MOLPERR_NULL_POINTER  - Parameter pWarning was a NULL pointer\n
 *         MOLPERR_NO_MODEL      - No model has been selected\n
 *         MOLPERR_OUTOFRANGE    - Index 'nWarningNo' out of range\n
 *         MOLPERR_OUTOFMEMORY   - Could not allocate necessary memory for the\n
 *
 * @param ohPred     [in]  - Handle of Predictor to query
 * @param nWarningNo [in]  - Object warning number (index)
 * @param pWarning   [out] - Ptr. to TObjWarning buffer where the requested object
 *                           warning data is returned.
 */
/*UINT32 molpGetObjWarning( TObjHandle ohPred, 
                          UINT32 nWarningNo, 
                          TObjWarning* pWarning );
*/
/**************************************************************************\
**
** $Log: molup.h,v $
** Revision 1.3  2004/12/28 13:21:02  rajani
** Replaced History with Log
**
**
\***************************************************************************/

#endif
