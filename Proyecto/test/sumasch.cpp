#include "../include/ParallelPic.hh"
#include <omp.h>
#include <time.h>

int main(int argc, char** argv)
{
        clock_t time;
        Image img1 (argv[1]);
        Image img2(argv[2]);
        int number_threads=atoi(argv[3]);
		//img1.display("original");
        time = clock();
        Image result = img1.sum_img(img2,number_threads);
        time = clock() -time;
		//result.display("disp");
        cout<<((float)time)/CLOCKS_PER_SEC<<endl;
        return 0;
}

