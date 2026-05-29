
#define BIT_MATH_H
#define SET_BIT(REG, BIT) ((REG) |= (1U << BIT)) // set bit lên 1
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(1U << BIT)) // clear bit về 0
#define TOGGLE_BIT(REG, BIT) ((REG) ^= (1U << BIT)) //lật trạng thái 1->0, 0->1
#define READ_BIT(REG, BIT) ((REG) & (1U << BIT)) // đọc bit tại vị trí
