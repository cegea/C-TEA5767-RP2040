/**
 ********************************************************************************
 * @file    tea5737_i2c.c
 * @author  Carlos Egea
 * @date    06/06/2023
 * @brief   I2C Driver for NXP FM Radio module.
 ********************************************************************************
 */

#ifndef _HARDWARE_TEA5767_I2C_H
#define _HARDWARE_TEA5767_I2C_H

/************************************
 * INCLUDES
 ************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#ifdef RASPBERRYPI_PICO
#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include "pico/binary_info.h"
#elif UNIT_TESTING
#include <tea5767_i2c_mockup.h>
#endif
/************************************
 * MACROS AND DEFINES
 ************************************/
#define DUMY_LED_PIN 0 // Pin number for a dummy LED
#define MAX_FREQ_JP 91.0 // Maximum frequency limit for the Japanese band
#define MAX_FREQ_EU 108.0 // Maximum frequency limit for the European band
#define MIN_FREQ_JP 76.0 // Minimum frequency limit for the Japanese band
#define MIN_FREQ_EU 87.5 // Minimum frequency limit for the European band
#define JP_BAND 1 // Constant for the Japanese band mode
#define EU_BAND 0 // Constant for the European band mode
#define ADC_LOW 5 // Constant for the low level of ADC readings
#define ADC_MID 7 // Constant for the medium level of ADC readings
#define ADC_HIGH 10 // Constant for the high level of ADC readings
#define TEA5767_REGISTERS 5 // Number of registers in the TEA5767 chip

/************************************
 * TYPEDEFS
 ************************************/
/*! @brief The TEA5757 radio module configuration structure
*/
typedef struct {
uint8_t address;                //< I2C device address
uint8_t mute_mode;              //< Audio mute mode
uint8_t band_mode;              //< Frequency band mode
uint8_t standby;                // Standby mode
uint8_t searchMode;             // Search mode
uint8_t searchUpDown;           // Search up-down mode
uint8_t searchLevel;            // Search level
uint8_t stereoMode;             // Stereo mode
uint8_t muteLmode;              // Left channel mute mode
uint8_t muteRmode;              // Right channel mute mode
uint8_t stereoNoiseCancelling;  // Stereo noise cancelling mode
uint8_t softMuteMode;           // Soft mute mode
uint8_t hpfMode;                // High pass filter mode
uint8_t isReady;                // Radio is ready flag
uint8_t isStereo;               // Stereo mode flag
uint8_t stationLevel;           // Station level
float frequency;                // Frequency in MHz
} TEA5757_t;

/************************************
 * EXPORTED VARIABLES
 ************************************/
static const TEA5757_t default_cfg = {
    .address = 0x60,
    .mute_mode = false,
    .searchMode = false,
    .frequency = 102.7,
    .searchUpDown = 1,
    .searchLevel = ADC_HIGH,
    .stereoMode = true,
    .muteLmode = false,
    .muteRmode = false,
    .standby = false,
    .band_mode = EU_BAND,
    .softMuteMode = false,
    .hpfMode = true,
    .stereoNoiseCancelling = true
};

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
// ----------------------------------------------------------------------------
// Setup

/*! \brief   Initialize an struct with the parameters needed for initialization.
 *  \ingroup tea5767_i2c
 *
 * TO DO
 *
 * \param radio Variable of type TEA5767_t.
 */
void tea5767_init(TEA5757_t *radio);

/*! @brief Gets the current station frequency from the TEA5757 radio and prints it to stdout.
* This function reads the raw data from the TEA5757 radio using the _tea5767_read_raw() function,
* extracts the frequency values from the read buffer, and calculates the frequency in MHz.
* The extracted frequency is then printed to stdout with two decimal places.
* @param radio The TEA5757_t structure representing the radio device.
* @note This function assumes that the TEA5757 radio device has been initialized and is currently powered on.
* @return void
*/
float tea5767_getStation();

/*! @brief Initializes the TEA5757_t structure for the TEA5757 tuner.
* This function initializes the TEA5757_t structure with the default values for the TEA5757 tuner.
* The default I2C address of the tuner is 0x60.
* @return TEA5757_t The TEA5757_t structure initialized with default values.
* @note This function must be called before using any other function related to the TEA5757 tuner.
*/
int tea5767_getReady();

/*! @brief Configures the search mode and direction of the TEA5757 tuner.
* This function sets the search mode and direction of the TEA5757 tuner. It updates the values of the TEA5757_t structure
* with the given search mode and search direction and then writes them to the tuner using the _tea5767_write_registers() function.
* @param radio A pointer to the TEA5757_t structure representing the tuner.
* @param searchMode An 8-bit unsigned integer specifying the search mode to use. Valid values are:

* TEA5767_SEARCH_MUTE_LOW
* TEA5767_SEARCH_MUTE_HIGH
* TEA5767_SEARCH_MUTE_BW
* TEA5767_SEARCH_MUTE_OFF
* TEA5767_SEARCH_MUTE_SW
* @param searchUpDown An 8-bit unsigned integer specifying the direction of the search. Valid values are:
* TEA5767_SEARCH_UP
* TEA5767_SEARCH_DOWN
* @note This function updates the TEA5757_t structure pointed to by radio and writes the updated values to the tuner.
*/
void tea5767_setSearch(uint8_t searchMode, uint8_t searchUpDown);

/*! @brief Sets the frequency of the TEA5757 tuner.
* This function sets the frequency of the TEA5757 tuner to the given value.
* If the frequency is out of range for the current band mode, it will be adjusted to the nearest valid frequency.
* After setting the frequency, the new value will be written to the tuner through the _tea5767_write_registers function.
* @param radio A pointer to a TEA5757_t structure representing the tuner.
* @param freq The desired frequency to set the tuner to.
* @return void
* @note The tuner must be initialized and ready before calling this function.
*/
void tea5767_setStation(float freq);

/*! @brief Increments the current frequency of the TEA5757 radio by a given value.
* This function increases the current frequency of the TEA5757 radio by a given value.
* If the given value is negative, the search mode is set to down, otherwise the search mode is set to up.
* The frequency is then incremented by the given value, but it is first checked to ensure that it
* is within the frequency limits specified by the radio's band mode. The resulting frequency is then
* written to the radio using the _tea5767_write_registers function.
* @param radio A pointer to the TEA5757 radio structure.
* @param freq The frequency increment value.
* @note This function should be called only after the radio has been properly initialized and tuned to a station.
*/
void tea5767_setStationInc(float freq);

/*! @brief Sets the mute mode of the TEA5757 tuner.
* This function sets the mute mode of the TEA5757 tuner to the specified value. When mute mode is enabled,
* the audio output is muted.
* @param radio A pointer to a TEA5757_t structure representing the tuner.
* @param mute The desired mute mode value. true to enable mute mode, false to disable it.
* @note The function _tea5767_write_registers() is called to write the new mute mode value to the tuner.
*/
void tea5767_setMute(bool mute);

/*! @brief Sets the soft mute mode of the TEA5767 radio.
* This function sets the soft mute mode of the TEA5767 radio to either on or off.
* @param radio A pointer to a TEA5757_t struct representing the TEA5767 radio.
* @param mute A boolean indicating whether the soft mute mode should be on (true) or off (false).
* @return void
* @note The soft mute mode reduces the hissing noise when tuning the radio but can also cause distortion in weak signals.
*/
void tea5767_setSoftMute(bool mute);

/*! @brief Sets the left channel mute mode for the TEA5767 radio.
* @param radio Pointer to the TEA5757_t struct representing the radio.
* @param mute Boolean value indicating whether the left channel should be muted.
*/
void tea5767_setMuteLeft(bool mute);

/*! @brief Sets the right channel mute mode for the TEA5767 radio.
* @param radio Pointer to the TEA5757_t struct representing the radio.
* @param mute Boolean value indicating whether the left channel should be muted.
*/
void tea5767_setMuteRight(bool mute);

/*! @brief Sets the standby mode of the TEA5757 radio.
* This function sets the standby mode of the TEA5757 radio. When the radio is in standby mode, it consumes less power but cannot receive or transmit signals.
* @param radio Pointer to the TEA5757_t structure.
* @param standby Set to true to activate standby mode, false to deactivate it.
*/
void tea5767_setStandby(bool standby);

/*! @brief Sets the stereo mode of the TEA5757 radio.
* This function sets the stereo mode of the TEA5757 radio. When the radio is in stereo mode, it receives stereo signals if available. When in mono mode, it receives only mono signals.
* @param radio Pointer to the TEA5757_t structure.
* @param stereo Set to true to activate stereo mode, false to activate mono mode.
*/
void tea5767_setStereo(bool stereo);



#endif