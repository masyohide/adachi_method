/*
 * main.cpp
 *
 * Data   : 2017.10.26
 * Author : M.Kato
 * Update : 2017.12.21
 */

#include "mode.h"
#include "config.h"

int main( void ){
	// 初期設定
	Init_All();

	/*MODE_My_Sample();*/

	// 各モード実行
	while( 1 ){
	 MODE_Run( MODE_Select() );
	}

	return 0;
}
