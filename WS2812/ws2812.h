/**
 * @file ws2812.h
 * @author KkrainL15
 * @brief 
 * @version 0.1
 * @date 2026-09-30
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef __WS2812_H__
#define __WS2812_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <cubemx.h>

#define WS2812_LED_NUM          1U
#define WS2812_PERIOD           300U
#define WS2812_0_CODE           72U
#define WS2812_1_CODE           156U
#define WS2812_RESET_SLOTS      240U
#define WS2812_DATA_BITS        (WS2812_LED_NUM * 24U)
#define WS2812_BUFFER_SIZE      \
        (WS2812_DATA_BITS + WS2812_RESET_SLOTS)

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} WS2812_Color_t;

void WS2812_Init(void);
void WS2812_SetPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
void WS2812_SetAll(uint8_t r, uint8_t g, uint8_t b);
void WS2812_Show(void);
void WS2812_Clear(void);
void WS2812_DMA_Callback(void);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __WS2812_H__ */