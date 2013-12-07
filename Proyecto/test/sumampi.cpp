#include "../include/ParallelPic.hh"
#include <mpi.h>

Image Image :: sum_par(Image image2)
{
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	
	if(this->get_width() == image2.get_width() && this->get_height() == image2.get_height() && this->get_depth() == image2.get_depth() && this->get_spectrum() == image2.get_spectrum())
	{
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

	}
	return result;
}

int main(int argc, char** argv)
{
	Image img1 (argv[1]);
	Image img2 (argv[2]);
	img1.display("imagen 1");
	img2.display("imagen 2");
		
	int procs;
	int id;
	long local_size=0;
	Image result_local;
	Image result, img2_local, img1_local;
	
	MPI_INIT(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	local_size=floor(img1.get_width()*img1.get_height()*img1.get_depth()*img1.get_spectrum()/procs);
	if(id==0)
	{
		result(img1.get_width(), img1.get_height(), img1.get_depth(), img1.get_spectrum(), 0); 	
	
		//result_local( img1.get_width()/procs, img1.get_height(), img1.get_depth(), img1.get_spectrum(), 0); 	
	}	

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(&local_size,1, MPI_LONG, 0, MPI_COMM_WORLD);
	MPI_Bcast(&result,1, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
	MPI_Bcast(&result_local,1, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
	
	MPI_Scatter(&img1,local_size , MPI_UNSIGNED_CHAR, img1_local, local_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
	MPI_Scatter(&img2,local_size , MPI_UNSIGNED_CHAR, img2_local, local_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

	result_local = img1_local.sum_par(img2_local);
	
	MPI_Gather(&result_local, local_size, MPI_UNSIGNED_CHAR, result, local_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_FINALIZE();
	
	result.display("disp");
}
