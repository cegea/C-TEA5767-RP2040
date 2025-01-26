#include <tea5767_i2c_mockup.h>
#include <unity.h>
#include <string.h>


void swap_array(uint8_t *array, size_t size)
{
    uint8_t tmp_array[size];
    memcpy(tmp_array, array, size);
    for (size_t i = 0; i < size; i++)
    {
        array[i] = tmp_array[size-i-1];
    }
}

void test_mockup(void)
{
    uint8_t buffer[6] = {0,1,2,3,4,5};
    i2c_write_mockup(0x00, buffer, sizeof(buffer));

    uint8_t* write_buffer = get_i2c_buffer();

 
    for (int i = 0; i < sizeof(buffer); i++) { 
        TEST_ASSERT(buffer[i] == *(write_buffer + i));
    } 

    swap_array(buffer, sizeof(buffer));
    i2c_write_mockup(0x00, buffer, sizeof(buffer));
    write_buffer = get_i2c_buffer();
    for (int i = 0; i < sizeof(buffer); i++) { 
        TEST_ASSERT(buffer[i] == *(write_buffer + i));
    } 
    
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
    RUN_TEST(test_mockup);

    return UNITY_END();
}
