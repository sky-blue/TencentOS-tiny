#include "tos_k.h"
#include "esp8266_tencent_firmware.h"
#include "tencent_firmware_module_wrapper.h"
#include "ch20_parser.h"
#include "oled.h"


#define PRODUCT_ID              "16AMSMHX5A"
#define DEVICE_NAME             "dev_608"
#define DEVICE_KEY              "7JmkXHFeeckSrpBUEFhqMw=="

#define REPORT_DATA_TEMPLATE    "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"ch20_ppm_value\\\":%.3f}}"
#define REPORT_DATA_RING        "{\\\"method\\\":\\\"report\\\"\\,\\\"clientToken\\\":\\\"00000001\\\"\\,\\\"params\\\":{\\\"ring\\\":%d}}"


void default_message_handler(mqtt_message_t* msg)
{
    if (strstr(msg->payload, "led1_switch") != NULL)
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    }

    printf("\r\nxxx1 callback:\r\n");
    printf("---------------------------------------------------------\r\n");
    printf("\ttopic:%s\r\n", msg->topic);
    printf("\tpayload:%s\r\n", msg->payload);
    printf("---------------------------------------------------------\r\n");
/*}

void default_message_handler(mqtt_message_t* msg)
{*/
    return;

    char out[300] = {0};
    char tmp[100];

    strcat(out, "\r\nxxx2 callback:\r\n");
    strcat(out, "---------------------------------------------------------\r\n");

    snprintf(tmp, 100, "\ttopic:%s\r\n", msg->topic);
    strcat(out, tmp);

    snprintf(tmp, 100, "\tpayload:%s\r\n", msg->payload);
    strcat(out, tmp);

    strcat(out, "---------------------------------------------------------\r\n");

    printf("%s", out);
}

char payload[256] = {0};
static char report_topic_name[TOPIC_NAME_MAX_SIZE] = {0};
static char report_reply_topic_name[TOPIC_NAME_MAX_SIZE] = {0};

k_mail_q_t mail_q;
ch20_data_t ch20_value;
uint8_t ch20_value_pool[5 * sizeof(ch20_data_t)];

void mqtt_demo_task(void)
{
    int ret = 0;
    int size = 0;
    int lightness = 0;
    mqtt_state_t state;

    char *product_id = PRODUCT_ID;
    char *device_name = DEVICE_NAME;
    char *key = DEVICE_KEY;

    device_info_t dev_info;
    memset(&dev_info, 0, sizeof(device_info_t));

    size_t mail_size;
    float  ch20_ppm_value;
    char   ch20_ppm_str[20];


    /* OLED显示日志 */
    OLED_ShowString(0, 2, (uint8_t*)"connecting...", 16);

    /**
     * Please Choose your AT Port first, default is HAL_UART_2(USART2)
    */
    ret = esp8266_tencent_firmware_sal_init(HAL_UART_PORT_2);

    if (ret < 0) {
        printf("esp8266 tencent firmware sal init fail, ret is %d\r\n", ret);
    }

    esp8266_tencent_firmware_join_ap("HUAWEI Mate 40 Pro", "0755608608");

    strncpy(dev_info.product_id, product_id, PRODUCT_ID_MAX_SIZE);
    strncpy(dev_info.device_name, device_name, DEVICE_NAME_MAX_SIZE);
    strncpy(dev_info.device_serc, key, DEVICE_SERC_MAX_SIZE);
    tos_tf_module_info_set(&dev_info, TLS_MODE_PSK);

    mqtt_param_t init_params = DEFAULT_MQTT_PARAMS;
    if (tos_tf_module_mqtt_conn(init_params) != 0) {
        printf("module mqtt conn fail\n");
    } else {
        printf("module mqtt conn success\n");
    }

    if (tos_tf_module_mqtt_state_get(&state) != -1) {
        printf("MQTT: %s\n", state == MQTT_STATE_CONNECTED ? "CONNECTED" : "DISCONNECTED");
    }

    /* 开始订阅topic */
    size = snprintf(report_reply_topic_name, TOPIC_NAME_MAX_SIZE, "$thing/down/property/%s/%s", product_id, device_name);

    if (size < 0 || size > sizeof(report_reply_topic_name) - 1) {
        printf("sub topic content length not enough! content size:%d  buf size:%d", size, (int)sizeof(report_reply_topic_name));
    }
    if (tos_tf_module_mqtt_sub(report_reply_topic_name, QOS0, default_message_handler) != 0) {
        printf("module mqtt sub fail\n");
    } else {
        printf("module mqtt sub success\n");
    }

    memset(report_topic_name, sizeof(report_topic_name), 0);
    size = snprintf(report_topic_name, TOPIC_NAME_MAX_SIZE, "$thing/up/property/%s/%s", product_id, device_name);

    if (size < 0 || size > sizeof(report_topic_name) - 1) {
        printf("pub topic content length not enough! content size:%d  buf size:%d", size, (int)sizeof(report_topic_name));
    }

    /* 创建邮箱 */
    tos_mail_q_create(&mail_q, ch20_value_pool, 3, sizeof(ch20_data_t));

    HAL_NVIC_DisableIRQ(USART3_4_IRQn);

    if (ch20_parser_init() == -1) {
        printf("ch20 parser init fail\r\n");
        return;
    }

    while (1) {
        /* 通过接收邮件来读取数据 */
        HAL_NVIC_EnableIRQ(USART3_4_IRQn);
        tos_mail_q_pend(&mail_q, (uint8_t*)&ch20_value, &mail_size, TOS_TIME_FOREVER);
        HAL_NVIC_DisableIRQ(USART3_4_IRQn);

        /* 接收到之后打印信息 */
        ch20_ppm_value = ch20_value.data / 1000.0;
        printf("ch20 value: %.3f\r\n", ch20_ppm_value);

        /* OLED显示值 */
        sprintf(ch20_ppm_str, "%.3f ppm(mg/m3)", ch20_ppm_value);
        OLED_ShowString(0, 2, (uint8_t*)ch20_ppm_str, 16);

        /* 上报值 */
        memset(payload, 0, sizeof(payload));
        snprintf(payload, sizeof(payload), REPORT_DATA_TEMPLATE, ch20_ppm_value);

        if (lightness > 100) {
            lightness = 0;
        }

        int err = tos_tf_module_mqtt_pub(report_topic_name, QOS0, payload);
        if (err != 0)
        {
            printf("module mqtt pub fail: %d\n", err);
        }
        else
        {
            printf("module mqtt pub success\n");
        }

//        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

        tos_sleep_ms(10000);
    }
}

void report_ring(int door)
{
    door += 600;

    char payload[256] = {0};
    snprintf(payload, sizeof(payload), REPORT_DATA_RING, door);

    int err = tos_tf_module_mqtt_pub(report_topic_name, QOS0, payload);

    if (err != 0)
    {
        printf("module mqtt pub fail: %d\n", err);
    }
    else
    {
        printf("module mqtt pub success\n");
    }
}


extern void application_init(void);

void application_entry(void *arg)
{
    char *str = "TencentOS-tiny";

    /* 初始化OLED */
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)str, 16);

    application_init();

    mqtt_demo_task();
    while (1) {
        printf("_.");
//        printf("This is a mqtt demo!\r\n");
//        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        tos_task_delay(10000);
    }
}

