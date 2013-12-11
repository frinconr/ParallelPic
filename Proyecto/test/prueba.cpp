#include "../include/ParallelPic.hh"


int main()
{
	clock_t time;  
	
	Image img1 ("../../Multimedia/huge.jpg");
	
	
	time= clock();
	Image result= img1.filter_median(3);
	time= clock()-time;
	cout<<"Tiempo de ejecucion secuencial (s) : "<<((float)time)/CLOCKS_PER_SEC<<endl;
	result.display("result");
	
	return 0;
}
 
