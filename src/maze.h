#ifndef SRC_MAZE_H_
#define SRC_MAZE_H_


void MAZE_Init_Wall( void );					// 壁情報の初期化
void MAZE_Create_Step( int mode );			// 歩数マップ作成
void MAZE_Out_Wall( int mode );				// 壁情報の出力
void MAZE_Set_Goal( int x, int y );			// 目標ゴール座標の設定
void MAZE_Set_Wall( int x, int y );			// 壁情報の保存
void MAZE_Update_Position( int dir );		// 現在位置と方角の更新
void FREE_Run(void);							// 好きな走行方法
void left_handed_method();
void adachi_method();
int MAZE_Get_Next_Dir_Adachi();


#endif /* SRC_MAZE_H_ */
