#include<Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>

#define screen_width 128
#define screen_hight 64

const int buttonpin[]={13,12,14,27,26,25};
int l=20;
int cx=77,cy=10,r=6;
int xx1=71,xy1=4,xx2=83,xy2=16;
int input[3][3];
int output[3][3];
int a=0,b=0,c=0;
int sx1=69,sy1=2,sx2=85,sy2=18;
int type[2]={1,2};
int level[3]={1,2,3};
int at=0,al=0;
int w=0;
bool ow=0,xw=0;
int O_count=0,X_count=0,d=0,draw_count=0;
bool condition=0,reset=1,restart=0,for_loop=1;
int isfull=0;
int sum;

int win_sum=0;
int edge_positions[][2] = {
  {0, 1},
  {1, 0},
  {1, 2},
  {2, 1}
};
int corner_positions[][2] = {
  {0, 0},
  {0, 2},
  {2, 0},
  {2, 2}
};

String words;

void button_selection();
void player_turn();
void AI_turn();
void circle();
void cross();
void select();
void invert();
void select_type();
void invert_type();
void select_level();
void invert_level();
void easy_AI();
void medium_AI();
void win_condition();
void hard_AI();
void checkwin();
void win();
void after_win();
void game_reset();

enum gamestate{
    main_menu,
    singleplayer,
    difficulty_menu,
    easy_mode,
    medium_mode,
    hard_mode,
    multiplayer
  };
gamestate state=main_menu;


Adafruit_SSD1306 display(screen_width,screen_hight, &Wire,-1);

void setup(){
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    Serial.print(F("failed"));
    for (;;);
  }
  for (int i=0;i<6;i++)pinMode(buttonpin[i],INPUT_PULLUP);
  int x=1;
  for (int i=0;i<3;i++){
    for (int j=0;j<3;j++){
      input[i][j]=x;
      output[i][j]=0;
      x=x+1;
    }
  }
  display.clearDisplay();
  display.display();
}
void loop(){
  switch (state)
  {
    case main_menu:
    delay(200);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(5,5);
    display.print("Singleplayer");
    display.setCursor(5,20);
    display.print("Multiplayer");
    display.display();
    select_type();
    while(digitalRead(buttonpin[4])==HIGH){
      if(digitalRead(buttonpin[3])==LOW){
        if(at==0){
          at=at+1;
          select_type();
        }
        delay(500);
      }
      if(digitalRead(buttonpin[2])==LOW){
        if(at==1){
          at=at-1;
          select_type();
        }
        delay(500);
      }
      display.display();
    }
    delay(1000);
    if(type[at]==1) state=singleplayer;
    else if(type[at]==2) state=multiplayer;
    break;
  
    case singleplayer:
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(5,5);
      display.print("Easy");
      display.setCursor(5,20);
      display.print("Medium");
      display.setCursor(5,35);
      display.print("Hard");
      display.display();
      select_level();
      while (digitalRead(buttonpin[4])==HIGH){
        if(digitalRead(buttonpin[5])==LOW){
          state=main_menu;
          return;
        }
        if(digitalRead(buttonpin[3])==LOW){
          if(al<2){
            al=al+1;
            select_level();
          }
          delay(500);
        }
        if(digitalRead(buttonpin[2])==LOW){
          if(al>0){
            al=al-1;
            select_level();
          }
          delay(500);
        }
        display.display();
      }      
    delay(1000);
    if(level[al]==1) state=easy_mode;
    else if(level[al]==2) state=medium_mode;
    else if(level[al]==3) state=hard_mode;
    break;

    case easy_mode:
    game_reset();
     if(c==0){
        while(for_loop==1){
          easy_AI();
          AI_turn();
        }
        c=c+1;
        for_loop=1;
        delay(200);
      }
      else if (c==1)
      {
        player_turn();
      }
      
      delay(200);
      if(restart==1){
        delay(200);
        state=singleplayer;
        restart=0;
        reset=1;
        X_count=0;O_count=0;draw_count=0;
      }
      break;

    case medium_mode:
    game_reset();
    if(c==0){
        while(for_loop==1){
          medium_AI();
          AI_turn();
        }
        c=c+1;
        for_loop=1;
        delay(200);
      }
    
      else if (c==1){
        player_turn();
      }
        
        delay(200);
      if(restart==1){
        delay(200);
        state=singleplayer;
        restart=0;
        reset=1;
        X_count=0;O_count=0;draw_count=0;
      }
        break;

    case hard_mode:
    game_reset();
    if(c==0){
        while(for_loop==1){
          hard_AI();
          AI_turn();
        }
        c=c+1;
        for_loop=1;
        delay(200);
      }
    
      else if (c==1){
        player_turn();
      }
        
        delay(200);
      if(restart==1){
        delay(200);
        state=singleplayer;
        restart=0;
        reset=1;
        X_count=0;O_count=0;draw_count=0;
      }
        break;

    case multiplayer:
    game_reset();
      button_selection();
      if((c==0) && (output[a][b]==0))
      {
        circle();
        c=c+1;
      }
      else if ((c==1) && (output[a][b]==0))
      {
        cross();
        c=c-1;
      }
      delay(200);
      display.display();
      win();
      if(restart==1){
        delay(200);
        state=main_menu;
        restart=0;
        reset=1;
        X_count=0;O_count=0;draw_count=0;
      }
      break;
   default:
      state=main_menu;
    break;
  }
}

void button_selection(){
  while(digitalRead(buttonpin[4])==HIGH){
    if(digitalRead(buttonpin[5])==LOW){
      restart=1;
      return;
    }
    if(digitalRead(buttonpin[3])==LOW){
      if(a<2){
        a=a+1;
        select();
      }
      delay(500);
      
    }
    if(digitalRead(buttonpin[2])==LOW){
      if(a>0){
        a=a-1;
        select();
      }
      delay(500);
    }
    if(digitalRead(buttonpin[1])==LOW){
      if(b<2){
        b=b+1;
        select();
      }
      delay(500);
    }
    if(digitalRead(buttonpin[0])==LOW){
      if(b>0){
        b=b-1;
        select();
      }
      delay(500);
    }
    display.display();
  }
}
void circle() {
  int value = input[a][b];
  int col = (value - 1) % 3;
  int row = (value - 1) / 3;
  display.drawCircle(cx + col * l, cy + row * l, r, BLACK);
  output[a][b] = -1;
}
void cross(){
  int value = input[a][b];
  int col = (value - 1) % 3;
  int row = (value - 1) / 3;
  display.drawLine(xx1 + col * l,xy1 + row * l,xx2 + col * l,xy2 + row * l,BLACK);
  display.drawLine(xx2 + col * l,xy1 + row * l,xx1 + col * l,xy2 + row * l,BLACK);
  output[a][b]= 1;
  
}

void win(){
  Serial.println("win execute");
  w=-1;
  checkwin();
  if(condition==1){
    words="O Wins";
    after_win();
    condition=0;
    ow=1;
    reset=1;
    for_loop=0;
  }
  w=1;
  checkwin();
  if(condition==1){
    words="X Wins";
    after_win();
    condition=0;
    xw=1;
    reset=1;
    for_loop=0;
  }
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      if(output[i][j]!=0){
        isfull=isfull+1;
      }
    }
  }

  if(isfull==9){
    reset=1;
    for_loop=0;
    d=1;
    words="Match Draw";
    after_win();
  }
  isfull=0;
}
void checkwin(){
  Serial.println("checkwin execute");
  for (int i=0;i<3;i++){
    if((output[i][0]==w) && (output[i][1]==w) && (output[i][2]==w)){
      condition=1;}
    if((output[0][i]==w) && (output[1][i]==w) && (output[2][i]==w)){
      condition=1;}
  }
  if((output[0][0]==w) && (output[1][1]==w) && (output[2][2]==w)){
    condition=1;}
  if((output[0][2]==w) && (output[1][1]==w) && (output[2][0]==w)){
    condition=1;}
}

void select(){
  int value=input[a][b];
  invert();
  int col = (value - 1) % 3;
  int row = (value - 1) / 3;
  for(int i1=(sx1 + col*l);i1<(sx2 + col*l);i1++){
    for(int i2=(sy1 + row*l);i2<(sy2 + row*l);i2++){
      uint16_t color=display.getPixel(i1,i2);
      display.drawPixel(i1,i2,!color);
    }
  }

}
void invert(){
  for(int value=1;value<=9;value++){
    int col = (value - 1) % 3;
    int row = (value - 1) / 3;
    uint16_t color=display.getPixel(sx1 + col*l,sy1 + row*l);
    if(color==WHITE){
      for(int i1=(sx1 + col*l);i1<(sx2 + col*l);i1++){
        for(int i2=(sy1 + row*l);i2<(sy2 + row*l);i2++){
          uint16_t color=display.getPixel(i1,i2);
          display.drawPixel(i1,i2,!color);
        }
    }
    }
  }
  display.display();
}
void select_type(){
  int value=type[at];
  invert_type();
  for(int i1=2;i1<78;i1++){
    for(int i2=2+(value-1)*15;i2<14+(value-1)*15;i2++){
      uint16_t color=display.getPixel(i1,i2);
      display.drawPixel(i1,i2,!color);
    }
  }
}
void invert_type(){
  for(int value=1;value<=2;value++){
    uint16_t color=display.getPixel(2,2+(value-1)*15);
    if(color==WHITE){
      for(int i1=2;i1<78;i1++){
        for(int i2=2+(value-1)*15;i2<14+(value-1)*15;i2++){
        uint16_t color=display.getPixel(i1,i2);
        display.drawPixel(i1,i2,!color);
        }
      }
    }
  }
  display.display();
}

void select_level(){
  int value=level[al];
  invert_level();
  for(int i1=2;i1<44;i1++){
    for(int i2=2+(value-1)*15;i2<14+(value-1)*15;i2++){
      uint16_t color=display.getPixel(i1,i2);
      display.drawPixel(i1,i2,!color);
    }
  }
}
void invert_level(){
  for(int value=1;value<=3;value++){
    uint16_t color=display.getPixel(2,2+(value-1)*15);
    if(color==WHITE){
      for(int i1=2;i1<44;i1++){
        for(int i2=2+(value-1)*15;i2<14+(value-1)*15;i2++){
        uint16_t color=display.getPixel(i1,i2);
        display.drawPixel(i1,i2,!color);
        }
      }
    }
  }
  display.display();
}
void AI_turn(){
  if(output[a][b]==0){
    select();
    circle();
    display.display();
    for_loop=0;
  }
  Serial.println("ai turn");
  win();
  
}
void player_turn(){
  while(for_loop==1){
    select();
    button_selection();
    if(restart==1){
      return;
    }
    if(output[a][b]==0){
      cross();
      display.display();
      for_loop=0;
    }
    Serial.println("player turn");
    win();
    
  } 
  c=c-1;
  for_loop=1;
}

void easy_AI(){
  uint32_t r1=esp_random();
  a=r1%3;
  uint32_t r2=esp_random();
  b=r2%3;
}
void medium_AI(){
  win_sum=-2;
  win_condition();
  if(reset==1){
    reset=0;
    return;
  }
  win_sum=2;
  win_condition();
  if(reset==1){
    reset=0;
    return;
  }
  easy_AI();
  
}
void win_condition(){

  for (int i=0;i<3;i++){
    sum=output[i][0]+output[i][1]+output[i][2];
    if(sum==win_sum){
      for(int j=0;j<3;j++){
        if(output[i][j]==0){
          a=i;
          b=j;
          reset=1;
          return;
        }
      }
    }
  }
  for (int i=0;i<3;i++){
    sum=output[0][i]+output[1][i]+output[2][i];
    if(sum==win_sum){
      for(int j=0;j<3;j++){
        if(output[j][i]==0){
          a=j;
          b=i;
          reset=1;
          return;
        }
      }
    }
  }
  sum=output[0][0]+output[1][1]+output[2][2];
  if(sum==win_sum){
    for(int i=0;i<3;i++){
      if(output[i][i]==0){
        a=i;
        b=i;
        reset=1;
        return;
      }
    }
  }
  sum=output[0][2]+output[1][1]+output[2][0];
  if(sum==win_sum){
    for(int i=0;i<3;i++){
      if(output[i][i]==0){
        a=i;
        b=2-i;
        reset=1;
        return;
      }
    }
  }
}
void hard_AI(){
  win_sum=-2;
  win_condition();
  if(reset==1){
    reset=0;
    return;
  }
  win_sum=2;
  win_condition();
  if(reset==1){
    reset=0;
    return;
  }
  if(output[1][1]==0){
    a=1;
    b=1;
    return;
  }
  if(((output[2][0]==1) && (output[0][2]==1) && (output[1][1]==-1)) || ((output[0][0]==1) && (output[2][2]==1) && (output[1][1]==-1))){
    for (int i = 0; i < 4; i++) {
      int x = edge_positions[i][0];
      int y = edge_positions[i][1];
      if (output[x][y] == 0) {
        a = x;
        b = y;
        return;
      }
    }
  }
  for (int i = 0; i < 4; i++) {
    int x = corner_positions[i][0];
    int y = corner_positions[i][1];
      if (output[x][y] == 0) {
        a = x;
        b = y;
        return;
      }
  }
  easy_AI();

}
void after_win(){
delay(200);
display.clearDisplay();
display.display();
  int16_t x1,y1;
  uint16_t width,hight;
  display.getTextBounds(words,0,0,&x1,&y1,&width,&hight);
  int x =(screen_width - width)/2;
  int y =(screen_hight-hight)/2;
  display.setCursor(x,y);
  display.setTextSize(1);
  display.print(words);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
}
void game_reset(){
  if(reset==1){
    display.clearDisplay();
      a=0;
      b=0;
      for (int i=0;i<3;i++){
        for (int j=0;j<3;j++){
        output[i][j]=0;
        }
      }
      if(ow==1){
        O_count=O_count+1;
      }
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(5,5);
      display.print("O wins:");
      display.setCursor(50,5);
      display.print(O_count);
      ow=0;
      if(xw==1){
        X_count=X_count+1;
      }
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(5,20);
      display.print("X wins:");
      display.setCursor(50,20);
      display.print(X_count);
      xw=0;
      if(d==1){
        draw_count=draw_count+1;
      }
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(5,45);
      display.print("Draws:");
      display.setCursor(50,45);
      display.print(draw_count);
      d=0;
      display.display();
      display.drawLine(87,0,87,60,WHITE);
      display.drawLine(107,0,107,60,WHITE);
      display.drawLine(67,20,127,20,WHITE);
      display.drawLine(67,40,127,40,WHITE);
      select();
      reset=0;
    }
}