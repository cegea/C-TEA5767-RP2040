/**
 * @file i2c.h
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

void i2c_init(i2c_default, uint16_t speed);
void i2c_read_blocking(i2c_default, uint8_t address, uint8_t *buffer, uint8_t length, bool blocking);
void i2c_write_blocking(i2c_default, uint8_t address, uint8_t *buffer, uint8_t length, bool blocking);
