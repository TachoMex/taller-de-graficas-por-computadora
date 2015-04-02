#include <cstring>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <cmath>

using namespace std;

#define SCREEN_X 1040
#define SCREEN_Y 520
#define round(X) (int)floor(X+0.5)
#define MAYOR(a,b) (a>b?a:b)
#define MENOR(a,b) (a<b?a:b)

int xi, yi, xf, yf;
int estadoCurva;
int controlx0, controly0, controlx1, controly1;
bool dibujando;


void pintar(int x, int y){
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void linea(int x1,int  y1,int x2, int y2, void(*accion)(int x, int y)=pintar){
	int dx=abs(x2-x1);
	int dy=abs(y2-y1);
	if(dy==0){
		if(x2<x1)
		swap(x1,x2);
		for(;x1<=x2;x1++)
		accion(x1,y1);
	}else if(dx==0){
		if(y1>y2)
		swap(y1,y2);
		for(;y1<=y2;y1++)
		accion(x1,y1);
		
	}else if(dx==dy){
		if(x1>x2){
			swap(x1,x2);
			swap(y1, y2);
		}
		int incremento=(y1<y2?1:-1);
		for(;x1<=x2;x1++,y1+=incremento){
			accion(x1,y1);
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
			accion(x,y);
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
			accion(x,y);
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
	accion(x2,y2);
	glFlush();
}

void curvaSpline(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4){
	int p, q;
	p=round(x1);
	q=round(y1);
	for(double t=0;t<=1;t+=1.0/45){
		int x=round(pow(1-t, 3)*x1+3*t*pow(1-t, 2)*x3+3*t*t*(1-t)*x4+t*t*t*x2);
		int y=round(pow(1-t, 3)*y1+3*t*pow(1-t, 2)*y3+3*t*t*(1-t)*y4+t*t*t*y2);
		linea(p,q, x, y);
		p=x;
		q=y;
	}
}

void curva(int x, int y){
    glClear(GL_COLOR_BUFFER_BIT);
	switch(estadoCurva){
		case 0:
			controlx1=x;
			controly1=y;
			curvaSpline(xi,SCREEN_Y- yi, xf, SCREEN_Y-yf, xf,SCREEN_Y- yf,xf,SCREEN_Y- yf);
			break;
		case 1:
			curvaSpline(xi,SCREEN_Y-yi, controlx1, SCREEN_Y-controly1, x,SCREEN_Y-y, x ,SCREEN_Y- y);
			controlx0=x;
			controly0=y;
			break;
		case 2:
			curvaSpline(xi,SCREEN_Y- yi, controlx1, SCREEN_Y-controly1, controlx0, SCREEN_Y-controly0, x, SCREEN_Y-y);
			break;
	}
}

void renderFunction(){
	    glClear(GL_COLOR_BUFFER_BIT);
	if(dibujando){
		curva(xf, yf);
		glFlush();
	}
	glFlush();
}

void eventoClick(int b , int e, int x, int y){
	if(b==GLUT_LEFT_BUTTON and e==GLUT_DOWN){
		dibujando=true;
		if(estadoCurva==2){
			xi=x;
			yi=y;
		}
		estadoCurva=(estadoCurva+1)%3;
	}else if(b==GLUT_LEFT_BUTTON and e==GLUT_UP){
		dibujando=false;
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
	estadoCurva=2;
    glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(SCREEN_X,SCREEN_Y);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Curvas");
	glClearColor(0, 0,0, 0.0);
	glMatrixMode(GL_PROJECTION);
    glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(255,255,0);
	gluOrtho2D(0.0, SCREEN_X, 0.0, SCREEN_Y); 
	glFlush();
	glutMouseFunc(eventoClick);
	glutMotionFunc(eventoArrastre);
	glutDisplayFunc(renderFunction);
	glutMainLoop();   
	return 0;
}