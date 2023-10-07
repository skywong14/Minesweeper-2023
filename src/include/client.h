#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <utility>
#include <queue>
#include <cstring>

extern int rows;     // The count of rows of the game map
extern int columns;  // The count of columns of the game map

// You can not use any other external variables except for rows and columns.
struct Point2{
  int x=-1,y=-1;
};
struct GRID2{
  int legal;
  int state=-1; //-1:unknown 0:blank 1:mine
  int num=0;
  int vis=0;//1:show 0:hidden
}mp2[100][100];


int adjacent2_x[9],adjacent2_y[9];
void init_adjacent2(){
  adjacent2_x[1]=-1;adjacent2_x[2]=-1;adjacent2_x[3]=-1;adjacent2_x[4]=0;
  adjacent2_x[5]=0;adjacent2_x[6]=1;adjacent2_x[7]=1;adjacent2_x[8]=1;
  adjacent2_y[1]=-1;adjacent2_y[2]=0;adjacent2_y[3]=1;adjacent2_y[4]=-1;
  adjacent2_y[5]=1;adjacent2_y[6]=-1;adjacent2_y[7]=0;adjacent2_y[8]=1;
  return;
}
int calc_state(int pos_x,int pos_y,int State){
  int tmp=0;
  for (int i=1;i<=8;i++)
    if (mp2[pos_x+adjacent2_x[i]][pos_y+adjacent2_y[i]].state==State &&
        mp2[pos_x+adjacent2_x[i]][pos_y+adjacent2_y[i]].legal==1)
      tmp++;
  return tmp;
}

void Execute(int row, int column);

void init_legal(){
  for (int i=1;i<=rows;i++)
    for (int j=1;j<=columns;j++)
      mp2[i][j].legal=1;
}
void InitGame() {
  init_legal();
  init_adjacent2();
  int first_row, first_column;
  std::cin >> first_row >> first_column;
  Execute(first_row, first_column);
}

void ReadMap() {
  std::string s;
  for (int i=1;i<=rows;i++){
    std::cin>>s;
    for (int j=1;j<=columns;j++)
      if (s[j-1]=='X') {mp2[i][j].state=1; mp2[i][j].vis=1;}
      else if (s[j-1]!='?'){
        mp2[i][j].state=0;
        mp2[i][j].vis=1;
        mp2[i][j].num=(int)s[j-1]-(int)'0';
      }
  }
  return;
}

/**
 * @brief The definition of function Decide()
 *
 * @details This function is designed to decide the next step when playing the client's (or player's) role. Open up your
 * mind and make your decision here!
 */
 Point2 get_ans(){  
  Point2 p;p.x=-1;p.y=-1;
  for (int i=1;i<=rows;i++)
    for (int j=1;j<=columns;j++)
    if (mp2[i][j].state==0 && mp2[i][j].vis==0){
      p.x=i;p.y=j;
      break;
    }
  return p;
 }  //able to update!!!
void update1(){
  int flag=1,tmp1,tmp2,tmp3,tmpx,tmpy;
  while (flag){
    flag=0;
    for (int i=1;i<=rows;i++)
      for (int j=1;j<=columns;j++)
      if (mp2[i][j].vis==1 && mp2[i][j].state==0){
        tmp1=calc_state(i,j,-1);
        if (tmp1==0) continue;
        tmp2=calc_state(i,j,0);
        tmp3=calc_state(i,j,1);
        if (tmp1+tmp3==mp2[i][j].num){
          flag=1;
          for (int k=1;k<=8;k++){
            tmpx=i+adjacent2_x[k]; tmpy=j+adjacent2_y[k];
            if (mp2[tmpx][tmpy].legal==1 && mp2[tmpx][tmpy].state==-1)
              mp2[tmpx][tmpy].state=1;
          }
        }
        if (tmp3==mp2[i][j].num){
          flag=1;
          for (int k=1;k<=8;k++){
            tmpx=i+adjacent2_x[k]; tmpy=j+adjacent2_y[k];
            if (mp2[tmpx][tmpy].legal==1 && mp2[tmpx][tmpy].state==-1)
              mp2[tmpx][tmpy].state=0;
          }
        }
      }
  }
}


int num_border;
int border_size[1000];
int besides[100][100],belong[100][100];
Point2 arr[1000];int arr_state[1000];
int state_trial[100][100];
Point2 border[1000][1000];
struct GRID3{
  int legal;//0:illegal 1:legal_to_try
  int num0,num1;//0:times of blank;1:times of mine;
  int definite; //0:perhaps 1:definite
  int vis; //0:unvisited 1:visited;
}trial[100][100];
void init_besides(){
  memset(besides,0,sizeof(besides));
  int tmpx,tmpy;
  for (int xx=1;xx<=rows;xx++)
    for (int yy=1;yy<=rows;yy++)
    if (mp2[xx][yy].vis==1 && mp2[xx][yy].state==0)
      for (int i=1;i<=8;i++){
       tmpx=xx+adjacent2_x[i];tmpy=yy+adjacent2_y[i];
       if (mp2[tmpx][tmpy].vis==0 && mp2[tmpx][tmpy].state==-1)
        besides[tmpx][tmpy]++;
    }
  return;
}
void get_a_border(int pos_x,int pos_y,int CNT,int NUM){
  int tmpx,tmpy;Point2 p1;
  for (int i=1;i<=8;i++){
    tmpx=pos_x+adjacent2_x[i];tmpy=pos_y+adjacent2_y[i];
    if (belong[tmpx][tmpy]==(tmpx-1)*columns+tmpy && besides[tmpx][tmpy]>0){
      belong[tmpx][tmpy]=NUM;
      border_size[CNT]++;
      p1.x=tmpx;p1.y=tmpy;
      border[num_border][border_size[CNT]]=p1;
      get_a_border(tmpx,tmpy,CNT,NUM);
    }
  }
}
void get_borders(){
  for (int i=1;i<=rows;i++)
    for (int j=1;j<=columns;j++){
      if (mp2[i][j].state==-1) belong[i][j]=(i-1)*columns+j;
      else belong[i][j]=-1;
    }
  Point2 p1;
  for (int i=1;i<=rows;i++)
    for (int j=1;j<=columns;j++)
    if (belong[i][j]==(i-1)*columns+j && besides[i][j]>0){
 //     std::cout<<i<<','<<j<<';';
      num_border++;
      border_size[num_border]=1;
      p1.x=i;p1.y=j;
      border[num_border][1]=p1;
      belong[i][j]=-1;
      get_a_border(i,j,num_border,(i-1)*columns+j);
      belong[i][j]=(i-1)*columns+j;
    } 
}
void update_trial(int MAXCNT){
  for (int i=1;i<=MAXCNT;i++){
    if (arr_state[i]==0)
      trial[arr[i].x][arr[i].y].num0++;
    else
      trial[arr[i].x][arr[i].y].num1++;
  }
  
  return;
}
void search_trial(int k,int MAXCNT){
  if (k>MAXCNT){
    update_trial(MAXCNT);
    return;
  }
  int tmpx,tmpy,tmpx2,tmpy2,tmpnum,tmpnum2;
  for (int STATE=0;STATE<=1;STATE++){
    arr_state[k]=STATE;
    state_trial[arr[k].x][arr[k].y]=STATE;
    int flag=1;
    for (int i=1;i<=8;i++){
      tmpx=arr[k].x+adjacent2_x[i];
      tmpy=arr[k].y+adjacent2_y[i];
      if (mp2[tmpx][tmpy].legal!=1 || mp2[tmpx][tmpy].state!=0 || mp2[tmpx][tmpy].vis!=1) continue;
      tmpnum=0;tmpnum2=0;
      for (int j=1;j<=8;j++){
        tmpx2=tmpx+adjacent2_x[j];
        tmpy2=tmpy+adjacent2_y[j];
        if (mp2[tmpx2][tmpy2].legal!=1 || mp2[tmpx2][tmpy2].state==0) continue;
        if (state_trial[tmpx2][tmpy2]==1) tmpnum++;
        else if (mp2[tmpx2][tmpy2].state==1) tmpnum++;
        else if (state_trial[tmpx2][tmpy2]==-1) tmpnum2++; 
        //tmpnum:mine tmpnum2:waiting
      }
      if (tmpnum>mp2[tmpx][tmpy].num || tmpnum+tmpnum2<mp2[tmpx][tmpy].num) flag=0;
    }
    if (!flag) continue;
    search_trial(k+1,MAXCNT);
  }
  state_trial[arr[k].x][arr[k].y]=-1;
}
void init_trial(){
    memset(state_trial,-1,sizeof(state_trial));
    for (int i=1;i<=rows;i++)
      for (int j=1;j<=columns;j++){
        trial[i][j].num0=0;trial[i][j].num1=0;
      }
        
}

void update2(){
  init_besides();
  memset(border_size,0,sizeof(border_size));
  num_border=0;

  get_borders();

  int visited[1000],flag,cnt=0;
  //wait for update->change order
  for (int i=1;i<=num_border;i++){
    cnt=border_size[i];
    for (int j=1;j<=cnt;j++)
      arr[j]=border[i][j];
    
    init_trial();

    search_trial(1,cnt);

    flag=0;
    for (int j=1;j<=cnt;j++)
    if (trial[arr[j].x][arr[j].y].num0==0){
      mp2[arr[j].x][arr[j].y].state=1;
    }else if (trial[arr[j].x][arr[j].y].num1==0){
      mp2[arr[j].x][arr[j].y].state=0;flag=1;
    }
    if (flag) return;
  }
}
Point2 guess(){
  Point2 p0,p1;
  int left_num=0;
  double maxp=0,tmp;
  srand(99399);
  for (int i=1;i<=rows;i++)
    for (int j=1;j<=columns;j++)
    if (trial[i][j].num0+trial[i][j].num1>0 && mp2[i][j].state==-1){
      tmp=(double)trial[i][j].num0/(double)(trial[i][j].num0+trial[i][j].num1);
      if (tmp>maxp){
        maxp=tmp;
        p0.x=i;p0.y=j;
      }
    }

  if (maxp>0.82) return p0; 
  p1=p0;
    
  int STEPS=0;
  while (STEPS<2000){
    STEPS++;
    p0.x=rand()%rows+1;
    p0.y=rand()%columns+1;
    if (trial[p0.x][p0.y].num0+trial[p0.x][p0.y].num1>0) continue; 
    if (mp2[p0.x][p0.y].state!=1 && mp2[p0.x][p0.y].vis==0) return p0;
  }
  return p1;
}
void Decide() {
  int flag=0;
  Point2 p0,p1;
 while (true){
    update1();
    p0=get_ans();
    if (p0.x!=-1) {
      Execute(p0.x-1,p0.y-1);
      continue;
    }
    update2(); 
    p0=get_ans();
    if (p0.x!=-1) {
      Execute(p0.x-1,p0.y-1);
      continue;
    }
    p0=guess();
    Execute(p0.x-1,p0.y-1);
  }
}

#endif