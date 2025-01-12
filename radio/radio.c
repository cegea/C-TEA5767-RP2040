/**
 ********************************************************************************
 * @file    radio.c
 * @author  Carlos Egea
 * @date    06/06/2023
 * @brief   Example for I2C TEA5767 radio module.
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "pico.h"
#include <tea5767_i2c.h>
#include <pico/stdlib.h>

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define CLK_PIN 16
#define DT_PIN 17
#define DUMY_LED_PIN 0

void dummy_blink(int led_pin){
    gpio_put(led_pin,1);
    sleep_ms(500);
    gpio_put(led_pin,0);
    sleep_ms(500);
}

void gpio_callback(uint /*gpio*/, uint32_t /*event_mask*/) {
    ;;
}

int main(){    

    stdio_init_all();
    stdio_printf("Hello, TEA5757! Reading raw data from registers...\n");
    
    gpio_init(DUMY_LED_PIN);    
    gpio_set_dir(DUMY_LED_PIN, GPIO_OUT);  

    gpio_init(CLK_PIN);
    gpio_init(DT_PIN);
    
    gpio_set_dir(CLK_PIN, GPIO_IN);
    gpio_set_dir(DT_PIN, GPIO_IN);  

    gpio_set_irq_enabled_with_callback(CLK_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(DT_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    // TODO: Allow other pins than I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    // Create the radio device and initialize it
    TEA5757_t radio = {0};
    tea5767_init(&radio);
    
    int encState = (gpio_get(CLK_PIN) << 4) | gpio_get(DT_PIN);

    sleep_ms(5000);
    // Clear terminal 
    stdio_printf("\e[1;1H\e[2J");
    sleep_ms(500);
    
    tea5767_setStation(100.7);
    while(true){
        
        //tea5767_unmute();
        tea5767_setMuteLeft(false);
        tea5767_setMuteRight(true);
         sleep_ms(5000);
        // Clear terminal 
        //printf("\e[1;1H\e[2J");
        tea5767_getStation(&radio);
        dummy_blink(DUMY_LED_PIN);
        //printf("Station: %.2f\n",radio.frequency);
        //tea5767_setStation(101.7);
        //tea5767_setStationInc(-2.0);
        tea5767_setMuteLeft(true);
        tea5767_setMuteRight(false);
        sleep_ms(2000);
    }
    return 0;
}
