/**
 ********************************************************************************
 * @file    tea5737_i2c.c
 * @author  Carlos Egea
 * @date    06/06/2023
 * @brief   I2C Driver for NXP FM Radio module.
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/

#include "tea5767_i2c.h"


/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
static TEA5757_t *_radio = NULL;

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/

/*! \brief   Read all five bytes from TEA5767 w/o format.
 *  \ingroup tea5767_i2c
 *
 * Reads five bytes from the TEA5767_t variable and fills the buffer variable.
 *
 * \param radio From type TEA5767_t.
 * \param buffer \ref TEA5767_REGISTERS amount to this variable.
 */
void _tea5767_read_raw(uint8_t *buffer);

/*! \brief   Writes all five bytes from TEA5767 with format.
 *  \ingroup tea5767_i2c
 *
 * Writtes five bytes to the TEA5767_t variable and fills the buffer variable.
 * The data from the radio variable is modified to fit the memory map of TEA5767.
 *
 * \param radio Variable of type TEA5767_t.
 */
void _tea5767_write_registers(void);

/*! @brief Checks if the given frequency is within the limits of the current band mode of the TEA5757 radio.
* This function checks if the given frequency is within the limits of the current band mode of the TEA5757 radio, and adjusts it if necessary.
* @param radio A pointer to a TEA5757_t structure that holds information about the radio, including its current band mode.
* @param freq The frequency to check.
* @return float The frequency adjusted if it is out of bounds for the current band mode, or the original frequency if it is within bounds.
* @note The function assumes that the TEA5757_t structure pointed to by radio has its band_mode field set correctly.
*/
float _tea5767_checkFreqLimits(float freq);

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void _tea5767_read_raw(uint8_t *buffer) {
    i2c_read_blocking(i2c_default, _radio->address, buffer, TEA5767_REGISTERS, false);
}

void _tea5767_write_registers() {
    uint8_t registers[TEA5767_REGISTERS];
    // Calculate the frequency value to be written to the TEA5767 register based on the current radio frequency in MHz. 
    // The calculation takes into account the fixed offset of 225kHz and the 4:1 prescaler used by the TEA5767 module.
    float freq = 4*(_radio->frequency * 1000000 + 225000) / 32768; 
    int integer_freq = freq;
    registers[0] = integer_freq >> 8 | _radio->mute_mode << 7 | _radio->searchMode << 6;
    registers[1] = integer_freq & 0xff;
    registers[2] = _radio->searchUpDown << 7 | _radio->searchLevel << 5 | 1 << 4 | _radio->stereoMode << 3
            | _radio->muteRmode << 2 | _radio->muteLmode << 1;
    registers[3] = _radio->standby << 6 | _radio->band_mode << 5 | 1 << 4 | _radio->softMuteMode << 3 | _radio->hpfMode << 2;
    registers[3] = registers[3] | _radio->stereoNoiseCancelling << 1;
    registers[4] = 0x00;
    i2c_write_blocking(i2c_default, _radio->address, registers, TEA5767_REGISTERS, false);
    // TODO: Use a timer instead.
    sleep_ms(100);
}

void tea5767_init(TEA5757_t *radio){
    _radio = radio;
    _radio->address = 0x60;
    // Default (See datasheet).
    //buf[0] = 0x40;buf[1] = 0x00;buf[2] = 0x90;buf[3] = 0x1E;buf[4] = 0x00;

    _radio->mute_mode = false;
    _radio->searchMode = false;
    _radio->frequency = 102.7;
    _radio->searchUpDown = 1;
    _radio->searchLevel = ADC_HIGH;
    _radio->stereoMode = true;
    _radio->muteLmode = false;
    _radio->muteRmode = false;
    _radio->standby = false;
    _radio->band_mode = EU_BAND;
    _radio->softMuteMode = false;
    _radio->hpfMode = true;
    _radio->stereoNoiseCancelling = true;

    // TODO: Allow other pins than I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    // Start the radio
    _tea5767_write_registers();
}

float tea5767_getStation() {
    uint8_t buf[TEA5767_REGISTERS];

    // Read current settings from the TEA5767 module
    _tea5767_read_raw(buf);

    // Calculate the current frequency based on the TEA5767's register values
    float integer_freq = (buf[0] & 0x3f) << 8 | buf[1];
    _radio->frequency = (integer_freq*32768/4 - 225000) / 1000000;

    return _radio->frequency;
}

int tea5767_getReady() {
    uint8_t buf[TEA5767_REGISTERS];
    _tea5767_read_raw(buf);
    _radio->isReady = buf[0] >> 7;
    return _radio->isReady;
}

void tea5767_setSearch(uint8_t searchMode, uint8_t searchUpDown) {
    _radio->searchUpDown = searchUpDown;
    _radio->searchMode = searchMode;
    _tea5767_write_registers();
}

float _tea5767_checkFreqLimits(float freq) {
    float min_freq = MIN_FREQ_EU;
    float max_freq = MAX_FREQ_EU;

    switch (_radio->band_mode) {
        case JP_BAND:
            min_freq = MIN_FREQ_JP;
            max_freq = MAX_FREQ_JP;
            break;

        case EU_BAND:
            min_freq = MIN_FREQ_EU;
            max_freq = MAX_FREQ_EU;
            break;

        default:
            return freq;
            break;
    }

    if (freq > max_freq) {
        freq = max_freq;
        //printf("Warning: Frequency out of range. Setting to maximum value.\n");
    } else if (freq < min_freq) {
        freq = min_freq;
        //printf("Warning: Frequency out of range. Setting to minimum value.\n");
    }

    return freq;
}

void tea5767_setStation(float freq) {
    _radio->frequency = _tea5767_checkFreqLimits(freq);
    _tea5767_write_registers();
}

void tea5767_setStationInc(float freq) {
    // Set search mode based on sign of freq
    if (freq < 0) {
        tea5767_setSearch(1, 0);
    } else {
        tea5767_setSearch(1, 1);
    }

    // Calculate new frequency and verify it's within limits
    float new_freq = _radio->frequency + freq;
    _radio->frequency = _tea5767_checkFreqLimits(new_freq);

    // Update registers with new frequency
    _tea5767_write_registers();
}

void tea5767_setMute(bool mute) {
    _radio->mute_mode = mute;
    _tea5767_write_registers();
}

void tea5767_setMuteLeft(bool mute) {
    _radio->muteLmode = mute;
    _tea5767_write_registers();
}

void tea5767_setMuteRight(bool mute) {
    _radio->muteRmode = mute;
    _tea5767_write_registers();
}

void tea5767_setStandby(bool standby) {
    _radio->standby = standby;
    _tea5767_write_registers();
}

void tea5767_setStereo(bool stereo) {
    _radio->stereoMode = !stereo;
    _tea5767_write_registers();
}


