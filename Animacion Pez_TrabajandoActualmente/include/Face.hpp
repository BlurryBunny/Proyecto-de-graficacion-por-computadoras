/*----------------- Clase Cara----------------*/

#ifndef FACE_H
#define FACE_H
#include <vector>
#include <armadillo>


class Face{
	private:
		std::vector<int>index_vertex_list;
		arma::frowvec calculateNormal();
		arma::frowvec vertexRes(Vertex vf, Vertex vi);
	public:
		std::vector<Edge> list_Edges;
		arma:: frowvec normal;
		void imprime_Cara();
		int get_size_indexVertexList();
		int get_value_indexVertexList(int index);
		Face();
		Face(std::vector<Edge> e_list);
		Face(std::vector<Edge> e_list,std::vector<int>ivl);
		

};

#endif // FACE_H
