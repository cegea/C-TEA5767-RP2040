#include "tea5767_i2c_mockup.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdint.h> 

#define DBG_LOG

// This struct helps with the logic and data.
typedef struct{
    bool ready;
    bool stereo;
    bool mute;
    uint8_t ifCounter;
    uint8_t adcCounter;
    uint8_t id;
} simulated_tea5767_t;

static simulated_tea5767_t simulated_tea5767 = {
    .ready = 1,
    .adcCounter = 1,
    .stereo = 0,
    .ifCounter = 5,
    .id = 0x12
};

static uint8_t *i2c_buffer = NULL;
static uint8_t i2c_buffer_length = 0;

uint8_t* get_i2c_buffer(){
    return i2c_buffer;
}

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

    // Map to simulated device
    simulated_tea5767.stereo = (i2c_buffer[2] & 0x04) >> 3; 

}

void i2c_read_mockup(uint8_t address, uint8_t *read_buffer, uint8_t length) {
    if (i2c_buffer == NULL || length > i2c_buffer_length) {
        printf("[ERROR] Invalid buffer or reading length exceeds buffer size\n");
        return;
    }

    // Map data from write buffer to read buffer
    i2c_buffer[0] = simulated_tea5767.ready << 7; // Always ready
    i2c_buffer[0] |= 0 << 6; // TODO: Logic to test band limits
    // PLL data is in the same position.
    i2c_buffer[2] = simulated_tea5767.stereo << 7;
    i2c_buffer[2] = simulated_tea5767.ifCounter & 0x7f;
    i2c_buffer[3] = simulated_tea5767.adcCounter << 4;
    i2c_buffer[4] = 0;

    memcpy(read_buffer, i2c_buffer, length);

#ifdef DBG_LOG
    printf("[MOCK] After read buffer %p <- i2c_buffer %p \n", (void *)read_buffer, (void *)i2c_buffer);
    for (int i = 0; i < length; i++) { 
        printf("[MOCK] buffer <- %d i2c_buffer %d \n", read_buffer[i], i2c_buffer[i]);
    } 
#endif
}
