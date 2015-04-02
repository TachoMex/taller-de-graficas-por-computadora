#include"imagen.h"
#include<algorithm>
using namespace std;

#define round(X) (int)floor(X+0.5)
#define MAYOR(a,b) (a>b?a:b)
#define MENOR(a,b) (a<b?a:b)

#define SCREEN_X 777
#define SCREEN_Y 520

color colorFondo(255,255,255);
color colorUso;

int xi, yi, xf, yf;
bool dibujando;
imagen M;
imagen auxR;

int estadoRecorte;
int controlx0, controly0, controlx1, controly1;

	void pintar(int x, int y){
		if(x >= 940)
			return;
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

void recorte(int x, int y){
	if(estadoRecorte==0)
		M.drawAt(0,1);
	
	if(estadoRecorte  and (xi > controlx1 or xi < controlx0 or yi > controly1 or yi < controly0)){
		estadoRecorte=0;
		M.read();
	}
	
	
	switch(estadoRecorte){
		case 0:
			int h,k;
			int p,q;
			h=MENOR(x,xi);
			p=MAYOR(x,xi);
			k=MENOR(y,yi);
			q=MAYOR(y,yi);
			controlx0=h;
			controlx1=p;
			controly0=k;
			controly1=q;
			linea(h,k,h,q);
			linea(h,k,p,k);
			linea(h,q,p,q);
			linea(p,k,p,q);
			glFlush();
		break;
		case 1:
			M.drawAt(0,1);
			auxR.drawAt(y-yi+controly0,x-xi+controlx0);
			glColor3ub(colorUso.r, colorUso.g, colorUso.b);
			glFlush();
		break;
	}
}

void renderFunction(){
	M.drawAt(0,1);

	if(dibujando){
		recorte(xf, yf);
		glFlush(); 
	}
}


void eventoClick(int b , int e, int x, int y){
	y=SCREEN_Y-y;
	if(x>=SCREEN_X)
		x=SCREEN_X-1;
	if(y>=SCREEN_Y)
		y=SCREEN_Y-1;
	if(x<0)
		x=0;
	if(y<0)
		y=0;

	if(b==GLUT_LEFT_BUTTON and e==GLUT_DOWN){
		dibujando=true;
		xi=x;
		yi=y;	
	}else if(b==GLUT_LEFT_BUTTON and e==GLUT_UP){
		dibujando=false;
		if(estadoRecorte==0){
			M.region(controlx0,controly0,controlx1 -controlx0,controly1 -controly0, auxR);
			for(int i=controlx0;i<controlx1;i++)
				for(int j=controly0;j<controly1;j++)
					M.en(j,i) = colorFondo;
			M.drawAt(0,1);
			auxR.drawAt(controly0,controlx0+1);
			glFlush();  
			glColor3ub(colorUso.r, colorUso.g, colorUso.b);
			estadoRecorte=1;
		}else{
			controly0=y-yi+controly0;
			controly1=y-yi+controly1;
			controlx0=x-xi+controlx0;
			controlx1=x-xi+controlx1;
			M.read();	
		}
		
	}
}

void eventoArrastre(int x, int y){
	y=SCREEN_Y-y;
	if(x>=SCREEN_X)
		x=SCREEN_X-1;
	if(y>=SCREEN_Y)
		y=SCREEN_Y-1;
	if(x<0)
		x=0;
	if(y<0)
		y=0;

	xf=x;
	yf=y;
	
	glutPostRedisplay();
}



int main(int argc, char** argv){
    glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(SCREEN_X,SCREEN_Y);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Recorte");
	glClearColor(1, 1,1, 0.0);
	glMatrixMode(GL_PROJECTION);
    glClear(GL_COLOR_BUFFER_BIT);
    imagen i;
    M.leerBMP("catedral.bmp");
    M.drawAt(0,0);


	glColor3ub(255,255,0);
	colorUso=color(255,255,0);
	gluOrtho2D(0.0, SCREEN_X, 0.0, SCREEN_Y); 
	glFlush();  
	glutMouseFunc(eventoClick);
	glutMotionFunc(eventoArrastre);
	glutDisplayFunc(renderFunction);
	glutMainLoop();   
	return 0;
}