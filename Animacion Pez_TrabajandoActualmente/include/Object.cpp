/*----------------- Object----------------*/

#include <vector>
#include <fstream>
#include<bits/stdc++.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <iterator>
#include <armadillo>

#include "Vertex.hpp"
#include "Edge.hpp"
#include "Face.hpp"
#include "Object.hpp"


//#include "Vertex.cpp"
//#include "Edge.cpp"

using namespace std;

vector<string> Object :: split(std::string line, std::string subString){

	std::string auxline;
	std::vector <string> values;
	char char_separation;

	//Se elimina la subcadena de la cadena principal
	while(subString.size()>1){
		while(line.find(subString) < line.size())
		{
			line.erase(line.begin()+line.find(subString));
		}
		subString.erase(subString.begin()+(subString.size()-1));
	}

	char_separation=subString[0];
	stringstream lineStream(line);

	while(getline(lineStream,auxline,char_separation)){
		values.push_back(auxline);
	}

	/*for(int i=0;i<values.size();i++){
		cout<<values[i]<<"\n";
	}*/
	return values;

}

//Separa las palabras por espacios encontrados
void Object :: read_for_split(){
	string line;
	ifstream myfile(nameFile);
	int indexEdges=0;
	int indexVertex=0;
	/*Se abre el archivo*/
	if(myfile.is_open()){
		cout<<"El archivo esta abierto"<<endl;
		while(getline(myfile,line)){
			vector<string> objsValues;
			
			/*Se hace split para eliminar la separacion entre elementos de cadena*/
			objsValues=split(line," ");

			if(objsValues.size()){
			
				//Crea un nuevo Vertice y lo agrega a la lista de vertices
				if(objsValues[0]=="v"){
					v.push_back(Vertex(std::stof(objsValues[1]),std::stof(objsValues[2]),std::stof(objsValues[3]),indexVertex+1));
					indexVertex++;
				
				//Caso contrario es un cara, crea una cara con los vertices de la lista de vertices y agrega el resultado a la lista de caras si es que esta no existe.
				}else if(objsValues[0]=="f"){
					vector<string>objsValuesFaces;
					vector<int>index;
					
					/*Ciclo que recorre la lista con las cadenas tipo 1//3 4//5
						Se empieza desde el segundo elemento evitando el caracter v/vn/f */

					for(int i=1;i<objsValues.size();i++){
						vector<string>objsValuesFaces;
							objsValuesFaces=split(objsValues[i],"//");
												
							/*Si realizo split entonces se recorre los elementos separados en la nueva lista*/
							if(objsValuesFaces.size()){
								
								for(int j=0;j<objsValuesFaces.size();j++){
										
								/* Loop que verifica si el elemento de cadena es un numero*/
									bool bol=true;
									for(int k=0;k<objsValuesFaces[j].size()&&bol!=false;k++){
									/*	Verifica si esta en el rango del codigo ASCII para numeros del 0 al 9*/
										if(objsValuesFaces[j][k]<48 || objsValuesFaces[j][k]>57){
											bol=false;
										}
									}
									
									/*Si el elemento de cadena es un numero entonces lo inserta a la lista de valores*/
									if(bol){
											index.push_back((std::stoi(objsValuesFaces[j]))-1);
									}
									
								}
							}
							
					}
		
					/*ciclo que agrega los elementos de index a la lista auxiliar de listas*/
					vector<Edge>EdgesOfMyFace;
					vector<Edge>EdgesOfMyFace_notRepeated;
					Edge eaux;
					int idEaux=0;
					
					/*Se toma en cuenta solo los elementos pares que son en este caso por el formato de archivos OBJ las referencias a los vertices 
					que forman las aristas */
					
					
					for(int k=0;k<index.size()-2;k+=2){
						eaux=Edge(v[index[k]],v[index[k+2]],indexEdges+1);
					
					/*Muchas caras usan elementos de vertices repetidos, se verifica si ya existe para ocupar el mismo vertice y no hacen redundancia de datos*/	
						if(edgeRepetOrNot(eaux,&idEaux)){
							indexEdges++;
							EdgesOfMyFace_notRepeated.push_back(eaux);
						}else{
							eaux.setID(idEaux);
							//cout<<"idEaux value:  "<<idEaux<<endl;
							//cout<<"Se cambio ID anterior por: "<<eaux.getID()<<endl;
						}
						
						EdgesOfMyFace.push_back(eaux);
					}
					
					/*Se agrega la ultima arista del */
					eaux=Edge(v[index[index.size()-2]],v[index.front()],indexEdges+1);
					if(edgeRepetOrNot(eaux,&idEaux)){
						indexEdges++;
						EdgesOfMyFace_notRepeated.push_back(eaux);
					}else{
						eaux.setID(idEaux);
					}
					
					EdgesOfMyFace.push_back(eaux);
					
					/*se agregan las aristas de la lista auxiliar a la original*/
					for(int l=0; l<EdgesOfMyFace_notRepeated.size(); l++){
						e.push_back(EdgesOfMyFace_notRepeated[l]);
					}
					
					/*se crea lista auxiliar que contiene los vertices de la cara*/
					std::vector<int>ivl;
					for(int u=0;u<index.size()-1;u+=2) {ivl.push_back(index[u]);}
					f.push_back(Face(EdgesOfMyFace,ivl));
				}
			}
		}
	}
	cout<<"Ya terminamos"<<endl;
}

bool Object::edgeRepetOrNot(Edge eaux, int *idEaux){
	bool band=true;
	int index;
	for(index=0;index<e.size() && band == true;index++){
		if(eaux.vi.getX()==e[index].vi.getX() && eaux.vf.getX()==e[index].vf.getX() && eaux.vi.getY()==e[index].vi.getY() && eaux.vf.getY()==e[index].vf.getY() && eaux.vi.getZ()==e[index].vi.getZ() && eaux.vf.getZ()==e[index].vf.getZ()){
			band =false;
		}
		
		if(eaux.vi.getX()==e[index].vf.getX() && eaux.vf.getX()==e[index].vi.getX() && eaux.vi.getY()==e[index].vf.getY() && eaux.vf.getY()==e[index].vi.getY() && eaux.vi.getZ()==e[index].vf.getZ() && eaux.vf.getZ()==e[index].vi.getZ()){
			band= false;
		}
	}
	
	if(band==false){
		*idEaux= e[index-1].getID();
		//cout<<"encontre arista repetida!"<<"\tNuevo ID: "<<*idEaux<<endl;
	}
	
	return band;
}


void Object::imprimeInfo(){
	int index;

			cout << "================= List Vertex ================="<<endl;
		    for(index=0;index<v.size();index++){ 
		    	cout<< "#" <<index+1<< "\t";
		      v[index].imprime_Vertice();
		    }
		    cout << endl; 
		
		
			cout << "================= List Edges ================="<<endl;
			cout<<"References to Vertex"<<endl;
		    for(index=0;index<e.size();index++){ 
		    	cout<<"#"<<index+1<<"\t";
		    	e[index].imprime_Arista();
		    }
		    cout << endl; 
		
		
			cout << "================= List Face ================="<<endl;
			cout<<"References to Edges"<<endl;
		    for(index=0;index<f.size();index++){ 
		    	cout<< "#" <<index+1<< "\t";
					f[index].imprime_Cara();
		    }
}

int Object::printMenu(){
	int option=5;
	cout<<"----------MENU----------\nInserta un numero\n"<<"Objeto:"<<nameFile<<"\n1.- Imprime Lista de Vertices"<<"\n2.- Imprime Lista de Aristas"<<"\n3.- Imprime Lista de Caras"<<"\n4.- Salir"<<endl;
	cin>>option;
	return option;
}


std::vector<int> Object::get_Allfaces_index_vertex(){
	std::vector<int> aux;

	for(int i=0;i<f.size();i++){
		//ponerlos en face

		for(int j=0;j<f[i].get_size_indexVertexList();j++){
			aux.push_back(f[i].get_value_indexVertexList(j));
		}
	}

	return (aux);
}

std::vector<Vertex> Object::get_vertexList(){
	return (v);
}

Object::Object(std::string file){
	nameFile=file;
}

Object::Object(){
	nameFile="objetos/roca.obj";
}




