#include <omp.h>
#include <iostream>

using namespace std;
int main(){
#pragma omp parallel
{
	#pragma omp
	{
		cout<<"Hello from thread"<< omp_get_thread_num()<<"of"<< omp_get_num_threads()<<endl;
	}
	
}

int chunk_size;	
int a[4] ={1,2,3,4};
int b[4] ={4,5,10,6};
int c=0;
#pragma omp parallel for 

	for (int i=0; i<4; ++i){
		c+=a[i]+b[i];
		//cout<<c<<endl;

	}

cout<<"sec "<<c<<endl;
	

	
}
