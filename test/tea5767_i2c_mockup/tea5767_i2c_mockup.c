#include "tea5767_i2c_mockup.h"

static uint8_t *i2c_buffer =  NULL;

void i2c_read_mockup(uint8_t address, uint8_t *buffer, uint8_t length){
    i2c_buffer = buffer;
}

void i2c_write_mockup(uint8_t address, uint8_t *buffer, uint8_t length){
    buffer = i2c_buffer;
}
