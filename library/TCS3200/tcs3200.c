#include "tcs3200.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void setup_pins() {
    gpio_set_direction(S0, GPIO_MODE_OUTPUT);
    gpio_set_direction(S1, GPIO_MODE_OUTPUT);
    gpio_set_direction(S2, GPIO_MODE_OUTPUT);
    gpio_set_direction(S3, GPIO_MODE_OUTPUT);
    gpio_set_direction(sensorOut, GPIO_MODE_INPUT);

    gpio_set_level(S0, 1);  // S0 HIGH
    gpio_set_level(S1, 0);  // S1 LOW
}

void setup_pcnt() {
    pcnt_config_t pcnt_config = {
        .pulse_gpio_num = sensorOut,
        .ctrl_gpio_num = PCNT_PIN_NOT_USED,
        .channel = PCNT_CHANNEL_0,
        .unit = PCNT_UNIT,
        .pos_mode = PCNT_COUNT_INC,
        .neg_mode = PCNT_COUNT_DIS,
        .lctrl_mode = PCNT_MODE_KEEP,
        .hctrl_mode = PCNT_MODE_KEEP,
        .counter_h_lim = 32767,
        .counter_l_lim = -32768,
    };

    pcnt_unit_config(&pcnt_config);
    pcnt_set_filter_value(PCNT_UNIT, 1000);
    pcnt_filter_enable(PCNT_UNIT);
    pcnt_counter_pause(PCNT_UNIT);
    pcnt_counter_clear(PCNT_UNIT);
    pcnt_counter_resume(PCNT_UNIT);
}

int get_frequency(int s2_level, int s3_level) {
    gpio_set_level(S2, s2_level);
    gpio_set_level(S3, s3_level);
    vTaskDelay(pdMS_TO_TICKS(100));

    int16_t count = 0;
    pcnt_counter_clear(PCNT_UNIT);
    vTaskDelay(pdMS_TO_TICKS(1000));  // Count for 1 second
    pcnt_get_counter_value(PCNT_UNIT, &count);
    return count;
}
