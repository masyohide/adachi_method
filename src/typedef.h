#ifndef SRC_TYPEDEF_H_
#define SRC_TYPEDEF_H_


typedef enum{									//真偽値を取り扱う列挙型
	false = 0,									//偽
	true = 1,									//真
}t_bool;

typedef enum{									// 方角の定義
	north = 4,									// 北
	east = 3,									// 東
	south = 2,									// 南
	west = 1,									// 西
}t_direction;

typedef enum{									// 自分から見た方向の定義
	front = 0,									// 前
	left = 1,									// 左
	right = 2,									// 右
	rear = 3,									// 後
	unknown,									// 方向不明
}t_local_dir;

typedef enum{									// 走行の状態
	search = 0,									// 探索走行
	fast_run = 1,								// 最短走行
}t_stepMode;

typedef enum{									// アルゴリズムの定義
	adatchi_method = 0,							// 足立法
	fast_method = 1,							// 最短走行
	param_adjust = 2,							// 調整用
}t_runMode;

typedef enum{									// パラメータの定義
	param_search = 0,							// 探索用(加速なし)
	param_slow = 1,								// 最短走行用(加速あり)
	param_medium = 2,							// 最短走行用(加速あり)
	param_fast = 3,								// 最短走行用(加速あり)
	param_fastest = 4,							// 最短走行用(加速あり)
}t_parameter;

typedef struct{								// 迷路定義用構造体
	int maze_size_x;							// 迷路サイズ( X )
	int maze_size_y;							// 迷路サイズ( Y )
	int maze_goal_x;							// ゴールマス( X )
	int maze_goal_y;							// ゴールマス( Y )
}t_mazeDef;

typedef struct{								// 壁センサしきい値用構造体
	int sr_threshold;							// 右横壁のしきい値
	int fr_threshold;							// 右前壁のしきい値
	int fl_threshold;							// 左前壁のしきい値
	int sl_threshold;							// 左横壁のしきい値
	int sr_center;								// 迷路中央に置いた時の右横壁の値
	int sl_center;								// 迷路中央に置いた時の左横壁の値
	int fr_center;								// 迷路中央に置いた時の右前壁の値
	int fl_center;								// 迷路中央に置いた時の左前壁の値
}t_sensorDef;

typedef struct{								// 壁センサ用構造体
	int value;									// AD変換値
	int p_value;								// 1つ前のAD変換値
	float error;								// value - center
	int is_wall;								// 壁の有無( 0:無 1:有 )
	int is_control;								// 壁制御の有無
	int diff_value;								// AD変換値の変化量( ABS() )
}t_sensor;

typedef struct{								// モータ変数用構造体
	int parameter;								// パラメータの状態
	float vel;									// 現在速度
	float max_vel;								// 最高速度
	float accel;								// 現在加速度
	float tar_accel;							// 目標加速度
	int max_ang_vel;							// 最高角速度
	int step_r;									// 右モータステップ数
	int step_l;									// 左モータステップ数
	int is_accel_start;							// 加速処理の有無
}t_moter;

typedef struct{								// モータ制御用構造体
	float control;								// 最終的な制御量
	float error;								// 偏差
	float kp;									// 比例制御定数
	int enable;									// 横壁制御のon/off
	int front_enable;							// 前壁壁制御のon/off
}t_control;

typedef struct{								// マウスの情報
	int x;										// 現在位置( X )
	int y;										// 現在位置( Y )
	int dir;									// 向いている方向
}t_position;

typedef struct{								// 迷路情報
	int goal_x;									// 目標のゴール座標
	int goal_y;									// 目標のゴール座標
	int start_x;								// スタートマス
	int start_y;								// スタートマス
	int step[16][16];							// 歩数マップ
	int wall_north[16][16];						// 壁情報
	int wall_east[16][16];						// 壁情報
	int wall_south[16][16];						// 壁情報
	int wall_west[16][16];						// 壁情報
	int is_search[16][16];						// 探索済みフラグ
	int step_mode;								// 歩数マップ作成時の状態(探索用 最短走行用)
}t_maze;

typedef struct{								// 時間情報
	volatile float start_time;					// 電源ONからの時間[s]
}t_time;

typedef struct{								// モード情報
	int run_mode;								// 現在のモード
}t_run_mode;


#endif /* SRC_TYPEDEF_H_ */
