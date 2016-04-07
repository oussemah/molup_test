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
//a = 0x3fe0df41
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
            if ((fres = f_open(&fil, "tests_file.txt", FA_READ)) == FR_OK) {
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
char MODEL_FILE_NAME[8]= "PLS.AMO";
//char MODEL_FILE_NAME[]= "test_file.txt";

void getStrFromErrorCode(uint32_t nError )
{
    printf("Error:%u\n\r", nError);
    return;
}

void molup_test()
{
    typedef uint32_t (*TObjHandle)(),(*myUINT32)();
    typedef void (*myVOID)();

	TMatrix *pPredictionData   = NULL;  /* Matrix to hold Data to be predicted */
	TMatrix *pResultMatrix     = NULL;	/* Matrix to hold the predicted results */
	
	FILE *pResultFile    = NULL;		/* File pointer for to store Result Data */
	FILE *pInDataFile    = NULL;     /* File pointer for input Data */
	FIL *pModelFile = NULL;     /* File pointer for Model (*.AMO) */

	TOptions *hOptions   = NULL;
	TFilter* pTFilter    = NULL;
	TModelInfo modelInfo;
	TObjHandle objHandle;

	
	uint32_t iSamCtr			= 0;     /* Sample Counter */
	uint32_t iVarCtr			= 0;     /* Variable Counter */
	uint32_t iPCsCtr			= 0;     /* PC Counter */	
	uint32_t nSamPrediction   = 0;
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
	
	float fDataBuf			= 0.0f;
	
	char mod[MAXMODULE];
	char sDataBuf;
	char szPredictType[5]	= "ALL";
	char   sAlphaCheck      = 0; /* checks if the user options is alpha */
	char cFilter            = 0;
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

    return ;
}
