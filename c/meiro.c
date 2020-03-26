#include <stdio.h>

#define MAZE_ROW 7 //迷路の行数
#define MAZE_COLUMN 7 //迷路の列数

//プレイヤー
typedef struct
{
	int row; //プレイヤー位置(行)
	int column; //プレイヤー位置(列)
} MazePlayer;

//迷路の各ブロック
enum MazeKind {FRAME, PATH, WALL, START, GOAL}; //ブロックの種類（枠、道、壁、スタート、ゴール）
enum MazeFlag {FALSE, TRUE}; //ブロックの判定

typedef struct
{
	enum MazeKind kind;
	enum MazeFlag flag;
} MazeBlock;

//プレイヤー初期化
int MazePlayerInit(int *playerRow, int *playerColumn, MazeBlock maze[MAZE_ROW][MAZE_COLUMN])
{
	int i, j;

	for(i = 0; i < MAZE_ROW; i++)
	{
		for(j = 0; j < MAZE_COLUMN; j++)
		{
			if(maze[i][j].kind == START)
			{
				*playerRow = i;
				*playerColumn = j;
				return 0;
			}
		}
	}
	printf("スタートがありません\n");
	return -1;
}

//迷路表示
void MazeDraw(int playerRow, int playerColumn, MazeBlock maze[MAZE_ROW][MAZE_COLUMN])
{
	int i, j;

	for(i = 0; i < MAZE_ROW; i++) //行
	{
		for(j = 0; j < MAZE_COLUMN; j++) //列
		{
			if(i == playerRow && j == playerColumn) //プレイヤー位置
				printf("P");
			else if(maze[i][j].flag == FALSE) //ブロックが判明していない場合
				printf("?");
			else
			{
				switch(maze[i][j].kind)
				{
					case FRAME:
						printf("□"); break; //枠
					case WALL:
						printf("#"); break; //壁
					case GOAL:
						printf("G"); break; //ゴール
					default:
						printf(" "); break; //その他
				}
			}
		}
		printf("\n");
	}
}

int main(void)
{
	//プレイヤー
	MazePlayer player;

	//迷路
	MazeBlock maze[MAZE_ROW][MAZE_COLUMN] =
	{
		{ {FRAME, TRUE } , {FRAME, TRUE } , {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE } },
		{ {FRAME, TRUE } , {START, TRUE } , {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {FRAME, TRUE } },
		{ {FRAME, TRUE } , {WALL , FALSE} , {WALL , FALSE}, {PATH , FALSE}, {WALL , FALSE}, {WALL , FALSE}, {FRAME, TRUE } },
		{ {FRAME, TRUE } , {WALL , FALSE} , {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {FRAME, TRUE } },
		{ {FRAME, TRUE } , {PATH , FALSE} , {PATH , FALSE}, {WALL , FALSE}, {WALL , FALSE}, {WALL , FALSE}, {FRAME, TRUE } },
		{ {FRAME, TRUE } , {WALL , FALSE} , {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {GOAL , TRUE }, {FRAME, TRUE } },
		{ {FRAME, TRUE } , {FRAME, TRUE } , {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE } },
	};

	//プレイヤー初期化（スタートがなかったら終了）
	if(MazePlayerInit(&player.row, &player.column, maze) == -1)
		return 0;
	
	//迷路表示
	MazeDraw(player.row, player.column, maze);

	return 0;
}