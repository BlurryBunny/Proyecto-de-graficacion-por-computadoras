/*-----------Planet Class---------------*/
#include <vector>
#include <fstream>
#include<bits/stdc++.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <iterator>
#include <armadillo>
#include <stdlib.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <time.h>

#include "Vertex.hpp"
#include "Edge.hpp"
#include "Face.hpp"
#include "Object.hpp"
#include "Transform.hpp"
#include "Bezier.hpp"
#include "Bezier_M.hpp"
#include "ObjectToMove.hpp"

#define Xmin 40  
#define Xmax 60 

#define Ymin_Xmin 10
#define Ymax_Xmin 45
#define Ymin_Xmax 15
#define Ymax_Xmax 35

#define Zmax_Xmin 35 
#define Zmax_Xmax 25
using namespace std;


/*------------------------------------ Constructors ------------------------------------*/
ObjectToMove::ObjectToMove(){
    object_name= "none";
    file_name="objetos/roca.obj";
    angleX=0.0f;
    angleY=0.0f;
    angleZ=0.0f;
    sense_onX = 0.0f;
    //sense_onY = 0.0f;
    sense_onZ = 0.0f;

    

}

ObjectToMove::ObjectToMove(std::string name_f,std::string name_o){
    object_name= name_o;
    file_name= name_f;
    angleX=0.0f;
    angleY=0.0f;
    angleZ=0.0f;
    sense_onX = 0.0f;
    //sense_onY = 0.0f;
    sense_onZ = 0.0f;


}

/*------------------------------------ Init ------------------------------------*/
void ObjectToMove :: create_obj(){
    srand(time(NULL));

    my_Obj = Object(file_name);
    my_Obj.read_for_split();

    //Obtain the references and vertex of each face. 
    index_vertices = my_Obj.get_Allfaces_index_vertex();
    obj_vertices= my_Obj.get_vertexList();

    Tr = Transform();   // Transport / Rotation / Scale

    //gets value for bezier and dinamic object
    if(object_name=="Fish"){
        dinamic_obj=true;
        be_m = Bezier_M(0.05);

    }else{
        dinamic_obj=false;
        be_m = Bezier_M();
    }


}

void ObjectToMove :: init (){

    lcp= {center_point[0], center_point[1], center_point[2]};
    new_transform();
    //put in memory the values of the new vertex according to perspective. 
    //if the obj is static this values not change in all animation.
    for ( unsigned int i=0; i<index_vertices.size(); i++ ) {
        arma::fcolvec v = obj_vertices[index_vertices[i]].get_value_toTransform();
        arma::fcolvec vp = transf * v;
        Vertex rv = Vertex();
        rv.set_value(arma::trans(vp));
        new_vertexs.push_back(rv);
    }
    

}


/*------------------------------------ Each Frame ------------------------------------*/
void ObjectToMove::act(){

    if(dinamic_obj!=false){
        move_for_dinamic_obj();
    }
    paint();
}

void ObjectToMove :: paint() {
    glColor4f(r,g,b,alpha);
    glBegin(GL_TRIANGLES);
        for ( unsigned int i=0; i<new_vertexs.size(); i++ ) {
            arma::frowvec vert = new_vertexs[i].get_value_3dView();
            glVertex3f(vert[0], vert[1], vert[2]);
        }
    glEnd();
}

/*------------------------------------ Actions for dinamic object with bezier curves ------------------------------------*/

void ObjectToMove :: move_for_dinamic_obj(){

    //check if the points move according the new bezier points.
    //imprime_vertices_bezier();

    lcp= {center_point[0], center_point[1], center_point[2]};
    center_point = be_m.calculate_new_points(center_point);

    /*The bezier curve not finish yet*/
    if(lcp[0]!=center_point[0] || lcp[1]!=center_point[1] || lcp[2]!=center_point[2]){

        new_transform();
        for(unsigned int i=0; i<index_vertices.size(); i++){
            new_vertexs[i].set_value(transport_point(index_vertices[i],obj_vertices));
        }

    }else{

        //the curve is already done, so its time to choose another endpoint and points to reference the curve.
        be_m.reset_value_t();
        //considerate if the user press a key to change the movement. if not do a rotate around the z axe.
        set_new_points_Bezier(); 
        be_m.set_points(center_point,cp1,cp2,endpoint);
    }
}

void  ObjectToMove :: new_transform(){


    if(dinamic_obj!=false){
        //The object need rotate corresponding to it movement.
  
        check_new_senseRotation();

        //Rotations (update the angle of each axe)
        if(sense_onX!=0.0f){
            //angleX=rotate_on_Axe(angleX,45.0f,sense_onX,5.0f);
           //if (angleX==45) { sense_onX=0.0; }
        }

        if(sense_onY!=0.0){
            float angle_expected;
            (sense_onY==1.0) ? angle_expected=180.0 : angle_expected=0.0;
            angleY=rotate_on_Axe(angleY,angle_expected,sense_onY,45.0f);
        }

        if(sense_onZ!=0.0f){
            angleZ=rotate_on_Axe(angleZ,20.0f,sense_onZ,4.0f);
           //if(angleX==180) { sense_onX=0.0f; }
        }
        
    }
    
    transf= size_World * Tr.S(tam,tam,tam) *Tr.R(1.0f, 0.0f, 0.0f, angleX) * Tr.R(0.0f, 1.0f, 0.0f, angleY) * Tr.R(0.0f, 0.0f, 1.0f, angleZ) * Tr.T(center_point[0],center_point[1], center_point[2]) ;

}


arma::frowvec ObjectToMove :: transport_point(int index, std::vector<Vertex> list_vert ){
    arma::fcolvec v_actual = list_vert[index].get_value_toTransform();  //se obtiene el vertice 
    arma::fcolvec vp = transf * v_actual;
    Vertex rv = Vertex();
    rv.set_value(arma::trans(vp));
    return (rv.get_value_3dView());
}

/*------------------------------------ Bezier curves ------------------------------------*/
void   ObjectToMove:: set_new_points_Bezier(){
    
    cout<<"================ Nuevos puntos de bezier ====================="<<endl;
    do{
        cp1 = create_random_point();
        cp2 = create_random_point();
        endpoint = create_random_point();
    }while(compare_points(cp1,cp2) || compare_points(cp1,endpoint) || compare_points(cp2,endpoint) );

    be_m.set_points(center_point,cp1,cp2,endpoint);
    imprime_vertices_bezier();
}

arma:: frowvec ObjectToMove :: create_random_point(){

    float new_x;
    float new_y;
    float new_z;
    int miniMax;
    int signo;

    //we only need a max or min for X because other axes depends of axe X.

        miniMax = rand() % 2;   //  0-> min   || 1-> max

        signo = rand() % 2;     //  0->negative || 1->positive 
        new_x = (miniMax < 1) ? rand() % Xmin : (rand() % Xmax - Xmin - 1) + Xmin + 1;
        new_x = (signo < 1) ? new_x * -1.0 : new_x;


        signo = rand() % 2;     //  0->negative || 1->positive 
        new_y = (miniMax < 1 ) ? (rand() % Ymax_Xmin - Ymin_Xmin - 1) + Ymin_Xmin + 1 : (rand() % Ymax_Xmax - Ymin_Xmax - 1) + Ymin_Xmax + 1;
        new_y = (signo < 1) ? new_y * -1.0 : new_y;

        signo = rand() % 2;     //  0->negative || 1->positive 
        new_z = (miniMax < 1) ? (rand() % Zmax_Xmin -1) +1  : (rand() % Zmax_Xmax -1) +1;
        new_z = (signo < 1) ? new_z * -1.0 : new_z;

    arma::frowvec new_point = {new_x, new_y, new_z};

    //cout<<"random point: "<< new_point[0] << " | "<< new_point[1] << " | "<< new_point[2]<<endl;
    return(new_point);
}


bool ObjectToMove :: compare_points (arma::frowvec p1, arma::frowvec p2){
    bool res = false; 

    if(p1[0] == p2 [0] && p1[1] == p2[1] && p1[2] == p2[2]){
        res=true;
    }

    return (res);
}

/*------------------------------------ Rotations ------------------------------------*/
float ObjectToMove :: rotate_on_Axe(float angle, float angle_exp,float sense,float add_value){
    if(angle!=angle_exp){angle+=(add_value*sense);}
    return angle;
}

void ObjectToMove :: check_new_senseRotation(){
    //Check axe x

    //check axe y
     if(lcp[0]<center_point[0] && sense_onY!=1.0){
        sense_onY=1.0;
        //angleY=0;
    }else if(lcp[0]>center_point[0] && sense_onY!=-1.0){
        sense_onY=-1.0f;
        //angleY=0;
    }



    //Check axe z
    if(lcp[2]<center_point[2] && sense_onZ!=1.0){
        sense_onZ=1.0;
        //angleZ=0;
    }else if(lcp[2]>center_point[2] && sense_onZ!=-1.0){
        sense_onZ=-1.0;
        //angleZ=0;
    }

}
/*------------------------------------ Setters ------------------------------------*/
void ObjectToMove:: set_value_transform(arma::fmat transf){
    this->transf=transf;
}

void ObjectToMove:: set_value_transform_SizeWorld(arma::fmat size_World){
    this->size_World=size_World;
}

void ObjectToMove::set_color(arma::frowvec new_color){
    color={new_color[0],new_color[1],new_color[2]};
}

void ObjectToMove::set_color_rgb(float r, float g, float b,float alpha){
    this->r=r;
    this->g=g;
    this->b=b;
    this->alpha=alpha;
}

void ObjectToMove::set_aceleration(float aceleration){
    this->aceleration=aceleration;
}

void ObjectToMove:: set_position(float px,float py,float pz){

    center_point= {px,py,pz};
}

void ObjectToMove:: set_tam(float tam){
    this->tam=tam;
}

void ObjectToMove:: set_angleToRotateX(float angleX){
    this->angleX=angleX;
}

void ObjectToMove:: set_angleToRotateY(float angleY){
    this->angleY=angleY;
}

void ObjectToMove:: set_angleToRotateZ(float angleZ){
    this->angleZ=angleZ;
}

/*------------------------------------ Getters ------------------------------------*/

std::string ObjectToMove:: get_objName(){
    return object_name;
}

arma::fmat ObjectToMove:: get_transf(){
    return (transf);
}


std::vector<int> ObjectToMove :: get_list_index_vertices(){
    return index_vertices;
}

std::vector<Vertex> ObjectToMove :: get_list_obj_vertices(){
    return obj_vertices;
}

bool ObjectToMove:: get_dinamic_obj(){
    return dinamic_obj;
}

/*------------------------------------ Pruebas / Codigo por modificar ------------------------------------*/

void ObjectToMove :: imprime_vertices_bezier(){

    cout<< "\n --------------- new vertexs ------------------"<<endl;
    cout<<"centro: "<<center_point[0] << " | " <<center_point[1] << " | " <<center_point[2]<<endl;
    cout<<"cp1: "<< cp1[0] << " | " << cp1[1] << " | " << cp1[2] << endl;
    cout<<"cp2 "<< cp2[0] << " | " << cp2[1] << " | " << cp2[2] << endl;
    cout<<"endpoint: "<< endpoint[0] << " | " << endpoint[1] << " | " << endpoint[2] << endl;
}

