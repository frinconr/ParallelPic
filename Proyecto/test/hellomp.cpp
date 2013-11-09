#include <omp.h>
#include <iostream>
 
int main()
{	
	int id;
	#pragma omp parallel 
	{
		id= omp_get_thread_num();
		cout<<"Greeting from process"<<id<<endl;
	}
	return 0;

}
