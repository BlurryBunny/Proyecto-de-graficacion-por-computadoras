#include<bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Bezier_M.hpp"

using namespace std;


Bezier_M :: Bezier_M(float add_value) {
	this->add_value=add_value;
	t=0.0f;
}

Bezier_M :: Bezier_M() {
	add_value = 0.0;
	t=0.0f;
}

void Bezier_M :: set_points(arma::frowvec P1,arma::frowvec P2,arma::frowvec P3,arma::frowvec P4){
	
	point=P1;
	cp1=P2;
	cp2=P3;
	endpoint=P4;

}
	
arma::frowvec Bezier_M  :: calculate_new_points(arma::frowvec center){

	if(t+add_value<1){

		add_value_to_t();

		float t_3 = powf(t,3);
		float t_2 = (powf(t,2)*3)*(1-t);
		float t_1 = (powf(1-t,2)*t*3);
		float t_0 = powf(1-t,3);


		float x = point[0]*t_0 + cp1[0]*t_1 + cp2[0]*t_2 + endpoint[0]*t_3;
		float y = point[1]*t_0 + cp1[1]*t_1 + cp2[1]*t_2 + endpoint[1]*t_3;
		float z = point[2]*t_0 + cp1[2]*t_1 + cp2[2]*t_2 + endpoint[2]*t_3;

		center[0]=x;
		center[1]=y;
		center[2]=z;

		//cout<<"bezier curve still in process"<<endl;
	}

	return (center);
}


void Bezier_M :: reset_value_t(){
	t=0.0;
}

void  Bezier_M ::add_value_to_t(){
	t+=add_value;
}
