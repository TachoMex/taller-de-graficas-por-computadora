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
		for(;x1<=x2;x1++,y1+=incremento){
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
		while(x!=x2 or y!=y2){
			pintar(x,y);
			if(p>=0){
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
		while(y!=y2 or x!=x2){
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
	glDrawPixels(640,480,GL_RGB,GL_UNSIGNED_BYTE,buff);
	glFlush();
	if(dibujando){
		linea(xi,480-yi,xf,480-yf);
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
	glutCreateWindow("Practica 2");
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