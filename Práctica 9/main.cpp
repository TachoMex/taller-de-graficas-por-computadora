#include<cstring>
#include<cstdlib>
#include<ctime>
#include<GL/freeglut.h>
#include<GL/gl.h>
#include"imagen.h"

imagen M;

void renderFunction(){
	M.drawAt(0,0);
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv){
	M.leerBMP("catedral.bmp");
	M.guardaBMP("dibujo.bmp");

    
    glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(M.columnas(), M.filas());
	glutInitWindowPosition(0,0);
	glutCreateWindow("BMP");
	glClearColor(1, 1,1, 0.0);
	glMatrixMode(GL_PROJECTION);
    glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(255,255,0);
	gluOrtho2D(0.0, M.columnas(), 0.0, M.filas()); 
	glFlush(); glutSwapBuffers(); 
	

	glutDisplayFunc(renderFunction);
	glutMainLoop();   
	return 0;
}