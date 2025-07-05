#include "vfd.h"

// void write_6302(SPI_HandleTypeDef *hspi, uint8_t reg,uint8_t w_data)
// {
//     uint8_t buf[2]={0};
//     buf[0] = reg; // Register address
//     buf[1] = w_data; // Data to write
//     VFD_CSL_Set0; // Set chip select low
//     HAL_SPI_Transmit(hspi, buf, 2, HAL_MAX_DELAY); // Transmit data
//     VFD_CSL_Set1; // Set chip select high
// }

// void VFD_init(SPI_HandleTypeDef *hspi)
// {
//     VFD_ENA_Set1; // Enable the VFD

//     // Initialize the VFD
//     VFD_CSL_Set0;
//     // write_6302(hspi, 0xe0); // Command to set DIM
//     // HAL_Delay(2);
//     // write_6302(hspi, 0x07); // Set DIM to 7
//     write_6302(hspi, 0xe0, 0x07); // Command to set DIM
//     VFD_CSL_Set1;
//     HAL_Delay(2);

//     VFD_CSL_Set0;
//     // write_6302(hspi, 0xe4); // Command to set brightness
//     // HAL_Delay(2);
//     // write_6302(hspi, 0xff); // Brightness level
//     write_6302(hspi, 0xe4, 0xff); // Command to set brightness
//     VFD_CSL_Set1;
//     HAL_Delay(2);
// }

// void S1201_show(SPI_HandleTypeDef *hspi)
// {
//     VFD_CSL_Set0; // Start transmission
//     // write_6302(hspi, 0xe8, 0x00); // Set address register start
//     uint8_t cmd = 0xe8; // Command to set address register start
//     HAL_SPI_Transmit(hspi, &cmd, 1, HAL_MAX_DELAY); // Transmit data
//     VFD_CSL_Set1; // Stop transmission
// }

// void S1201_WriteOneChar(SPI_HandleTypeDef *hspi, unsigned char x, unsigned char chr)
// {
//     VFD_CSL_Set0; // Start transmission
//     HAL_Delay(2); // Delay 300ns
//     write_6302(hspi, 0x20 + x, chr + 0x30); // Address register start and character data
//     VFD_CSL_Set1; // Stop transmission
//     S1201_show(hspi); // Update display
// }

// void S1201_WriteStr(SPI_HandleTypeDef *hspi, unsigned char x, char *str)
// {
//     VFD_CSL_Set0; // Start transmission
//     HAL_Delay(2); // Delay 300ns
//     write_6302(hspi, 0x20 + x, 0); // Address register start
//     while (*str)
//     {
//         write_6302(hspi, 0, *str - 0x30); // ASCII to character mapping
//         str++;
//     }
//     VFD_CSL_Set1; // Stop transmission
//     S1201_show(hspi); // Update display
// }

// void VFD_test_out(SPI_HandleTypeDef *hspi)
// {

//   // VFD2 test
//   VFD_ENA_Set1; // Enable the VFD
//   HAL_Delay(100);
//   VFD_RST_Set0;
//   HAL_Delay(100);
//   VFD_RST_Set1;
//   VFD_init(hspi);
//   // HAL_Delay(1000);
//   // S1201_WriteStr(0, "ABCDEFGH");
//   // S1201_show();
//   // HAL_Delay(1000);
//   int ptr = 0;
//   char str[40] = "It's Behind You LOL :D 123456789";
//   while (1)
//   {
//     //Rolling String
//     for (int i = 0; i < 8; i++)
//     {
//       S1201_WriteOneChar(hspi,i, str[(i + ptr)%strlen(str)] - 0x30);
//     }
//     ptr++;
//     ptr%=strlen(str);

//     S1201_show(hspi);
//     HAL_Delay(200);
//   }
// }

void Delay_ms(uint16_t time)
{
	uint16_t i=0;
	while (time--) 
		{
			i=12000;
			while(i--);
		}
}

void Delay_us(uint16_t time)
{    
   uint16_t i=0;  
   while(time--)
   {
      i=10;
      while(i--);    
   }
}

// VFD screen test 


// VFD2 Operation

/******************************
*用于8位数据/命令传输
*******************************/

void write_6302(unsigned char w_data)
 { 
    unsigned char i;      
    for(i=0;i<8;i++)   
  {  
        clk_0;    
        if( (w_data&0x01) == 0x01)       
  { 
           din_1;      
      }        
else       
 { 
            din_0;    
  } 
     w_data>>=1;      
     Delay_us(2);      
     clk_1;    
     Delay_us(2); 
}
 }
/******************************
*初始化
*******************************/
void VFD_init()
 { 
  EN_1;
  HAL_Delay(300);
  reset_0;	
	HAL_Delay(300);
	reset_1;	
  
  cs_0;
  write_6302(0xe0); 
  Delay_us(2);  
  write_6302(0x07);//DIM 0~7 
  cs_1;
  Delay_us(2);

  cs_0;
  write_6302(0xe4); 
  Delay_us(2);  
  write_6302(0xff);//bright	    write_6302(0xff)
  cs_1;
  Delay_us(2);
  }

/******************************
*用于更新当前屏幕显示
*******************************/
void S1201_show(void)
{
  cs_0; //开始传输
  Delay_us(2);    //延时300ns 
  write_6302(0xe8);//地址寄存器起始位置
  cs_1; //停止传输
}
/******************************
*在指定位置打印一个字符(用户自定义,所有CG-ROM中的)
*x:0~11;chr:要显示的字符编码
*******************************/
void S1201_WriteOneChar(unsigned char x, unsigned char chr)
{
  cs_0; //开始传输
  Delay_us(2);     //延时300ns  
  write_6302(0x20+x);//地址寄存器起始位置
  Delay_us(2); 
  write_6302(chr+0x30); 
  cs_1; //停止传输	
 // S1201_show();	
}
/******************************
*在指定位置打印字符串
*(仅适用于英文,标点,数字)
*x:0~11;str:要显示的字符串
*******************************/
void S1201_WriteStr(unsigned char x, char *str)
{
  cs_0; //开始传输
  Delay_us(2);    //延时300ns
  write_6302(0x20+x);//地址寄存器起始位置	
  while (*str) 
  {
    write_6302(*str); //ascii与对应字符表转换    
    str++;
  }  
  cs_1; //停止传输	
  //S1201_show();	
}

void VFD_test_out(void) 
{
  EN_1; //使能VFD
  //VFD2 test
  reset_0;	
	Delay_ms(300);
	reset_1;	
	VFD_init();

	while(1)
	{
	
	//亮度//	
  cs_0;
  write_6302(0xe4); 
  Delay_us(2);  
  write_6302(0x88);//bright 0-255
  cs_1;
  Delay_us(2);

	//亮度//
    // EN_1; //使能VFD
    // HAL_Delay(100);
		S1201_WriteOneChar(0,0);
		S1201_WriteOneChar(1,1);
		S1201_WriteOneChar(2,2);
		S1201_WriteOneChar(3,3);
		S1201_WriteOneChar(4,4);
		S1201_WriteOneChar(5,5);
		S1201_WriteOneChar(6,6);
		S1201_WriteOneChar(7,7);
		 S1201_show();	
   
		 HAL_Delay(1000);
		 S1201_WriteStr(0,"ABCDEFGH")	;
		 S1201_show();
		 HAL_Delay(1000);
    //  EN_0; //禁用VFD
    //  HAL_Delay(1000);


 }
}