#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * This struct represents the circular buffer properties
 */
typedef struct CircularBuffer CircularBuffer_t;

/**
 * Initialize the circular_buffer struct members
 * @param  size Size of the circular_buffer to set
 *
 * @return A pointer to the created circular_buffer, ownership is given to the caller
 */
CircularBuffer_t *circular_buffer_create(size_t size);

/**
 * Push data into the circular_buffer
 * If full, will overwrite the oldest value
 * @param  cbuffer The circular_buffer
 * @param  data  Value to be inserted into the circular_buffer
 */
void circular_buffer_push(CircularBuffer_t *cbuffer, float data);

/**
 * Retrieve the most recent value in the circular_buffer
 * The index is relative to the most recent value in the buffer.
 * Eg. 1 is the most recent value, 2 is the second most recent, etc
 *
 * NOTE: `index` must be less than the max_size of the circular_buffer
 *
 * @param  cbuffer The circular_buffer
 * @param  index Index of most recent value
 *
 * @return Data at the index of the most recent value
 */
float circular_buffer_getAtIndex(CircularBuffer_t *cbuffer, uint8_t index);

/**
 * Return if the circular_buffer is full
 * @param  cbuffer The circular_buffer
 *
 * @return A boolean indicating if the circular_buffer is full
 */
bool circular_buffer_isFull(CircularBuffer_t *cbuffer);