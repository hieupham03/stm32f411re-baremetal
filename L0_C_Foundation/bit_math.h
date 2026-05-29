#ifndef BIT_MATH_H
#define BIT_MATH_H

/*
 * Các macro thao tác bit cơ bản
 * Dùng toán tử bitwise để tác động vào thanh ghi (hoặc biến)
 */

// Set bit lên 1 tại vị trí BIT
#define SET_BIT(REG, BIT)    ((REG) |= (1U << (BIT)))

// Clear bit về 0 tại vị trí BIT
#define CLEAR_BIT(REG, BIT)  ((REG) &= ~(1U << (BIT)))

// Lật trạng thái bit (1->0, 0->1) tại vị trí BIT
#define TOGGLE_BIT(REG, BIT) ((REG) ^= (1U << (BIT)))

// Đọc bit tại vị trí BIT (kết quả trả về là khác 0 nếu bit = 1, bằng 0 nếu bit = 0)
#define READ_BIT(REG, BIT)   (((REG) >> (BIT)) & 1U)

#endif // BIT_MATH_H
