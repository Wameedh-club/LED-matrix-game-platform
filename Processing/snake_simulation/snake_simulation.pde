final int size = 16;
int w=16;
int snake_size;
int [][] matrix= new int [size][size];
int [] road_line= new int[size];
Grid [] snake = new Grid [size*size];

int X_snake, Y_snake;
int X_target, Y_target;

int state;
int X_next=1;//0, 1 or -1
int Y_next=0;//0, 1 or -1
boolean crashed = false;
boolean start = false;

void setup() {
  size(320,320);
  frameRate(12);
  init_road();
  init_snake();
}

void draw() {
  
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
            snake[i].x=8-i;
            snake[i].y=8;
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
   display();
}

void init_snake(){
  for(int i=0; i<100; i++){
    snake[i]=new Grid();
    //snake[i].x=snake[i].y=-1;
  }
}

void mousePressed(){
  start=true;
}

void move_snake(){
  for(int i=snake_size; i>0; i--){
    snake[i].x=snake[i-1].x;
    //snake[i].x%=16;
    //if(snake[i].x<0) snake[i].x=15;
    
    snake[i].y=snake[i-1].y;
    //snake[i].y%=16;
    //if(snake[i].y<0) snake[i].y=15;
  }
  
  snake[0].x = snake[0].x+X_next;
  snake[0].x %= 16;
  if(snake[0].x<0) snake[0].x=15;
  
  snake[0].y = snake[0].y+Y_next;
  snake[0].y %= 16;
  if(snake[0].y<0) snake[0].y=15;
}

void keyPressed(){
  if (key=='d'){
    X_next=1; Y_next=0;
    start=true;
  }
  else if (key=='a'){
    X_next=-1; Y_next=0;
    start=true;
  }
  else if (key=='w'){
    X_next=0; Y_next=-1;
    start=true;
  } 
  else if (key=='s'){
    X_next=0; Y_next=1;
    start=true;
  }
}

void addGrid(){
  snake[snake_size].x=X_target;
  snake[snake_size].y=Y_target;
  snake_size++;
}


void checkCollusion(){
  for (int i=0; i<snake_size; i++)
    for (int j=i+1; j<snake_size; j++){
      if((snake[i].x==snake[j].x)&&(snake[i].y==snake[j].y)){
        crashed=true;
        return;
      }
    }
    if((snake[0].x==X_target)&&(snake[0].y==Y_target)){
      addGrid();
      create_target();
    }
      
}

void update_all(){
  init_matrix();
  
  //update snake
  for(int i=0; i<snake_size; i++){
    matrix[snake[i].x][snake[i].y]++;
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

void display() {
  for (int x=0; x<16; x++) {
    for (int y=0; y<16; y++) {
      if (matrix[x][y]>=1) fill(0);
      else fill(255);
      stroke(0);
      rect(x*w, y*w, w,w);
    }
  }
}