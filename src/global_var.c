/*
 * global_var.c
 *
 * Data   : 2017.10.26
 * Author : M.Kato
 * Update : 2017.12.21
 */

#include "global_var.h"


t_mazeDef mazeDef = { MAZE_SIZE_X, MAZE_SIZE_Y, MAZE_GOAL_X, MAZE_GOAL_Y };	// 迷路定義用
t_sensorDef senDef = { SR_WALL_THRESHOLD, FR_WALL_THRESHOLD, FL_WALL_THRESHOLD, SL_WALL_THRESHOLD, SR_WALL_CENTER, SL_WALL_CENTER, FR_WALL_CENTER, FL_WALL_CENTER };	// しきい値用
t_sensor sen_sr = { 0, 0, 0, 0, 0, 0 };										// 右横壁用
t_sensor sen_fr = { 0, 0, 0, 0, 0, 0 };										// 右前壁用
t_sensor sen_fl = { 0, 0, 0, 0, 0, 0 };										// 左前壁用
t_sensor sen_sl = { 0, 0, 0, 0, 0, 0 };										// 左横壁用
t_moter moter = { 0, 0, MIN_TARGET_VEL, 0, 0, 0, 0, 0 };					// モータ変数用
t_control control = {};														// モータ制御用
t_position position = { MAZE_START_X, MAZE_START_Y, north };				// マウスの情報
t_maze maze = {};															// 迷路情報
t_time tim={};
t_run_mode run_mode={};
