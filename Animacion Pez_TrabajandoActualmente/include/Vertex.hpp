/*----------------- Clase Vertice----------------*/
#ifndef VERTEX_H
#define VERTEX_H

#include <armadillo>
class Vertex{
	private:
		arma::frowvec v;
	public:
		int id;
		Vertex(float x,float y,float z, int id);
		Vertex();
		void imprime_Vertice();
		float getX();
		float getY();
		float getZ();
		void set_value(arma::frowvec v_new);
		//homogenea
		arma::fcolvec get_value_toTransform();
		//vertice normal
		arma::frowvec get_value_3dView();
};
#endif
