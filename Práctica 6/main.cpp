#include<cstring>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include<cmath>
#include<GL/freeglut.h>
#include<GL/gl.h>

using namespace std;

#define SCREEN_X 1040
#define SCREEN_Y 520
#define SPRAY_RAD 15
#define RAND_DOUBLE (double(rand()%10000)/10000.0)

bool dibujando;
void (*activa)(int, int );
int xi, yi, xf, yf;


void pintar(int x, int y){
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void borrar(int x, int y){
	float colores[4];
	float colores2[4];
	glGetFloatv(GL_CURRENT_COLOR, colores);
	glGetFloatv(GL_COLOR_CLEAR_VALUE, colores2);
	glColor4f(colores2[0], colores2[1], colores2[2], colores2[3]);
	for(int i=-10;i<=10;i++)
		for(int j=-10;j<=10;j++)
				pintar(x+i, y+j);
	glColor4f(colores[0], colores[1], colores[2], colores[3]);
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

void lapiz(int x, int y){
	linea(xi,SCREEN_Y-yi, x, SCREEN_Y-y);
	xi=x;
	yi=y;
}

void borrador(int x, int y){
	linea(xi, SCREEN_Y-yi, x, SCREEN_Y-y, borrar);
	xi=x;
	yi=y;
}

void spray(int x, int y){
	y=SCREEN_Y-y;
	for(int i=0;i<SPRAY_RAD*SPRAY_RAD-(SPRAY_RAD-1)*(SPRAY_RAD-1);i++){
		double r=RAND_DOUBLE*SPRAY_RAD;
		double ang=2*M_PI*RAND_DOUBLE;
		pintar(round(x+r*cos(ang)), round(y+r*sin(ang)));
	}
}



void renderFunction(){
	activa(xf, yf);
	glFlush();
}

void renderSpray(){
	if(dibujando){
		spray(xf, yf);
		glFlush();
		glutTimerFunc(300, (void(*)(int))renderSpray, 0);
	}
}

void eventoClick(int b , int e, int x, int y){
	if(b==GLUT_LEFT_BUTTON and e==GLUT_DOWN){
		dibujando=true;
		xi=x;
		yi=y;
		if(activa==spray){
			xf=x;
			yf=y;
			glutTimerFunc(300, (void(*)(int))renderSpray, 0);
		}
		if(activa==lapiz or activa==borrador){
			xf=xi;
			yf=yi;
			renderFunction();
		}
	}else  if(b==GLUT_LEFT_BUTTON and e==GLUT_UP){
		dibujando = false;
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
	switch(k){
		case 'p':
			glutDisplayFunc(renderFunction);
			activa=lapiz;
		break;
		case 'b':
			glutDisplayFunc(renderFunction);
			activa=borrador;
		break;
		case 's':
			activa=spray;
			glutDisplayFunc(renderSpray);
		break;
	}
}


int main(int argc, char** argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(SCREEN_X,SCREEN_Y);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Herramientas");
	glClearColor(0, 0,0, 0.0);
	glMatrixMode(GL_PROJECTION);
    glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(255,255,0);
	gluOrtho2D(0.0, SCREEN_X, 0.0, SCREEN_Y); 
	activa=lapiz;
	glutMouseFunc(eventoClick);
	glutMotionFunc(eventoArrastre);
	glutDisplayFunc(renderFunction);
	glutKeyboardFunc(eventoTeclado);
	glutMainLoop();   
	return 0;
}