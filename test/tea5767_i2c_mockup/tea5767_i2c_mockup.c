#include "tea5767_i2c_mockup.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define DBG_LOG

static uint8_t *i2c_buffer = NULL;
static uint8_t i2c_buffer_length = 0;

void i2c_write_mockup(uint8_t address, uint8_t *write_buffer, uint8_t length) {
    // Free allocated space
    if (i2c_buffer != NULL) {
        free(i2c_buffer);
    }

    // Assing new buffer size and copy data
    i2c_buffer = (uint8_t *)malloc(length);
    if (i2c_buffer == NULL) {
        printf("[ERROR] Cannot allocate memory for i2c_buffer\n");
        return;
    }
    memcpy(i2c_buffer, write_buffer, length);
    i2c_buffer_length = length;

#ifdef DBG_LOG
    printf("[MOCK] Write buffer %p -> i2c_buffer %p \n", (void *)write_buffer, (void *)i2c_buffer);
    for (int i = 0; i < length; i++) { 
        printf("[MOCK] buffer -> %d i2c_buffer %d \n", write_buffer[i], i2c_buffer[i]);
    }
#endif
}

void i2c_read_mockup(uint8_t address, uint8_t *read_buffer, uint8_t length) {
    if (i2c_buffer == NULL || length > i2c_buffer_length) {
        printf("[ERROR] Invalid buffer or reading length exceeds buffer size\n");
        return;
    }

    memcpy(read_buffer, i2c_buffer, length);

#ifdef DBG_LOG
    printf("[MOCK] After read buffer %p <- i2c_buffer %p \n", (void *)read_buffer, (void *)i2c_buffer);
    for (int i = 0; i < length; i++) { 
        printf("[MOCK] buffer <- %d i2c_buffer %d \n", read_buffer[i], i2c_buffer[i]);
    } 
#endif
}
