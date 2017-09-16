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

int main(void)
{
	/******** まずはLチカを実装 ********/
	/* LD2 はGPIO PA5(pin21)もしくはPB13(pin34)と接続 */
	/* Highで点灯。Lowで消灯 */

	// レジスタ設定

	// 初期化
	BSP_LED_Init(LED2);
	BSP_LED_Off(LED2);

	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
	for(;;) {
		uint32_t pb_val = BSP_PB_GetState(BUTTON_KEY);
		// ボタンが押されたらLEDの点灯・消灯を切り替える
		if (pb_val == GPIO_PIN_RESET) {
			BSP_LED_Toggle(LED2);
		}
	}
}
