#ifndef HERRAMIENTAS_H
#define HERRAMIENTAS_H
#include"imagen.h"
#include"primitivas.h"
#include<cstdlib>
#include<ctime>
#include<cstdio>
#include<fstream>
#include<stack>

#define SCREEN_X 1040
#define SCREEN_Y 600
#define RAND_DOUBLE (double(rand()%10000)/10000.0)

color colorUso;
color colorFondo(255,255,255);

int xi, yi, xf, yf;
bool dibujando;
bool borrable;
imagen M;
imagen auxR;
imagen auxR2;
int l;
int estadoCurva;
int estadoRecorte;
int controlx0, controly0, controlx1, controly1;
#define SPRAY_RAD 15



void elipse(int x, int y){
	int a=abs(x-xi);
	int b=abs(y-yi);
	primitivas::elipse(xi,SCREEN_Y-yi, a, b);
	glFlush();
}

void poligono(int x, int y){
	double dx=xi-x;
	double dy=yi-y;
	int r=floor(sqrt(dx*dx+dy*dy));
	double ang=atan2(dy,dx);
	primitivas::poligono(xi,SCREEN_Y-yi,r,M_PI-ang, l);
}

void circulo(int x, int y){
	int difh=xi-x;
	int difk=yi-y;
	double r=sqrt(difh*difh+difk*difk);
	primitivas::circulo(xi, SCREEN_Y-yi, r);
}

void recta(int x, int y){
	primitivas::linea(xi, SCREEN_Y-yi, x, SCREEN_Y-y);
}

void lapiz(int x, int y){
	primitivas::linea(xi,SCREEN_Y-yi, x, SCREEN_Y-y);
	xi=x;
	yi=y;
}

void borrador(int x, int y){
	primitivas::linea(xi, SCREEN_Y-yi, x, SCREEN_Y-y, primitivas::borrar);
	xi=x;
	yi=y;
}

void spray(int x, int y){
	y=SCREEN_Y-y;
	for(int i=0;i<SPRAY_RAD*SPRAY_RAD-(SPRAY_RAD-1)*(SPRAY_RAD-1);i++){
		double r=RAND_DOUBLE*SPRAY_RAD;
		double ang=2*M_PI*RAND_DOUBLE;
		primitivas::pintar(round(x+r*cos(ang)), round(y+r*sin(ang)));
	}
}

void estrella(int x, int y){
	double dx=xi-x;
	double dy=yi-y;
	int r=floor(sqrt(dx*dx+dy*dy));
	double ang=atan2(dy,dx);
	primitivas::estrella(xi,SCREEN_Y-yi,r,M_PI-ang, l);
}

void corazon(int x, int y){
	int a=abs(x-xi);
	int b=abs(y-yi);
	primitivas::corazon(x+(x>xi?-a/2:a/2),SCREEN_Y-y+(y<yi?-b/2:b/2)-b/2,a/2,b);
}

void curva(int x, int y){
	M.draw();
	// printf("%d ", estadoCurva);
	switch(estadoCurva){
		case 0:
			controlx1=x;
			controly1=y;
			primitivas::curvaSpline(xi,SCREEN_Y- yi, xf, SCREEN_Y-yf, xf,SCREEN_Y- yf,xf,SCREEN_Y- yf);
			break;
		case 1:
			primitivas::curvaSpline(xi,SCREEN_Y-yi, controlx1, SCREEN_Y-controly1, x,SCREEN_Y-y, x ,SCREEN_Y- y);
			controlx0=x;
			controly0=y;
			break;
		case 2:
			primitivas::curvaSpline(xi,SCREEN_Y- yi, controlx1, SCREEN_Y-controly1, controlx0, SCREEN_Y-controly0, x, SCREEN_Y-y);
			break;
	}
}

void ilumina(int x, int y){
	y=SCREEN_Y-y;
	std::stack<int> cola;
	cola.push(y);
	cola.push(x);
	M.read();
	color c;
	try{
		 c=M.en(y,x);
	}catch(int e){
		return;
	}
	
	if(c==colorUso)
		return;
	int dx[]={0,-1,1,0};
	int dy[]={-1,0,0,1};
	while(!cola.empty()){
		int h, k;
		h=cola.top();
		cola.pop();
		k=cola.top();
		cola.pop();
		//printf("%d %d %d \n",x, y, (int)cola.size());
		if(h<939)
		for(int i=0;i<4;i++){
			x=h+dx[i];
			y=k+dy[i];
			try{
				color pix = M.en(y,x);
				if(pix==c){
					M.en(y,x) = colorUso;
					cola.push(y);
					cola.push(x);
				}
			}
			catch(int e){
			}
		}
	}
	M.drawAt(0,1);
	glFlush();
	
}

void viewer(int x, int y){
	y=SCREEN_Y-y;
	try{
		colorUso = M.en(y,x);
		glColor3ub (colorUso.r, colorUso.g, colorUso.b);
	}catch(int e){
	}
}

void recorte(int x, int y){
	y=SCREEN_Y-y;
	if(x>=940)
		x=939;
	if(y>=SCREEN_Y)
		y=SCREEN_Y-1;
	if(x<0)
		x=0;
	if(y<0)
		y=0;


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
			primitivas::linea(h,k,h,q);
			primitivas::linea(h,k,p,k);
			primitivas::linea(h,q,p,q);
			primitivas::linea(p,k,p,q);
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


#endif
