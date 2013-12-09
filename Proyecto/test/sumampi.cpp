#include <mpi.h>
#include "../include/ParallelPic.hh"

int main(int argc, char** argv)
{
	Image img1 (argv[1]);
	Image img2 (argv[2]);
	img1.display("imagen 1");
	img2.display("imagen 2");
	int matrix[img1.get_width()*img1.get_height()*img1.get_depth()*img1.get_spectrum()];
	int matrix2[img1.get_width()*img1.get_height()*img1.get_depth()*img1.get_spectrum()];
	int mat_result [img1.get_width()*img1.get_height()*img1.get_depth()*img1.get_spectrum()];
	
	//creamos dos matrices de enteros apartir de la imagen
	
	for(int z=0; z< img1.get_depth(); ++z)
	{	
		for(int x=0; x<img1.get_height();++x)
		{					
			for(int y=0; y< img1.get_height();++y)
			{	
				for(int c=0; c< img1.get_spectrum();++c)
				{
					matrix[img1.get_height()*x+y+img1.get_height()*img1.get_width()*z+img1.get_height()*img1.get_width()*img1.get_depth()*c]=img1.get_pixel_value(x,y,z,c);
					matrix2[img1.get_height()*x+y+img1.get_height()*img1.get_width()*z+img1.get_height()*img1.get_width()*img1.get_depth()*c]=img2.get_pixel_value(x,y,z,c);
					

				}	
			}
		}
	}
	
	
		
	int procs;
	int id;

	int local_size,size;
	Image result(img1.get_width(), img1.get_height(), img1.get_depth(), img1.get_spectrum(), 0); 
	int *matrix2_local, *matrix_local, *result_local;
	int i=0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	size=img1.get_width()*img1.get_height()*img1.get_depth()*img1.get_spectrum();
	local_size=size/procs;
	
	if(id==0)
	{
		//crea las matrices locales que debe tener cada thread
		matrix2_local[local_size];
		matrix_local[local_size];
		result_local[local_size];
	}	

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(&local_size,1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&matrix,size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&matrix2,size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&matrix_local,local_size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&matrix2_local,local_size, MPI_INT, 0, MPI_COMM_WORLD);
	
	
	MPI_Scatter(matrix,local_size , MPI_INT, &matrix_local, local_size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(matrix2,local_size , MPI_UNSIGNED, &matrix2_local, local_size, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

	cout<<"aqui"<<endl;

	for(i=0; i<local_size; ++i)
		result_local[i] = matrix_local[i]+matrix2_local[i];
	

	MPI_Gather(&result_local, local_size, MPI_UNSIGNED, &mat_result, local_size, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	
	MPI_Finalize();
	
	// Esto lo único que hace es volver a construir la matriz
	for(int z=0; z< img1.get_depth(); ++z)
	{	
		for(int x=0; x<img1.get_height();++x)
		{					
			for(int y=0; y< img1.get_height();++y)
			{	
				for(int c=0; c< img1.get_spectrum();++c)
				{
					result.set_pixel_value(x,y,z,c,static_cast<unsigned char>(mat_result[img1.get_height()*x+y+img1.get_height()*img1.get_width()*z+img1.get_height()*img1.get_width()*img1.get_depth()*c]));
				}	
			}
		}
	}

	
	
	result.display("disp");
}
