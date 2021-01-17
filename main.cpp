#include <iostream>
#include <Windows.h>
#include <queue>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

#define MAP_SIZE 20
#define BUFFER_SIZE 2
#define INITIAL_LENGTH 4

using namespace std;

template<typename T, typename Container=std::deque<T> >
class iterable_queue : public std::queue<T,Container>
{
public:
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

    iterator begin() { return this->c.begin(); }
    iterator end() { return this->c.end(); }
    const_iterator begin() const { return this->c.begin(); }
    const_iterator end() const { return this->c.end(); }
};

struct Point{
    int x;
    int y;
};
struct Snake{
    int length;
    int dir;
    Point head;
    iterable_queue <Point> body;
};

Snake my_snake;
Point apple;
char error_message[25];
char matrix[MAP_SIZE][MAP_SIZE];
int is_running;

int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

void borderMap(int map_size)
{
    for(int i = 0; i <= map_size; i++)
    {
        matrix[0][i] = '.';
        matrix[map_size][i] = '.';
        matrix[i][0] = '.';
        matrix[i][map_size] = '.';
    }
}

void resetMap(int map_size)
{
    for(int i = 1; i < map_size; i++)
        for(int j = 1; j < map_size; j++)
            matrix[i][j] = ' ';
}

void renderSnake()
{
    matrix[my_snake.head.x][my_snake.head.y] = '@';
    for(auto a : my_snake.body)
        matrix[a.x][a.y] = '<';
}

void renderApple()
{
    matrix[apple.x][apple.y] = '$';
}

void printMap(int map_size)
{
    resetMap(map_size);
    renderSnake();
    renderApple();

    cout << "Your score: " << my_snake.length - INITIAL_LENGTH << "\n";
    for(int i = 0; i <= map_size; i++)
    {
        for(int j = 0; j <= map_size; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
}

void initSnake(int map_size)
{
    my_snake.length = INITIAL_LENGTH;
    my_snake.dir = 3;
    my_snake.head = {map_size / 2, map_size / 2};
    my_snake.body.push({map_size / 2, map_size / 2 + 3});
    my_snake.body.push({map_size / 2, map_size / 2 + 2});
    my_snake.body.push({map_size / 2, map_size / 2 + 1});
}

bool isValid(Point apple)
{
    if(apple.x == my_snake.head.x && apple.y == my_snake.head.y)
        return 0;
    for(auto a : my_snake.body)
    {
        if(apple.x == a.x && apple.y == a.y)
            return 0;
    }
    return 1;
}

void initApple(int map_size)
{
    apple = {rand() % (map_size - 1) + 1, rand() % (map_size - 1) + 1};

   while(!isValid(apple))
        apple = {rand() % (map_size - 1) + 1, rand() % (map_size - 1) + 1};
}

void updateSnake(int map_size)
{
    Point new_head = {my_snake.head.x + dx[my_snake.dir], my_snake.head.y + dy[my_snake.dir]};
    my_snake.body.push(my_snake.head);

    if(new_head.x == apple.x && new_head.y == apple.y)
    {
        my_snake.length++;
        initApple(MAP_SIZE - BUFFER_SIZE);
    }
    else
        my_snake.body.pop();

    my_snake.head = new_head;


    if(new_head.x <= 0 || new_head.x >= map_size || new_head.y <= 0 || new_head.y >= map_size)
    {
        strcpy(error_message, "You hit a wall");
        is_running = false;
    }
    for(auto a : my_snake.body)
    {
        if(a.x == new_head.x && a.y == new_head.y)
        {
            strcpy(error_message, "You hit your own body");
            is_running = false;
        }
    }
}

void listenKeys()
{
    char ch = getch();

    if(ch == 'w')
        my_snake.dir = 0;
    if(ch == 'd')
        my_snake.dir = 1;
    if(ch == 's')
        my_snake.dir = 2;
    if(ch == 'a')
        my_snake.dir = 3;
}

void clearCls()
{
    DWORD n;
    DWORD size;
    COORD coord = {0};
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    GetConsoleScreenBufferInfo ( h, &csbi );
    size = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter ( h, TEXT ( ' ' ), size, coord, &n );
    GetConsoleScreenBufferInfo ( h, &csbi );
    FillConsoleOutputAttribute ( h, csbi.wAttributes, size, coord, &n );
    //SetConsoleCursorPosition ( h, coord );
}

void hideCursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

int main()
{
    srand(time(NULL));

    hideCursor();
    borderMap(MAP_SIZE - BUFFER_SIZE);
    initSnake(MAP_SIZE - BUFFER_SIZE);
    initApple(MAP_SIZE - BUFFER_SIZE);
    printMap(MAP_SIZE - BUFFER_SIZE);

    is_running = true;
    while(is_running)
    {
        listenKeys();
        updateSnake(MAP_SIZE - BUFFER_SIZE);
        Sleep(100);
        clearCls();
        printMap(MAP_SIZE - BUFFER_SIZE);
    }

    if(is_running == false)
    {
        cout << "GAME OVER, YOU SCORED: " << my_snake.length - 4 << "\n";
        cout << error_message;
    }

    return 0;
}
