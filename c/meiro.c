#include <stdio.h>
#include <stdlib.h>

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
int MazePlayerInit(int *playerRow, int *playerColumn, MazeBlock *maze, int mazeRow, int mazeColumn)
{
	int i, j;

	for(i = 0; i < mazeRow; i++)
	{
		for(j = 0; j < mazeColumn; j++)
		{
			if(maze[mazeColumn * i + j].kind == START) //スタート地点ならばプレイヤーの位置に設定
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
void MazeDraw(int playerRow, int playerColumn, MazeBlock *maze, int mazeRow, int mazeColumn)
{
	int i, j;

	for(i = 0; i < mazeRow; i++) //行
	{
		for(j = 0; j < mazeColumn; j++) //列
		{
			if(i == playerRow && j == playerColumn) //プレイヤー位置
				printf("P");
			else if(maze[mazeColumn * i + j].flag == FALSE) //ブロックが判明していない場合
				printf("?");
			else
			{
				switch(maze[mazeColumn * i + j].kind)
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
void MazePlayerMove(int *playerRow, int *playerColumn, MazeBlock *maze, int mazeRow, int mazeColumn)
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
				maze[mazeColumn * ((*playerRow) - 1) + (*playerColumn)].flag = TRUE;
				if(maze[mazeColumn * ((*playerRow) - 1) + (*playerColumn)].kind != WALL) //壁判定
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
			if(*playerRow + 1 < mazeRow - 1)
			{
				maze[mazeColumn * ((*playerRow) + 1) + (*playerColumn)].flag = TRUE;
				if(maze[mazeColumn * ((*playerRow) + 1) + (*playerColumn)].kind != WALL)
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
				maze[mazeColumn * (*playerRow) + ((*playerColumn) - 1)].flag = TRUE;
				if(maze[mazeColumn * (*playerRow) + ((*playerColumn) - 1)].kind != WALL)
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
			if(*playerColumn + 1 < mazeColumn - 1)
			{
				maze[mazeColumn * (*playerRow) + ((*playerColumn) + 1)].flag = TRUE;
				if(maze[mazeColumn * (*playerRow) + ((*playerColumn) + 1)].kind != WALL)
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
int MazeGoalCheck(int playerRow, int playerColumn, MazeBlock *maze, int mazeColumn)
{
	if(maze[mazeColumn * playerRow + playerColumn].kind == GOAL)
	{
		printf("ゴール！\n");
		return 1;
	}
	return 0;
}

//迷路ゲーム
void MazeGame()
{
	char fileName[100];
	char buf[100];
	FILE* fp;
	int i, kind;
	int goalCheck = 0;
	int mazeRow, mazeColumn;

	//プレイヤー
	MazePlayer player;

	//迷路
	MazeBlock *maze;

	//ファイル読み込み
	printf("ファイル名を入力してください。: ");
	fgets(fileName, sizeof(fileName), stdin);

	//改行を終端文字に書き換える
	for(i = 0; i < 100; i++)
	{
		if(fileName[i] == '\n')
			fileName[i] = '\0';
	}

	fp = fopen(fileName, "r");
	if(fp == NULL)
	{
		printf("ファイルが存在しません。\n");
		return;
	}

	//迷路サイズ取得
	if(fgets(buf, sizeof(buf), fp) != NULL) //1行読み込み
	{
		if(sscanf(buf, "%d,%d", &mazeRow, &mazeColumn) != 2)
		{
			//2つの数字を読み込めなかった場合失敗
			printf("サイズが読み込めません。\n");
			fclose(fp);
			return;
		}
	}
	else
	{
		//データがない場合、fgetsが失敗
		printf("データ数がありません。\n");
		fclose(fp);
		return;
	}

	//配列の動的確保
	maze = (MazeBlock *)malloc(sizeof(MazeBlock) * mazeRow * mazeColumn);
	if(maze == NULL)
	{
		printf("メモリの確保に失敗しました。\n");
		fclose(fp);
		return;
	}

	//迷路初期化
	for(i = 0; i < mazeRow * mazeColumn; i++)
	{
		if(fgets(buf, sizeof(buf), fp) != NULL) //1行読み込み
		{
			if(sscanf(buf, "%d", &kind) == 1)
			{
				switch(kind)
				{
					case PATH:
					case WALL:
						maze[i].kind = kind;
						maze[i].flag = FALSE;
						break;
					case GOAL:
						goalCheck = 1;
					case START:
					case FRAME:
						maze[i].kind = kind;
						maze[i].flag = TRUE;
						break;
					default:
						//ファイルのデータが正しい数値ではなかった場合
						printf("不正なデータが存在します。\n");
						fclose(fp);
						free(maze);
						return;
						break;
				}
			}
			else
			{
				//数字でなかった場合、sscanfが失敗
				printf("不正なデータが存在します。\n");
				fclose(fp);
				free(maze);
				return;
			}
		}
		else
		{
			//データが途中で終わった場合、fgetsが失敗
			printf("データ数が足りません。\n");
			fclose(fp);
			free(maze);
			return;
		}
	}
	fclose(fp);

	//ゴールが存在するか確認
	if(goalCheck == 0)
	{
		printf("ゴールが存在しません。\n");
		free(maze);
		return;
	}



	//プレイヤー初期化（スタートがなかったら終了）
	if(MazePlayerInit(&player.row, &player.column, maze, mazeRow, mazeColumn) == -1)
		return;
	
	while(MazeGoalCheck(player.row, player.column, maze, mazeColumn) != 1) //ゴールするまで繰り返す
	{
		MazeDraw(player.row, player.column, maze, mazeRow, mazeColumn); //迷路表示
		MazePlayerMove(&player.row, &player.column, maze, mazeRow, mazeColumn); //プレイヤー移動
	}

	//迷路最終結果発表
	MazeDraw(player.row, player.column, maze, mazeRow, mazeColumn);

	free(maze);
}

//メニュー
enum MazeMenu {GAMESTART, EXIT};

//タイトル
int MazeTitle()
{
	char buf[100];
	int menu = -1;

	printf("\n迷路ゲーム\n\n");
	printf("メニュー\n");
	printf("%d : ゲーム開始\n", GAMESTART);
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

	while(1)
	{
		//メニュー選択
		menu = MazeTitle();
		printf("\n");

		if(menu == EXIT) //EXITならwhileを抜ける
			break;
		
		//迷路ゲーム
		MazeGame();

		//ゲームが終わるとwhileの先頭に戻る
	}

	return 0;
}