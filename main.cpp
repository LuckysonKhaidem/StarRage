/*TTILE: Star Rage
  DESCRIPTION: A 2d top down space shooting game implmented with openGL
  AUTHOR: Luckyson Khaidem
*/


#include<GL/glut.h>
#include<stdlib.h>
#include<iostream>
#include<math.h> 

#define n 500
#define game 0
#define over 1
int enemyX[481];
GLfloat colors[][3]={{1,0,0},{1,1,1},{0,1,0},{0,0,1}};

using namespace std;
int height=500, width=500;
class star
{
	public:
	
		double x;
		double y;
		void move()
		{
			y--;
		}
		void show()
		{
			glPointSize(1);
			glColor3f(1,1,1);
			glBegin(GL_POINTS);
			glVertex2f(x,y);
			glEnd();
		}
		
	
};

class myship
{
	public:
		double x;
		double y;
		int shoot;
		int explode;
		int alive;
		float r; // Blast radius
		myship()
		{
			x=250;
			y=40;
			shoot=0;
			r=0;
			explode=0;
			alive=1;
		}
		void move_left(int offset)
		{
			x=x-offset;
		}
		void move_right(int offset)
		{
			x=x+offset;
		}
		void explosion()
		{
			float i,j,J;
			float asp;
			for(i=x-r;i<=x+r;i++)
			{
				asp=r*r-(i-x)*(i-x);
				j=sqrt(asp)+y;
				J=-sqrt(asp)+y;
				glBegin(GL_POINTS);
					glVertex2f(i,j);
					glVertex2f(i,J);
				glEnd();
			}
			r++;
			
		}
};

class bullet
{
	public:
		double x;
		double y;
		int firing;
		bullet()
		{
			firing=0;
		}
		void getPosition(myship ship)
		{
			x=ship.x;
			y=ship.y+35;
		}
		void fire()
		{

			firing=1;
		}
		void draw()
		{
			glColor3f(1,0,0);
			glLineWidth(3);
			glBegin(GL_LINES);
				glVertex2f(x,y);
				glVertex2f(x,y+10);
			glEnd();
		}
		void move(int offset)
		{
			y=y+offset;
		}
		void reinit()
		{
			firing=0;
		}
};

class enemy
{
	public:
	double x;
	double y;

	int alive;
	int explode;
	float r; // Blast Radius;
	enemy()
	{
		alive=1;
		explode=0;
		r=0;
	}
	
	void init()
	{
		x=enemyX[rand()%481];
		y=500;
		alive=1;
		explode=0;
		r=0;
	}

	void draw()
	{
		glColor3f(0.9,0.91,0.98);
		glBegin(GL_POLYGON);
			glVertex2f(x-2,y-20);
			glVertex2f(x-10,y-5);
			glVertex2f(x-2,y+20);
			glVertex2f(x+2,y+20);
			glVertex2f(x+10,y-5);
			glVertex2f(x+2,y-20);
			glVertex2f(x-2,y-20);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex2f(x-10,y-5);
			glVertex2f(x-15,y-5);
			glVertex2f(x-15,y+15);
			glVertex2f(x-5,y+10.625);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex2f(x+10,y-5);
			glVertex2f(x+15,y-5);
			glVertex2f(x+15,y+15);
			glVertex2f(x+5,y+10.625);
		glEnd();
		glColor3f(0,0,0);
		glBegin(GL_LINE_STRIP);
			glVertex2f(x-2,y-20);
			glVertex2f(x-10,y-5);
			glVertex2f(x-2,y+20);
			glVertex2f(x+2,y+20);
			glVertex2f(x+10,y-5);
			glVertex2f(x+2,y-20);
			glVertex2f(x-2,y-20);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex2f(x-10,y-5);
			glVertex2f(x-15,y-5);
			glVertex2f(x-15,y+15);
			glVertex2f(x-5,y+10.625);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex2f(x+10,y-5);
			glVertex2f(x+15,y-5);
			glVertex2f(x+15,y+15);
			glVertex2f(x+5,y+10.625);
		glEnd();
			
	}
	void move(float offset)
	{
		y=y-offset;
	}
	void explosion()
	{
		float i,j,J;
		float asp;
		glColor3f(1,1,1);
		for(i=x-r;i<=x+r;i++)
		{
			asp=r*r-(i-x)*(i-x);
			j=sqrt(asp)+y;
			J=-sqrt(asp)+y;
			glBegin(GL_POINTS);
				glVertex2f(i,j);
				glVertex2f(i,J);
			glEnd();
		}
		r++;
		y--;
	}
};
class special
{
	public:
	float y;
	int firing;
	int shoot;
	
	int curcolor;
	special()
	{
			curcolor=0;
	}
	void getPosition(myship ship)
	{
		y=ship.y+30;
	}
	void move()
	{
		glColor3fv(colors[curcolor]);
		glBegin(GL_LINES);
			glVertex2f(0,y);
			glVertex2f(500,y);
			glEnd();
		curcolor=(curcolor+1)%4;
		y=y+2;
	}
};


namespace GameObjects
{
	myship ship;
	star s[n];
	bullet b[30];
	int nobull;
	enemy e[50];
	special sp;
	char score[]="Score:";
	char health[]="Health:";
	int sco;
	int noe=4;
	int level;
	float enemyspeed=2;
	int bulletspeed=26;
	int hea=100;
	int gamestate=game;
	int spec=5;	
}

using namespace GameObjects;



void showstars()
{
	int i;
	
	for(i=0;i<n;i++)
	{
		if(s[i].y >= 0)
		{
			s[i].show();
			s[i].move();
		}
		else
		{
			s[i].y=height;
			s[i].x=rand()%width;
		}	
	}
	
}

void drawship()
{
	
	double x=ship.x;
	double y=ship.y;
	glLineWidth(1);
	glColor3f(1,1,1);
	if(ship.alive)
	{
		 
		
		glColor3f(1,1,1);
			glBegin(GL_LINE_STRIP);
                        glVertex2f(x-5,y-15);
                        glVertex2f(x-15,y-10);
                        glVertex2f(x-20,y-12.5);
                        glVertex2f(x-30,y-5);
                        glVertex2f(x-5,y+15);
                        glVertex2f(x-5,y-15);
                        glEnd();
                glBegin(GL_LINE_STRIP);
                        glVertex2f(x+5,y-15);
                        glVertex2f(x+15,y-10);
                        glVertex2f(x+20,y-12.5);
                        glVertex2f(x+30,y-5);
                        glVertex2f(x+5,y+15);
                        glVertex2f(x+5,y-15);
                        glEnd();
                glBegin(GL_LINE_STRIP);
                        glVertex2f(x-5,y-10);
                        glVertex2f(x,y-12.5);
                        glVertex2f(x+5,y-10);
                        glEnd();
                glBegin(GL_LINE_STRIP);
                        glVertex2f(x-5,y+15);
                        glVertex2f(x-7.5,y+20);
                        glVertex2f(x,y+30);
                        glVertex2f(x+7.5,y+20);
                        glVertex2f(x+5,y+15);
                        glEnd();
                glBegin(GL_LINE_STRIP);
                        glVertex2f(x+5,y+15);
                        glVertex2f(x+5,y-10);
                        glVertex2f(x,y-12.5);
                        glVertex2f(x-5,y-10);
                        glVertex2f(x-5,y+15);
                        glEnd();


	}
	else if(ship.explode)
	{
		ship.explosion();
		if(ship.r == 30)
		{
			gamestate=over;
		}
	}
	if(ship.shoot)
	{
		b[nobull-1].fire();
		b[nobull-1].getPosition(ship);
		ship.shoot=0;
	}

	
}
void drawbullet()
{
	int i;

	for(i=0;i<nobull;i++)
	{
		if(b[i].firing)
		{
			b[i].draw();
			b[i].move(bulletspeed);
		}
		if(b[i].y > 500)
		{
			b[i].reinit();
			
		}
	}
	if(nobull>30)
	{
		nobull=0;
	}
}
void drawenemy()
{
	
	int i;
	for(i=0;i<noe;i++)
	{
		if(e[i].alive)
		{
			e[i].draw();
			e[i].move(enemyspeed);
			if((e[i].y-10) < 0)
			{
				e[i].init();
			}
		}
		if(e[i].explode==1)
		{
			e[i].explosion();
			if(e[i].r == 20)
			{
				e[i].init();
			}
			
		}
	}

	
	
}
void CollisionDetect()
{
	
	int i;
	int j;
	for(i=0;i<nobull;i++)
	{
		for(j=0;j<noe;j++)
		if((e[j].x-10)<=b[i].x && b[i].x <= (e[j].x+10) && e[j].alive)
		{
			if((b[i].y+20)>= (e[j].y-20) && (b[i].y+20) <= (e[j].y+20))
			{
				e[j].alive=0;
				e[j].explode=1;
				b[i].firing=0;
				b[i].x=0;
				b[i].y=0;
				sco +=1;
				level++;
			}
		}
	}
}
void ShipCollision()
{
	
	int i;
	for(i=0;i<noe;i++)
	{
		if(e[i].alive)
		{
			if( ( (ship.x-7.5 >= e[i].x-10) && (ship.x-7.5 <= e[i].x+10) && ship.alive ) || ( (ship.x+7.5 >= e[i].x-10) && (ship.x+7.5 <= e[i].x+10) && ship.alive ) )
			{
				if(((ship.y+15 >= e[i].y-20) && (ship.y+15 <= e[i].y+20) )|| ((ship.y+30 >= e[i].y-20) && (ship.y+30 <= e[i].y+20)))
				{
					hea=hea-5;
					if(hea < 0) {hea=0;ship.alive =0;ship.explode=1;} 
					e[i].alive=0;
					e[i].explode=1;
				}
			}
			if( ( (ship.x-30 >= e[i].x-10) && (ship.x-30 <= e[i].x+10) && ship.alive ) || ( (ship.x+30 >= e[i].x-10) && (ship.x+30 <= e[i].x+10) ) && ship.alive )
			{
				if(((ship.y-15 >= e[i].y-20) && (ship.y-15 <= e[i].y+20) )|| ((ship.y+15 >= e[i].y-20) && (ship.y+15 <= e[i].y+20)))
				{
					hea=hea-2;
					if(hea< 0) {hea=0;ship.alive=0;ship.explode=1;}
					e[i].alive=0;
					e[i].explode=1;
				}
			}
		}
	}
	
}

void specialcollision()
{
	int i;
	if(sp.firing)
	for(i=0;i<noe;i++)
	{
		if(sp.y >= e[i].y-20 && sp.y <= e[i].y+20 && e[i].alive)
		{
			e[i].alive=0;
			e[i].explode=1;
			sco++;
			level++;
		}
	}
}

void displayText()
{
	char spe[]="Special:";
	int temp=sco;
	char ch[3]={'0','0','0'};
	char c;
	int rem;
	glColor3f(0,0,1);
	glRasterPos2f(400,470);
	int i;
	for(i=0;i<sizeof(score);i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,score[i]);
	i=2;
	do
	{
		rem=temp%10;
		temp=temp/10;
		c='0'+rem;
		ch[i]=c;
		i--;
	}while(temp);
	for(i=0;i<sizeof(ch);i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,ch[i]);

 	for(i=0;i<3;i++)
 	{
 		ch[i]='0';
 	}
	 glRasterPos2f(0,470);
        for(i=0;i<sizeof(health);i++)
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,health[i]);
        temp=hea;
        i=2;
        do
        {
                rem=temp%10;
                temp=temp/10;
                c='0'+rem;
                ch[i]=c;
                i--;
        }while(temp);
        for(i=0;i<sizeof(ch);i++)
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,ch[i]);
        glRasterPos2f(0,450);
        for(i=0;i<sizeof(spe);i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,spe[i]);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,c='0'+spec);

}	
void drawspecial()
{
	if(sp.shoot)
	{
		sp.firing=1;
		sp.getPosition(ship);
		sp.shoot=0;
	}
	if(sp.firing)
	{
		sp.move();
		if(sp.y > 500)
		{
			sp.firing=0;
			sp.y=-10;
		}
	}
}
void gamedisplay()
{
	if(level >= 10)
	{
		enemyspeed +=0.5;
		level=0;
		if(noe <= 7) noe++;
	}
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	showstars();
	
	drawship();
	drawenemy();
	drawbullet();
	drawspecial();
	CollisionDetect();
	ShipCollision();
	specialcollision();
	displayText();
	glFlush();
	system("sleep 0.00001");
	glutSwapBuffers();
	glutPostRedisplay();	
}
void move(int x, int y)
{
	ship.x=x;
	glutPostRedisplay();
}
void reshape(int w, int h)
{
	height=h;
	width=w;
	glViewport(0,0,w,h);
	double asp=(float)w/(float)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w<=h)
		glOrtho(0,500,0,500,-1,1);
	else
		glOrtho(0,500,0,500,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'z':	if(ship.alive)
					{ship.shoot=1;
				nobull++;}
			break;
		case 'x': if(ship.alive && !sp.firing && spec > 0)
			{
			sp.shoot=1;
			spec--;
		}
			
		break;
			
	}
	glutPostRedisplay();
}
void overdisplay()
{
	char text[]="GAME OVER";
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3f(0,0,1);
	glRasterPos2f(250,250);
	int i;
	for(i=0;i<sizeof(text);i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,text[i]);
	glEnd();
	glFlush();
}
void display()
{
	if(gamestate == game)
	{
		gamedisplay();
	}
	overdisplay();
	
	
}
void myinit()
{	
	int i;
	int inc=10;
	glClearColor(0,0,0,0);
	for(i=0;i<n;i++)
	{
		s[i].x=rand()%width;
		s[i].y=rand()%height;
		
	}
	for(i=0;i<481;i++)
	{
		enemyX[i]=inc;
		inc++;
	}
}


int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Star Rage");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutPassiveMotionFunc(move);
	glutKeyboardFunc(keyboard);
	glutSetCursor(GLUT_CURSOR_NONE);
	glEnable(GL_TEXTURE_2D);
	myinit();
	glutMainLoop();
}


