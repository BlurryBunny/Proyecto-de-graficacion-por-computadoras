#ifndef BEZIER_M_H
#define BEZIER_M_H

#include <istream>
#include <armadillo>
#include <cmath>

class Bezier_M{
public:
	Bezier_M();
	Bezier_M(float add_value);
	void set_points(arma::frowvec P1,arma::frowvec P2,arma::frowvec P3,arma::frowvec P4);
	arma::frowvec calculate_new_points(arma::frowvec center);
	void reset_value_t();
	void add_value_to_t();
private:
	float add_value;
	float t;
	arma::frowvec point;
	arma::frowvec cp1;
	arma::frowvec cp2;
	arma::frowvec endpoint;

};

#endif