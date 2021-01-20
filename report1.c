#include "gba1.h"

//プロトタイプ宣言
void draw_bullet(hword, hword, hword, int);			 //円を表示
void draw_square(hword, hword, hword);				 //四角形を表示
void shoot(hword, hword);							 //弾を出す関数
void draw_enemy(hword ix, hword iy, hword i_color);	 //敵を表示
void draw_player(hword px, hword py, hword p_color); //プレイヤーを表示
void display_time(hword val);						 //timeを表示

// //プロトタイプ宣言
// void draw_bullet(hword, hword, hword, int);			 //円を表示
// void draw_square(hword, hword, hword);				 //四角形を表示
// void shoot(hword, hword);							 //弾を出す関数
// void draw_enemy(hword ix, hword iy, hword i_color);	 //敵を表示
// void draw_player(hword px, hword py, hword p_color); //プレイヤーを表示
// void display_time(hword val);						 //timeを表示

unsigned char enemy[10][10] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

unsigned char player[10][10] = {
	{0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
	{0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
	{0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
	{0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
	{0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
	{0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
int main(void)
{
	hword key;
	// *((hword *)0x04000000) = 0x0F03;
	// hword *ptr2 = (hword *)0x04000130;
	/* 画面初期化ルーチン */
	*((hword *)IOBASE) = 0xF03;

	hword player_x = 120; //プレイヤーの初期のx座標
	hword player_y = 140; //プレイヤーの初期のy座標
	hword enemy_x = 0;	  //敵の初期のx座標
	hword enemy_y = 40;	  //敵の初期のy座標
	hword start_flag = 1; //GAME STARTのwhile文の真理値
	hword point = 0;	  //初めの点数

	volatile unsigned short time;

	/* タイマカウンタ設定レジスタ */
	*((unsigned short *)0x04000100) = 0xFF00; // タイマ0 カウンタ設定(元クロック1/2^24秒 約60n秒)
	*((unsigned short *)0x04000104) = 0xFF00; // タイマ1 カウンタ設定(元クロック1/2^16秒 約15.4u秒)
	*((unsigned short *)0x04000108) = 0xFFE5; // タイマ2 カウンタ設定(元クロック1/2^8秒  約3.93m秒)
	*((unsigned short *)0x0400010C) = 0x0000; // タイマ3 カウンタ設定(元クロック1秒)

	/* タイマ制御レジスタ */
	*((unsigned short *)0x04000102) = 0x0080; // タイマ0 制御設定（タイマON，割込みOFF，カスケードOFF，プリスケーラなし）
	*((unsigned short *)0x04000106) = 0x0084; // タイマ1 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）
	*((unsigned short *)0x0400010A) = 0x0084; // タイマ2 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）
	*((unsigned short *)0x0400010E) = 0x0084; // タイマ3 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）

	while (1)
	{

		/* キー入力 */
		key = ~(*(hword *)KEY_STATUS) & KEY_ALL;

		//仕様を表示
		locate(10, 3);
		prints("Shoot Game");

		locate(8, 5);
		prints("Push B to start");

		locate(13, 8);
		prints("Move");

		locate(7, 10);
		prints("RIGHT or LEFT key");

		locate(13, 13);
		prints("Shoot");

		locate(15, 15);
		prints("A");

		if (key == KEY_B)
		{ //	Bが押されたらwhile文を抜け出す
			break;
		}
	}

	draw_square(240, 160, 0x0000); //画面を一旦黒に塗りつぶす
	locate(1, 1);
	prints("Point ="); //点数表示

	// for (j = 0; j < 480; j++)
	// {
	// 	draw_point(j, 16, BGR(0x1F, 0x00, 0x1F)); //マゼンタのラインを表示
	// }

	// *((unsigned short *)0x0400010E) = 0x0000; // タイマ3 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）
	// *((unsigned short *)0x0400010E) = 0x0084; // タイマ3 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）

	locate(19, 3);
	prints("Time = ");

	locate(16, 1);
	prints("Goal Point = 5");
	while (start_flag)
	{

		/* タイマ3 カウント値読込み */
		time = *((unsigned short *)0x0400010C);

		/* time 表示位置設定 */
		locate(26, 3);
		display_time(time);

		/* キー入力 */

		key = ~(*(hword *)KEY_STATUS) & KEY_ALL;

		draw_player(player_x, player_y, BLUE); //プレイヤーを画面に表示

		draw_enemy(enemy_x, enemy_y, 0x0000); //敵を画面に表示
		enemy_x += 4;						  //敵の移動処理

		if (enemy_x == 240)
		{ //右端までいったら下の段に降りる処理
			// enemy_y += 19;
			enemy_x = 0;
		}
		else if (enemy_y >= 133)
		{
			start_flag = 0; //while文を抜け出す処理
		}

		draw_enemy(enemy_x, enemy_y, YELLOW);

		locate(9, 1);
		printn(point); //点数表示

		switch (key)
		{

		case KEY_LEFT:
			if (player_x == 0)
			{ //左端で左が押されたら何もしない
			}
			else
			{
				draw_player(player_x, player_y, 0x0000); //色を一度背景色に
				player_x -= 3;							 //x座標を左に1ずらす
				draw_player(player_x, player_y, BLUE);	 //再び色を白に戻す
			}
			break;

		case KEY_RIGHT:
			if (player_x == 216)
			{ //右端で右が押されたら何もしない
			}
			else
			{
				draw_player(player_x, player_y, 0x0000); //色を一度背景色に
				player_x += 3;							 //x座標を右に1ずらす
				draw_player(player_x, player_y, BLUE);	 //再び色を白に戻す
			}
			break;

		case KEY_A:
			shoot(player_x, player_y - 3);
			//敵とのx座標のずれが±5の場合(弾が当たった場合)
			if (player_x + 12 > enemy_x && player_x + 12 < enemy_x + 24)
			{
				draw_enemy(enemy_x, enemy_y, 0x0000);
				enemy_x = 0;
				enemy_y = 40;
				point += 1; //点数加算
			}
			break;
		}

		int i;
		for (i = 0; i < 20000; i++)
		{
		} //時間調節
		if (point == 5)
		{

			locate(13, 8);
			prints("Clear");

			locate(11, 11);
			prints("Thank You");

			while (1)
				;
		}
	}

	// draw_square(240, 160, BGR(0x1F, 0x00, 0x00)); //GAMEOVERを表示
	// locate(10, 10);
	// prints("Game Over...");
	prints("F I N I S H");
	draw_enemy(116, 56, BLUE);

	/* 無限ループによるプログラム停止 */
	while (1)
		;

	return 0;
}

//四角形を表示
void draw_square(hword x, hword y, hword color)
{
	hword x1, y1;
	for (x1 = 0; x1 < x; x1++)
	{
		for (y1 = 0; y1 < y; y1++)
		{
			draw_point(x1, y1, BGR(0x00, 0x00, 0x00));
		}
	}
}

//円を表示
void draw_bullet(hword a, hword b, hword color, int r)
{
	unsigned short *ptr;
	hword x, y;
	for (x = a - 10; x < a + 10; x++)
	{
		for (y = b - 10; y < b + 10; y++)
		{
			if ((x - a) * (x - a) + (y - b) * (y - b) <= r * r)
			{
				draw_point(x, y, color);
			}
		}
	}
}
//弾をうつ関数
void shoot(hword x, hword y)
{
	while (y > 40)
	{									  //y座標が最上位に来るまで
		draw_bullet(x + 4, y, 0x0000, 2); //背景色に塗りつぶす
		y--;							  //y座標を1ドット上に移動
		draw_bullet(x + 4, y, BLUE, 2);	  //弾を表示
		//ゆっくりにする為の処理
		int i;
		for (i = 0; i < 50; i++)
		{
		}
	}
	draw_bullet(x + 12, y, 0x0000, 2);
}
//敵を表示
void draw_enemy(hword ix, hword iy, hword i_color)
{
	int n, m;

	for (n = 0; n < 10; n++)
	{
		for (m = 0; m < 10; m++)
		{
			if (enemy[n][m] == 1)
			{
				draw_point((m + ix), (n + iy), i_color);
			}
		}
	}
}
//プレイヤーを表示
void draw_player(hword px, hword py, hword p_color)
{
	int n, m;

	for (n = 0; n < 10; n++)
	{
		for (m = 0; m < 10; m++)
		{
			if (player[n][m] == 1)
			{
				draw_point((m + px), (n + py), p_color);
			}
		}
	}
}

void display_time(hword val)
{

	/* 引数を10進数として表示する関数（最大値16ビット）*/

	byte char_data[] = "0123456789";
	byte buf[6];
	hword tmp;
	int i;

	/* 入力値valの桁数設定 */
	i = 3;

	/* 文字列の最後にNULLコード挿入 */
	buf[i + 1] = 0;

	/* 最下位桁の文字（10で割った余り）を挿入し，入力値を10で割る */
	for (; i >= 0; i--)
	{
		buf[i] = char_data[mod(val, 10)];
		val = div(val, 10);
	}

	/* 文字列全体を表示 */
	prints(buf);

	return;
}
