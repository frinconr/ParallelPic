#include <omp.h>
#include <iostream>

using namespace std;
 
int main()
{	
	int n=0;
	int i,j;
	//omp_set_schedule(omp_sched_static,3);
	#pragma omp parallel private(i,j)
	{
	
		#pragma omp for ordered schedule (dynamic,4)
	
			for(i=0;i<6;++i)
				for(j=0;j<2;++j)
				{
					#pragma omp ordered
					//{
					cout<<"n: "<<n<<" num thread"<<omp_get_thread_num()<<endl;
					//}
					n++;
				}
	}
	cout<<"n: "<<n<<endl;
	return 0;

}
