#ifndef SRC_PARAMETER_H_
#define SRC_PARAMETER_H_


#define PI (3.141592653589793)					// 円周率
#define ABS(IN) ((IN) < 0 ? - (IN) : (IN))		// 絶対値を返すマクロ

// 機体
#define TIRE_DIAMETER		(48.00 + 4.0)		    // タイヤの直径//smaller->increase
#define TIRE_CIRCUIT		(PI*TIRE_DIAMETER)	// タイヤの円周
#define TREAD_WIDTH			(79.55 + 4.5)		    // トレッド幅 (右側：増やす)bigger->increase
#define TREAD_CIRCUIT		(TREAD_WIDTH*PI/4)	// 超信地旋回の時にタイヤが動く距離
#define MIN_TARGET_VEL			3000			// 最低速度(ステップ数ベース)2000

// 迷路
#define MAZE_SIZE_X				15				// 迷路サイズ(X)
#define MAZE_SIZE_Y				15				// 迷路サイズ(Y)
#define MAZE_START_X			0				// スタートマス(X)
#define MAZE_START_Y			0				// スタートマス(Y)
#define MAZE_GOAL_X				1				// ゴールマス(X)
#define MAZE_GOAL_Y				0				// ゴールマス(Y)
#define ONE_BLOCK				180.0			// 1区画の長さ[mm]
#define HALF_BLOCK				90.0			// 半区画の長さ[mm]
#define ADJ_BACK				120.0			// 位置決めの時に後進する長さ[mm]
#define ADJ_FRONT				100.0			// 機体の一番後ろからマスの境界までの長さ[mm]
#define MAX_STEP				255				// 歩数マップの最大値

// センサ
#define SR_WALL_THRESHOLD		200				// 右横壁のしきい値
#define FR_WALL_THRESHOLD		125				// 右前壁のしきい値
#define FL_WALL_THRESHOLD		90				// 左前壁のしきい値
#define SL_WALL_THRESHOLD		375				// 左横壁のしきい値
#define SR_WALL_CENTER			450				// 迷路中央に置いた時の右横壁の値
#define FR_WALL_CENTER			1035			// 迷路中央に置いた時の右前壁の値
#define FL_WALL_CENTER			1765			// 迷路中央に置いた時の右左壁の値
#define SL_WALL_CENTER			1025			// 迷路中央に置いた時の左横壁の値
#define ADC_WAIT_TIME			50				// LED発光してからAD変換開始までの時間[us]
#define WALL_DIFF_THRESHOLD		50				// 壁センサ変化量のしきい値(吸い込み防止)


#endif /* SRC_PARAMETER_H_ */
