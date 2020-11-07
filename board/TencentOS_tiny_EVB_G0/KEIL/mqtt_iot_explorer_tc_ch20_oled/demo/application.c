#include "tos_k.h"
#include "tim.h"


uint32_t time_ms = 0;


void tim3_cnt()
{
    printf("\t tim3_cnt: time_ms=%d, CNT=%d, CCR1=%d, CCR2=d%, CCR3=%d, CCR4=%d \r\n"
        , time_ms
        , htim3.Instance->CNT
        , htim3.Instance->CCR1
        , htim3.Instance->CCR2
        , htim3.Instance->CCR3
        , htim3.Instance->CCR4
        );
}


void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    printf("\tHAL_GPIO_EXTI_Rising_Callback: %d @1105 tick %d\r\n", GPIO_Pin, tos_systick_get());

    tim3_cnt();

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

//    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    printf("\tHAL_GPIO_EXTI_Falling_Callback: %d tick %d\r\n", GPIO_Pin, tos_systick_get());

    tim3_cnt();

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}












/**
  * @}
  */

/** @defgroup TIM_Exported_Functions_Group9 TIM Callbacks functions
  *  @brief    TIM Callbacks functions
  *
@verbatim
  ==============================================================================
                        ##### TIM Callbacks functions #####
  ==============================================================================
 [..]
   This section provides TIM callback functions:
   (+) TIM Period elapsed callback
   (+) TIM Output Compare callback
   (+) TIM Input capture callback
   (+) TIM Trigger callback
   (+) TIM Error callback

@endverbatim
  * @{
  */

/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

//  printf("%s: 0x%x\r\n", __FUNCTION__, htim);
    time_ms += 50;
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
}

/**
  * @brief  Period elapsed half complete callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  printf("%s: 0x%x\r\n", __FUNCTION__, htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedHalfCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Output Compare callback in non-blocking mode
  * @param  htim TIM OC handle
  * @retval None
  */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  printf("%s: 0x%x\r\n", __FUNCTION__, htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_OC_DelayElapsedCallback could be implemented in the user file
   */
}

/**
  * @brief  Input Capture callback in non-blocking mode
  * @param  htim TIM IC handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  printf("%s: 0x%x\r\n", __FUNCTION__, htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_IC_CaptureCallback could be implemented in the user file
   */
}

/**
  * @brief  Input Capture half complete callback in non-blocking mode
  * @param  htim TIM IC handle
  * @retval None
  */
void HAL_TIM_IC_CaptureHalfCpltCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  printf("%s: 0x%x\r\n", __FUNCTION__, htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_IC_CaptureHalfCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  PWM Pulse finished callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  printf("%s: 0x%x\r\n", __FUNCTION__, htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PWM_PulseFinishedCallback could be implemented in the user file
   */
}

/**
  * @brief  PWM Pulse finished half complete callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  printf("%s: 0x%x\r\n", __FUNCTION__, htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PWM_PulseFinishedHalfCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Hall Trigger detection callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  printf("%s: 0x%x\r\n", __FUNCTION__, htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_TriggerCallback could be implemented in the user file
   */
}

/**
  * @brief  Hall Trigger detection half complete callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_TriggerHalfCpltCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  printf("%s: 0x%x\r\n", __FUNCTION__, htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_TriggerHalfCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Timer error callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  printf("%s: 0x%x\r\n", __FUNCTION__, htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_ErrorCallback could be implemented in the user file
   */
}









