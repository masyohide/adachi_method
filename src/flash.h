#ifdef __cplusplus
extern "C" {
#endif

#ifndef SRC_FLASH_H_
#define SRC_FLASH_H_

#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif


//フラッシュ用アドレスの設定
#define FLASH_BASE_ADDR 	((uint32_t)0x08003000)			// ページ6からページ255まで
#define START_PAGE      				60					// ページ10から

// アドレス・ページ設定
// 迷路定義用
#define FLASH_PAGE_DEF_MAZE							0		// 迷路定義用ページ
#define FLASH_ADDR_DEF_MAZE_TIME					0		// 書き込んだ回数
#define FLASH_ADDR_DEF_MAZE_SIZE_X					1		// 迷路サイズ(X)
#define FLASH_ADDR_DEF_MAZE_SIZE_Y					2		// 迷路サイズ(Y)
#define FLASH_ADDR_DEF_MAZE_GOAL_X					3		// ゴールマス(X)
#define FLASH_ADDR_DEF_MAZE_GOAL_Y					4		// ゴールマス(Y)
// センサ定義用
#define FLASH_PAGE_DEF_SENSOR						1		// センサ定義用ページ
#define FLASH_ADDR_DEF_SENSOR_TIME 					0		// 書き込んだ回数
#define FLASH_ADDR_DEF_SENSOR_SR_THRESHOLD			1		// 右横壁のしきい値
#define FLASH_ADDR_DEF_SENSOR_FR_THRESHOLD			2		// 右前壁のしきい値
#define FLASH_ADDR_DEF_SENSOR_FL_THRESHOLD			3		// 左前壁のしきい値
#define FLASH_ADDR_DEF_SENSOR_SL_THRESHOLD			4		// 左横壁のしきい値
#define FLASH_ADDR_DEF_SENSOR_SR_CENTER				5		// 迷路中央に置いた時の右横壁の値
#define FLASH_ADDR_DEF_SENSOR_SL_CENTER				6		// 迷路中央に置いた時の左横壁の値
#define FLASH_ADDR_DEF_SENSOR_FR_CENTER				7		// 迷路中央に置いた時の前横壁の値
#define FLASH_ADDR_DEF_SENSOR_FL_CENTER				8		// 迷路中央に置いた時の前横壁の値
// 迷路情報
#define FLASH_PAGE_MAZE_WALL						2		// 迷路情報用ページ
#define FLASH_ADDR_MAZE_WALL_TIME 					0		// 書き込んだ回数
#define FLASH_ADDR_MAZE_WALL_NORTH					1		// 北壁用
#define FLASH_ADDR_MAZE_WALL_EAST					17		// 東壁用
#define FLASH_ADDR_MAZE_WALL_SOUTH					33		// 南壁用
#define FLASH_ADDR_MAZE_WALL_WEST					49		// 西壁用
#define FLASH_ADDR_MAZE_SEARCH						65		// 探索済み


// FLASH
void FLASH_Erase( int page );								// FLASH消去
void FLASH_Write( int page, int address, int word );		// FLASH書き込み
int FLASH_Read( int page, int address );					// FLASH読み込み
void FLASH_Write_Def_Maze( void );						// 迷路定義書き込み関数
void FLASH_Read_Def_Maze( void );							// 迷路定義読み込み関数
void FLASH_Write_Def_Sensor( void );						// 壁センサしきい値書き込み関数
void FLASH_Read_Def_Sensor( void );						// 壁センサしきい値読み込み関数
void FLASH_Write_Maze_Wall( void );						// 壁情報書き込み関数
void FLASH_Read_Maze_Wall( void );						// 壁情報読み込み関数


#endif /* SRC_FLASH_H_ */
#ifdef __cplusplus
}
#endif
