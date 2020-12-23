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
#include "ObjectToMove.hpp"


using namespace std;

ObjectToMove::ObjectToMove(){
    object_name= "none";
    file_name="objetos/roca.obj";
    angleX=0.0f;
    angleY=0.0f;
    angleZ=0.0f;
    type_move = previous_type_move= 1;
    p2_fromFishbowl = Vertex();
    p3_fromFishbowl = Vertex();
    endpoint = Vertex();
}

ObjectToMove::ObjectToMove(std::string name_f,std::string name_o){
	object_name= name_o;
    file_name= name_f;
    angleX=0.0f;
    angleY=0.0f;
    angleZ=0.0f;
    type_move = previous_type_move= 1;
    p2_fromFishbowl = Vertex();
    p3_fromFishbowl = Vertex();
    endpoint = Vertex();
}

void ObjectToMove :: create_obj(){

    my_Obj = Object(file_name);
    my_Obj.read_for_split();

    //Se obtiene la referencia y los vertices de todas las caras.
    index_vertices = my_Obj.get_Allfaces_index_vertex();
    obj_vertices= my_Obj.get_vertexList();

    //se hace la instancia de la transformacion que usara el objeto.
    Tr = Transform();

    //Instancia de clase bezier, para pez se necesita la mayor fluides de movimiento. 
    if(object_name=="Fish"){
        dinamic_obj=true;
        be = Bezier(0.01);
    }else{
        dinamic_obj=false;
        be= Bezier();
    }

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

void ObjectToMove:: set_angleToRotateX(float angleX){
    this->angleX=angleX;
}

void ObjectToMove:: set_angleToRotateY(float angleY){
    this->angleY=angleY;
}

void ObjectToMove:: set_angleToRotateZ(float angleZ){
    this->angleZ=angleZ;
}

void ObjectToMove :: rotate_Axe_Y(){

    angleY = (angleY < 360.0f) ? angleY+aceleration : 0.0f;
}

void ObjectToMove::act(){

    if(dinamic_obj!=false){
        new_transform();
        move_for_dinamic_obj();
    }
    paint();
}


void  ObjectToMove :: new_transform(){

    //Pez es el unico elemento que se mueve
    if(object_name!="Fish"){
        transf= Tr.S(tam,tam,tam) *Tr.R(1.0f, 0.0f, 0.0f, angleX) * Tr.R(0.0f, 1.0f, 0.0f, angleY) * Tr.R(0.0f, 0.0f, 1.0f, angleZ) * Tr.T(px, py, pz) ;
    }else{
        transf= Tr.S(tam,tam,tam) * Tr.R(1.0f, 0.0f, 0.0f, angleX) * Tr.R(0.0f, 1.0f, 0.0f, angleY) * Tr.R(0.0f, 0.0f, 1.0f, angleZ) * Tr.T(px, py, pz)  ;
        //transf = transf *transf_toMult;
    }
}


void ObjectToMove :: move_for_dinamic_obj(){

    bool res=true;
    for ( unsigned int i=0; i<index_vertices.size() && res!=false; i++ ) {
        
        //transport each vertex according to the porto view.
        Vertex rv = Vertex();
        rv.set_value(transport_point(i,new_vertexs));

        //return a frowvec vector
        arma::frowvec v_to_move = rv.get_value_3dView();
        
        //move of each point according to the curve
        be.set_points(v_to_move,p2_fromFishbowl.get_value_3dView(),p3_fromFishbowl.get_value_3dView(),endpoint.get_value_3dView());

        //calculate the new point
        res = be.calculate_new_points(&v_to_move);
        
        //If the vertex change it will be storage in the list of new vertexs
        if(res){
            rv.set_value(v_to_move);
            new_vertexs[i]=rv;
        }
    }

    //the curve move still in process so the variable not change an everything continue.
    if(res!=false){
        be.add_value_to_t();

        //the curve is already done, so its time to choose another endpoint and points to reference the curve.
    }else{
        be.reset_value_t();
        //considerate if the user press a key to change the movement. if not do a rotate around the z axe.
        set_new_points_Bezier();   
    }
}

//need to access to vertex list of fishbowl
void   ObjectToMove:: set_new_points_Bezier(){
    srand(time(NULL));

    int n_rand,n2_rand,n3_rand;
    int limit= index_vertices_fishbowl.size()-1;
    do{
        n_rand = rand() % limit + 1;
        n2_rand = rand() % limit + 1;
        n3_rand = rand() % limit + 1;
    }while(n_rand<0 && n2_rand<0 && n3_rand<0 && (n_rand == n2_rand || n_rand == n3_rand || n2_rand == n3_rand));

    n_rand= index_vertices_fishbowl[n_rand];
    n2_rand= index_vertices_fishbowl[n2_rand];
    n3_rand= index_vertices_fishbowl[n3_rand];

    cout<<"Numeros random: "<<n_rand<<"    "<< n2_rand<<"    "<< n3_rand<<endl;

    //set new values for the reference and extension to the curve.
    p2_fromFishbowl.set_value(transport_point(n_rand,obj_vertices_fishbowl));
    p3_fromFishbowl.set_value(transport_point(n2_rand,obj_vertices_fishbowl));
    endpoint.set_value(transport_point(n3_rand,obj_vertices_fishbowl));

    cout<<"p2_fromFishbowl ";
    p2_fromFishbowl.imprime_Vertice();
    cout<<endl;

    cout<<"p3_fromFishbowl ";
    p3_fromFishbowl.imprime_Vertice();
    cout<<endl;

    cout<<"endpoint ";
    endpoint.imprime_Vertice();
    cout<<endl;

    p2_fromFishbowl.set_value(modify_vertex_insideFishbowl(p2_fromFishbowl.get_value_3dView()));
    p3_fromFishbowl.set_value(modify_vertex_insideFishbowl(p3_fromFishbowl.get_value_3dView()));

    cout<<"p2_fromFishbowl ";
    p2_fromFishbowl.imprime_Vertice();
    cout<<endl;

    cout<<"p3_fromFishbowl ";
    p3_fromFishbowl.imprime_Vertice();
    cout<<endl;
}   

arma::frowvec ObjectToMove :: modify_vertex_insideFishbowl(arma::frowvec v_to_modify){
    
    if(v_to_modify[0]<0){             // x axe
        v_to_modify[0]+=1.0;
    }else{
        v_to_modify[0]-=1.0;
    }

    if(v_to_modify[1]<0){              // y axe
        v_to_modify[1]+=1.0;
    }else{
        v_to_modify[1]-=1.0;
    }

    if(v_to_modify[2]<0){              // z axe
        v_to_modify[2]+=1.0;
    }else{
        v_to_modify[2]-=1.0;
    }

    return (v_to_modify);
}


arma::frowvec ObjectToMove :: transport_point(int index, std::vector<Vertex> list_vert ){
    arma::fcolvec v_actual = list_vert[index].get_value_toTransform();  //se obtiene el vertice 
    arma::fcolvec vp = transf * v_actual;
    Vertex rv = Vertex();
    rv.set_value(arma::trans(v_actual));
    return (rv.get_value_3dView());
}

void ObjectToMove :: paint() {

    glColor4f(r,g,b,alpha);
    glBegin(GL_TRIANGLES);
        for ( unsigned int i=0; i<new_vertexs.size(); i++ ) {
            arma::frowvec vert = new_vertexs[i].get_value_3dView();
            glVertex3f(vert[0], vert[1], vert[2]);
        }
    glEnd();

    if(dinamic_obj!=false){
        cout<<"imprimi pez"<<endl;
    }
}


void ObjectToMove:: set_value_transform(arma::fmat transf){
    this->transf=transf;
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
    this->px=px;
    this->py=py;
    this->pz=pz;
}

void ObjectToMove:: set_tam(float tam){
    this->tam=tam;
}

std::string ObjectToMove:: get_objName(){
    return object_name;
}

arma::fmat ObjectToMove:: get_transf(){
    return (transf);
}

void ObjectToMove :: set_value_list_vertex_fishbowl(std::vector<int> index_vertices_fishbowl, std::vector<Vertex> obj_vertices_fishbowl){
    this-> index_vertices_fishbowl = index_vertices_fishbowl;
    this-> obj_vertices_fishbowl = obj_vertices_fishbowl;
}

std::vector<int> ObjectToMove :: get_list_index_vertices(){
    return index_vertices;
}

std::vector<Vertex> ObjectToMove :: get_list_obj_vertices(){
    return obj_vertices;
}