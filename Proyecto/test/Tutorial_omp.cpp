#include "../include/ParallelPic.hh" // include ParallelPic header

int main(){

	// Call to a constructor of the class Image by giving it the name of the picture file.
	
	Image wave ("../../Multimedia/huge.jpg"); // Image "__name__" ("__image_path__");

	//It is possible to display the image created before:
	
	wave.display("Original Image");
	
	/*Now you can manipulate the image with the variety of filters and functions of the class ParallelPic.
	 * All of them has the numbers of threads as a parameter so you can define with 
	 * how many do you want to run the function.
	 */
	 
	 //for example:
	 
	 Image result = wave.filter_Laplacian_no_diagonal(2);// image_object.filter_name(params);
	 
	// and then view the result
	
	result.display("Laplacian filter");
	
	//note that ParallelPic do not modifies the original image, it only creates one resultant.
	
	return 0;
}
