#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<curses.h>
#include<unistd.h>
#include<termios.h>
#include<sys/time.h>

#define FIELD_SIZE_X 10
#define FIELD_SIZE_Y 10

void init();
void playerMove();
void update();
void draw();
void init_keyboard();
void close_keyboard();
int kbhit();
int readkey();

int p_x, p_y, e_x, e_y;
int field[FIELD_SIZE_X][FIELD_SIZE_Y] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                         {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                         {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                         {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                         {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                         {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                         {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                         {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
static struct termios init_tio;

int main(){
  init();

  while(1){
    draw();
    playerMove();
    update();
  }

  return 0;
}

void init(){
  p_x = 5;
  p_y = 5;
  e_x = 5;
  e_y = 2;
}

void playerMove(){

  if(kbhit()){
    switch(getch()){
      case 'w':
        p_y--;
        break;
      case 's':
        p_y++;
        break;
      case 'a':
        p_x--;
        break;
      case 'd':
        p_x++;
        break;
      case 'q':
        exit(1);
        break;
    }
  }
}

void update(){
  field[p_x][p_y] = 2;
  field[e_x][e_y] = 3;
}

void draw(){
  int i, j;

  system("clear");

  for(i = 0; i < FIELD_SIZE_X; i++){
    for(j = 0; j < FIELD_SIZE_Y; j++){
      switch(field[i][j]){
        case 0:
          printf(" ");
          break;
        case 1:
          printf("+");
          break;
        case 2:
          printf("@");
          break;
        case 3:
          printf("#");
          break;
      }
    }
    printf("\n");
  }
}

void init_keyboard()  { tcgetattr(0,&init_tio); }

void close_keyboard() { tcsetattr(0,TCSANOW,&init_tio); }

int kbhit(){
  struct termios tio;
  struct timeval tv;
  fd_set rfds;

  memcpy(&tio,&init_tio,sizeof(struct termios));
  tio.c_lflag &= ~(ICANON);
  tcsetattr(0,TCSANOW,&tio);
  FD_ZERO(&rfds);
  FD_SET(0,&rfds);
  tv.tv_usec = 0;
  tv.tv_sec  = 0;
  select(1,&rfds,NULL,NULL,&tv);
  tcsetattr(0,TCSANOW,&init_tio);

  return (FD_ISSET(0,&rfds)?1:0);

}
int readkey(){
  int ch;
  struct termios tio;

  memcpy(&tio,&init_tio,sizeof(struct termios));
  tio.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO,TCSANOW,&tio);
  read(0,&ch,1);
  tcsetattr(0,TCSANOW,&init_tio);

  return ch;
}
