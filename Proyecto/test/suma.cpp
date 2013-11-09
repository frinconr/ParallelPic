#include "../include/ParallelPic.hh"

Image Image :: sum_par(Image image2)
{
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	if(this->get_width() == image2.get_width() && this->get_height() == image2.get_height() && this->get_depth() == image2.get_depth() && this->get_spectrum() == image2.get_spectrum())
	{
		#pragma omp sections
		{
		#pragma omp section
		{
		
		for(unsigned int c = 0; c < this->get_spectrum(); c++)
		{

			for(unsigned int z = 0; z < this->get_depth(); z++)
			{

				for(unsigned int x = 0; x < this->get_width(); x++)
				{

					for(unsigned int y = 0; y < this->get_height()/2; y++)
					{
						unsigned char pixel;
						int sum = this->get_pixel_value(x,y,z,c)+image2.get_pixel_value(x,y,z,c);

						{

							{
								if (sum <= 255)
								{
									pixel = static_cast<unsigned int>(sum);
								}
							}
	
							{
								if(sum>255)
								{
								pixel = 255;
								}
							}
						}
						result.set_pixel_value(x,y,z,c,pixel);
					}
				}
			}
		}}
		#pragma omp section
		{
		
		for(unsigned int c = 0; c < this->get_spectrum(); c++)
		{

			for(unsigned int z = 0; z < this->get_depth(); z++)
			{

				for(unsigned int x = 0; x < this->get_width(); x++)
				{

					for(unsigned int y = this->get_height()/2; y < this->get_height(); y++)
					{
						unsigned char pixel;
						int sum = this->get_pixel_value(x,y,z,c)+image2.get_pixel_value(x,y,z,c);

						{

							{
								if (sum <= 255)
								{
									pixel = static_cast<unsigned int>(sum);
								}
							}
	
							{
								if(sum>255)
								{
								pixel = 255;
								}
							}
						}
						result.set_pixel_value(x,y,z,c,pixel);
					}
				}
			}
		}}}}
	
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
