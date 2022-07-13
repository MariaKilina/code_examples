#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <math.h>

#define HIGHT 60
#define LENGTH 115 
#define GROUND 45

using namespace std;

class Time {
	public:
	static int TimeCount;
	static void delay(int ms) {
                std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }
};

int Time::TimeCount = 0;

class figure {
	protected:
	char **fig;
        int linelen;
	int linenum;
	int x,y; //offset relative to a higher coordinate system
	public:
	figure() {}
	virtual ~figure() {}
	virtual void draw(int a,int b, char **f)=0;
	virtual void move(int a, int b,int newa, int newb, char **f)=0; 
	virtual void erase(int a, int b, char **f) {
		for (int i=0;i<linenum;++i) {
                	for (int j=0; j<linelen && a+j<LENGTH; ++j) {
                        	if (a<0 && a+j>=0 || a>=0) {
                                	if (fig[i][j]!=' ')
                                        	if (b+i == GROUND)
                                                	f[b+i][a+j]='=';
                                              	else
                                                	f[b+i][a+j]=' ';
                                }
                        }
		}
	}
};


class cactus: public figure {
	public:
		cactus () {
			int b=0;
			linelen = 12;
			linenum = 12;
			fig = new char*[linenum];
			for (int i=0;i<linenum; i++) {
				fig[i] = new char [linelen];
			}
			char s0[] = "     @@     ";	memcpy(fig[b++],s0,linelen);
			char s1[] = "    @@@@    ";	memcpy(fig[b++],s1,linelen);
			char s2[] = "    @@@@  @@";	memcpy(fig[b++],s2,linelen);
			char s3[] = "@@  @@@@  @@";	memcpy(fig[b++],s3,linelen);
			char s4[] = "@@  @@@@  @@";	memcpy(fig[b++],s4,linelen);
			char s5[] = "@@  @@@@  @@";	memcpy(fig[b++],s5,linelen);
			char s6[] = "@@  @@@@  @@";	memcpy(fig[b++],s6,linelen);
			char s7[] = "@@  @@@@@@@ ";	memcpy(fig[b++],s7,linelen);
			char s8[] = " @@@@@@@    ";	memcpy(fig[b++],s8,linelen);
			char s9[] = "    @@@@    ";	memcpy(fig[b++],s9,linelen);
			char s10[]= "    @@@@    ";	memcpy(fig[b++],s10,linelen);
			char s11[] ="    @@@@    ";	memcpy(fig[b++],s11,linelen);
		/*	for (int i=0; i< linenum; i++)
				strcpy(fig[i], s[i]);
			for (int i=0; i<linenum; i++)
				delete []s[i];
			delete []s;*/

		}
		~cactus() {
			for (int i=0; i<linenum; i++)
				delete []fig[i];
			delete []fig;
		}
		void draw(int a, int b, char **frame) {
			for (int i=0; i<linenum; ++i) {
	                        for (int j=0; j<linelen && a+j<LENGTH;++j) {
                              	//	if ((frame[b+i][a+j]==' ' || frame[b+i][a+j]=='=') && fig[i][j] != ' '){
					if (fig[i][j]!=' ') {//(!(frame[b+i][a+j]!=' ' && fig[i][j]==' ')){	
						if (a<0 && a+j>=0 || a>=0) {
							frame[b+i][a+j]=fig[i][j];
						}    
                        		}
                		}
			}
		}
		void move (int a, int b, int newa, int newb, char **f) {
			erase(a,b,f);
			draw(newa, newb,f);
		}
};

class body: public figure {
	bool eye=0;
	public:
		body() {
			x=0;
			y=0;
			int b=0;
			linelen=37;
			linenum=19;
			fig = new char*[linenum];
			for (int i=0; i<linenum; i++) {
				fig[i] = new char[linelen];
			}
			char s0[] = "                    @@@@@@@@@@@@@@@  ";	memcpy(fig[b++],s0,linelen);
			char s1[] = "                   @@@  @@@@@@@@@@@@@";	memcpy(fig[b++],s1,linelen);
			char s2[] = "                   @@@@@@@@@@@@@@@@@@";	memcpy(fig[b++],s2,linelen);
			char s3[] = "                   @@@@@@@@@@@@@@@@@@";	memcpy(fig[b++],s3,linelen);
			char s4[] = "                   @@@@@@@@@         ";	memcpy(fig[b++],s4,linelen);
			char s5[] = "                   @@@@@@@@@         ";	memcpy(fig[b++],s5,linelen);
			char s6[] = "@                @@@@@@@@@           ";	memcpy(fig[b++],s6,linelen);
			char s7[] = "@             @@@@@@@@@@@@           ";	memcpy(fig[b++],s7,linelen);
			char s8[] = "@@         @@@@@@@@@@@@@@@@@@@       ";	memcpy(fig[b++],s8,linelen);
			char s9[] = "@@@      @@@@@@@@@@@@@@@@@  @@       ";	memcpy(fig[b++],s9,linelen);
			char s10[]= "@@@@@@@@@@@@@@@@@@@@@@@@@@           ";	memcpy(fig[b++],s10,linelen);
			char s11[]= "@@@@@@@@@@@@@@@@@@@@@@@@@@           ";	memcpy(fig[b++],s11,linelen);
			char s12[]= "  @@@@@@@@@@@@@@@@@@@@@@             ";	memcpy(fig[b++],s12,linelen);
			char s13[]= "    @@@@@@@@@@@@@@@@@@@              ";	memcpy(fig[b++],s13,linelen);
			char s14[]= "      @@@@@@@@@@@@@@@@               ";	memcpy(fig[b++],s14,linelen);
			char s15[]= "        @@@@@@@@@@@@                 ";	memcpy(fig[b++],s15,linelen);
			char s16[]= "        @@@@@   @@@@                 ";	memcpy(fig[b++],s16,linelen);
			char s17[]= "        @@        @@                 ";	memcpy(fig[b++],s17,linelen);
			char s18[]= "        @@                           ";	memcpy(fig[b++],s18,linelen);
		}
		~body() {
			for(int i=0; i<linenum; i++)
				delete []fig[i];
			delete []fig;
		}
		void draw(int a, int b, char **f) {
			int m=a;
			int n=b;
			for (int i=0; i<linenum; i++) {
				for (int j=0; j<linelen && a+j<LENGTH-1; j++) {
					if (fig[i][j]!=' ') //(!(f[n+i][m+j] !=' ' && fig[i][j] == ' '))
						f[n+i][m+j]=fig[i][j];
				}
			}
		}
		void BigEyes () {
                	int b=0;
                        char s0[] = "                    @@PPPP@@@@@@@@@  ";    memcpy(fig[b++],s0,linelen);
                        char s1[] = "                   @@@ oo @@@@@@@@@@@";    memcpy(fig[b++],s1,linelen);
                        char s2[] = "                   @@@qqqq@@@@@@@@@@@";    memcpy(fig[b++],s2,linelen);
		}
		void move(int a, int b, int newa, int newb, char **f) {
			erase(a,b,f);
			if (newa>a && !eye) {
				BigEyes();
				eye=1;
			}
			draw (newa, newb, f);
		}
};

class jaw: public figure {
	public:
		jaw() {
			x=28;
			y=5;
			linelen=6;
			linenum=1;
			fig = new char*[linenum];
			fig[0] = new char[linelen];
			char s[] = "@@@@@@";	
			memcpy(fig[0],s,linelen);
		}
		~jaw() {
                        for(int i=0; i<linenum; i++)
                                delete []fig[i];
			delete []fig;
                }
		void draw (int a, int b, char **f) {
			int m=a+x;
			int n=b+y;
			for (int i=0; i<linelen && m+i<LENGTH; i++) {
				f[n][m+i]=fig[0][i];
			}
		}
		void move (int a, int b, int newa, int newb, char **f) {
			erase(a,b,f);
			if (!(Time::TimeCount % 6)) 
				if (Time::TimeCount % 12)
					y=5;
				else
					y=4;
			draw(newa,newb,f);		
		}
};

class foot: public figure {
	public:
		foot(int a, int b) {
			x=a;
			y=b;
			linelen=4;
			linenum=1;
			fig = new char*[linenum];
			fig[0] = new char[linelen];
			char s[] = "@@@@";
			memcpy(fig[0],s,linelen);
		}
		~foot() {
                        for(int i=0; i<linenum; i++)
                                delete []fig[i];
			delete []fig;
                }
		void draw (int a, int b, char **f) {
			int m=x+a; int n=y+b;
			for (int i=0; i<linelen && m+i<LENGTH; i++) {
			//	if (fig[0][i] != ' ') //(!(f[n][m+i] !=' ' && fig[0][i] == ' '))
					f[n][m+i]=fig[0][i];
			}
			if (m<LENGTH)
				if (n==GROUND) {
					f[n][m-2]='='; f[n][m-1]='=';
				}
				else {
					f[n][m-2]=' '; f[n][m-1]=' ';
				}
		}
		void move (int a, int b, int newa, int newb, char **f) {
			erase(a,b,f);			
			if (!(Time::TimeCount % 2)){
				if (Time::TimeCount % 4) 
					y=18;
				else
					if(x<10)
						y=19;
					else
						y=17;				
			}
		//	erase(a,b,f);
			draw(newa, newb,f);		
		}
};

			
class picture: public figure {
        protected:
	int count;
        figure **subfigure;
	public:

	~picture() {
		for (int i=0; i<count; i++)
			delete subfigure[i];
		delete []subfigure;
	}
			
	virtual void draw(int a, int b, char **f) {  //offset relative to the scene
                x=a;
		y=b;
                for(int i=0; i<count;i++) {
                	subfigure[i]->draw(x,y,f);
                }
        }
	virtual void move(int a, int b, int newa, int newb, char **f) {	
		for(int i=0; i<count; i++)
			subfigure[i]->move(a, b, newa, newb, f);
		draw(newa,newb, f);
	}
};

class dinosaur: public picture {
	public:
		dinosaur() {
			count=4;
			subfigure = new figure*[count];
			subfigure[0] = new body;
			subfigure[1] = new jaw;
			subfigure[2] = new foot(8,19);
			subfigure[3] = new foot(18,18);
		}
		~dinosaur() {
	//		for (int i=0; i<count; i++)
	//			delete subfigure[i];
	//		delete []subfigure;
		}
};


class scene {
	protected:
	int count=10;
	figure **p;
	int *x, *y;
	public:
	char **frame;
	scene() {
		int i;
		x = new int[count];
		y = new int[count];
		p = new figure*[count];
		for (i=0; i<count; i++)
			p[i]=0;
		frame = new char*[HIGHT];
		for(i=0; i<HIGHT; i++) {
			frame[i] = new char[LENGTH+1];
		}
		clear();
	}
	~scene() {
		int i;
		for (i=0;i<HIGHT;i++) {
			delete []frame[i];
		}
		delete []frame;
		for (i=0; i<count; i++)
			delete p[i];
		delete []p;
		delete []x;
		delete []y;
	}
	void clear() {
		int i,j;
		for (i=0; i<HIGHT; i++) {
			for (j=0; j<LENGTH; j++)
			       if (i==GROUND)
					frame[i][j]='=';
			       else 		
					frame[i][j]=' ';
			frame[i][j] = '\0';
		}
	}
	void print(){
		for (int i=0; i<HIGHT; i++) 
			cout<<frame[i]<<endl;
	}

	virtual void start() {
		p[0] = new dinosaur;
		p[1] = new cactus;
//		p[2] = new cactus;
		int a=30,b=37;
		x[0]=30; y[0]=30;
		x[1]=108; y[1]=37;

		for (int i=0; i<150; i++) {
		//	if (! Time::TimeCount % 5)
		//		system("clear");
			Time::delay(50);
			Time::TimeCount++;
			p[1]->move(x[1],y[1],x[1]-=2,y[1],frame);
			if (x[1]<=x[0]+30 && x[1]>=x[0]+10)
				p[0]->move(x[0],y[0],x[0],y[0]-=2, frame);
			else
				if (x[1]<x[0]+4 && x[1]>=x[0]-18)
					p[0]->move(x[0],y[0],x[0],y[0]+=2,frame);
				else
					p[0]->move(x[0],y[0],x[0],y[0],frame);
			if (x[1]==10) {
				p[2] = new cactus;
				x[2] = 108; y[2] = 37;
			}
			if (p[2]) {
				if (x[2]<=x[0]+65 && x[2]>x[0]+25){
					if (x[2]>x[0]+50) {
						p[2]->move(x[2],y[2],x[2]-2,y[2]-2,frame);
                                       		 x[2]-=2; y[2]-=2;
					}
					else {
						p[2]->move(x[2],y[2],x[2]-2,y[2]-1,frame);
						x[2]-=2; y[2]-=1;
					}
				}
				else
					if (x[2]<x[0]+15 && x[2]>=x[0]-25){
						if (x[2]<x[0]-15) {
							p[2]->move(x[2],y[2],x[2]-2,y[2]+2,frame);
							x[2]-=2; y[2]+=2;
						}
						else {
							p[2]->move(x[2],y[2],x[2]-2,y[2]+1,frame);
                                                        x[2]-=2; y[2]+=1;
						}
					}
					else {
						p[2]->move(x[2],y[2],x[2]-2,y[2],frame);
						x[2]-=2;
					}
			}
			if (i >100){
				p[0]->move(x[0],y[0],x[0]+2,y[0],frame);
				x[0]+=2;
				}
			print();
			clear();

		}
		//system ("clear") ;
	}


};

int main() {
	Time T;
	scene S;
	S.start();
	return 0;
}
