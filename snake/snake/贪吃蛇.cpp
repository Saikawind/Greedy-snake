#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <process.h>
#include <stdio.h>


#define Max 100		//蛇的最大节数

#define	up 72
#define	down 80
#define left 75
#define	right 77

typedef struct pointxy
{
	int x;
	int y;
}pointxy;

//蛇
struct Snake
{
	int lenth;			//蛇的节数
	pointxy xy[Max];	//蛇最大长度
	char position;		//标记方向
	int speed;			//蛇的速度
}snake;


//食物
struct Food
{
	pointxy foodxy;		//食物坐标
	int symbol;		//食物存在标志
	int grade;			//分数
}Food;

enum positon { sright, sleft, sup, sdown, paus}; //把方向判断枚举

/*所用函数声明*/

void CreateMap();			//初始化地图
void createsnake();			//初始化蛇
void drawsnake();			//蛇的绘制
void SnakeMove();			//蛇的移动
void KeyCommunity();		//键盘交互功能
void CreateFood();			//初始化食物
void drawfood();			//绘制食物
void gameover();			//停止游戏
void topic();				//生成封面
void grade();				//显示分数
void theme();				//游戏剧本和难度更替
void GameEnd();				//游戏结束界面
void HitWall();				//判断是否撞墙
void EatFood();				//吃食物
void regame();				//重新开始游戏

int main()
{
	initgraph(800, 650);
	topic();				//显示封面
	snake.speed = 200;
	setbkcolor(BLACK);
	cleardevice();
	createsnake();
	while (1)
	{
		cleardevice();		//刷新蛇的路径
		CreateMap();
		grade();
		drawsnake();
		drawfood();
		theme();
		SnakeMove();
		if (Food.symbol == 0)
		{
			CreateFood();
		}
		Sleep(snake.speed);			//控制蛇的速度
		HitWall();
		EatFood();
	}

	_getch();
	closegraph();
	return 0;

}
//绘画地图
void CreateMap()
{
	setlinecolor(COLORREF RGB(11, 191, 206));
	rectangle(90, 90, 510, 610);
	rectangle(100, 100, 500, 600);
}

//初始化蛇
void createsnake()
{
	snake.xy[2].x = 100;
	snake.xy[2].y = 100;

	snake.xy[1].x = 110;
	snake.xy[1].y = 100;

	snake.xy[0].x = 120;
	snake.xy[0].y = 100;

	snake.lenth = 3;
	snake.position = sright;

	Food.symbol = 0;

}

//蛇的绘制(与蛇的运动函数结合实现蛇的移动)
void drawsnake()
{
	int i;
	for (i = 0; i < snake.lenth; i++)
	{
		setlinecolor(LIGHTCYAN);
		fillrectangle(snake.xy[i].x, snake.xy[i].y, snake.xy[i].x + 10, snake.xy[i].y + 10);
	}

}

//蛇的运动
void SnakeMove()
{
	int i, j;
	//除了第一节以外,后一节蛇身的坐标是前一节的坐标
	for (i = snake.lenth - 1; i > 0; i--)
	{
		snake.xy[i].x = snake.xy[i - 1].x;
		snake.xy[i].y = snake.xy[i - 1].y;
	}
	//蛇头运动方向,根据方向标志来移动
	switch (snake.position)
	{
	case sup:
		snake.xy[0].y -= 10;
		break;
	case sdown:
		snake.xy[0].y += 10;
		break;
	case sright:
		snake.xy[0].x += 10;
		break;
	case sleft:
		snake.xy[0].x -= 10;
		break;
	case paus:
		_getch();
		break;
	}
	while (_kbhit())
	{
		KeyCommunity();
	}
	//判断蛇撞到自己时
	//当蛇头坐标等于蛇身时,撞到自己
	for (j = 2; j < snake.lenth; j++)
	{
		if (snake.xy[0].x == snake.xy[j].x && snake.xy[0].y == snake.xy[j].y)
			gameover();
	}
}


//判断蛇是否撞墙
void HitWall()
{
	if (snake.xy[0].x > 490 || snake.xy[0].x < 100 || snake.xy[0].y > 590 || snake.xy[0].y < 100)
	{
		gameover();		//撞墙时,游戏结束
	}
}

//蛇吃食物功能
void EatFood()
{
	if (snake.xy[0].x == Food.foodxy.x && snake.xy[0].y == Food.foodxy.y)
	{
		snake.lenth++;
		Food.grade += 10;
		Food.symbol = 0;
	}
}

//按键交互功能
void KeyCommunity()
{
	char key = _getch();

	switch (key)
	{
	case up:
		//实现蛇的移动逻辑判断,不能往相反的方向移动
		if (snake.position != sdown)
			snake.position = sup;
		break;
	case down:
		if (snake.position != sup)
			snake.position = sdown;
		break;
	case left:
		if (snake.position != sright)
			snake.position = sleft;
		break;
	case right:
		if (snake.position != sleft)
			snake.position = sright;
		break;
	case 'p':
			snake.position = paus;
			break;
	}		
}

//初始化食物
void CreateFood()
{
	int i, x, y;
	srand((unsigned int)time(NULL));
	x = rand() % 40 + 10;	//初始化食物坐标,要为10的倍数,否则无法对齐
	y = rand() % 50 + 10;
	Food.foodxy.x = x * 10;
	Food.foodxy.y = y * 10;
	Food.symbol = 1;

	//食物不与蛇身重叠
	for (i = 0; i < snake.lenth; i++)
	{
		if (Food.foodxy.x == snake.xy[i].x && Food.foodxy.y == snake.xy[i].y)
		{
			x = rand() % 40 + 10;	//重新生成一次食物
			y = rand() % 50 + 10;
			Food.foodxy.x = x * 10;
			Food.foodxy.y = y * 10;
		}
	}
}

//绘制食物
void drawfood()
{
	setfillcolor(COLORREF RGB(255, 0, 128));
	solidrectangle(Food.foodxy.x, Food.foodxy.y, Food.foodxy.x + 10, Food.foodxy.y + 10);
}

//显示分数

void grade()
{
	char player_grade[] = "当前分数";
	char player_speed[] = "当前移速";
	char grades[100];
	char speed[50];
	sprintf(grades, "%d", Food.grade);
	sprintf(speed, "%d", snake.speed);
	outtextxy(530, 100, player_grade);	//在右上角界面显示分数
	outtextxy(530, 400, player_speed);	//在右下角界面显示速度
	outtextxy(530, 200, "操作说明");
	outtextxy(530, 225, "使用方向键操作");
	outtextxy(530, 250, "p键暂停,按其他键继续");
	outtextxy(635, 100, grades);
	outtextxy(635, 400, speed);

}

//根据分数递进剧情并提高难度
void theme()
{
	char type[15][20] = { "暗影龙", "青绿龙", "唤潮龙", "加仑炎龙", "剧毒龙", "韩金龙", "符文龙", "彩虹龙", "错误龙", "代码龙", "天空龙", "东方龙", "时空龙", "真正的龙", "[error:数据删除]" };
	char topic[15][200] = { "你现在只是一个灵体，飘荡在虚无中。", "你还有一个更常用的名字，毛毛虫。", "你只是在水池里反复拍打着水花。", "你的行动总是以一个球状物体命中一张卡牌作为开始。", "你的体内蕴含着大量的毒素，准确来说，你是一条毒蛇。", "你的走位稳中带皮，似上非上，丝毫不慌。", "你开始注意到自己身上奇异的颜色了。", "你身上的颜色变得越发亮丽了，你想试着一探究竟。", "你发现自己变成了蓝色，你以为这是你所在的世界出现了故障。", "你偷偷进入了世界的隐藏区域，修改了自己的身份。", "你省略了冗长的进化步骤，你现在在天空中自由的翱翔，像个真正的龙那样。", "你重新修改了整个世界，现在他们将你视作神明，视作权利的象征。", "你厌倦了先前的生活，你走遍了所有的时间线，只为从中汲取哪怕一丝快乐。", "你终于成为了你所想要成为的存在，但这有什么意义呢，你这样问到自己。", "你不再想要进化，你明白自己无论如何都只是一个程序，你删除了关于自己的一切，你现在只想静静地，等待末日的到来。", };
	switch (Food.grade / 10)
	{
	case 0:	outtextxy(150, 30, type[0]);
		outtextxy(150, 55, topic[0]);
		setfillcolor(LIGHTGRAY);			//实现给进化的龙换色
		break;
	case 1: outtextxy(150, 30, type[1]);
		outtextxy(150, 55, topic[1]);
		setfillcolor(COLORREF RGB(114, 233, 200));
		snake.speed = 120;
		break;
	case 2: outtextxy(150, 30, type[2]);
		outtextxy(150, 55, topic[2]);
		setfillcolor(COLORREF RGB(39, 247, 252));
		snake.speed = 120;
		break;
	case 3: outtextxy(150, 30, type[3]);
		outtextxy(150, 55, topic[3]);
		setfillcolor(COLORREF RGB(252, 44, 60));
		snake.speed = 110;
		break;
	case 4: outtextxy(150, 30, type[4]);
		outtextxy(150, 55, topic[4]);
		setfillcolor(COLORREF RGB(173, 200, 91));
		snake.speed = 110;
		break;
	case 5: outtextxy(150, 30, type[5]);
		outtextxy(150, 55, topic[5]);
		setfillcolor(COLORREF RGB(231, 240, 51));
		snake.speed = 100;
		break;
	case 6: outtextxy(150, 30, type[6]);
		outtextxy(150, 55, topic[6]);
		setfillcolor(COLORREF RGB(105, 182, 186));
		snake.speed = 100;
		break;
	case 7: outtextxy(150, 30, type[7]);
		outtextxy(150, 55, topic[7]);

		snake.speed = 100;
		break;
	case 8: outtextxy(150, 30, type[8]);
		outtextxy(150, 55, topic[8]);
		setfillcolor(COLORREF RGB(254, 254, 254));
		snake.speed = 90;
		break;
	case 9: outtextxy(150, 30, type[9]);
		outtextxy(150, 55, topic[9]);
		setfillcolor(COLORREF RGB(72, 251, 90));
		snake.speed = 90;
		break;
	case 10:outtextxy(150, 30, type[10]);
		outtextxy(150, 55, topic[10]);
		snake.speed = 85;
		break;
	case 11:outtextxy(150, 30, type[11]);
		outtextxy(150, 55, topic[11]);
		snake.speed = 80;
		break;
	case 12:outtextxy(150, 30, type[12]);
		outtextxy(150, 55, topic[12]);
		snake.speed = 70;
		break;
	case 13:outtextxy(150, 30, type[13]);
		outtextxy(150, 55, topic[13]);
		snake.speed = 70;
		break;
	case 14:outtextxy(150, 30, type[14]);
		outtextxy(150, 55, topic[14]);
		snake.speed = 60;
		break;
	case 15:outtextxy(150, 30, type[15]);
		outtextxy(150, 55, topic[15]);
		snake.speed = 60;
		break;
	}

}


//游戏结束
void gameover()
{
	settextstyle(25, 0, "楷体");
	char end[] = "你心怀不甘,";
	char end1[] = "也许下一次能做的更好。";
	char tips[] = "按任意键继续";
	outtextxy(530, 200, end);
	outtextxy(530, 250, tips);
	outtextxy(530, 225, end1);
	_getch();
	GameEnd();
}

//显示封面及设置字体
void topic()
{
	char topic[] = "贪吃龙物语";
	char choice[2][50] = { "1.游戏模式(未完成)", "2.游戏说明(未完成)" };
	settextstyle(80, 0, "华文琥珀");
	settextcolor(COLORREF RGB(134, 235, 246));
	outtextxy(200, 125, topic);
	settextstyle(25, 0, "楷体");
	settextcolor(COLORREF RGB(144, 236, 194));
	outtextxy(150, 285, choice[0]);
	outtextxy(550, 285, choice[1]);
	_getch();
}

//游戏结束界面
void GameEnd()
{
	cleardevice();
	char choose;
	char End[] = "世界线收束失效";
	char reburn[] = "是否链接下一世界线？(y || n)";
	settextstyle(40, 0, "微软雅黑");
	settextcolor(COLORREF RGB(134, 235, 246));
	outtextxy(100, 125, End);
	outtextxy(100, 175, reburn);
	settextstyle(25, 0, "楷体");
	settextcolor(COLORREF RGB(144, 236, 194));
	_getch();
	choose = _getch();
	switch (choose)
	{
	case 'y': regame();
	case 'n': exit(0);
	}
	_getch();
	exit(0);
}

void regame()				//重新开始游戏
{
	snake.speed = 200;
	setbkcolor(BLACK);
	cleardevice();
	createsnake();
	while (1)
	{
		cleardevice();		//刷新蛇的路径
		CreateMap();
		grade();
		drawsnake();
		drawfood();
		theme();
		SnakeMove();
		if (Food.symbol == 0)
		{
			CreateFood();
		}
		Sleep(snake.speed);			//控制蛇的速度
		HitWall();
		EatFood();
	}

	_getch();

}