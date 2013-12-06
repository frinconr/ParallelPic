#include <map>
#include <utility>
#include <iostream>
#include <iomanip>

using namespace std;


class pixel{
	
	public:
		int pix[3];
		
		pixel (){
			
			pix[0]=0;
			pix[1]=0;
			pix[2]=0;
			
			};
	
		pixel(int first,int second , int third){
			
			pix[0]=first;
			pix[1]=second;
			pix[2]=third;
			
			}
		~pixel(void){};
		
		
	};
	

int main(){
	
	pixel pic1(0,0,0);
	pixel pic2(19,29,890);
	pair<pixel,pixel> pixel_par(pic1,pic2);
	map<pixel,pixel> LUT;
	
	LUT.insert(pixel_par);
	
	return 0;

}
