#include <stdio.h>
#include <stdint.h>

uint8_t OLED_DisplayBuf[2][4] = {
    0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000};

int32_t pow32i(int32_t base, int32_t exp)
{
    int32_t result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

int main()
{
    printf("pow32i(3,5)=%d,%d",pow32i(3,1),3*3*3*3*3);

    //// 先遍历行
    //for (int32_t i = 0; i < 2; i++) {
    //    // 遍历位压缩下的每一位
    //    for (int32_t j = 0; j < 8; j++) {
    //        // 遍历列
    //        for (int32_t k = 0; k < 4; k++) {
    //            uint8_t* p   = (uint8_t*)OLED_DisplayBuf;
    //            int val = *(p + i * 4 + k) & (1 << j);
    //            printf("[%d][%d]=%d\n",i*8+j , k,val);
    //        }
    //    }
    //}

    return 0;
}
