/*----------------- Clase Objeto----------------*/

#ifndef OBJECT_H
#define OBJECT_H



#include <vector>
#include <string>
#include <armadillo>

class Object{
	private:
    std::string nameFile;
    std::string name_obj;
    std::vector<Edge> e;
  	std::vector<Vertex> v;
  	std::vector<Face> f;
    std::vector<int> faces_index_vertex;
    arma::fmat transf;
    float angle;

    //Control the color of obj.
    float r;
    float g;
    float b;
    float alpha;

    //Control the position center of the obj.
    float px;
    float py;
    float pz;
    
    //Control move//size/ of the obj.
    float aceleration;
    float tam;

  	std::vector<std::string> split(std::string line, std::string subString);
  	bool edgeRepetOrNot(Edge eaux,int *idEaux);
  	int printMenu();
    void set_Allfaces_index_vertex();

	public:
		void read_for_split();
		void imprimeInfo();
		Object(std::string file, std::string name_obj);

        void move_and_paint();
        
        void set_color_rgb(float r, float g, float b,float alpha);
        void set_aceleration(float aceleration);
        void set_position(float px, float py, float pz);
        void set_tam(float tam);

        void set_value_transform(arma::fmat transf);
        float get_value_angle();
        arma::fmat get_transf();
        std::string get_objName();
};

#endif // FACE_H
