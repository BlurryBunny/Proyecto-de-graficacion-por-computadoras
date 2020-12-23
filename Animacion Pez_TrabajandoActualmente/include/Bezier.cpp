#include<bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Bezier.hpp"

using namespace std;
/*----------------- Clase Bezier----------------*/
Bezier::Bezier(){
	t=0;
	add_value=0.1;
	MB = {{-1,3,-3,1},
		 {3,-6,3,0},
		 {-3,3,0,0},
		 {1,0,0,0}};
	T = {powf(t,3), powf(t,2), t, 1};

}

Bezier::Bezier(float add_value){
	t=0;
	this->add_value=add_value;
	MB = {{-1,3,-3,1},
		 {3,-6,3,0},
		 {-3,3,0,0},
		 {1,0,0,0}};
	T = {powf(t,3), powf(t,2), t, 1};

}

void Bezier :: set_points(arma::frowvec P1,arma::frowvec P2,arma::frowvec P3,arma::frowvec P4){
	arma::fmat GB_aux (4,3);
	GB_aux.row(0)=P1;
	GB_aux.row(1)=P2;
	GB_aux.row(2)=P3;
	GB_aux.row(3)=P4;

	GB = GB_aux;
}

bool Bezier :: calculate_new_points(arma::frowvec *QT_modify){
	
	bool res=false;

	//if(MB && GB ){
		if(t+add_value<1){
			T = {powf(t+add_value,3), powf(t+add_value,2), t+add_value, 1};
			res=true;
		}

		*QT_modify =T * MB *GB;
	//}
	
	return (res);
}

void Bezier:: add_value_to_t(){
	t+=add_value;
}

void Bezier:: reset_value_t(){
	t=0.0;
}