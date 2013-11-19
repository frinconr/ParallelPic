#include "../include/ParallelPic.hh"
#include <omp.h>

Image Image :: operator+ (Image image2)
{
	unsigned int pixel,sum=0;
	
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	if(this->get_width() == image2.get_width() && this->get_height() == image2.get_height() && this->get_depth() == image2.get_depth() && this->get_spectrum() == image2.get_spectrum())
	{
	#pragma omp parallel for schedule(dynamic,1)
		for(int c = 0; c < this->get_spectrum(); c++)
		{
	//#pragma omp parallel for schedule(dynamic,1) /*private(sum) private(sum) shared(z,c,result)*/
			for(int z = 0; z < this->get_depth(); z++)
			{
			//#pragma omp parallel for schedule(dynamic,this->get_width()/2) /*private(sum) private(sum) shared(z,c,result)*/
				for(int x = 0; x < this->get_width(); x++)
				{
				//#pragma omp parallel for schedule(dynamic, this->get_height()/2) firstprivate(sum) reduction(+:pixel) shared(x,z,c,result)
					for(int y = 0; y < this->get_height(); y++)
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
								
						#pragma omp critical
						result.set_pixel_value(x,y,z,c,pixel);
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
