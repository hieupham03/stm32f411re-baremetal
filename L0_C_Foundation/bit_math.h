#ifndef BIT_MATH_H
#define BIT_MATH_H

// Set bit lên 1 tại vị trí BIT
#define SET_BIT(REG, BIT)    ((REG) |= (1U << (BIT)))

// Clear bit về 0 tại vị trí BIT
#define CLEAR_BIT(REG, BIT)  ((REG) &= ~(1U << (BIT)))

// Lật trạng thái bit
#define TOGGLE_BIT(REG, BIT) ((REG) ^= (1U << (BIT)))

// Đọc bit tại vị trí BIT
#define READ_BIT(REG, BIT)   (((REG) >> (BIT)) & 1U)

#endif 
