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
						printf(" "); break; //道、スタート
				}
			}
		}
		printf("\n");
	}
}

//方向
enum MazeDirection{UP, DOWN, LEFT, RIGHT, Invalid};

//プレイヤー移動
void MazePlayerMove(int *playerRow, int *playerColumn, MazeBlock maze[MAZE_ROW][MAZE_COLUMN])
{
	char buf[100];
	int direction = -1;

	printf("%d : 上\n", UP);
	printf("%d : 下\n", DOWN);
	printf("%d : 左\n", LEFT);
	printf("%d : 右\n", RIGHT);
	printf("数字を入力してください。 :");

	fgets(buf, sizeof(buf), stdin); //文字列で入力を読み込む
	sscanf(buf, "%d", &direction); //数字にできる場合は変換（できなければ何もしない）

	while(direction < 0 || direction > (Invalid - 1)) //入力が正しい場合まで繰り返す
	{
		printf("入力が不正です。再入力してください :");
		fgets(buf, sizeof(buf), stdin);
		sscanf(buf, "%d", &direction);
	}

	switch(direction)
	{
		//上移動
		case UP:
		{
			if(*playerRow - 1 >= 1) //範囲外でないことを確認
			{
				maze[*playerRow - 1][*playerColumn].flag = TRUE;
				if(maze[*playerRow - 1][*playerColumn].kind != WALL) //壁判定
				{
					*playerRow -= 1; //移動
					printf("\n上に移動しました。\n");
				}
				else
					printf("\n壁です。\n");
			}
			else
				printf("\n範囲外です。\n");
		}
		break;

		//下移動
		case DOWN:
		{
			if(*playerRow + 1 < MAZE_ROW - 1)
			{
				maze[*playerRow + 1][*playerColumn].flag = TRUE;
				if(maze[*playerRow + 1][*playerColumn].kind != WALL) //壁判定
				{
					*playerRow += 1; //移動
					printf("\n下に移動しました。\n");
				}
				else
					printf("\n壁です。\n");
			}
			else
				printf("\n範囲外です。\n");
		}
		break;

		//左移動
		case LEFT:
		{
			if(*playerColumn - 1 >= 1)
			{
				maze[*playerRow][*playerColumn - 1].flag = TRUE;
				if(maze[*playerRow][*playerColumn - 1].kind != WALL)
				{
					*playerColumn -= 1;
					printf("\n左に移動しました。\n");
				}
				else
					printf("\n壁です。\n");
			}
			else
				printf("\n範囲外です。\n");
		}
		break;

		//右移動
		case RIGHT:
		{
			if(*playerColumn + 1 < MAZE_COLUMN - 1)
			{
				maze[*playerRow][*playerColumn + 1].flag = TRUE;
				if(maze[*playerRow][*playerColumn + 1].kind != WALL)
				{
					*playerColumn += 1;
					printf("\n右に移動しました。\n");
				}
				else
					printf("\n壁です。\n");
			}
			else
				printf("\n範囲外です。\n");
		}
		break;
	}
}

//ゴール判定
int MazeGoalCheck(int playerRow, int playerColumn, MazeBlock maze[MAZE_ROW][MAZE_COLUMN])
{
	if(maze[playerRow][playerColumn].kind == GOAL) //プライヤー位置がゴール地点に等しい
	{
		printf("ゴール！\n");
		return 1;
	}
	return 0;
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
	
	while(MazeGoalCheck(player.row, player.column, maze) != 1)
	{
	//迷路表示
	MazeDraw(player.row, player.column, maze);
	//プレイヤー移動
	MazePlayerMove(&player.row, &player.column, maze);
	}

	//迷路最終結果発表
	MazeDraw(player.row, player.column, maze);

	return 0;
}