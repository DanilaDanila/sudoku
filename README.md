BUILD:
   g++ main.cpp -std=c++11 -o sudoku
 
RUN:
  ./sudoku    task will be created randomly
  ./sudoku -c file    check sudoku file(correct/incorrect)
  ./sudoku file    solve "file"(all output)
  ./sudoku file -less    solve "fille"; less output
