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

/* ======== �萔 ======== */

int main(void)
{
	/******** �܂���L�`�J������ ********/
	/* LD2 ��GPIO PA5(pin21)��������PB13(pin34)�Ɛڑ� */
	/* High�œ_���BLow�ŏ��� */

	// ���W�X�^�ݒ�

	// ������
	BSP_LED_Init(LED2);
	BSP_LED_Off(LED2);

	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
	for(;;) {
		uint32_t pb_val = BSP_PB_GetState(BUTTON_KEY);
		// �{�^���������ꂽ��LED�̓_���E������؂�ւ���
		if (pb_val == GPIO_PIN_RESET) {
			BSP_LED_Toggle(LED2);
		}
	}
}
