#include<cstdio>
#include<algorithm>
#include<cmath>
#include<GL/freeglut.h>
#include<GL/gl.h>
#include<cstring>
using namespace std;

void pintar(int x, int y){
	//printf("(%d,%d)\n",x,y);
	glBegin(GL_POINTS);
		glVertex2i(x, y);
	glEnd();

}

void linea(int x1,int  y1,int x2,int y2){
	int dx=abs(x2-x1);
	int dy=abs(y2-y1);
	if(dy==0){
		if(x2<x1)
			swap(x1,x2);
		for(;x1<=x2;x1++)
			pintar(x1,y1);
	}else if(dx==0){
		if(y1>y2)
			swap(y1,y2);
		for(;y1<=y2;y1++)
			pintar(x1,y1);
			
	}else if(dx==dy){
		if(x1>x2){
			swap(x1,x2);
			swap(y1, y2);
		}
		int incremento=(y1<y2?1:-1);
		for(;x1<=x2;x1++,y1++){
			pintar(x1,y1);
		}
	}
	else if(dy<dx){
		if(x1>x2){
			swap(x1,x2);
			swap(y1, y2);
		}
		int x=x1, y=y1;
		int incremento=(y1<y2?1:-1);
		int p=0;
		p=2*dy-dx;
		while(x!=x2 and y!=y2){
			pintar(x,y);
			if(p>0){
				x++;
				y+=incremento;
				p=p+2*dy-2*dx;
			}else{
				x++;
				p=p+2*dy;
			}
		}
	}else{	
		if(y1>y2){
			swap(x1,x2);
			swap(y1, y2);
		}
		int y=y1, x=x1;
		int incremento=(x1<x2?1:-1);
		int p=0;
		p=2*dx-dy;
		while(y!=y2 and x!=x2){
			pintar(x,y);
			if(p>0){
				y++;
				x+=incremento;
				p=p+2*dx-2*dy;
			}else{
				y++;
				p=p+2*dx;
			}
		}
	}
	pintar(x2,y2);
	glFlush();
}

void renderFunction(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
	linea(10,10,630,10);
	linea(630,10,630,460);
	linea(630,460,10,460);
	linea(10,460,10,10);
	linea(20,240,620,240);
	linea(320,20,320,450);
	linea(20,240,320,450);
	linea(320,450,620,240);
	linea(620, 240, 320,20);
	linea(320,20,20,240);
}


int main(int argc, char** argv){
    glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Practica 1");
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	   gluOrtho2D(0.0, 640.0, 0.0, 480.0); //  proyección en 2D

	glutDisplayFunc(renderFunction);
	glutMainLoop();   
	return 0;
}