/**
 * @file gpio.h
 * @author cegea
 * @brief 
 * @version 0.1
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 * This file is a mockup for testing
 * 
 */

#define PICO_DEFAULT_I2C_SCL_PIN 1
#define PICO_DEFAULT_I2C_SDA_PIN 2
#define GPIO_FUNC_I2C 3


void gpio_set_function(uint8_t pin, uint8_t mode);
void gpio_pull_up(uint8_t pin);
void bi_2pins_with_func(uint8_t pin, uint8_t pin, uint8_t mode);
void bi_decl(uint8_t tbd);
