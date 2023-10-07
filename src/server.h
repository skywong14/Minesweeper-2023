#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <queue>
#include <iostream>
#include <cstring>

/*
 * You may need to define some global variables for the information of the game map here.
 * Although we don't encourage to uss global variables in real cpp projects, you may have to use them because the use of
 * class is not taught yet. However, if you are member of A-class or have learnt the use of cpp class, member functions,
 * etc., you're free to modify this structure.
 */
int rows; // The count of rows of the game map
int columns;  // The count of columns of the game map
int game_state; // The state of the game, 0 for continuing, 1 for winning, -1 for losing

struct Point{
  int x=-1,y=-1;
};
struct GRID{
  int order=0,state=-1,num=0;
  int vis=0;
}mp[100][100];  //state: -1:barrier 0:ok 1:mines 2:visited
int adjacent_x[9],adjacent_y[9];
int mine_num;
int visit_count,step_count;

/**
 * @brief The definition of function InitMap()
 *
 * @details This function is designed to read the initial map from stdin. For example, if there is a 3 * 3 map in which
 * mines are located at (0, 1) and (1, 2) (0-based), the stdin would be
 *     3 3
 *     .X.
 *     ...
 *     ..X
 * where X stands for a mine block and . stands for a normal block. After executing this function, your game map would
 * be initialized, with all the blocks unvisited.
 */
void init_adjacent(){
  adjacent_x[1]=-1;adjacent_x[2]=-1;adjacent_x[3]=-1;adjacent_x[4]=0;
  adjacent_x[5]=0;adjacent_x[6]=1;adjacent_x[7]=1;adjacent_x[8]=1;
  adjacent_y[1]=-1;adjacent_y[2]=0;adjacent_y[3]=1;adjacent_y[4]=-1;
  adjacent_y[5]=1;adjacent_y[6]=-1;adjacent_y[7]=0;adjacent_y[8]=1;
  return;
}
int calc_mines(int pos_x,int pos_y){
  int tmp=0;
  for (int i=1;i<=8;i++)
    if (mp[pos_x+adjacent_x[i]][pos_y+adjacent_y[i]].state==1)
      tmp++;
  return tmp;
}
void InitMap() {
  game_state=0;
  visit_count=0;
  step_count=0;
  mine_num=0;
  init_adjacent();
  std::string s;
  //char s[100];
  std::cin >> rows >> columns;
  for (int i=1;i<=rows;i++){
   // scanf("%s",&s);
    std::cin>>s;
    for (int j=1;j<=columns;j++)
        if (s[j-1]=='.') mp[i][j].state=0;
        else if (s[j-1]=='X') mp[i][j].state=1;
  }
  for (int i=1;i<=rows;i++)
    for (int j=1;j<=columns;j++)
    if (mp[i][j].state==0)
      mp[i][j].num=calc_mines(i,j);
    else mine_num++;
  return;
}

/**
 * @brief The definition of function VisitBlock(int, int)
 *
 * @details This function is designed to visit a block in the game map. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call VisitBlock(0, 0), the return value would be 0 (game continues), and the game map would
 * be
 *     1??
 *     ???
 *     ???
 * If you call VisitBlock(0, 1) after that, the return value would be -1 (game ends and the players loses) , and the
 * game map would be
 *     1X?
 *     ???
 *     ???
 * If you call VisitBlock(0, 2), VisitBlock(2, 0), VisitBlock(1, 2) instead, the return value of the last operation
 * would be 1 (game ends and the player wins), and the game map would be
 *     1@1
 *     122
 *     01@
 *
 * @param row The row coordinate (0-based) of the block to be visited.
 * @param column The column coordinate (0-based) of the block to be visited.
 *
 * @note You should edit the value of game_state in this function. Precisely, edit it to
 *    0  if the game continues after visit that block, or that block has already been visited before.
 *    1  if the game ends and the player wins.
 *    -1 if the game ends and the player loses.
 */
void VisitBlock(int row, int column) {  
  step_count++;
  row++;column++;
  Point p1,p2;p1.x=row;p1.y=column;
  if (mp[p1.x][p1.y].vis!=0) return;
  
  std::queue <Point> q;int Sta;
  while (!q.empty()) q.pop();
  q.push(p1);
  mp[p1.x][p1.y].vis=1;
  while (!q.empty()){
    p1=q.front();q.pop();
    Sta=mp[p1.x][p1.y].state;
    if (Sta==0) {
      visit_count++;
      if (mp[p1.x][p1.y].num==0)
        for (int i=1;i<=8;i++){
          p2.x=p1.x+adjacent_x[i];
          p2.y=p1.y+adjacent_y[i];
          if (mp[p2.x][p2.y].vis==0 && mp[p2.x][p2.y].state!=-1){
            mp[p2.x][p2.y].vis=1;
            q.push(p2);
          }
        }
    }else if (Sta==1){
      game_state=-1;
      return;
    }
  }
  if (visit_count==rows*columns-mine_num) {game_state=1;}
  // TODO (student): Implement me!
}

/**
 * @brief The definition of function PrintMap()
 *
 * @details This function is designed to print the game map to stdout. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call PrintMap(), the stdout would be
 *    ???
 *    ???
 *    ???
 * If you call VisitBlock(2, 0) and PrintMap() after that, the stdout would be
 *    ???
 *    12?
 *    01?
 * If you call VisitBlock(0, 1) and PrintMap() after that, the stdout would be
 *    ?X?
 *    12?
 *    01?
 * If the player visits all blocks without mine and call PrintMap() after that, the stdout would be
 *    1@1
 *    122
 *    01@
 * (You may find the global variable game_state useful when implementing this function.)
 *
 * @note Use std::cout to print the game map, especially when you want to try the advanced task!!!
 */
void PrintMap() {
  for (int i=1;i<=rows;i++){
    for (int j=1;j<=columns;j++){
      if (mp[i][j].vis==0){
        if (game_state==1) std::cout<<'@';
        else std::cout<<'?';
      } 
      else {
        if (mp[i][j].state==1) 
          std::cout<<'X';
        else 
          std::cout<<mp[i][j].num;
      }
    }
    std::cout<<std::endl;
  }
  return;
}

/**
 * @brief The definition of function ExitGame()
 *
 * @details This function is designed to exit the game. 
 * It outputs a line according to the result, and a line of two integers, visit_count and step_count,
 * representing the number of blocks visited and the number of steps taken respectively.
 */
void ExitGame() {
  // TODO (student): Implement me!
  if (game_state==1)
    std::cout<<"YOU WIN!"<<std::endl;
  else
    std::cout<<"GAME OVER!"<<std::endl;
  std::cout<<visit_count<<' '<<step_count<<std::endl;
  exit(0); // Exit the game immediately
}

#endif
