extern "C"
{
#include "firmware/shared/circular_buffer.h"

#include "shared/constants.h"
}

#include <gtest/gtest.h>

class CircularBufferTest : public testing::Test
{
   protected:
    virtual void SetUp(void)
    {
        circular_buffer = circular_buffer_create(10);
    }

    CircularBuffer_t* circular_buffer;
};

TEST_F(CircularBufferTest, circular_buffer_exact_full)
{
    float mockData[]     = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    float expectedData[] = {10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
    int mockDataSize     = sizeof(mockData) / sizeof(mockData[0]);

    // Populate circular buffer with mock data
    for (int i = 0; i < mockDataSize; i++)
    {
        circular_buffer_push(circular_buffer, mockData[i]);
    }

    EXPECT_EQ(true, circular_buffer_isFull(circular_buffer));

    for (int i = 0; i < 10; i++)
    {
        EXPECT_EQ(expectedData[i], circular_buffer_getAtIndex(circular_buffer, i + 1));
    }
}

TEST_F(CircularBufferTest, circular_buffer_overfill)
{
    float mockData[]     = {1.0, 2.0,  3.0,  4.0,  5.0,  6.0,  7.0, 8.0,
                        9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0};
    float expectedData[] = {15.0, 14.0, 13.0, 12.0, 11.0, 10.0, 9.0, 8.0, 7.0, 6.0};
    int mockDataSize     = sizeof(mockData) / sizeof(mockData[0]);

    // Populate circular buffer with mock data
    for (int i = 0; i < mockDataSize; i++)
    {
        circular_buffer_push(circular_buffer, mockData[i]);
    }

    EXPECT_EQ(true, circular_buffer_isFull(circular_buffer));

    for (int i = 0; i < 10; i++)
    {
        EXPECT_EQ(expectedData[i], circular_buffer_getAtIndex(circular_buffer, i + 1));
    }
}

TEST_F(CircularBufferTest, circular_buffer_not_full)
{
    float mockData[]     = {1.0, 2.0, 3.0, 4.0, 5.0};
    float expectedData[] = {5.0, 4.0, 3.0, 2.0, 1.0};
    int mockDataSize     = sizeof(mockData) / sizeof(mockData[0]);

    // Populate circular buffer with mock data
    for (int i = 0; i < mockDataSize; i++)
    {
        circular_buffer_push(circular_buffer, mockData[i]);
    }

    EXPECT_EQ(false, circular_buffer_isFull(circular_buffer));

    for (int i = 0; i < mockDataSize; i++)
    {
        EXPECT_EQ(expectedData[i], circular_buffer_getAtIndex(circular_buffer, i + 1));
    }
}

TEST_F(CircularBufferTest, circular_buffer_get_at_0)
{
    float mockData[] = {1.0, 2.0, 3.0};
    int mockDataSize = sizeof(mockData) / sizeof(mockData[0]);

    // Populate circular buffer with mock data
    for (int i = 0; i < mockDataSize; i++)
    {
        circular_buffer_push(circular_buffer, mockData[i]);
    }

    EXPECT_EQ(false, circular_buffer_isFull(circular_buffer));

    ASSERT_DEATH(circular_buffer_getAtIndex(circular_buffer, 0);, "index > 0");
}

TEST_F(CircularBufferTest, circular_buffer_get_at_index_larger_buffer_size)
{
    float mockData[] = {1.0, 2.0, 3.0};
    int mockDataSize = sizeof(mockData) / sizeof(mockData[0]);

    // Populate circular buffer with mock data
    for (int i = 0; i < mockDataSize; i++)
    {
        circular_buffer_push(circular_buffer, mockData[i]);
    }

    EXPECT_EQ(false, circular_buffer_isFull(circular_buffer));

    ASSERT_DEATH(circular_buffer_getAtIndex(circular_buffer, 12);
                 , "index <= cbuffer->max_size");
}

TEST_F(CircularBufferTest, circular_buffer_get_at_index_unpopulated_not_full)
{
    float mockData[] = {1.0, 2.0, 3.0};
    int mockDataSize = sizeof(mockData) / sizeof(mockData[0]);

    // Populate circular buffer with mock data
    for (int i = 0; i < mockDataSize; i++)
    {
        circular_buffer_push(circular_buffer, mockData[i]);
    }

    EXPECT_EQ(false, circular_buffer_isFull(circular_buffer));

    ASSERT_DEATH(circular_buffer_getAtIndex(circular_buffer, 5);
                 , "circular_buffer_isFull(cbuffer) == true || index <= cbuffer->head");
}