/*----------------- Clase Objeto----------------*/

#ifndef OBJECT_H
#define OBJECT_H



#include <vector>
#include <string>

class Object{
	private:
    std::string nameFile;
    std::vector<Edge> e;
  	std::vector<Vertex> v;
  	std::vector<Face> f;
  	std::vector<std::string> split(std::string line, std::string subString);
  	bool edgeRepetOrNot(Edge eaux,int *idEaux);
  	int printMenu();
	public:
		void read_for_split();
		void imprimeInfo();
		std::vector<int> get_Allfaces_index_vertex();
		std::vector<Vertex> get_vertexList();
		Object(std::string file);
		Object();
};

#endif // FACE_H
