#include "../include/ParallelPic.hh"

Image Image :: sum_par(Image image2)
{
	unsigned int c,z,x,y,pixel,sum;
	
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	if(this->get_width() == image2.get_width() && this->get_height() == image2.get_height() && this->get_depth() == image2.get_depth() && this->get_spectrum() == image2.get_spectrum())
	{
		
		for(c = 0; c < this->get_spectrum(); c++)
		{

			for(z = 0; z < this->get_depth(); z++)
			{
			
				for(x = 0; x < this->get_width(); x++)
				{
				#pragma omp parallel private(sum) shared(x,z,c) 
				{
					#pragma omp for ordered schedule(dynamic,1)
					for(y = 0; y < this->get_height(); y++)
					{
						#pragma omp ordered
						sum= this->get_pixel_value(x,y,z,c)+image2.get_pixel_value(x,y,z,c);
						
						if (sum <= 255)
						{
							pixel = static_cast<unsigned int>(sum);
						}
						else
						{
							pixel = 255;
						}
								
					
						result.set_pixel_value(x,y,z,c,pixel);
					}
				}
			}
		}
	}}
		
	
	return result;
}

int main()
{
	Image img1 ("../../Multimedia/openmp.jpg");
	img1.display("soba");
	Image result = img1.sum_par(img1);
	result.display("disp");
	return 0;
}
