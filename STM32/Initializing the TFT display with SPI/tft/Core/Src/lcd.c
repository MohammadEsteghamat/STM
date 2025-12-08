#include "lcd.h"
#include "font5x7.h"

#define ABS(x) ((x) > 0 ? (x) : -(x))

ILI9341_Colors COLOR = {
    .BLACK   = 0x0000,
    .WHITE   = 0xFFFF,
    .BLUE    = 0xF800,
    .GREEN   = 0x07E0,
    .RED     = 0x001F,
    .YELLOW  = 0x07FF,
    .MAGENTA = 0xF81F,
    .CYAN    = 0xFFE0,
    .ORANGE  = 0x031F,
    .GRAY    = 0x8410
};
uint16_t  POINT_COLOR=0x0000;
uint16_t  BACK_COLOR=0xFFFF; 

// Internal SPI write
static void ILI9341_SPI_Write(uint8_t data) {
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
}

// Internal command and data writing
static void ILI9341_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET);
    ILI9341_SPI_Write(cmd);
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_SET);
}

static void ILI9341_WriteData(uint8_t data) {
    HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET);
    ILI9341_SPI_Write(data);
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_SET);
}

// Set drawing address
static void ILI9341_SetAddress(uint16_t x, uint16_t y) {
    ILI9341_WriteCommand(0x2A);
    ILI9341_WriteData(x >> 8);
    ILI9341_WriteData(x & 0xFF);
    ILI9341_WriteData(x >> 8);
    ILI9341_WriteData(x & 0xFF);

    ILI9341_WriteCommand(0x2B);
    ILI9341_WriteData(y >> 8);
    ILI9341_WriteData(y & 0xFF);
    ILI9341_WriteData(y >> 8);
    ILI9341_WriteData(y & 0xFF);

    ILI9341_WriteCommand(0x2C);
}

// Initialization
void ILI9341_Init(void) {
    HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(10);

    ILI9341_WriteCommand(0x01); // Software reset
    HAL_Delay(100);
    ILI9341_WriteCommand(0x28); // Display OFF
    ILI9341_WriteCommand(0x29); // Display ON
}

void LCD_PutPixel(uint16_t x, uint16_t y, uint16_t color) {
    ILI9341_SetAddress(y, x);
    ILI9341_WriteData(color >> 8);
    ILI9341_WriteData(color & 0xFF);
}

uint16_t change_color(unsigned short color,float r_factor,float g_factor,float b_factor){
		uint8_t r = (color >> 11) & 0x1F; 
		uint8_t g = (color >> 5) & 0x3F;   
		uint8_t b = color & 0x1F;         

	
		r = (uint8_t)(r * r_factor);
		if (r > 31) r = 31;

		g = (uint8_t)(g * g_factor);
		if (g > 63) g = 63;

		b = (uint8_t)(b * b_factor);
		if (b > 31) b = 31;

		
		uint16_t newColor = (r << 11) | (g << 5) | b;
	return newColor;
}


void LCD_Clear(uint16_t color) {
    ILI9341_WriteCommand(0x2A);
    ILI9341_WriteData(0 >> 8);
    ILI9341_WriteData(0 & 0xFF);
    ILI9341_WriteData((ILI9341_WIDTH - 1) >> 8);
    ILI9341_WriteData((ILI9341_WIDTH - 1) & 0xFF);

    ILI9341_WriteCommand(0x2B);
    ILI9341_WriteData(0 >> 8);
    ILI9341_WriteData(0 & 0xFF);
    ILI9341_WriteData((ILI9341_HEIGHT - 1) >> 8);
    ILI9341_WriteData((ILI9341_HEIGHT - 1) & 0xFF);

    ILI9341_WriteCommand(0x2C);

    HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET);

    uint8_t data[2] = {color >> 8, color & 0xFF};
    for (uint32_t i = 0; i < ILI9341_WIDTH * ILI9341_HEIGHT; i++) {
        HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
    }

    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_SET);
}

void LCD_SetBackColor (unsigned short color) {
  BACK_COLOR = color;
}

void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t* imageData,float r_factor, float g_factor, float b_factor) {
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            uint16_t color = imageData[j * width + i];

          
            uint8_t r = (color >> 11) & 0x1F;  
            uint8_t g = (color >> 5) & 0x3F;   
            uint8_t b = color & 0x1F;          

           
            r = (uint8_t)(r * r_factor);
            if (r > 31) r = 31;

            g = (uint8_t)(g * g_factor);
            if (g > 63) g = 63;

            b = (uint8_t)(b * b_factor);
            if (b > 31) b = 31;

         
            uint16_t newColor = (r << 11) | (g << 5) | b;

            LCD_PutPixel(x + i, y + j, newColor);
        }
    }
}

void LCD_DrawLine(int x0, int y0, int x1, int y1, uint16_t color) {
    int dx = ABS(x1 - x0);
    int dy = -ABS(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;

    while (1) {
        LCD_PutPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void LCD_ShowChar(uint16_t x, uint16_t y, char ch) {
    if (ch < FONT5X7_OFFSET || ch >= (FONT5X7_OFFSET + FONT5X7_LENGTH)) return;
    const uint8_t* bitmap = font5x7[ch - FONT5X7_OFFSET];
    for (int i = 0; i < 5; i++) {
        uint8_t line = bitmap[i];
        for (int j = 0; j < 7; j++) {
            uint16_t color = (line & (1 << j)) ?  POINT_COLOR : BACK_COLOR;
            LCD_PutPixel(x + i, y + j, color);
        }
    }
}

void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p){
    uint16_t startX = x;
    while (*p)
    {
        if (x + 6 > x + width) { 
            x = startX;
            y += 8;
        }

        if (y + 7 > y + height) {
            break;
        }

        LCD_ShowChar(x, y, *p);
        x += 6; 
        p++;
    }
}

void LCD_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    if (x2 < x1) { uint16_t t = x1; x1 = x2; x2 = t; }
    if (y2 < y1) { uint16_t t = y1; y1 = y2; y2 = t; }

    for (uint16_t y = y1; y <= y2; y++) {
        for (uint16_t x = x1; x <= x2; x++) {
            LCD_PutPixel(x, y, color);
        }
    }
}


void Draw_Circle(uint16_t xCenter, uint16_t yCenter, uint16_t radius, uint16_t color) {
    int16_t x = radius, y = 0, err = 1 - radius;
    while (x >= y) {
        LCD_PutPixel(xCenter + x, yCenter + y, color);
        LCD_PutPixel(xCenter + y, yCenter + x, color);
        LCD_PutPixel(xCenter - y, yCenter + x, color);
        LCD_PutPixel(xCenter - x, yCenter + y, color);
        LCD_PutPixel(xCenter - x, yCenter - y, color);
        LCD_PutPixel(xCenter - y, yCenter - x, color);
        LCD_PutPixel(xCenter + y, yCenter - x, color);
        LCD_PutPixel(xCenter + x, yCenter - y, color);
        y++;
        if (err <= 0) {
            err += 2 * y + 1;
        } else {
            x--;
            err += 2 * (y - x + 1);
        }
    }
}

