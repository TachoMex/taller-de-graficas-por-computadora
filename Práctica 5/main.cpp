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

	int l=5;

void poligono(int x, int y, int r, double ang){
	int h, k;
	int p, q;
	h=(int)floor(r*cos(ang)); 
	k=(int)floor(r*sin(ang)); 
	for(int i=1;i<=l;i++){
		p=(int)floor(r*cos(i*(2.0*M_PI/l)+ang));
		q=(int)floor(r*sin(i*(2.0*M_PI/l)+ang));
		linea(h+x, k+y, p+x, q+y);
		h=p;
		k=q;
	}
}

void renderFunction(){
	glDrawPixels(640,480,GL_RGB,GL_UNSIGNED_BYTE,buff);
	glFlush();
	if(dibujando){
		double dx=abs(xi-xf);
		double dy=abs(yi-yf);
		double r=floor( sqrt(dx*dx+dy*dy) );
		poligono(xi,480-yi,(int)r, -atan2(yf-yi, xf-xi));
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

void eventoTeclado(unsigned char k, int x, int y){
	if(k>='3' and k<='9'){
		l=k-'0';
	}
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
	glutKeyboardFunc(eventoTeclado);
	glutMainLoop();   
	return 0;
}