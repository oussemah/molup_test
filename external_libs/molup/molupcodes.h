#ifndef _MOLUP_CODES_H
#define _MOLUP_CODES_H

/****************************************************************************\
**
** molupcodes.h
**
** Error codes, enums and Constants to use internaly and also exposed to  
** Aoutside world. All constants/enums/error codes you want to  expose to  
** outside world should be kept here.
**
** Copyright (c) 2004 Camo Technologies.
** All Rights Reserved
**
** Authors:
**	Zala VijaySinh Khumansinh
** Rajani Davuluri
** Lokanadham R
**
** $Header: /home/cvsroots/devroot/MobileOLUP/cpp/molup/inc/molupcodes.h,v 1.2 2004/12/28 13:21:02 rajani Exp $
**
\****************************************************************************/


#ifndef _MOLP_ERRCODES
#define _MOLP_ERRCODES

/**
 * OLUP Error Status codes
 */
#define MOLP_OK                                    0     /**< No error                                      */
#define MOLPERR_INTERNAL                           1     /**< Internal error (bug?)                         */
#define MOLPERR_OUTOFHANDLES                       2     /**< Cannot create object handle                   */
#define MOLPERR_OUTOFMEMORY                        3     /**< Not enough memory available                   */
#define MOLPERR_OUTOFRANGE                         4     /**< Indexing outside valid range                  */
#define MOLPERR_VERSION                            9     /**< Incorrect version number                      */
#define MOLPERR_BADHANDLE                          10    /**< Bad/unknown handle specified                  */
#define MOLPERR_NULL_POINTER                       11    /**< A required pointer param. was NULL            */
#define MOLPERR_INVALID_VALUE                      12    /**< Parameter value not valid                     */
#define MOLPERR_RESLEVEL_RANGE                     13    /**< Result level value too high or low            */
#define MOLPERR_VALUE_COUNT                        14    /**< Wrong number of values received               */
#define MOLPERR_NUM_COLUMNS                        15    /**< Incorrect # columns in matrix                 */
#define MOLPERR_NUM_ROWS                           16    /**< Incorrect # rows in matrix                    */
#define MOLPERR_NUM_PLANES                         17    /**< Incorrect # planes in matrix                  */
#define MOLPERR_TRUNCATED                          20    /**< Number/string was truncated to fit            */
#define MOLPERR_UNDEFINED                          21    /**< Required setting/value undefined              */
#define MOLPERR_NO_MODEL                           22    /**< No model has been selected                    */
#define MOLPERR_FILE_TYPE                          30    /**< Bad file type specified                       */
#define MOLPERR_FILE                               31    /**< Error during file I/O                         */
#define MOLPERR_PREDICTION                         40    /**< Calc. failure during prediction               */
#define MOLPERR_FEATURE_NOT_IMPL                   50    /**< Feature is not implemented yet.               */
#define MOLPERR_UNSUPPORTED_PRETREATMENT           51    /**< The PreTreatments apart from Norris,          */
                                                         /*   SGolay & MSC, in AMO file are
                                                              considered as invalid type                    */
#define MOLPERR_INVALID_FILE                       52    /**< File name is Invalid                          */
#define MOLPERR_MODEL_FILE_NOT_SUPPORTED           53    /**< AMO files created earlier to 9.1.3            */
                                                         /*   cant be supported in MobileOLUP	            */
#define MOLPERR_PRETREATMENTS_MISSING              54    /**< Missing Data is not allowed in                */
                                                         /*   Norris, SGolay & MSC Pretreatments            */
#define MOLPERR_LESS_NUM_COLUMNS                   60    /**< Incorrect number of variables at              */
                                                         /*   least 5 variables to perform
                                                              Norris derivation                             */
#define MOLPERR_LESS_SUM_AVG_POINTS                61    /**< Order must be less than or equal the          */
                                                         /*   sum of averaging points                  		*/
#define MOLPERR_DER_ORDER_EQL_POLY_ORDER           62    /**< Derivation order must be less than or         */
                                                         /*	  equal the polynomial order                    */
#define MOLPERR_OUTOFPOLY_ORDER                    63    /**< The polynomial order must be between          */
                                                         /*   0 and 12                                      */
#define MOLPERR_NOT_LESS_POINTS_ORDER              64    /**< The sum of averaging points +1 must           */
                                                         /*   be less than this number                      */
#define MOLPERR_DIVIDE_BY_ZERO                     65    /**< Divide by the zero                            */
#define MOLPERR_OUT_MSC_MODEL_TYPE                 66    /**< Valid MSC model type between 1 and 3          */
#define MOLPERR_OUT_MSC_RESULT_TYPE                67    /**< Valid MSC result type between 1 and 2         */
#define MOLPERR_NO_MSC_SOLUTION                    68    /**< No MSC solution found                         */
#define MOLPERR_MSC_INFO_NOT_AVAILABLE             69    /**< MSC Matrix not Available                      */
#define MOLPERR_SGOLAY_DERIVATIVE_GREATER          70    /**< SGolay Supports 4 Derivatives only            */
#define MOLPERR_SGOLAY_XVARS_GREATER_POINTS        71    /**< SGolay the Sum of Averaging points            */
                                                         /*   must be less than the XVars                   */
#define MOLPERR_SGOLAY_DERIVATIVE_ORDER_GREATER    72    /**< SGolay Derivation order must be               */
                                                         /*   less than or equal to the Polynomial order    */
#define MOLPERR_SGOLAY_POLYNOMIAL_ORDER_GREATER    73    /**< SGolay Order must be less that or equal to    */
                                                         /*   sum of Averaging points                    	*/
#define MOLPERR_SGOLAY_POLYNOMIAL_ORDER_MISMATCH   74    /**< SGolay Polynomial order must be between 2     */
                                                         /*   to 12 only                                    */
#define MOLPERR_NORRIS_SEGSIZE_GREATER_XVARS       75    /**< Norris Segment size should be less than       */
                                                         /*   or equal to number of variable slected        */
#define MOLPERR_NORRIS_DIFF_GREATER                76    /**< Norris Difference must to 1 less than         */
                                                         /*   the Segment Size.                             */
#define MOLPERR_MSC_MODEL_GREATER                  77    /**< MSC Model should not be more than 3,          */
                                                         /*   since it has only 3 options i.e (Full MSC,
                                                              Common Offset,Common Amplification)           */
#define MOLPERR_MSC_TASK_GREATER                   78    /**< MSC Task should not be more than 2,since      */
                                                         /*   it has only 2 task i.e (New & Existing)       */
#define MOLPERR_MSC_MORE_THAN_ONE                  79    /**< MSC Model File(AMO) should not have more      */
                                                         /*   than 1 MSC Transformation                     */
#define MOLPERR_NOT_SUPPORTED                      999   /**< Feature not supported                         */


#endif   /* _MOLP_ERRCODES */


#ifndef _MOLU_FILTERS
#define _MOLU_FILTERS

/**
 * Filter types & special setting values
 */
#define FT_SAMPLES               1     /**< Identifies filter for Samples       */
#define FT_XVARS                 2     /**< Identifies X-Variables filter       */
#define FT_YVARS                 3     /**< Identifies Y-Variables filter       */
#define FT_COMPSUSED             4     /**< Identifies a CompsUsed filter       */
#define FS_ALL                   5     /**< Setting: All in range               */ 
#define FS_RANGES                0     /**< Setting: Listed ranges used         */

#endif   /* _MOLU_FILTERS */

 
#ifndef _MOLU_PREDMIDS
#define _MOLU_PREDMIDS

/**
 * Result Matrix ID numbers
 */
#define PIDX_YPRED            0     /* Result matrix: yPredicted                 */
#define PIDX_TAI              1     /* Result matrix: Scores                     */
#define PIDX_YPREDWGTED       2     /* Result matrix: yPredWeight						*/
                                    /* NOTE: This is a temporary matrix and
                                       not avaiable with molpGetResultMatrix()   */
#define PIDX_HI               3     /* Result matrix: Leverages                  */
#define PIDX_EIX              4     /* Result matrix: Eix                        */
#define PIDX_PVXTOT           5     /* Result matrix:	ResXValTot                 */	
#define PIDX_VXI              6     /* Result matrix: ResXValSamp                */
#define PIDX_YDEV             7     /* Result matrix: YDeviation                 */
#define PIDX_NSMISS           8     /* NOTE: This is a temporary matrix and 		*/
                                    /* not avaiable with molpGetResultMatrix()   */
#define PIDX_NXMISS           9     /* NOTE: This is a temporary matrix and 		*/
                                    /* not avaiable with molpGetResultMatrix()   */
#define PIDX_NVXI             10    /* Result matrix: YCent                      */
#define PIDX_MAHDIST          11    /* Result matrix: Mahalanobis distance       */
/*
   If you add new result matrices in above list then update 
   NUM_RESULT_MATRICES as well.
*/
#define NUM_RESULT_MATRICES      12    /* Total number of Result matrices */

#endif   /* _MOLU_PREDMIDS */

 
#ifndef _MOLU_CALMETHODS
#define _MOLU_CALMETHODS

/**
 * Calibration methods
 */
#define CALMETHOD_PCA            1     /**< Method name: PCA                    */
#define CALMETHOD_PCR            2     /**< Method name: PCR                    */
#define CALMETHOD_PLS1           3     /**< Method name: PLS1                   */
#define CALMETHOD_PLS2           4     /**< Method name: PLS2                   */
#define CALMETHOD_MLR            5     /**< Method name: MLR                    */

#endif   /* _MOLU_CALMETHODS */

#define MISSING (FLOAT32)9.973e23

 
#ifndef _MOLU_STYPES
#define _MOLU_STYPES

/*!!-CHECK
 *
 * Special data types (must be customized to fit compiler)
 */

#if WIN32
typedef unsigned short UINT16;    /**< Unsigned 16-bit integer                   */
#if _MSC_VER < 1200 // VC++ 6.0
typedef DWORD  UINT32;           /**< Unsigned 32-bit integer                    */
typedef LONG   INT32;            /**< Signed 32-bit integer                      */
#else
#include <basetsd.h>             /**< use predefined UINT32,INT32                */
#endif
typedef float  FLOAT32;          /**< 32-bit floating point (single precision)   */
typedef signed long int tBOOL;

#define FALSE   0
#define TRUE    1

#else

typedef unsigned int                   UINT32;
typedef long                           INT32 ;
typedef unsigned short                 UINT16;
typedef  float                         FLOAT32;
typedef signed long int                tBOOL; 

#define FALSE   0
#define TRUE    1

	/*!!-CHECK
    * ZV - Define Data type UINT16, UINT32, INT32 and FLOAT32 on Linux/Motarala processor,
	 * 		once we get the spcification 
	 */
	//#error "Yet to Define Data types for Platform other than windows"
#endif

#if WIN32
#define CAMO_DLL_EXPORT __declspec (dllexport)
#else
#define CAMO_DLL_EXPORT 
#endif

/**
 * OLUP specific simple types
 */
typedef UINT32    TObjHandle;    /**< Reference handle of an OLUP object        */
                                 /**< Currently: Predictor or Classificator     */
typedef INT32     TMID;          /**< Identifies a data/result matrix           */
typedef void      *TPointer;     /**< Pointer to a memory block.                */
                                 

#endif   /* _MOLU_STYPES */


#ifndef _MOLU_CFS
#define _MOLU_CFS

/**
 * Clipboard formats for communicating with OLUP DDE Server
 */

typedef struct tagTCFUnscMatrix
{
   UINT32   NumColumns;
   UINT32   NumRows;
   UINT32   NumPlanes;
   FLOAT32  aData[1];            /**< (NumColumns*NumRows*NumPlanes) elements   */
   /* NOTE:
    * The correct size of this struct should be calculated as:
    *       offsetof(TCFUnscMatrix,aData) + NumElements * sizeof(FLOAT32)
    */
} TCFUnscMatrix;

typedef struct tagTCFUnscDWord
{
   UINT32   Value;               /**< The one and only integer value transf'd   */
} TCFUnscDWord;

#endif   /* _MOLU_CFS */


#ifndef _MOLU_RTYPES
#define _MOLU_RTYPES

/*
 * Data record types
 */

#define MOLP_CURRENT_VERSION   1  	/**< Current value of VersionID fields in     	*/
                                  	/**< extendible data records.                  	*/

typedef struct tagTOptions
{
   UINT32   VersionID;           /**< Version number of record layout           */
                                 /**< Current value is 2.                       */
                                 /**< The value will increase as more fields    */
                                 /**< are added to the record (from _Reserved)  */
   UINT16   ResultLevel;
   UINT16   OutlierDetection;
   UINT16   DoPretreatments;
   UINT16   _Reserved[19];       /**< Reserved for future extension             */
} TOptions;


typedef struct tagTModelInfo
{
   UINT32   VersionID;           /**< Version number of record layout           */
                                 /**< Current value is 2.                       */
                                 /**< The value will increase as more fields    */
                                 /**< are added to the record (from _Reserved)  */
   UINT32   CalMethod;
   UINT32   CompsUsed;
   UINT32   CompsOpt;
   UINT32   NumComps;
   UINT32   NumXVars;            /**< Excluding interaction & square effects    */
   UINT32   NumYVars;
   UINT32   NumUsedXVars;        /**< Excluding interaction & square effects    */
   UINT32   NumUsedYVars;
   /**< --- Following fields are only valid with VersionID >= 2 --------------- */
   UINT32   NumTotalXVars;       /**< Including interaction & square effects    */
   UINT32   NumUsedTotalXVars;   /**< Including interaction & square effects    */
   /**< --- Following fields are only valid with VersionID >= 3 --------------- */
   UINT32   NumPretreatmentVars;  /**< Number of Pretreatment variables set for
									        the model if any transformations are */
   UINT32   _Reserved[16];       /**< Reserved for future extensions            */
} TModelInfo;


typedef struct tagTRange
{
   UINT32   StartElement;
   UINT32   EndElement;
} TRange;


typedef struct tagTFilter
{
   UINT32   FilterType;          /**< Valid for FT_x values only          */
   INT32    Setting;             /**< FT_COMPSUSED: # comps used          */
                                 /**< Other filters: Use 'aRanges' array  */
   UINT32   NumRanges;           /**< # of elements in array              */
   TRange   aRanges[1];          /**< 1st element in array                */
   /* NOTE:
    * Actual number of elements in 'aRanges' is 'NumRanges'.
    * The correct size of this struct should therefore be
    * calculated as:
    *       offsetof(TFilter,aRanges) + NumRanges * sizeof(TRange)
    */
} TFilter;


typedef struct tagTMatrix
{
   UINT32   NumColumns;
   UINT32   NumRows;
   UINT32   NumPlanes;
   FLOAT32  aData[1];            /**< (NumColumns*NumRows*NumPlanes) elements   */
   /* NOTE:
    * The correct size of this struct should be calculated as:
    *       offsetof(TMatrix,aData) + NumElements * sizeof(FLOAT32)
    */
} TMatrix;


typedef enum
{
   H_UNUSED,
   H_PRED,
   H_CLASS,
   H_MATRIX,
   H_FILTER
} THandleType;

#endif   /* _MOLU_RTYPES */


#ifndef _MOLU_OWTYPES
#define _MOLU_OWTYPES

/**
 * Object Warnings data record types
 */

typedef struct tagTObjWarning    /*--Object Warning--*/
{
   INT32    nObjWarningID;       /**< Object warning ID                   */
   FLOAT32  fValue1;             /**< Evaluated value #1 (or MISSING)     */
   FLOAT32  fValue2;             /**< Evaluated value #2 (or MISSING)     */
   INT32    nSampleNo;           /**< Sample number                       */
   INT32    nXVariableNo;        /**< X-variable number                   */
   INT32    nYVariableNo;        /**< Y-variable number                   */
   INT32    nCompNo;             /**< Component (PC) number               */
} TObjWarning;

typedef struct tagTOWL           /*--Object Warning List--*/
{
   UINT32      NumWarnings;      /**< # of elements in array              */
   TObjWarning aWarnings[1];     /**< 1st element in array                */
   /* NOTE:
    * Actual number of elements in 'aWarnings' is 'NumWarnings'.
    * The correct size of this struct should therefore be
    * calculated as:
    *       offsetof(TOWL,aWarnings) + NumWarnings * sizeof(TObjWarning)
    */
} TOWL;

#endif   /* _MOLU_OWTYPES */


/**************************************************************************\
**
** $Log: molupcodes.h,v $
** Revision 1.2  2004/12/28 13:21:02  rajani
** Replaced History with Log
**
**
\***************************************************************************/

#endif /*_MOLUP_CODES_H*/

