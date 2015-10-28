//Steven Mike 901849187
#include "gfxF.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define XSIZE	500
#define YSIZE	390

//Creates a linked list to keep track of the lasers the character shoots
typedef struct goodLaser {
	int xpos;
	int ypos;
	int dmg;
	double speed;
	struct goodLaser *next;
} goodLaserType;

//Function prototypes
void movement(int*,int*,int[]);
void drawChar(int,int);
void drawGoodLasers(goodLaserType *,goodLaserType *,goodLaserType **,int[][2],int*);
void drawEnemies(int[][5]);
int updateEnemies(int[][5],goodLaserType *,goodLaserType *,int*,int*,int*);
void displayScore(char[],int,int);
void displayLives(char[],int);
void createEnemies(int[][5]);
void callWave(int,int[][5]);
void boss(int[][5]);
void enemyLasers(int[][5],int[][2],int[][2],int*,int*);
void drawEnemyLasers(int[][2],int[][2],int[][5]);
int hit(int,int,int[][2],int[][2],int*,int*);
void gameOver(int,int,int);
int main(){

	char choice = '0';//holds inputs
	int i=0; //holds number
	int j; //used for a for loop
	int las=0; //holds position of lasers that wont hit enemies
	int *plas=&las; //pointer to las
	int goodLas[50][2]; //array that holds lasers that wont hit enemies
	char str[20];
	int xpos = XSIZE/2;//starting x position
	int ypos = 350;//starting y position
	int *pxpos = &xpos;//pointer to xpos
	int *pypos = &ypos;//pointer to ypos
	int keys[]={0,0,0,0,0}; //holds inputs for a,s,d,w, and space
	double laserSpeed = 7.5; //speed of good lasers
	int score = 0; 
	int dmg = 1;//damage of good lasers
	int dead = 0;//number of dead enemies
	int *pdmg = &dmg;//pointer to dmg
	int *pdead = &dead;//pointer to dead
	int lives = 3;//number of lives
	int *plives = &lives;//pointer to lives
	int lasers1[5][2];//array of enemy lasers
	int lasers2[5][2];//array of enemy lasers
	int *pscore = &score;//pointer to score
	int shotsFired = 0;//number of good shots fired
	int time = 0;//incrementer to use for time delays
	int *ptime = &time;//pointer to time
	int inv = 1;//boolean to see if character is invincible or not
	int time2=0;//incrementer for time delay
	int enemies[10][5]; //Holds the enemies, with ints that store information such as x and y position, dx, health, on or off field
	goodLaserType *p,*head = NULL, *tail;//creates three instances 
	goodLaserType **phead = &head;//pointer to head

	
	gfx_open(XSIZE,YSIZE,"Final Project");
	
	for(j=0;j<5;j++) lasers2[j][1]=0;//sets to 0
	for(j=0;j<50;j++) goodLas[j][1]=0;//sets to 0
	createEnemies(enemies); //Sets up values for the enemies

	while (choice != 'p'){
		
		gfx_clear();
		movement(pxpos,pypos,keys);//movement of char

		drawChar(xpos,ypos);

	
		
		if (keys[0]){ //creates good lasers if space was pushed
			p = (goodLaserType *) malloc(sizeof(goodLaserType));
			if (!head){
				head = tail = p;//if head is empty starts list
			}
			p->xpos = xpos;
			p->ypos = ypos;
			p->dmg = dmg;
			p->speed = laserSpeed;
			tail->next = p;//creating linked list
			tail = p;
			tail->next = NULL;//end of list
			shotsFired++;//keeps track of number of total shots
		}
		drawGoodLasers(p,head,phead,goodLas,plas);
		if (enemies[9][4]==0){
			 drawEnemies(enemies);//draws enemies except for boss
		}

		else{
			boss(enemies);//draws boss
			i=1;//used to detect end of program
		}

		//Updates the enemies with positioning and dmg info
		if(updateEnemies(enemies,p,head,pscore,pdmg,pdead)){
			if (head){	
				head->ypos=-1;
				head = head->next;
			}
		}
	
		//Creates and draws enemy lasers, damages character if hit
		enemyLasers(enemies,lasers1,lasers2,ptime,pdead);
		drawEnemyLasers(lasers1,lasers2,enemies);

		
		if (inv){
			//determing time of invincibility
			time2++;
			if (time2 > 10){
				time2=0;
				inv = 0;
			}
		}

		else{
			//hits character if character isn't invincible
			inv = hit(xpos,ypos,lasers1,lasers2,plives,pscore);
		}
		callWave(dead,enemies); //Sets the waves of enemies to appear at the appropriate time
	
		displayScore(str,score,shotsFired);
		displayLives(str,lives);
		gfx_flush();
		usleep(60000);
		time++;
		choice = '0';

		if (gfx_event_waiting()){
			choice = gfx_wait(keys);
		}


		if (lives <= 0) break; //ends program when lives run out

		//ends the program once boss is dead and screen is updated
		if (enemies[9][4]==0 && i >= 1){
			if(i==2) break;
			i=2;
		}
	}
	
	//Gives the user results of the game
	gameOver(score,lives,shotsFired);

	return 1;
}


void movement(int *xpos, int *ypos, int keys[]){

        int dx=0;
        int dy=0;

	//determines movement based on the w,a,s,or d keys pushed
        if (keys[3]){
                dy = -5;
        }

        if (keys[1]){
                dx = -5;
        }

        if (keys[2]){
                dy = 5;
        }

        if (keys[4]){
                dx = 5;
        }

	//updates position
        if (*xpos + dx < 0){
                dx = 1;
        }

        else if (*xpos + dx > XSIZE){
                dx = -1;
        }

        if (*ypos + dy < 0){
                dy = 1;
        }
	
	else if (*ypos + dy > YSIZE){
                dy = -1;
        }

        *xpos += dx;
        *ypos += dy;
}

void drawChar(int xpos, int ypos){
	
	int i;
	gfx_color(0,0,255);
	//draws a blue circle with lines in the middle
	gfx_ellipse(xpos,ypos,10,10);
	for (i=0;i<360;i+=45){
		gfx_line(xpos,ypos,xpos+10*cos(i*3.141592/180),ypos-10*sin(i*3.141592/180));
	}

}

void drawGoodLasers(goodLaserType *p, goodLaserType *head, goodLaserType **phead,int goodLas[][2],int *plas){

	int i;
	int first = 0; //Checks for the first valid laser
	gfx_color(218,4,218);
	p = head; //Starts the list at head
	while (p){
		if (p->ypos < 109){
			//puts useless lasers in array instead of linked list
			if(*plas==50) *plas = 0;
			goodLas[*plas][0] = p->xpos;
			goodLas[*plas][1] = p->ypos;
			*plas = *plas + 1;	
			p = p->next;
			continue;
		}
		if (first==0){
			//sets head of linked list
			*phead = p;
			first++;
		}
		gfx_line(p->xpos,p->ypos,p->xpos,p->ypos-5);
		p->ypos -= p->speed;
		p = p->next;
	}

	for (i=0;i<50;i++){
		if (goodLas[i][1]>0){
			gfx_line(goodLas[i][0],goodLas[i][1],goodLas[i][0],goodLas[i][1]-5);
			goodLas[i][1] = goodLas[i][1]-7;
		}
	}

//sets head to null if no lasers in list
if (first == 0) *phead = NULL;

}

void drawEnemies(int enemies[][5]){

	int i;
	gfx_color(255,0,0);
	//Draws the enemies as red squares
	for (i=0;i<10;i++){
		if (enemies[i][4]){
			gfx_rectangle(enemies[i][0]-10,enemies[i][1]-10,20,20);
			gfx_fill_rectangle(enemies[i][0]-10, enemies[i][1]-10,20,20);	
		}
	}
}

int updateEnemies(int enemies[][5], goodLaserType *p, goodLaserType *head, int *pscore,int *pdmg, int *pdead){

	int score = 0;
	int i;
	int hit=0;
	

	p=head;
		for (i=0;i<10;i++){
			if (enemies[i][4]){
				//if enemies are hit, they lose life
				if (p){
					if((p->xpos >= enemies[i][0]-10 && p->xpos <= enemies[i][0]+10) && (p->ypos-5 <= enemies[i][1]+10 && p->ypos >= enemies[i][1]-10)){

						enemies[i][3] -= p->dmg;
						*pscore += p->dmg;
						hit = 1;
						if (enemies[i][3]<=0){
							enemies[i][4]=0;
							*pdmg *= 2;
							*pdead += 1;
						}
					}
				}
		//updates enemy positioning
				if (enemies[i][0] + enemies[i][2] < 0 || enemies[i][0] + enemies[i][2] > XSIZE){
                			enemies[i][2] *= - 1;
        			}
				enemies[i][0] += enemies[i][2];

			}
		}
	
	return hit;
}

void displayScore(char str[], int score, int shotsFired){

	score = score - (shotsFired + 1) * .2;//scoring formula

	if (score<0) score=0;
	gfx_color(255,255,255);
	sprintf(str,"Score: %d",score);
	gfx_text(50,20,str);
}

void displayLives(char str[],int lives){

	gfx_color(255,255,255);	
	sprintf(str,"Lives: %d",lives);
	gfx_text(450,20,str);
}

void createEnemies(int enemies[][5]){

	int i;


	//Set up first Enemy
	enemies[0][0] = XSIZE/2; //Xposition
	enemies[0][1] = 100; //Yposition
	enemies[0][2] = 0; //Dx
	enemies[0][3] = 10; //Healh
	enemies[0][4] = 1; //Boolean determining prescence

	for (i=1; i<10; i++){
		enemies[i][4]=0;
	}

	
	enemies[1][0] = XSIZE/4; //Xposition
	enemies[1][1] = 100; //Yposition
	enemies[1][2] = 0; //Dx
	enemies[1][3] = 20; //Healh



	enemies[2][0] = 3*XSIZE/4; //Xposition
	enemies[2][1] = 100; //Yposition
	enemies[2][2] = 0; //Dx
	enemies[2][3] = 20; //Healh



	enemies[3][0] = XSIZE/2; //Xposition
	enemies[3][1] = 100; //Yposition
	enemies[3][2] = 5; //Dx
	enemies[3][3] = 160; //Healh



	enemies[4][0] = XSIZE/4; //Xposition
	enemies[4][1] = 100; //Yposition
	enemies[4][2] = 0; //Dx
	enemies[4][3] = 160; //Healh



	enemies[5][0] = 3*XSIZE/4; //Xposition
	enemies[5][1] = 100; //Yposition
	enemies[5][2] = 0; //Dx
	enemies[5][3] = 160; //Healh



	enemies[6][0] = XSIZE/4; //Xposition
	enemies[6][1] = 100; //Yposition
	enemies[6][2] = 5; //Dx
	enemies[6][3] = 1280; //Healh



	enemies[7][0] = 3*XSIZE/4; //Xposition
	enemies[7][1] = 100; //Yposition
	enemies[7][2] = -5; //Dx
	enemies[7][3] = 1280; //Healh



	enemies[8][0] = XSIZE/2; //Xposition
	enemies[8][1] = 100; //Yposition
	enemies[8][2] = 10; //Dx
	enemies[8][3] = 7680; //Healh



	enemies[9][0] = XSIZE/2; //Xposition
	enemies[9][1] = 100; //Yposition
	enemies[9][2] = 5; //Dx
	enemies[9][3] = 25600; //Healh

}

void callWave(int dead, int enemies[][5]){

	//determines when to start drawing the next enemies
	switch (dead){
	
		case 1:
			enemies[1][4] = 1;
			enemies[2][4] = 1;
			break;
		case 3:
			enemies[3][4] = 1;
			break;
		case 4: 
			enemies[4][4] = 1;
			enemies[5][4] = 1;
			break;
		case 6:
			enemies[6][4] = 1;
			enemies[7][4] = 1;
			break;
		case 8:
			enemies[8][4] = 1;
			break;
		case 9: 
			enemies[9][4] = 1;
			break;
	}
}


void boss(int enemies[][5]){
	//draws the boss purple instead of red
	int i;
	gfx_color(75,0,130);
	if (enemies[9][4]){
		gfx_rectangle(enemies[9][0]-10,enemies[9][1]-10,20,20);
		gfx_fill_rectangle(enemies[9][0]-10, enemies[9][1]-10,20,20);	
	}
}

void enemyLasers(int enemies[][5], int lasers1[][2], int lasers2[][2],int *ptime,int*pdead){
	int i,j;
	int k = 0;
	int div = 40;
	//every 40 ticks, enemies fire (every 20 for boss)
	if (enemies[9][4]==1) div = 20;
	if (*ptime%div==0){
	
		for(j=0;j<10;j++){	

			if(enemies[j][4]==1){
				if (k==0){
					for(i=0;i<5;i++){
						lasers1[i][0]=enemies[j][0];
						lasers1[i][1]=enemies[j][1];
					}
					*ptime=0;
					k++;
				}

				else{
					//allows two enemies to shoot 
					for(i=0;i<5;i++){
						lasers2[i][0]=enemies[j][0];
						lasers2[i][1]=enemies[j][1];
					}
				}
			}
		}
	}	
}

void drawEnemyLasers(int lasers1[][2],int lasers2[][2],int enemies[][5]){

	int i;
	int speed = 7;
	//draws the enemy lasers in the proper position using trigonometry
	if(enemies[9][4]==1) speed = 14;
	gfx_color(255,255,255);
	for (i=0;i<5;i++){
		if (i!=2){
			gfx_line(lasers1[i][0],lasers1[i][1],lasers1[i][0]+5*cos((210+30*i)*3.141592/180),lasers1[i][1]-5*sin((210+30*i)*3.141592/180));

			lasers1[i][0] += speed*cos((210+30*i)*3.141592/180);
			lasers1[i][1] -= speed*sin((210+30*i)*3.141592/180);


			if (lasers2[i][1] != 0){			
				gfx_line(lasers2[i][0],lasers2[i][1],lasers2[i][0]+5*cos((210+30*i)*3.141592/180),lasers2[i][1]-5*sin((210+30*i)*3.141592/180));

				lasers2[i][0] += speed*cos((210+30*i)*3.141592/180);
				lasers2[i][1] -= speed*sin((210+30*i)*3.141592/180);
			}
		}
	}


	//For some reason 270degrees wasn't straight down, so I did this
	gfx_line(lasers1[2][0],lasers1[2][1],lasers1[2][0],lasers1[2][1]+5);

	lasers1[2][1] += speed;

	if (lasers2[2][1] != 0){	

		gfx_line(lasers2[2][0],lasers2[2][1],lasers2[2][0],lasers2[2][1]+5);

		lasers2[2][1] += speed;
	}
		
}

int hit(int xpos, int ypos, int lasers1[][2],int lasers2[][2], int *plives, int *pscore){

	int i;
	//determines if character is hit and takes away a life and lowers score
	for (i=0;i<5;i++){

	
		if((lasers1[i][0] >= xpos-10 && lasers1[i][0] <= xpos+10) && (lasers1[i][1] <= ypos+10 && lasers1[i][1]+5 >= ypos-10)){
			*plives = *plives-1;
			*pscore = *pscore/2;
			lasers1[i][1]=YSIZE*2;
			return 1;
		}


		if((lasers2[i][0] >= xpos-10 && lasers2[i][0] <= xpos+10) && (lasers2[i][1] <= ypos+10 && lasers2[i][1]+5 >= ypos-10)){
			*plives = *plives-1;
			*pscore = *pscore/2;
			lasers2[i][1]=YSIZE*2;
			return 1;
		}
	}

	return 0;
}	

void gameOver(int score, int lives,int shotsFired){

	char c='a';	
	int i;
	FILE *fp;
	char *p;
	char highscore[20];
	char name[4];
	char hold[15];
	int high;
	char newName[4];
	char newHighscore[20];
	FILE *fpw;

	//reads the highscore from a textfile
	fp = fopen("highscore.txt","r");
	fgets(highscore,80,fp);	
	for(i=0;i<3;i++){
		name[i]=highscore[i];
	}
	name[3] = '\0';
	i = 3;

	while (c != '\n' && c != '\0'){
		c = highscore[i];
		hold[i-3] = c;
		i++;
	}

	hold[i]='\0';

	high = strtol(hold,&p,10);	


	score = score - (shotsFired + 1) * .2;
	
	//Determines if the player won oro last and prints scores
	if (score<0) score = 0;
	if (lives == 0){
		printf("\nYou lost. Better luck next time.\n");
		printf("\nHigh Score: %s - %d\n",name, high);
		printf("Your Score: %d\n",score);
	}

	else {
		printf("\nCongratulations, you won!.\n");
		printf("\nHigh Score: %s - %d\n",name, high);
		printf("Your Score: %d\n",score);
	}

	if(score>high){
		//if player's score is better than highschore, the player is prompted to put in his or her 3 initials and the textfile is overwritten
		//I am trusting that the user only inputs 3 initials
		fpw = fopen("highscore.txt","w");
		printf("\nPlease enter your 3 initial: ");
		scanf("%s",newName);
		

	sprintf(newHighscore,"%s%d",newName,score);
	fprintf(fpw,"%s",newHighscore);
	}		

}
