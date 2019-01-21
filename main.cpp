#include <iostream>
#include <fstream>
#include <time.h>
#include <unistd.h>

void loadTaskFromFile(unsigned int **&task, const char *file_name)
{
    std::fstream f;
    f.open(file_name, std::ios::in);

    for(int i=0; i<9; i++)
    {
        std::string line;
        f>>line;
        for(int j=0; j<9; j++)
            task[i][j] = int(line[j]-'0');
    }

    f.close();
}

char getSudokuSymbol(unsigned int **task, int i, int j)
{
    return (task[i][j]>0)?(task[i][j] + '0'):' ';
}

void drawTask(unsigned int **task)
{
    std::cout<<"/–––––––––––––––––\\";
    for(int i=0; i<9; i++)
    {
        std::cout<<"\n|"<<getSudokuSymbol(task, i, 0)
                 <<" "<<getSudokuSymbol(task, i, 1)
                 <<" "<<getSudokuSymbol(task, i, 2)
                 <<"|"<<getSudokuSymbol(task, i, 3)
                 <<" "<<getSudokuSymbol(task, i, 4)
                 <<" "<<getSudokuSymbol(task, i, 5)
                 <<"|"<<getSudokuSymbol(task, i, 6)
                 <<" "<<getSudokuSymbol(task, i, 7)
                 <<" "<<getSudokuSymbol(task, i, 8)
                 <<"|";
        if((i+1)%3 == 0) std::cout<<"\n|–––––+–––––+–––––|";
    }
    std::cout<<"\r"<<"\\–––––––––––––––––/\n";
}

bool tryInsert(unsigned int **task, int x, int y, int c)
{
    for(int i=0; i<9; i++)
        if((task[i][y] == c) or (task[x][i] == c)) return false;

    int square_x = x/3*3;
    int square_y = y/3*3;
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            if(task[square_x+i][square_y+j] == c) return c;

    return true;
}

bool isResolved(unsigned int **task)
{
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
            if(task[i][j] == 0) return false;
    return true;
}

bool isNotResolving(unsigned int **task)
{
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
        {
            if(task[i][j] != 0) continue;

            bool not_resolving = true;

            for(int c=1; c<=9; c++)
                not_resolving = not_resolving and !tryInsert(task, i, j, c);
            if(not_resolving)
                return true;
        }

    return false;
}

struct taskStack
{
    unsigned int **task;
    int x;
    int y;
    int c;

    taskStack *back = nullptr;
};

taskStack *stack = new taskStack;

void pushTask(unsigned int **task, int x, int y, int c)
{
    stack->task = new unsigned int*[9];

    for(int i=0; i<9; i++)
    {
        stack->task[i] = new unsigned int[9];

        for(int j=0; j<9; j++)
            stack->task[i][j] = task[i][j];
    }

    stack->x = x;
    stack->y = y;
    stack->c = c;

    taskStack *new_stack = new taskStack;
    new_stack->back = stack;
    stack = new_stack;
}

bool popTask(unsigned int **&task, int &x, int &y, int &c)
{
    if(stack->back == nullptr) return false;

    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
            task[i][j] = stack->back->task[i][j];

    x = stack->back->x;
    y = stack->back->y;
    c = stack->back->c;

    task[x][y] = 0;

    stack = stack->back;

    return true;
}

bool isRootStack()
{
    return stack->back == nullptr;
}

bool setSmValue(unsigned int **&task, int &x, int &y, int &c)
{
    c++;
    for(int i=x; i<9; i++)
        for(int j=y; j<9; j++)
            if(task[i][j] == 0)
                for(int k=c; k<=9; k++)
                    if(tryInsert(task, i, j, k))
                    {
                        x = i;
                        y = j;
                        c = k;
                        task[x][y] = c;
                        return true;
                    }
    return false;
}

bool checkCorrect(unsigned int **task)
{
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
        {
            if(task[i][j] == 0) continue;
            int sq_x = i/3*3;
            int sq_y = j/3*3;

            for(int k=0; k<9; k++)
                if(((task[i][j] == task[i][k]) && (k!=j)) || ((k!=i) && (task[i][j] == task[k][j]))) return false;

            for(int dx = 0; dx<3; dx++)
                for(int dy = 0; dy<3; dy++)
                    if((dx+sq_x != i) && (dy+sq_y != j) && (task[dx+sq_x][dy+sq_y] != 0))
                        if(task[dx+sq_x][dy+sq_y] == task[i][j]) return false;
        }
    return true;
}

int main(int argc, const char **argv)
{   
    unsigned int **task;
    task = new unsigned int*[9];
    for(int i=0; i<9; i++)
        task[i] = new unsigned int [9];

    if((argc > 1) && (argv[1][0] == '-') && (argv[1][1] == 'c'))
    {
        loadTaskFromFile(task, argv[2]);
        std::cout<<checkCorrect(task)<<"\n";
        drawTask(task);
        return 1;
    }

    if(argc > 1) loadTaskFromFile(task, argv[1]);
    else
    {
        std::cout<<"No task\n";
        std::cout<<"Task will be created randomly(may have not solution)\n";

        for(int i=0; i<9; i++)
            for(int j=0; j<9; j++)
                task[i][j] = rand()%9;
    }

    std::cout<<"TASK:\n";
    drawTask(task);

    int x=0, y=0, c=0;

    while(!isResolved(task))
    {
        bool dead_loop = true;

        for(int i=0; i<9; i++)
            for(int j=0; j<9; j++)
            {
                if(task[i][j] != 0) continue;

                bool single_variant = false;
                unsigned int variant = 10;
                for(unsigned int c=1; c<=9; c++)
                    if(tryInsert(task, i, j, c))
                    {
                        if(variant == 10)
                        {
                            variant = c;
                            single_variant = true;
                        }
                        else
                            single_variant = false;
                    }

                if(single_variant)
                {
                    task[i][j] = variant;
                    dead_loop = false;
                }
        }

        if(dead_loop)
        {   
            if(isNotResolving(task))
            {
                if(isRootStack())
                    {
                        std::cout<<"IMPOSSIBLE!\n";
                        return 0;
                    }
                    else
                    {
                        popTask(task, x, y, c);
                    }
            }
            else
            {
                bool result = setSmValue(task, x, y, c);

                if(result)
                {
                    pushTask(task, x, y, c);
                    x=y=c=0;
                }
                else
                {
                    if(isRootStack())
                    {
                        std::cout<<"IMPOSSIBLE!\n";
                        return 0;
                    }
                    else
                    {
                        popTask(task, x, y, c);
                    }
                }
            }
        }

        drawTask(task);
    }

    drawTask(task);
    std::cout<<"DONE!\n";

    return 0;
}