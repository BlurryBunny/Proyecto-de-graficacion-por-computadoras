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

	//each frame
	void act();
	void paint();
	void new_transform();
	void move_for_dinamic_obj();
	
	//Setters
	void set_new_points_Bezier();
	void set_value_list_vertex_fishbowl(std::vector<int> index_vertices_fishbowl, std::vector<Vertex> obj_vertices_fishbowl);
	void set_color(arma::frowvec new_color);
	void set_color_rgb(float r, float g, float b,float alpha);
	void set_aceleration(float aceleration);
	void set_position(float px,float py,float pz);
	void set_angleToRotateX(float angleX);
	void set_angleToRotateY(float angleY);
	void set_angleToRotateZ(float angleZ);
	void set_tam(float tam);
	void set_value_transform(arma::fmat transf);

	//Getters
	std::string get_objName();
	arma::fmat get_transf();
	std::vector<int> get_list_index_vertices();
	std::vector<Vertex> get_list_obj_vertices();
	
	//Auxiliar methods
	arma::frowvec transport_point(int index, std::vector<Vertex> list_vert );
	

	private:
		//variables
		arma::fmat transf;
		std::vector<int> index_vertices;
		std::vector<Vertex> obj_vertices;

		std::vector<int> index_vertices_fishbowl;
		std::vector<Vertex> obj_vertices_fishbowl;

		std::vector<Vertex> new_vertexs;

		//points to control the Bezier curve.
		Bezier be;
		Vertex p2_fromFishbowl;
		Vertex p3_fromFishbowl;
		Vertex endpoint;
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
		float px;
		float py;
		float pz;
		float tam;
		int type_move;
		int previous_type_move;
	//metodos
	void rotate_Axe_Y();
	arma::frowvec modify_vertex_insideFishbowl(arma::frowvec v_to_modify);

};

#endif
