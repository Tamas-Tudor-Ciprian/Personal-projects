#include <iostream>
#include <math.h>
#include <Windows.h>
#include <chrono>


using namespace std;

const int WIDTH = 120;
const int HEIGHT = 40;

float mod_diff(float x,float y){
if(x>=y)
    return x-y;
if(y>=x)
    return y-x;
}
int wrap_aroundx(float x){
int a;
a = (int)x/WIDTH;
if(x>0)
    {


return x-WIDTH*a;
    }else
    {
     return x-WIDTH*a+WIDTH;
    }
}
int wrap_aroundy(float y){
int a;
a = (int)y/HEIGHT;
if(y>0)
    {


return y-HEIGHT*a;
    }else
    {
     return y-HEIGHT*a+HEIGHT;
    }
}
int coord_to_screen(float x,float y){
return WIDTH*wrap_aroundy(y)+wrap_aroundx(x);
}
float squared(float x){
return x*x;
}
float x_component(float vector,float angle){
return cos(angle)*vector;
}
float y_component(float vector,float angle){
return sin(angle)*vector;
}
float arctan(float x,float y){
 if(x<0&&y<0)
    return M_PI+atan(y/x);
  else  if(x<0&&y>0)
    return atan(y/x)-M_PI;
else
    return atan(y/x);



}
void accelerate(float *velocity,float *velocityAngle,float acceleration,float angle){
float x = x_component(*velocity,*velocityAngle)+x_component(acceleration,angle);
float y = y_component(*velocity,*velocityAngle)+y_component(acceleration,angle);
*velocity = sqrt(squared(x)+squared(y));
*velocityAngle = arctan(x,y);
}
float vector_from_components(float x,float y){
return sqrt(squared(x)+squared(y));
}




class Object{
private:
float x;
float y;
public:float angle;
public:float velocityAngle = 0;
public:float velocity = 0;
public:bool exists = true;


public:float size =  0;
float vertex[200][2];
int vertexNum = 0;

protected:

//getters
public:float getAngle(){
    return angle;
}
float getVelocity(){
return velocity;
}
public:float getVelocityAngle(){
return velocityAngle;
}
public:float getX(){
return x;
}
public:float getY(){
    return y;
}
public:float getSize(){
return size;
}

//setters
    void setVertexNum(int vertexNum){
    this->vertexNum = vertexNum;
    }
    void setVelocity(float velocity){
        this->velocity = velocity;
       }
    void setAngle(float angle){
        this->angle =angle;
     float a = (int)angle%(int)(2*M_PI);
     if(angle>=2*M_PI||angle<=-2*M_PI){
    this->angle = a;}

    velocityAngle = angle;
    }
 public:void setAngle(float fElapsedTime,int angularVelocity){
angle += angularVelocity*fElapsedTime;
 float a = (int)angle%(int)(2*M_PI);
 if(angle>=2*M_PI||angle<=-2*M_PI)
angle = a;
}
    void setSize(float size){
    this->size = size;
    }
   public:void setCoords(float x,float y){
    this->x=x;
    this->y=y;
     int a = (int)x%WIDTH;
    int b = (int)y%HEIGHT;
   if(x>WIDTH){
    this->x = a;}

    if(x<0){
    this->x = WIDTH-1;}

   if(y>HEIGHT){
    this->y = b;}

    if(y<0){
    this->y = HEIGHT-1;}
    }


public:

 Object(float x,float y){
    this->x = x;
    this->y = y;
 }
 Object(){
 exists = false;
 }
void update_position(float fElapsedTime){
    x += x_component(velocity,velocityAngle)*fElapsedTime;
    y += y_component(velocity,velocityAngle)*fElapsedTime;
    int a = (int)x%WIDTH;
    int b = (int)y%HEIGHT;
   if(x>WIDTH){
    x = a;}

    if(x<0){
    x = WIDTH-1;}

   if(y>HEIGHT){
    y = b;}
    if(y<0){
    y = HEIGHT-1;}

}
void display(wchar_t screen[]){

for(int i =0 ;i < vertexNum ;i++)
{

  vertex[i][1] = cos(angle+2*M_PI*(vertexNum-i)/vertexNum)*size+x;
  vertex[i][2] = sin(angle+2*M_PI*(vertexNum-i)/vertexNum)*size+y;

  if(vertexNum==3&&i==0)
  screen[coord_to_screen(vertex[i][1],vertex[i][2])] = 'O';
  else
    screen[coord_to_screen(vertex[i][1],vertex[i][2])] = '@';
//connect vertexes(simpify later)
if(i>0){
    float rise = vertex[i][2]-vertex[i-1][2];
    float run = vertex[i][1]-vertex[i-1][1];
    float angle = arctan(run,rise);
    float distance=vector_from_components(run,rise);
    for(int j = 1;j<distance;j++)
    screen[coord_to_screen(vertex[i-1][1]+cos(angle)*j,vertex[i-1][2]+sin(angle)*j)]='@';
    if(i ==vertexNum-1){
        rise = vertex[i][2]-vertex[0][2];
       run = vertex[i][1]-vertex[0][1];
    float angle = arctan(run,rise);
    for(int j = 1;j<distance;j++)
         screen[coord_to_screen(vertex[i][1]-cos(angle)*j,vertex[i][2]-sin(angle)*j)]='@';
    }
}

}

}
void work(wchar_t screen[],float fElapsedTime){
    if(exists == true){
update_position(fElapsedTime);
display(screen);}
}
};

class SpaceShip : public Object{

    public:
SpaceShip(float x,float y):Object(x,y){
setVertexNum(3);
setSize(3);
setVelocity(3);
setAngle(5);

}

void acceleration(){
    float maxVelocity = 15;
float acceleration = 0.1;
accelerate(&velocity,&velocityAngle,acceleration,getAngle());
if(velocity>maxVelocity)
    velocity-=mod_diff(velocity,maxVelocity);
}

};

class Asteroid : public Object{
public:


 Asteroid():Object(){
 size = rand()%7+2;

    int num=M_PI*size;
setVertexNum(num);
setAngle(rand()%100);
setVelocity(rand()%10);

setCoords(rand(),rand());
 };
 Asteroid(float x,float y):Object(x,y){
size = rand()%7+2;

    int num=M_PI*size;
setVertexNum(num);

setSize(size);
setAngle(rand()%100);
setVelocity(rand()%10);
}
};

class Bullet : public Object{
public:

Bullet():Object(){
setSize(0);
setVertexNum(1);
};

Bullet(float x,float y):Object(x,y){
    setSize(0);
    setVertexNum(1);
};

};

void cls(wchar_t screen[]){
         for(int i = 0;i < WIDTH*HEIGHT;i++)
            screen[i] = ' ';
         }



int main()
{

 SpaceShip ship(60,20);

 Asteroid asteroid[200];

 Bullet bullet[200];

 asteroid[0].exists = true;
 asteroid[1].exists = true;
 asteroid[2].exists = true;




int score = 0;
int i = 0;
int asteroidIndex = 3;




    wchar_t *screen =new wchar_t[WIDTH*HEIGHT];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;


auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();



auto start= chrono::system_clock::now();
float xDistanceB[200];
float yDistanceB[200];

bool ok = true;

int level = 0;

while(1){

bool asteroids = false;

for(int i = 0;i<200;i++){
        if(asteroid[i].exists == true){
          asteroids = true;
 float yDistance = ship.getY()-asteroid[i].getY();
 float xDistance = ship.getX()-asteroid[i].getX();
 if(vector_from_components(xDistance,yDistance)<=asteroid[i].size+1)
    ok = false;
}else{
asteroid[i].setCoords(rand(),rand());
asteroid[i].setVelocity(rand()%10);
asteroid[i].setAngle(rand());
}
}



 for(int i = 0;i<200;i++)
    for(int j = 0;j<200;j++){
        if(bullet[i].exists == true&&asteroid[j].exists == true){
    xDistanceB[i] = bullet[i].getX()-asteroid[j].getX();
    yDistanceB[i] = bullet[i].getY()-asteroid[j].getY();
    if(vector_from_components(xDistanceB[i],yDistanceB[i])<=asteroid[j].size+1){
        asteroid[j].exists = false;
        bullet[i].exists = false;
        score +=100*asteroid[j].size;
        float size = asteroid[j].size;

        while(size>2){
            float holder = rand()%((int)asteroid[j].size/2)+2;
            asteroid[asteroidIndex].exists = true;
            asteroid[asteroidIndex].setCoords(asteroid[j].getX(),asteroid[j].getY());
            asteroid[asteroidIndex].size = holder;
        size -=holder;
        asteroidIndex++;

        }
    }
    }
 }

if(ok){
        if(asteroids == false){
    asteroids = true;
    level++;
    asteroidIndex =3+level;
    for(int i = 0;i<3+level;i++)
        asteroid[i].exists = true;
}
        cls(screen);

 tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();


		ship.work(screen,fElapsedTime);



for(int i = 0;i<200;i++){
        asteroid[i].work(screen,fElapsedTime);
           bullet[i].work(screen,fElapsedTime);
           if(bullet[i].getX()<1.5||bullet[i].getX()>WIDTH-1.5||bullet[i].getY()<1.5||bullet[i].getY()>HEIGHT-1.5)
            bullet[i].exists = false;
}




     if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
        ship.setAngle(fElapsedTime,-3);

     if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
       ship.setAngle(fElapsedTime,3);

    if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
        ship.acceleration();


auto end= chrono::system_clock::now();
chrono::duration<float> elapsed_Time = end - start;
float shoot = elapsed_Time.count();
    if ((GetAsyncKeyState((unsigned short)' ') & 0x8000)&&(shoot>1)){
      start=end;
        bullet[i].exists = true;
        bullet[i].setCoords(cos(ship.getAngle())*ship.getSize()+ship.getX(),sin(ship.getAngle())*ship.getSize()+ship.getY());
        bullet[i].setAngle( ship.getAngle());
        bullet[i].setVelocity(100);
        i++;
    }








swprintf_s(screen, 80, L"SCORE:%d", score);



		screen[WIDTH*HEIGHT - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, WIDTH*HEIGHT, { 0,0 }, &dwBytesWritten);
}else{
    level = 0;
    score = 0;
    asteroidIndex = 3;
    i = 0;
char message[100][100]={
    {
        "########################"
    },
    {
        "#       GAME OVER      #"
    },
    {
        "# Press 'R' to restart #"
    },
    {
        "########################"
    }
    };




for(int i = 0;i<=3;i++)
    for(int j = 0;j<24;j++)
screen[WIDTH*(i+HEIGHT/2-4)+j+WIDTH/2-10] = message[i][j];

screen[WIDTH*HEIGHT - 1] = '\0';
WriteConsoleOutputCharacter(hConsole, screen, WIDTH*HEIGHT, { 0,0 }, &dwBytesWritten);



for(int i = 0;i<200;i++)
    bullet[i].exists = false;

    for(int i = 0;i<200;i++)
    asteroid[i].exists = false;

if (GetAsyncKeyState((unsigned short)'R') & 0x8000){
 ship.setCoords(60,20);
 ship.setVelocity(3);
 ship.setAngle(5);
 ok = true;
 asteroid[0].exists = true;
 asteroid[1].exists = true;
 asteroid[2].exists = true;
}

}
}

    return 0;
}

