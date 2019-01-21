#include <iostream>
#include <fstream>

void loadTaskFromFile(unsigned int **&task, const char *file_name)
{

}

void drawTask(unsigned int **task)
{
    std::cout<<"/–––––––––––––––––\\";
    for(int i=0; i<9; i++)
    {
        std::cout<<"\n|"<<task[i][0]
                 <<" "<<task[i][1]
                 <<" "<<task[i][2]
                 <<"|"<<task[i][3]
                 <<" "<<task[i][4]
                 <<" "<<task[i][5]
                 <<"|"<<task[i][6]
                 <<" "<<task[i][7]
                 <<" "<<task[i][8]
                 <<"|";
        if((i+1)%3 == 0) std::cout<<"\n|–––––––––––––––––|";
    }
    std::cout<<"\r"<<"\\–––––––––––––––––/\n";
}

int main(int argc, const char **argv)
{
    unsigned int **task;
    task = new unsigned int*[9];
    for(int i=0; i<9; i++)
        task[i] = new unsigned int [9];

    if(argc > 1) loadTaskFromFile(task, argv[1]);
    else
    {
        std::cout<<"No task\n";

        for(int i=0; i<9; i++)
            for(int j=0; j<9; j++)
                task[i][j] = rand()%9;
    }

    drawTask(task);
    return 0;
}