#include <mpi.h>
#include "../../../ParallelPic/Proyecto/include/ParallelPic.hh"


int main(int argc, char** argv)
{
	// Call to a constructor of the class Image by giving it the name of the picture file.
	
	Image img1 ("../../Multimedia/huge.jpg"); // Image "__name__" ("__image_path__");
	Image img2 ("../../Multimedia/huge.jpg"); // Image "__name__" ("__image_path__");
	
	//It is possible to display the image created before:
	img1.display("imagen 1");
	img2.display("imagen 2");
	
	//OpenMPI works only with primitive types so we need to pass 
	//the images to a vector of integers or other type,
	// if we want to parallelize the operation.
	
	int size=img1.get_width()*img1.get_height()*img1.get_depth()*img1.get_spectrum(); //define the size of the image result
	
	//As the images could be huge, we need to use malloc to apart the block of memory of the vector. 
	//Note that to use MPI the blocks of memory needs to be continuous so we use malloc.
	int *matrix= (int*)malloc(size*sizeof(int)); //img1
	int *matrix2= (int*)malloc(size*sizeof(int)); //img2
	int *mat_result= (int*)malloc(size*sizeof(int)); //result matrix
	
	Image result(img1.get_width(), img1.get_height(), img1.get_depth(), img1.get_spectrum(), 0);  //This is the constructor of the result image
	
	//We need to declare the variables of the function, the num of processes to use, the id of each process and the local parts of the matrix
	int x,y,z,c, procs, id, local_size, i,*matrix_local, *matrix2_local, *result_local;
	i=0;

	double time;

	MPI_Init(&argc, &argv); //This function is used to initialize the parallel section
	MPI_Comm_rank(MPI_COMM_WORLD, &id);// This function obtains the identifier of each process
	MPI_Comm_size(MPI_COMM_WORLD, &procs); // This function obtains the number of processes that be used in the communicator.
	time=MPI_Wtime(); //This function measure the time of the execution
	
	//Set the local size of each chunk to be send to each process
	local_size=size/procs;

	//Fill the vector with the pixel values of the images.
		for(c=0; c< img1.get_spectrum();++c)
		{
			for(z=0; z< img1.get_depth(); ++z)
			{	
				for(x=0; x<img1.get_width();++x)
				{					
					for(y=0; y< img1.get_height();++y)
					{	

						matrix[img1.get_height()*x+y+img1.get_height()*img1.get_width()*z+img1.get_height()*img1.get_width()*img1.get_depth()*c]=img1.get_pixel_value(x,y,z,c);
						matrix2[img1.get_height()*x+y+img1.get_height()*img1.get_width()*z+img1.get_height()*img1.get_width()*img1.get_depth()*c]=img2.get_pixel_value(x,y,z,c);

					}	
				}
			}
		}		
		//Create the local matrices, and the local result
		matrix2_local=(int*)malloc(local_size*sizeof(int));
		matrix_local=(int*)malloc(local_size*sizeof(int));
		result_local=(int*)malloc(local_size*sizeof(int));
	}	
	
	MPI_Barrier(MPI_COMM_WORLD); //Barrier sets a wait point to all process in the communicator
	//Scatter cut the original vector and send a part to each process
	MPI_Scatter(matrix, local_size , MPI_INT, matrix_local, local_size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(matrix2,local_size , MPI_INT, matrix2_local, local_size, MPI_INT, 0, MPI_COMM_WORLD);
	
	//free the large block of the memory because we don't use it anymore.
	free(matrix);
	free(matrix2);
	
	//Do the function in the local parts
	for(i=0; i<local_size; ++i)
	{
		result_local[i] = matrix_local[i]+matrix2_local[i];
		if(result_local[i]>255)
		{
			result_local[i] = 255;
		}
	
	}
	free(matrix_local);
	free(matrix2_local);
	//Gather join the information of the local parts in the total vector result
	MPI_Gather(result_local, local_size, MPI_INT, mat_result, local_size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	free(result_local);
	time=MPI_Wtime()-time; //measure the final time
	MPI_Finalize();// To finalize the parallel section we use this function.
	// Copy the pixel values to the image result

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
	result.display("disp"); //It is possible to displays the result or save it.
	result.save("sumampi2.jpg");
	return 0;
}
