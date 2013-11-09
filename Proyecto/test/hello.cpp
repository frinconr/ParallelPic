#include <omp.h>
#include <iostream>

using namespace std;
int main(){
#pragma omp parallel
     cout<<"Hello from thread"<< omp_get_thread_num()<<"of"<< omp_get_num_threads()<<endl;
}
