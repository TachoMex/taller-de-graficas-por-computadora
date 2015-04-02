#include<cstring>
#include"color.h"
#include<GL/freeglut.h>
#include<GL/gl.h>
class imagen{
	private:
		color*pixels;
		int x, y;
	public:
		imagen(int a, int b){
			x=a;
			y=b;
			pixels= new color[x*y];
		}
		
		imagen(){
			pixels=NULL;
			x=y=0;
		}
		
		imagen(const imagen &i){
			x=i.x;
			y=i.y;
			pixels=new color[x*y];
			memcpy(pixels,i.pixels, x*y);
		}
		
		int filas(){
			return y;
		}
		
		int columnas(){
			return x;
		}
		
		color& en(int i, int j){
			if(i < 0 or i >= y or j < 0 or j >= x )
				throw 0;
			//std::cout<<i*x+j<<std::endl;
			return pixels[i*x+j];
		}

		
		void drawAt(int h, int k){
			for(int i=0;i<y;i++)
				for(int j=0;j<x;j++){
					int p,q;
					p=h+i;
					q=k+j;
					try{
						color c=en(i,j);
						glColor3ub(c.r,c.g,c.b);
						glBegin(GL_POINTS);
							glVertex2i(q, p);
						glEnd();
					}catch(int e){
					}
				}
				glFlush();
		}
		
		~imagen(){
			delete pixels;
		}
		
		imagen operator=(imagen i){
			delete pixels;
			x=i.x;
			y=i.y;
			pixels=new color[x*y];
			memcpy(pixels,i.pixels, x*y);
			return i;
		}

		

		void guardaBMP(const char *nombre){
			std::ofstream f(nombre);
			f.put('B'); f.put('M'); //Tipo
			int tam=54+x*y;
			f.write((char*)&tam,sizeof(int)); 
			int reservado=0;
			f.write((char*)&reservado,sizeof(int));  
			int offset=0x36;
			f.write((char*)&offset,sizeof(int));  
			int tamCabeceras=40;
			f.write((char*)&tamCabeceras,sizeof(int));
			f.write((char*)&(x),sizeof(int)); //ancho
			f.write((char*)&(y),sizeof(int)); //alto
			short planos=1;
			f.write((char*)&planos,sizeof(short));
			short bits=24;
			f.write((char*)&bits,sizeof(short));
			int compresion=0;
			f.write((char*)&compresion,sizeof(int));
			f.put(0); f.put(0); f.put(0); f.put(0); //Tamaño Paleta
			f.put(0); f.put(0); f.put(0); f.put(0); //BitsPorMetroX
			f.put(0); f.put(0); f.put(0); f.put(0); //BitsPorMetroY
			f.put(0); f.put(0); f.put(0); f.put(0); //Colores Usados
			f.put(0); f.put(0); f.put(0); f.put(0); //Colores Importantes
			int ajuste=(4-(x*3)%4)%4;
			//std::cout<<f.tellp()<<std::endl;
			for(int i=0;i<y;i++){
				for(int j=0;j<x;j++)
					try{f<<en(i,j);}catch(int t){f<<color();}
				for(int j=0;j<ajuste;j++)
					f.put(0);
			}
			f.close();
		}
		
		void leerBMP(const char *nombre){
			delete pixels;
			std::ifstream f(nombre);
			if(f.get()!='B' or f.get()!='M'){
				std::cout<<"No es BMP"<<std::endl;
			}
			int tam;
			f.read((char*)&tam,sizeof(int));
			f.get(); f.get(); f.get(); f.get(); //Reservado
			f.get(); f.get(); f.get(); f.get(); //Offset
			f.get(); f.get(); f.get(); f.get(); //Tamaño cabecera
			f.read((char*)&x,sizeof(int)); //x++;
			f.read((char*)&y,sizeof(int)); //y++;
			f.get(); f.get(); //Planos
			short bits;
			f.read((char*)&bits,sizeof(short));
			f.get(); f.get(); f.get(); f.get(); //Compresion
			f.get(); f.get(); f.get(); f.get(); //Tamaño Paleta
			f.get(); f.get(); f.get(); f.get(); //BitsPorMetroX
			f.get(); f.get(); f.get(); f.get(); //BitsPorMetroY
			f.get(); f.get(); f.get(); f.get(); //Colores Usados
			f.get(); f.get(); f.get(); f.get(); //Colores Importantes
			pixels=new color[x*y];
			int ajuste=(4-(x*3)%4)%4;
			for(int i=0;i<y;i++){
				for(int j=0;j<x;j++){
					//std::cout<<x-j<<std::endl;
					f>>en(i,j);
				}
				for(int j=0;j<ajuste;j++)
					f.get();
			}
			f.close();
		}
		
		

		void read(){
			delete pixels;
			x=glutGet(GLUT_WINDOW_WIDTH);
			y=glutGet(GLUT_WINDOW_HEIGHT);
			pixels = new color[x*y];
			glReadPixels(0,0,x,y,GL_RGB,GL_UNSIGNED_BYTE,pixels);
		}

		void read(int x2, int y2){
			delete pixels;
			x=x2;
			y=y2;
			pixels = new color[x*y];
			glReadPixels(0,0,x,y,GL_RGB, GL_UNSIGNED_BYTE, pixels);

		}
		
		void draw(){
			glDrawPixels(x,y,GL_RGB,GL_UNSIGNED_BYTE, pixels);
		}
};
