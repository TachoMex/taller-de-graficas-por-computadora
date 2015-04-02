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
	//printf("(%d,%d)\n",x,y);
	glBegin(GL_POINTS);
		glVertex2i(x, y);
	glEnd();
}

void elipse(int h, int k, double a, double b){
	int x=0;
	int y=b;
	int p;
	p=int(b*b-a*a*b+0.25*a*a);
	pintar(h+x,k+y);
	pintar(h-x,k+y);
	pintar(h+x,k-y);
	pintar(h-x,k-y);
	
	while(x*b*b < y*a*a){
		if(p<0){
			p=p+2*x*b*b+b*b;
		}else{
			y--;
			p=p+2*x*b*b+b*b-2*y*a*a;
		}
		x++;
		pintar(h+x,k+y);
		pintar(h-x,k+y);
		pintar(h+x,k-y);
		pintar(h-x,k-y);
	}
	
	p=int(b*b*(x+0.5)*(x+0.5)+a*a*(y-1)*(y-1)-a*a*b*b);
	
	while(y>0){
		if(p>0){
			y--;
			p=p-2*a*a*y+a*a;
		}else{
			x++;
			y--;
			p=p+2*b*b*x-2*a*a*y+a*a;
		}
		pintar(h+x,k+y);
		pintar(h-x,k+y);
		pintar(h+x,k-y);
		pintar(h-x,k-y);
	}
	glFlush();
}
void renderFunction(){
	glDrawPixels(640,480,GL_RGB,GL_UNSIGNED_BYTE,buff);
	glFlush();
	if(dibujando){
		elipse(xi,480-yi,abs(xf-xi), abs(yf-yi));
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
	glutCreateWindow("Practica 4");
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