#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/i2c.h>
#include <esp_timer.h>

#define I2C_MASTER_SCL_IO    22
#define I2C_MASTER_SDA_IO    21
#define I2C_MASTER_NUM       I2C_NUM_0
#define I2C_MASTER_FREQ_HZ   100000

#define HDC2010_ADDR         0x40

static const char *TAG = "HDC2010";





uint16_t get_temp() {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    esp_err_t ret;
    uint8_t temp[2];
    uint8_t reg_add=0x00;

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (HDC2010_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_add, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (HDC2010_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, temp, sizeof(temp), I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 2000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C communication error: %s", esp_err_to_name(ret));
        return 0;
    }
    return  (uint16_t) ((temp[1] <<8) | temp[0]);
}
uint16_t get_hum(){
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	esp_err_t ret;
	uint8_t hum[2],reg_add=0x02;

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (HDC2010_ADDR << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, reg_add, true);
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (HDC2010_ADDR << 1) | I2C_MASTER_READ, true);
	i2c_master_read(cmd, hum, sizeof(hum), I2C_MASTER_LAST_NACK);
	i2c_master_stop(cmd);

	ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 2000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);

	if (ret != ESP_OK) {
	   ESP_LOGE(TAG, "I2C communication error: %s", esp_err_to_name(ret));
	   return 0;
	}
	return  (uint16_t) ((hum[1] <<8) | hum[0]);

}
void app_main() {

    uint64_t current_time;
    current_time=esp_timer_get_time();

    ESP_LOGI(TAG, "Time taken before i2c intialization: %llu microseconds", current_time);
    i2c_config_t conf;
    esp_err_t ret;

    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;

    ret = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C parameter configuration error: %s", esp_err_to_name(ret));
        return;
    }

    ret = i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C driver installation error: %s", esp_err_to_name(ret));
        return;
    }
    uint16_t rawT,rawH;
    uint8_t config[] = {0x0F, 0x01};

    current_time=esp_timer_get_time();

    ESP_LOGI(TAG, "Time taken after i2c initialization: %llu microseconds", current_time);



    float temp;
    int hum;

    while (1) {

    	    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    	    i2c_master_start(cmd);//Sensor Initialization
    	    i2c_master_write_byte(cmd, (HDC2010_ADDR << 1) | I2C_MASTER_WRITE, true);
    	    i2c_master_write(cmd, config, sizeof(config), true);
    	    i2c_master_stop(cmd);

    	    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 2000 / portTICK_PERIOD_MS);
    	    i2c_cmd_link_delete(cmd);

    	    if (ret != ESP_OK) {
    	        ESP_LOGE(TAG, "I2C configuration error: %s", esp_err_to_name(ret));
    	        return;
    	    }

        rawT = get_temp(); // fetch temperature from register
        rawH = get_hum();//fetch humidity from register
        temp = ((rawT / 65536.0) * 165.0 - 40.0);
        hum = ((rawH/65536.0)*100);

            ESP_LOGI(TAG, "Temperature: %.2f °C", temp);
            ESP_LOGI(TAG,"Humidity: %d",hum);
            current_time = esp_timer_get_time();
            ESP_LOGI(TAG, "Time taken after reading temp and hum : %llu microseconds", current_time);



        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
