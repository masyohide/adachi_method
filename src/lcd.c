/*
 * lcd.c
 *
 * Data   : 2017.10.26
 * Author : M.Kato
 * Update : 2017.12.21
 */
#include <stdio.h>
#include <stm32f10x.h>
#include <stdarg.h>
#include "iodefine.h"
#include "lcd.h"

//	LCD関連
void LCD_Init(void){
	GPIO_In_Configuration();
	// RS=0:Instruction
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	// R/W=0:Write (Output to LCD)
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	// E=0:Disable
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	// Wait more than 15ms
	LCD_Delay(15);
	// Initial Set : (00),Initial Set(1), 8 bit(1)
	LCD_Write_Init_Inst(0b00110000);
	// Wait more than 4.1ms
	LCD_Delay(5);
	// Initial Set : (00),Initial Set(1), 8 bit(1)
	LCD_Write_Init_Inst(0b00110000);
	// Wait more than 100us
	LCD_Delay(1);
	// Initial Set : (00),Initial Set(1), 8 bit(1)
	LCD_Write_Init_Inst(0b00110000);
	LCD_Delay(1);
	// BF can be checked from this point
	// Initial Set : (00),Initial Set(1), 8 bit(1), 2 lines(1), 5x7 dot font(0), (00)
	LCD_Write_Inst(0b00111000);
	// Display OFF : (0000), Display mode (1), Display off(0), Cursor off(0), Blink off(0)
	LCD_Write_Inst(0b00001000);
	// Display Clear : (0000000), Display Clear(1)
	LCD_Write_Inst(0b00000001);
	// Entry mode : (00000), Entry mode(1), Increment(1), Shift off(0)
	LCD_Write_Inst(0b00000110);
	// Display ON : (0000), Display mode (1), Display on(1), Cursor on(1), Blink off(1)
	LCD_Write_Inst(0b00001111);
}
void LCD_Write_Core(int WriteData){
	// R/W=0: Write (Output to LCD)
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	// Wait: Setup time: tAS(40) : 40ns : 3 clocks
	WAIT_3_CLOCK;
	// Set GPIO as output
	GPIO_Out_Configuration();

	// Output Data
//	GPIO_Write(GPIOX_PORT, (LCD_GPIO_ReadInputData() & 0xFF00) | (int)WriteData);

	int temp_WriteData=(LCD_GPIO_ReadInputData() & 0xFF00) | (int)WriteData;
	int DB0=(temp_WriteData&0b00000001);
	int DB1=(temp_WriteData&0b00000010)>>1;
	int DB2=(temp_WriteData&0b00000100)>>2;
	int DB3=(temp_WriteData&0b00001000)>>3;
	int DB4=(temp_WriteData&0b00010000)>>4;
	int DB5=(temp_WriteData&0b00100000)>>5;
	int DB6=(temp_WriteData&0b01000000)>>6;
	int DB7=(temp_WriteData&0b10000000)>>7;

	if(DB0==0b1){GPIO_SetBits(GPIOB,GPIO_Pin_13);}
		else{GPIO_ResetBits(GPIOB,GPIO_Pin_13);}

	if(DB1==0b1){GPIO_SetBits(GPIOB,GPIO_Pin_1);}
		else{GPIO_ResetBits(GPIOB,GPIO_Pin_1);}

	if(DB2==0b1){GPIO_SetBits(GPIOB,GPIO_Pin_0);}
		else{GPIO_ResetBits(GPIOB,GPIO_Pin_0);}

	if(DB3==0b1){GPIO_SetBits(GPIOC,GPIO_Pin_5);}
		else{GPIO_ResetBits(GPIOC,GPIO_Pin_5);}

	if(DB4==0b1){GPIO_SetBits(GPIOC,GPIO_Pin_4);}
		else{GPIO_ResetBits(GPIOC,GPIO_Pin_4);}

	if(DB5==0b1){GPIO_SetBits(GPIOA,GPIO_Pin_7);}
		else{GPIO_ResetBits(GPIOA,GPIO_Pin_7);}

	if(DB6==0b1){GPIO_SetBits(GPIOA,GPIO_Pin_6);}
		else{GPIO_ResetBits(GPIOA,GPIO_Pin_6);}

	if(DB7==0b1){GPIO_SetBits(GPIOA,GPIO_Pin_5);}
		else{GPIO_ResetBits(GPIOA,GPIO_Pin_5);}

	// E=1: Enable
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	// Wait: Pulse time and holdtime: PWEH(220) + tH(10) : 230ns : 18 clocks
	WAIT_18_CLOCK;
	// E=0: Disable
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	// Wait: tC(500) - PWEH(220) - tH(10) - tAS(40) : 230ns : 18 clocks
	WAIT_18_CLOCK;
}
void LCD_Write_Init_Inst(int WriteData){
	// RS=0: Instruction
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);

	LCD_Write_Core(WriteData);
}
void LCD_Write_Inst(int WriteData){
	while( LCD_Read_Inst() & 0b10000000);
	// RS=0: Instruction
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);

	LCD_Write_Core(WriteData);
}
void LCD_Write_Data(int WriteData){
	while(LCD_Read_Inst() & 0b10000000);
	// RS=1: Data
	GPIO_SetBits(GPIOB, GPIO_Pin_14);

	LCD_Write_Core(WriteData);
}
int LCD_Read_Core(void){
	int ReadData;

	// R/W=1: Read (Output from LCD)
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	// Wait: Setup time: tAS(40) : 40ns : 3 clocks
	WAIT_3_CLOCK;
	// Set GPIO as input
	GPIO_In_Configuration();
	// E=1:Enable
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	// Wait: Pulse time: PWEH(220) : 220ns : 18 clocks
	WAIT_18_CLOCK;

	// Read data from GPIO
	ReadData = LCD_GPIO_ReadInputData();	//ReadData =(int)GPIO_ReadInputData(GPIOX_PORT);

	// E=0:Disable
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	// Wait: Pulse holdtime: tH(10) : 10ns
	// should be passed during commands
	// R/W=0: Write (Output to LCD)
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	//Wait: tC(500) - PWEH(220) - tH(10) - tAS(40) : 230ns : 18 clocks
	WAIT_18_CLOCK;
	return(ReadData);
}
int LCD_GPIO_ReadInputData(void){
	int ReadData=0;
	int DB0=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
	int DB1=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
	int DB2=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
	int DB3=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5);
	int DB4=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4);
	int DB5=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7);
	int DB6=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
	int DB7=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
	return ReadData = ( (DB0) | (DB1<<1) | (DB2<<2) | (DB3<<3) | (DB4<<4) |(DB5<<5) | (DB6<<6) | (DB7<<7) );

}
int LCD_Read_Inst(void){
	// RS=0: Instruction
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);

	return(LCD_Read_Core());
}
int LCD_Read_Data(void){
	// RS=1: Data
	GPIO_SetBits(GPIOB, GPIO_Pin_14);

	return(LCD_Read_Core());
}
void GPIO_Configuration(void){
	/* <概要>   : LCD RS,E,R/W初期設定
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void GPIO_Out_Configuration(void){
	/* <概要>   : LCD DB0~DB7出力初期設定
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC , ENABLE );
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void GPIO_In_Configuration(void){
	/* <概要>   : LCD DB0~DB7入力初期設定
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC , ENABLE );
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void LCD_Config(void){
	GPIO_Configuration();

	GPIO_In_Configuration();

	LCD_Init();
}
void LCD_printf(int line , int row , const char *WriteData){
	/* <概要>   : LCD表示関数
	 * <引数>   : line:行(1または2), row:列(1以上), WriteData:表示する文字列
	 * <戻り値> : なし
	 */
	int i = 0;
	const char *ad;
	ad = WriteData;

	if(line==1){
		//LCD_Clear(1);
		LCD_Write_Inst(0x00+(row-1) | 0b10000000);
		while(ad[i]!='\0'){
			LCD_Write_Data(ad[i]);
			i++;
		}
	}

	if(line==2){
		//LCD_Clear(2);
		LCD_Write_Inst(0x40+(row-1) | 0b10000000);
		while(ad[i]!='\0'){
			LCD_Write_Data(ad[i]);
			i++;
		}
	}
}
void LCD_Clear(int line){
	/* <概要>   : LCDクリア関数
	 * <引数>   : 消去する文字列(0:全ての行, 1:1行目, 2:2行目)
	 * <戻り値> : なし
	 */

	int i=0;
	int8_t ad[]="                ";

	if(line==0){
		while( LCD_Read_Inst() & 0b10000000);
		LCD_Write_Inst(0b00000001);
	}
	else if(line==1){
		LCD_Write_Inst(0x00 | 0b10000000);
		while(ad[i]!='\0'){
			LCD_Write_Data(ad[i]);
			i++;
		}
	}
	else if(line==2){
		LCD_Write_Inst(0x40 | 0b10000000);
		while(ad[i]!='\0'){
			LCD_Write_Data(ad[i]);
			i++;
		}
	}
}

void LCD_printf1(const char* format, ...){
	/* <概要>   : LCD1列目表示関数
	 * <引数>   : format:表示する文字列(使い方はprintfと同じ)
	 * <戻り値> : なし
	 */
	va_list args;
	char buff[64]={};

	va_start( args, format );
	vsprintf(buff, format, args);
	va_end( args );

	LCD_Clear(1);
	LCD_printf(1 , 1 , buff);
}
void LCD_printf2(const char* format, ...){
	/* <概要>   : LCD2列目表示関数
	 * <引数>   : format:表示する文字列(使い方はprintfと同じ)
	 * <戻り値> : なし
	 */
	va_list args;
	char buff[64]={};

	va_start( args, format );
	vsprintf(buff, format, args);
	va_end( args );

	LCD_Clear(2);
	LCD_printf(2 , 1 , buff);
}
void LCD_Delay( int waittime ){
	while( --waittime ){
		for( volatile int i=0; i++; i<1000 );
	}
}
