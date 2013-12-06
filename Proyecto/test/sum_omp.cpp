#include "../include/ParallelPic.hh"
#include <omp.h>
#include <time.h>

Image Image :: operator+ (Image image2)
{
	unsigned int c,pixel,sum=0;
	//int cont1,cont2=0;
	//int n = this->get_spectrum()/(omp_get_num_threads()+1);
	
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	if(this->get_width() == image2.get_width() && this->get_height() == image2.get_height() && this->get_depth() == image2.get_depth() && this->get_spectrum() == image2.get_spectrum())
	{
			
		#pragma omp parallel for ordered schedule(dynamic,1) private(sum,c) shared(result)
		for(c =0; c < this->get_spectrum(); c++)
		
		{
			for(int z = 0; z < this->get_depth(); z++)
			{
				for(int x = 0; x < this->get_width(); x++)
				{
					for(int y=0; y < this->get_height(); y++)
						{
							//cout<<x<<"   "<<y<<"   "<<z<<"   "<<c<<"->"<<omp_get_thread_num()<<endl;
							sum= this->get_pixel_value(x,y,z,c)+image2.get_pixel_value(x,y,z,c);
						
							if (sum <= 255)
							{
								pixel = static_cast<unsigned int>(sum);
							}
						
							else
							{
								pixel = 255;
							}
							
							//if(omp_get_thread_num()==0){++cont1;}
							//if(omp_get_thread_num()==1){++cont2;}
							
							#pragma omp ordered
							result.set_pixel_value(x,y,z,c,pixel);
							
						}
					}
				}
			}
		} 
	
	//cout<<"Cantidad de operaciones del thread 0: "<<cont1<<endl;
	//cout<<"Cantidad de operaciones del thread 1: "<<cont2<<endl;
		
	return result;
}

int main()
{
	clock_t time;  
	
	Image img1 ("../../Multimedia/openmp.jpg");
	img1.display("original");
	
	time= clock();
	Image result = img1+img1;
	time= clock()-time;
	cout<<"Tiempo de ejecucion paralela (s) : "<<((float)time)/CLOCKS_PER_SEC<<endl;
	
	result.display("original");
	
	time= clock();
	img1.sum_img(img1);
	time= clock()-time;
	cout<<"Tiempo de ejecucion secuencial (s) : "<<((float)time)/CLOCKS_PER_SEC<<endl;
	return 0;
}
