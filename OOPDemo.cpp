#include <bits/stdc++.h>
#include <graphics.h>
#include <conio.h>
#include<dos.h>
using namespace std;

class World;
class Bullet
{
public:
    int x, y;
    Bullet(int _x,int _y)
    {
        x = _x+25;
        y = _y-50;
    }
    void moveUp(){
        y -= 25;
    }
    void draw(){///draw bullet
        setcolor(RED);
        setfillstyle(1, RED);
        circle(x, y, 5);
        floodfill(x, y, RED);
    }
};

class Artilary {
    public:
    int x,y;
    Artilary(){
        x = 400;
        y = 477;
    }
    void moveLeft(){
        x -= 20;
    }
    void moveRight(){
        x += 20;
    }
    void moveUp(){
       y -= 20;
    }
    void moveDown(){
        y += 20;
    }
    void fire(vector<Bullet *> &blist){
        blist.push_back(new Bullet(x,y));
    }
    bool Artilarycollitionx1(){
        return x <= -10;
    }
    bool Artilarycollitionx2(int width){
        return x +43 >= width;
    }
    bool Artilarycollitiony1(){
        return y <= 70;
    }
    bool Artilarycollitiony2(int height){
        return y  >= height;
    }

    void draw(){///draw artilarry
        setcolor(RED);
        setfillstyle(4,4);
        rectangle(x,y-60,x+50,y);///big rectangle
        rectangle((x+20),y-77,(x+30),y-60);///small rectangle
        floodfill(x+25,y-10 ,RED);
    }
};
class Enemy
{
public:
    Artilary ar;
    int x,y,r;
    int health;
    bool leftSwing, downSwing;
    char scr[100];
    Enemy(int _x, int _r){
      leftSwing = downSwing = true;
      x = _x;
      y=90;
      r = _r;
      health = 5;
    }
    void moveDown(){
        y +=10;
    }
   void moveUp(){
        y -=10;
    }
    void moveLeft(){
        x -= 10;
    }
    void moveRight(){
        x += 10;
    }
    bool leftcollition(){
        return x < 2*r;
    }
    bool rightcollition(int width){
        return x+r >= width;
    }
    bool topcollition(){
        return y < 2*r;
    }
    bool bottomcollition(int height){
        return y + r >= height;
    }
    int  Artilarycollition(int x1,int y1){
       int m=0;
        if(((x+r)-x1) >= -5 && ((x+r)-x1)<=75){
            if(y1-(y+r)<=65 && (y1-(y+r))>=0)
            return 0;
        }
        else return 1;
    }
    void draw(){
        setcolor(health);
        setfillstyle(1,health);
        circle(x,y,r);
        floodfill(x,y,health);

    }
};

class World
{
    public:
    Artilary *ar,atr;
    char scr[100];
    vector<Bullet *> bulletList;
    vector<Enemy *> enemyList;
    int gd = DETECT, gm, height, width;
    bool gameOver;
    int game_clock;
    int score;
    World(){
        initgraph(&gd, &gm, "C:\\TC\\BGI");
        width = getmaxx();
        height = getmaxy();
        ar = new Artilary();
        gameOver = true;
        game_clock = 0;
        score = 0;
        srand(time(NULL));
    }
    ~World(){
         closegraph();
    }

    void startGame(){
        outtextxy(200,250,"***PRESS ANY KEY TO START GAME***");
        getch();
    }
    void playerAction(){
        char cmd;
        for(int i=0; i<5; i++){
            if(kbhit()){
                cmd = getch();
                if(cmd == 72 || cmd == 'w' || cmd == 'W'){
                    ar->moveUp();
                    if(ar->Artilarycollitiony1()) ar->moveDown();
                }
                if(cmd == 80 || cmd == 's' || cmd == 'S'){
                    ar->moveDown();
                    if(ar->Artilarycollitiony2(height)) ar->moveUp();
                }
                if(cmd == 77 || cmd == 'D' || cmd == 'd'){
                    ar->moveRight();
                    if(ar->Artilarycollitionx2(width)) ar->moveLeft();
                } else if(cmd == 75 || cmd == 'A' || cmd == 'a'){
                    ar->moveLeft();
                    if(ar->Artilarycollitionx1()) ar->moveRight();
                } else if(cmd == ' ' || cmd == 'f' || cmd == 'F'){
                    ar->fire(bulletList);
                }

            }
        }
    }
    void AIAction(){
        printf("Bullet: %d\n", bulletList.size());
        printf("Enemy: %d\n", enemyList.size());
        printf("Score: %d\n", score*5);
        for(int i=bulletList.size()-1; i>=0; i--){
            if(bulletList[i]->y <= 0){
                bulletList.erase(bulletList.begin()+i);
            } else {
                bulletList[i]->moveUp();
            }
        }
        for(int i = enemyList.size()-1; i>=0; i--){
            if(enemyList[i]->health <= 0){
                enemyList.erase(enemyList.begin()+i);
            } else {
                if(enemyList[i]->downSwing) enemyList[i]->moveDown();
                else enemyList[i]->moveUp();
                if(enemyList[i]->leftSwing) enemyList[i]->moveLeft();
                else enemyList[i]->moveRight();
            }
        }
    }
    void gameLogic(){
        if(game_clock == 1000) gameOver = false;
        if(game_clock == 1) enemyList.push_back(new Enemy(rand()%width, rand()%5+15));
        if(game_clock%100 == 0 && enemyList.size() < 5) enemyList.push_back(new Enemy(rand()%width, rand()%5+15));
        if(enemyList.empty()) enemyList.push_back(new Enemy(rand()%width, rand()%5+10));
        //if(game_clock%70 == 0) enemyList.push_back(new Enemy(rand()%width, rand()%5+15));
        for(int i = enemyList.size()-1; i>=0; i--){
            if(enemyList[i]->leftcollition()) enemyList[i]->leftSwing = false;
            if(enemyList[i]->rightcollition(width)) enemyList[i]->leftSwing = true;
            if(enemyList[i]->topcollition()) enemyList[i]->downSwing = true;
            if(enemyList[i]->bottomcollition(height)) enemyList[i]->downSwing = false;
            gameOver = (enemyList[i]->Artilarycollition(ar->x,ar->y));
        }
        for(int i=0; i<bulletList.size(); i++){
            int xb = bulletList[i]->x;
            int yb = bulletList[i]->y;
            int rb = 5;
            for(int j=0; j<enemyList.size(); j++){
                if(enemyList[j]->health <= 0) continue;
                int xe = enemyList[j]->x;
                int ye = enemyList[j]->y;
                int re = enemyList[j]->r;

                if((xb-xe)*(xb-xe)+(yb-ye)*(yb-ye) < (re+5)*(re+5)){
                    bulletList[i]->y = -10;
                    enemyList[j]->health--;
                    score++;
                }
            }
        }
    }
    void worldPainter(){///shape printing by pointer
        cleardevice();
        ar->draw();
        rectangle(540,5,635,60);
        sprintf(scr, "SCORE: %d", score);
        outtextxy(545,20,scr);
        for(int i=0; i<bulletList.size(); i++){
            bulletList[i]->draw();
        }
        for(int i=0; i<enemyList.size(); i++){
            enemyList[i]->draw();
        }
    }
    void mainLoop(){///all action calling
        startGame();
        while(gameOver){
            game_clock++;
            playerAction();
            AIAction();
            gameLogic();
            worldPainter();
            delay(200);
        }
    }
};

int main()
{
    World w;
    w.mainLoop();
}
