#include "../include/ParallelPic.hh"


/**\file ../include/image.hh
 * Header for the image class
 */
 
 // *************************************************************************
 // ********************* CONTRUCTORS ***************************************
 // *************************************************************************
 
 /** \fn Image::Image()
 * \brief Constructor
 * This constructor initializes the four dimension params at 0;
 * The Img calls the constructor of CImg to create an empty image.
 */
Image::Image()
{
	this->Img = new CImg<unsigned char>();
	this->width = 0;
	this->height = 0;
	this->depth = 0;
	this->spectrum = 0;

}
/** \fn Image::Image(const char *const filename)
 * \brief Constructor
 * 
 * This constructor is used when the image already exist's and is stored in the \param filename path.
 */

Image::Image(const char *const filename)
{
	this->Img = new CImg<unsigned char>(filename);
	/// \param <Img> is a var of type Cimg that is treated like an unsigned char.
	this->width = this->Img->width();
	/// \param <width> refers to the number of columns of pixels in the image.
	this->height = this->Img->height();
	/// \param <height> refers to the number of rows of pixels in the image.
	this->depth = this->Img->depth();
	/// \param <depth> is the amount of layers of depth the image has, usually is one, except for 3D images.
	this->spectrum = this->Img->spectrum();
	/// \param <spectrum> is the number of channels in the image, RGB has a spectrum of 3, a monocromatic image has a spectrum of 1.
}
/** \fn Image::Image(const unsigned int width, const unsigned int height, const unsigned int depth, const unsigned int spectrum, int value)
 * \brief This constructor is used when we need to create an image, and gives the dimensions of the image, and the value of a color that fills all the pixels.
 * 
 */
Image::Image(const unsigned int width, const unsigned int height, const unsigned int depth, const unsigned int spectrum, int value)
{
	this->Img = new CImg<unsigned char>(width, height, depth, spectrum, value);
	this->width = width;
	this->height = height;
	this->depth = depth;
	this->spectrum = spectrum;
}
/** \fn ~Image(void) 
 * \brief is the destructor of the class.
 */ 
Image::~Image(void)
{
	
}
// *************************************************************************
// ************************** SAVE & DISPLAY *******************************
// *************************************************************************
/** \fn void Image:: save(const char *const savefilename)
 * \brief Allows to save the image in a file with the name in the parameter \param const char *const savefilename
 * 
 */

void Image:: save(const char *const savefilename)
{
	this->Img->save(savefilename);
}

void Image :: display(const char* message)
{
	CImgDisplay main (*(this->Img), message);
	while(!main.is_closed())
	{
		main.wait();
	}
}

// *************************************************************************
// *************************** GETs & SETs *********************************
// *************************************************************************

/** \fn unsigned int Image:: get_width()
 * \brief returns a pointer of type int, with the width of the image.
 * 
 */
unsigned int Image:: get_width()
{
	return this->width;
}
/** \fn unsigned int Image:: get_height()
 * \brief returns a pointer of type int, with the height of the image.
 * 
 */
unsigned int Image:: get_height()
{
	return this->height;
}

/** \fn unsigned int Image:: get_depth()
 * \brief returns a pointer of type int, with the depth of the image.
 * 
 */
unsigned int Image:: get_depth()
{
	return this->depth;
}

/** \fn unsigned int Image:: get_spectrum()
 * \brief returns a pointer of type int, with the spectrum of the image.
 * 
 */
unsigned int Image:: get_spectrum()
{
	return this->spectrum;
}


/*! \fn unsigned int Image:: get_pixel_value(int x, int y, int z, int c)
 * \brief Returns the value of the unsigned char in the x, y, z, and c coordinates.
 */

unsigned int Image:: get_pixel_value(int x, int y, int z, int c)
{
	return this->Img->get_vector_at(x, y, z)[c];
}

/*! \fn void Image:: set_pixel_value(unsigned char (int x, int y, int z, int c, unsigned char value)
 * \brief Sets the value of the unsigned chars in the x, y and z coordinates of a pixel.
 */

void Image:: set_pixel_value(int x, int y, int z, int c, unsigned char value)
{
	(*(this->Img))(x, y, z, c)= value;
} 

 
// *************************************************************************
// **************************Arithmetic and Logic **************************
// *************************************************************************

/*! \fn Image Image :: substract_img(Image image2)
 * \brief This function substracts the pixel values of two images, that can be used to see the differences between them. 
 * \param Image image2: Is the image that will be substracted to the original image.
 * \return Image result: Is the result of the substraction of both images.
 */
Image Image :: substract_img(Image image2, int num_threads)
{
	unsigned int x,y,c,z;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	if(this->get_width() == image2.get_width() && this->get_height() == image2.get_height() && this->get_depth() == image2.get_depth() && this->get_spectrum() == image2.get_spectrum())
	{
		for(c = 0; c < this->get_spectrum(); c++)
		{
			for(z = 0; z < this->get_depth(); z++)
			{
				#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y) shared(z,c,result,chunk)
				for(x = 0; x < this->get_width(); x++)
				{
					for(y = 0; y < this->get_height(); y++)
					{
						pixel= static_cast<unsigned int>(abs(this->get_pixel_value(x,y,z,c)-image2.get_pixel_value(x,y,z,c)));
						
						#pragma omp ordered
						result.set_pixel_value(x,y,z,c,pixel);
					}
				}
			}
		}
	}
	return result;
}


Image Image :: sum_img(Image image2, int num_threads)
{
	unsigned int x,y,c,z,sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	if(this->get_width() == image2.get_width() && this->get_height() == image2.get_height() && this->get_depth() == image2.get_depth() && this->get_spectrum() == image2.get_spectrum())
	{
		for(c = 0; c < this->get_spectrum(); c++)
		{
			for(z = 0; z < this->get_depth(); z++)
			{
				#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,sum) shared(z,c,result,chunk)
				for(x = 0; x < this->get_width(); x++)
				{
					for(y = 0; y < this->get_height(); y++)
					{
			
						sum = this->get_pixel_value(x,y,z,c)+image2.get_pixel_value(x,y,z,c);
						if (sum <= 255)
						{
							pixel = static_cast<unsigned int>(sum);
						}
						else
						{
							pixel = 255;
						}
						#pragma omp ordered
						result.set_pixel_value(x,y,z,c,pixel);
					}
				}
			}
		}
	}
	return result;
}


/*! \fn Image Image :: multiply_img(double)
 * \brief This function multiplies the pixel values by a factor. If the pixel value is higher than 255, adjust the pixel value to 255.
 * \param double multiplier is the factor that mutiplies all the pixel values.
 * \return Image result: Is the result of multiply the image.
 */

Image Image :: multiply_img(double multiplier,int num_threads)
{
	unsigned int x,y,c,z;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0);
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel) shared(z,c,result,chunk)
			for(x = 0; x < this->get_width(); x++)
			{
				for(y = 0; y < this->get_height(); y++)
				{
					pixel= static_cast<unsigned int>(abs(this->get_pixel_value(x,y,z,c)*multiplier));
					if (pixel >255)
						pixel = 255;
					#pragma omp parallel
					result.set_pixel_value(x,y,z,c,pixel);
				}
			}
		}
	}
	return result;
}

/*! \fn Image Image :: bynarize_img(double)
 * \brief This function set the pixel value to 255 if the original pixel value is higher than a cutoff value, and 0 if the pixel value is less than the cut off value
 * \param unsigned int cutoff_value is the limit of the pixel value, to be changed by 0 or 255.
 * \return Image result: Is the result of binarize the image.
 */

Image Image :: binarize_img(unsigned int cutoff_value , int num_threads)
{
	unsigned int x,y,c,z;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);	
	
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0);
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel) shared(z,c,result,chunk)
			for(x = 0; x < this->get_width(); x++)
			{
				for(y = 0; y < this->get_height(); y++)
				{
					pixel= static_cast<unsigned int>(this->get_pixel_value(x,y,z,c));
					if(pixel >= cutoff_value)
						pixel=255;
					else
						pixel=0;
						
					#pragma omp ordered
					result.set_pixel_value(x,y,z,c,pixel);

				}
			}
		}
	}

	
	return result;
}


// *************************************************************************
// ************************** SPACE DOMAIN FILTERS *************************
// *************************************************************************




// *************************************************************************
// *********************** Sharpening Spatial Filters **********************
// *************************************************************************

/*! \fn Image Image::filter_Laplacian(): 
 *	\brief Returns an image after applying the Laplacian filter to the image. Considers the diagonal values
 * This function applies a convolution with this kernel: \f$ ((1, 1, 1), (1, -8, 1), (1, 1, 1)) \f$
 * \return A Filtered image with the Laplacian filter applied.
 */
 

Image Image::filter_Laplacian(int num_threads)
{
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	unsigned int x,y,c,z;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,sum) shared(z,c,filtered,chunk,m)
			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = 0;
					
					for (unsigned int i = 0 ; i < 3; i++)
					{
						for(unsigned int j = 0 ; j < 3; j++)
						{
						sum += -this->get_pixel_value(x+i-1, y+i-1, z, c);
						}
					}
					
					sum += 9*(this->get_pixel_value(x,y,z,c));
					
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	return filtered; 
}

/*! \fn Image Image :: filter_Laplacian_no_diagonal(): The same as the  \fn filter_Laplacian(), but doesn't include the diagonal values.
* Works as a derivative function, reacts to high change on the pixels value, especially to noise, and borders.
* Applies the following filter: \f$ ((0, -1, 0) , (-1, 4, -1) , (0, -1, 0)) \f$
* \return A filtered Image with the laplacian filter.
*/
Image Image :: filter_Laplacian_no_diagonal(int num_threads)
{
	unsigned int x,y,c,z;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,sum) shared(z,c,filtered,chunk,m)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = 4*(this->get_pixel_value(x,y,z,c)) - (this->get_pixel_value(x-1,y,z,c) + this->get_pixel_value(x+1,y,z,c) + this->get_pixel_value(x,y-1,z,c) +this->get_pixel_value(x,y+1,z,c));
					
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	return filtered; 
	
}

/*! \fn  Image Image :: filter_Gradient_horizontal()
 * This filter is used as as Sharpening Spatial Filter, used to identify borders and noise in the image.
 * Can be used to identify horizontal borders or discrepation
 * Applies the following filter:  \f$ ((1, 2, 1) , (0, 0, 0) , (-1, -2, -1)) \f$
 * \return An image object that contains the original image after receiving a gradient filter in the 
 * horizontal direction. Could be used to identify horizontal borders.
 */ 
Image Image :: filter_Gradient_horizontal(int num_threads)
{	
	unsigned int x,y,c,z;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,sum) shared(z,c,filtered,chunk,m)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = this->get_pixel_value(x-1, y-1, z, c) + 2*(this->get_pixel_value(x, y-1, z, c)) + this->get_pixel_value(x+1, y-1, z, c) - (this->get_pixel_value(x-1, y+1, z, c) + 2*(this->get_pixel_value(x, y+1, z, c)) + this->get_pixel_value(x+1, y+1, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	 return filtered;
}

/*! \fn  Image Image :: filter_Gradient_vertical()
 * This filter is used as as Sharpening Spatial Filter, used to identify borders and noise in the image.
 * Can be used to identify vertical borders or discrepations.
 * Applies the following filter:  \f$ ((1, 0, -1) , (2, 0, -2) , (1, 0, -1)) \f$
 * \return An image object that contains the original image after receiving a gradient filter in the 
 * vertical direction. Could be used to identify vertical borders.
 */ 
Image Image :: filter_Gradient_vertical(int num_threads)
{
	unsigned int x,y,c,z;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,sum) shared(z,c,filtered,chunk,m)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = get_pixel_value(x-1, y-1, z, c) + 2*get_pixel_value(x-1, y, z, c) + get_pixel_value(x-1, y+1, z, c) - (get_pixel_value(x+1, y-1, z, c) + 2*get_pixel_value(x+1, y, z, c) + get_pixel_value(x+1, y+1, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	 return filtered;
}

/*! \fn Image Image :: filter_Prewwit_N_S()
 * \brief One implementation of the Prewwit mask. 
 * Technically, it is a discrete differentiation operator, 
 * computing an approximation of the gradient of the image 
 * intensity function. At each point in the image, the result
 *  of the Prewitt operator is either the corresponding gradient
 *  vector or the norm of this vector. The Prewitt operator is 
 * based on convolving the image with a small, separable, and i
 * nteger valued filter in horizontal and vertical direction and 
 * is therefore relatively inexpensive in terms of computations.
 *  On the other hand, the gradient approximation which it produces
 *  is relatively crude, in particular for high frequency variations 
 * in the image.
 *  The Prewitt operator was developed by Judith M. S. Prewitt
 */


Image Image :: filter_Prewitt_N_S(int num_threads)
{
	unsigned int x,y,c,z;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,sum) shared(z,c,filtered,chunk,m)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = get_pixel_value(x-1, y-1, z, c) + get_pixel_value(x, y-1, z, c) + get_pixel_value(x+1, y-1, z, c) - (get_pixel_value(x-1, y+1, z, c) + get_pixel_value(x, y+1, z, c) + get_pixel_value(x+1, y+1, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}
	
Image Image ::filter_Prewitt_NE_SW(int num_threads)
{
	unsigned int x,y,c,z;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,sum) shared(z,c,filtered,chunk,m)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = get_pixel_value(x, y-1, z, c) + get_pixel_value(x+1, y-1, z, c) + get_pixel_value(x+1, y, z, c) - (get_pixel_value(x-1, y, z, c) + get_pixel_value(x-1, y+1, z, c) + get_pixel_value(x, y+1, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

Image Image ::filter_Prewitt_E_W(int num_threads)
{
	unsigned int x,y,c,z;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,sum) shared(z,c,filtered,chunk,m)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = get_pixel_value(x-1, y-1, z, c) + get_pixel_value(x-1, y, z, c) + get_pixel_value(x-1, y+1, z, c) - (get_pixel_value(x+1, y-1, z, c) + get_pixel_value(x+1, y, z, c) + get_pixel_value(x+1, y+1, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}
	
Image Image ::filter_Prewitt_NW_SE(int num_threads)
{
	unsigned int x,y,c,z;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);	
	
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,sum) shared(z,c,filtered,chunk,m)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = get_pixel_value(x-1, y-1, z, c) + get_pixel_value(x-1, y, z, c) + get_pixel_value(x, y-1, z, c) - (get_pixel_value(x+1, y, z, c) + get_pixel_value(x+1, y+1, z, c) + get_pixel_value(x, y+1, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
		
}

Image Image ::filter_edge_enhacement_displacement(unsigned int horizontal_dis, unsigned int vertical_dis, int num_threads)
{
	unsigned int x,y,c,z;
	unsigned char value;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); 
	if((horizontal_dis < this->get_width()) && (vertical_dis < this->get_height()))
	{
		for(c = 0; c < this->get_spectrum(); c++)
		{
			for(z = 0; z < this->get_depth(); z++)
			{
				#pragma omp parallel for ordered schedule(dynamic,chunk) private(value) shared(z,c,result,chunk)

				for(x = horizontal_dis; x < this->get_width(); x++)
				{
					for(y = vertical_dis; y < this->get_height(); y++)
					{
						value = static_cast<unsigned char>(abs(this->get_pixel_value(x,y,z,c) - this->get_pixel_value(x-horizontal_dis, y-vertical_dis, z, c)));
						
						#pragma omp ordered
						result.set_pixel_value(x,y,z,c, value);
					}
				}
			}
		}
	}
	return result;
}

Image Image :: filter_vertical_borders(int num_threads)
{
	unsigned int x,y,c,z;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel) shared(z,c,filtered,chunk,m)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = 0; y < this->get_height()-m; y++)
				{
					pixel = (unsigned char)static_cast<unsigned char> (abs(this->get_pixel_value(x-1, y, z, c) - get_pixel_value(x+1, y, z, c)));
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	 return filtered;
}

Image Image :: filter_horizontal_borders(int num_threads)
{
	unsigned int x,y,c,z;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel) shared(z,c,filtered,chunk,m)

			for(x = 0; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					
					pixel = (unsigned char)static_cast<unsigned char> (abs(this->get_pixel_value(x, y-1, z, c) - get_pixel_value(x, y+1, z, c)));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	 return filtered;
}

// *************************************************************************
// *********************** Smoothing Spatial Filters **********************
// *************************************************************************


/*! \fn Image Image :: filter_median (int dim)
 * \brief This function calculates the median of the range of pixels into the kernel and sets this value in the central pixel of the kernel.
 * \param Only receives the dimension of the kernel (dim), wich only can be an impair number.
 * \return Image filtered which is the image with the median filter applied.
 */

Image Image :: filter_median (int dim, int num_threads)
{
	unsigned int x,y,c,z;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	int m = (dim-1)/2;
	unsigned char pixel_values [dim*dim];
	unsigned char temp,pixel;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
		#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel_values,temp,x,y) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width(); x++)
			{
				for(y = m; y < this->get_height(); y++)
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
					pixel = pixel_values[((dim*dim-1)/2)-1];
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	 return filtered;
 }

/*! \fn Image Image :: filter_average(int dim)
 * \brief This function calculates the average of the range of pixels into the kernel and sets this value in the central pixel of the kernel.
 * \param Only receives the dimension of the kernel (dim), wich only can be an impair number.
 * \return Image filtered which is the image with the average filter applied.
 */

Image Image :: filter_average(int dim, int num_threads)
{
	unsigned int y,c,z,sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,y,sum) shared(z,c,filtered,chunk)

			for(unsigned int x = dim; x < this->get_width()-dim; x++)
			{
				for(y = dim; y < this->get_height()-dim; y++)
				{
					sum = 0;
					for(unsigned int i = x-dim; i<= x+dim; i++)
					{
						for(unsigned int j = y-dim; j<= y+dim; j++)
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

/*! \fn Image Image :: filter_gaussian(int o, int dim_kernel)
 * \brief This function applies a gaussian kernel trough the hole image. 
 * \param Receives the dimension of the kernel (dim_kernel) and a paremeter o wich stablish the values on the gaussian kernel.
 * \return Image filtered which is the image with the gaussian filter applied.
 */
 
Image Image :: filter_gaussian(int o, int dim_kernel, int num_threads)
{
	unsigned int x,y,c,z,cont;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0);

	double kernel[dim_kernel*dim_kernel];
	
	int m = (dim_kernel-1)/2;
	
	double gaussian =1/pow(3.1415*2*o*o,0.5);

	for(int i =-m; i <=m; i++)
	{
		for(int j =-m; j<=+m; j++)
		{
			double exp= -(i*i+j*j)*0.5/(o*o);
			kernel[(i+m)*dim_kernel + (j+m)]=gaussian*pow(2.7,exp); 
			
		}
	}
	
	
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,cont) shared(z,c,filtered,chunk,kernel)

			for(x = m; x < this->get_width(); x++)
			{
				for(y = m; y < this->get_height(); y++)
				{
					cont=0;
					pixel=0;
					
					for(unsigned int i = x-m; i < x+m; i++)
					{
						for(unsigned int j = y-m; j< y+m; j++)
						{
							pixel+= this->get_pixel_value(i, j, z, c)*(kernel[cont]); 
							cont++;
						}
					}
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, (pixel/2));
				}
				
			 }
			 
		 }
	}  
	 return filtered;		
}	

/*! \fn Image Image :: filter_modal(int dim)
 * \brief This function calculates the modal of the range of pixels into the kernel and sets this value in the central pixel of the kernel.
 * \param Only receives the dimension of the kernel (dim), wich only can be an impair number.
 * \return Image filtered which is the image with the modal filter applied.
 */

Image Image :: filter_modal(int dim, int num_threads)
{
	
	unsigned int x,y,c,z;
	unsigned char average=0;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0);
	unsigned char pixel_values[dim*dim];
	unsigned char moda;
	
	int m=(dim-1)/2;
	unsigned char copy_pixels[dim*dim];

  	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel_values,moda,x,y) shared(z,c,filtered,chunk,m)

			for(x = m; x < this->get_width(); x++)
			{
				for(y = m; y < this->get_height(); y++)
				{
					for(unsigned int i = x-m; i < x+m; i++)
					{
						for(unsigned int j = y-m; j< y+m; j++)
						{
							pixel_values [(i-x+m)*dim + (j-y+m)]= this->get_pixel_value(i, j, z, c);
		
							int frequency[dim*dim];
							moda=0;	
							
							for(int k=0;k<dim*dim;k++)
							{
								copy_pixels[k]=	pixel_values[k];
								frequency[k]=0;
							}
							
							for(int p=0;p<dim*dim;p++)
							{
								for(int q=p+1;q<dim*dim;q++)
								{
									if(copy_pixels[p]==pixel_values[q]){
										frequency[p]++;
								
									}
			
								}
	
							}



							for(int s=0; s<dim*dim ; s++)
							{
								for(int e=s+1 ; e<dim*dim ; e++)
								{
									if(frequency[e] < frequency[s])
									{
										moda = copy_pixels[s];
										average=copy_pixels[s];
									}
								}
							}
							

							if(moda==0)
							{
								for(int k=0;k<dim*dim;k++)
								{
									moda += pixel_values[k];
								}
							average=(moda/dim);
							}


						}	
					}
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, average);
				}
				
			 }
			 
		 }
	}

return filtered;

}
	


// *************************************************************************
// *********************** Dot to Dot Transformations **********************
// *************************************************************************

/*! \fn  Image Image :: inverse()
 * Executes this transformation: \f$ v(x,y,z,c) = 255 - u(x,y,z,c) \f$
 * \return An image object that contains the inverse of the original image, this means that every pixel value is substracted to 255.
 */ 

Image Image ::inverse(int num_threads)
{
	unsigned int y,c,z,x;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image inverted (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y) shared(z,c,inverted,chunk)

			for(x = 0; x < this->get_width(); x++)
			{
				for(y = 0; y < this->get_height(); y++)
				{
					pixel= static_cast<unsigned int>(255-this->get_pixel_value(x,y,z,c));
					#pragma omp ordered
					inverted.set_pixel_value(x,y,z,c,pixel);
				}
			}
		}
	}
	return inverted;
}

/*! \fn  Image Image :: log_transformation()
 * Executes this transformation: \f$ v(x,y,z,c) = c log(u(x,y,z,c)+1)\f$
 *  where v(x,y,z,c) is the transformed pixel, and u(x,y,z,c) is the original pixel.
 * \return An image object that contains the inverse of the original image, this means that every pixel value is substracted to 255.
 */
Image Image :: log_transformation(int num_threads)
{
	unsigned int y,c,z,x;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 

	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y) shared(z,c,filtered,chunk)

			for(x = 0; x < this->get_width(); x++)
			{
				for(y = 0; y < this->get_height(); y++)
				{
					pixel = static_cast<unsigned char>((255/log(256)) * log(1+this->get_pixel_value(x, y, z, c)));
					#pragma omp ordered
					filtered.set_pixel_value(x,y,z,c, pixel);
				}
			}
		}
	}
	return filtered;
}

/*! \fn  Image Image ::filter_dynamic_range_dilatation(unsigned char a, unsigned char b, double alpha, double beta, double gamma)
 * \brief All the pixel values are divided in 3 ranges, and each range suffer a diferent transformation.
 * This function is used to transform the range of lower pixel values in medium values and the higher too, to smooth the image.
 * \param unsigned char a is the first cutoff pixel value. 
 * \param unsigned char b is the second cutoff pixel value.
 * \param double alpha is the first multiplier.
 * \param double beta is the second multiplier.
 * \param double gamma is the third multiplier.
 * \return An image object that contains the dilatated image.
 */ 
Image Image ::filter_dynamic_range_dilatation(unsigned char a, unsigned char b, double alpha, double beta, double gamma, int num_threads)
{
	unsigned int y,c,z,x;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); 
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y) shared(z,c,filtered,chunk)
			for(x = 0; x < this->get_width(); x++)
			{
				for(y = 0; y < this->get_height(); y++)
				{
					pixel=0;
					
					if(this->get_pixel_value(x,y,z,c)<a)
						pixel =abs(alpha*this->get_pixel_value(x,y,z,c));
						
					else if(this->get_pixel_value(x,y,z,c)>=a && this->get_pixel_value(x,y,z,c)<b)
						pixel=abs(beta*(this->get_pixel_value(x,y,z,c)-a)+alpha*a);
					
					else if(this->get_pixel_value(x,y,z,c)<=b)
						
					pixel=abs(gamma*(this->get_pixel_value(x,y,z,c)-b)+((beta*(b-a))+alpha*a));
					#pragma omp ordered
					filtered.set_pixel_value(x,y,z,c,static_cast<unsigned int>(pixel));
				}
			}
		}
	}
	return filtered;
}


/*! \fn Image Image :: power_law_transformatiom(double exponent)
 *  Applies a transformation given by the ecuation \f$ v(x,y) = c {u(x,y)}^{\gamma} \f$
 * 	where \f$ u(x,y) \f$ is the value of the non filtered image, and \f$ v(xy) \f$ is the 
 * 	intensity value in the filtered image. \f$ \gamma, c \f$ are constants. In this case
 *  \f$ \gamma \f$ is a parameter.
 * 	\return A filtered image with the power law transformation
 */
Image Image :: power_law_transformatiom(double exponent, int num_threads)
{
	unsigned int y,c,z,x;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	double power_law;
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	double k = (pow(255, 1-exponent));
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,power_law) shared(z,c,filtered,chunk,k)

			for(x = 0; x < this->get_width(); x++)
			{
				for(y = 0; y < this->get_height(); y++)
				{
					power_law = k * pow( (this->get_pixel_value(x,y,z,c)) , exponent);
					pixel = static_cast<unsigned char>(power_law);
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	return filtered;
}

/*! \fn Image Image :: color_slicing(unsigned char color1[], unsigned char color2[], unsigned char neutral)
 *  \brief Highlights the desired colors, between the two colors given as parameters.
 * \attention{The colors must be unsigned char, and must be the size of the spectrum of the image (Number of channels), 3 in case o RBG images}
 * \param unsigned char color1[]: The start color of the color slicing.
 * \param unsigned char color2[]: The end color of the color slicing.
 * \param unsigned char neutral: The intensity every other pixels that are not between the given colors will be set to.
 */
Image Image :: color_slicing(unsigned char color1[], unsigned char color2[], unsigned char neutral[], int num_threads)
{
	unsigned int y,c,z,x;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); 
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y) shared(z,c,filtered,chunk)

			for(x = 0; x < this->get_width(); x++)
			{
				for(y = 0; y < this->get_height(); y++)
				{
					pixel = this->get_pixel_value(x,y,z,c);
					if(pixel > color1[c] && pixel < color2[c] )
					{
						#pragma omp ordered
						filtered.set_pixel_value(x, y, z, c, pixel);
					}
					else
					{
						#pragma omp ordered
						filtered.set_pixel_value(x,y,z,c, neutral[c]);
					}
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

// *************************************************************************
// *********************** HISTOGRAM AND EQUALIZATION **********************
// *************************************************************************

/*! \fn int* Image :: get_histogram(int c, int z)
 * This function returns an array containing the values of the histogram points, in the 
 * desired channel and depth. 
 * An Histogram is measure of the frecquency of a intensity value in an image, and is often 
 * used as a parameter to improve the constrast and quality of the image. After observing the 
 * histogram ( see plot_histogram() ) you could ecualizate the image.
 */
int* Image :: get_histogram(unsigned int c, unsigned int z)
{
	int histogram [256];
	for(int i = 0; i<256; i++)
	{
	histogram[i] = 0;	
	}
	
	if (c < this->get_spectrum() && z < this->get_depth())
	{
		for(unsigned int x = 0; x < this->get_width(); x++)
		{
			for(unsigned int y = 0; y < this->get_height(); y++)
			{
				unsigned char pixel_value = this->get_pixel_value(x,y,z,c);
				(histogram[pixel_value])++;
			}
		}
	}
	
	int* histogram_pointer = histogram;
	
	return histogram_pointer;
}

/*! \fn void Image :: plot_histogram(int levels, const char* title)
 * \brief This function plot the histogram, using the CImg histogram function
 * \param levels is the number of bars or columns that appear in the histogram.
 * \param title is the title of the histogram.
 */
void Image :: plot_histogram(int levels,const char* title)
{
	CImg<unsigned char> img = this->Img->histogram(levels);
	
	CImgDisplay main_display (*(this->Img), title);
	
	img.display_graph(main_display, 3, 1, "Pixel Intensity", 0, 0, "Frequency", 0, 0);
}

/*! \fn void Image :: plot_histogram_ecualization(int levels, const char* title)
 * \brief This function plot the equalized histogram, using the CImg equalize function
 * \param levels is the number of bars or columns that appear in the histogram.
 * \param title is the title of the histogram.
 */
void Image :: plot_histogram_equalization(int levels, const char* title)
{
	CImg<unsigned char> img = this->Img->equalize(levels);
	
	CImgDisplay main_display (*(this->Img), title);
	
	img.display_graph(main_display, 3, 1, "Pixel Intensity", 0, 0, "Frequency", 0, 0);
}

// *************************************************************************
// *********************** OTHER TRANSFORMATIONS ***************************
// *************************************************************************

/*! \fn Image Image :: filter_kirsch_0()
 *  \brief Applies the kirsch mask at 0°.
 * 	\f$(-3,-3,5)(-3,0,5)(-3,-3,5)\f$
 */
Image Image ::filter_kirsch_0(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = -3*(get_pixel_value(x-1, y-1, z, c)+get_pixel_value(x-1, y, z, c)+get_pixel_value(x-1, y+1, z, c)+get_pixel_value(x, y-1, z, c)+get_pixel_value(x, y+1, z, c))+5*(get_pixel_value(x+1, y-1, z, c)+get_pixel_value(x+1, y, z, c)+get_pixel_value(x+1, y+1, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

/*! \fn Image Image :: filter_kirsch_45()
 *  \brief Applies the kirsch mask at 45°.
 * 	\f$(-3,5,5)(-3,0,5)(-3,-3,-3)\f$
 */
Image Image ::filter_kirsch_45(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = -3*(get_pixel_value(x-1, y-1, z, c)+get_pixel_value(x-1, y, z, c)+get_pixel_value(x-1, y+1, z, c)+get_pixel_value(x+1, y+1, z, c)+get_pixel_value(x, y+1, z, c))+5*(get_pixel_value(x+1, y-1, z, c)+get_pixel_value(x+1, y, z, c)+get_pixel_value(x, y-1, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
	
}

/*! \fn Image Image :: filter_kirsch_90()
 *  \brief Applies the kirsch mask at 90°.
 * 	\f$(5,5,5)(-3,0-3)(-3,-3,-3)\f$
 */
Image Image ::filter_kirsch_90(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = -3*(get_pixel_value(x-1, y, z, c)+get_pixel_value(x-1, y+1, z, c)+get_pixel_value(x, y+1, z, c)+get_pixel_value(x+1, y+1, z, c)+get_pixel_value(x+1, y, z, c))+5*(get_pixel_value(x+1, y-1, z, c)+get_pixel_value(x, y-1, z, c)+get_pixel_value(x-1, y-1, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

/*! \fn Image Image :: filter_kirsch_135()
 *  \brief Applies the kirsch mask at 135°.
 * 	\f$(5,5,-3)(5,0,-3)(-3,-3,-3)\f$
 */
Image Image ::filter_kirsch_135(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = -3*(get_pixel_value(x-1, y-1, z, c)+get_pixel_value(x+1, y, z, c)+get_pixel_value(x+1, y+1, z, c)+get_pixel_value(x, y+1, z, c)+get_pixel_value(x-1, y+1, z, c))+5*(get_pixel_value(x-1, y, z, c)+get_pixel_value(x-1, y-1, z, c)+get_pixel_value(x, y-1, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

/*! \fn Image Image :: filter_kirsch_180()
 *  \brief Applies the kirsch mask at 180°.
 * 	\f$(5,-3,-3)(5,0,-3)(5,-3,-3)\f$
 */
Image Image ::filter_kirsch_180(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = -3*(get_pixel_value(x, y-1, z, c)+get_pixel_value(x+1, y-1, z, c)+get_pixel_value(x+1, y, z, c)+get_pixel_value(x+1, y+1, z, c)+get_pixel_value(x, y+1, z, c))+5*(get_pixel_value(x-1, y, z, c)+get_pixel_value(x-1, y+1, z, c)+get_pixel_value(x-1, y-1, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

/*! \fn Image Image :: filter_kirsch_225()
 *  \brief Applies the kirsch mask at 225°.
 * 	\f$(-3,-3,-3)(5,0,-3)(5,5,-3)\f$
 */
Image Image ::filter_kirsch_225(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = -3*(get_pixel_value(x-1, y-1, z, c)+get_pixel_value(x, y-1, z, c)+get_pixel_value(x+1, y-1, z, c)+get_pixel_value(x+1, y, z, c)+get_pixel_value(x+1, y+1, z, c))+5*(get_pixel_value(x-1, y, z, c)+get_pixel_value(x-1, y+1, z, c)+get_pixel_value(x, y+1, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

/*! \fn Image Image :: filter_kirsch_270()
 *  \brief Applies the kirsch mask at 270°.
 * 	\f$(-3,-3,-3)(-3,0,-3)(5,5,5)\f$
 */
Image Image ::filter_kirsch_270(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = -3*(get_pixel_value(x-1, y-1, z, c)+get_pixel_value(x-1, y, z, c)+get_pixel_value(x, y-1, z, c)+get_pixel_value(x+1, y-1, z, c)+get_pixel_value(x+1, y, z, c))+5*(get_pixel_value(x, y+1, z, c)+get_pixel_value(x+1, y+1, z, c)+get_pixel_value(x-1, y+1, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

/*! \fn Image Image :: filter_kirsch_315()
 *  \brief Applies the kirsch mask at 315°.
 * 	\f$(-3,-3,-3)(-3,0,5)(-3,5,5)\f$
 */
Image Image ::filter_kirsch_315(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = -3*(get_pixel_value(x-1, y+1, z, c)+get_pixel_value(x-1, y, z, c)+get_pixel_value(x-1, y-1, z, c)+get_pixel_value(x, y-1, z, c)+get_pixel_value(x+1, y-1, z, c))+5*(get_pixel_value(x, y+1, z, c)+get_pixel_value(x+1, y+1, z, c)+get_pixel_value(x+1, y, z, c));
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

/*! \fn Image Image :: filter_freeman_0()
 *  \brief Applies the freeman mask \f$(1,1,1)(1,-2,1)(1,-1,-1)\f$.
 */

Image Image ::filter_freeman_0(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = (get_pixel_value(x-1, y-1, z, c)+get_pixel_value(x-1, y, z, c)+get_pixel_value(x-1, y+1, z, c)+get_pixel_value(x, y-1, z, c)+get_pixel_value(x+1, y-1, z, c)+get_pixel_value(x+1,y,z,c))-1*(get_pixel_value(x+1, y+1, z, c)+get_pixel_value(x, y+1, z, c))+-2*get_pixel_value(x, y, z, c);
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

/*! \fn Image Image :: filter_freeman_1()
 *  \brief Applies the freeman mask \f$(1,1,1)(-1,-2,1)(1,-1,1)\f$.
 */
Image Image ::filter_freeman_1(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = (get_pixel_value(x-1, y-1, z, c)+get_pixel_value(x+1, y+1, z, c)+get_pixel_value(x-1, y+1, z, c)+get_pixel_value(x, y-1, z, c)+get_pixel_value(x+1, y-1, z, c)+get_pixel_value(x+1,y,z,c))-1*(get_pixel_value(x-1, y, z, c)+get_pixel_value(x, y+1, z, c))+-2*get_pixel_value(x, y, z, c);
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

/*! \fn Image Image :: filter_freeman_2()
 *  \brief Applies the freeman mask \f$)-1,1,1)(-1,-2,1)(1,1,1)\f$.
 */
Image Image ::filter_freeman_2(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = (get_pixel_value(x, y-1, z, c)+get_pixel_value(x+1, y-1, z, c)+get_pixel_value(x+1, y, z, c)+get_pixel_value(x+1, y+1, z, c)+get_pixel_value(x, y+1, z, c)+get_pixel_value(x-1,y+1,z,c))-1*(get_pixel_value(x-1, y, z, c)+get_pixel_value(x-1, y-1, z, c))+-2*get_pixel_value(x, y, z, c);
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

/*! \fn Image Image :: filter_freeman_3()
 *  \brief Applies the freeman mask \f$(-1,-1,1)(-1,-2,1)(1,1,1)\f$.
 */
Image Image ::filter_freeman_3(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = (get_pixel_value(x+1, y-1, z, c)+get_pixel_value(x+1, y, z, c)+get_pixel_value(x+1, y+1, z, c)+get_pixel_value(x, y+1, z, c)+get_pixel_value(x-1,y+1,z,c))-1*(get_pixel_value(x-1, y, z, c)+get_pixel_value(x, y-1, z, c)+get_pixel_value(x-1,y-1,z,c))+-2*get_pixel_value(x, y, z, c);
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

/*! \fn Image Image :: filter_freeman_4()
 *  \brief Applies the freeman mask \f$(-1,-1,-1)(1,-2,1)(1,1,1)\f$.
 */
Image Image ::filter_freeman_4(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = (get_pixel_value(x-1, y, z, c)+get_pixel_value(x-1, y+1, z, c)+get_pixel_value(x, y+1, z, c)+get_pixel_value(x+1, y+1, z, c)+get_pixel_value(x+1, y, z, c))-1*(get_pixel_value(x-1, y-1, z, c)+get_pixel_value(x, y-1, z, c)+get_pixel_value(x+1,y-1,z,c))+-2*get_pixel_value(x, y, z, c);
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}


/*! \fn Image Image :: filter_freeman_5()
 *  \brief Applies the freeman mask \f$(1,-1,-1)(1,-2,-1)(1,1,1)\f$.
 */
Image Image ::filter_freeman_5(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = (get_pixel_value(x-1, y-1, z, c)+get_pixel_value(x-1, y, z, c)+get_pixel_value(x-1, y+1, z, c)+get_pixel_value(x, y+1, z, c)+get_pixel_value(x+1, y+1, z, c))-1*(get_pixel_value(x+1, y, z, c)+get_pixel_value(x, y-1, z, c)+get_pixel_value(x+1,y-1,z,c))+-2*get_pixel_value(x, y, z, c);
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}


/*! \fn Image Image :: filter_freeman_6()
 *  \brief Applies the freeman mask \f$(1,1,-1)(1,-2,-1)(1,1,-1)\f$.
 */
Image Image ::filter_freeman_6(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = (get_pixel_value(x-1, y-1, z, c)+get_pixel_value(x-1, y, z, c)+get_pixel_value(x-1, y+1, z, c)+get_pixel_value(x, y+1, z, c)+get_pixel_value(x, y-1, z, c))-1*(get_pixel_value(x+1, y-1, z, c)+get_pixel_value(x+1, y, z, c)+get_pixel_value(x+1,y+1,z,c))+-2*get_pixel_value(x, y, z, c);
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

/*! \fn Image Image :: filter_freeman_7()
 *  \brief Applies the freeman mask \f$(1,1,1)(1,-2,-1)(1,-1,-1)\f$.
 */
Image Image ::filter_freeman_7(int num_threads)
{
	unsigned int y,c,z,x;
	int sum;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	int m = 1;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,sum) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width()-m; x++)
			{
				for(y = m; y < this->get_height()-m; y++)
				{
					sum = (get_pixel_value(x-1, y-1, z, c)+get_pixel_value(x-1, y, z, c)+get_pixel_value(x-1, y+1, z, c)+get_pixel_value(x+1, y-1, z, c)+get_pixel_value(x, y-1, z, c))-1*(get_pixel_value(x, y+1, z, c)+get_pixel_value(x+1, y, z, c)+get_pixel_value(x+1,y+1,z,c))+-2*get_pixel_value(x, y, z, c);
					if (sum > 255 || sum < -255)
					{
						sum = 255;
					}
					pixel = (unsigned char)static_cast<unsigned char> (abs(sum));
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	
	return filtered;
	
}

/*! \fn Image Image :: filter_maximum()
 *  \brief Assigns the highest value in the neighborhood.
 * Assigns the highest value in the neighborhood around the desired pixel.
 */


Image Image :: filter_maximum(int num_threads)
{
	unsigned int y,c,z,x;
	unsigned char max,pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,max,x,y) shared(z,c,filtered,chunk)

			for(x = 1; x < this->get_width()-1; x++)
			{
				for(y = 1; y < this->get_height()-1; y++)
				{
					max = 0;
					
					for (unsigned int i = x-1; i< x+2; i++)
					{
						for (unsigned int j = y-1; j< y+2; j++)
						{
							pixel = (this->get_pixel_value(i, j, z, c));
							 
							if (pixel > max)
							{
								max = this->get_pixel_value(i, j, z, c);
							}
						} 
					}
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, max);
				}
				
			 }
			 
		 }
	}  
	return filtered;
}


/*! \fn Image Image :: filter_minimun()
 *  \brief Assigns the lowest value in the neighborhood.
 * Assigns the lowest value in the neighborhood around the desired pixel.
 */

Image Image :: filter_minimum(int num_threads)
{
	unsigned int y,c,z,x;
	unsigned char minimun;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(minimun,x,y) shared(z,c,filtered,chunk)

			for(x = 1; x < this->get_width()-1; x++)
			{
				for(y = 1; y < this->get_height()-1; y++)
				{
					minimun = 255;
					
					for (unsigned int i = x-1; i< x+2; i++)
					{
						for (unsigned int j = y-1; j< y+2; j++)
						{
							if ((this->get_pixel_value(i, j, z, c)) < minimun)
							{
								minimun = this->get_pixel_value(i, j, z, c);
							}
						} 
					}
					
					filtered.set_pixel_value(x, y, z, c, minimun);
				}
				
			 }
			 
		 }
	}  
	 return filtered;
}

Image Image :: filter_order_stadistics(int dim, int order, int num_threads)
{
	unsigned int y,c,z,x;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	
	
	int m = (dim-1)/2;
	
	unsigned char pixel_values [dim*dim];
	unsigned char temp;
	
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel,x,y,pixel_values,temp) shared(z,c,filtered,chunk)

			for(x = m; x < this->get_width(); x++)
			{
				for(y = m; y < this->get_height(); y++)
				{
					for(unsigned int i = x-m; i < x+m+1; i++)
					{
						for(unsigned int j = y-m; j< y+m+1; j++)
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
					pixel = pixel_values[order];
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	 return filtered;
}


// *************************************************************************
// ****************************** NOISES ***********************************
// *************************************************************************

/*! \fn void Image :: salt_pepper(double intensity)
 *  \brief Put pepper (black pixels) and salt(white pixels) 
 *  \param intensity is used to compute the percentage of salt and pepper that is applied to the image.
 */
void Image :: salt_pepper(double intensity, int num_threads)
{
	srand(1);
	unsigned int x,y,z,c;
	int chunk= (this->get_width()/num_threads);
	double percentage = 1-(intensity/100);
	omp_set_num_threads(num_threads);
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(x,y) shared(z,c,chunk)

			for(x = 0; x < this->get_width(); x++)
			{
				for(y = 0; y < this->get_height(); y++)
				{
					double random= 2.0*(rand()-RAND_MAX/2.0)/RAND_MAX;
					if(random > percentage)
					{
						#pragma omp ordered
						(*(this->Img))(x, y, z, c)= 255;
					}	

					else if(random<-1*percentage)
					{
						#pragma omp ordered
						(*(this->Img))(x, y, z, c)= 0;					
					}
				}
				
				
			 }
			 
		 }
	}  
	 
}

/*! \fn void Image :: gaussian_noise(double variance)
 *  \brief This function applies the gaussian noise to an image.
 * 	The gaussian noise increases or decreases intensity to a pixel, depending of the variance.
 *	\param variance this parameter is used to set the value of noise that is applied to the image.
 */
void Image :: gaussian_noise(double variance, int num_threads)
{
	srand(1);
	unsigned int x,y,z,c;
	unsigned char pixel;
	double random;
	omp_set_num_threads(num_threads);
	int chunk= (this->get_width()/num_threads);
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(x,y,pixel, random) shared(z,c,chunk)
			
			for(x = 0; x < this->get_width(); x++)
			{
				for(y = 0; y < this->get_height(); y++)
				{
					random= variance*(rand()-RAND_MAX/variance)/RAND_MAX;
					pixel= this->get_pixel_value(x,y,z,c) + random;
					
					if((pixel<255) & (pixel>0))
					{
						#pragma omp ordered
						(*(this->Img))(x, y, z, c)= pixel;
					}	

				}
				
				
			 }
			 
		 }
	}  
	 
}


/*! \fn Image Image :: interpolation()
 * \brief This function doubles the size of the image and use the closer neighborhood interpolation.
 * \return This function returns the image interpolated.
 */

Image Image :: interpolation(int num_threads)
{
	int i,j=0;
	unsigned int x,y,z,c;
	unsigned char pixel;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image result (2*this->get_width(),2*this->get_height(),this->get_depth(),this->get_spectrum(),0);
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(x,y,pixel) shared(z,c,chunk)
			
			for(y = 0; y < this->get_width(); y++)
			{
				for(x = 0; x < this->get_height(); x++)
				{
					pixel=this->get_pixel_value(x,y,z,c);
					
					#pragma omp ordered
					{
						result.set_pixel_value(x+i,y+j,z,c,pixel);
						result.set_pixel_value(x+1+i,y+j,z,c,pixel);
						result.set_pixel_value(x+i,y+1+j,z,c,pixel);
						result.set_pixel_value(x+1+i,y+1+j,z,c,pixel);
					}
					++i;
				}
				i=0;
				++j;
			}
			j=0;
		}
	}
	return result;
}

/*! \fn CImg<float> Image:: autocovariance (int hor_dis, int ver_dis)
 * \brief Calculates the autocovariance matrix for the image.
 * The covariance, calculates the covariance matrix of an image.
 *  This function calculates something similar to the function below:
\f$
	g(x,y) = \sum \limits_{n=0}^{N} \sum \limits_{m=1}^M \left( f(x,y) - \overline{f(x,y)}\right)\left(f(x + \Delta x, y + \Delta y) - \overline{f(x + \Delta x, y + \Delta y)} \right)
 \f$
 * Where it calculates the variation between two series, one is the normal one, and the other is displaced by two parameters $\Delta x$ \& $\Delta y$.
 *  For an image it its calculated for a neighborhood around each pixel.
 * \return A CImg object, because it must contain float values.
 */


CImg<float> Image:: autocovariance (int hor_dis, int ver_dis,int num_threads)
{
	CImg<float> autocovariance (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	int chunk= (this->get_width()/num_threads);
	unsigned int x,y,z,c;
	int sum;
	omp_set_num_threads(num_threads);
	Image average = this->filter_average(1,num_threads);

	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(x,y,sum) shared(z,c,chunk)
			
			for(x = 3+hor_dis; x < this->get_width()-(3+hor_dis); x++)
			{
				for(y = 3+ver_dis; y < this->get_height()-(3+ver_dis); y++)
				{
					sum = 0;
					for(unsigned int i = x-3; i<x+4; i++)
					{
						for(unsigned int j= y-3; j<y+4; j++)
						{
							sum += ( (this->get_pixel_value(i,j,z,c))  -  average.get_pixel_value(i,j,z,c)) * ( (this->get_pixel_value(i+hor_dis,j+ver_dis,z,c))  -  average.get_pixel_value(i+hor_dis,j+ver_dis,z,c)) ;
						}
					}
					#pragma omp ordered
					autocovariance(x,y,z,c) = sum/49;
				}
			}
		}
	 }
	return autocovariance;
}

/*! \fn Image Image :: variance(int dim)
 * \brief This function compute the variance of an image.
 * The variance is gived by  the summation of the average multiplied by the substraction of the average with the pixel value, squared.
 * \return This function returns the image interpolated.
 */

Image Image :: variance(int dim, int num_threads)
{
	Image filtered (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0); /// 
	int chunk= (this->get_width()/num_threads);
	unsigned int x,y,z,c;
	unsigned char pixel;
	int sum;
	omp_set_num_threads(num_threads);
	for(c = 0; c < this->get_spectrum(); c++)
	{
		for(z = 0; z < this->get_depth(); z++)
		{
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(x,y,sum, pixel) shared(z,c,chunk)
			
			for(x = dim; x < this->get_width()-dim; x++)
			{
				for(y = dim; y < this->get_height()-dim; y++)
				{
					sum = 0;
					double variance=0;
					int kernel_values[(dim*2+1)*(dim*2+1)];
					int cont=0;

					for(unsigned int i = x-dim; i<= x+dim; i++)
					{
						for(unsigned int j = y-dim; j<= y+dim; j++)
						{
							sum += this->get_pixel_value(i, j, z, c);
							kernel_values[cont]=this->get_pixel_value(i, j, z, c);
							cont++;
						}
					}
			
					double average =  sum/((dim*2+1)*(dim*2+1));
					for(int i=0;i<(dim*2+1)*(dim*2+1);i++)
					{
						variance+=pow(kernel_values[i]-average,2)/((dim*2+1)*(dim*2+1));
					}
					
					
					pixel = (unsigned char)static_cast<unsigned char> (variance);
					#pragma omp ordered
					filtered.set_pixel_value(x, y, z, c, pixel);
				}
				
			 }
			 
		 }
	}  
	 return filtered;
}

/*! \fn Image Image :: gray_scale()
 * \brief This function converts an RGB image to one in gray scale.
 * The library uses this conversion: f(x,y)= 0.11R+0.56G + 0.14B
 * Where f is the intensity of the pixel on the gray scale and R,G and B the pixel values on the different channels.
 * \return This function returns the mochromathic image.
 */
 
Image Image :: gray_scale(int num_threads)
{
	unsigned int y,z,x;
	unsigned char pixel_intensity;
	int chunk= (this->get_width()/num_threads);
	omp_set_num_threads(num_threads);
	Image gray_image (this->get_width() , this->get_height(), this->get_depth(), 1, 0); /// 


	for(z = 0; z < this->get_depth(); z++)
	{
		#pragma omp parallel for ordered schedule(dynamic,chunk) private(pixel_intensity,x,y) shared(z,gray_image,chunk)

		for(x = 0; x < this->get_width(); x++)
		{
			for(y = 0; y < this->get_height(); y++)
			{
			
				pixel_intensity = 0.56*this->get_pixel_value(x,y,z,1)+0.14*this->get_pixel_value(x,y,z,0)+0.11*this->get_pixel_value(x,y,z,2);
				#pragma omp ordered
				gray_image.set_pixel_value(x, y, z, 0, pixel_intensity);
				
			}
			 
		 }
	}  
	 return gray_image;	
} 

/*! \fn Image Image :: coorrelogram(unsigned int ver, unsigned int hor)
 * \brief This function compute the coorrelogram of an image.
 * \param unsigned int ver is the vertical distance of the original pixel that we use to compute the coorrelogram.
 * \param unsigned int hor is the horizontal distance of the original pixel that we use to compute the coorrelogram.
 * \return This function returns the coorrelogram image.
 */
Image Image :: coorrelogram(unsigned int ver,unsigned int hor, int num_threads)
{
	
	Image result (256,256, 1, 1, 0);	
	int chunk= (this->get_width()/num_threads);
	unsigned int i,j,x,y;
	unsigned int pixel;
	unsigned char first, secnd;
	omp_set_num_threads(num_threads);
	for(i = 0; i < 256; i++)
	{
		for(j=0; j< 256; j++)
		{
			pixel = 0;
			
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(x,y,first, secnd, pixel) shared(i,j,chunk)
			
			for(x=0; x< (this->get_width()-hor);++x)
			{
				
				for(y=0; y< (this->get_height()-ver);++y)
				{
					first = (this->get_pixel_value(x,y,0,0));
					secnd = (this->get_pixel_value(x+hor, y+ver, 0, 0));
					
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
			#pragma omp ordered
			result.set_pixel_value(i, j, 0, 0, pixel);
			
		}
		
		cout<<"\n"<<i<<"\n"<<endl;	
	}
	return result;

}

/*! \fn Image Image :: coorrelogram(unsigned int ver, unsigned int hor, unsigned int z, unsigned int c)
 * \brief This function compute the coorrelogram of a specified depth and spectrum of an image.
 * \param unsigned int ver is the vertical distance of the original pixel that we use to compute the coorrelogram.
 * \param unsigned int hor is the horizontal distance of the original pixel that we use to compute the coorrelogram.
 * \param unsigned int z is the specified depth of the image that will be obtained the coorrelogram.
 * \param unsigned int c is the specified spectrum of the image that will be obtained the coorrelogram.
 * \return This function returns the coorrelogram image.
 */
Image Image :: coorrelogram_ZC(unsigned int ver,unsigned int hor, unsigned int z, unsigned int c, int num_threads)
{
	
	Image result (256,256, 1, 1, 0);	
	int chunk= (this->get_width()/num_threads);
	unsigned int i,j,x,y;
	unsigned int pixel;
	unsigned char first, secnd;
	omp_set_num_threads(num_threads);
	for(i = 0; i < 256; i++)
	{
		for(j=0; j< 256; j++)
		{
			pixel = 0;
			#pragma omp parallel for ordered schedule(dynamic,chunk) private(x,y,first, secnd, pixel) shared(i,j,chunk)
			
			for(x=0; x< (this->get_width()-hor);++x)
			{
				
				for(y=0; y< (this->get_height()-ver);++y)
				{
					first = (this->get_pixel_value(x,y,z,c));
					secnd = (this->get_pixel_value(x+hor, y+ver, z, c));
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
			#pragma omp ordered
			result.set_pixel_value(i, j, 0, 0, pixel);
			
		}
		
	}
	return result;

}




