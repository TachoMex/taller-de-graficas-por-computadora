#include<cstring>
#include<cstdlib>
#include<ctime>
#include"herramientas.h"


void (*activa)(int, int );

string texto;
int escribiendo;

void dibujaPaleta(){
	glColor3ub(1,1,1);
	for(int i=0;i<100;i++){
		for(int j=0;j<=SCREEN_Y;j++){
			glBegin(GL_POINTS);
					glVertex2i(SCREEN_X-i, j);
				glEnd();
		}
	}

	for(int i=0;i<36; i++){
		color r = hsl(i*10);
		glColor3ub(r.r,r.g,r.b);
		for(int j=0;j<20;j++)
			for(int k=0;k<20;k++){
				glBegin(GL_POINTS);
					glVertex2i(SCREEN_X-100+10 +j + 30*(i>=18), i>=18?36*30-20-i*30+k:i*30+10+k  );
				glEnd();
			}		
	}

	glColor3ub(255,255,255);

	for(int j=0;j<20;j++)
		for(int k=0;k<20;k++){
			glBegin(GL_POINTS);
				glVertex2i(SCREEN_X-120+30 +j, k+SCREEN_Y-50  );
				glVertex2i(SCREEN_X-90+30 +j, k+SCREEN_Y-50  );
			glEnd();
		}	

	glColor3ub(0,0,0);

	for(int j=1;j<19;j++)
		for(int k=1;k<19;k++){
			glBegin(GL_POINTS);
				glVertex2i(SCREEN_X-90+30 +j, k+SCREEN_Y-50  );
			glEnd();
		}	

	double ru,gu,bu;
	double norm = sqrt((int)colorUso.r*colorUso.r+(int)colorUso.b*colorUso.b+(int)colorUso.g*colorUso.g);
	ru = double(colorUso.r)/norm;
	gu = double(colorUso.g)/norm;
	bu = double(colorUso.b)/norm;

	for(int i=0;i<=500;i++){
		glColor3ub(ru*i>255?255:ru*i,gu*i>255?255:gu*i,bu*i>255?255:bu*i);
		glBegin(GL_POINTS);
			for(int j=0;j<2;j++)
				for(int k=0;k<10;k++)
					glVertex2i(SCREEN_X-k-1,i*2+j);
		glEnd();
	}


	glColor3ub(colorUso.r,colorUso.g,colorUso.b);
}


void renderFunction(){
	if(borrable){
		M.drawAt(0,1);
	}
	if(dibujando){
		activa(xf, yf);
		dibujaPaleta();
		glFlush(); glutSwapBuffers(); 
	}
}

void renderSpray(){
	if(dibujando){
		spray(xf, yf);
		glFlush(); glutSwapBuffers(); 
		glutTimerFunc(300, (void(*)(int))renderSpray, 0);
	}
	dibujaPaleta();
}

void eventoClick(int b , int e, int x, int y){
	if(b==GLUT_LEFT_BUTTON and e==GLUT_DOWN){
		if(x>=940){
			if(M.en(SCREEN_Y-y,x)!=color(1,1,1))
			colorUso= M.en(SCREEN_Y-y, x);
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			return;
		}
		dibujando=true;
		if(activa!=curva or estadoCurva==2){
			xi=x;
			yi=y;
		}
		if(activa==recorte){
			xi=x;
			yi=SCREEN_Y-y;
		}
		if(activa==spray){
			xf=x;
			yf=y;
			glutTimerFunc(300, (void(*)(int))renderSpray, 0);
		}
		if(activa==lapiz or activa==borrador or activa==ilumina){
			xf=xi;
			yf=yi;
			renderFunction();
		}
		if(activa==curva){
			estadoCurva=(estadoCurva+1)%3;
		}
	}else if(b==GLUT_LEFT_BUTTON and e==GLUT_UP){
		dibujando=false;
		if(activa==recorte){
			if(estadoRecorte==0){
				M.region(controlx0,controly0,controlx1 -controlx0,controly1 -controly0,auxR);
				for(int i=controlx0;i<controlx1;i++)
					for(int j=controly0;j<controly1;j++)
						M.en(j,i) = colorFondo;
				M.drawAt(0,1);
				auxR.drawAt(controly0,controlx0+1);
			//	auxR.guardaBMP("test.bmp");
				glFlush(); glutSwapBuffers(); 
				glColor3ub(colorUso.r, colorUso.g, colorUso.b);
				estadoRecorte=1;
			}else{
				controly0=(SCREEN_Y-y)-yi+controly0;
				controly1=(SCREEN_Y-y)-yi+controly1;
				controlx0=x-xi+controlx0;
				controlx1=x-xi+controlx1;
			}
		}
		if((activa!=curva  and activa!=recorte)or estadoCurva==2  )
			M.read();
		
	}else if(b==GLUT_RIGHT_BUTTON and e==GLUT_DOWN){
		srand((unsigned)time(NULL));
		colorUso=color(rand()%256,rand()%256,rand()%256);
		glColor3ub(colorUso.r,colorUso.b,colorUso.b);
	}
}

void eventoArrastre(int x, int y){
	xf=x;
	yf=y;
	glutPostRedisplay();
}

void eventoTeclado(unsigned char k, int x, int y){
	printf("%d\n", k);
	imagen i;
	switch(escribiendo){
		case 1:
			M.drawAt(0,0);
			if(k==13){
				i.leerBMP(texto.c_str());
				glClear(GL_COLOR_BUFFER_BIT);
				i.drawAt(0,0);
				dibujaPaleta();
				glFlush(); glutSwapBuffers(); 
				M.read();
				escribiendo=0;
			}else if(k==8){
				texto=texto.substr(0,texto.size()-1);
				primitivas::ponerTexto(0,0,GLUT_BITMAP_TIMES_ROMAN_24,texto.c_str());
				glFlush(); glutSwapBuffers(); 
			}else if(k==27){
				texto=string();
				escribiendo=0;
				M.read();
				glFlush(); glutSwapBuffers(); 
			}
			else{
				texto+=k;
				primitivas::ponerTexto(0,0,GLUT_BITMAP_TIMES_ROMAN_24,texto.c_str());
				glFlush(); glutSwapBuffers(); 
			}
			return;
		case 2:
			M.drawAt(0,0);
			if(k==13){
				M.region(0,0,SCREEN_X-100,SCREEN_Y).guardaBMP(texto.c_str());
				dibujaPaleta();
				glFlush(); glutSwapBuffers(); 
				texto=string();
				escribiendo=0;
			}else if(k==8){
				texto=texto.substr(0,texto.size()-1);
				primitivas::ponerTexto(0,0,GLUT_BITMAP_TIMES_ROMAN_24,texto.c_str());
				glFlush(); glutSwapBuffers(); 
			}else if(k==27){
				texto=string();
				escribiendo=0;
				M.read();
				glFlush(); glutSwapBuffers(); 
			}
			else{
				texto+=k;
				primitivas::ponerTexto(0,0,GLUT_BITMAP_TIMES_ROMAN_24,texto.c_str());
				glFlush(); glutSwapBuffers(); 
			}
			return;		
	}
	
	if(activa==recorte)
		M.read();
	if(k>='3' and k<='9'){
		l=k-'0';
	}else switch(k){
		case 33:
			i.leerBMP("tetera.bmp");
			glClear(GL_COLOR_BUFFER_BIT);
			i.drawAt(0,0);
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			M.read();
			break;
		case 34:
			i.leerBMP("cohete.bmp");
			glClear(GL_COLOR_BUFFER_BIT);
			i.drawAt(0,0);
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			M.read();
			break;
		case 35:
			i.leerBMP("planetas2.bmp");
			glClear(GL_COLOR_BUFFER_BIT);
			i.drawAt(0,0);
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			M.read();
			break;
		case 3:
			glutLeaveMainLoop ();
		break;
		case 7:
			texto=string();
			escribiendo=2;
			break;
		case 1:
			texto=string();
			escribiendo=1;
		break;
		case 'c':
			activa=circulo;
			glutDisplayFunc(renderFunction);
			borrable=true;
		break;
		case 'e':
			glutDisplayFunc(renderFunction);
			activa=elipse;
			borrable=true;
		break;
		case 'p':
			glutDisplayFunc(renderFunction);
			activa=lapiz;
			borrable=false;
			l=3;
		break;
		case 'l':
			glutDisplayFunc(renderFunction);
			activa=recta;
			borrable=true;
		break;
		case 'b':
			glutDisplayFunc(renderFunction);
			activa=borrador;
			borrable=false;
		break;
		case 'o':
			glutDisplayFunc(renderFunction);
			activa=poligono;
			borrable=true;
		break;
		case 's':
			glutDisplayFunc(renderFunction);
			activa=estrella;
			borrable=true;
		break;
		case 'h':
			glutDisplayFunc(renderFunction);
			activa=corazon;
			borrable=true;
		break;
		case 'C':
			glClear(GL_COLOR_BUFFER_BIT);
			glFlush(); glutSwapBuffers(); 
			M.read();
		break;
		case 'S':
			activa=spray;
			glutDisplayFunc(renderSpray);
			borrable=false;
		break;
		case 'u':
			activa=curva;
			glutDisplayFunc(renderFunction);
			borrable=false;
			estadoCurva=2;
		break;
		case 'i':
			activa=ilumina;
			glutDisplayFunc(renderFunction);
			borrable=false;
		break;
		case 'r':
			activa=recorte;
			glutDisplayFunc(renderFunction);
			borrable=false;
			estadoRecorte=1;
			controlx0=-1;
			controly0=-1;
			controlx1=-1;
			controly1=-1;
		break;
		case 'g':
			M.escalaDeGrises().drawAt(0,1);
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			M.read();
		break;
		case 'x':
			M.derivadax().drawAt(0,1);
			dibujaPaleta();
			glFlush(); glutSwapBuffers();
			M.read(); 
		break;
		case 'y':
			M.derivaday().drawAt(0,1);
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			M.read();
		break;
		case 'd':
			M.escalaDeGrises().derivada().drawAt(0,1);
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			M.read();
		break;
		case 'U':
			M.umbral(186, color(), color(255,255,255)).drawAt(0,1);;
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			M.read();
		break;
		case 'G':
			M.filtroGauss().drawAt(0,1);
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			M.read();
		break;
		case 'w':
			M.filtroAfilador().drawAt(0,1);
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			M.read();
		break;
		case 'E':
			M.erosionar().drawAt(0,1);
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			M.read();
		break;

		case 'D':
			M.dilatar().drawAt(0,1);
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			M.read();
		break;

		case 'M':
			M.filtroMediana().drawAt(0,1);
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			M.read();
		break;
		case 'L':
			M.laplace().drawAt(0,1);
			dibujaPaleta();
			glFlush(); glutSwapBuffers(); 
			M.read();
		break;
	}
}


int main(int argc, char** argv){
    glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_X,SCREEN_Y);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Paint");
	glClearColor(1, 1,1, 0.0);
	glMatrixMode(GL_PROJECTION);
    glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(255,255,0);
	colorUso=color(255,255,0);
	gluOrtho2D(0.0, SCREEN_X, 0.0, SCREEN_Y); 
	dibujaPaleta();
	M.read();
	glFlush(); glutSwapBuffers(); 
	activa=lapiz;
	glutMouseFunc(eventoClick);
	glutMotionFunc(eventoArrastre);
	glutDisplayFunc(renderFunction);
	glutKeyboardFunc(eventoTeclado);
	glutMainLoop();   
	return 0;
}
