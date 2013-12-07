#include <vector>
#include <utility>
#include <iostream>
#include <iomanip>

using namespace std;


class pixel{
	
	public:
		double pix[3];
		
		pixel (){
			
			pix[0]=0;
			pix[1]=0;
			pix[2]=0;
			
			};
	
		pixel(double first,double second , double third){
			
			pix[0]=first;
			pix[1]=second;
			pix[2]=third;
			
			}
		~pixel(void){};
		
		bool operator ==(const pixel &pix){
			
			if(this->pix[0]==pix.pix[0] && this->pix[1]==pix.pix[1] && this->pix[2]==pix.pix[2]) {return true;}
			return false;
			}
		
	};
