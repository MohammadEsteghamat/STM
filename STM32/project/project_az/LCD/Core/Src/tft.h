/* TFT*/
#include "stm32f1xx.h"
#include "font.h"

/* LCD RGB color definitions */
#define Black           0x0000		/*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255, 100   */
#define White           0xFFFF      /* 255, 255, 255 */
#define WHITE           0XFFFF      /* 255, 255, 255 */   
#define RED             0xF800      /* 255,   0,   0 */
#define BLUE            0x001F      /*   0,   0, 255 */
#define LightBlue       0xAEDC      /* 173, 216, 230 */
#define LightYellow     0xFFFB      /* 255, 255, 224 */
#define LightPeach      0xFC94      /* 255, 200, 160 */


//{Black,Navy,DarkGreen,DarkCyan,Maroon,Purple,Olive,LightGrey,DarkGrey,Blue,Green ,Cyan,Red,Magenta,Yellow,White}
typedef struct  
{										    
	uint16_t width;			
	uint16_t height;			
	uint16_t id;				
	uint8_t  dir;			
	uint8_t	wramcmd;		
	uint8_t  setxcmd;		
	uint8_t  setycmd;		
}_lcd_dev; 	  

 _lcd_dev lcddev;	

uint16_t  POINT_COLOR=0x0000;
uint16_t  BACK_COLOR=0xFFFF; 
 
#ifdef ORIENTATION_VERTICAL

	#define WIDTH       	240 
	#define HEIGHT      	400

#else

	#define WIDTH       	400
	#define HEIGHT      	240

#endif

typedef struct
{
	uint16_t LCD_REG;
	uint16_t LCD_RAM;
} LCD_TypeDef;
#define LCD_BASE        ((uint32_t)(0x6C000000 | 0x000007FE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)


void wait(void)
{
   __NOP();
	
}

void wait_delay(int count)
{
  while(count--);
}

void delayms(int dly)
{
	int i;
	while(dly--)
	{
		for(i=0;i<10000;i++)
		wait();
	}
}

void LCD_WR_REG(uint16_t  regval)
{ 
	LCD->LCD_REG=regval;
}

void LCD_WR_DATA(uint16_t data)
{										    	   
	LCD->LCD_RAM=data;		 
}

uint16_t LCD_RD_DATA(void)
{										    	   
	return LCD->LCD_RAM;		 
}					   

void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		
	LCD->LCD_RAM = LCD_RegValue;   		 
}	

void wr_reg (unsigned char reg, unsigned short val) 
{
	LCD->LCD_REG = reg;		
	LCD->LCD_RAM = val;  	
}

uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		
	delayms(1);	
	return LCD_RD_DATA();		
}   

void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	 

void LCD_WriteRAM(uint16_t RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;
}

void LCD_Init(void)
{ 

	HAL_Delay(50); // delay 50 ms 
	LCD_WriteReg(0x0000,0x0001);
	HAL_Delay(50); // delay 50 ms 
	
	
		HAL_Delay(50); // delay 50 ms 
	LCD_WriteReg(0x0000,0x0001);
	HAL_Delay(50); // delay 50 ms 
	
	lcddev.id = LCD_ReadReg(0x0000);   


	wr_reg(0x001A, 0x0004); // BT[3:0]=0100, VCL=-VCI; VGH=VCI+2DDVDH; VGL=-2DDVDH
	wr_reg(0x001B, 0x000C); // VRH[4:0]=0Ch, VREG1=(2.5v*1.9)=4.75V		VCIRE=1; 
	// VCOM offset
	wr_reg(0x0023, 0x0000); // SELVCM=0, R24h and R25h selects VCOM
	wr_reg(0x0024, 0x0040); // VCM[6:0]=1000000, VCOMH voltage=VREG1*0.748 (originally 5F)
	wr_reg(0x0025, 0x000F); // VDV[4:0]=01111, VCOMH amplitude=VREG*1.00
	wr_reg(0x002D, 0x0006); // NOW[2:0]=110, Gate output non-overlap period = 6 clocks
	delayms(20);
	// Power on Setting
	wr_reg(0x0018, 0x0008); // RADJ[3:0]=1000, Display frame rate 60Hz 100%
	wr_reg(0x0019, 0x0001); // OSC_EN=1, start OSC
	delayms(20);
	wr_reg(0x0001, 0x0000); // DSTB=0, out deep sleep
	wr_reg(0x001F, 0x0088); // STB=0
	wr_reg(0x001C, 0x0006); // AP[2:0]=110, High OPAMP current (default 011)
	delayms(10);
	wr_reg(0x001F, 0x0080); // DK=0
	delayms(10);
	wr_reg(0x001F, 0x0090); // PON=1
	delayms(5);
	wr_reg(0x001F, 0x00D0); // VCOMG=1
	delayms(10);
	wr_reg(0x0017, 0x0005); // IFPF[2:0]=101, 16-bit/pixel
	
	// Panel Configuration
	//wr_reg(0x0036, 0x0011); // REV_PANEL=1, SM_PANEL=1, GS_PANEL=1, SS_PANEL=1
	//wr_reg(0x0029, 0x0031); // NL[5:0]=110001, 400 lines
	//wr_reg(0x0071, 0x001A); // RTN0

  delayms(20);
	//Gamma 2.2 Setting
	
	wr_reg(0x0040, 0x0000);	
	wr_reg(0x0041, 0x0009);
	wr_reg(0x0042, 0x0012);
	wr_reg(0x0043, 0x0004);
	wr_reg(0x0044, 0x0000);
	wr_reg(0x0045, 0x0023);
	wr_reg(0x0046, 0x0003);
	wr_reg(0x0047, 0x005E);
	wr_reg(0x0048, 0x0000);
	wr_reg(0x0049, 0x0000);
	wr_reg(0x004A, 0x0000);
	wr_reg(0x004B, 0x0000);
	wr_reg(0x004C, 0x0000);
	wr_reg(0x004D, 0x0000);
	wr_reg(0x004E, 0x0000);
	wr_reg(0x0057, 0x004F);

#ifdef ORIENTATION_VERTICAL
	wr_reg(0x0016, 0x00C8);	
#else
	wr_reg(0x0016, 0x00A8);//A8
#endif
	wr_reg(0x0028, 0x0038); //GON=1; DTE=1; D[1:0]=10
	delayms(40);
	wr_reg(0x0028, 0x003C); //GON=1; DTE=1; D[1:0]=11

	delayms(10);

}


void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	 wr_reg(0x0080, Xpos>>8);
	 wr_reg(0x0081, Xpos&0xFF);
	 wr_reg(0x0082, Ypos>>8);
	 wr_reg(0x0083, Ypos&0xFF); 
}

void LCD_Window(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
	 wr_reg(0x0002, Xstart>>8);
	 wr_reg(0x0003, Xstart&0xFF);
	 wr_reg(0x0004, Xend>>8);
	 wr_reg(0x0005, Xend&0xFF);
	
	 wr_reg(0x0006, Ystart>>8);
	 wr_reg(0x0007, Ystart&0xFF);
	 wr_reg(0x0008, Yend>>8);
	 wr_reg(0x0009, Yend&0xFF);
	
#ifdef ORIENTATION_VERTICAL
	LCD_SetCursor(Xstart , Ystart);	
#else
	LCD_SetCursor(Ystart , Xstart);
#endif
}


void LCD_WindowMax (void) 
{
	LCD_Window(0, 0, WIDTH-1, HEIGHT-1);
}

void LCD_PutPixel (uint16_t x, uint16_t y, uint16_t color) 
{
#ifdef ORIENTATION_VERTICAL
	LCD_SetCursor(x, y);
#else
	LCD_SetCursor(y, x);
#endif	
	LCD_WR_REG(0x22);  					
	LCD_WR_DATA(color);
	//LCD_WindowMax();
}



uint16_t change_color(unsigned short color,float r_factor,float g_factor,float b_factor){
		r_factor /= 5;
g_factor /= 5;
b_factor /= 5;	
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




void LCD_Clear (unsigned short color) 
{
	unsigned int   x,y;

	LCD_WindowMax();
		  
	LCD_WR_REG(0x22);
	
	
	for(y = 0; y <HEIGHT; y++)
	{
		for (x=0; x<WIDTH; x++)
		{
			
			LCD_WR_DATA(color);
		}	
	}
}

void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{                    
  uint32_t x,y;
											
	LCD_Window(xsta, ysta, xend, yend);			
	LCD_WR_REG(0x22);  					
	
	for (y=0; y<yend-ysta+1; y++)
	{
		for (x=0; x<xend-xsta+1; x++)
		{
			LCD_WR_DATA(color);
		}
	}	
	LCD_WindowMax();	
} 



void LCD_SetBackColor (unsigned short color) 
{
  BACK_COLOR = color;
}


void LCD_Show_Image(int x0, int y0, int w, int h, const uint16_t *code,float r_factor, float g_factor, float b_factor)
{
		for (uint16_t j = 0; j < h; j++) {
						for (uint16_t i = 0; i < w; i++) {
								uint16_t color = code[j * w + i];
					
								uint16_t newColor = change_color(color,r_factor,g_factor,b_factor);

								LCD_PutPixel(x0 + i, y0 + j, newColor);
						}
		}
}


void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode)
{  							  
  uint16_t temp;
	uint16_t t1,t,Fsize;
	uint16_t colortemp=POINT_COLOR;      			      
	num=num-' ';
				if(size==26)Fsize=16;
			  else Fsize=11;
	
	uint16_t x0=x;
	if(!mode)
	{
	    for(t=0;t<size;t++)
	    {   
				x=x0;
				y++;
				if(size==26)temp=Font26[num][t];
			  else temp=Font18[num][t];
				
	        for(t1=0;t1<Fsize;t1++)
					{			    
		        if(temp&0x8000) 
								LCD_PutPixel(x, y, colortemp);
						else 
								LCD_PutPixel(x, y, BACK_COLOR);
						
				     if(x>=WIDTH){POINT_COLOR=colortemp;return;}
						 
						temp<<=1;
						x++;
				
					} 
			
	    }    
		}
	else{
	   for(t=0;t<size;t++)
	    {   
				x=x0;
				y++;
				if(size==26)temp=Font26[num][t];
			  else temp=Font18[num][t];
				
	        for(t1=0;t1<Fsize;t1++)
					{			    
		        if(temp&0x8000) 
								LCD_PutPixel(x, y, colortemp);
				     if(x>=WIDTH){POINT_COLOR=colortemp;return;}
						 
						temp<<=1;
						x++;
				
					} 
			
	    } 
	  }
			
	POINT_COLOR=colortemp;	    	   	 	  
} 




void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
				if(size==26)x+=3;
        p++;
    }  
}


void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1;
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1;
	else if(delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x;
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )
	{  
		LCD_PutPixel(uRow, uCol, POINT_COLOR);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
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


/* USERhgjg */
