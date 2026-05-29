#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 256

typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    uint8_t write_index;
    uint8_t read_index;
    uint16_t count;
} RingBuffer;

// Khởi tạo trạng thái ban đầu
void RingBuffer_Init(RingBuffer *rb) {
    rb->write_index = 0;
    rb->read_index = 0;
    rb->count = 0;
}

// Hàm ghi dữ liệu
int RingBuffer_Write(RingBuffer *rb, uint8_t data_in) {
    if (rb->count == BUFFER_SIZE) {
        return -1; // Buffer đầy
    }
    
    rb->buffer[rb->write_index] = data_in;
    rb->write_index++; // Tự động quay vòng nhờ uint8_t
    rb->count++;
    
    return 0; // Thành công
}

// Hàm đọc dữ liệu
int RingBuffer_Read(RingBuffer *rb, uint8_t *data_out) {
    if (rb->count == 0) {
        return -1; // Buffer trống
    }
    
    *data_out = rb->buffer[rb->read_index];
    rb->read_index++; // Tự động quay vòng nhờ uint8_t
    rb->count--;
    
    return 0; // Thành công
}

// Hàm kiểm tra trạng thái trống
bool RingBuffer_IsEmpty(RingBuffer *rb) {
    return (rb->count == 0);
}