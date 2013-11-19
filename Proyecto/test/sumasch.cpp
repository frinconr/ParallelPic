#include "../include/ParallelPic.hh"
#include <omp.h>

Image Image :: operator+ (Image image2)
{
	unsigned int c,z,x,y,pixel,sum=0;
	 
	int n = this->get_height()/omp_get_num_threads();
	
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	if(this->get_width() == image2.get_width() && this->get_height() == image2.get_height() && this->get_depth() == image2.get_depth() && this->get_spectrum() == image2.get_spectrum())
	{
		
		for(c = 0; c < this->get_spectrum(); c++)
		{

			for(z = 0; z < this->get_depth(); z++)
			{
			//#pragma omp parallel for schedule(dynamic,this->get_height()) private(sum,x,y) shared(z,c,result)
				for(x = 0; x < this->get_width(); x++)
				{
					for (int m=0; m<omp_get_num_threads(); ++m)
					{	
						#pragma omp parallel for schedule(dynamic, n) private(sum,y) shared(x,z,c,result)
						for(y = m*n; y < n*(m+1); y++)
						{
							sum= this->get_pixel_value(x,y,z,c)+image2.get_pixel_value(x,y,z,c);
						
							if (sum <= 255)
							{
								pixel = static_cast<unsigned int>(sum);
							}
						
							else
							{
								pixel = 255;
							}
							//#pragma omp critical
							result.set_pixel_value(x,y,z,c,pixel);
						}
					}
				}
			}
		}
	}
		
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
