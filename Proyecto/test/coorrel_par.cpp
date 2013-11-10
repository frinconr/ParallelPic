#include "../include/ParallelPic.hh"
#include <omp.h>

Image Image :: coorrelogram_par(unsigned int ver,unsigned int hor, unsigned int z, unsigned int c)
{
	unsigned int i,j,y,x,pixel;
	
	Image result (256,256, 1, 1, 0);	
	
	for(unsigned int i = 0; i < 256; i++)
	{
		for(unsigned int j=0; j< 256; j++)
		{
			unsigned int pixel = 0;
			
			for(unsigned int x=0; x< (this->get_width()-hor);++x)
			{
				#pragma omp parallel for schedule(dynamic, this->get_width()-hor) private(pixel) shared(i,j,x,result)
					
				for(unsigned int y=0; y< (this->get_height()-ver);++y)
				{
					unsigned char first = (this->get_pixel_value(x,y,z,c));
					unsigned char secnd = (this->get_pixel_value(x+hor, y+ver, z, c));
					if(first == i && secnd == j)
					{
						pixel ++;
					}
				}
			}
			if(pixel>255)
			{
				pixel=255;
			}
			#pragma omp critical
			result.set_pixel_value(i, j, 0, 0, pixel);
			
		}
		
	}
	return result;

}

int main()
{
	Image img1 ("../../Multimedia/openmp.jpg");
	img1.display("original");
	Image result = img1.coorrelogram_par(3,2,1,1);
	result.display("disp");
	return 0;
}

