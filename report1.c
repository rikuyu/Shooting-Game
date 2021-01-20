#include "gba1.h"

//�v���g�^�C�v�錾
void draw_bullet(hword, hword, hword, int);			 //�~��\��
void draw_square(hword, hword, hword);				 //�l�p�`��\��
void shoot(hword, hword);							 //�e���o���֐�
void draw_enemy(hword ix, hword iy, hword i_color);	 //�G��\��
void draw_player(hword px, hword py, hword p_color); //�v���C���[��\��
void display_time(hword val);						 //time��\��

// //�v���g�^�C�v�錾
// void draw_bullet(hword, hword, hword, int);			 //�~��\��
// void draw_square(hword, hword, hword);				 //�l�p�`��\��
// void shoot(hword, hword);							 //�e���o���֐�
// void draw_enemy(hword ix, hword iy, hword i_color);	 //�G��\��
// void draw_player(hword px, hword py, hword p_color); //�v���C���[��\��
// void display_time(hword val);						 //time��\��

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
	/* ��ʏ��������[�`�� */
	*((hword *)IOBASE) = 0xF03;

	hword player_x = 120; //�v���C���[�̏�����x���W
	hword player_y = 140; //�v���C���[�̏�����y���W
	hword enemy_x = 0;	  //�G�̏�����x���W
	hword enemy_y = 40;	  //�G�̏�����y���W
	hword start_flag = 1; //GAME START��while���̐^���l
	hword point = 0;	  //���߂̓_��

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

		//�d�l��\��
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
		{ //	B�������ꂽ��while���𔲂��o��
			break;
		}
	}

	draw_square(240, 160, 0x0000); //��ʂ���U���ɓh��Ԃ�
	locate(1, 1);
	prints("Point ="); //�_���\��

	// for (j = 0; j < 480; j++)
	// {
	// 	draw_point(j, 16, BGR(0x1F, 0x00, 0x1F)); //�}�[���^�̃��C����\��
	// }

	// *((unsigned short *)0x0400010E) = 0x0000; // �^�C�}3 ����ݒ�i�^�C�}ON�C������OFF�C�J�X�P�[�hON�C�v���X�P�[���Ȃ��j
	// *((unsigned short *)0x0400010E) = 0x0084; // �^�C�}3 ����ݒ�i�^�C�}ON�C������OFF�C�J�X�P�[�hON�C�v���X�P�[���Ȃ��j

	locate(19, 3);
	prints("Time = ");

	locate(16, 1);
	prints("Goal Point = 5");
	while (start_flag)
	{

		/* �^�C�}3 �J�E���g�l�Ǎ��� */
		time = *((unsigned short *)0x0400010C);

		/* time �\���ʒu�ݒ� */
		locate(26, 3);
		display_time(time);

		/* �L�[���� */

		key = ~(*(hword *)KEY_STATUS) & KEY_ALL;

		draw_player(player_x, player_y, BLUE); //�v���C���[����ʂɕ\��

		draw_enemy(enemy_x, enemy_y, 0x0000); //�G����ʂɕ\��
		enemy_x += 4;						  //�G�̈ړ�����

		if (enemy_x == 240)
		{ //�E�[�܂ł������牺�̒i�ɍ~��鏈��
			// enemy_y += 19;
			enemy_x = 0;
		}
		else if (enemy_y >= 133)
		{
			start_flag = 0; //while���𔲂��o������
		}

		draw_enemy(enemy_x, enemy_y, YELLOW);

		locate(9, 1);
		printn(point); //�_���\��

		switch (key)
		{

		case KEY_LEFT:
			if (player_x == 0)
			{ //���[�ō��������ꂽ�牽�����Ȃ�
			}
			else
			{
				draw_player(player_x, player_y, 0x0000); //�F����x�w�i�F��
				player_x -= 3;							 //x���W������1���炷
				draw_player(player_x, player_y, BLUE);	 //�ĂѐF�𔒂ɖ߂�
			}
			break;

		case KEY_RIGHT:
			if (player_x == 216)
			{ //�E�[�ŉE�������ꂽ�牽�����Ȃ�
			}
			else
			{
				draw_player(player_x, player_y, 0x0000); //�F����x�w�i�F��
				player_x += 3;							 //x���W���E��1���炷
				draw_player(player_x, player_y, BLUE);	 //�ĂѐF�𔒂ɖ߂�
			}
			break;

		case KEY_A:
			shoot(player_x, player_y - 3);
			//�G�Ƃ�x���W�̂��ꂪ�}5�̏ꍇ(�e�����������ꍇ)
			if (player_x + 12 > enemy_x && player_x + 12 < enemy_x + 24)
			{
				draw_enemy(enemy_x, enemy_y, 0x0000);
				enemy_x = 0;
				enemy_y = 40;
				point += 1; //�_�����Z
			}
			break;
		}

		int i;
		for (i = 0; i < 20000; i++)
		{
		} //���Ԓ���
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

	// draw_square(240, 160, BGR(0x1F, 0x00, 0x00)); //GAMEOVER��\��
	// locate(10, 10);
	// prints("Game Over...");
	prints("F I N I S H");
	draw_enemy(116, 56, BLUE);

	/* �������[�v�ɂ��v���O������~ */
	while (1)
		;

	return 0;
}

//�l�p�`��\��
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

//�~��\��
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
//�e�����֐�
void shoot(hword x, hword y)
{
	while (y > 40)
	{									  //y���W���ŏ�ʂɗ���܂�
		draw_bullet(x + 4, y, 0x0000, 2); //�w�i�F�ɓh��Ԃ�
		y--;							  //y���W��1�h�b�g��Ɉړ�
		draw_bullet(x + 4, y, BLUE, 2);	  //�e��\��
		//�������ɂ���ׂ̏���
		int i;
		for (i = 0; i < 50; i++)
		{
		}
	}
	draw_bullet(x + 12, y, 0x0000, 2);
}
//�G��\��
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
//�v���C���[��\��
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

	/* ������10�i���Ƃ��ĕ\������֐��i�ő�l16�r�b�g�j*/

	byte char_data[] = "0123456789";
	byte buf[6];
	hword tmp;
	int i;

	/* ���͒lval�̌����ݒ� */
	i = 3;

	/* ������̍Ō��NULL�R�[�h�}�� */
	buf[i + 1] = 0;

	/* �ŉ��ʌ��̕����i10�Ŋ������]��j��}�����C���͒l��10�Ŋ��� */
	for (; i >= 0; i--)
	{
		buf[i] = char_data[mod(val, 10)];
		val = div(val, 10);
	}

	/* ������S�̂�\�� */
	prints(buf);

	return;
}
