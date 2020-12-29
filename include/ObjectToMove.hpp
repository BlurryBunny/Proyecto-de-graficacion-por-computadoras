#ifndef OBJECTTOMOVE_H
#define OBJECTTOMOVE_H

#include <vector>
#include <string>
#include <armadillo>

class ObjectToMove{
	public:
		//variables
		//metodos

	//init
	ObjectToMove(std::string name_f,std::string name_o);
	ObjectToMove();
	void create_obj();
	void init();

	//each frame
	void act();
	void paint();
	void new_transform();
	void move_for_dinamic_obj();
	arma::frowvec transport_point(int index, std::vector<Vertex> list_vert);

	//Bezier curve
	void set_new_points_Bezier();
	bool compare_points (arma::frowvec p1, arma::frowvec p2);
	arma::frowvec create_random_point();

	//Setters
	void set_color(arma::frowvec new_color);
	void set_color_rgb(float r, float g, float b,float alpha);
	void set_aceleration(float aceleration);
	void set_position(float px,float py,float pz);
	void set_angleToRotateX(float angleX);
	void set_angleToRotateY(float angleY);
	void set_angleToRotateZ(float angleZ);
	void set_tam(float tam);
	void set_value_transform(arma::fmat transf);
	void set_value_transform_SizeWorld(arma::fmat size_World);

	/*Rotations*/
	void rotate_Axe_Y();

	//Getters
	std::string get_objName();
	arma::fmat get_transf();
	std::vector<int> get_list_index_vertices();
	std::vector<Vertex> get_list_obj_vertices();
	bool get_dinamic_obj();

	//pruebas
	void imprime_vertices_bezier();

	private:
		//variables
		arma::fmat transf;
		arma::fmat size_World;
		std::vector<int> index_vertices;
		std::vector<Vertex> obj_vertices;

		std::vector<Vertex> new_vertexs;

		//points to control the Bezier curve.
		//Bezier be;
		Bezier_M be_m;

		/*Variables for my Bezier_m */
		arma::frowvec lcp;	//last point select like center
		arma::frowvec center_point;
		arma::frowvec cp1;
		arma::frowvec cp2;
		arma::frowvec endpoint;
		bool dinamic_obj;

		//variables to control the traslation, rotation.
		Transform Tr;
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
		float tam;
		int type_move;
		int previous_type_move;

};

#endif
