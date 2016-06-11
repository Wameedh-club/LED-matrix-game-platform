byte const size = 16;
byte w=16;
byte speed=5;//=2
byte matrix[size][size] ;
byte const maxEnemy = 2;
byte enemyNb;
byte b_left;
byte b_right;
byte road_line[size];
byte X_car;
int X_enemy[maxEnemy];
int Y_enemy[maxEnemy];
byte state;
bool crashed = false;
bool click=false;

//for display
int ii,jj,kk,ll,rr;
//bool bin8[8][3]={{0,0,0},{1,0,0},{0,1,0},{1,1,0},{0,0,1},{1,0,1},{0,1,1},{1,1,1}};
bool bin16[16][4]={{0,0,0,0},{1,0,0,0},{0,1,0,0},{1,1,0,0},{0,0,1,0},{1,0,1,0},{0,1,1,0},{1,1,1,0},{0,0,0,1},{1,0,0,1},{0,1,0,1},{1,1,0,1},{0,0,1,1},{1,0,1,1},{0,1,1,1},{1,1,1,1}};


void setup() {
  Serial.begin(9600);
  init_road();
  X_car = 8;
  //buttons
  //pinMode(12, INPUT);
  //pinMode(13, INPUT);
  //attachInterrupt(0, pin_ISR, CHANGE);

  //display here
  
   for (ii=0 ; ii<=11 ; ii++){
    pinMode(ii,OUTPUT);
   }
   
}

void loop() {


  switch (state) {
    case 0 : update_idle();
      if ((b_left == 1) || (b_right == 1))
          state = 1;
      break;

      case 1 : crashed = false;
          init_road();
          init_matrix();
          X_car = 8;
          enemyNb=0;
          create_enemy(0);
          create_enemy(1);
          state = 2;
          break;

       case 2:  //update_car();
            update_enemies();
            update_road();
            update_all();
            checkCollusion();
            if (crashed == true)
             state = 0;
            break;
        
        default: break;
   }//switch
   //display();
     b_right=0;
   b_left=0;
   //click=false;
for (int lo =0 ; lo<speed; lo++) {
  printdecoder(matrix);
}
   
}


//void display(){}


void printdecoder(byte prinarray[16][16]){
   //
    int result=600;//delay
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

void update_enemies(){
  for(byte i=0; i<enemyNb; i++)
    if(Y_enemy[i]>18){
      Y_enemy[i]=-1;
      int r = (int)random(10);
      if (r > 5)
        X_enemy[i]=5;
      else
        X_enemy[i]=10;
    }
    else
      Y_enemy[i]++;
  }

void checkCollusion(){
  for (byte i=0; i<size; i++)
    for (byte j=0; j<size; j++)
      if(matrix[i][j]>=2){
        crashed=true;
        return;
      }
}

void update_road(){
  for(byte i=0; i<size; i++)
    if(road_line[i]==0)
      road_line[i]=1;
    else
      road_line[i]=0;
  }


void update_all(){
  init_matrix();
  
  // update road
  for (byte i=0; i<size; i++){
    matrix[i][1]=road_line[i];
    matrix[i][14]=road_line[i];
  }

  //update car
  matrix[15][X_car]++;
  matrix[15][X_car+1]++;
  matrix[15][X_car-1]++;
  matrix[14][X_car]++;
  matrix[13][X_car]++;
  matrix[13][X_car-1]++;
  matrix[13][X_car+1]++;
  matrix[12][X_car]++;

  //update enemies
  for (byte i=0; i<enemyNb; i++){
    if ((Y_enemy[i]<16)&&(Y_enemy[i]>2)){
      matrix[Y_enemy[i]][X_enemy[i]]++;
      matrix[Y_enemy[i]-1][X_enemy[i]-1]++;
      matrix[Y_enemy[i]-1][X_enemy[i]]++;
      matrix[Y_enemy[i]-1][X_enemy[i]+1]++;
      matrix[Y_enemy[i]-2][X_enemy[i]-1]++;
      matrix[Y_enemy[i]-2][X_enemy[i]]++;
      matrix[Y_enemy[i]-2][X_enemy[i]+1]++;
    }
  }
}

void create_enemy(int n){
  if(n==0)
    Y_enemy[enemyNb]=2;
  else
    Y_enemy[enemyNb]=-8;// or some (-) nb
  int r = (int)random(10);
  if (r > 5)
    X_enemy[enemyNb]=5;
  else
    X_enemy[enemyNb]=10;
    
  enemyNb++;
}

void init_road(){
  for(byte i=0; i<size; i++)
    if((i%2)==0)
      road_line[i]=1;
    else
      road_line[i]=0;
}

void init_matrix(){
  for(byte i=1; i<size; i++)
    for(byte j=1; j<size; j++)
      matrix[i][j]=0;
}

void update_idle(){
  init_matrix();
  update_road();
  for(byte i=1; i<size-1; i++){
    matrix[i][1]=matrix[i][14]=road_line[i];
    matrix[1][i]=matrix[14][i]=road_line[i];
  }
}

void update_car(){    //2-->12
  if((b_left==1)&&(X_car>3))
    X_car--;
  else if((b_right==1)&&(X_car<12))
    X_car++;
  }
  
 void serialEvent(){
  char d =Serial.read();
  if(d=='1'){
    b_right = 1;
    update_car();
    //click=true;
  }
  else {
    b_left = 1;
    update_car();
    //click=true;
    }
}
