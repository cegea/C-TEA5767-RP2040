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
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

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
void _tea5767_read_registers(void);

/*! \brief   Writes all five bytes from TEA5767 with format.
 *  \ingroup tea5767_i2c
 *
 * Writtes five bytes to the TEA5767_t variable and fills the buffer variable.
 * The data from the radio variable is modified to fit the memory map of
 * TEA5767.
 *
 * \param radio Variable of type TEA5767_t.
 */
void _tea5767_write_registers(void);

/*! @brief Checks if the given frequency is within the limits of the current
 * band mode of the TEA5757 radio. This function checks if the given frequency
 * is within the limits of the current band mode of the TEA5757 radio, and
 * adjusts it if necessary.
 * @param radio A pointer to a TEA5757_t structure that holds information about
 * the radio, including its current band mode.
 * @param freq The frequency to check.
 * @return float The frequency adjusted if it is out of bounds for the current
 * band mode, or the original frequency if it is within bounds.
 * @note The function assumes that the TEA5757_t structure pointed to by radio
 * has its band_mode field set correctly.
 */
float _tea5767_checkFreqLimits(float freq);

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void _tea5767_read_registers() {
  uint8_t registers[TEA5767_REGISTERS] = {0};
#ifdef RASPBERRYPI_PICO
  i2c_read_blocking(i2c_default, _radio->address, buffer, TEA5767_REGISTERS,
                    false);
#else
  i2c_read_mockup(_radio->address, registers, TEA5767_REGISTERS);
#endif
  _radio->read.ready = registers[0] >> 7;
  _radio->read.bandLimitsFlag = registers[0] >> 6;
  _radio->read.pll =((uint16_t)(registers[0] & 0x3f) << 8);
  _radio->read.pll += (uint16_t)registers[1];
  _radio->read.stereo = registers[2] >> 7;
  _radio->read.ifCounter = registers[2] & 0x7f;
  _radio->read.adcLevelOutput = registers[3] >> 4;
  _radio->read.tea5737ID = (registers[3] & 0x0e) >> 1;

  _radio->frequency =
      ((float)_radio->read.pll * 32768.0 / 4.0 - 225000.0) / 1000000.0;
}

void _tea5767_write_registers() {
  uint8_t registers[TEA5767_REGISTERS] = {0};
  // Calculate the frequency value to be written to the TEA5767 register based
  // on the current radio frequency in MHz. The calculation takes into account
  // the fixed offset of 225kHz and the 4:1 prescaler used by the TEA5767
  // module.
  float freq = 4.0 * (_radio->frequency * 1000000.0 + 225000.0) / 32768.0;
  uint16_t integer_freq = (uint16_t)round(freq);
  registers[0] = (uint8_t)(integer_freq >> 8) | _radio->write.mute << 7 |
                 _radio->write.searchModeEnabled << 6;
  registers[1] = (uint8_t)(integer_freq & 0xff);
  registers[2] = _radio->write.searchDownUp << 7;
  registers[2] |= _radio->write.searchStopLevel << 5 | _radio->write.hlsi << 4;
  registers[2] |= _radio->write.monoToStereo << 3;
  registers[2] |=
      _radio->write.muteR << 2 | _radio->write.muteL << 1 | _radio->write.swp1;
  registers[3] = _radio->write.swp2 << 7;
  registers[3] |= _radio->write.standy << 6 | _radio->write.bandLimits << 5;
  registers[3] |= _radio->write.xtal << 4;
  registers[3] |= _radio->write.softMute << 3 | _radio->write.hcc << 2;
  registers[3] |= _radio->write.stereoNoiseCancelling << 1;
  registers[3] |= _radio->write.searchIndicator;
  registers[4] = _radio->write.pllref << 7 | _radio->write.dtc << 6;
#ifdef RASPBERRYPI_PICO
  i2c_write_blocking(i2c_default, _radio->address, registers, TEA5767_REGISTERS,
                     false);
#else
  i2c_write_mockup(_radio->address, registers, TEA5767_REGISTERS);
#endif
}

void tea5767_init(TEA5757_t *radio) {
  _radio = radio;
  *_radio = default_cfg;
  _radio->write = default_write_cfg;

  // Start the radio
  _tea5767_write_registers();
  _tea5767_read_registers();
}

float tea5767_getStation() {

  // Read current settings from the TEA5767 module
  _tea5767_read_registers();
  return _radio->frequency;
}

void tea5767_setStation(float freq) {
  _radio->frequency = _tea5767_checkFreqLimits(freq);
  _tea5767_write_registers();
}

float _tea5767_checkFreqLimits(float freq) {
  float min_freq = MIN_FREQ_EU;
  float max_freq = MAX_FREQ_EU;

  if (_radio->write.bandLimits == JP_BAND) {
    min_freq = MIN_FREQ_JP;
    max_freq = MAX_FREQ_JP;
  } else if (_radio->write.bandLimits == EU_BAND) {
    min_freq = MIN_FREQ_EU;
    max_freq = MAX_FREQ_EU;
  } else {
    return freq;
  }

  if (freq > max_freq) {
    freq = max_freq;
  } else if (freq < min_freq) {
    freq = min_freq;
  }

  return freq;
}

bool tea5767_getReady() {
  _tea5767_read_registers();
  return _radio->read.ready;
}

void tea5767_setSearch(bool searchModeEnabled, bool searchUpDown) {
  _radio->write.searchDownUp = searchUpDown;
  _radio->write.searchModeEnabled = searchModeEnabled;
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

void tea5767_mute() {
  _radio->write.mute = true;
  _tea5767_write_registers();
}

void tea5767_unMute() {
  _radio->write.mute = false;
  _tea5767_write_registers();
}

void tea5767_softMute() {
  _radio->write.softMute = true;
  _tea5767_write_registers();
}

void tea5767_unSoftMute() {
  _radio->write.softMute = false;
  _tea5767_write_registers();
}

void tea5767_muteLeft() {
  _radio->write.muteL = true;
  _tea5767_write_registers();
}

void tea5767_unMuteLeft() {
  _radio->write.muteL = false;
  _tea5767_write_registers();
}

void tea5767_muteRight() {
  _radio->write.muteR = true;
  _tea5767_write_registers();
}

void tea5767_unMuteRight() {
  _radio->write.muteR = false;
  _tea5767_write_registers();
}

void tea5767_sleep() {
  _radio->write.standy = true;
  _tea5767_write_registers();
}

void tea5767_wakeUp() {
  _radio->write.standy = false;
  _tea5767_write_registers();
}

void tea5767_enableStereo() {
  _radio->write.monoToStereo = false;
  _tea5767_write_registers();
}

void tea5767_disableStereo() {
  _radio->write.monoToStereo = true;
  _tea5767_write_registers();
}

void tea5767_setSWPin1(bool pin1_state) {
  _radio->write.swp1 = pin1_state;
  _tea5767_write_registers();
}

void tea5767_setSWPin2(bool pin2_state) {
  _radio->write.swp2 = pin2_state;
  _tea5767_write_registers();
}

void tea5767_enableSearchIndicatorOnPin1() {
    _radio->write.searchIndicator = true;
    _tea5767_write_registers();
}

void tea5767_disableSearchIndicatorOnPin1() {
    _radio->write.searchIndicator = false;
    _tea5767_write_registers();
}
