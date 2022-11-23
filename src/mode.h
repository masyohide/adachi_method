/*
 * mode.h
 *
 *  Created on: 2017/11/08
 *      Author: x240
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef SRC_MODE_H_
#define SRC_MODE_H_

// モード選択
int MODE_Select( void );						// モード選択
void MODE_Run( int mode );						// モード実行
void MODE_Mouse( void );						// 迷路走行			mode0
void MODE_Circuit( void );						// サーキット		mode1
void MODE_Sensor_Check( void );				// センサチェック	mode2
void MODE_Sensor_Setting( void );				// センサ値調整		mode3
void MODE_Maze_Setting(void);					// 迷路設定			mode4
void MODE_Check_DIAMETER( void );				// タイヤ直径確認 	mode5
void MODE_Check_TREAD( void );					// トレッド幅確認	mode6
void MODE_Check_kp( void );					// ゲインkp確認		mode7
void MODE_My_Sample(void);						//自作プロ実行		mode8
void MODE_Circuit_Select();					//左手法、右手法などを選ぶ mode9

void MODE_Mouse_Run( void );

#endif /* SRC_MODE_H_ */

#ifdef __cplusplus
}
#endif
