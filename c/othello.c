#include <stdio.h>

#define BOARD_SQUARE 9
#define EMPTY 0
#define BLACK 1
#define WHITE 2

int board[BOARD_SQUARE][BOARD_SQUARE];

//盤面リセット
void shokika(void)
{
	int i,j;

	for(i = 0; i < BOARD_SQUARE; i++)
	{
		for(j = 0; j < BOARD_SQUARE; j++)
			board[i][j] = EMPTY;
	}
	board[4][4] = WHITE;
	board[4][5] = BLACK;
	board[5][4] = BLACK;
	board[5][5] = WHITE;
}

//盤面表示・更新
void display(void)
{
	int i,j;

	for(i = 0; i < BOARD_SQUARE; i++)
	{
		for(j = 0; j < BOARD_SQUARE; j++)
		{
			switch(board[i][j])
			{
				case WHITE:
					printf("🔴"); break;
				case BLACK:
					printf("⚫︎"); break;
				default:
					if(i == 0 || j == 0)
					{
						if(i == 0)
							board[i][j] = j;
						else
							board[i][j] = i;
						
						if(board[i][j] == board[0][0])
							printf("%d", board[i][j]);
						else if(i == 0)
						{
							switch(j)
							{
								case 1:
									printf(" １２３４５６７８");
									break;
							}
						}
						else
							printf("%d ", board[i][j]);
						break;
					}
					else
						printf("・");
			}
			if(j == BOARD_SQUARE -1)
				printf("\n");
		}
	}
}

int main(int argc, char *argv[])
{
	shokika();
	display();

	return 0;
}