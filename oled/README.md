# کتابخانه OLED SSD1306 برای STM32

این کتابخانه برای راه‌اندازی و کنترل نمایشگر OLED با درایور SSD1306 طراحی شده است و از پروتکل I2C یا SPI پشتیبانی می‌کند. در این بخش، توابع موجود در کتابخانه و نحوه استفاده از آن‌ها توضیح داده شده است.

---

## ⚙️ توابع اصلی

### `void ssd1306_Init(void);`
راه‌اندازی اولیه نمایشگر. باید قبل از استفاده از سایر توابع فراخوانی شود.

### `void ssd1306_UpdateScreen(void);`
به‌روزرسانی نمایشگر با داده‌های موجود در بافر. بعد از هر تغییر گرافیکی باید این تابع را فراخوانی کنید.

### `void ssd1306_Fill(SSD1306_COLOR color);`
صفحه را به‌صورت کامل با رنگ مشخص (سیاه یا سفید) پر می‌کند.
مثال:
```c
ssd1306_Fill(Black); // خاموش کردن کل صفحه
```

---

## 📍 ترسیم پیکسل و متن

### `void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);`
ترسیم یک پیکسل در مختصات مشخص.
مثال:
```c
ssd1306_DrawPixel(5, 5, White);
```

### `void ssd1306_SetCursor(uint8_t x, uint8_t y);`
تنظیم مکان‌نما برای نوشتن متن.

### `char ssd1306_WriteChar(char ch, SSD1306_Font_t Font, SSD1306_COLOR color);`
نوشتن یک کاراکتر در مکان فعلی مکان‌نما.

### `char ssd1306_WriteString(char* str, SSD1306_Font_t Font, SSD1306_COLOR color);`
نوشتن یک رشته کامل در مکان فعلی مکان‌نما.
مثال:
```c
ssd1306_WriteString("Hello", Font_7x10, White);
```

---

## 📏 اشکال هندسی پایه

### `void ssd1306_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color);`
رسم یک خط بین دو نقطه.
مثال:
```c
ssd1306_Line(0, 0, 127, 63, White);
```


### `void ssd1306_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color);`
رسم یک مستطیل خالی.
مثال:
```c
ssd1306_DrawRectangle(10, 10, 50, 30, White);
```

### `void ssd1306_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_COLOR color);`
رسم یک مستطیل پرشده.
مثال:
```c
ssd1306_FillRectangle(20, 20, 40, 40, White);

```

### `void ssd1306_DrawCircle(uint8_t x, uint8_t y, uint8_t r, SSD1306_COLOR color);`
رسم دایره توخالی با شعاع و مرکز مشخص.
مثال:
```c
ssd1306_DrawCircle(64, 32, 20, White);
```

### `void ssd1306_FillCircle(uint8_t x, uint8_t y, uint8_t r, SSD1306_COLOR color);`
رسم دایره پرشده با شعاع و مرکز مشخص.
مثال:
```c
ssd1306_FillCircle(64, 32, 10, White);
```

---

## 🔄 رسم کمان و چندضلعی

### `void ssd1306_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_COLOR color);`
رسم کمان (Arc) با زاویه شروع، زاویه چرخش و شعاع مشخص.
مثال:
```c
ssd1306_DrawArc(64, 32, 20, 0, 180, White);
```

### `void ssd1306_DrawArcWithRadiusLine(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_COLOR color);`
رسم کمان به‌همراه خطوط شعاعی از مرکز به ابتدا و انتهای کمان.
مثال:
```c
ssd1306_DrawArcWithRadiusLine(64, 32, 20, 0, 90, White);
```

### `void ssd1306_Polyline(const SSD1306_VERTEX *points, uint16_t count, SSD1306_COLOR color);`
رسم چندضلعی با استفاده از آرایه‌ای از نقاط.
مثال:
```c
SSD1306_VERTEX v[] = {
  {10, 10},
  {30, 20},
  {20, 40}
};
ssd1306_Polyline(v, 3, White);

```

---

## 🎨 تعریف رنگ

نوع رنگ در این کتابخانه به‌صورت زیر تعریف شده است:

```c
typedef enum {
    Black = 0,
    White = 1
} SSD1306_COLOR;
```
📝 نکته
برای مشاهده تغییرات در نمایشگر پس از فراخوانی هر تابع گرافیکی، حتماً باید ssd1306_UpdateScreen() را در پایان صدا بزنید.

🧪 مثال ساده
```c
ssd1306_Init();
ssd1306_Fill(Black);
ssd1306_SetCursor(0, 0);
ssd1306_WriteString("Hello OLED", Font_7x10, White);
ssd1306_UpdateScreen();
```
---
🖋️ مثال کامل
```c
ssd1306_Init();
ssd1306_Fill(Black);

ssd1306_SetCursor(0, 0);
ssd1306_WriteString("Hello OLED", Font_7x10, White);
ssd1306_UpdateScreen();
HAL_Delay(1000);

ssd1306_DrawRectangle(10, 10, 60, 30, White);
ssd1306_UpdateScreen();
HAL_Delay(1000);

ssd1306_FillCircle(64, 32, 15, White);
ssd1306_UpdateScreen();
HAL_Delay(1000);

```
