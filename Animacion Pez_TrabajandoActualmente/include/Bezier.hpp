/*----------------- Clase Arista----------------*/
#ifndef BEZIER_H
#define BEZIER_H


#include <istream>
#include <armadillo>
#include <cmath>

class Bezier{
private:
	arma::fmat MB;
	arma::frowvec T;
	arma::fmat GB;
	float t;
	float add_value;
public:
	Bezier();
	Bezier(float add_value);
	void set_points(arma::frowvec P1,arma::frowvec P2,arma::frowvec P3,arma::frowvec P4);
	bool calculate_new_points(arma::frowvec *QT_modify);
	void reset_value_t();
	void add_value_to_t();
};

#endif