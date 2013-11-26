#include "../include/ParallelPic.hh"
#include "mpi.h"

Image Image :: sum_par(Image image2)
{
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int procs, id;
	Image result_local ();
	
	if(this->get_width() == image2.get_width() && this->get_height() == image2.get_height() && this->get_depth() == image2.get_depth() && this->get_spectrum() == image2.get_spectrum())
	{
		MPI_Init();
		MPI_Comm_rank(MPI_COMM_WORLD, &id);
		MPI_Comm_size(MPI_COMM_WORLD, &procs);

		if(id==0)
		{
			result_local(this->get_width()/procs , this->get_height(), this->get_depth(), this->get_spectrum(), 0); 	
		}
		
		MPI_Barrier(MPI_COMM_WORLD);	
		//MPI_Bcast(id,this->get_width()/procs, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
		//MPI_Scatter(result_local,this->get_width()/procs , MPI_UNSIGNED_CHAR, result, this->get_width(), MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
		
		for(unsigned int c = 0; c < this->get_spectrum(); c++)
		{

			for(unsigned int z = 0; z < this->get_depth(); z++)
			{

				for(unsigned int x = 0; x < this->get_width(); x++)
				{

					for(unsigned int y = 0; y < this->get_height(); y++)
					{
						unsigned char pixel;
						int sum = this->get_pixel_value(x,y,z,c)+image2.get_pixel_value(x,y,z,c);

								if (sum <= 255)
								{
									pixel = static_cast<unsigned int>(sum);
								}

								if(sum>255)
								{
								pixel = 255;
								}

						result.set_pixel_value(x,y,z,c,pixel);
					}
				}
			}
		}
		//MPI_Reduce(**DIRECCION PRODUCTO LOCAL**, **DIRECCION PRODUCTO GLOBAL**, **CANTIDAD DE DATOS**, MPI_DOUBLE, **OPERACION**, **RECEPTOR**, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);

		MPI_FINALIZE();
	}
	return result;
}

int main()
{
	Image img1 ("../../Multimedia/huge.jpg");
	img1.display("original");
	Image result = img1.sum_par(img1);
	result.display("disp");
	return 0;
}
