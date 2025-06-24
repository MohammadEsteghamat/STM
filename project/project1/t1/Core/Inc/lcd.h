#ifndef __ILI9341_H
#define __ILI9341_H

#include "stm32f1xx_hal.h"  
#include "spi.h"
#include "gpio.h"

// Screen dimensions
#define ILI9341_WIDTH   240
#define ILI9341_HEIGHT  320

// Pin configuration 
#define ILI9341_CS_PORT     GPIOB
#define ILI9341_CS_PIN      GPIO_PIN_0
#define ILI9341_RST_PORT    GPIOB
#define ILI9341_RST_PIN     GPIO_PIN_1
#define ILI9341_DC_PORT     GPIOB
#define ILI9341_DC_PIN      GPIO_PIN_2


typedef struct {
    uint16_t BLACK;
    uint16_t WHITE;
    uint16_t RED;
    uint16_t GREEN;
    uint16_t BLUE;
    uint16_t CYAN;
    uint16_t MAGENTA;
    uint16_t YELLOW;
    uint16_t ORANGE;
    uint16_t GRAY;
} ILI9341_Colors;

extern ILI9341_Colors COLOR;
extern uint16_t  POINT_COLOR;
extern uint16_t  BACK_COLOR;
// API functions
void ILI9341_Init(void);
void LCD_Clear(uint16_t color);
void LCD_SetBackColor (unsigned short color);
void LCD_PutPixel(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawLine(int x0, int y0, int x1, int y1, uint16_t color);
void LCD_ShowChar(uint16_t x, uint16_t y, char ch);
void Draw_Circle(uint16_t xCenter, uint16_t yCenter, uint16_t radius, uint16_t color);
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t* imageData,float r_factor, float g_factor, float b_factor);
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);
void LCD_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
#endif // __ILI9341_H
