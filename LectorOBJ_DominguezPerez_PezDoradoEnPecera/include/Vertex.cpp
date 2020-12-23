/*----------------- Clase Vertice----------------*/
#include<bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <armadillo>
#include "Vertex.hpp"

using namespace std;

Vertex::Vertex(){
	v={0.0,0.0,0.0};
	id=100000;
}


Vertex::Vertex(float x,float y,float z,int id){
	v={x,y,z};
	this->id=id;
}


void Vertex :: imprime_Vertice(){
	cout<<"Coordenadas: ( "<<v[0]<<","<<v[1]<<","<<v[2]<<" )"<<endl;
}

float Vertex:: getX(){
	return (v[0]);
}

float Vertex:: getY(){
	return (v[1]);
}

float Vertex:: getZ(){
	return (v[2]);
}

void Vertex:: set_value(arma::frowvec v_new){
	v={v_new[0],v_new[1],v_new[2]};
}

arma::frowvec Vertex:: get_value_3dView(){
	return (v);
}

arma::fcolvec Vertex:: get_value_toTransform(){
	arma::fcolvec hom= {{v[0]},{v[1]},{v[2]},{1.0}};
	return (hom);
}

