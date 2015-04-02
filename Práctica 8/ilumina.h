#include<stack>

#define SCREEN_X 1040
#define SCREEN_Y 520
#include<algorithm>
#include<cmath>
#include<GL/freeglut.h>
#include<GL/gl.h>

void pintar(int x, int y){
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void ilumina(int x, int y){
	y=SCREEN_Y-y;
	std::stack<int> pila;
	pila.push(y);
	pila.push(x);
	unsigned char color[3];
	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);
	int dx[]={0,-1,1,0};
	int dy[]={-1,0,0,1};
	while(!pila.empty()){
		int h, k;
		h=pila.top();
		pila.pop();
		k=pila.top();
		pila.pop();
		for(int i=0;i<4;i++){
			x=h+dx[i];
			y=k+dy[i];
			if(x>2 and x<SCREEN_X-1 and y>0 and y<SCREEN_Y-2){
				unsigned char colorPix[3];
				glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, colorPix);
				if(color[0]==colorPix[0] and color[1]==colorPix[1] and color[2]==colorPix[2]){
					pintar(x, y);
					pila.push(y);
					pila.push(x);
				}
			}
		}
	}
	glFlush();
}
