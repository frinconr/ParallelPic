#include "../include/ParallelPic.hh"
#include <omp.h>
#include <iostream>


Image Image :: median_omp (int dim, int num_threads)
{
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	int m = (dim-1)/2;
	unsigned char pixel_values [dim*dim];
	unsigned char temp;
	int chunk;
	omp_set_num_threads(num_threads);
	#pragma omp parallel private(pixel_values,temp,chunk) shared(filtered)
	{
	chunk= (this->get_width()/omp_get_num_threads());
	
	#pragma omp for ordered schedule(dynamic,1)  
	for(unsigned int c = 0; c < this->get_spectrum(); c++)
	{
		for(unsigned int z = 0; z < this->get_depth(); z++)
		{
			
		//#pragma omp parallel for ordered schedule(dynamic,dim+1) private(pixel_values,temp,chunk) shared(z,c,filtered)
		
			for(unsigned int x = m; x < this->get_width(); x++)
			{		
				for(unsigned int y = m; y < this->get_height(); y++)
				{
					for(unsigned int i = x-m; i < x+m; i++)
					{
						for(unsigned int j = y-m; j< y+m; j++)
						{
							pixel_values [(i-x+m)*dim + (j-y+m)]= this->get_pixel_value(i, j, z, c);
							
						}	
					}
					for(int k=0; k<dim*dim ; k++)
					{
						for(int p=k+1 ; p<dim*dim ; p++)
						{
							if(pixel_values[p] < pixel_values[k])
							{
							// Intercambiar los valores
							temp = pixel_values[k];
							pixel_values[k] = pixel_values[p];
							pixel_values[p] = temp;
							}
						}
					}					
					unsigned char pixel = pixel_values[((dim*dim-1)/2)-1];
					
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			}
		}  
	}}
	 return filtered;
 }
 
 
int main()
{
	clock_t time;  
	
	Image img1 ("../../Multimedia/huge.jpg");
	
	
	time= clock();
	Image result=img1.median_omp(3, 2);
	time= clock()-time;
	cout<<"Tiempo de ejecucion paralela (s) : "<<((float)time)/CLOCKS_PER_SEC<<endl;
	result.display("result");

	
	return 0;
}
 
 
