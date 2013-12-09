#include "../include/ParallelPic.hh"
#include <iostream>
#include "pixel.hh"
#include "utility"
#include "vector"

using namespace std;

Image Image::rgb_hsv()
{
	int Max,Min;
	double H,S,V;
	double RGB[3];
	vector<pair<pixel,pixel> > LUT;
	bool is_in;
	int cont;
	
	Image result (this->get_width() , this->get_height(), this->get_depth(), this->get_spectrum(),255);

	/*for(unsigned int z=0; z< this->get_depth(); ++z)
	{	
		for(unsigned int x=0; x<this->get_width();++x)
		{					
			for(unsigned int y=0; y< this->get_height();++y)
			{	
				//Min=255;
				//Max=0;*/
				//for(unsigned int c=0; c< this->get_spectrum();++c)
				{
					result.Img= (&(this->Img)->RGBtoHSV());
					//result.Img=&img;
					//unsigned char pixel=static_cast<unsigned int>(this->get_pixel_value(x,y,z,c).get_RGBtoHSV());
					//result.set_pixel_value(x,y,z,c,pixel);
				/*
					RGB[c]=imagen.get_pixel_value(x,y,z,c)/255;
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
				
				result.set_pixel_value(x,y,z,0,static_cast<double>(H*255/360));
				result.set_pixel_value(x,y,z,1,static_cast<double>(S*255));
				result.set_pixel_value(x,y,z,2,static_cast<double>(V*255));
				cout<< H*255/360<<"   "<<S*255<<"    "<<V*255<<"    "<<endl;*/	
	/*		}
		}
		}
	}
*/}
	return result;
}

int main()
{
	Image img1 ("../../Multimedia/tulipanes.jpg");
	Image result = img1.rgb_hsv();
	result.display("RESULT");
	
	return 0;
}	

