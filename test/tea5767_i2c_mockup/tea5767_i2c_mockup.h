/**
 * @file tea5767_i2c_mockup.h
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

#ifndef _HARDWARE_TEA5767_I2C_MOCKUP_H
#define _HARDWARE_TEA5767_I2C_MOCKUP_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

uint8_t* get_i2c_buffer(void);
void i2c_read_mockup(uint8_t address, uint8_t *buffer, uint8_t length);
void i2c_write_mockup(uint8_t address, uint8_t *buffer, uint8_t length);

#endif