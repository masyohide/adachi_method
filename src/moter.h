#ifndef SRC_MOTER_H_
#define SRC_MOTER_H_


void MOTER_Straight( int len );				// 指定距離直進
void MOTER_Straight2(int len);
void MOTER_Straight3(int len);
void MOTER_Straight_Search( int len );		// 指定距離直進 探索用
void MOTER_Turn( int dir, int deg );			// 指定方向に指定角度回転
void MOTER_Back( int len );					// 指定距離後進
void MOTER_Calc_Control( void );				// モータ制御量計算
void MOTER_Set_Param( void );					// モータ関連パラメータ設定


#endif /* SRC_MOTER_H_ */
