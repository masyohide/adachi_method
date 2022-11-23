/*
 * lcd.h
 *
 *  Created on: 2017/10/26
 *      Author: x240
 */

#ifndef SRC_LCD_H_
#define SRC_LCD_H_


#define WAIT_3_CLOCK 	__asm__("mov r8,r8\n\tmov r8,r8\n\tmov r8,r8\n\t")
#define WAIT_18_CLOCK	__asm__("mov r8,r8\n\tmov r8,r8\n\tmov r8,r8\n\t"\
								"mov r8,r8\n\tmov r8,r8\n\tmov r8,r8\n\t"\
								"mov r8,r8\n\tmov r8,r8\n\tmov r8,r8\n\t"\
								"mov r8,r8\n\tmov r8,r8\n\tmov r8,r8\n\t"\
								"mov r8,r8\n\tmov r8,r8\n\tmov r8,r8\n\t"\
								"mov r8,r8\n\tmov r8,r8\n\tmov r8,r8\n\t")

//	LCD関連
void LCD_Init(void);
void LCD_Write_Core(int WriteData);
void LCD_Write_Init_Inst(int WriteData);
void LCD_Write_Inst(int WriteData);
void LCD_Write_Data(int WriteData);
int LCD_Read_Core(void);
int LCD_GPIO_ReadInputData(void);
int LCD_Read_Inst(void);
int LCD_Read_Data(void);
void GPIO_Configuration(void);
void GPIO_Out_Configuration(void);
void GPIO_In_Configuration(void);
void LCD_Config(void);
void LCD_printf(int line , int row , const char *WriteData);
void LCD_Clear(int line);

void LCD_printf1(const char* format, ...);
void LCD_printf2(const char* format, ...);
void LCD_Delay( int waittime );

#endif /* SRC_LCD_H_ */
