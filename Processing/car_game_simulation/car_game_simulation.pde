final int size = 16;
int w=16;
int [][] matrix= new int [size][size];
final int  maxEnemy = 2;
int enemyNb;
int b_left;
int b_right;
int [] road_line = new int[size];
int X_car;
int [] X_enemy=new int [maxEnemy];
int [] Y_enemy=new int [maxEnemy];
int state;
boolean crashed = false;

void setup() {
  size(320,320);
  frameRate(8);
  init_road();
  X_car = 8;
}

void draw() {
  
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
   display();
   b_right=0;
   b_left=0;
}

void keyPressed(){
  if (key=='d')
    b_right=1;
    
  else if (key=='a')
    b_left=1;
    
  update_car();
}

void update_enemies(){
  for(int i=0; i<enemyNb; i++)
    if(Y_enemy[i]>18){
      Y_enemy[i]=-1;
      if(random(1)>0.5)
        X_enemy[i]=5;
      else
        X_enemy[i]=10;
    }
    else
      Y_enemy[i]++;
  }


void checkCollusion(){
  //println("checking...");
  for (int i=0; i<size; i++)
    for (int j=0; j<size; j++)
      if(matrix[i][j]>=2){
        println("crashed=true");
        crashed=true;
        return;
      }
  }

void update_all(){
  init_matrix();
  // update road
  for (int i=0; i<size; i++){
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
  for (int i=0; i<enemyNb; i++){
    //if (Y_enemy[i]<16){
      try{
    matrix[Y_enemy[i]][X_enemy[i]]++;
    matrix[Y_enemy[i]-1][X_enemy[i]-1]++;
    matrix[Y_enemy[i]-1][X_enemy[i]]++;
    matrix[Y_enemy[i]-1][X_enemy[i]+1]++;
    matrix[Y_enemy[i]-2][X_enemy[i]-1]++;
    matrix[Y_enemy[i]-2][X_enemy[i]]++;
    matrix[Y_enemy[i]-2][X_enemy[i]+1]++;
    }catch(Exception e){}
  }
}

void create_enemy(int n){
  if(n==0)
    Y_enemy[enemyNb]=2;
  else
    Y_enemy[enemyNb]=-8;// or some (-) nb
  if (random(1)>0.5)
    X_enemy[enemyNb]=5;
  else
    X_enemy[enemyNb]=10;
    
  enemyNb++;
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

void update_car(){    //2-->12
  if((b_left==1)&&(X_car>3))
    X_car--;
  else if((b_right==1)&&(X_car<12))
    X_car++;
  }
 
  
void display() {
  for (int x=0; x<16; x++) {
    for (int y=0; y<16; y++) {
      if (matrix[y][x]==1) fill(0);
      else fill(255);
      stroke(0);
      rect(x*w, y*w, w,w);
    }
  }
}