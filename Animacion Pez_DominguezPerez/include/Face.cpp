#include<bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <armadillo>

#include "Vertex.hpp"
#include "Edge.hpp"
#include "Face.hpp"
using namespace std;

Face::Face(){

}

Face::Face(std::vector<Edge> e_list){
		list_Edges=e_list;
		index_vertex_list.push_back(-1);
		normal = calculateNormal();
}

Face::Face(std::vector<Edge> e_list,std::vector<int>ivl){
		list_Edges=e_list;
		index_vertex_list=ivl;
		normal = calculateNormal();
}

void Face:: imprime_Cara(){
			int index;
			
			for(index=0;index<list_Edges.size();index++){
				cout<< list_Edges[index].getID() << ",";
			}
			cout<<endl;
			cout<<"VN: \t"<<"("<<normal[0]<<","<<normal[1]<<","<<normal[2]<<")\n"<<endl;
}

//Calculo de normal de la cara actual
arma::frowvec Face :: calculateNormal(){

	//Nuestro lector solamente lee caras trianguladas por lo que existiran siempre maximo 3 Aristas para cada cara.
	//Para obtener la normal de una cara triangulas entonces obtenemos la determiante colvec = (i, j ,k) (E3.VI - E1.VI) (E1.VF -E1.VI)
		arma:: frowvec row1= vertexRes(list_Edges.back().getVI(), list_Edges.front().getVI());
		arma:: frowvec row2= vertexRes(list_Edges.front().getVF(), list_Edges.front().getVI());
		arma:: frowvec res = arma:: cross(row1, row2);
		
	return (res);
	
}

//Realiza la resta de las componentes de dos vertices ubicados en un plano tridimensional
arma::frowvec Face:: vertexRes(Vertex vf, Vertex vi){
	arma:: frowvec v= {vf.getX() -vi.getX(),vf.getY() -vi.getY(),vf.getZ() -vi.getZ()};
	return(v);
}


int Face :: get_size_indexVertexList(){
	return (index_vertex_list.size());
}

int Face :: get_value_indexVertexList(int index){
	return (index_vertex_list[index]);
}