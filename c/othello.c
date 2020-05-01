#include <stdio.h>
#include <stdlib.h>

#define BOARD_SQUARE 8

#define EMPTY 0
#define BLACK 1
#define WHITE 2

#define FALSE 0
#define TRUE  1

int board[BOARD_SQUARE][BOARD_SQUARE];
int player_number = 1;

void Board_Output(void);
void Game(void);
int Board_Judgment(int x, int y);
int Board_Judgment_Sub(int x, int y, int move_x, int move_y);
void Board_Scan(void);
void Finish(void);


int main(void)
{
	int i;
	int x, y;

	for(x = 0; x < BOARD_SQUARE; x++)
	{
		for(y = 0; y < BOARD_SQUARE; y++)
		{
			if((x == 3 && y == 3) || (x == 4 && y == 4))
				board[x][y] = WHITE;
			else if((x == 3 && y == 4) || (x == 4 && y == 3))
				board[x][y] = BLACK;
			else
				board[x][y] = EMPTY;
		}
	}

	Board_Output();

	printf("\nゲームスタート！\n\n");

	for(i = 0; i < (BOARD_SQUARE * BOARD_SQUARE - 4); i++)
	{
		Game();
		Board_Output();

		if(player_number < 2)
			player_number++;
		else
			player_number = 1;
	}
	
	Finish();

	return 0;
}

void Board_Output(void)
{
	int x, y;

	for(x = 0; x < BOARD_SQUARE; x++)
	{
		if (x == 0)
			// printf("\n  12345678");
			printf("\n  １２３４５６７８\n");

		printf("%d ", x + 1);
		for(y = 0; y < BOARD_SQUARE; y++)
		{
			switch(board[x][y])
			{
				case EMPTY:
					// printf("□"); break;
					printf("・"); break;
				case BLACK:
					// printf("●"); break;
					printf("⚫️"); break;
				case WHITE:
					// printf("○"); break;
					printf("⚪️"); break;
			}
		}
		printf("\n");
	}
}

void Game(void)
{
	int x, y;

	printf("%dP(",player_number);
	switch(player_number)
	{
		case BLACK:
			// printf("●"); break;
			printf("⚫️"); break;
		case WHITE:
			// printf("○"); break;
			printf("⚪️"); break;
	}
	printf(")の番です。\n");

	while(1)
	{
		printf("置く場所(縦の番号 横の番号)を入力してください。: ");
		scanf("%d %d", &x, &y);

		if(x >= 1 && x <= BOARD_SQUARE && y >= 1 && y <= BOARD_SQUARE)
		{
			x --;
			y --;
			if(board[x][y] == EMPTY)
			{
				if(Board_Judgment(x, y) == TRUE)
					break;
				else
					printf("その場所には置けません。\n");
			}
			else
				printf("すでに駒が置いてあるため、その場所には置けません。\n");
		}
		else
			printf("範囲外のため、その場所には置けません。\n");
	}
	Board_Scan();
}

int Board_Judgment(int x, int y)
{
	int move_x, move_y;
	int judgment_num = FALSE;

	//上方向
	move_x = -1;
	move_y = 0;
	if(Board_Judgment_Sub(x, y, move_x, move_y) == TRUE)
		judgment_num = TRUE;
	
	//右上方向
	move_x = -1;
	move_y = 1;
	if(Board_Judgment_Sub(x, y, move_x, move_y) == TRUE)
		judgment_num = TRUE;

	//右方向
	move_x = 0;
	move_y = 1;
	if(Board_Judgment_Sub(x, y, move_x, move_y) == TRUE)
		judgment_num = TRUE;

	//右下方向
	move_x = 1;
	move_y = 1;
	if(Board_Judgment_Sub(x, y, move_x, move_y) == TRUE)
		judgment_num = TRUE;

	//下方向
	move_x = 1;
	move_y = 0;
	if(Board_Judgment_Sub(x, y, move_x, move_y) == TRUE)
		judgment_num = TRUE;

	//左下方向
	move_x = 1;
	move_y = -1;
	if(Board_Judgment_Sub(x, y, move_x, move_y) == TRUE)
		judgment_num = TRUE;

	//左方向
	move_x = 0;
	move_y = -1;
	if(Board_Judgment_Sub(x, y, move_x, move_y) == TRUE)
		judgment_num = TRUE;

	//左上方向
	move_x = -1;
	move_y = -1;
	if(Board_Judgment_Sub(x, y, move_x, move_y) == TRUE)
		judgment_num = TRUE;

	return judgment_num;
}

int Board_Judgment_Sub(int x, int y, int move_x, int move_y)
{
	int i = 1;
	int other_player_num;
	int judgment_num = 0;

	if(player_number == BLACK)
		other_player_num = WHITE;
	else
		other_player_num = BLACK;

	while(i < BOARD_SQUARE)
	{
		if(x + (move_x * i) > BOARD_SQUARE || y + (move_y * i) > BOARD_SQUARE)
			return 0;
		if(board[x + (move_x * i)][y + (move_y * i)] == other_player_num)
			judgment_num = 1;
		else if (board[x + (move_x * i)][y + (move_y * i)] == player_number)
		{
			i = i - 1;
			if(judgment_num == 0)
				return 0;
			else
			{
				while(i >= 0)
				{
					board[x + (move_x * i)][y + (move_y * i)] = player_number;
					i--;
				}
				return 1;
			}
		}
		else
			return 0;
		i++;
	}
	return 0;
}

void Board_Scan(void)
{
	int x, y;
	int black_num = 0, white_num = 0;

	for(x = 0; x < BOARD_SQUARE; x++)
	{
		for(y = 0; y < BOARD_SQUARE; y++)
		{
			if(board[x][y] == BLACK)
				black_num = 1;
			else if(board[x][y] == WHITE)
				white_num = 1;
		}
	}

	if(black_num == 0 || white_num == 0)
		Finish();
}

void Finish(void)
{
	int x, y;
	int black_count = 0, white_count = 0;

	Board_Output();

	for(x = 0; x < BOARD_SQUARE; x++)
	{
		for(y = 0; y < BOARD_SQUARE; y++)
		{
			if(board[x][y] == BLACK)
				black_count += 1;
			else if(board[x][y] == WHITE)
				white_count += 1;
		}
	}

	// printf("1P(●) : %d  2P(○) : %d\n", black_count, white_count);
	printf("1P(⚫️) : %d  2P(⚪️) : %d\n", black_count, white_count);
	if(black_count > white_count)
		// printf("1P(●)の勝利！\n");
		printf("1P(⚫️)の勝利！\n");
	else if(black_count < white_count)
		// printf("2P(○)の勝利！\n");
		printf("2P(⚪️)の勝利！\n");
	else
		printf("引き分け\n");

	exit(0);
}