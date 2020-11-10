#include "tos_k.h"
#include "tim.h"
#include "cmsis_os.h"


uint32_t time_ms = 0;


typedef enum
{
    IT_RISING   = 1U,
    IT_FALLING  = 2U,
} it_type;


typedef struct interrupt_info
{
    uint16_t GPIO_Pin;
    it_type type;
    uint32_t t_ms;
    uint32_t t_cnt;
} interrupt_info_t;


#define MAIL_IT_INFO_SIZE   4
uint8_t mail_it_info_pool[sizeof(interrupt_info_t) * MAIL_IT_INFO_SIZE];
k_mail_q_t mail_it_info_q;


void entry_printf_interrupt_info(void *arg)
{
    k_err_t err;
    interrupt_info_t mail;
    size_t mail_size;

    uint32_t last_time_us = 0;

    while (K_TRUE) {

//        printf("entry_printf_interrupt_info\r\n");

        err = tos_mail_q_pend(&mail_it_info_q, &mail, &mail_size, TOS_TIME_FOREVER);
        if (err == K_ERR_NONE) {
            TOS_ASSERT(mail_size == sizeof(interrupt_info_t));

            uint32_t time_us = mail.t_ms * 1000 + (50000 - mail.t_cnt);

            if (last_time_us == 0)
                last_time_us = time_us;

            printf("entry_printf_interrupt_info: port=%d, type=%s, t_ms=%d, t_cnt=%d, \ttime_us=%d, \tdelta=%d\n",
                mail.GPIO_Pin, mail.type  == IT_RISING ? "\\\\" : "//", mail.t_ms, mail.t_cnt, time_us, time_us - last_time_us);

            last_time_us = time_us;
        } else {
            printf("tos_mail_q_pend err=%d\n", err);
        }

//        tos_sleep_ms(1000);
    }
}

#define STK_SIZE_TASK_PRINTF    512
k_stack_t stack_task_entry_printf_interrupt_info[STK_SIZE_TASK_PRINTF];



osThreadDef(entry_printf_interrupt_info, osPriorityNormal, 1, STK_SIZE_TASK_PRINTF);


void application_init(void)
{
    tos_mail_q_create(&mail_it_info_q, mail_it_info_pool, MAIL_IT_INFO_SIZE, sizeof(interrupt_info_t));

//    k_task_t task;

//    tos_task_create(&task, "entry_printf_interrupt_info", entry_printf_interrupt_info, NULL, 1, stack_task_entry_printf_interrupt_info, STK_SIZE_TASK_PRINTF, 0);


    osThreadCreate(osThread(entry_printf_interrupt_info), NULL);
}



void tim3_cnt()
{
    printf("\t tim3_cnt: time_ms=%d, CNT=%d, CCR1=%d, CCR2=%d, CCR3=%d, CCR4=%d \r\n"
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

//    printf("\tHAL_GPIO_EXTI_Rising_Callback: %d @1105 tick %d\r\n", GPIO_Pin, tos_systick_get());

    interrupt_info_t info;
    info.GPIO_Pin = GPIO_Pin;
    info.type = IT_RISING;
    info.t_ms = time_ms;
    info.t_cnt = htim3.Instance->CNT;

    tos_mail_q_post(&mail_it_info_q, &info, sizeof(interrupt_info_t));

//    tim3_cnt();
//
//    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

//    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
//    printf("\tHAL_GPIO_EXTI_Falling_Callback: %d tick %d\r\n", GPIO_Pin, tos_systick_get());

    interrupt_info_t info;
    info.GPIO_Pin = GPIO_Pin;
    info.type = IT_FALLING;
    info.t_ms = time_ms;
    info.t_cnt = htim3.Instance->CNT;

    tos_mail_q_post(&mail_it_info_q, &info, sizeof(interrupt_info_t));


//    tim3_cnt();
//
//    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
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




