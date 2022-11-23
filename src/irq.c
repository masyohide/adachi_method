/*
 * iqr.c
 *
 * Data   : 2017.10.26
 * Author : M.Kato
 * Update : 2017.12.21
 */

#include <stdio.h>
#include "config.h"
#include "stm32f10x_it.h"
#include "iodefine.h"
#include "global_var.h"
#include "moter.h"


static int L_Flag = 0;
static int R_Flag = 0;

void TIM4_IRQHandler( void ){
	/* <概要>   : 周期割り込み(1ms)関数
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	// 割り込みフラグ確認
	if(TIM_GetITStatus( TIM4, TIM_IT_CC1 ) != RESET ){
		// 割り込みフラグクリア
		TIM_ClearITPendingBit( TIM4, TIM_IT_CC1 );
		// 経過時間のカウント
		tim.start_time += 0.001;	// 1ms
		// 壁制御計算有効の場合
		if( moter.is_accel_start == true ){
			// センサ取得
			WALL_PT_Get( WALL_PT1_ADC_CH );
			WALL_PT_Get( WALL_PT2_ADC_CH );
			WALL_PT_Get( WALL_PT3_ADC_CH );
			WALL_PT_Get( WALL_PT4_ADC_CH );

			// LEDにセンサ情報を表示
			int LED_pattern = ( ( sen_fl.is_wall << 3 ) | ( sen_sl.is_wall << 2 ) |
					( sen_sr.is_wall << 1 ) | ( sen_fr.is_wall ) );
			LED_Controll( LED_pattern );

			// 加速処理
			moter.vel = moter.vel - moter.accel;
			// 最高速度の制限
			// ステップ数ベースでは小さくなるほど速くなるため
			if( moter.vel < moter.max_vel ){
				moter.vel = moter.max_vel;
			}
			// 最低速度の制限
			else if( moter.vel > MIN_TARGET_VEL ){
				moter.vel = MIN_TARGET_VEL;
			}

			// 壁制御
			MOTER_Calc_Control();
		}
	}
}
void TIM2_IRQHandler( void ){
	/* <概要>   : 左モータ用ステップ計測関数
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	if(TIM_GetITStatus( TIM2, TIM_IT_CC3 ) != RESET ){
		TIM_ClearITPendingBit( TIM2, TIM_IT_CC3 );
		if( L_Flag == 0 ){
			L_Flag = 1;
			GPIO_SetBits( MOTER_PWM_L_PORT, MOTER_PWM_L_PIN );
			moter.step_l++;
		}else{
			L_Flag = 0;
			GPIO_ResetBits( MOTER_PWM_L_PORT, MOTER_PWM_L_PIN );
		}
	}
}
void TIM3_IRQHandler( void ){
	/* <概要>   : 右モータ用ステップ計測関数
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	if(TIM_GetITStatus( TIM3, TIM_IT_CC3 ) != RESET ){
		TIM_ClearITPendingBit( TIM3, TIM_IT_CC3 );
		if( R_Flag == 0 ){
			R_Flag = 1;
			GPIO_SetBits( MOTER_PWM_R_PORT, MOTER_PWM_R_PIN );
			moter.step_r++;
		}else{
			R_Flag = 0;
			GPIO_ResetBits( MOTER_PWM_R_PORT, MOTER_PWM_R_PIN );
		}
	}
}
