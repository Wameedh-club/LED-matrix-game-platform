
const int size = 16;
int w=16;
int snake_size;
byte matrix[size][size] ;//= new int [size][size];
int road_line[size] ;//= new int[size];
int snakeX[100] ;// 
int snakeY[100] ;// 

int X_snake, Y_snake;
int X_target, Y_target;

int state;
int X_next=1;//0, 1 or -1
int Y_next=0;//0, 1 or -1
boolean crashed = false;
boolean start = false;

//for display
int ii,jj,kk,ll,rr;
bool bin16[16][4]={{0,0,0,0},{1,0,0,0},{0,1,0,0},{1,1,0,0},{0,0,1,0},{1,0,1,0},{0,1,1,0},{1,1,1,0},{0,0,0,1},{1,0,0,1},{0,1,0,1},{1,1,0,1},{0,0,1,1},{1,0,1,1},{0,1,1,1},{1,1,1,1}};


void setup() {
  Serial.begin(9600);
  init_road();
  init_snake();
  //pin init
  for (ii=0 ; ii<=11 ; ii++){
    pinMode(ii,OUTPUT);
   }
}

void loop() {
  
  switch (state) {
    case 0 : update_idle();
      if (start==true)
          state = 1;
      break;

      case 1 :
          crashed = false;
          init_matrix();
          X_snake = 8;
          Y_snake=8;
          snake_size=3;
          for(int i=0; i<snake_size; i++){
            snakeX[i]=8-i;
            snakeY[i]=8;
          }
          create_target();
          //update_all();
          //display();
          state = 2;
          break;

       case 2:
            move_snake();         
            update_all();
            checkCollusion();
            if (crashed == true){
              start=false;
              state = 0;
            }
            break;
        
        default: break;
   }//switch
   for (int lo =0 ; lo<15; lo++) {
      printdecoder(matrix);
   }
}

void init_snake(){
}


void printdecoder(byte prinarray[16][16]){
   //
    int result=900;//delay
  for (ii=0;ii<16;ii++){
    for (jj=0;jj<4;jj++){
      int d=jj+5;
      if(d==0)
        d=10;
      else if(d==1)
        d=11;
     digitalWrite(d,bin16[ii][jj]);
    } // select line
  for (kk=0; kk<16 ;kk++) {
    if(prinarray[kk][ii]==1){
      for (ll=0;ll<4;ll++){
        digitalWrite(4,0);
        int d=ll;
      if(d==0)
        d=10;
      else if(d==1)
        d=11;
        digitalWrite(d,bin16[kk][ll]);
      }
      delayMicroseconds(result);
       //delay(result);
      }else {
        digitalWrite(4,1);}
      }
  }
}
/*
void printdecoder(byte prinarray[16][16],int delays){
  for (ii=0;ii<16;ii++){
  for (jj=0;jj<4;jj++){
    digitalWrite(jj+2,bin16[ii][jj]);
  } // select line
  for (kk=0; kk<8 ;kk++) {
    if(prinarray[kk][ii]==1){
      for (ll=0;ll<3;ll++){
        digitalWrite(7,0);
        digitalWrite(8+ll,bin16[kk][ll]);
       }delayMicroseconds(delays);
      }else {digitalWrite(7,1);}
      }
  }
}
*/
void move_snake(){
  for(int i=snake_size; i>0; i--){
    snakeX[i]=snakeX[i-1];
    //snake[i].x%=16;
    //if(snake[i].x<0) snake[i].x=15;
    
    snakeY[i]=snakeY[i-1];
    //snake[i].y%=16;
    //if(snake[i].y<0) snake[i].y=15;
  }
  
  snakeX[0] = snakeX[0]+X_next;
  snakeX[0] %= 16;
  if(snakeX[0]<0) snakeX[0]=15;
  
  snakeY[0] = snakeY[0]+Y_next;
  snakeY[0] %= 16;
  if(snakeY[0]<0) snakeY[0]=15;
}

void addGrid(){
  snakeX[snake_size]=X_target;
  snakeY[snake_size]=Y_target;
  snake_size++;
}


void checkCollusion(){
  for (int i=0; i<snake_size; i++)
    for (int j=i+1; j<snake_size; j++){
      if((snakeX[i]==snakeX[j])&&(snakeY[i]==snakeY[j])){
        crashed=true;
        return;
      }
    }
    if((snakeX[0]==X_target)&&(snakeY[0]==Y_target)){
      addGrid();
      create_target();
    }
      
}

void update_all(){
  init_matrix();
  
  //update snake
  for(int i=0; i<snake_size; i++){
    matrix[snakeX[i]][snakeY[i]]++;
  }
  
  //update target
  matrix[X_target][Y_target]++;

  
}

void create_target(){
  X_target = (int) random(16);
  Y_target = (int) random(16);
}

void init_road(){
  for(int i=0; i<size; i++)
    if((i%2)==0)
      road_line[i]=1;
    else
      road_line[i]=0;
}


void update_road(){
  for(int i=0; i<size; i++)
    if(road_line[i]==0)
      road_line[i]=1;
    else
      road_line[i]=0;
}


void init_matrix(){
  for(int i=0; i<size; i++)
    for(int j=0; j<size; j++)
      matrix[i][j]=0;
}

void update_idle(){
  init_matrix();
  update_road();
  for(int i=1; i<size-1; i++){
    matrix[i][1]=matrix[i][14]=road_line[i];
    matrix[1][i]=matrix[14][i]=road_line[i];
  }
}

void serialEvent(){
  char d =Serial.read();
  if(d=='0'){//w
    X_next=-1; Y_next=0;
    start=true;
  }
  else if(d=='1'){//d
    X_next=0; Y_next=1;
    start=true;
  }
  else if(d=='2'){//s
    X_next=1; Y_next=0;
    start=true;
  }
  else{//(d=='3'){
    X_next=0; Y_next=-1;
    start=true;
  }
}


