#include "../include/ParallelPic.hh"
#include <iostream>

using namespace std;

double** Image::rgb_hsv(Image imagen)
{
	int cont=0;
	unsigned int Max,Min=0;
	double H,S,V;
	unsigned int RGB[3];
	int rows= imagen.get_height()*imagen.get_width();
	int colmns=3;
	double **ptr_table;
	ptr_table=new double* [rows];
	
	for(int i=0;i<rows;++i){ptr_table[i]=new double[colmns];}
	
	for(unsigned int z=0; z< imagen.get_depth(); ++z)
	{	
		for(unsigned int x=0; x<get_width();++x)
		{					
			for(unsigned int y=0; y< imagen.get_height();++y)
			{	
				for(unsigned int c=0; c< imagen.get_spectrum();++c)
				{
					RGB[c]=imagen.get_pixel_value(x,y,z,c);
					if(Max<=RGB[c]){Max=RGB[c];}
					if(Min>=RGB[c]){Min=RGB[c];}
					
				}
			//Calculate H
			
			if(Max==Min){}
			if(Max==RGB[0] & RGB[1]>=RGB[2]){H=60*(RGB[1]-RGB[2])/(Max-Min);}
			if(Max==RGB[0] & RGB[1]<RGB[2]){H=360+60*(RGB[1]-RGB[2])/(Max-Min);}
			if(Max==RGB[1])	{H=120+60*(RGB[2]-RGB[0])/(Max-Min);}
			if(Max==RGB[2]){H=240+60*(RGB[0]-RGB[1])/(Max-Min);}
			
			//Calculate S
			if(Max==0){S=0;}
			else{S=1-Min/Max;}
			
			//Calculate V
			V=Max;
			
			for(int i=0;i<cont;++i)
			{
				if(ptr_table[i][0]==H & ptr_table[i][0]==S & ptr_table[i][0]==V){i=cont;}
				else
				{
					ptr_table[cont][0]=H;
					ptr_table[cont][1]=S;
					ptr_table[cont][2]=V;
					cont++;
				}
			}}
		}
	}
return ptr_table;
}

int main()
{
	Image img1 ("../../Multimedia/openmp.jpg");
	//img1.display("soba");
	double** ptr_matrix = img1.rgb_hsv(img1);
	for(int i=0;i<10;++i){
	cout<<"H:"<<ptr_matrix[i][0]<<endl;
	}
	return 0;
}	

