#include <tea5767_i2c.h>
#include <unity.h>
#include <stdio.h>

void test_TEA5767_Init(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);
    
    printf("mute_mode %d \n",radio.mute_mode);
    TEST_ASSERT(radio.mute_mode == default_cfg.mute_mode);
    printf("searchMode %d \n",radio.searchMode);
    TEST_ASSERT(radio.searchMode == default_cfg.searchMode);
    printf("frequency %f \n",radio.frequency);
    // TEST_ASSERT(radio.frequency == default_cfg.frequency);
    printf("searchUpDown %d \n",radio.searchUpDown);
    TEST_ASSERT(radio.searchUpDown == default_cfg.searchUpDown);
    printf("searchLevel %d \n",radio.searchLevel);
    TEST_ASSERT(radio.searchLevel == default_cfg.searchLevel);
    printf("stereoMode %d \n",radio.stereoMode);
    TEST_ASSERT(radio.stereoMode == default_cfg.stereoMode);
    printf("muteLmode %d \n",radio.muteLmode);
    TEST_ASSERT(radio.muteLmode == default_cfg.muteLmode);
    printf("muteRmode %d \n",radio.muteRmode);
    TEST_ASSERT(radio.muteRmode == default_cfg.muteRmode);
    printf("standby %d \n",radio.standby);
    TEST_ASSERT(radio.standby == default_cfg.standby);
    printf("band_mode %d \n",radio.band_mode);
    TEST_ASSERT(radio.band_mode == default_cfg.band_mode);
    printf("softMuteMode %d \n",radio.softMuteMode);
    TEST_ASSERT(radio.softMuteMode == default_cfg.softMuteMode);
    printf("hpfMode %d \n",radio.hpfMode);
    TEST_ASSERT(radio.hpfMode == default_cfg.hpfMode);
    printf("stereoNoiseCancelling %d \n",radio.stereoNoiseCancelling);
    TEST_ASSERT(radio.stereoNoiseCancelling == default_cfg.stereoNoiseCancelling);
}

void test_TEA5767_getStation(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);
    for (float freq = MIN_FREQ_EU; freq < MAX_FREQ_EU; freq++)
    {
        tea5767_setStation(freq);
        printf("Current frequency %.3f \n",freq);
        printf("Current frequency %.3f \n",tea5767_getStation());
        // Compare as unsigned int due to float math
        TEST_ASSERT((uint32_t)tea5767_getStation() == (uint32_t)freq);
    }
}

void test_TEA5767_getReady(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);

}

void test_TEA5767_setSearch(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);
    
}

void test_TEA5767_setStation(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);
    
}

void test_TEA5767_setMute(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);
    
}

void test_TEA5767_setStadby(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);
    
}

void test_TEA5767_setStereo(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);
    
}

void setUp(void)
{
}

void tearDown(void)
{
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_TEA5767_Init);
    RUN_TEST(test_TEA5767_getStation);

    return UNITY_END();
}
