#include<cstdio>
#include<algorithm>
#include<cmath>
#include<GL/freeglut.h>
#include<GL/gl.h>
#include<cstring>
#include<ctime>
#include<cstdlib>

using namespace std;

int buff[1000000];

int xi, yi, xf, yf;

bool dibujando=false;


void pintar(int x, int y){
	glBegin(GL_POINTS);
		glVertex2i(x, y);
	glEnd();

}

void puntosCirc(int h,int k, int x, int y){
	pintar(h+x, k+y);
	pintar(h-x, k+y);
	pintar(h+x, k-y);
	pintar(h-x, k-y);
	pintar(h+y, k+x);
	pintar(h-y, k+x);
	pintar(h+y, k-x);
	pintar(h-y, k-x);
}

void circulo(int h, int k, int r){
	int x, y;
	x=0;
	y=r;
	double p;
	p = 1-r;

	puntosCirc(h, k, x, y);

	while(x<y){
		x++;
		if(p<0){
			p+=2*x+1;
		}else{
			y--;
			p+=2*(x-y)+1;
		}
		puntosCirc(h, k, x, y);
	}
	glFlush();
}


void renderFunction(){
	glDrawPixels(640,480,GL_RGB,GL_UNSIGNED_BYTE,buff);
	glFlush();
	if(dibujando){
		double dx=abs(xi-xf);
		double dy=abs(yi-yf);
		double r=floor( sqrt(dx*dx+dy*dy) );
		circulo(xi,480-yi,(int)r);
	}
}

void eventoClick(int b , int e, int x, int y){
	if(b==GLUT_LEFT_BUTTON and e==GLUT_DOWN){
		dibujando=true;
		xi=x;
		yi=y;
	}else if(b==GLUT_LEFT_BUTTON and e==GLUT_UP){
		dibujando=false;
		glReadPixels(0,0,640,480,GL_RGB,GL_UNSIGNED_BYTE,buff);
	}else if(b==GLUT_RIGHT_BUTTON and e==GLUT_DOWN){
		srand((unsigned)time(NULL));
		glColor3ub(rand()%256,rand()%256,rand()%256);
	}

	
}

void eventoArrastre(int x, int y){
	xf=x;
	yf=y;
	glutPostRedisplay();
}


int main(int argc, char** argv){
    glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Practica 3");
	glClearColor(0, 0,0, 0.0);
	glMatrixMode(GL_PROJECTION);
    glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(255,255,0);
	gluOrtho2D(0.0, 640, 0.0, 480); //  proyección en 2D
	glReadPixels(0,0,640,480,GL_RGB,GL_UNSIGNED_BYTE,buff);
	glutMouseFunc(eventoClick);
	glutMotionFunc(eventoArrastre);
	glutDisplayFunc(renderFunction);
	glutMainLoop();   
	return 0;
}