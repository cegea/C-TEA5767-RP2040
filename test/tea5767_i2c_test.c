#include <stdbool.h>
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
    
    TEST_ASSERT(radio.write.searchDownUp == true);
    TEST_ASSERT(radio.write.searchModeEnabled == true);
}

void test_TEA5767_mute(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);
    
    TEST_ASSERT(radio.write.mute == true);
    TEST_ASSERT(radio.write.softMute == true);
    TEST_ASSERT(radio.write.muteL == true);
    TEST_ASSERT(radio.write.muteR == true);
}

void test_TEA5767_setStadby(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);

    TEST_ASSERT(radio.write.standy == true);    
}

void test_TEA5767_setStereo(void)
{
    TEA5757_t radio = {0};
    tea5767_init(&radio);
    
    TEST_ASSERT(radio.read.stereo == true);    
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

    return UNITY_END();
}
