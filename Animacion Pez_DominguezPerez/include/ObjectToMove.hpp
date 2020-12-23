#ifndef OBJECTTOMOVE_H
#define OBJECTTOMOVE_H

#include <vector>
#include <string>
#include <armadillo>

class ObjectToMove{
	public:
		//variables
		//metodos
	ObjectToMove(std::string name_f,std::string name_o);
	ObjectToMove();
	void move_and_paint(arma::fmat transf_toMult);
	void set_value_transform(arma::fmat transf);
	void create_obj();
	void set_color(arma::frowvec new_color);
	void set_color_rgb(float r, float g, float b,float alpha);
	void set_aceleration(float aceleration);
	void set_position(float px,float py,float pz);
	void set_angleToRotateX(float angleX);
	void set_angleToRotateY(float angleY);
	void set_angleToRotateZ(float angleZ);
	void set_tam(float tam);
	std::string get_objName();
	arma::fmat get_transf();
	private:
		//variables
		arma::fmat transf;
		float angleX;
		float angleY;
		float angleZ;
		Object my_Obj;
		std::string object_name;
		std::string file_name;
		arma::frowvec color;
		float r;
		float g;
		float b;
		float alpha;
		float aceleration;
		float px;
		float py;
		float pz;
		float tam;
	//metodos
	void rotate_Axe_Y();

};

#endif
