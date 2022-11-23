/*
 * mode.c
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
#include "moter.h"
#include "mode.h"
#include "maze.h"


// モード選択
int MODE_Select( void ){
	/* <概要>   : モード選択関数
	 * <引数>   : なし
	 * <戻り値> : 選択したモード番号(0～15)
	 */
	int mode = 0;

	LED_Controll( mode );
	LCD_Clear( 0 );
	LCD_printf1( "mode select", mode );
	delay_ms(1000);

	while(1){
		// mode表示
		LED_Controll( mode );
		LCD_Clear( 0 );
		LCD_printf1( "mode=%d", mode );

		if( mode == 0 )  LCD_printf2( "mouse" );
		if( mode == 1 )  LCD_printf2( "circuit" );
		if( mode == 2 )  LCD_printf2( "sensor check" );
		if( mode == 3 )  LCD_printf2( "sensor setting" );
		if( mode == 4 )  LCD_printf2( "maze setting" );
		if( mode == 5 )  LCD_printf2( "check diameter" );
		if( mode == 6 )  LCD_printf2( "check tread" );
		if( mode == 7 )  LCD_printf2( "check kp" );
		if( mode == 8 )  LCD_printf2( "my sample" );
		if( mode == 9 )  LCD_printf2( "adachi_method" );
		if( mode == 10 ) LCD_printf2( "---" );
		if( mode == 11 ) LCD_printf2( "---" );
		if( mode == 12 ) LCD_printf2( "---" );
		if( mode == 13 ) LCD_printf2( "---" );
		if( mode == 14 ) LCD_printf2( "---" );
		if( mode == 15 ) LCD_printf2( "---" );

		// SWを押すまで待つ
		while( ( SW1 == SW_OFF )&&( SW2 == SW_OFF )&&( SW3 == SW_OFF ) );

		// 押したSWによってmode変更
		if( SW1 == SW_ON ){
			if( mode < 15 )	mode++;
		}
		if( SW2 == SW_ON ){
			if( mode > 0 )	mode--;
		}
		if( SW3 == SW_ON )	break;

		// チャタリング防止
		delay_ms(250);
	}
	delay_ms(500);
	return mode;
}
void MODE_Run( int mode ){
	/* <概要>   : モード実行関数
	 * <引数>   : モード番号
	 * <戻り値> : なし
	 */
	if( mode == 0 ) MODE_Mouse();				// mouse
	if( mode == 1 ) MODE_Circuit_Select();		// circuit
	if( mode == 2 ) MODE_Sensor_Check();		// sensor check
	if( mode == 3 ) MODE_Sensor_Setting();		// sensor setting
	if( mode == 4 ) MODE_Maze_Setting();		// maze setting
	if( mode == 5 ) MODE_Check_DIAMETER();		// check diameter
	if( mode == 6 ) MODE_Check_TREAD();			// check tread
	if( mode == 7 ) MODE_Check_kp();			// check kp
	if( mode == 8 ) MODE_My_Sample();			// run my sample plogram
	if( mode == 9 ) adachi_method();			// ---
	if( mode == 10 ){}							// ---
	if( mode == 11 ){}							// ---
	if( mode == 12 ){}							// ---
	if( mode == 13 ){}							// ---
	if( mode == 14 ){}							// ---
	if( mode == 15 ){}							// ---
}
void MODE_Mouse( void ){								// mode0
	/* <概要>   : 迷路走行
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	//int run_algorithm;
	//int run_mode;
	int mode = 0;
	int loop_flag = true;
	FLASH_Read_Maze_Wall();
	while( loop_flag == true ){
		// モード選択
		mode = 0;
		while(1){
			LCD_Clear(0);
			LCD_printf1( "mode=%d", mode );

			if( mode == 0 )	LCD_printf2( "run mode" );
			if( mode == 1 )	LCD_printf2( "restore wall" );
			if( mode == 2 )	LCD_printf2( "send wall data" );
			if( mode == 3 ) LCD_printf2( "exit mouse mode" );

			// SWを押すまで待つ
			while( ( SW1 == SW_OFF )&&( SW2 == SW_OFF )&&( SW3 == SW_OFF ) );

			if( SW1 == SW_ON ){
				if( mode < 3 )	mode++;
			}
			if( SW2 == SW_ON ){
				if( mode > 0 )	mode--;
			}
			if( SW3 == SW_ON ) break;

			delay_ms( 250 );
		}

		switch ( mode ){
		case 0:		// 走行モード
			run_mode.run_mode = adatchi_method;
			MODE_Mouse_Run();
			break;
		case 1:		// 壁情報復元
			FLASH_Read_Maze_Wall();
			break;
		case 2:		// 壁情報送信
			MAZE_Out_Wall( fast_run );
			break;
		case 3:		// マウスモード終了
			loop_flag = false;
			break;
		default:
			break;
		}
		delay_ms( 250 );
	}
}
void MODE_Circuit( void ){								// mode1
	/* <概要>   : サーキットモード
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
}
void MODE_Sensor_Check( void ){						// mode2
	/* <概要>   : センサチェック関数
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	int mode=0;
	LCD_Clear(0);
	int LED_pattern;
	while(1){
		WALL_PT_Get( WALL_PT1_ADC_CH );
		WALL_PT_Get( WALL_PT2_ADC_CH );
		WALL_PT_Get( WALL_PT3_ADC_CH );
		WALL_PT_Get( WALL_PT4_ADC_CH );

		LCD_Clear(0);
		if( mode == 0 )	LCD_printf1( "FL=%4d  FR=%4d", sen_fl.value, sen_fr.value );
		if( mode == 1 )	LCD_printf1( "sr_thr  =%4d", senDef.sr_threshold );
		if( mode == 2 )	LCD_printf1( "fr_thr  =%4d", senDef.fr_threshold );
		if( mode == 3 )	LCD_printf1( "fl_thr  =%4d", senDef.fl_threshold );
		if( mode == 4 )	LCD_printf1( "sl_thr  =%4d", senDef.sl_threshold );
		if( mode == 5 )	LCD_printf1( "sr_cen  =%4d", senDef.sr_center );
		if( mode == 6 )	LCD_printf1( "sl_cen  =%4d", senDef.sl_center );
		if( mode == 7 )	LCD_printf1( "fr_cen  =%4d", senDef.fr_center );
		if( mode == 8 )	LCD_printf1( "fl_cen  =%4d", senDef.fl_center );

		if( mode == 0 )	LCD_printf2( "SL=%4d  SR=%4d", sen_sl.value, sen_sr.value );
		if( mode == 1 )	LCD_printf2( "sr_value=%4d", sen_sr.value );
		if( mode == 2 )	LCD_printf2( "fr_value=%4d", sen_fr.value );
		if( mode == 3 )	LCD_printf2( "fl_value=%4d", sen_fl.value );
		if( mode == 4 )	LCD_printf2( "sl_value=%4d", sen_sl.value );
		if( mode == 5 )	LCD_printf2( "sr_value=%4d", sen_sr.value );
		if( mode == 6 )	LCD_printf2( "sl_value=%4d", sen_sl.value );
		if( mode == 7 )	LCD_printf2( "fr_value=%4d", sen_fr.value );
		if( mode == 8 )	LCD_printf2( "fl_value=%4d", sen_fl.value );

		LED_pattern = ( ( sen_fl.is_wall << 3 ) | ( sen_sl.is_wall << 2 ) |
						( sen_sr.is_wall << 1 ) | ( sen_fr.is_wall ) );
		LED_Controll( LED_pattern );

		if( SW1 == SW_ON ){
			if( mode < 8 )	mode++;
		}
		if( SW2 == SW_ON ){
			if( mode > 0 )	mode--;
		}
		if( SW3 == SW_ON ) break;

		delay_ms( 250 );
	}
}
void MODE_Sensor_Setting( void ){						// mode3
	/* <概要>   : センサ値調整関数
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	int temp_sensor[8];
	int mode = 0;
	while(1){
		// AD変換値を取得
		WALL_PT_Get( WALL_PT1_ADC_CH );
		WALL_PT_Get( WALL_PT2_ADC_CH );
		WALL_PT_Get( WALL_PT3_ADC_CH );
		WALL_PT_Get( WALL_PT4_ADC_CH );

		LCD_Clear( 0 );
		if( mode == 0 ) {LCD_printf1( "put left end" ); 	temp_sensor[0] = sen_sr.value;}
		if( mode == 1 ) {LCD_printf1( "put right end" );	temp_sensor[1] = sen_sl.value;}
		if( mode == 2 ) {LCD_printf1( "put right front" );	temp_sensor[2] = sen_fr.value;}
		if( mode == 3 ) {LCD_printf1( "put left front" );	temp_sensor[3] = sen_fl.value;}
		if( mode == 4 ) {LCD_printf1( "put center" );		temp_sensor[4] = sen_sr.value;}
		if( mode == 5 ) {LCD_printf1( "put center" );		temp_sensor[5] = sen_sl.value;}
		if( mode == 6 ) {LCD_printf1( "put center" );		temp_sensor[6] = sen_fr.value;}
		if( mode == 7 ) {LCD_printf1( "put center" );		temp_sensor[7] = sen_fl.value;}

		if( mode == 0 ) LCD_printf2( "sr_thr=%4d", temp_sensor[0] );
		if( mode == 1 ) LCD_printf2( "sl_thr=%4d", temp_sensor[1] );
		if( mode == 2 ) LCD_printf2( "fr_thr=%4d", temp_sensor[2] );
		if( mode == 3 ) LCD_printf2( "fl_thr=%4d", temp_sensor[3] );
		if( mode == 4 ) LCD_printf2( "sr_center=%4d", temp_sensor[4] );
		if( mode == 5 ) LCD_printf2( "sl_center=%4d", temp_sensor[5] );
		if( mode == 6 ) LCD_printf2( "fr_center=%4d", temp_sensor[6] );
		if( mode == 7 ) LCD_printf2( "fl_center=%4d", temp_sensor[7] );

		// 押したSWによってmode変更
		if( SW2 == SW_ON ){
			if( mode > 0 )	mode--;
		}
		if( SW3 == SW_ON ){
			if( mode != 7 ) mode++;
			else			break;
		}

		// チャタリング防止
		delay_ms(250);
	}
	senDef.sr_threshold = temp_sensor[0];
	senDef.sl_threshold = temp_sensor[1];
	senDef.fr_threshold = temp_sensor[2];
	senDef.fl_threshold = temp_sensor[3];
	senDef.sr_center = temp_sensor[4];
	senDef.sl_center = temp_sensor[5];
	senDef.fr_center = temp_sensor[6];
	senDef.fl_center = temp_sensor[7];
	FLASH_Write_Def_Sensor();
	FLASH_Read_Def_Sensor();
}
void MODE_Maze_Setting(void){							// mode4
	/* <概要>   : 迷路設定関数
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	int temp_maze[4] = { MAZE_SIZE_X, MAZE_SIZE_Y, MAZE_GOAL_X, MAZE_GOAL_Y };
	int mode = 0;
	LCD_Clear( 0 );
	LCD_printf1( "set maze", temp_maze[0], temp_maze[1] );
	while(1){
		if( mode == 2 ){
			if( temp_maze[mode] > temp_maze[0] ) temp_maze[mode] = temp_maze[0];
		}
		else if( mode == 3 ){
			if( temp_maze[mode] > temp_maze[1] ) temp_maze[mode] = temp_maze[1];
		}

		if( mode == 0 ) LCD_printf2( "maze size X=%d", temp_maze[0] );
		if( mode == 1 ) LCD_printf2( "maze size Y=%d", temp_maze[1] );
		if( mode == 2 ) LCD_printf2( "maze goal X=%d", temp_maze[2] );
		if( mode == 3 ) LCD_printf2( "maze goal Y=%d", temp_maze[3] );

		// SWを押すまで待つ
		while( ( SW1 == SW_OFF )&&( SW2 == SW_OFF )&&( SW3 == SW_OFF ) );

		// 押したSWによってmode変更
		if( SW1 == SW_ON ){
			if( mode == 2 ){
				if( temp_maze[mode] < temp_maze[0] ) temp_maze[mode]++;
			}
			else if( mode == 3 ){
				if( temp_maze[mode] < temp_maze[1] ) temp_maze[mode]++;
			}
			else{
				if( temp_maze[mode] < 15 )	temp_maze[mode]++;
			}
		}
		if( SW2 == SW_ON ){
			if( temp_maze[mode] > 0 )	temp_maze[mode]--;
		}
		if( SW3 == SW_ON ){
			if( mode != 3 ) mode++;
			else			break;
		}

		// チャタリング防止
		delay_ms(250);
	}
	mazeDef.maze_size_x = temp_maze[0];
	mazeDef.maze_size_y = temp_maze[1];
	mazeDef.maze_goal_x = temp_maze[2];
	mazeDef.maze_goal_y = temp_maze[3];
	FLASH_Write_Def_Maze();
}
void MODE_Check_DIAMETER( void ){						// mode5
	/* <概要>   : タイヤ直径確認関数(5マス直進)
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	control.enable = 0;							// 壁制御無効
	control.front_enable = 0;					// 前壁制御無効
	moter.step_l = moter.step_r = 0;			// ステップ数の初期化
	MOTER_Straight(180*5);					// モータ前進

}
void MODE_Check_TREAD( void ){							// mode6
	/* <概要>   : トレッド幅確認関数(5回転)
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	//mode.run_mode = param_adjust;		//add
	//MODE_Mouse_Run();						//add
	MOTER_Turn( left, 360 * 5 );
}
void MODE_Check_kp( void ){							// mode7
	/* <概要>   : ゲインkp確認関数(5マス直進)
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	run_mode.run_mode = param_adjust;
	MODE_Mouse_Run();
	MOTER_Straight( 5*ONE_BLOCK );
}

void MODE_Mouse_Run( void ){
	int mode = 0;
	delay_ms( 250 );
	// モードの設定
	while(1){
		if( run_mode.run_mode == param_adjust ) break;
		LCD_Clear(0);
		LCD_printf1( "set algorithm" );

		if( mode == adatchi_method )	LCD_printf2( "search" );
		if( mode == fast_method )		LCD_printf2( "fast run" );

		// SWを押すまで待つ
		while( ( SW1 == SW_OFF )&&( SW2 == SW_OFF )&&( SW3 == SW_OFF ) );

		if( SW1 == SW_ON ){
			if( mode < 1 )	mode++;
		}
		if( SW2 == SW_ON ){
			if( mode > 0 )	mode--;
		}
		if( SW3 == SW_ON ){
			run_mode.run_mode = mode;
			break;
		}
		delay_ms( 250 );
	}
	delay_ms( 250 );
	// パラメータの設定
	mode = 0;
	if( run_mode.run_mode == fast_method || run_mode.run_mode == param_adjust ){
		while(1){
			LCD_Clear(0);
			LCD_printf1( "set parameter" );

			if( mode == param_search )	LCD_printf2( "search" );
			if( mode == param_slow )	LCD_printf2( "slow" );
			if( mode == param_medium )	LCD_printf2( "medium" );
			if( mode == param_fast )	LCD_printf2( "fast" );
			if( mode == param_fastest )	LCD_printf2( "fastest");

			// SWを押すまで待つ
			while( ( SW1 == SW_OFF )&&( SW2 == SW_OFF )&&( SW3 == SW_OFF ) );
			if( SW1 == SW_ON ){
				if( mode < 4 )	mode++;
			}
			if( SW2 == SW_ON ){
				if( mode > 0 )	mode--;
			}
			if( SW3 == SW_ON ){
				moter.parameter = mode;
				break;
			}
			delay_ms( 250 );
		}
		delay_ms( 250 );
	}
	switch( run_mode.run_mode ){
	case adatchi_method:
		FREE_Run();
		break;
	case param_adjust:
		break;
	default:
		break;
	}
}

void MODE_My_Sample(){

	int i;

	moter.step_l = moter.step_r = 0;

	for(i=0;i<5;i++){
		MOTER_Straight(180);
	}

	MOTER_DRIVER_PWM_DISABLE();
	/*
	int count = 0;

	LCD_printf1("Select goal X ");
	while( SW3 == SW_OFF )
	{
		if( SW1 == SW_ON ){
			if( count < 16 ){
				count++;
				while(SW1 == SW_ON);
			}
		}
		if( SW2 == SW_ON ){
			if( count >  0 ){
				count--;
				while(SW2 == SW_ON);
			}
		}
		if( SW3 == SW_ON )	break;

		LCD_printf2("X = %d",count);
		delay_ms(100);

	}
	mazeDef.maze_goal_x = count;
	delay_ms(250);

	count = 0;
	LCD_printf1("Select goal Y ");
	while( SW3 == SW_OFF )
	{

		if( SW1 == SW_ON ){
			if( count < 16 ){
				count++;
				while(SW1 == SW_ON);
			}
		}
		if( SW2 == SW_ON ){
			if( count >  0 ){
				count--;
				while(SW2 == SW_ON);
			}
		}
		if( SW3 == SW_ON )	break;

		LCD_printf2("Y = %d",count);
		delay_ms(100);

	}
	mazeDef.maze_goal_y = count;
	delay_ms(250);

	LCD_printf1("Goal:X=%d,Y=%d",mazeDef.maze_goal_x,mazeDef.maze_goal_y);
	LCD_printf2("Now:X=%d,Y=%d",position.x,position.y);

	delay_ms(2000);

	MOTER_Straight(120);
	MAZE_Update_Position(front);
	delay_ms(1000);

	while(!(position.x == mazeDef.maze_goal_x && position.y == mazeDef.maze_goal_y))
	{
		if(sen_sl.is_wall == 0)
		{
			MOTER_Straight(90);		delay_ms(1000);
			MOTER_Turn(left,90);	delay_ms(1000);
			MOTER_Straight(90);		delay_ms(1000);
			MAZE_Update_Position(left);
		}
		else if(sen_fl.is_wall == 0 || sen_fr.is_wall == 0){
			MOTER_Straight(180);	delay_ms(1000);
			MAZE_Update_Position(front);
		}
		else if(sen_sr.is_wall == 0){
			MOTER_Straight(90);		delay_ms(1000);
			MOTER_Turn(right,90);	delay_ms(1000);
			MOTER_Straight(90);		delay_ms(1000);
			MAZE_Update_Position(right);
		}
		else
		{
			MOTER_Straight(90);		delay_ms(1000);
			MOTER_Turn(right,180);	delay_ms(1000);
			MOTER_Back(45);			delay_ms(1000);
			MOTER_Straight(120);	delay_ms(1000);
			MAZE_Update_Position(rear);
		}

		LCD_printf2("Now:X=%d,Y=%d",position.x,position.y);

	}

	MOTER_Straight(90);
	delay_ms(1000); */
}

void MODE_Circuit_Select(){
	int mode=0;

	LCD_Clear( 0 );
	LCD_printf1( "circuit select", mode );
	delay_ms(1000);

	while(1){
			LCD_Clear( 0 );
			LCD_printf1( "mode=%d", mode );

			if( mode == 0 )  LCD_printf2( "left handed method" );
			if( mode == 1 )  LCD_printf2( "right handed method" );
			if( mode == 2 )  LCD_printf2( "adachi method" );
			if( mode == 3 )  LCD_printf2( "FREE_RUN" );
						// SWを押すまで待つ
			while( ( SW1 == SW_OFF )&&( SW2 == SW_OFF )&&( SW3 == SW_OFF ) );

			// 押したSWによってmode変更
			if( SW1 == SW_ON ){
				if( mode < 3 )	mode++;
			}
			if( SW2 == SW_ON ){
				if( mode > 0 )	mode--;
			}
			if( SW3 == SW_ON )	break;

			// チャタリング防止
			delay_ms(250);
		}
		delay_ms(500);

		switch(mode){
		case 0:
			left_handed_method();
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		}

}

