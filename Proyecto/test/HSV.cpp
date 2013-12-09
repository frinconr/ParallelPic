#include "../include/ParallelPic.hh"
#include <iostream>
#include "pixel.hh"
#include "utility"
#include "vector"

using namespace std;

Image Image::rgb_hsv(Image imagen)
{
	int Max,Min;
	double H,S,V;
	unsigned int RGB[3];
	vector<pair<pixel,pixel> > LUT;
	bool is_in;
	int cont;
	
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(), 0);

	for(unsigned int z=0; z< imagen.get_depth(); ++z)
	{	
		for(unsigned int x=0; x<get_width();++x)
		{					
			for(unsigned int y=0; y< imagen.get_height();++y)
			{	
				Min=255;
				Max=0;
				for(unsigned int c=0; c< imagen.get_spectrum();++c)
				{
				
					RGB[c]=imagen.get_pixel_value(x,y,z,c);
					if(Max<RGB[c]){Max=RGB[c];}
					if(Min>RGB[c]){Min=RGB[c];}
				}
				
			pixel RGB2 (RGB[0],RGB[1],RGB[2]);
			
			//cout<<LUT.size()<<endl;
			
			for (int m=0; m<LUT.size();++m)
			{
				if (LUT[m].first==RGB2){ 
						is_in=true;
						cont=m;
				}
				
				else 
					is_in=false;
			}
				if(is_in && LUT.size()!=0){
					H=(LUT[cont].second).pix[0];
					S=(LUT[cont].second).pix[1];
					V=(LUT[cont].second).pix[2];
						
				}
				else{

					//Calculate H
					
					if(Max==Min){H=-1;}
					else{
					if(Max==RGB[0] && RGB[1]>=RGB[2]){H=60*(RGB[1]-RGB[2])/(Max-Min);}
					if(Max==RGB[0] && RGB[1]<RGB[2]){H=360+60*(RGB[1]-RGB[2])/(Max-Min);}
					if(Max==RGB[1])	{H=120+60*(RGB[2]-RGB[0])/(Max-Min);}
					if(Max==RGB[2]){H=240+60*(RGB[0]-RGB[1])/(Max-Min);}
					}	
					//Calculate S
					if(Max==0){S=0;}
					else{S=1-Min/Max;}
					
					//Calculate V
					V=Max;
					
					pixel HSV (H,S,V);
					pair<pixel,pixel> pix_pair(RGB2,HSV);
					//LUT.push_back(pix_pair);
				}
				
				cout<<H<<"  "<<S<<"   "<<V<<endl;
				
				result.set_pixel_value(x,y,z,0,H);
				result.set_pixel_value(x,y,z,1,S);
				result.set_pixel_value(x,y,z,2,V);
					
			}
		}
	}

	return result;
}

int main()
{
	Image img1 ("../../Multimedia/tulipanes.jpg");
	Image result = img1.rgb_hsv(img1);
	result.display("RESULT");
	
	return 0;
}	

