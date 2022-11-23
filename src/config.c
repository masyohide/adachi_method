/*
 * config.c
 *
 * Data   : 2017.10.26
 * Author : M.Kato
 * Update : 2017.12.21
 */

#include <stdio.h>
#include <stm32f10x.h>
#include "flash.h"
#include "config.h"
#include "lcd.h"
#include "global_var.h"
#include "iodefine.h"


// CLOCK
#define HSIcal 0x8 // 内蔵クロックキャリブレーション定数 0x0 - 0xf
void RCC_config()
{
	//-----------------------------------------------------------------------------
	//                           クロックの設定
	// 内蔵RC発振子(HSI) --> PLL : 64MHz
	//-----------------------------------------------------------------------------
	RCC_DeInit();
	// Enable Prefetch Buffer
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	// Flash 2 wait state
	FLASH_SetLatency(FLASH_Latency_2);
	// HCLK = SYSCLK
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	// PCLK2 = HCLK
	RCC_PCLK2Config(RCC_HCLK_Div1);
	// PCLK1 = HCLK/2
	RCC_PCLK1Config(RCC_HCLK_Div2);
	// ADCCLK = 64 / 6 = 10.7MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	// PLLCLK = 4MHz * 16 = 64 MHz
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
	RCC_AdjustHSICalibrationValue(HSIcal);
	// Enable PLL
	RCC_PLLCmd(ENABLE);
	// Wait till PLL is ready
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	// Select PLL as system clock source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	// Wait till PLL is used as system clock source
	while(RCC_GetSYSCLKSource() != 0x08);

	// SystemCoreClockの値を設定
	SystemCoreClockUpdate();
}

// USART
void USART_Config( void ){
	/* <概要>	USART初期設定
	 * <引数>	なし
	 * <戻り値>	なし
	 */
	// 割り込みテーブル位置のoffset
	init_vector_table_offset();
	// シリアル通信関係の設定
	USART1_DMAC_config();
}
void init_vector_table_offset(){
	//-----------------------------------------------------------------------------
	//                           ベクタテーブル
	// 割り込みテーブルの開始位置 = FLASH先頭アドレス
	//-----------------------------------------------------------------------------
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08000000);
}
DMA_InitTypeDef DMA_InitStructure_USART_TX;
#define TRX_BUFFER_SIZE 64 // 送受信バッファサイズ
struct {
    unsigned int head, end;
    char buff[TRX_BUFFER_SIZE];
} txb,rxb; // FIFOバッファ
void USART1_DMAC_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	// クロックを有効にする
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	// USART1用出力ポートの設定
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// USART1
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	// USART用DMAの初期化
	// 送信用
	DMA_StructInit(&DMA_InitStructure_USART_TX);
	DMA_InitStructure_USART_TX.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));
	DMA_InitStructure_USART_TX.DMA_MemoryBaseAddr = (uint32_t)txb.buff;
	DMA_InitStructure_USART_TX.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure_USART_TX.DMA_BufferSize = 0;
	DMA_InitStructure_USART_TX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure_USART_TX.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure_USART_TX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure_USART_TX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure_USART_TX.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure_USART_TX.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure_USART_TX.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure_USART_TX);
	// 受信用
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)rxb.buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = TRX_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	// バッファクリア
	txb.head = txb.end = rxb.head = rxb.end = 0;
	// USART1によるDMAリクエストon
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	// 受信DMA動作開始
	DMA_Cmd(DMA1_Channel5, ENABLE);

	// USART動作開始
	USART_Cmd(USART1, ENABLE);
}
void outbyte(char c)
{
// 送信バッファに一文字追加
// headに新しく追加し，endは次に送信されるデータを指す
// バッファに空きが無い場合は待機

	// バッファ内データ数をカウントし，空きがない場合待機する
	// バッファフルで待機しているときには割り込みを許可するためにwhileループになっている
	volatile int remain;
	while(1){
		// この関数は多重に実行されるとまずいので割り込みを禁止する
		//mask

		// DMACを一時的に停止
		DMA_Cmd(DMA1_Channel4, DISABLE);

		// 転送データ残数を取得
		remain = DMA_GetCurrDataCounter(DMA1_Channel4);

		// バッファに空きがあればループから抜ける
		if(remain != TRX_BUFFER_SIZE) break;

		// DMAC動作再開
		DMA_Cmd(DMA1_Channel4, ENABLE);

		// 割り込み許可
		//mask

		// バッファに空きができるまで待機（この間割り込みが発生してもよい）
		while((remain = DMA_GetCurrDataCounter(DMA1_Channel4)) == TRX_BUFFER_SIZE);
	}

	// ここの時点でDMACは停止，割り込みは禁止されている

	// 次に書き込もうとする領域がバッファ終端なら前に詰める
	if(txb.head == TRX_BUFFER_SIZE){
		int i = TRX_BUFFER_SIZE - remain; // バッファの空き数
		for(txb.head=0; txb.head+i<TRX_BUFFER_SIZE; txb.head++){
			txb.buff[txb.head] = txb.buff[txb.head+i];
		}
	}

	// 1文字追加
	txb.buff[txb.head++] = c;
	remain++;
	txb.end = txb.head - remain;

	// DMACの再設定
	DMA_InitStructure_USART_TX.DMA_MemoryBaseAddr = (uint32_t)(&txb.buff[txb.end]);
	DMA_InitStructure_USART_TX.DMA_BufferSize = remain;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure_USART_TX);

	// DMAC動作再開
	DMA_Cmd(DMA1_Channel4, ENABLE);

	// 割り込み許可
	//mask
}
volatile char inkey()
{
// 受信バッファから一文字取得
// headに新しく受信データが蓄積され，endから読み出せばいい
// 受信データがあるときはその文字を返す return c
// 受信バッファが空のときはヌル文字を返す return '\0'

	// この関数は多重に実行されるとまずいので割り込みを禁止する
	//mask

	// headの位置を読み取る
	unsigned short remain = DMA_GetCurrDataCounter(DMA1_Channel5);
	rxb.head = TRX_BUFFER_SIZE - remain;

	// バッファが空ならヌル文字を返す
	if(rxb.head == rxb.end){
		// 割り込み許可
		//mask

		return '\0';
	}

	// バッファから一文字読み取り返す
	char c;
	c = rxb.buff[rxb.end++];
	if(rxb.end == TRX_BUFFER_SIZE) rxb.end = 0;

	// 割り込み許可
	//mask

	return c;
}
void clear_rxb()
{
	while(inkey()!=0);
}
char inbyte()
{
// 受信バッファから一文字取得
// 受信データが来るまで待機

	volatile char c;
	while((c = inkey()) == '\0');
	return c;
}

// LED
void LED_Init( void ){
	/* <概要>	LED初期設定
	 * <引数>	なし
	 * <戻り値>	なし
	 */
	RCC_APB2PeriphClockCmd( LED_RCC, ENABLE );

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( LED1_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = LED2_PIN;
	GPIO_Init( LED2_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = LED3_PIN;
	GPIO_Init( LED3_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = LED4_PIN;
	GPIO_Init( LED4_PORT, &GPIO_InitStructure );
}
void LED_All_On( void ){
	/* <概要>	全てのLED点灯
	 * <引数>	なし
	 * <戻り値>	なし
	 */
	GPIO_SetBits( LED1_PORT, LED1_PIN );
	GPIO_SetBits( LED2_PORT, LED2_PIN );
	GPIO_SetBits( LED3_PORT, LED3_PIN );
	GPIO_SetBits( LED4_PORT, LED4_PIN );
}
void LED_All_Off( void ){
	/* <概要>	全てのLED消灯
	 * <引数>	なし
	 * <戻り値>	なし
	 */
	GPIO_ResetBits( LED1_PORT, LED1_PIN );
	GPIO_ResetBits( LED2_PORT, LED2_PIN );
	GPIO_ResetBits( LED3_PORT, LED3_PIN );
	GPIO_ResetBits( LED4_PORT, LED4_PIN );
}
void LED_Controll( int pattern ){
	/* <概要>	LED点灯パターン指定
	 * <引数>	pattern	: 右端のLEDを下位ビットとして順に指定
	 * <戻り値>	なし
	 */
	// 引数に応じてLED点灯パターンを決定
	if( (pattern & 0b0001) == 0b0001 )	GPIO_SetBits  ( LED1_PORT, LED1_PIN );
	else								GPIO_ResetBits( LED1_PORT, LED1_PIN );
	if( (pattern & 0b0010) == 0b0010 )	GPIO_SetBits  ( LED2_PORT, LED2_PIN );
	else								GPIO_ResetBits( LED2_PORT, LED2_PIN );
	if( (pattern & 0b0100) == 0b0100 )	GPIO_SetBits  ( LED3_PORT, LED3_PIN );
	else								GPIO_ResetBits( LED3_PORT, LED3_PIN );
	if( (pattern & 0b1000) == 0b1000 )	GPIO_SetBits  ( LED4_PORT, LED4_PIN );
	else								GPIO_ResetBits( LED4_PORT, LED4_PIN );
}

// SW
void SW_Init( void ){
	/* <概要>   : SW初期設定
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	RCC_APB2PeriphClockCmd( SW_RCC, ENABLE );

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SW1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( SW1_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = SW2_PIN;
	GPIO_Init( SW2_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = SW3_PIN;
	GPIO_Init( SW3_PORT, &GPIO_InitStructure );
}

// BUZZER
void BUZZER_Init( void ){
	/* <概要>   : BUZZER初期設定
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	RCC_APB2PeriphClockCmd( BUZZER_RCC, ENABLE );

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( BUZZER_PORT, &GPIO_InitStructure );

	// TIM5で駆動するための設定
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM5, ENABLE );

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 64 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM5, &TIM_TimeBaseStructure );

	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1000;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init( TIM5, &TIM_OCInitStructure );
	TIM_CtrlPWMOutputs( TIM5, ENABLE );

	TIM_Cmd( TIM5, DISABLE );
}
void BUZZER_ENABLE( void ){
	/* <概要>   : BUZZER出力ON
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	TIM_Cmd( TIM5, ENABLE );
}
void BUZZER_DISABLE( void ){
	/* <概要>   : BUZZER出力OFF
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	TIM_Cmd( TIM5, DISABLE );
}
void BUZZER_Set_Freq( int freq ){
	/* <概要>   : BUZZER周波数変更
	 * <引数>   : 周波数[Hz]
	 * <戻り値> : なし
	 */
	// 周波数から周期に変換
	int time = 1000 * 1000 / freq;
	TIM_SetAutoreload( TIM5, time );
	TIM_SetCompare4( TIM5, time/2 );
}
void BUZZER_Startup( void ){
	/* <概要>   : BUZZER起動時テスト
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	BUZZER_ENABLE();

	BUZZER_Set_Freq( 2000 );
	delay_ms( 100 );

	BUZZER_Set_Freq( 1000 );
	delay_ms( 100 );

	BUZZER_DISABLE();
}

// BATTERY
void BATTERY_Init( void ){
	/* <概要>   : バッテリーAD変換初期設定
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	RCC_APB2PeriphClockCmd( BATTERY_RCC | RCC_APB2Periph_ADC1 , ENABLE );

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = BATTERY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init( BATTERY_PORT, &GPIO_InitStructure );

	ADC_DeInit( ADC1 );
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init( ADC1, &ADC_InitStructure );

	ADC_Cmd( ADC1, ENABLE );

	ADC_ResetCalibration( ADC1 );
	while( ADC_GetResetCalibrationStatus( ADC1 ) );
	ADC_StartCalibration(ADC1);
	while( ADC_GetCalibrationStatus( ADC1 ) );
}
void BATTERY_Show( void ){
	/* <概要>   : バッテリー残量表示
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	// 50回AD変換をして平均値を取得
	float bat_row = 0;
	for( int i=0; i<50; i++ ){
		bat_row += Get_ADC1( BATTERY_ADC_CH );
	}
	bat_row /= 50.0;

	// 電源電圧に変換( 12kと39kの抵抗で分圧 )
	float bat = (12.0 + 39.0) / 12.0 * 3.3 / 4095.0 * bat_row;
	// 電源電圧を百分率に変換( min:10.0V, MAX:12.6V )
	int bat_per = (bat - 10.0) / 2.6 * 100;

	// PCに送信
	printf("bat=%.2lfV %d%%\r\n",bat , bat_per);

	// LCDに情報を表示
	LCD_printf1( "bat=%.2lfV  %d%%", bat, bat_per );

	// バッテリー残量低下を警告
	if( bat_per < 20 ){
		LCD_printf2( "replace battery");
		while( 1 ){
			LED_All_On();
			delay_ms( 250 );
			LED_All_Off();
			delay_ms( 250 );
		}
	}

	delay_ms( 1000 );
}

// MOTER_DRIVER
void MOTER_DRIVER_Init( void ){
	/* <概要>   : モータドライバ初期設定
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	RCC_APB2PeriphClockCmd( MOTER_RCC, ENABLE );

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = MOTER_EN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( MOTER_EN_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = MOTER_CW_L_PIN;
	GPIO_Init( MOTER_CW_L_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = MOTER_CW_R_PIN;
	GPIO_Init( MOTER_CW_R_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = MOTER_PWM_L_PIN;
	GPIO_Init( MOTER_PWM_L_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = MOTER_PWM_R_PIN;
	GPIO_Init( MOTER_PWM_R_PORT, &GPIO_InitStructure );

	MOTER_DRIVER_Forward();
	MOTER_DRIVER_DISABLE();
}
void MOTER_DRIVER_ENABLE( void ){
	/* <概要>   : モータドライバ出力有効
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	GPIO_SetBits( MOTER_EN_PORT, MOTER_EN_PIN );
}
void MOTER_DRIVER_DISABLE( void ){
	/* <概要>   : モータドライバ出力無効
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	GPIO_ResetBits( MOTER_EN_PORT, MOTER_EN_PIN );
}
void MOTER_DRIVER_Forward( void ){
	/* <概要>   : モータ前進
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	GPIO_SetBits  ( MOTER_CW_L_PORT, MOTER_CW_L_PIN );
	GPIO_ResetBits( MOTER_CW_R_PORT, MOTER_CW_R_PIN );
}
void MOTER_DRIVER_Back( void ){
	/* <概要>   : モータ後進
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	GPIO_ResetBits( MOTER_CW_L_PORT, MOTER_CW_L_PIN );
	GPIO_SetBits  ( MOTER_CW_R_PORT, MOTER_CW_R_PIN );
}
void MOTER_DRIVER_Left( void ){
	/* <概要>   : モータ左回転
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	GPIO_ResetBits( MOTER_CW_L_PORT, MOTER_CW_L_PIN );
	GPIO_ResetBits( MOTER_CW_R_PORT, MOTER_CW_R_PIN );
}
void MOTER_DRIVER_Right( void ){
	/* <概要>   : モータ右回転
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	GPIO_SetBits( MOTER_CW_L_PORT, MOTER_CW_L_PIN );
	GPIO_SetBits( MOTER_CW_R_PORT, MOTER_CW_R_PIN );
}
void MOTER_DRIVER_IRQ_Config( void ){
	/* <概要>   : モータドライバ用キャプチャコンペア割り込み設定
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	// 左モータ用
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 10 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 6400 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseStructure );

	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init( TIM2, &TIM_OCInitStructure );
	TIM_OC3PreloadConfig( TIM2, TIM_OCPreload_Disable );

	TIM_ITConfig( TIM2, TIM_IT_CC3, ENABLE );

	TIM_Cmd( TIM2, DISABLE );

	// 右モータ用
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );
	TIM_TimeBaseStructure.TIM_Period = 10 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 6400 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure );

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init( TIM3, &TIM_OCInitStructure );
	TIM_OC3PreloadConfig( TIM3, TIM_OCPreload_Disable );

	TIM_ITConfig( TIM3, TIM_IT_CC3, ENABLE );

	TIM_Cmd( TIM3, DISABLE );
}
void MOTER_DRIVER_PWM_ENABLE( void ){
	/* <概要>   : モータドライバPWM出力有効
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	TIM_Cmd( TIM2, ENABLE );
	TIM_Cmd( TIM3, ENABLE );
}
void MOTER_DRIVER_PWM_DISABLE( void ){
	/* <概要>   : モータドライバPWM出力無効
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	TIM_Cmd( TIM2, DISABLE );
	TIM_Cmd( TIM3, DISABLE );
}
void MOTER_DRIVER_Set_Freq_Left( int freq ){
	/* <概要>   : 左モータ割り込み周期変更
	 * <引数>   : 目標速度
	 * <戻り値> : なし
	 */
	TIM_PrescalerConfig( TIM2, freq*2, TIM_PSCReloadMode_Update );
}
void MOTER_DRIVER_Set_Freq_Right( int freq ){
	/* <概要>   : 右モータ割り込み周期変更
	 * <引数>   : 目標速度
	 * <戻り値> : なし
	 */
	TIM_PrescalerConfig( TIM3, freq*2, TIM_PSCReloadMode_Update );
}

// WALL_LED
void WALL_LED_Init( void ){
	/* <概要>   : 壁センサLED初期設定
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	RCC_APB2PeriphClockCmd( WALL_LED_RCC, ENABLE );

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = WALL_LED1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( WALL_LED1_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = WALL_LED2_PIN;
	GPIO_Init( WALL_LED2_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = WALL_LED3_PIN;
	GPIO_Init( WALL_LED3_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = WALL_LED4_PIN;
	GPIO_Init( WALL_LED4_PORT, &GPIO_InitStructure );
}
void WALL_LED_All_On( void ){
	/* <概要>   : 全ての壁センサ点灯
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	GPIO_SetBits( WALL_LED1_PORT, WALL_LED1_PIN );
	GPIO_SetBits( WALL_LED2_PORT, WALL_LED2_PIN );
	GPIO_SetBits( WALL_LED3_PORT, WALL_LED3_PIN );
	GPIO_SetBits( WALL_LED4_PORT, WALL_LED4_PIN );
}
void WALL_LED_All_Off( void ){
	/* <概要>   : 全ての壁センサ消灯
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	GPIO_ResetBits( WALL_LED1_PORT, WALL_LED1_PIN );
	GPIO_ResetBits( WALL_LED2_PORT, WALL_LED2_PIN );
	GPIO_ResetBits( WALL_LED3_PORT, WALL_LED3_PIN );
	GPIO_ResetBits( WALL_LED4_PORT, WALL_LED4_PIN );
}
void WALL_LED_Controll( int pattern ){
	/* <概要>	壁センサ点灯パターン指定
	 * <引数>	pattern	: 右端のLEDを下位ビットとして順に指定
	 * <戻り値>	なし
	 */
	// 引数に応じてLED点灯パターンを決定
	if( (pattern & 0b0001) == 0b0001 )	GPIO_SetBits  ( WALL_LED1_PORT, WALL_LED1_PIN );
	else								GPIO_ResetBits( WALL_LED1_PORT, WALL_LED1_PIN );
	if( (pattern & 0b0010) == 0b0010 )	GPIO_SetBits  ( WALL_LED2_PORT, WALL_LED2_PIN );
	else								GPIO_ResetBits( WALL_LED2_PORT, WALL_LED2_PIN );
	if( (pattern & 0b0100) == 0b0100 )	GPIO_SetBits  ( WALL_LED3_PORT, WALL_LED3_PIN );
	else								GPIO_ResetBits( WALL_LED3_PORT, WALL_LED3_PIN );
	if( (pattern & 0b1000) == 0b1000 )	GPIO_SetBits  ( WALL_LED4_PORT, WALL_LED4_PIN );
	else								GPIO_ResetBits( WALL_LED4_PORT, WALL_LED4_PIN );
}

// WALL_PT
void WALL_PT_Init( void ){
	/* <概要>   : 壁センサAD変換初期設定
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	RCC_APB2PeriphClockCmd( WALL_PT_RCC | RCC_APB2Periph_ADC1, ENABLE );

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = WALL_PT1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init( WALL_PT1_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = WALL_PT2_PIN;
	GPIO_Init( WALL_PT2_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = WALL_PT3_PIN;
	GPIO_Init( WALL_PT3_PORT, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = WALL_PT4_PIN;
	GPIO_Init( WALL_PT4_PORT, &GPIO_InitStructure );

	// ADC1
	ADC_DeInit( ADC1 );
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init( ADC1, &ADC_InitStructure );

	ADC_Cmd( ADC1, ENABLE );

	ADC_ResetCalibration( ADC1 );
	while( ADC_GetResetCalibrationStatus( ADC1 ) );
	ADC_StartCalibration( ADC1 );
	while( ADC_GetCalibrationStatus( ADC1 ) );
}
void WALL_PT_Get( int channel ){
	/* <概要>   : 指定した壁センサをAD変換し構造体に代入
	 * <引数>   : 壁センサのAD変換チャンネル
	 * <戻り値> : なし
	 */
	int adc_value;

	// 壁センサオフの時AD変換
	WALL_LED_All_Off();
	delay_us( ADC_WAIT_TIME );
	int adc_off_value = Get_ADC1( channel );

	// チャンネルごとにAD変換
	switch( channel ){
	case WALL_PT1_ADC_CH:
		// 指定した壁センサをAD変換し値を取得
		WALL_LED_Controll( 0b0001 );
		delay_us( ADC_WAIT_TIME );
		// 壁センサon-offの値を代入
		adc_value = Get_ADC1( channel ) - adc_off_value;
		if( adc_value < 0 )	adc_value = 0;
		WALL_LED_All_Off();
		// 構造体に代入
		sen_fr.p_value = sen_fr.value;
		sen_fr.value = adc_value;
		sen_fr.diff_value = ABS(sen_fr.value - sen_fr.p_value);
		// 壁の有無を判断
		if( sen_fr.value > senDef.fr_threshold ){
			// 壁がある場合
			sen_fr.is_wall = true;
		}
		else{
			sen_fr.is_wall = false;
		}
		// 壁制御の判断
		if( sen_fr.value > senDef.fr_center ){
			sen_fr.is_control = true;
			sen_fr.error = sen_fr.value - senDef.fr_center;
		}
		else{
			sen_fr.is_control = false;
			sen_fr.error = 0;
		}
		break;
	case WALL_PT2_ADC_CH:
		// 指定した壁センサをAD変換し値を取得
		WALL_LED_Controll( 0b0010 );
		delay_us( ADC_WAIT_TIME );
		adc_value = Get_ADC1( channel ) - adc_off_value;
		if( adc_value < 0 )	adc_value = 0;
		WALL_LED_All_Off();
		// 構造体に代入
		sen_sr.p_value = sen_sr.value;
		sen_sr.value = adc_value;
		sen_sr.diff_value = ABS(sen_sr.value - sen_sr.p_value);
		// 壁の有無を判断
		if( sen_sr.value > senDef.sr_threshold ){
			// 壁がある場合
			sen_sr.is_wall = true;
		}
		else{
			sen_sr.is_wall = false;
		}
		// 壁制御の判断
		if( sen_sr.value > 	senDef.sr_center - WALL_DIFF_THRESHOLD ){
			sen_sr.is_control = true;
			sen_sr.error = sen_sr.value - senDef.sr_center;
		}
		else{
			sen_sr.is_control = false;
			sen_sr.error = 0;
		}
		break;
	case WALL_PT3_ADC_CH:
		// 指定した壁センサをAD変換し値を取得
		WALL_LED_Controll( 0b0100 );
		delay_us( ADC_WAIT_TIME );
		adc_value = Get_ADC1( channel ) - adc_off_value;
		if( adc_value < 0 )	adc_value = 0;
		WALL_LED_All_Off();
		// 構造体に代入
		sen_sl.p_value = sen_sl.value;
		sen_sl.value = adc_value;
		sen_sl.diff_value = ABS(sen_sl.value - sen_sl.p_value);
		// 壁の有無を判断
		if( sen_sl.value > senDef.sl_threshold ){
			// 壁がある場合
			sen_sl.is_wall = true;
		}
		else{
			sen_sl.is_wall = false;
		}
		// 壁制御の判断
		if( sen_sl.value > 	senDef.sl_center - WALL_DIFF_THRESHOLD ){
			sen_sl.is_control = true;
			sen_sl.error = sen_sl.value - senDef.sl_center;
		}
		else{
			sen_sl.is_control = false;
			sen_sl.error = 0;
		}
		break;
	case WALL_PT4_ADC_CH:
		// 指定した壁センサをAD変換し値を取得
		WALL_LED_Controll( 0b1000 );
		delay_us( ADC_WAIT_TIME );
		adc_value = Get_ADC1( channel ) - adc_off_value;
		if( adc_value < 0 )	adc_value = 0;
		WALL_LED_All_Off();
		// 構造体に代入
		sen_fl.p_value = sen_fl.value;
		sen_fl.value = adc_value;
		sen_fl.diff_value = ABS(sen_fl.value - sen_fl.p_value);
		// 壁の有無を判断
		if( sen_fl.value > senDef.fl_threshold ){
			// 壁がある場合
			sen_fl.is_wall = true;
		}
		else{
			sen_fl.is_wall = false;
		}
		// 壁制御の判断
		if( sen_fl.value > senDef.fl_center ){
			sen_fl.is_control = true;
			sen_fl.error = sen_fl.value - senDef.fl_center;
		}
		else{
			sen_fl.is_control = false;
			sen_fl.error = 0;
		}
		break;
	default:
		break;
	}
}

// IRQ
void IRQ_TIMER_Config( void ){
	/* <概要>   : 周期割り込み(1ms)用タイマ設定
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/1000000 -1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit( TIM4, &TIM_TimeBaseStructure );

	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init( TIM4, &TIM_OCInitStructure );
	TIM_OC1PreloadConfig( TIM4, TIM_OCPreload_Disable );

	TIM_ITConfig( TIM4, TIM_IT_CC1, ENABLE );

	TIM_Cmd( TIM4, ENABLE );
}
void NVIC_Config( void ){
	/* <概要>   : NVIC設定
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;		//周期割り込み用
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;		//左モータ用
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;		//右モータ用
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0 ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );
}
// その他
void delay_us( int waittime ){
	/* <概要>   : マイクロ秒時間待ち
	 * <引数>   : 待ち時間[us]
	 * <戻り値> : なし
	 */
	__IO uint16_t delay_time = TIM4->CNT + waittime;

	int over_flag=false;
	if( delay_time > 999 ){
		over_flag = true;
	}

	while( TIM4->CNT + 999*over_flag < delay_time ){}
}
void delay_ms( int waittime ){
	/* <概要>   : ミリ秒時間待ち
	 * <引数>   : 待ち時間[ms]
	 * <戻り値> : なし
	 */
	float delay_time = tim.start_time + (float)waittime/1000;
	while( tim.start_time < delay_time ){}
}
int Get_ADC1( int channel ){
	/* <概要>   : AD変換手動読み取り関数
	 * <引数>   : AD変換をするチャンネル
	 * <戻り値> : AD変換値
	 */
	ADC_RegularChannelConfig( ADC1, channel, 1, ADC_SampleTime_71Cycles5 );
	ADC_SoftwareStartConvCmd( ADC1, ENABLE );
	while(ADC_GetFlagStatus( ADC1, ADC_FLAG_EOC ) == RESET );
	return ADC_GetConversionValue( ADC1 );
}
void Init_All( void ){
	/* <概要>   : 全ての初期設定
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	// 初期設定
	RCC_config();								// クロックの設定
	USART_Config();								// USART初期設定
	NVIC_Config();								// NVIC初期設定
	LED_Init();									// LED初期設定
	SW_Init();									// SW初期設定
	BUZZER_Init();								// ブザー初期設定
	BATTERY_Init();								// バッテリーAD変換初期設定
	MOTER_DRIVER_Init();						// モータドライバ初期設定
	MOTER_DRIVER_IRQ_Config();					// モータドライバ用割り込み設定
	IRQ_TIMER_Config();							// 周期割り込み用タイマ設定
	WALL_LED_Init();							// 壁センサLED初期設定
	WALL_PT_Init();								// 壁センサAD変換初期設定

	// LCD関連
	LCD_Config();								// LCD初期化
	LCD_Clear(0);								// LCD表示クリア

	// 起動時実行関数
	BUZZER_Startup();							// ブザー起動時テスト
	BATTERY_Show();								// バッテリー残量確認

	// 迷路情報の表示
	FLASH_Read_Def_Maze();
	LCD_printf1("size X=%2d Y=%2d", mazeDef.maze_size_x, mazeDef.maze_size_y );
	LCD_printf2("goal X=%2d Y=%2d", mazeDef.maze_goal_x, mazeDef.maze_goal_y );
	delay_ms(1000);

	// センサ初期設定復元
	FLASH_Read_Def_Sensor();
}

