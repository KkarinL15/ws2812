/**
 * @file ws2812.c
 * @author KkarinL15
 * @brief 
 * @version 0.1
 * @date 2026-09-30
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "ws2812.h"
#include "tim.h"

static WS2812_Color_t ws2812_led[WS2812_LED_NUM];
__attribute__((aligned(32)))
static uint16_t ws2812_pwm_buffer[WS2812_BUFFER_SIZE];
static volatile uint8_t ws2812_busy = 0;

void WS2812_Init(void)
{
    for(uint16_t i = 0; i < WS2812_LED_NUM; i++)
    {
        ws2812_led[i].r = 0;
        ws2812_led[i].g = 0;
        ws2812_led[i].b = 0;
    }
    ws2812_busy = 0;
}

void WS2812_SetPixel(uint16_t index, uint8_t r, 
                        uint8_t g, uint8_t b){
    if(index >= WS2812_LED_NUM)
    {
        return;
    }

    ws2812_led[index].r = r;
    ws2812_led[index].g = g;
    ws2812_led[index].b = b;
}

void WS2812_SetAll(uint8_t r, uint8_t g, uint8_t b)
{
    for(uint16_t i = 0; i < WS2812_LED_NUM; i++)
    {
        WS2812_SetPixel(i, r, g, b);
    }
}

void WS2812_Show(void){
    uint32_t index = 0;
    while(ws2812_busy)
    {
    }

    for(uint16_t led = 0;
        led < WS2812_LED_NUM;
        led++)
    {
        uint8_t data[3];
        data[0] = ws2812_led[led].g;
        data[1] = ws2812_led[led].r;
        data[2] = ws2812_led[led].b;

        for(uint8_t color = 0; color < 3; color++)
        {
            for(int8_t bit = 7;
                bit >= 0;
                bit--)
            {
                if(data[color] & (1U << bit))
                {
                    ws2812_pwm_buffer[index++] =
                        WS2812_1_CODE;
                }
                else
                {
                    ws2812_pwm_buffer[index++] =
                        WS2812_0_CODE;
                }
            }
        }
    }

    for(uint32_t i = 0;
        i < WS2812_RESET_SLOTS;
        i++)
    {
        ws2812_pwm_buffer[index++] = 0;
    }

    if(HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_2, ws2812_pwm_buffer, index) == HAL_OK)
    {
        ws2812_busy = 1;
    }
    else
    {
        ws2812_busy = 0;
    }
}

void WS2812_Clear(void){
    WS2812_SetAll(0,0,0);
    WS2812_Show();
}

void WS2812_DMA_Callback(void){

    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_2);
    __HAL_TIM_SET_COMPARE( &htim1, TIM_CHANNEL_2, 0 );

    ws2812_busy = 0;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM1)
    {
        WS2812_DMA_Callback();
    }
}