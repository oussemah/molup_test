/**
  ******************************************************************************
  * @file    FatFs/FatFs_USBDisk/Src/main.c 
  * @author  MCD Application Team
  * @version V1.3.3
  * @date    29-January-2016
  * @brief   Main program body
  *          This sample code shows how to use FatFs with USB disk drive.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"    

#include "file_handler.h"

#include "molup.h"
#include "molupcodes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef UartHandle;

/* Fatfs structure */
FATFS FS;
FIL fil;
FRESULT fres;

/* Buffer variable */
char buffer[100];

/* Private function prototypes -----------------------------------------------*/ 
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/
#define FPRINTF( file, fmt, ...)    sprintf(buffer, (fmt), ##__VA_ARGS__); \
                                    f_puts(buffer, (file))
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t splash_text[]="Starting LibMolup test on STM32\n\r";
  uint8_t rtext[100];
  uint8_t buff[7];
  unsigned int bytesread = 0;
  unsigned int var = 0;
  float *a;

  HAL_Init();
  
  /* Configure LED4 and LED5 */
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);  
  
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();

  UartHandle.Instance          = USARTx;
  
  UartHandle.Init.BaudRate     = 9600;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  BSP_LED_Toggle(LED3);
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_UART_Transmit(&UartHandle, (uint8_t*) splash_text, sizeof(splash_text), 500);

  /* Put on Orange LED to signal app start */
  BSP_LED_On(LED3);

  /* Try to mount card */
    if (f_mount(&FS, "", 1) == FR_OK) {
        molup_test();
        printf ("Mounted OK\r\n");
        /* Try to open file */
        if ((fres = f_open(&fil, "tests_file1.txt", FA_OPEN_ALWAYS | FA_WRITE)) == FR_OK) {
            printf("Open OK\r\n");
            var = 13520;
            /* Format string */
            sprintf(buffer, "Var value = : %u \n", var);
            
            /* Write total card size to file */
            f_puts(buffer, &fil);

            /* Close file */
            f_close(&fil);
            
            /* read file contents */
            if ((fres = f_open(&fil, "output.txt", FA_READ)) == FR_OK) {
                printf("Open OK\r\n");
                /* Read data from the text file */
                var = 0;
                m_fscanf(&fil, "%s = %u", buff, &var);
                printf("Read  %s : %d\r\n", buff, var);
                f_close(&fil);
            } else {
              printf("Unable to open file for reading\r\n");
              Error_Handler();
            }
            /* Turn led ON */
            BSP_LED_On(LED4);
        } else {
            printf("Unable to open file for writing\r\n");
            Error_Handler();
        }
        BSP_LED_Off(LED3);
        /* Unmount SDCARD */
        f_mount(NULL, "", 1);
    } else {
      printf("Unable to mount fatfs\r\n");
    }

  while (1)
  {
  }
}

static void SystemClock_Config  (void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig (&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

static void Error_Handler(void)
{
  /* Turn LED5 on */
  BSP_LED_On(LED5);
  f_mount(NULL, "", 1);
  while(1)
  {
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  /* Turn LED3 on: Transfer error in reception/transmission process */
  BSP_LED_On(LED3); 
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

#define MAXMODULE 50
char MODEL_FILE_NAME[]= "PLS.AMO";
char INPUT_FILE_NAME[]= "input.txt";
char OUTPUT_FILE_NAME[]= "output1.txt";

void getStrFromErrorCode(uint32_t nError )
{
    printf("Error:%u\n\r", nError);
    return;
}

TFilter* parseFilter( char* sXFilter, UINT32 nType, UINT32 nLimit, UINT32* nTotlElements,UINT32* nError );

const uint8_t skip_filter_ranges_xvars = 0;
const uint8_t skip_filter_ranges_samples = 0;

static 	float fDataBuf			= 0.0f;

void molup_test()
{
    typedef uint32_t (*TObjHandle)(),(*myUINT32)();
    typedef void (*myVOID)();

	TMatrix *pPredictionData   = NULL;  /* Matrix to hold Data to be predicted */
	TMatrix *pResultMatrix     = NULL;	/* Matrix to hold the predicted results */
	
	FIL *pResultFile    = NULL;		/* File pointer for to store Result Data */
	FIL *pInDataFile    = NULL;     /* File pointer for input Data */
	FIL *pModelFile = NULL;     /* File pointer for Model (*.AMO) */

	TOptions *hOptions   = NULL;
	TFilter* pTFilter    = NULL;
	TModelInfo modelInfo;
	TObjHandle objHandle;

	uint8_t tmp_res = 0;
  
	uint32_t iSamCtr			= 0;     /* Sample Counter */
	uint32_t iVarCtr			= 0;     /* Variable Counter */
	uint32_t iPCsCtr			= 0;     /* PC Counter */	
	uint32_t nSamPrediction   = 25; //ToDo: Get dynamicalyl from test command line
	uint32_t nXVarsPrediction = 0;
	uint32_t nModelXVars      = 0;
	uint32_t nPCs				= 0;
	uint32_t nTotYVars		= 1; /* For PLS 1 model number of y Variables = 1 */   
	uint32_t iSampleNo		= 0;
	uint32_t nTotSamples		= 0;
	uint32_t iDataCtr			= 0;    /* Data Counter */
	uint32_t nVal				= 0;
	uint32_t nOption			= 0;
	uint32_t nPreTreatment	= 0;	
	uint32_t nNumElements		= 0;
	uint32_t nByteSize		= 0;
	uint32_t iDataCounter		= 0;
	uint32_t nCounter         = 0;
	uint32_t nTotalElements	= 0;
	uint32_t nXVariables		= 0;
	uint32_t error			= 0;
	
	short int iPredictType	= 0;
	
	char mod[MAXMODULE];
	char sDataBuf;
	char szPredictType[5]	= "ALL";
	char sAlphaCheck      = 0; /* checks if the user options is alpha */
	char cFilter          = 0;
	char cXFilter			= 0;
	char cSamplFilter		= 0;
	char cPCsFilter			= 0;
	char sFiltersranges[250]= "";
	char pszVarName[50]		= "";
	char sSharedObj[50]		= "";

    /* Check if model file exists */
	pModelFile = m_fopen(MODEL_FILE_NAME,"r");
	if(pModelFile == NULL)
	{
		printf("\nError: Model file(AMO) not found\r\n");
		Error_Handler();
	}
	else
		m_fclose( pModelFile );

  /* Check if Data file exists */
	pInDataFile = m_fopen(INPUT_FILE_NAME,"r");
	if(pInDataFile == NULL)
	{
		printf("\nError: Input Data file not found\n");
		Error_Handler();
	}
  
    objHandle = (myUINT32)molpOpenPredictor();	/* Open predictor - Returns a handle */

    nPreTreatment = 1;
    nOption = 0;

    // Initialize TOptions struct. The OutlierDetection and DoPretreatments options are Disabled.
	hOptions = (TOptions*)malloc (sizeof(TOptions));
	
	hOptions->VersionID			= MOLP_CURRENT_VERSION;
	hOptions->ResultLevel		= nOption;
	hOptions->OutlierDetection	= 0;
	hOptions->DoPretreatments	= nPreTreatment;
	
	error = 0;
	error = molpSetOptions( objHandle, hOptions );

	if( error != MOLP_OK )
	{
		getStrFromErrorCode( error );
		
		if( hOptions != NULL )
		{
			free(hOptions);
			hOptions= NULL;
		}
		Error_Handler();
	}

    error = 0;
	error = molpSelectModelFile(objHandle,MODEL_FILE_NAME);

    if(error != MOLP_OK)
	{
		getStrFromErrorCode( error );
		
		molpFreeTMobileHandles( objHandle );
		if( hOptions != NULL )
		{
			free(hOptions);
			hOptions= NULL;
		}
		Error_Handler();
	}

	error = 0;
	error = molpGetModelInfo(objHandle,&modelInfo);
	
	if( error != MOLP_OK )
	{
		getStrFromErrorCode( error );
		
		molpFreeTMobileHandles(objHandle);
		free(hOptions);
		Error_Handler();
	}

  /* Validate Predict type */
	iPredictType = 0;
	if(m_strcmp(szPredictType,"ALL") == 0)
	{
		iPredictType = 0;
	}
	else if(m_strcmp(szPredictType,"ONE") == 0)
	{
		iPredictType = 1;
	}
  
  nPCs = modelInfo.CompsUsed;
  
  /* Set the PCs for prediction */
	nVal = molpSetCompsUsed(objHandle,nPCs);
  
  if(nVal == MOLPERR_INVALID_VALUE)
	{
		printf("\nError: MOLPERR_INVALID_VALUE\r\n");
		molpFreeTMobileHandles(objHandle);
		free(hOptions);
		Error_Handler();
	}
  
  /* Get number of x-variables from command line argument */
	if(hOptions->DoPretreatments == 1)
	{
		if(modelInfo.NumPretreatmentVars == 0)
		{
			printf("\nWarning: No transformations are there in Model file to be applied as auto-pretreatments\r\n");
			printf("Prediction will be performed without pretreatments\r\n");
			nModelXVars = modelInfo.NumXVars;
			
			//Call molpSetOptions to reset the value for DoPretreatments
		}
		else
		{
			nModelXVars = modelInfo.NumPretreatmentVars;
		}
	}
	else
	{
		nModelXVars = modelInfo.NumXVars;
	}

  nXVarsPrediction = nModelXVars;
  
  /* Open the file for writing the results */
	pResultFile = m_fopen(OUTPUT_FILE_NAME,"w+");
	if(pResultFile == NULL)
	{
		printf("\nError: sorry Out of Memory. File not created\n");
		
		molpFreeTMobileHandles(objHandle);
		free(hOptions);
		Error_Handler();
	}

  /* Writing prediction method in file */
	if(nOption == 0)
		f_puts("\n\nPrediction Method: Prediction using Beta coefficients\n\n", pResultFile);
	else if((nOption == 1) || (nOption == 2))
		f_puts("\n\nPrediction Method: Prediction by projection\n\n", pResultFile);
  
  f_puts("X-variable names in model file: \n", pResultFile);
	for( iVarCtr = 0; iVarCtr < (int)(modelInfo.NumXVars); iVarCtr++)
	{
		error = 0;
		error = molpGetXVarName(objHandle,iVarCtr,pszVarName,50);
		
		if(error != MOLP_OK)
		{
			getStrFromErrorCode( error );
			
			molpFreeTMobileHandles(objHandle);
			free(hOptions);
			Error_Handler();
		}
		sprintf(buffer,"%s\n",pszVarName);
		f_puts(buffer, pResultFile);
	}
  
  f_puts("\n\n", pResultFile);
	
	f_puts("Y-variable names in model file: \n", pResultFile);
	for( iVarCtr = 0; iVarCtr < nTotYVars; iVarCtr++)
	{
		error = 0;
		error = molpGetYVarName(objHandle,iVarCtr,pszVarName,50);
		
		if(error != MOLP_OK)
		{
			getStrFromErrorCode( error );
			
			molpFreeTMobileHandles(objHandle);
			free(hOptions);
			Error_Handler();
		}
		sprintf(buffer,"%s\n",pszVarName);
		f_puts(buffer, pResultFile);
	}
	
	m_fseek( pInDataFile, 0L, SEEK_SET );  /* Get the input data from the input Data file */
	
	f_puts("\nData Used For Prediction:\n", pResultFile);
  
  	iDataCtr = 0;
	for(iSamCtr = 1; iSamCtr <= nSamPrediction; iSamCtr++)
	{
		nNumElements = (nSamPrediction * nXVarsPrediction * 1);
		nByteSize    = sizeof(TMatrix);
		if(nNumElements > 0)
		{
			nByteSize = nNumElements * sizeof(FLOAT32);
			nByteSize += offsetof(TMatrix,aData);
		}
		
		if(pPredictionData == NULL)
		{
			pPredictionData = (TMatrix*)malloc (nByteSize / sizeof(char) + sizeof(char));
		}
		
		pPredictionData->NumColumns = nXVarsPrediction;
		pPredictionData->NumPlanes  = 1;
		pPredictionData->NumRows    = nSamPrediction;
		
		FPRINTF(pResultFile,"\nSample %d  ",iSamCtr);
		for(iVarCtr = 1; iVarCtr <= nXVarsPrediction; iVarCtr++)
		{
			m_fscanf(pInDataFile,"%c",&sDataBuf);
			if( isspace(sDataBuf))
				m_fscanf(pInDataFile,"%c",&sDataBuf);
			
			if( isalpha(sDataBuf) )
			{
				if(sDataBuf == 'm')
				{
					fDataBuf = MISSING;
					FPRINTF(pResultFile,"%c  ",'m');
					
					/* Read intput data one by one, write in file */
					pPredictionData->aData[iDataCtr++] = fDataBuf;
				}
				else
				{
					printf("\nError: Invalid Input Data\n");
					molpFreeTMobileHandles(objHandle);
					free(hOptions);
					Error_Handler();
				}
			}
			else
			{
				m_fseek( pInDataFile, -1, SEEK_CUR); //(m_ftell(pInDataFile)-1), SEEK_SET );
        tmp_res = m_fscanf(pInDataFile,"%f",&fDataBuf);
				if((tmp_res) == 1)
				{
					if( fDataBuf == MISSING )
					{
						fDataBuf= MISSING;
						FPRINTF(pResultFile,"%c  ",'m');
					}
					else
						FPRINTF(pResultFile,"%e  ",fDataBuf);
					
					/* Read intput data one by one, write in file */
					pPredictionData->aData[iDataCtr++] = fDataBuf;
				}
			}
			fDataBuf = 0.0;
		}
	}
	
	if(iDataCtr < nNumElements)
	{
		printf("\nError: Number of X-variables misMatch\n");
		molpFreeTMobileHandles(objHandle);
		free(hOptions);
		Error_Handler();
	}
	
	if(iPredictType == 1) //One Sample at a time
	{
		nTotSamples = 1;
	}
	else
	{
		nTotSamples		= nSamPrediction;
		nSamPrediction 	= 1;
	}
	
  printf("HERE \r\n");
/* Filters start */
	cFilter = 0;
	nXVariables  = nXVarsPrediction;
    nCounter = 0;
		  do
		  {
			  if( pTFilter != NULL )
			  {
				  free( pTFilter );
				  pTFilter = NULL;
			  }
			  
              if (skip_filter_ranges_xvars)
                break;
              sprintf(sFiltersranges, "%d", 1+nCounter);

			  nTotalElements = nModelXVars;
			  error = 0;
			  pTFilter = parseFilter( sFiltersranges, FT_XVARS, nModelXVars, &nTotalElements, &error );
			  
			  if( error == 1 )
			  {
				  printf("\nSelected ranges is overlaping %s, Selected X-variable ranges must be non-overlaping  ",sFiltersranges );
			  }
			  else if( error == 2 )
			  {
				  if( nCounter == 2 )
					  break;
				  printf("\nSelect the  X-Variables Ranges between 1 - %d\n",nXVarsPrediction );
			  }
        else {
          printf("sFiltersranges %s\r\n", sFiltersranges);
        }
			  
			  nCounter = nCounter + 1;
			  
		  } while ( ((error == 1) || (error == 2)) && (nCounter < nXVarsPrediction + 1));
		  
		  if( (error == 0) && (nCounter >= 1) )
		  {
			  error = molpSetResultFilter( objHandle,pTFilter );
			  if(error != MOLP_OK)
			  {
				  getStrFromErrorCode( error );
				  
				  if( pTFilter != NULL )
				  {
					  free( pTFilter );
					  pTFilter = NULL;
				  }
				  molpFreeTMobileHandles(objHandle);
				  free(hOptions);
				  Error_Handler();
			  }
			  if( pTFilter != NULL )
			  {
				  free( pTFilter );
				  pTFilter = NULL;
			  }
			  nXVarsPrediction = nTotalElements;
		  }
		  else if (skip_filter_ranges_xvars == 0)
			  printf("\nWarning: Selected Ranges is overlaping/Mismatching X-Variables. Prediction will be done with all Variables\n");
		  /*}*/
		  
		  if( iPredictType == 0 )		/* All Sample at a time */
		  {
			  nCounter = 0;
			  do
			  {
				  if( pTFilter != NULL )
				  {
					  free( pTFilter );
					  pTFilter = NULL;
				  }
				  
                  if (skip_filter_ranges_samples)
                    break;
                  sprintf(sFiltersranges, "%d", 1+nCounter);

				  nTotalElements = nTotSamples;
				  error = 0;
				  pTFilter = parseFilter( sFiltersranges,FT_SAMPLES, nTotSamples, &nTotalElements, &error );
				  if( error == 1 )
				  {
					  printf("\nSelected ranges is overlaping %d, Selected Sample ranges must be non-overlaping ",nTotSamples );
				  }
				  else if( error == 2 )
				  {
					  
					  if( nCounter == 2 )
						  break;
					  printf( "\nSelect the Sample Ranges between 1 - %d\n",nTotSamples );
				  }
				  
				  nCounter = nCounter + 1;
				  
			  } while( ((error == 1) || (error == 2) ) && (nCounter < nTotSamples+1));
			  
			  if( (error == 0) && (atoi(sFiltersranges) != 0) )
			  {
				  error = molpSetResultFilter( objHandle,pTFilter );
				  if(error != MOLP_OK)
				  {
					  getStrFromErrorCode( error );
					  
					  if( pTFilter != NULL )
					  {
						  free( pTFilter );
						  pTFilter = NULL;
					  }
					  molpFreeTMobileHandles(objHandle);
					  free(hOptions);
					  Error_Handler();
				  }
				  if( pTFilter != NULL )
				  {
					  free( pTFilter );
					  pTFilter = NULL;
				  }
				  nTotSamples = nTotalElements;
			  }
			  else if( skip_filter_ranges_samples == 0 )
				  printf("\nWarning: Selected ranges is overlaping/Mismatching Sample. Prediction will be done with all Samples\n");
		  }
		  
		  nCounter = 0;
		  do
		  {
			  if( pTFilter != NULL )
			  {
				  free( pTFilter );
				  pTFilter = NULL;
			  }
			  
			  printf("\nEnter the PC's Ranges between 1 to %d,(0 - to Skip)\t:",nPCs );
			  //fflush( stdin );
			  //scanf("%s",sFiltersranges);
        sprintf(sFiltersranges, "1");
			  
			  if( atoi(sFiltersranges) == 0 )
				  break;
			  
			  error = 0;
			  nTotalElements = nPCs;
			  pTFilter = parseFilter( sFiltersranges,FT_COMPSUSED, nPCs, &nTotalElements, &error );
			  if( error == 1 )
			  {
				  printf("\nSelected ranges is overlaping %d, Selected PC's ranges must be non-overlaping  ",nPCs );
			  }
			  else if( error == 2 )
			  {			  
				  if( nCounter == 2 )
					  break;
				  printf("\nSelect the PC's Ranges between 1 - %d\t",nPCs);
			  }
			  nCounter = nCounter + 1;
			  
		  }while( (error == 1) || (error == 2) );
		  
		  if( (error == 0) && (atoi(sFiltersranges) != 0) )
		  {
			  error = molpSetResultFilter( objHandle,pTFilter );
			  if(error != MOLP_OK)
			  {
				  getStrFromErrorCode( error );
				  
				  if( pTFilter != NULL )
				  {
					  free( pTFilter );
					  pTFilter = NULL;
				  }
				  molpFreeTMobileHandles(objHandle);
				  free(hOptions);
				  Error_Handler();
			  }
			  if( pTFilter != NULL )
			  {
				  free( pTFilter );
				  pTFilter = NULL;
			  }
			  nPCs = nTotalElements;
		  }
		  else if( atoi(sFiltersranges) != 0 )
			  printf("\nWarning: Selected ranges is overlaping/Mismatching PC's. Prediction will be done with all PC's\n");
		  /*}*/
		  /*}*/
		  
		  /* - Use of below code only user need molupGetFiletrs only
		  if( pTFilter != NULL )
		  {
		  free( pTFilter );
		  pTFilter = NULL;
		  }        
		  
			molpGetResultFilter( objHandle, 1, (TPointer*)&pTFilter );
			
			  if( pTFilter != NULL )
			  {
			  molpFree( pTFilter );
			  pTFilter = NULL;
			  }
		  */
		  iDataCounter = 0;
		  for(iSampleNo = 1; iSampleNo <= nSamPrediction; iSampleNo++)
		  {
			  if(iPredictType == 1)
			  {
				  nByteSize = (nTotSamples * nXVariables * 1) * sizeof(FLOAT32);
				  nByteSize += offsetof(TMatrix,aData);
				  if(pResultMatrix == NULL)
				  {
					  pResultMatrix = (TMatrix*)malloc (nByteSize / sizeof(char) + sizeof(char));
				  }
				  pResultMatrix->NumColumns = nXVariables;
				  pResultMatrix->NumPlanes  = 1;
				  pResultMatrix->NumRows    = nTotSamples;
				  
				  for(iVarCtr = 0; iVarCtr < nXVariables; iVarCtr++,iDataCounter++)
				  {
					  pResultMatrix->aData[iVarCtr] = pPredictionData->aData[iDataCounter];
				  }
				  
				  error = 0;
				  error = molpPredict( objHandle, pResultMatrix );
				  
				  if(error != MOLP_OK)
				  {
					  getStrFromErrorCode( error );
					  
					  molpFreeTMobileHandles(objHandle);
					  free(hOptions);
					  free(pResultMatrix);
					  Error_Handler();
				  }
				  
				  free(pResultMatrix);
				  pResultMatrix = NULL;
			  }
			  else
			  {
				  /* Performing prediction */
				  error = 0;
				  error = molpPredict( objHandle, pPredictionData );
				  
				  if(error != MOLP_OK)
				  {
					  getStrFromErrorCode( error );
					  
					  molpFreeTMobileHandles(objHandle);
					  free(hOptions);
					  Error_Handler();
				  }
			  }
			  
			  /*  Storing Result Matrices in file:
			  If Prediction is done using Beta Coefficients only Y-Predicted result matrix is stored
			  If Prediction is done by Projection following matrices are stored:
			  Y-Predicted          - PIDX_YPRED
			  Scores               - PIDX_TAI
			  Leverages            - PIDX_HI
			  X-Residuals          - PIDX_EIX
			  ResXValSamp          - PIDX_VXI
			  ResXValTot           -
			  Y-Deviation          - PIDX_YDEV
			  Mahalanobis distance - PIDX_MAHDIST
			  */
			  if(iSampleNo == 1) { /* Write the below line only once in file */
				  FPRINTF(pResultFile,"\n\nPREDICTION RESULTS: \n");
			  }
			  if(iPredictType == 1) {
				  FPRINTF(pResultFile,"\nResults for Sample %d\n\n", iSampleNo);
			  }
			  /* Y-Predicted Result */
			  if( (nOption == 0) || (nOption == 1) || (nOption == 2))
			  {
				  nByteSize = (nTotSamples * nPCs * 1) * sizeof(FLOAT32);
				  nByteSize += offsetof(TMatrix,aData);
				  if(pResultMatrix == NULL)
				  {
					  pResultMatrix = (TMatrix*)malloc (nByteSize / sizeof(char) + sizeof(char));
				  }
				  
				  error = 0;
				  error = molpGetResultMatrix(objHandle, PIDX_YPRED, pResultMatrix);
				  if(error != MOLP_OK)
				  {
					  getStrFromErrorCode( error );
					  
					  molpFreeTMobileHandles(objHandle);
					  free(hOptions);
					  Error_Handler();
				  }
				  
				  FPRINTF(pResultFile,"Y-Predicted (%d %d %d)\n",pResultMatrix->NumRows,pResultMatrix->NumColumns,pResultMatrix->NumPlanes);
				  
				  for( iDataCtr = 0, iSamCtr = 1; iSamCtr <= pResultMatrix->NumRows; iSamCtr++ )
				  {
					  if(iPredictType == 0)
						  FPRINTF(pResultFile,"\nSample %d  ", iSamCtr);
					  for(iVarCtr = 1; iVarCtr <= pResultMatrix->NumPlanes; iVarCtr++ )
					  {
						  for(iPCsCtr = 1; iPCsCtr <= pResultMatrix->NumColumns; iPCsCtr++ )
						  {
							  fDataBuf = pResultMatrix->aData[iDataCtr++];
							  if( fDataBuf == MISSING ) {
								  FPRINTF(pResultFile,"%c  ",'m' );
							  } else {
								  FPRINTF(pResultFile,"%e  ",fDataBuf );
                }
						  }
					  }
				  }
				  iSamCtr = 0;
				  
				  FPRINTF(pResultFile,"\n\n");
				  free(pResultMatrix);
				  pResultMatrix = NULL;
			  }
			  
			  /* Scores(Tai) - Result */
			  if( (nOption == 2) || (nOption == 1) )
			  {
				  nByteSize = (nTotSamples * nPCs * 1) * sizeof(FLOAT32);
				  nByteSize += offsetof(TMatrix,aData);
				  if(pResultMatrix == NULL)
				  {
					  pResultMatrix = (TMatrix*)malloc (nByteSize / sizeof(char) + sizeof(char));
				  }
				  
				  error = 0;
				  error = molpGetResultMatrix(objHandle, PIDX_TAI, pResultMatrix);
				  if(error != MOLP_OK)
				  {
					  getStrFromErrorCode( error );
					  
					  molpFreeTMobileHandles(objHandle);
					  free(hOptions);
					  Error_Handler();
				  }
				  
				  FPRINTF(pResultFile,"Scores (%d %d %d)\n",pResultMatrix->NumRows,pResultMatrix->NumColumns,pResultMatrix->NumPlanes);
				  
				  for( iDataCtr = 0, iPCsCtr = 1; iPCsCtr <= pResultMatrix->NumRows; iPCsCtr++ )
				  {
					  FPRINTF(pResultFile,"\nPC  %d  ", iPCsCtr);
					  for( iSamCtr = 1; iSamCtr <= pResultMatrix->NumColumns; iSamCtr++ )
					  {
						  for(iVarCtr = 1; iVarCtr <= pResultMatrix->NumPlanes; iVarCtr++ )
						  {
							  fDataBuf = pResultMatrix->aData[iDataCtr++];
							  if( fDataBuf == MISSING ) {
								  FPRINTF(pResultFile,"%c  ",'m' );
							  } else {
								  FPRINTF(pResultFile,"%e  ",fDataBuf );
                }
						  }
					  }
				  }
				  iSamCtr = 0;
				  
				  FPRINTF(pResultFile,"\n\n");
				  free(pResultMatrix);
				  pResultMatrix = NULL;
			  }
			  
			  /* Leverges(Hi) - Result */
			  if( (nOption == 2) || (nOption == 1) )
			  {
				  nByteSize = (nTotSamples * nPCs * 1) * sizeof(FLOAT32);
				  nByteSize += offsetof(TMatrix,aData);
				  if(pResultMatrix == NULL)
				  {
					  pResultMatrix = (TMatrix*)malloc (nByteSize / sizeof(char) + sizeof(char));
				  }
				  
				  error = 0;
				  error = molpGetResultMatrix(objHandle, PIDX_HI, pResultMatrix); /* TBD: Need to handle error codes */
				  if(error != MOLP_OK)
				  {
					  getStrFromErrorCode( error );
					  
					  molpFreeTMobileHandles(objHandle);
					  free(hOptions);
					  Error_Handler();
				  }
				  FPRINTF(pResultFile,"Leverages (%d %d %d)\n",pResultMatrix->NumRows,pResultMatrix->NumColumns,pResultMatrix->NumPlanes);
				  
				  for( iDataCtr = 0, iPCsCtr = 1; iPCsCtr <= pResultMatrix->NumRows; iPCsCtr++ )
				  {
					  FPRINTF(pResultFile,"\nPC %d  ", iPCsCtr);
					  for( iSamCtr = 1; iSamCtr <= pResultMatrix->NumColumns; iSamCtr++ )
					  {
						  for(iVarCtr = 1; iVarCtr <= pResultMatrix->NumPlanes; iVarCtr++ )
						  {
							  fDataBuf = pResultMatrix->aData[iDataCtr++];
							  if( fDataBuf == MISSING ) {
								  FPRINTF(pResultFile,"%c  ",'m' );
							  }  else {
								  FPRINTF(pResultFile,"%e  ",fDataBuf );
                }
						  }
					  }
				  }
				  iSamCtr = 0;
				  
				  FPRINTF(pResultFile,"\n\n");
				  free(pResultMatrix);
				  pResultMatrix = NULL;
			  }
			  
			  /* X-Residuals(Eix) - Result */
			  if(nOption >= 2)
			  {
				  nByteSize = (nTotSamples * (nPCs + 1) * nXVarsPrediction) * sizeof(FLOAT32);
				  nByteSize += offsetof(TMatrix,aData);
				  
				  if(pResultMatrix == NULL)
				  {
					  pResultMatrix = (TMatrix*)malloc (nByteSize / sizeof(char) + sizeof(char));
				  }
				  
				  error = 0;
				  error = molpGetResultMatrix(objHandle, PIDX_EIX, pResultMatrix);
				  if(error != MOLP_OK)
				  {
					  getStrFromErrorCode( error );
					  
					  molpFreeTMobileHandles(objHandle);
					  free(hOptions);
					  Error_Handler();
				  }
				  
				  FPRINTF(pResultFile,"X-Residuals (%d %d %d)\n",pResultMatrix->NumRows,pResultMatrix->NumColumns,pResultMatrix->NumPlanes);
				  
				  for( iDataCtr = 0,iPCsCtr = 1; iPCsCtr <= pResultMatrix->NumPlanes; iPCsCtr++ )
				  {
					  FPRINTF(pResultFile,"\nPC %d  ", iPCsCtr - 1);
					  for( iSamCtr = 1; iSamCtr <= pResultMatrix->NumRows; iSamCtr++ )
					  {
						  if(iPredictType == 0)
							  FPRINTF(pResultFile,"\nSample %d  ", iSamCtr);
						  
						  for(iVarCtr = 1; iVarCtr <= pResultMatrix->NumColumns; iVarCtr++ )
						  {
							  fDataBuf = pResultMatrix->aData[iDataCtr++];
							  if( fDataBuf == MISSING ) {
								  FPRINTF(pResultFile,"%c  ",'m' );
							  } else {
								  FPRINTF(pResultFile,"%e  ",fDataBuf );
                }
						  }
					  }
				  }
				  iSamCtr = 0;
				  
				  FPRINTF(pResultFile,"\n\n");
				  free(pResultMatrix);
				  pResultMatrix = NULL;
			  }
			  
			  /* ResXValSamp - PIDX_VXI  */
			  if(nOption >= 2)
			  {
				  nByteSize = (nTotSamples * (nPCs + 1)) * sizeof(FLOAT32);
				  nByteSize += offsetof(TMatrix,aData);
				  
				  if(pResultMatrix == NULL)
				  {
					  pResultMatrix = (TMatrix*)malloc (nByteSize / sizeof(char) + sizeof(char));
				  }
				  error = 0;
				  error = molpGetResultMatrix(objHandle, PIDX_VXI, pResultMatrix);
				  if(error != MOLP_OK)
				  {
					  getStrFromErrorCode( error );
					  
					  molpFreeTMobileHandles(objHandle);
					  free(hOptions);
					  Error_Handler();
				  }
				  
				  FPRINTF(pResultFile,"ResXValSamp (%d %d %d)\n",pResultMatrix->NumRows,pResultMatrix->NumColumns,pResultMatrix->NumPlanes);
				  
				  for( iDataCtr = 0, iVarCtr = 1; iVarCtr <= pResultMatrix->NumPlanes; iVarCtr++ )
				  {
					  for( iPCsCtr = 1; iPCsCtr <= pResultMatrix->NumRows; iPCsCtr++ )
					  {
						  FPRINTF(pResultFile,"\nPC %d  ", iPCsCtr - 1);
						  for( iSamCtr = 1; iSamCtr <= pResultMatrix->NumColumns; iSamCtr++ )
						  {
							  fDataBuf = pResultMatrix->aData[iDataCtr++];
							  if( fDataBuf == MISSING ) {
								  FPRINTF(pResultFile,"%c  ",'m' );
							  } else {
								  FPRINTF(pResultFile,"%e  ",fDataBuf );
                }
						  }
					  }
				  }
				  iSamCtr = 0;
				  
				  FPRINTF(pResultFile,"\n\n");
				  free(pResultMatrix);
				  pResultMatrix = NULL;
			  }
			  
			  /* ResXValTot - PIDX_PVXTOT */
			  if(nOption >= 2)
			  {
				  nByteSize = (1 * (nPCs + 1)) * sizeof(FLOAT32);
				  nByteSize += offsetof(TMatrix,aData);
				  
				  if(pResultMatrix == NULL)
				  {
					  pResultMatrix = (TMatrix*)malloc (nByteSize / sizeof(char) + sizeof(char));
				  }
				  
				  error = 0;
				  error = molpGetResultMatrix(objHandle, PIDX_PVXTOT, pResultMatrix);
				  if(error != MOLP_OK)
				  {
					  getStrFromErrorCode( error );
					  
					  molpFreeTMobileHandles(objHandle);
					  free(hOptions);
					  Error_Handler();
				  }
				  
				  FPRINTF(pResultFile,"ResXValTot (%d %d %d)\n",pResultMatrix->NumRows,pResultMatrix->NumColumns,pResultMatrix->NumPlanes);
				  
				  for( iDataCtr = 0, iSamCtr = 1; iSamCtr <= pResultMatrix->NumColumns; iSamCtr++ )
				  {
					  for( iPCsCtr = 1; iPCsCtr <= pResultMatrix->NumRows; iPCsCtr++ )
					  {
						  FPRINTF(pResultFile,"\nPC %d  ", iPCsCtr - 1);
						  for(iVarCtr = 1; iVarCtr <= pResultMatrix->NumPlanes; iVarCtr++ )
						  {
							  fDataBuf = pResultMatrix->aData[iDataCtr++];
							  if( fDataBuf == MISSING ) {
								  FPRINTF(pResultFile,"%c  ",'m' );
                } else {
								  FPRINTF(pResultFile,"%e  ",fDataBuf );
                }
						  }
					  }
				  }
				  iSamCtr = 0;
				  
				  FPRINTF(pResultFile,"\n\n");
				  free(pResultMatrix);
				  pResultMatrix = NULL;
			  }
			  
			  /* YDeviation - PIDX_YDEV */
			  if(nOption >= 2)
			  {
				  nByteSize = (nTotSamples * nPCs * 1) * sizeof(FLOAT32);
				  nByteSize += offsetof(TMatrix,aData);
				  if(pResultMatrix == NULL)
				  {
					  pResultMatrix = (TMatrix*)malloc (nByteSize / sizeof(char) + sizeof(char));
				  }
				  
				  error = 0;
				  error = molpGetResultMatrix(objHandle, PIDX_YDEV, pResultMatrix);
				  if(error != MOLP_OK)
				  {
					  getStrFromErrorCode( error );
					  
					  molpFreeTMobileHandles(objHandle);
					  free(hOptions);
					  Error_Handler();
				  }
				  
				  FPRINTF(pResultFile,"YDeviation (%d %d %d)\n",pResultMatrix->NumRows,pResultMatrix->NumColumns,pResultMatrix->NumPlanes);
				  
				  for( iDataCtr = 0, iSamCtr = 1; iSamCtr <= pResultMatrix->NumRows; iSamCtr++ )
				  {
					  if(iPredictType == 0)
						  FPRINTF(pResultFile,"\nSample %d  ", iSamCtr);
					  
					  for( iPCsCtr = 1; iPCsCtr <= pResultMatrix->NumColumns; iPCsCtr++ )
					  {
						  for(iVarCtr = 1; iVarCtr <= pResultMatrix->NumPlanes; iVarCtr++ )
						  {
							  fDataBuf = pResultMatrix->aData[iDataCtr++];
							  if( fDataBuf == MISSING ) {
								  FPRINTF(pResultFile,"%c  ",'m' );
                } else {
								  FPRINTF(pResultFile,"%e  ",fDataBuf );
                }
						  }
					  }
				  }
				  iSamCtr = 0;
				  
				  FPRINTF(pResultFile,"\n\n");
				  free(pResultMatrix);
				  pResultMatrix = NULL;
			  }
			  
			  /* Mahalanobis distance(PIDX_MAHDIST) - Result */
			  if( (nOption == 2) || (nOption == 1) )
			  {
				  nByteSize = (nTotSamples * nPCs * 1) * sizeof(FLOAT32);
				  nByteSize += offsetof(TMatrix,aData);
				  if(pResultMatrix == NULL)
				  {
					  pResultMatrix = (TMatrix*)malloc (nByteSize / sizeof(char) + sizeof(char));
				  }
				  
				  error = 0;
				  error = molpGetResultMatrix(objHandle, PIDX_MAHDIST, pResultMatrix); /* TBD: Need to handle error codes */
				  if(error != MOLP_OK)
				  {
					  getStrFromErrorCode( error );
					  
					  molpFreeTMobileHandles(objHandle);
					  free(hOptions);
					  Error_Handler();
				  }
				  
				  FPRINTF(pResultFile,"Mahalanobis Distance (%d %d %d)\n",pResultMatrix->NumRows,pResultMatrix->NumColumns,pResultMatrix->NumPlanes);
				  
				  for( iDataCtr = 0, iPCsCtr = 1; iPCsCtr <= pResultMatrix->NumRows; iPCsCtr++ )
				  {
					  FPRINTF(pResultFile,"\nPC  %d  ", iPCsCtr);
					  for( iSamCtr = 1; iSamCtr <= pResultMatrix->NumColumns; iSamCtr++ )
					  {
						  for(iVarCtr = 1; iVarCtr <= pResultMatrix->NumPlanes; iVarCtr++ )
						  {
							  fDataBuf = pResultMatrix->aData[iDataCtr++];
							  if( fDataBuf == MISSING ) {
								  FPRINTF(pResultFile,"%c  ",'m' );
							  } else {
								  FPRINTF(pResultFile,"%e  ",fDataBuf );
                }
						  }
					  }
				  }
				  iSamCtr = 0;
				  
				  FPRINTF(pResultFile,"\n\n");
				  free(pResultMatrix);
				  pResultMatrix = NULL;
			  }
   }
   
   fclose( pInDataFile );
   free( pPredictionData );
   pPredictionData = NULL;
   
   molpFreeTMobileHandles( objHandle );
   
   printf("\nPrediction is done. The results are stored in file %s\n", "output.txt");
   
   free( hOptions );  
   fclose( pResultFile );
   
   return MOLP_OK;
}

TFilter* parseFilter( char* sXFilter, UINT32 nType, UINT32 nLimit, UINT32* nTotlElements,UINT32* nError )
{
	UINT32 nNumRanges			= 1;
	UINT32 nCtr					= 0;
	UINT32 nRanges				= 0;
	UINT32 nElements			= 0;
	UINT32 preStartElement		= 0;
	UINT32 preEndElement		= 0;
	
	int nStringPos    = 0;
	int nCount        = 0;	
	
	char delimiters[] = ",";
	char *pszToken    = NULL;
	char *pszSubToken = NULL;
	char *pBuffer	  = NULL;
	char szStart[10]  = "";
	char szEnd[10]    = "";
	
	TFilter* pFilter  = NULL;
	
	// Create buffer for TFilter structure, and initiates pRanges to point
	// at the first TRange object
	
	// Count the number of ranges in the input string
	for( nCtr = 0; nCtr < (int)strlen(sXFilter); nCtr++)
	{
		if(sXFilter[nCtr] == ',')
			nNumRanges++;
	}
	
	pBuffer = (char*)malloc(sizeof(char) * (offsetof(TFilter,aRanges)+ nNumRanges * sizeof(TRange)));
	
	pFilter = (TFilter*)pBuffer;
	pFilter->FilterType  = nType;
	pFilter->NumRanges   = nNumRanges;
	pFilter->Setting     = FS_RANGES;
	
	pszToken = strtok( sXFilter, delimiters );    /* Subdivide the Token wrt to ',' (eg: 2-5,7-11,15) */
	nNumRanges = 0;
	while( pszToken != NULL )
	{
		nStringPos = 0;
		pszSubToken = NULL;
		
		pszSubToken = strrchr( pszToken, '-' ); /* Subdivide the Token wrt to '-' (eg: 2-5) */
		
		if( pszSubToken != NULL )
		{
			nStringPos = pszSubToken - pszToken ;
			
			for( nCtr=0; nCtr < (UINT32)nStringPos; nCtr++ )
			{
				szStart[nCtr] = pszToken[nCtr];
			}
			szStart[nCtr] = '\0';
			
			if( ((UINT32)atoi(szStart) < 1) || ((UINT32)atoi(szStart) > nLimit) )
			{
				*nError = 2;
				free(pFilter);
				pFilter = NULL;
				return pFilter;
			}
			
			if( nNumRanges >= 1 )
			{
				if( (preStartElement == (UINT32)atoi(szStart)) || ( preEndElement == (UINT32)atoi(szStart)) )
				{
					*nError = 1;
					free(pFilter);
					pFilter = NULL;
					return pFilter;
				}
			}
			pFilter->aRanges[nNumRanges].StartElement = preStartElement = atoi(szStart); /* Start Interval*/
			
			nCount=0;
			for( nCtr = (nStringPos + 1); nCtr < (UINT32)strlen(pszToken); nCtr++)
			{
				szEnd[nCount] = pszToken[nCtr];
				nCount++;
			}
			szEnd[nCount] = '\0';
			
			if( ((UINT32)atoi(szEnd) < 1) || ((UINT32)atoi(szEnd) > nLimit) )
			{
				*nError = 2;
				free(pFilter);
				pFilter = NULL;
				return pFilter;
			}
			
			if( nNumRanges >= 1 )
			{
				if( (preStartElement == (UINT32)atoi(szEnd)) || (preEndElement == (UINT32)atoi(szEnd)) )
				{
					*nError = 1;
					free(pFilter);
					pFilter = NULL;
					return pFilter;
				}
			}
			pFilter->aRanges[nNumRanges].EndElement = preEndElement = atoi(szEnd); /* End Interval*/		   
		}
		else
		{
			nStringPos = 0;
			for( nCtr=0; nCtr  < (UINT32)strlen(pszToken); nCtr++ )
			{
				szStart[nCtr] = pszToken[nCtr];
			}
			szStart[nCtr] = '\0';
			
			if( (UINT32)(atoi(szStart) < 1) || ((UINT32)atoi(szStart) > nLimit) )
			{
				*nError = 2;
				free(pFilter);
				pFilter = NULL;
				return pFilter;
			}
			
			if( nNumRanges >= 1 )
			{
				if( (preStartElement == (UINT32)atoi(szStart)) || (preEndElement == (UINT32)atoi(szStart)) )
				{ 
					*nError = 1;
					free(pFilter);
					pFilter = NULL;
					return pFilter;
				}
			}
			
			pFilter->aRanges[nNumRanges].StartElement = preStartElement = atoi(szStart);
			pFilter->aRanges[nNumRanges].EndElement   = preEndElement   = atoi(szStart);
		}
		
		nNumRanges = nNumRanges + 1;
		pszToken = strtok( NULL, delimiters ); /* Scans the token from the point of the current delimiter */
	}
	
	nElements = 0;
	
	for( nRanges = 0; nRanges < pFilter->NumRanges; nRanges++ )
	{
		for( nCtr = pFilter->aRanges[nRanges].StartElement; nCtr <= pFilter->aRanges[nRanges].EndElement; nCtr++ )
		{
			nElements = nElements + 1;
		}
	}
	if( *nTotlElements >= nElements )
		*nTotlElements = nElements;
	else
	{
		*nError = 2;
		free(pFilter);
		pFilter = NULL;
		return pFilter;
	}
	
	return pFilter;
}
