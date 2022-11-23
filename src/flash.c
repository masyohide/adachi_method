/*
 * flash.c
 *
 * Data   : 2017.10.26
 * Author : M.Kato
 * Update : 2017.12.21
 */
#include <stdio.h>
#include <stm32f10x.h>
#include "global_var.h"
#include "flash.h"


// ステータス用フラグ
typedef enum { FAILED = 0, PASSED = !FAILED } TestStatus;
volatile FLASH_Status FLASHStatus;
volatile TestStatus MemoryProgramStatus;

// FLASH
void FLASH_Erase( int page ){
	/* <概要>	FLASH消去
	 * <引数>	StartPageから数えた消去するページ
	 * <戻り値>	なし
	 */
	//フラッシュのアンロック
	FLASH_Unlock();

	//フラグのクリア
	FLASHStatus = FLASH_COMPLETE;
	MemoryProgramStatus = PASSED;
	FLASH_ClearFlag( FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR );

	//フラッシュのクリア
	FLASHStatus = FLASH_ErasePage( FLASH_BASE_ADDR + ( ( START_PAGE + page ) * FLASH_PAGE_SIZE ) );
}
void FLASH_Write( int page, int address, int word ){
	/* <概要>	FLASH書き込み
	 * <引数>	StartPageから数えた書き込むページ, 書き込むアドレス(0～50), 書き込む値(int 16_t)
	 * <戻り値>	なし
	 */
	uint32_t EraseCounter = 0x00, Address = 0x00;

	//フラッシュのアンロック
	FLASH_Unlock();

	//書き込むアドレス
	Address = FLASH_BASE_ADDR + ( ( START_PAGE + page ) * FLASH_PAGE_SIZE ) + address * 16;

	//フラッシュの書き込み
	FLASHStatus = FLASH_ProgramWord( Address     , ( word >> 12 ) & 0xffff );
	FLASHStatus = FLASH_ProgramWord( Address +  4, ( word >>  8 ) & 0xffff );
	FLASHStatus = FLASH_ProgramWord( Address +  8, ( word >>  4 ) & 0xffff );
	FLASHStatus = FLASH_ProgramWord( Address + 12, ( word       ) & 0xf );
}
int FLASH_Read( int page , int address ){
	/* <概要>	FLASH読み込み
	 * <引数>	StartPageから数えた読み込むページ, 読み込むアドレス(0～50)
	 * <戻り値> 読み込んだ値(int 16_t)
	 */
	uint32_t Address = 0x00;

	int READ_Word;
	int READ_Word_Temp[4];

	//読み取るアドレス
	Address = FLASH_BASE_ADDR + ( ( START_PAGE + page ) * FLASH_PAGE_SIZE ) + address*16;

	//フラッシュの読み取り
	READ_Word_Temp[0] = ( (*(__IO uint32_t*)( Address     ) & 0x000f ) << 12 );
	READ_Word_Temp[1] = ( (*(__IO uint32_t*)( Address + 4 ) & 0x000f ) <<  8 );
	READ_Word_Temp[2] = ( (*(__IO uint32_t*)( Address + 8 ) & 0x000f ) <<  4 );
	READ_Word_Temp[3] = ( (*(__IO uint32_t*)( Address +12 ) & 0x000f )       );

	READ_Word = READ_Word_Temp[0] | READ_Word_Temp[1] | READ_Word_Temp[2] | READ_Word_Temp[3] ;

	return READ_Word;
}

void FLASH_Write_Def_Maze( void ){
	/* <概要>	迷路定義書き込み関数
	 * <引数>	なし
	 * <戻り値>	なし
	 */
	// 書き込んだ回数の取得
	int write_time = FLASH_Read( FLASH_PAGE_DEF_MAZE, FLASH_ADDR_DEF_MAZE_TIME );

	// ページの消去
	FLASH_Erase( FLASH_PAGE_DEF_MAZE );

	int addr=0;
	FLASH_Write( FLASH_PAGE_DEF_MAZE, FLASH_ADDR_DEF_MAZE_TIME, write_time+1 );
	FLASH_Write( FLASH_PAGE_DEF_MAZE, FLASH_ADDR_DEF_MAZE_SIZE_X, mazeDef.maze_size_x );
	FLASH_Write( FLASH_PAGE_DEF_MAZE, FLASH_ADDR_DEF_MAZE_SIZE_Y, mazeDef.maze_size_y );
	FLASH_Write( FLASH_PAGE_DEF_MAZE, FLASH_ADDR_DEF_MAZE_GOAL_X, mazeDef.maze_goal_x );
	FLASH_Write( FLASH_PAGE_DEF_MAZE, FLASH_ADDR_DEF_MAZE_GOAL_Y, mazeDef.maze_goal_y );
}
void FLASH_Read_Def_Maze( void ){
	/* <概要>	迷路定義読み込み関数
	 * <引数>	なし
	 * <戻り値>	なし
	 */
	mazeDef.maze_size_x = FLASH_Read( FLASH_PAGE_DEF_MAZE, FLASH_ADDR_DEF_MAZE_SIZE_X );
	mazeDef.maze_size_y = FLASH_Read( FLASH_PAGE_DEF_MAZE, FLASH_ADDR_DEF_MAZE_SIZE_Y );
	mazeDef.maze_goal_x = FLASH_Read( FLASH_PAGE_DEF_MAZE, FLASH_ADDR_DEF_MAZE_GOAL_X );
	mazeDef.maze_goal_y = FLASH_Read( FLASH_PAGE_DEF_MAZE, FLASH_ADDR_DEF_MAZE_GOAL_Y );
}
void FLASH_Write_Def_Sensor( void ){
	/* <概要>	壁センサしきい値書き込み関数
	 * <引数>	なし
	 * <戻り値>	なし
	 */
	// 書き込んだ回数の取得
	int write_time = FLASH_Read( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_TIME );

	// ページの消去
	FLASH_Erase( FLASH_PAGE_DEF_SENSOR );

	int addr=0;
	FLASH_Write( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_MAZE_TIME, write_time+1 );
	FLASH_Write( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_SR_THRESHOLD, senDef.sr_threshold );
	FLASH_Write( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_FR_THRESHOLD, senDef.fr_threshold );
	FLASH_Write( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_FL_THRESHOLD, senDef.fl_threshold );
	FLASH_Write( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_SL_THRESHOLD, senDef.sl_threshold );
	FLASH_Write( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_SR_CENTER, senDef.sr_center );
	FLASH_Write( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_SL_CENTER, senDef.sl_center );
	FLASH_Write( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_FR_CENTER, senDef.fr_center );
	FLASH_Write( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_FL_CENTER, senDef.fl_center );
}
void FLASH_Read_Def_Sensor( void ){
	/* <概要>	壁センサしきい値読み込み関数
	 * <引数>	なし
	 * <戻り値>	なし
	 */
	senDef.sr_threshold = FLASH_Read( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_SR_THRESHOLD );
	senDef.fr_threshold = FLASH_Read( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_FR_THRESHOLD );
	senDef.fl_threshold = FLASH_Read( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_FL_THRESHOLD );
	senDef.sl_threshold = FLASH_Read( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_SL_THRESHOLD );
	senDef.sr_center = FLASH_Read( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_SR_CENTER );
	senDef.sl_center = FLASH_Read( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_SL_CENTER );
	senDef.fr_center = FLASH_Read( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_FR_CENTER );
	senDef.fl_center = FLASH_Read( FLASH_PAGE_DEF_SENSOR, FLASH_ADDR_DEF_SENSOR_FL_CENTER );
}
void FLASH_Write_Maze_Wall( void ){
	/* <概要>	壁情報書き込み関数
	 * <引数>	なし
	 * <戻り値>	なし
	 */
	// ページの消去
	FLASH_Erase( FLASH_PAGE_MAZE_WALL );

	// 書き込む情報を16ビットに変換
	int data_north;
	int data_east;
	int data_south;
	int data_west;
	int data_search;
	for( int i = 0; i <= MAZE_SIZE_X; i++ ){
		data_north  = 0;
		data_east   = 0;
		data_south  = 0;
		data_west   = 0;
		data_search = 0;
		for( int j = 0; j <= MAZE_SIZE_Y; j++ ){
			data_north = data_north | ( maze.wall_north[i][j] << j );
			data_east  = data_east  | ( maze.wall_east [i][j] << j );
			data_south = data_south | ( maze.wall_south[i][j] << j );
			data_west  = data_west  | ( maze.wall_west [i][j] << j );
			data_search= data_search| ( maze.is_search [i][j] << j );
		}
		FLASH_Write( FLASH_PAGE_MAZE_WALL, FLASH_ADDR_MAZE_WALL_NORTH + i, data_north );
		FLASH_Write( FLASH_PAGE_MAZE_WALL, FLASH_ADDR_MAZE_WALL_EAST  + i, data_east  );
		FLASH_Write( FLASH_PAGE_MAZE_WALL, FLASH_ADDR_MAZE_WALL_SOUTH + i, data_south );
		FLASH_Write( FLASH_PAGE_MAZE_WALL, FLASH_ADDR_MAZE_WALL_WEST  + i, data_west  );
		FLASH_Write( FLASH_PAGE_MAZE_WALL, FLASH_ADDR_MAZE_SEARCH     + i, data_search);
	}
}
void FLASH_Read_Maze_Wall( void ){
	/* <概要>	壁情報読み込み関数
	 * <引数>	なし
	 * <戻り値>	なし
	 */
	int data_north;
	int data_east;
	int data_south;
	int data_west;
	int data_search;
	for( int i = 0; i <= MAZE_SIZE_X; i++ ){
		data_north  = FLASH_Read( FLASH_PAGE_MAZE_WALL, FLASH_ADDR_MAZE_WALL_NORTH + i );
		data_east   = FLASH_Read( FLASH_PAGE_MAZE_WALL, FLASH_ADDR_MAZE_WALL_EAST  + i );
		data_south  = FLASH_Read( FLASH_PAGE_MAZE_WALL, FLASH_ADDR_MAZE_WALL_SOUTH + i );
		data_west   = FLASH_Read( FLASH_PAGE_MAZE_WALL, FLASH_ADDR_MAZE_WALL_WEST  + i );
		data_search = FLASH_Read( FLASH_PAGE_MAZE_WALL, FLASH_ADDR_MAZE_SEARCH     + i );
		for( int j = 0; j <= MAZE_SIZE_Y; j++ ){
			maze.wall_north[i][j] = ( data_north  >> j ) & 1;
			maze.wall_east [i][j] = ( data_east   >> j ) & 1;
			maze.wall_south[i][j] = ( data_south  >> j ) & 1;
			maze.wall_west [i][j] = ( data_west   >> j ) & 1;
			maze.is_search [i][j] = ( data_search >> j ) & 1;
		}
	}
}
