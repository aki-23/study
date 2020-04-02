#include <stdio.h>

#define STAGE //ステージ
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
int MazePlayerInit(int *playerRow, int *playerColumn, MazeBlock maze[MAZE_ROW * MAZE_COLUMN])
{
	int i, j;

	for(i = 0; i < MAZE_ROW; i++)
	{
		for(j = 0; j < MAZE_COLUMN; j++)
		{
			if(maze[MAZE_COLUMN * i + j].kind == START) //スタート地点ならばプレイヤーの位置に設定
			{
				*playerRow = i;
				*playerColumn = j;
				return 0;
			}
		}
	}
	printf("スタートがありません。\n");
	return -1;
}

//迷路表示
void MazeDraw(int playerRow, int playerColumn, MazeBlock maze[MAZE_ROW * MAZE_COLUMN])
{
	int i, j;

	for(i = 0; i < MAZE_ROW; i++) //行
	{
		for(j = 0; j < MAZE_COLUMN; j++) //列
		{
			if(i == playerRow && j == playerColumn) //プレイヤー位置
				printf("P");
			else if(maze[MAZE_COLUMN * i + j].flag == FALSE) //ブロックが判明していない場合
				printf("?");
			else
			{
				switch(maze[MAZE_COLUMN * i + j].kind)
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
void MazePlayerMove(int *playerRow, int *playerColumn, MazeBlock maze[MAZE_ROW * MAZE_COLUMN])
{
	char buf[100];
	int direction = -1;

	printf("%d : 上\n", UP);
	printf("%d : 下\n", DOWN);
	printf("%d : 左\n", LEFT);
	printf("%d : 右\n", RIGHT);
	printf("数字を入力してください。: ");

	fgets(buf, sizeof(buf), stdin); //文字列で入力を読み込む
	sscanf(buf, "%d", &direction); //数字にできる場合は変換（できなければ何もしない）

	while(direction < 0 || direction > (Invalid - 1)) //入力が正しい場合まで繰り返す
	{
		printf("入力が不正です。再入力してください。: ");
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
				maze[MAZE_COLUMN * ((*playerRow) - 1) + (*playerColumn)].flag = TRUE;
				if(maze[MAZE_COLUMN * ((*playerRow) - 1) + (*playerColumn)].kind != WALL) //壁判定
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
				maze[MAZE_COLUMN * ((*playerRow) + 1) + (*playerColumn)].flag = TRUE;
				if(maze[MAZE_COLUMN * ((*playerRow) + 1) + (*playerColumn)].kind != WALL)
				{
					*playerRow += 1;
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
				maze[MAZE_COLUMN * (*playerRow) + ((*playerColumn) - 1)].flag = TRUE;
				if(maze[MAZE_COLUMN * (*playerRow) + ((*playerColumn) - 1)].kind != WALL)
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
				maze[MAZE_COLUMN * (*playerRow) + ((*playerColumn) + 1)].flag = TRUE;
				if(maze[MAZE_COLUMN * (*playerRow) + ((*playerColumn) + 1)].kind != WALL)
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
int MazeGoalCheck(int playerRow, int playerColumn, MazeBlock maze[MAZE_ROW * MAZE_COLUMN])
{
	if(maze[MAZE_COLUMN * playerRow + playerColumn].kind == GOAL)
	{
		printf("ゴール！\n");
		return 1;
	}
	return 0;
}

//迷路ゲーム
void MazeGame(int stage)
{
	//プレイヤー
	MazePlayer player;

	//迷路
	MazeBlock maze[STAGE][MAZE_ROW * MAZE_COLUMN] =
	{
		{ //STAGE0
			{FRAME, TRUE } , {FRAME, TRUE } , {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE },
			{FRAME, TRUE } , {START, TRUE } , {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {FRAME, TRUE },
			{FRAME, TRUE } , {WALL , FALSE} , {WALL , FALSE}, {PATH , FALSE}, {WALL , FALSE}, {WALL , FALSE}, {FRAME, TRUE },
			{FRAME, TRUE } , {WALL , FALSE} , {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {FRAME, TRUE },
			{FRAME, TRUE } , {PATH , FALSE} , {PATH , FALSE}, {WALL , FALSE}, {WALL , FALSE}, {WALL , FALSE}, {FRAME, TRUE },
			{FRAME, TRUE } , {WALL , FALSE} , {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {GOAL , TRUE }, {FRAME, TRUE },
			{FRAME, TRUE } , {FRAME, TRUE } , {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE },
		},
		{ //STAGE1
			{FRAME, TRUE } , {FRAME, TRUE } , {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE },
			{FRAME, TRUE } , {PATH , FALSE} , {WALL , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {FRAME, TRUE },
			{FRAME, TRUE } , {PATH , FALSE} , {WALL , FALSE}, {PATH , FALSE}, {WALL , FALSE}, {WALL , FALSE}, {FRAME, TRUE },
			{FRAME, TRUE } , {START, TRUE } , {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {GOAL , TRUE }, {FRAME, TRUE },
			{FRAME, TRUE } , {PATH , FALSE} , {WALL , FALSE}, {WALL , FALSE}, {WALL , FALSE}, {WALL , FALSE}, {FRAME, TRUE },
			{FRAME, TRUE } , {PATH , FALSE} , {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {PATH , FALSE}, {FRAME, TRUE },
			{FRAME, TRUE } , {FRAME, TRUE } , {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE }, {FRAME, TRUE },
		}
	};

	//プレイヤー初期化（スタートがなかったら終了）
	if(MazePlayerInit(&player.row, &player.column, maze[stage]) == -1)
		return;
	
	while(MazeGoalCheck(player.row, player.column, maze[stage]) != 1) //ゴールするまで繰り返す
	{
		MazeDraw(player.row, player.column, maze[stage]); //迷路表示
		MazePlayerMove(&player.row, &player.column, maze[stage]); //プレイヤー移動
	}

	//迷路最終結果発表
	MazeDraw(player.row, player.column, maze[stage]);
}

//メニュー
enum MazeMenu {STAGE0, STAGE1, EXIT};

//タイトル
int MazeTitle()
{
	char buf[100];
	int menu = -1;

	printf("\n迷路ゲーム\n\n");
	printf("メニュー\n");
	printf("%d : ステージ0\n", STAGE0);
	printf("%d : ステージ1\n", STAGE1);
	printf("%d : 終了\n", EXIT);

	printf("メニュー（番号）を選択してください。: ");
	fgets(buf, sizeof(buf), stdin);
	sscanf(buf, "%d", &menu);

	while(menu < 0 || menu > EXIT)
	{
		printf("入力が不正です。再入力してください。: ");
		fgets(buf, sizeof(buf), stdin);
		sscanf(buf, "%d", &menu);
	}

	return menu;
}

int main(void)
{
	int menu;
	int stage;

	while(1)
	{
		//メニュー選択
		menu = MazeTitle();
		printf("\n");

		if(menu == EXIT) //EXITならwhileを抜ける
			break;
		else
		{
			//ステージ選択
			switch(menu)
			{
				case STAGE0:
					stage = 0;
					break;
				case STAGE1:
					stage = 1;
					break;
			}
		}
		
		//迷路ゲーム
		MazeGame(stage);

		//ゲームが終わるとwhileの先頭に戻る
	}

	return 0;
}