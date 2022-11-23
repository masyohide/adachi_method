/*
 * moter.c
 *
 * Data   : 2017.10.26
 * Author : M.Kato
 * Update : 2017.12.21
 */

#include <stm32f10x.h>
#include <stdio.h>
#include "config.h"
#include "global_var.h"
#include "iodefine.h"
#include "moter.h"


void MOTER_Straight( int len ){
	/* <概要>   : 指定距離直進
	 * <引数>   : 進む距離[mm]
	 * <戻り値> : なし
	 */
	// 壁制御計算無効
	moter.is_accel_start = false;
	// パラメータ初期化
	MOTER_Set_Param();
	// 機体速度の初期化(加速あり)
	moter.vel = MIN_TARGET_VEL;
	// 壁制御有効
	control.enable = true;
	// 前壁制御有効
	//control.front_enable = true;
	control.front_enable = false;
	// ステップ数の初期化
/*	moter.step_l = moter.step_r = 0; */
	// モータ前進
	MOTER_DRIVER_Forward();
	// 目標ステップ数の計算
	float tar_step = 2.0 * 400.0 * len / TIRE_CIRCUIT;
	// 加速に要するステップ数の計算
	float accel_step = 0;
	if( moter.accel != 0 ){
		accel_step = 2 * ABS( moter.vel - moter.max_vel ) / moter.tar_accel;
	}
	// モータドライバ出力有効
	MOTER_DRIVER_ENABLE();
	delay_ms(100);
	// 壁制御計算有効
	moter.is_accel_start = true;
	// モータドライバPWM出力有効
	MOTER_DRIVER_PWM_ENABLE();

	// 前壁制御有効の場合
	if( control.front_enable == true ){
		while(1){
			// 減速の場合加速度をマイナスに設定
			if( moter.step_l + moter.step_r > tar_step - accel_step ){
				control.enable = false;
				moter.accel = - moter.tar_accel;
			}
			// 前壁がある場合
			if( sen_fl.is_wall == true && sen_fr.is_wall == true ){
				// 前壁に近づいた場合
				if( sen_fl.is_control == true && sen_fr.is_control == true ){
					// 目標ステップの半分以上走行した場合走行を終了
					if( moter.step_l + moter.step_r > tar_step / 2 ){
						break;
					}
				}
			}
			// 前壁がない場合
			if( sen_fl.is_wall == false || sen_fl.is_wall == false ){
				// 目標ステップまで走行
				if( moter.step_l + moter.step_r > tar_step ){
					break;
				}
			}
		}
	}
	// 前壁制御無効の場合
	else{
		// 目標ステップまで走行
		while( moter.step_l + moter.step_r < tar_step ){}
	}

	moter.step_l = moter.step_r = 0;

	// モータドライバPWM出力無効
/*	MOTER_DRIVER_PWM_DISABLE(); */
	// 壁制御計算無効
	moter.is_accel_start = false;
	// モータドライバ出力無効
/*	delay_ms(100); */
/*	MOTER_DRIVER_DISABLE(); */
}

void MOTER_Straight2( int len ){
	/* <概要>   : 指定距離直進
	 * <引数>   : 進む距離[mm]
	 * <戻り値> : なし
	 */
	// 壁制御計算無効
	moter.is_accel_start = false;
	// パラメータ初期化
	MOTER_Set_Param();
	// 機体速度の初期化(加速あり)
	moter.vel = MIN_TARGET_VEL;
	// 壁制御有効
	control.enable = true;
	// 前壁制御有効
	//control.front_enable = true;
	control.front_enable = false;
	// ステップ数の初期化
/*	moter.step_l = moter.step_r = 0; */
	// モータ前進
	MOTER_DRIVER_Forward();
	// 目標ステップ数の計算
	float tar_step = 2.0 * 400.0 * len / TIRE_CIRCUIT;
	// 加速に要するステップ数の計算
	float accel_step = 0;
	if( moter.accel != 0 ){
		accel_step = 2 * ABS( moter.vel - moter.max_vel ) / moter.tar_accel;
	}
	// モータドライバ出力有効
	MOTER_DRIVER_ENABLE();
	delay_ms(100);
	// 壁制御計算有効
	moter.is_accel_start = true;
	// モータドライバPWM出力有効
	MOTER_DRIVER_PWM_ENABLE();

	// 前壁制御有効の場合
	if( control.front_enable == true ){
		while(1){
			// 減速の場合加速度をマイナスに設定
			if( moter.step_l + moter.step_r > tar_step - accel_step ){
				control.enable = false;
				moter.accel = - moter.tar_accel;
			}
			// 前壁がある場合
			if( sen_fl.is_wall == true && sen_fr.is_wall == true ){
				// 前壁に近づいた場合
				if( sen_fl.is_control == true && sen_fr.is_control == true ){
					// 目標ステップの半分以上走行した場合走行を終了
					if( moter.step_l + moter.step_r > tar_step / 2 ){
						break;
					}
				}
			}
			// 前壁がない場合
			if( sen_fl.is_wall == false || sen_fl.is_wall == false ){
				// 目標ステップまで走行
				if( moter.step_l + moter.step_r > tar_step ){
					break;
				}
			}
		}
	}
	// 前壁制御無効の場合
	else{
		// 目標ステップまで走行
		while( moter.step_l + moter.step_r < tar_step ){}
	}

	moter.step_l = moter.step_r = 0;

	// モータドライバPWM出力無効
	MOTER_DRIVER_PWM_DISABLE();
	// 壁制御計算無効
	moter.is_accel_start = false;
	// モータドライバ出力無効
/*	delay_ms(100); */
/*	MOTER_DRIVER_DISABLE(); */
}

void MOTER_Straight3( int len ){
	/* <概要>   : 指定距離直進
	 * <引数>   : 進む距離[mm]
	 * <戻り値> : なし
	 */
	// 壁制御計算無効
	moter.is_accel_start = false;
	// パラメータ初期化
	MOTER_Set_Param();
	// 機体速度の初期化(加速あり)
	moter.vel = MIN_TARGET_VEL;
	// 壁制御有効
	control.enable = true;
	// 前壁制御有効
	//control.front_enable = true;
	control.front_enable = false;
	// ステップ数の初期化
	moter.step_l = moter.step_r = 0;
	// モータ前進
	MOTER_DRIVER_Forward();
	// 目標ステップ数の計算
	float tar_step = 2.0 * 400.0 * len / TIRE_CIRCUIT;
	// 加速に要するステップ数の計算
	float accel_step = 0;
	if( moter.accel != 0 ){
		accel_step = 2 * ABS( moter.vel - moter.max_vel ) / moter.tar_accel;
	}
	// モータドライバ出力有効
	MOTER_DRIVER_ENABLE();
	delay_ms(100);
	// 壁制御計算有効
	moter.is_accel_start = true;
	// モータドライバPWM出力有効
	MOTER_DRIVER_PWM_ENABLE();

	// 前壁制御有効の場合
	if( control.front_enable == true ){
		while(1){
			// 減速の場合加速度をマイナスに設定
			if( moter.step_l + moter.step_r > tar_step - accel_step ){
				control.enable = false;
				moter.accel = - moter.tar_accel;
			}
			// 前壁がある場合
			if( sen_fl.is_wall == true && sen_fr.is_wall == true ){
				// 前壁に近づいた場合
				if( sen_fl.is_control == true && sen_fr.is_control == true ){
					// 目標ステップの半分以上走行した場合走行を終了
					if( moter.step_l + moter.step_r > tar_step / 2 ){
						break;
					}
				}
			}
			// 前壁がない場合
			if( sen_fl.is_wall == false || sen_fl.is_wall == false ){
				// 目標ステップまで走行
				if( moter.step_l + moter.step_r > tar_step ){
					break;
				}
			}
		}
	}
	// 前壁制御無効の場合
	else{
		// 目標ステップまで走行
		while( moter.step_l + moter.step_r < tar_step ){}
	}

	// モータドライバPWM出力無効
	MOTER_DRIVER_PWM_DISABLE();
	// 壁制御計算無効
	moter.is_accel_start = false;
	// モータドライバ出力無効
	delay_ms(100);
	MOTER_DRIVER_DISABLE();
}

void MOTER_Straight_Search( int len ){
	/* <概要>   : 指定距離直進
	 * <引数>   : 進む距離[mm]
	 * <戻り値> : なし
	 */
	// パラメータ初期化
	MOTER_Set_Param();
	// 機体速度の初期化(加速なし)
	moter.max_vel = MIN_TARGET_VEL;
	moter.accel = 0;
	// ステップ数の初期化
	moter.step_l = moter.step_r = 0;
	// モータ前進
	MOTER_DRIVER_Forward();
	// 目標ステップ数の計算
	float tar_step = 2.0 * 400.0 * len / TIRE_CIRCUIT;

	// モータドライバ出力有効
	MOTER_DRIVER_ENABLE();
	// モータドライバPWM出力有効
	MOTER_DRIVER_PWM_ENABLE();

	// 前壁制御有効の場合
	if( control.front_enable == true ){
		while(1){
			// 前壁がある場合
			if( sen_fl.is_wall == true && sen_fr.is_wall == true ){
				// 前壁に近づいた場合
				if( sen_fl.is_control == true && sen_fr.is_control == true ){
					// 目標ステップの半分以上走行した場合走行を終了
					if( moter.step_l + moter.step_r > tar_step / 2 ){
						break;
					}
				}
			}
			// 前壁がない場合
			if( sen_fl.is_wall == false || sen_fl.is_wall == false ){
				// 目標ステップまで走行
				if( moter.step_l + moter.step_r > tar_step ){
					break;
				}
			}
		}
	}
	// 前壁制御無効の場合
	else{
		// 目標ステップまで走行
		while( (moter.step_l + moter.step_r) < tar_step ){}
	}
}
void MOTER_Turn( int dir, int deg ){
	/* <概要>   : 指定方向に指定角度回転
	 * <引数>   : 回転方向,回転角度[deg]
	 * <戻り値> : なし
	 */
	// 壁制御計算無効
	moter.is_accel_start = false;
	// パラメータ初期化
	MOTER_Set_Param();
	// 角速度設定
	MOTER_DRIVER_Set_Freq_Left ( moter.max_ang_vel );
	MOTER_DRIVER_Set_Freq_Right( moter.max_ang_vel );
	// 壁制御無効
	control.enable = false;
	// 前壁制御無効
	control.front_enable = false;
	// ステップ数の初期化
	moter.step_l = moter.step_r = 0;
	// 回転方向の設定
	if( dir == left ){
		MOTER_DRIVER_Left();
	}else{
		MOTER_DRIVER_Right();
	}
	// 目標ステップ数の計算
	float tar_step = 2.0 * 400.0 * deg / 90.0 * TREAD_CIRCUIT / TIRE_CIRCUIT;
	// モータドライバ出力有効
	MOTER_DRIVER_ENABLE();
	delay_ms(100);
	// モータドライバPWM出力有効
	MOTER_DRIVER_PWM_ENABLE();

	// 目標ステップまで走行
	while( moter.step_l + moter.step_r < tar_step );


/*	moter.step_l = moter.step_r = 0; */
	// モータドライバPWM出力無効
	MOTER_DRIVER_PWM_DISABLE();
	// モータドライバ出力無効
	delay_ms(100);
	MOTER_DRIVER_DISABLE();
}
void MOTER_Back( int len ){
	/* <概要>   : 指定距離後進
	 * <引数>   : 進む距離[mm]
	 * <戻り値> : なし
	 */
	// 壁制御計算無効
	moter.is_accel_start = false;
	// パラメータ初期化
	MOTER_Set_Param();
	moter.max_vel = 2800;
	// 機体速度の初期化(加速なし)
	moter.vel = moter.max_vel;
	moter.accel = 0;
	// 速度設定
	MOTER_DRIVER_Set_Freq_Left ( moter.max_vel );
	MOTER_DRIVER_Set_Freq_Right( moter.max_vel );
	// 壁制御無効
	control.enable = false;
	// 前壁制御無効
	control.front_enable = false;
	// ステップ数の初期化
	moter.step_l = moter.step_r = 0;
	// モータ後進
	MOTER_DRIVER_Back();
	// 目標ステップ数の計算
	float tar_step = 2.0 * 400.0 * len / TIRE_CIRCUIT;
	// モータドライバ出力有効
	MOTER_DRIVER_ENABLE();
	delay_ms(100);
	// モータドライバPWM出力有効
	MOTER_DRIVER_PWM_ENABLE();

	// 目標ステップまで走行
	while( moter.step_l + moter.step_r < tar_step );

	// モータドライバPWM出力無効
	MOTER_DRIVER_PWM_DISABLE();
	// モータドライバ出力無効
	delay_ms(100);
	MOTER_DRIVER_DISABLE();
}
void MOTER_Calc_Control( void ){
	/* <概要>   : モータ制御量計算
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	// 壁制御が有効の場合
	if( control.enable == true ){
		// 両壁がある場合の偏差を計算
		if( ( sen_sr.is_control == true ) && ( sen_sl.is_control == true ) ){
			control.error = sen_sr.error - sen_sl.error;
		}
		// 片壁または両壁がない場合の偏差を計算
		else{
			control.error = 2.0 * ( sen_sr.error - sen_sl.error );
		}
		control.control = 0.001 * moter.max_vel * control.kp * control.error;
	}
	else{
		control.control = 0;
	}

	MOTER_DRIVER_Set_Freq_Left ( (int)( moter.vel + control.control ) );
	MOTER_DRIVER_Set_Freq_Right( (int)( moter.vel - control.control ) );
}
void MOTER_Set_Param( void ){
	/* <概要>   : モータ関連パラメータ設定
	 * <引数>   : パラメータ
	 * <戻り値> : なし
	 */
	switch( moter.parameter ){
	case param_search:
		moter.max_vel = MIN_TARGET_VEL;
		moter.tar_accel = 0.0;
		moter.accel = moter.tar_accel;
		moter.max_ang_vel = 2500;
		control.kp = 0.12;//初期0.12
		break;
	case param_slow:
		moter.max_vel = 1500;
		moter.tar_accel = 5.0;
		moter.accel = moter.tar_accel;
		moter.max_ang_vel = 2500;
		control.kp = 0.14;
		break;
	case param_medium:
		moter.max_vel = 1000;
		moter.tar_accel = 5.0;
		moter.accel = moter.tar_accel;
		moter.max_ang_vel = 2500;
		control.kp = 0.17;
		break;
	case param_fast:
		moter.max_vel = 500;
		moter.tar_accel = 5.0;
		moter.accel = moter.tar_accel;
		moter.max_ang_vel = 2500;
		control.kp = 0.25;
		break;
	case param_fastest:
		moter.max_vel = 300;
		moter.tar_accel = 5.0;
		moter.accel = moter.tar_accel;
		moter.max_ang_vel = 2500;
		control.kp = 0.3;
		break;
	default:
		break;
	}
}
