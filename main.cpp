/*TTILE: Star Rage
  DESCRIPTION: A 2d top down space shooting game implmented with openGL
  AUTHOR: Luckyson Khaidem
*/


#include<GL/glut.h>
#include<stdlib.h>
#include<iostream>
#include<math.h>
//#include<irrKlang.h>
#define n 500
int enemyX[481];
using namespace std;
//using namespace irrklang;
int height=500, width=500;
//ISoundEngine* engine;
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
		myship()
		{
			x=250;
			y=40;
			shoot=0;
		}
		void move_left(int offset)
		{
			x=x-offset;
		}
		void move_right(int offset)
		{
			x=x+offset;
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
		glBegin(GL_LINE_LOOP);
			glVertex2f(x-10,y-20);
			glVertex2f(x-10,y+20);
			glVertex2f(x+10,y+20);
			glVertex2f(x+10,y-20);
		glEnd();
			
	}
	void move(int offset)
	{
		y=y-offset;
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
		y--;
	}
};

namespace GameObjects
{
	myship ship;
	star s[n];
	bullet b[30];
	int nobull;
	enemy e[50];
	char score[]="Score:";
	int sco;
	int noe=4;
	int level;
	int enemyspeed=2;
	int bulletspeed=5;	
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
				b[i].x=9;
				b[i].y=0;
				sco +=1;
				level++;
			}
		}
	}
}
void displayText()
{
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
}	
void display()
{
	if(level == 10)
	{
		enemyspeed +=1;
		level=0;
		if(noe <= 10) noe++;
		bulletspeed +=2;
	}
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	showstars();
	drawship();
	drawenemy();
	drawbullet();
	CollisionDetect();
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
	cout<<"Height="<<height<<" Width="<<width<<"\n";
	gluOrtho2D(0,width,0,height);
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'z':	ship.shoot=1;
				nobull++;
			
		break;
	
	}
	glutPostRedisplay();
}
void myinit()
{	
	int i;
	int inc=10;
	glClearColor(0,0,0,0);
	gluOrtho2D(0,width,0,height);
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
void soundinit()
{
	engine=createIrrKlangDevice();
	engine->play2D("./Sound/getout.ogg",true);
}
int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Star Rage");
//	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutPassiveMotionFunc(move);
	glutKeyboardFunc(keyboard);
	soundinit();
	myinit();
	glutMainLoop();
}


