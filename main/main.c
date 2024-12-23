#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "unistd.h"


#include "tcs3200.h"
#include "i2c-lcd.h"

#include "esp_log.h"


static const char *TAG = "lcd";


#define I2C_MASTER_SCL_IO           GPIO_NUM_13      
#define I2C_MASTER_SDA_IO           GPIO_NUM_12      
#define I2C_MASTER_NUM              0                      
#define I2C_MASTER_FREQ_HZ          400000                     
#define I2C_MASTER_TX_BUF_DISABLE   0                          
#define I2C_MASTER_RX_BUF_DISABLE   0                          
#define I2C_MASTER_TIMEOUT_MS       1000

#define REDMAX         2480
#define REDMIN         950
#define GREENMAX       2340
#define GREENMIN       970
#define BLUEMAX        2440
#define BLUEMIN        1150

static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

int convertR(int redFreq) {
    int d = (int)((redFreq - REDMIN) * (255.0 / (REDMAX - REDMIN)));
    if (d > 255) d = 255;
    else if (d < 0) d = 0;
    return d;
}

int convertG(int greenFreq) {
    int d = (int)((greenFreq - GREENMIN) * (255.0 / (GREENMAX - GREENMIN)));
    if (d > 255) d = 255;
    else if (d < 0) d = 0;
    return d;
}

const char* getColor(int R, int G, int B) {
    if (R > G && R > B && R > 100 ) return "RED";
    else if (G > B && G > R && G > 100) return "GREEN";
    else if (G < B && B > R && B > 100) return "BLUE";
    else if (G < 50 && R < 50 && B < 50) return "BLACK";
    else if (G > 200 && R > 200 && B > 200 ) return "WHITE";
    else return "UNKNOW";
 }

int convertB(int blueFreq) {
    int d = (int)((blueFreq - BLUEMIN) * (255.0 / (BLUEMAX - BLUEMIN)));
    if (d > 255) d = 255;
    else if (d < 0) d = 0;
    return d;
}

void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");
    
    lcd_init();
    lcd_clear();

    setup_pins();
    setup_pcnt();
    while (1) {
        
        int redFreq = get_frequency(0, 0);
        int greenFreq = get_frequency(1, 1);
        int blueFreq = get_frequency(0, 1);
        int R = convertR(redFreq);
        int G = convertG(greenFreq);
        int B = convertB(blueFreq);
        lcd_clear();
        printf("%d %d %d", R, G, B);
        char msg[200];                    
        snprintf(msg, sizeof(msg), "R:%d, G:%d", R, G);
        printf("R = %d Hz, G = %d Hz, B = %d Hz\n", redFreq, greenFreq, blueFreq);
        char a[100];
        snprintf(a, sizeof(a), "B:%d|%s", B,getColor(R, G, B) );

        lcd_put_cur(0, 0);
        lcd_send_string(msg);
        
        lcd_put_cur(1, 0);
        lcd_send_string(a);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
    
}
    