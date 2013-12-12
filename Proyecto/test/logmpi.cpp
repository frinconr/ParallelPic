#include <mpi.h>
#include "../../../ParallelPic/Proyecto/include/ParallelPic.hh"

int main(int argc, char** argv)
{
	Image img1 (argv[1]);
	img1.display("imagen 1");
	int size=img1.get_width()*img1.get_height()*img1.get_depth()*img1.get_spectrum();
	int *matrix= (int*)malloc(size*sizeof(int));
	int *mat_result= (int*)malloc(size*sizeof(int));
	Image result(img1.get_width(), img1.get_height(), img1.get_depth(), img1.get_spectrum(), 0); 
	int x,y,z,c, procs, id, local_size, i,*matrix_local, *result_local;
	i=0;
	//creamos una matriz de enteros a partir de la imagen

	double time;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	time=MPI_Wtime();
	
	local_size=size/procs;
	
	//if(id==0)
	{
	
		for(c=0; c< img1.get_spectrum();++c)
		{
			for(z=0; z< img1.get_depth(); ++z)
			{	
				for(x=0; x<img1.get_width();++x)
				{					
					for(y=0; y< img1.get_height();++y)
					{	

						matrix[img1.get_height()*x+y+img1.get_height()*img1.get_width()*z+img1.get_height()*img1.get_width()*img1.get_depth()*c]=img1.get_pixel_value(x,y,z,c);
						
					}	
				}
			}
		}		
		//crea las matrices locales que debe tener cada thread
		matrix_local=(int*)malloc(local_size*sizeof(int));
		result_local=(int*)malloc(local_size*sizeof(int));
	}	
	
	MPI_Barrier(MPI_COMM_WORLD);
	cout<<local_size<<"   "<<size<<endl;
	MPI_Scatter(matrix, local_size , MPI_INT, matrix_local, local_size, MPI_INT, 0, MPI_COMM_WORLD);
	free(matrix);
	
	for(i=0; i<local_size; ++i)
	{
		result_local[i] = (255/log(256)) * log(1+matrix_local[i]);
	
	}
	free(matrix_local);
	MPI_Gather(result_local, local_size, MPI_INT, mat_result, local_size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	free(result_local);
	time=MPI_Wtime()-time;
	MPI_Finalize();
	// Esto lo único que hace es volver a construir la matriz

	for(c=0; c< img1.get_spectrum();++c)
	{
		for(z=0; z< img1.get_depth(); ++z)
		{	
			for(x=0; x<img1.get_width();++x)
			{					
				for(y=0; y< img1.get_height();++y)
				{	

					result.set_pixel_value(x,y,z,c,static_cast<unsigned char>(mat_result[img1.get_height()*x+y+img1.get_height()*img1.get_width()*z+img1.get_height()*img1.get_width()*img1.get_depth()*c]));
				}	
			}
		}
	}
	free(mat_result);
	cout<<"Tiempo de ejecución con "<<procs<<" procesadores: "<< time <<endl;
	result.display("disp");
	result.save("logmpi.jpg");
	return 0;
}
