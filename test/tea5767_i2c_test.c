#include <stdbool.h>
#include <stdint.h>
#include <tea5767_i2c.h>
#include <unity.h>
#include <stdio.h>

void test_TEA5767_Init(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);
}

void test_TEA5767_getSetStation(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);
    for (float freq = MIN_FREQ_EU; freq < MAX_FREQ_EU; freq++)
    {
        tea5767_setStation(freq);
        printf("Requested frequency %.3f \n",freq);
        printf("Current frequency %.3f \n",tea5767_getStation());
        // Compare as unsigned int due to float math
        TEST_ASSERT((uint32_t)tea5767_getStation() == (uint32_t)freq);
    }


}

void test_TEA5767_getReady(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);

    TEST_ASSERT(radio.read.ready == true);
}

void test_TEA5767_setSearch(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);

    tea5767_setSearch(false,false);
    
    TEST_ASSERT(radio.write.searchDownUp == false);
    TEST_ASSERT(radio.write.searchModeEnabled == false);
    
    tea5767_setSearch(true,false);
    
    TEST_ASSERT(radio.write.searchDownUp == false);
    TEST_ASSERT(radio.write.searchModeEnabled == true);
    
    tea5767_setSearch(true,true);
    
    TEST_ASSERT(radio.write.searchDownUp == true);
    TEST_ASSERT(radio.write.searchModeEnabled == true);
}

void test_TEA5767_mute(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);

    tea5767_mute();    
    TEST_ASSERT(radio.write.mute == true);
    
    tea5767_unMute();    
    TEST_ASSERT(radio.write.mute == false);

    tea5767_softMute();
    TEST_ASSERT(radio.write.softMute == true);
    
    tea5767_unSoftMute();
    TEST_ASSERT(radio.write.softMute == false);
    
    tea5767_muteLeft();
    TEST_ASSERT(radio.write.muteL == true);
    
    tea5767_unMuteLeft();
    TEST_ASSERT(radio.write.muteL == false);
    
    tea5767_muteRight();
    TEST_ASSERT(radio.write.muteR == true);
    
    tea5767_unMuteRight();
    TEST_ASSERT(radio.write.muteR == false);
}

void test_TEA5767_setStadby(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);

    tea5767_sleep();
    TEST_ASSERT(radio.write.standy == true);    

    tea5767_wakeUp();
    TEST_ASSERT(radio.write.standy == false);    
}

void test_TEA5767_setStereo(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);
    
    tea5767_disableStereo();
    TEST_ASSERT(radio.write.monoToStereo == true);    
    
    tea5767_enableStereo();
    TEST_ASSERT(radio.write.monoToStereo == false);    
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
    RUN_TEST(test_TEA5767_getSetStation);
    RUN_TEST(test_TEA5767_getReady);
    RUN_TEST(test_TEA5767_setSearch);
    RUN_TEST(test_TEA5767_mute);
    RUN_TEST(test_TEA5767_setStadby);
    RUN_TEST(test_TEA5767_setStereo);

    return UNITY_END();
}
