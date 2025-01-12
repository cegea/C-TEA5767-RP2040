#include "tea5767_i2c_mockup.h"
#include <stdio.h>
#include <string.h> 

#define DBG_LOG

static uint8_t *i2c_buffer =  NULL;

void i2c_write_mockup(uint8_t address, uint8_t *write_buffer, uint8_t length){
    i2c_buffer = write_buffer;
#ifdef DBG_LOG
    printf("[MOCK] Wirte buffer %d -> i2c_buffer %d \n",write_buffer, i2c_buffer);
    for (int i = 0; i < length; i++) { 
        printf("[MOCK] buffer -> %d i2c_buffer %d \n",write_buffer[i],i2c_buffer[i]);
    }
#endif
}

void i2c_read_mockup(uint8_t address, uint8_t *read_buffer, uint8_t length){
    memcpy(read_buffer, i2c_buffer, length);
#ifdef DBG_LOG
    printf("[MOCK] After read buffer %d <- i2c_buffer %d \n",read_buffer, i2c_buffer);
    for (int i = 0; i < length; i++) { 
        printf("[MOCK] buffer <- %d i2c_buffer %d \n",read_buffer[i],i2c_buffer[i]);
    } 
#endif
}
