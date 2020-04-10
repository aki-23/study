#include <stdio.h>
#include <stdlib.h>

#define BOARD_SQUARE 8
#define EMPTY 0
#define BLACK 1
#define WHITE 2

int board[BOARD_SQUARE][BOARD_SQUARE];

int main(void)
{
	int x,y;

	for(x = 0; x < BOARD_SQUARE; x++)
	{
		for(y = 0; y < BOARD_SQUARE; y++)
			board[x][y] = EMPTY;
	}

	for(x = 0; x < BOARD_SQUARE; x++)
	{
		for(y = 0; y < BOARD_SQUARE; y++)
		{
			switch(board[x][y])
			{
				case EMPTY:
					printf("□"); break;
				case BLACK:
					printf("●"); break;
				case WHITE:
					printf("○"); break;
			}
		}
		printf("\n");
	}

	return 0;
}