#include <iostream>
#include <sstream>
#include <windows.h>
#include <ctime>
#include <conio.h>

using namespace std;

const int FPS_LIMIT = 100;

enum ItemsSymbols {
	SNAKE_BODY = '*',
	SNAKE_HEAD = 'o',
	APPLE = 'a',
};

enum Keys {
	UP_KEY = 'w',
	DOWN_KEY = 's',
	LEFT_KEY = 'a',
	RIGHT_KEY = 'd',
	EXIT_KEY = 'q'
};

enum Colors {
	THEME_COLOR = 4,
	SNAKE_COLOR = 10,
	APPLE_COLOR = 12,
};

enum WORKSPACE_SIZES {
	WINDOW_XPOS = 50,
	WINDOW_YPOS = 50,
	WINDOW_WIDTH = 60,
	WINDOW_HEIGHT = 60,
	COLOMS = 41,
	LINES = 21
};

Keys getOppositKey(char directionKey) {
	switch (directionKey)
	{
	case UP_KEY:
		return DOWN_KEY;
		break;
	case DOWN_KEY:
		return UP_KEY;
		break;
	case LEFT_KEY:
		return RIGHT_KEY;
		break;
	case RIGHT_KEY:
		return LEFT_KEY;
		break;
	default:
		return EXIT_KEY;
		break;
	}
}

int main()
{
	srand(5);
	rand();

	system("mode con cols=41 lines=41");

	MoveWindow(GetConsoleWindow(), WINDOW_XPOS, WINDOW_YPOS, WINDOW_WIDTH, WINDOW_HEIGHT, true);

	const int width = COLOMS-1, height = LINES-1;
	const int max_length = 50;

	int array_X[max_length], array_Y[max_length];
	int length = 1;
	array_X[0] = width / 2; array_Y[0] = height / 2;
	int dx = 1, dy = 0;
	int X_apple, Y_apple;

	do 
	{
		X_apple = rand() % (width - 2) + 1;
		Y_apple = rand() % (height - 2) + 1;
	} while (X_apple != array_X[length - 1] && Y_apple != array_Y[length - 1]);

	int sleep_time = FPS_LIMIT;

	COORD c;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci = { sizeof(cci), false };
	SetConsoleCursorInfo(h, &cci);

	SetConsoleTextAttribute(h, THEME_COLOR); 

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			char s;
			if (x == 0 && y == 0) 
				s = 218;
			else if (x == 0 && y == height - 1)
				s = 192;
			else if (y == 0 && x == width - 1)
				s = 191;
			else if (y == height - 1 && x == width - 1)
				s = 217;
			else if (y == 0 || y == height - 1)
				s = 196;
			else if (x == 0 || x == width - 1)
				s = 179;
			else s = ' ';
			putchar(s);
		}

		std::cout << endl;
	}

	c.X = X_apple;
	c.Y = Y_apple;
	SetConsoleCursorPosition(h, c);
	SetConsoleTextAttribute(h, APPLE_COLOR);
	putchar(APPLE);

	c.X = array_X[0];
	c.Y = array_Y[0];
	SetConsoleCursorPosition(h, c);
	SetConsoleTextAttribute(h, SNAKE_COLOR);
	putchar(SNAKE_HEAD);

	bool flag = true;
	
	char lastKey = EXIT_KEY;
	do 
	{
		Sleep(sleep_time);

		if (_kbhit())
		{
			char k = _getch();
			
			if (k && (lastKey != getOppositKey(k)) ) {
				lastKey = k;

				switch (k) 
				{
				case DOWN_KEY:
					dy = 1;
					dx = 0; 
					break;
				case UP_KEY: 
					dy = -1; 
					dx = 0;
					break;
				case LEFT_KEY:
					dy = 0;
					dx = -1;
					break;
				case RIGHT_KEY:
					dy = 0;
					dx = 1;
					break;
				case EXIT_KEY:
					flag = false;
					break;
				}
			}
		}

		int X = array_X[length - 1] + dx;
		int Y = array_Y[length - 1] + dy;
		if (X == 0 || X == width - 1 || Y == 0 || Y == height - 1)
		{
			flag = false;
		}
		else if (X == X_apple && Y == Y_apple) 
		{
			c.X = array_X[length - 1];
			c.Y = array_Y[length - 1];
			SetConsoleCursorPosition(h, c);
			putchar(SNAKE_BODY);

			length++;
			c.X = array_X[length - 1] = X;
			c.Y = array_Y[length - 1] = Y;
			SetConsoleCursorPosition(h, c);
			putchar(SNAKE_HEAD);

			if (length == max_length)
			{
				break;
			}

			int i;
			do
			{
				X_apple = rand() % (width - 2) + 1;
				Y_apple = rand() % (height - 2) + 1;
				i = 0;
				for (; i < length; i++)
					if (X_apple == array_X[i] && Y_apple == array_Y[i])
						break;
			} while (i < length);

			c.X = X_apple;
			c.Y = Y_apple;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, APPLE_COLOR);
			putchar(APPLE);
			SetConsoleTextAttribute(h, SNAKE_COLOR);
		}
		else
		{
			int i = 1;
			for (; i < length; i++)
				if (X == array_X[i] && Y == array_Y[i])
					break;
			if (i < length)
			{
				break;
			}
			else
			{
				c.X = array_X[0];
				c.Y = array_Y[0];
				SetConsoleCursorPosition(h, c);
				putchar(' ');

				if (length > 1)
				{
					c.X = array_X[length - 1];
					c.Y = array_Y[length - 1];
					SetConsoleCursorPosition(h, c);
					putchar(SNAKE_BODY);
				}

				for (int i = 0; i < length - 1; i++)
				{
					array_X[i] = array_X[i + 1];
					array_Y[i] = array_Y[i + 1];
				}

				c.X = array_X[length - 1] = X;
				c.Y = array_Y[length - 1] = Y;
				SetConsoleCursorPosition(h, c);
				putchar(SNAKE_HEAD);
			}
		}
	} while (flag);

	system("cls");
	SetConsoleTextAttribute(h, THEME_COLOR);
	std::cout << "GAME OVER\n";
	system("pause");
	return 0;
}