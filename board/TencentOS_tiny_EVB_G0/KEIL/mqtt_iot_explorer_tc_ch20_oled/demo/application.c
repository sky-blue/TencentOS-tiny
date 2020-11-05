#include "tos_k.h"




void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    printf("\tHAL_GPIO_EXTI_Rising_Callback: %d @1105 tick %d\r\n", GPIO_Pin, tos_systick_get());

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    printf("\tHAL_GPIO_EXTI_Falling_Callback: %d tick %d\r\n", GPIO_Pin, tos_systick_get());

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}


