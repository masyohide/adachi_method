#ifdef __cplusplus
extern "C" {
#endif

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

// CLOCK
void RCC_config();

// USART
void USART_Config( void );						// USART初期設定
void init_vector_table_offset();
void USART1_DMAC_config();
volatile char inkey();
void clear_rxb();
char inbyte();
void outbyte(char c);

// LED
void LED_Init( void );							// LED初期設定
void LED_All_On( void );						// 全てのLED点灯
void LED_All_Off( void );						// 全てのLED消灯
void LED_Controll( int pattern );				// LED点灯パターン指定

// SW
void SW_Init( void );							// SW初期設定

// BUZZER
void BUZZER_Init( void );						// ブザー初期設定
void BUZZER_ENABLE( void );					// ブザー出力ON
void BUZZER_DISABLE( void );					// ブザー出力OFF
void BUZZER_Set_Freq( int freq );				// ブザー周波数変更
void BUZZER_Startup( void );					// ブザー起動時テスト

// BATTERY
void BATTERY_Init( void );						// バッテリーAD変換初期設定
void BATTERY_Show( void );						// バッテリー残量表示

// MOTER_DRIVER
void MOTER_DRIVER_Init( void );				// モータドライバ初期設定
void MOTER_DRIVER_ENABLE( void );				// モータドライバ出力有効
void MOTER_DRIVER_DISABLE( void );			// モータドライバ出力無効
void MOTER_DRIVER_Forward( void );			// モータ前進
void MOTER_DRIVER_Back( void );				// モータ後進
void MOTER_DRIVER_Left( void );				// モータ左回転
void MOTER_DRIVER_Right( void );				// モータ右回転
void MOTER_DRIVER_IRQ_Config( void );			// モータドライバ用キャプチャコンペア割り込み設定
void MOTER_DRIVER_PWM_ENABLE( void );			// モータドライバPWM出力有効
void MOTER_DRIVER_PWM_DISABLE( void );		// モータドライバPWM出力無効
void MOTER_DRIVER_Set_Freq_Left( int freq );	// 左モータ割り込み周期変更
void MOTER_DRIVER_Set_Freq_Right( int freq );	// 右モータ割り込み周期変更

// WALL_LED
void WALL_LED_Init( void );					// 壁センサLED初期設定
void WALL_LED_All_On( void );					// 全ての壁センサ点灯
void WALL_LED_All_Off( void );					// 全ての壁センサ消灯
void WALL_LED_Controll( int pattern );			// 壁センサ点灯パターン指定

// WALL_PT
void WALL_PT_Init( void );						// 壁センサAD変換初期設定
void WALL_PT_Get( int channel );				// 指定した壁センサをAD変換し構造体に代入

// IRQ
void IRQ_TIMER_Config( void );					// 周期割り込み(1ms)用タイマ設定
void NVIC_Config( void );						// NVIC初期設定

// その他
void delay_us( int waittime );					// マイクロ秒時間待ち
void delay_ms( int waittime );					// ミリ秒時間待ち
int Get_ADC1( int channel );					// AD変換手動読み取り関数
void Init_All( void );							// 全ての初期設定


#endif /* SRC_CONFIG_H_ */
#ifdef __cplusplus
}
#endif
