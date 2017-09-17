/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"

/* ======== 定数 ======== */
#define I2C_ADDRESS 0x45 // 温湿度センサーのI2Cアドレス
#define DATA_SIZE 256

static void Error_Handler(void)
{
  while(1)
  {
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);
    HAL_Delay(40);
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows:
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 16
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 6
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSI Oscillator and activate PLL with HSI as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 0x10;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 360;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	RCC_OscInitStruct.PLL.PLLR = 6;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/* Activate the OverDrive to reach the 180 MHz Frequency */
	ret = HAL_PWREx_EnableOverDrive();
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}
	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}

}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable GPIO Clocks #################################################*/
  /* Enable GPIO TX/RX clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* I2C TX GPIO pin configuration  */
  GPIO_InitStruct.Pin = GPIO_PIN_8; // SCL
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_9; // SDA
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*##-3- Enable I2C peripheral Clock ########################################*/
  /* Enable I2C1 clock */
  __HAL_RCC_I2C1_CLK_ENABLE();

}

void HAL_I2C_Generate(I2C_HandleTypeDef *hi2c) {
	hi2c->Instance             = I2C1;
	hi2c->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	hi2c->Init.ClockSpeed      = 400000;
	hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c->Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
	hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	hi2c->Init.OwnAddress1     = I2C_ADDRESS;
	hi2c->Init.OwnAddress2     = 0;

	if (HAL_I2C_Init(hi2c) != HAL_OK) {
		// Error
		Error_Handler();
	}
}

int main(void)
{
	uint8_t rec_data[DATA_SIZE];
	uint16_t rec_size = DATA_SIZE;
	/******** まずはLチカを実装 ********/
	/* LD2 はGPIO PA5(pin21)もしくはPB13(pin34)と接続 */
	/* Highで点灯。Lowで消灯 */

	// レジスタ設定

	// 初期化
	HAL_Init();
	SystemClock_Config();
	BSP_LED_Init(LED2);
	BSP_LED_Off(LED2);
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
	I2C_HandleTypeDef i2c_handle;
	HAL_I2C_Generate(&i2c_handle);
	HAL_I2C_MspInit(&i2c_handle);

	// I2Cマスター受信
	while(HAL_I2C_Master_Receive(&i2c_handle, (uint16_t)I2C_ADDRESS, (uint8_t *)rec_data, rec_size, 10000) != HAL_OK) {
		if (HAL_I2C_GetError(&i2c_handle) != HAL_I2C_ERROR_AF) {
			// Error
			printf("Error");
			Error_Handler();
		}
	}

	for(;;) {
		uint32_t pb_val = BSP_PB_GetState(BUTTON_KEY);
		// ボタンが押されたらLEDの点灯・消灯を切り替える
		if (pb_val == GPIO_PIN_RESET) {
			HAL_Delay(20);
			BSP_LED_Toggle(LED2);
		}
	}
}
