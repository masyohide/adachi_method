/*
 * maze.c
 *
 * Data   : 2017.10.26
 * Author : M.Kato
 * Update : 2017.12.21
 */

#include <stdio.h>
#include <stm32f10x.h>
#include "config.h"
#include "iodefine.h"
#include "moter.h"
#include "maze.h"
#include "lcd.h"
#include "global_var.h"
#include "flash.h"


void MAZE_Init_Wall( void ){
	/* <概要>   : 壁情報の初期化
	 * <引数>   : なし
	 * <戻り値> : なし
	 */
	// 初期化
	for( int i = 0; i < 16; i++ ){
		for( int j = 0; j < 16; j++ ){
			maze.wall_north[i][j] = 0;
			maze.wall_east[i][j] = 0;
			maze.wall_south[i][j] = 0;
			maze.wall_west[i][j] = 0;
			maze.is_search[i][j] = false;
		}
	}
	// スタートマスの代入
	maze.wall_east[0][0] = true;
	// 外壁の代入
	for( int i = 0; i <= MAZE_SIZE_X; i++ ){
		maze.wall_north[i][MAZE_SIZE_Y] = true;		// 北壁
		maze.wall_south[i][0] 			= true;		// 南壁
	}
	for( int j = 0; j <= MAZE_SIZE_Y; j++ ){
		maze.wall_east[MAZE_SIZE_X][j]	= true;		// 東壁
		maze.wall_west[0][j]			= true;		// 西壁
	}
}
void MAZE_Create_Step( int mode ){											//ERASE
	/* <概要>   : 歩数マップ作成
	 * <引数>   : 走行の状態( 探索,最短 )
	 * <戻り値> : なし
	 */
	// 初期化
	for( int i = 0; i <= MAZE_SIZE_X; i++ ){
		for( int j = 0; j <= MAZE_SIZE_Y; j++ ){
			if( i == maze.goal_x && j == maze.goal_y ){
				maze.step[i][j] = 0;
			}
			else{
				maze.step[i][j] = MAX_STEP;
			}
		}
	}

	int end_flag = true;	// 終了フラグ
	int stepNum = 0;		// 設定中の歩数
	// 探索
	if( mode == search ){
		while( end_flag == true ){
			end_flag = false;
			for( int i = 0; i <= mazeDef.maze_size_x; i++ ){
				for( int j = 0; j <= mazeDef.maze_size_y; j++ ){
					if( maze.step[i][j] == MAX_STEP ){
						if( j < mazeDef.maze_size_y ){	//北壁
							if( (maze.wall_north[i][j] == false)&&(maze.step[i][j+1] == stepNum) ){
								maze.step[i][j] = stepNum + 1;
								end_flag = true;
							}
						}
						if( i < mazeDef.maze_size_x ){	//東壁
							if( (maze.wall_east[i][j] == false)&&(maze.step[i+1][j] == stepNum) ){
								maze.step[i][j] = stepNum + 1;
								end_flag = true;
							}
						}
						if( j > 0 ) {					//南壁
							if( (maze.wall_south[i][j] == false)&&(maze.step[i][j-1] == stepNum) ){
								maze.step[i][j] = stepNum + 1;
								end_flag = true;
							}
						}
						if( i > 0 ){					//西壁
							if( (maze.wall_west[i][j] == false)&&(maze.step[i-1][j] == stepNum) ){
								maze.step[i][j] = stepNum + 1;
								end_flag = true;
							}
						}
					}
				}
			}
			if (end_flag == true) {
				stepNum++;
			}
		}
	}
}
void MAZE_Out_Wall( int mode ){
	/* <概要>   : 壁情報の出力
	 * <引数>   : 歩数マップ作成時のモード
	 * <戻り値> : なし
	 */
	// ゴール座標の設定
	MAZE_Set_Goal( mazeDef.maze_goal_x, mazeDef.maze_goal_y );
	// 歩数マップの作成
	MAZE_Create_Step( mode );

	printf("\r\nwall data\r\n");
	if( mode == search )	printf("mode search\r\n");
	if( mode == fast_run )	printf("mode fast run\r\n");
	for( int j = MAZE_SIZE_Y; j >= 0; j-- ){
		//横壁
		for( int i = 0; i <= MAZE_SIZE_X; i++ ){
			printf("+");
			if( maze.wall_north[i][j] == true ){
				printf("---");
			}
			else {
				printf("   ");
			}
		}
		printf("+\r\n");

		//縦壁
		for( int i = 0; i <= MAZE_SIZE_X; i++ ){
			if( maze.wall_west[i][j] == true ){
				printf("|");
			}
			else{
				printf(" ");
			}

			//マスの中央に表示するもの
			if( maze.step[i][j] == 0 ){
				printf(" G ");
			}
			else if( (i == MAZE_START_X)&&(j == MAZE_START_Y) ){
				printf(" S ");
			}
			else{
				printf("%3d", maze.step[i][j]);
			}
		}
		if( maze.wall_east[MAZE_SIZE_X][j] == true ){
			printf("|");
		}
		else{
			printf(" ");
		}
		printf("\r\n");

		//外壁(南壁)の描画
		if( j == 0 ){
			for( int i = 0; i <= MAZE_SIZE_X; i++ ){
				printf("+");
				if( maze.wall_south[i][j] == true ){
					printf("---");
				}
				else{
					printf("   ");
				}
			}
			printf("+\r\n");
		}
	}
}
void MAZE_Set_Goal( int x, int y ){
	/* <概要>   : 目標ゴール座標の設定
	 * <引数>   : 目標のゴール座標(x,y)
	 * <戻り値> : なし
	 */
	maze.goal_x = x;
	maze.goal_y = y;
	maze.start_x = position.x;
	maze.start_y = position.y;
}
void MAZE_Set_Wall( int x, int y ){
	/* <概要>   : 壁情報の保存
	 * <引数>   : 現在のマス(x,y)
	 * <戻り値> : なし
	 */
	int n_wall, e_wall, s_wall, w_wall, wall;
	// 探索済みに設定
	maze.is_search[x][y] = true;
	// 方向別に壁の状態を取得
	switch( position.dir ){
	case north:
		n_wall = sen_fr.is_wall | sen_fl.is_wall;
		e_wall = sen_sr.is_wall;
		w_wall = sen_sl.is_wall;
		s_wall = 0;
		break;
	case east:
		e_wall = sen_fr.is_wall | sen_fl.is_wall;
		s_wall = sen_sr.is_wall;
		n_wall = sen_sl.is_wall;
		w_wall = 0;
		break;
	case south:
		s_wall = sen_fr.is_wall | sen_fl.is_wall;
		w_wall = sen_sr.is_wall;
		e_wall = sen_sl.is_wall;
		n_wall = 0;
		break;
	case west:
		w_wall = sen_fr.is_wall | sen_fl.is_wall;
		n_wall = sen_sr.is_wall;
		s_wall = sen_sl.is_wall;
		e_wall = 0;
		break;
	default:
		break;
	}
	// 方向別に保存
	maze.wall_north[x][y] = n_wall;
	maze.wall_east[x][y] = e_wall;
	maze.wall_south[x][y] = s_wall;
	maze.wall_west[x][y] = w_wall;
	// 隣り合う壁の情報を保存
	if( y < MAZE_SIZE_Y-1 ){
		maze.wall_south[x][y+1] = n_wall;
	}
	if( x < MAZE_SIZE_X-1){
		maze.wall_west[x+1][y] = e_wall;
	}
	if(y > 0){
		maze.wall_north[x][y-1] = s_wall;
	}
	if(x > 0){
		maze.wall_east[x-1][y] = w_wall;
	}
}
void MAZE_Update_Position( int dir ){
	/* <概要>   : 現在位置と方角の更新(回転後は前進を必ず行うこと)
	 * <引数>   : 次の動作
	 * <戻り値> : なし
	 */
	// 次の動作と現在位置で場合分け
	switch( dir ){
	case front:
		switch( position.dir ){
		case north:
			position.y++;
			break;
		case east:
			position.x++;
			break;
		case south:
			position.y--;
			break;
		case west:
			position.x--;
			break;
		default:
			break;
		}
		break;
	case left:
		switch( position.dir ){
		case north:
			position.x--;
			position.dir = west;
			break;
		case east:
			position.y++;
			position.dir = north;
			break;
		case south:
			position.x++;
			position.dir = east;
			break;
		case west:
			position.y--;
			position.dir = south;
			break;
		default:
			break;
		}
		break;
	case right:
		switch( position.dir ){
		case north:
			position.x++;
			position.dir = east;
			break;
		case east:
			position.y--;
			position.dir = south;
			break;
		case south:
			position.x--;
			position.dir = west;
			break;
		case west:
			position.y++;
			position.dir = north;
			break;
		default:
			break;
		}
		break;
	case rear:
		switch( position.dir ){
		case north:
			position.y--;
			position.dir = south;
			break;
		case east:
			position.x--;
			position.dir = west;
			break;
		case south:
			position.y++;
			position.dir = north;
			break;
		case west:
			position.x++;
			position.dir = east;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void left_handed_method(void){
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
	}

}

void adachi_method(void)
{
	int t = 500;
	int next_dir;

	MAZE_Init_Wall();

	MAZE_Set_Goal(3,3);

/*	moter.step_l = moter.step_r = 0; */
	MOTER_Straight3(120); delay_ms(t);
	MAZE_Update_Position(front);

	while(!(position.x == maze.goal_x && position.y == maze.goal_y))
	{
		MAZE_Set_Wall(position.x,position.y);
		MAZE_Create_Step(0);
		next_dir = MAZE_Get_Next_Dir_Adachi();

		switch(next_dir){
		case front:
			MOTER_Straight3(180*1);	delay_ms(t);
			MAZE_Update_Position(front);
			break;
		case left:
			MOTER_Straight3(90*1); 	delay_ms(t);
			MOTER_Turn(left,90); 	delay_ms(t);
			MOTER_Straight3(90*1);	delay_ms(t);
			MAZE_Update_Position(left);
			break;
		case right:
			MOTER_Straight3(90*1); 	delay_ms(t);
		    MOTER_Turn(right,90); 	delay_ms(t);
		    MOTER_Straight3(90*1);	delay_ms(t);
			MAZE_Update_Position(right);
			break;
		case rear:
			MOTER_Straight3(90*1); 	delay_ms(t);
			MOTER_Turn(right,180); 	delay_ms(t);
			MOTER_Back(45*1); 		delay_ms(t);
			MOTER_Straight3(120*1); delay_ms(t);
			MAZE_Update_Position(rear);
			break;
		}

/*	LCD_printf1("Goal:X=%d,Y=%d",mazeDef.maze_goal_x,mazeDef.maze_goal_y);
	LCD_printf2("Now:X=%d,Y=%d",position.x,position.y); */

	}

	MOTER_Straight3(90);
}

int MAZE_Get_Next_Dir_Adachi( void ){
 /* <概要>   : 次の動作を決定
  * <引数>   : なし
  * <戻り値> : 次の動作
  */
	int tmp_step = MAX_STEP;  // 歩数
	int tmp_dir = rear;    // 方向
	// 現在の向きに応じて場合分けし、 歩数が少ない方向を判断
	// 迷路外に進むのとゴールがスタートマス以外の場合に(0,0)に進むのを阻止
	switch( position.dir ){
	case north:
		if( maze.step[position.x][position.y+1] < tmp_step ){
			if( position.y < mazeDef.maze_size_y ){
				if( maze.wall_north[position.x][position.y] == false ){//この行大事
					tmp_step = maze.step[position.x][position.y+1];
					tmp_dir = front;
				}
			}
		}
		if( maze.step[position.x-1][position.y] < tmp_step ){
			if( position.x > 0 ){
				if( maze.wall_west[position.x][position.y] == false ){//この行大事
					tmp_step = maze.step[position.x-1][position.y];
					tmp_dir = left;
				}
			}
		}
		if( maze.step[position.x+1][position.y] < tmp_step ){
			if( position.x < mazeDef.maze_size_x ){
				if( maze.wall_east[position.x][position.y] == false ){//この行大事
					tmp_step = maze.step[position.x+1][position.y];
					tmp_dir = right;
				}
			}
		}
		if( tmp_step == MAX_STEP ){//前左右に壁があるとき(上の「この行大事」3行と対応)
			//tmp_step > maze.step[position.x][position.y-1]
			tmp_dir = rear;
		}
		break;
	case east:
		if( maze.step[position.x+1][position.y] < tmp_step ){
			if( position.x < mazeDef.maze_size_x ){
				if( maze.wall_east[position.x][position.y] == false ){
					tmp_step = maze.step[position.x+1][position.y];
					tmp_dir = front;
				}
			}
		}
		if( maze.step[position.x][position.y+1] < tmp_step ){
			if( position.y < mazeDef.maze_size_y ){
				if( maze.wall_north[position.x][position.y] == false ){
					tmp_step = maze.step[position.x][position.y+1];
					tmp_dir = left;
				}
			}
		}
		if( maze.step[position.x][position.y-1] < tmp_step ){
			if( position.y > 0 ){
				if( maze.wall_south[position.x][position.y] == false ){
					tmp_step = maze.step[position.x][position.y-1];
					tmp_dir = right;
				}
			}
		}
		if( tmp_step == MAX_STEP ){
			//tmp_step > maze.step[position.x][position.y-1]
			tmp_dir = rear;
		}
		LCD_Clear(0);
		break;
	case south:
		if( maze.step[position.x][position.y-1] < tmp_step ){
			if( position.y > 0 ){
				if( maze.wall_south[position.x][position.y] == false ){
					// スタートマスに戻るとき
					if( maze.goal_x == mazeDef.maze_goal_x && maze.goal_y == mazeDef.maze_goal_y ){
						if( position.x == 0 && position.y == 1){
						}
						else{
							tmp_step = maze.step[position.x][position.y-1];
							tmp_dir = front;
						}
					}
					else{
						tmp_step = maze.step[position.x][position.y-1];
						tmp_dir = front;
					}
				}
			}
		}
		if( maze.step[position.x+1][position.y] < tmp_step ){
			if( position.x < mazeDef.maze_size_x ){
				if( maze.wall_east[position.x][position.y] == false ){
					tmp_step = maze.step[position.x+1][position.y];
					tmp_dir = left;
				}
			}
		}
		if( maze.step[position.x-1][position.y] < tmp_step ){
			if( position.x > 0 ){
				if( maze.wall_west[position.x][position.y] == false ){
					tmp_step = maze.step[position.x-1][position.y];
					tmp_dir = right;
				}
			}
		}
		if( tmp_step == MAX_STEP ){
			//tmp_step > maze.step[position.x][position.y-1]
			tmp_dir = rear;
		}
		break;
	case west:
		if( maze.step[position.x-1][position.y] < tmp_step ){
			if( position.x > 0 ){
				if( maze.wall_west[position.x][position.y] == false ){
					tmp_step = maze.step[position.x-1][position.y];
					tmp_dir = front;
				}
			}
		}
		if( maze.step[position.x][position.y-1] < tmp_step ){
			if( position.y > 0 ){
				if( maze.wall_south[position.x][position.y] == false ){
					if( maze.goal_x == mazeDef.maze_goal_x && maze.goal_y == mazeDef.maze_goal_y ){
						if( position.x == 0 && position.y == 1){
						}
						else{
							tmp_step = maze.step[position.x][position.y-1];
							tmp_dir = left;
						}
					}
					else{
						tmp_step = maze.step[position.x][position.y-1];
						tmp_dir = left;
					}
				}
			}
		}
		if( maze.step[position.x][position.y+1] < tmp_step ){
			if( position.y < mazeDef.maze_size_y ){
				if( maze.wall_north[position.x][position.y] == false ){
					tmp_step = maze.step[position.x][position.y+1];
					tmp_dir = right;
				}
			}
		}
		if( tmp_step == MAX_STEP ){
			//tmp_step > maze.step[position.x][position.y-1]
			tmp_dir = rear;
		}
		break;
	default:
		break;
	}
	return tmp_dir;
}

void FREE_Run(void){

}
