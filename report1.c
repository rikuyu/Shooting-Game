#include "gba1.h"

//�v���g�^�C�v�錾
void display_bullet(hword, hword, hword, int);
void transition_screen(hword, hword, hword);
void shoot(hword, hword);
void display_player(hword px, hword py, hword color);
void display_block(hword bx, hword by, hword color);
void display_time(hword val);

unsigned char block[10][10] = {
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
	/* ��ʏ�����*/
	*((hword *)IOBASE) = 0xF03;

	hword player_x = 120;
	hword player_y = 140;
	hword block_x = 0;
	hword block_y = 40;
	hword start_flag = 1;
	hword point = 0;
	hword clear_time;
	hword display_width = 240;
	hword display_height = 160;
	hword enemy_color = YELLOW;
	hword enemy_level = 1;

	volatile unsigned short time;

	/* �^�C�}�J�E���^�ݒ背�W�X�^ */
	*((unsigned short *)0x04000100) = 0xFF00; // �^�C�}0 �J�E���^�ݒ�(���N���b�N1/2^24�b ��60n�b)
	*((unsigned short *)0x04000104) = 0xFF00; // �^�C�}1 �J�E���^�ݒ�(���N���b�N1/2^16�b ��15.4u�b)
	*((unsigned short *)0x04000108) = 0xFFE5; // �^�C�}2 �J�E���^�ݒ�(���N���b�N1/2^8�b  ��3.93m�b)
	*((unsigned short *)0x0400010C) = 0x0000; // �^�C�}3 �J�E���^�ݒ�(���N���b�N1�b)

	/* �^�C�}���䃌�W�X�^ */
	*((unsigned short *)0x04000102) = 0x0080; // �^�C�}0 ����ݒ�i�^�C�}ON�C������OFF�C�J�X�P�[�hOFF�C�v���X�P�[���Ȃ��j
	*((unsigned short *)0x04000106) = 0x0084; // �^�C�}1 ����ݒ�i�^�C�}ON�C������OFF�C�J�X�P�[�hON�C�v���X�P�[���Ȃ��j
	*((unsigned short *)0x0400010A) = 0x0084; // �^�C�}2 ����ݒ�i�^�C�}ON�C������OFF�C�J�X�P�[�hON�C�v���X�P�[���Ȃ��j
	*((unsigned short *)0x0400010E) = 0x0084; // �^�C�}3 ����ݒ�i�^�C�}ON�C������OFF�C�J�X�P�[�hON�C�v���X�P�[���Ȃ��j

	while (1)
	{
		/* �L�[���� */
		key = ~(*(hword *)KEY_STATUS) & KEY_ALL;

		//���������\��
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
		{
			break;
		}
	}

	//���������ʂ���̑J��
	transition_screen(display_width, display_height, BLACK);
	locate(1, 1);
	prints("Point =");

	locate(19, 3);
	prints("Time = ");

	locate(16, 1);
	prints("Goal Point = 5");
	while (start_flag)
	{
		/* �^�C�}3 �J�E���g�l�Ǎ��� */
		time = *((unsigned short *)0x0400010C);

		locate(26, 3);
		display_time(time);

		/* �L�[���� */
		key = ~(*(hword *)KEY_STATUS) & KEY_ALL;

		display_player(player_x, player_y, BLUE);

		display_block(block_x, block_y, BLACK);
		block_x += 4;

		switch (point)
		{
		case 0:
			enemy_color = YELLOW;
			break;
		case 1:
			enemy_color = GREEN;
			break;
		case 2:
			enemy_color = CYAN;
			break;
		case 3:
			enemy_color = MAGENTA;
			break;
		case 4:
			enemy_color = RED;
			break;
		default:
			break;
		}
		if (block_x == 240)
		{
			block_x = 0;
		}
		display_block(block_x, block_y, enemy_color);

		switch (key)
		{
		case KEY_LEFT:
			if (player_x != 0)
			{
				display_player(player_x, player_y, BLACK);
				player_x -= 3;
				display_player(player_x, player_y, BLUE);
			}
			break;

		case KEY_RIGHT:
			if (player_x != 231)
			{
				display_player(player_x, player_y, BLACK);
				player_x += 3;
				display_player(player_x, player_y, BLUE);
			}
			break;

		case KEY_A:
			shoot(player_x, player_y - 3);
			if (player_x + 12 > block_x && player_x + 12 < block_x + 20)
			{
				display_block(block_x, block_y, BLACK);
				block_x = 0;
				block_y = 40;
				point += 1;
			}
			break;
		}
		locate(9, 1);
		printn(point);
		int i;
		for (i = 0; i < 30000; i++)
		{
		}
		if (point == 5)
		{
			clear_time = time;
			locate(13, 7);
			prints("Clear");
			locate(11, 9);
			prints("Thank You");
			locate(11, 11);
			prints("Claer Time");
			locate(13, 13);
			display_time(clear_time);
			locate(17, 13);
			prints("ms");
			while (1)
				;
		}
	}
	while (1)
		;
	return 0;
}

void transition_screen(hword x, hword y, hword color)
{
	hword point_x, point_y;
	for (point_x = 0; point_x < x; point_x++)
	{
		for (point_y = 0; point_y < y; point_y++)
		{
			draw_point(point_x, point_y, BLACK);
		}
	}
}

void display_bullet(hword a, hword b, hword color, int r)
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

void shoot(hword x, hword range_height)
{
	while (range_height > 40)
	{
		display_bullet(x + 4, range_height, BLACK, 2);
		range_height--;
		display_bullet(x + 4, range_height, BLUE, 2);
		int i;
		for (i = 0; i < 140; i++)
		{
		}
	}
	display_bullet(x + 12, range_height, BLACK, 2);
}

void display_player(hword px, hword py, hword color)
{
	int x, y;
	for (x = 0; x < 10; x++)
	{
		for (y = 0; y < 10; y++)
		{
			if (player[x][y] == 1)
			{
				draw_point((y + px), (x + py), color);
			}
		}
	}
}
void display_block(hword bx, hword by, hword color)
{
	int x, y;
	for (x = 0; x < 10; x++)
	{
		for (y = 0; y < 10; y++)
		{
			if (block[x][y] == 1)
			{
				draw_point((y + bx), (x + by), color);
			}
		}
	}
}

void display_time(hword val)
{
	byte char_data[] = "0123456789";
	byte buf[6];
	hword tmp;
	int i = 3;
	buf[i + 1] = 0;
	for (; i >= 0; i--)
	{
		buf[i] = char_data[mod(val, 10)];
		val = div(val, 10);
	}

	prints(buf);

	return;
}
