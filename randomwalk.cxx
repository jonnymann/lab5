#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>

using namespace std;

struct colloid{
    double x,y;
};

void init(colloid* const c, const int N);
void print(const colloid* const c, const int N, const string fname);
void condition(int* rx, int* ry, int N){
  for(int k=0; k<N;k++){
	    rx[k]=int(3*(double(rand())/RAND_MAX))-1;
	    ry[k]=int(3*(double(rand())/RAND_MAX))-1;
  }
}

void pusher(colloid* c, int* rx, int* ry, int N, const double step);

void state(colloid* c, int N, double &meanx,double &meany,double &var){
  meanx=0;
  meany=0;
  var=0;
  
  for (int j=0; j<N;j++){
  meanx+= c[j].x;
  }
  meanx=meanx/N;
  
  for (int j=0; j<N;j++){
  meany+= c[j].y;
  }
  meany=meany/N;
  for (int j=0; j<N;j++){
  var+=pow(c[j].x-meanx,2);
  }
  var=var/N;
  
}
  

int main(void){
  
    int N  = 50;
    cout << "Right now # of colloids is set to " << N << ", how many do you want?" << endl;
    cin  >> N;
    
    colloid*  c  = new colloid[N];	// dynamically allocate memory for N particles
    int*      rx = new int[N];		// dynamically allocate memory for movement-conditions in x direction
    int*      ry = new int[N];		// dynamically allocate memory for movement-conditions in y direction
    
    double meanx, meany, var;		// statistics
    ofstream stat("statistics.dat");	// ... to be printed out here
    
    const double step = 0.1;		// step size
    
    init(c, N);				// initialize all colloids
    
    stringstream s;			// set up stringstream -> multiple output files
    string       fname = "rwalk";	// basic name tag for multiple output files
    
    const int Nsteps = 100;		// total # of steps
    const int Nfiles = 10;		// total # of output files
    int Nsubsteps    = Nsteps / Nfiles; // resulting substeps
    
    s.str("");				// empty stringstream
    s << fname << "_0.dat";		// fill in string for initial file
    print(c, N, s.str());		// print initial positions
    
    srand(time(NULL));			// randomize random numbers
    
    for(int i = 1; i <= Nfiles; i++){
	for(int j = 0; j < Nsubsteps; j++){
	  condition(rx,ry,N);
	  pusher(c,rx,ry,N,step);
	  state(c,N,meanx,meany,var);
	    stat << (i-1)*Nsubsteps+j << "\t" << meanx << "\t";
	    stat << meany << "\t" << var << endl;
	}
	s.str("");						// preparations for printing...
	s << fname << "_" << i << ".dat";			// ... colloid positions
	print(c, N, s.str());					
    }
    
    stat.close();			// close statistics file
    delete[] rx,ry,c;			// delete dynamically allocated arrays
    return 0;
}

void init(colloid* const c, const int N){
    for(int i = 0; i < N; i++){
	c[i].x = 0;
	c[i].y = 0;
    }
}

void print(const colloid* const c, const int N, const string fname){
    ofstream out(fname.c_str());
    for(int i = 0; i < N; i++)
	out << c[i].x << "\t" << c[i].y << endl;
    out.close();
}

void pusher(colloid* c, int* rx, int* ry, int N, const double step)
{
    for(int k=0; k<N;k++){
      c[k].x = c[k].x + (rx[k])*step;
      c[k].y = c[k].y + (ry[k])*step;
    }
}

