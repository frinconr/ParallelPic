#include "../include/ParallelPic.hh"
#include <omp.h>


Image Image::average_omp(int dim){
	
	unsigned int c,pixel,sum,i,j=0;
	int cont1,cont2=0;
	int n = this->get_spectrum()/(omp_get_num_threads()+1);
	
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	#pragma omp parallel for ordered schedule(dynamic,1) private(sum,c,pixel,i,j) shared(filtered,dim)
	
	for(c =0; c < this->get_spectrum(); c++)
	{
		for(unsigned int z = 0; z < this->get_depth(); z++)
		{
			for(unsigned int x = dim; x < this->get_width()-dim; x++)
			{
				for(unsigned int y = dim; y < this->get_height()-dim; y++)
				{
					//cout<<x<<"   "<<y<<"   "<<z<<"   "<<c<<"->"<<omp_get_thread_num()<<endl;
					int sum = 0;
					for(i = x-dim; i<= x+dim; i++)
					{
						for(j = y-dim; j<= y+dim; j++)
						{			

							sum += this->get_pixel_value(i, j, z, c);
						}
					}
			
					pixel = (unsigned char)static_cast<unsigned char> (sum/((dim*2+1)*(dim*2+1)));
					
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	 return filtered;	
} 
	
int main()
{
	clock_t time;  
	
	Image img1 ("../../Multimedia/openmp.jpg");
	//img1.display("original");
	
	time= clock();
	img1.average_omp(3);
	time= clock()-time;
	cout<<"Tiempo de ejecucion paralela (s) : "<<((float)time)/CLOCKS_PER_SEC<<endl;
	
	
	
	time= clock();
	img1.filter_average(3);
	time= clock()-time;
	cout<<"Tiempo de ejecucion secuencial (s) : "<<((float)time)/CLOCKS_PER_SEC<<endl;
	
	return 0;
}
