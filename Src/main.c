/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "sdio.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
//------------------------------------------------------------------------------
/* Projeto    : STM32F4-DIS_SDcard_FatFS
 * MCU        : STM32F407VG (STM32F4-DISCOVERY)
 * Copyright  : Atribuição (CC BY 4.0) Diego Moreno
 * Versão     : 1.0.0
 * Data	      : 15/02/2017
 * Descrição  : Cria um arquivo TXT em um SDCard conectado na placa
 *              STM32F4-DISCOVERY com a STM32F4DIS-BB.
 */

/* Includes */


//------------------------------------------------------------------------------
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

//SD Variables
FATFS SDFatFs;                                  //File system object for SD disk logical drive
FIL MyFile;                                     //File object
extern char SD_Path[4];                         //SD logical drive path
FRESULT res;                                    //FatFs function common result code
uint32_t byteswritten, bytesread;               //File write/read counts
uint8_t wtext[] = "Congrats, it's working :)";  //File write buffer
uint8_t rtext[100];                             //File read buffer

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void uSD_Application(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FATFS_Init();
  MX_SDIO_SD_Init();

  /* USER CODE BEGIN 2 */
  
  //Unlink USB disk Drivers
  FATFS_UnLinkDriver(SD_Path);
  
  uSD_Application();
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

void uSD_Application(void)
{
  //Link SD disk Drivers
  FATFS_LinkDriver(&SD_Driver, SD_Path);
  
  //Register the file system object to the FatFs module
  if(f_mount(&SDFatFs, (TCHAR const*)SD_Path, 0) != FR_OK)
  {
    //FatFs Initialization Error
    Error_Handler();
  }
  else
  {
    //Create and Open a new text file object with write access
    if(f_open(&MyFile, "IT_WORKS.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
      //'IT_WORKS.TXT' file Open for write Error
      Error_Handler();
    }
    else
    {
      //Write data to the text file
      res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
      
      if((byteswritten == 0) || (res != FR_OK))
      {
        //'IT_WORKS.TXT' file Write or EOF Error
        Error_Handler();
      }
      else
      {
        //Close the open text file
        f_close(&MyFile);
        
        //Open the text file object with read access
        if(f_open(&MyFile, "IT_WORKS.TXT", FA_READ) != FR_OK)
        {
          //'IT_WORKS.TXT' file Open for read Error
          Error_Handler();
        }
        else
        {
          //Read data from the text file
          res = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&bytesread);
          
          if((bytesread == 0) || (res != FR_OK)) /* EOF or Error */
          {
            //'IT_WORKS.TXT' file Read or EOF Error
            Error_Handler();
          }
          else
          {
            //Close the open text file
            f_close(&MyFile);
            
            //Compare read data with the expected data
            if ((bytesread != byteswritten))
            {                
              //Read data is different from the expected data
              Error_Handler(); 
            }
            else
            {
              //Success of the demo: no error occurrence: Turn LED4_GREEN ON
              HAL_GPIO_WritePin(GPIOD, LED4_GREEN_Pin, GPIO_PIN_SET);
            }
          }
        }
      }
    }
  }
  
  //Unlink USB disk Drivers
  FATFS_UnLinkDriver(SD_Path);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  
  //Turn LED5_RED ON
  HAL_GPIO_WritePin(GPIOD, LED5_RED_Pin, GPIO_PIN_SET);
  
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
