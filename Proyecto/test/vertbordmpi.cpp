#include <mpi.h>
#include "../../../mylib/imagelib/Proyecto/include/image.hh"

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
	//creamos dos matrices de enteros apartir de la imagen
	
	clock_t time;
	time=clock();
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	
	local_size=size/procs;
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
	
	
	//if(id==0)
	{
		//crea las matrices locales que debe tener cada thread
		matrix_local=(int*)malloc(local_size*sizeof(int));
		result_local=(int*)malloc(local_size*sizeof(int));
	}	
	
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(&local_size,1, MPI_INT, 0, MPI_COMM_WORLD);
	
	/*if(procs >1 && id==procs-1)
	{
		local_size+= size%procs;
		matrix_local=(int*)malloc(local_size*sizeof(int));
		result_local=(int*)malloc(local_size*sizeof(int));
	}*/
	
	//MPI_Bcast(&matrix_local,local_size, MPI_INT, 0, MPI_COMM_WORLD);
	//MPI_Bcast(&matrix,size, MPI_INT, 0, MPI_COMM_WORLD);
	//MPI_Bcast(&result_local,local_size, MPI_INT, 0, MPI_COMM_WORLD);
	
	MPI_Barrier(MPI_COMM_WORLD);
	cout<<local_size<<"   "<<size<<endl;
	MPI_Scatter(matrix, local_size , MPI_INT, matrix_local, local_size, MPI_INT, 0, MPI_COMM_WORLD);
	
	free(matrix);
	
	for(i=1; i<local_size-1; ++i)
	{
		result_local[i] = matrix_local[i-1]-matrix_local[i+1];
	}
	free(matrix_local);
	MPI_Gather(result_local, local_size, MPI_INT, mat_result, local_size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	free(result_local);
	MPI_Finalize();
	// Esto lo único que hace es volver a construir la matriz
	for(c=0; c< img1.get_spectrum();++c)
	{
		for(z=0; z< img1.get_depth(); ++z)
		{	
			for(x=1; x<img1.get_width()-1;++x)
			{					
				for(y=0; y< img1.get_height();++y)
				{	
				
					result.set_pixel_value(x,y,z,c,static_cast<unsigned char>(abs(mat_result[img1.get_height()*x+y+img1.get_height()*img1.get_width()*z+img1.get_height()*img1.get_width()*img1.get_depth()*c])));
				}	
			}
		}
	}
	time=clock()-time;
	free(mat_result);
	cout<<"Tiempo de ejecución con "<<procs<<" procesadores: "<< ((float)time)/CLOCKS_PER_SEC <<endl;
	result.display("disp");

	return 0;
}
