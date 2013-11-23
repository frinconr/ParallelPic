#include "../include/ParallelPic.hh"
#include <omp.h>


Image Image :: operator+ (Image image2)
{
	unsigned int c,z,x,y,m,pixel,sum=0;
	int contador, cont2;
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	if(this->get_width() == image2.get_width() && this->get_height() == image2.get_height() && this->get_depth() == image2.get_depth() && this->get_spectrum() == image2.get_spectrum())
	{
		
		for(c = 0; c < this->get_spectrum(); ++c)
		{

			for(z = 0; z < this->get_depth(); ++z)
			{
			//for (int m=0; m<=omp_get_num_threads(); ++m)
			
						
				#pragma omp parallel private(sum,x,y,m) shared(z,c,result)
				{
					int n = this->get_width()/omp_get_num_threads();
					#pragma omp for schedule(dynamic, 1) 
					for(m=0;m<omp_get_num_threads(); ++m){	
					for(x = n*m; x < (m+1)*n; ++x)
					{		
						//#pragma omp parallel for schedule(dynamic, n) private(sum,y) shared(x,z,c,result)
						for(y =0; y < this->get_height(); ++y)
						{
							//cerr<<x<<" "<<omp_get_thread_num()<<endl;
						
							sum= this->get_pixel_value(x,y,z,c)+image2.get_pixel_value(x,y,z,c);
						
							if (sum <= 255)
							{
								pixel = static_cast<unsigned int>(sum);
							}
						
							else
							{
								pixel = 255;
							}
							
							if(omp_get_thread_num()==0)
								++contador;
							if(omp_get_thread_num()==1)
								++cont2;
							//sleep(0.01);
							result.set_pixel_value(x,y,z,c,pixel);
						}
						
					}
					
					}
					
				}
			}
		}
	}
	cerr<<contador<<endl;
	cerr<<cont2<<endl;	
	return result;
}

int main()
{
	Image img1 ("../../Multimedia/openmp.jpg");
	img1.display("original");
	Image result = img1+img1;
	result.display("disp");
	return 0;
}
