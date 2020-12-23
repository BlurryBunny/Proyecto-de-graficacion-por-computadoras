#include<bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Vertex.hpp"
#include "Edge.hpp"


using namespace std;
/*----------------- Clase Arista----------------*/
Edge::Edge(){
}

Edge::Edge(Vertex vi, Vertex vf, int id){
	this->vi=vi;
	this->vf=vf;
	this->id=id;

}

void Edge::imprime_Arista(){
	cout<<"(V"<< vi.id <<",V"<<vf.id<<")\n";
}

int Edge:: getID(){
	return id;
}

void Edge:: setID(int idnuevo){
	this->id=idnuevo;
}

Vertex Edge ::getVI(){
	return vi;
}

Vertex Edge:: getVF(){
	return vf;
}


