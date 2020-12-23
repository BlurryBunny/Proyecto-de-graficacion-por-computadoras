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

#include "Vertex.hpp"
#include "Edge.hpp"
#include "Face.hpp"
#include "Object.hpp"
#include "Transform.hpp"
#include "ObjectToMove.hpp"

using namespace std;

ObjectToMove::ObjectToMove(){
    object_name= "none";
    file_name="objetos/roca.obj";
    angleX=0.0f;
    angleY=0.0f;
    angleZ=0.0f;
}

ObjectToMove::ObjectToMove(std::string name_f,std::string name_o){
	object_name= name_o;
    file_name= name_f;
    angleX=0.0f;
    angleY=0.0f;
    angleZ=0.0f;
}

void ObjectToMove :: create_obj(){
    cout<<file_name<<endl;
    my_Obj = Object(file_name);
    my_Obj.read_for_split();
    //my_Obj.imprimeInfo();
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

void ObjectToMove::move_and_paint(arma::fmat transf_toMult){

    Transform Tr = Transform();
    std::vector<int> index_vertices = my_Obj.get_Allfaces_index_vertex();
    std::vector<Vertex> obj_vertices= my_Obj.get_vertexList();
    std::vector<Vertex> new_vertexs;

    //Pez es el unico elemento que se mueve
    if(object_name!="Fish"){
        transf= transf_toMult * Tr.R(1.0f, 0.0f, 0.0f, angleX) * Tr.R(0.0f, 1.0f, 0.0f, angleY) * Tr.R(0.0f, 0.0f, 1.0f, angleZ) * Tr.T(px, py, pz) * Tr.S(tam,tam,tam);
    }else{
        //switch para tipo de movimiento
        rotate_Axe_Y();
        transf=  transf_toMult* Tr.R(1.0f, 0.0f, 0.0f, angleX) * Tr.R(0.0f, 1.0f, 0.0f, angleY) * Tr.R(0.0f, 0.0f, 1.0f, angleZ) * Tr.T(px, py, pz) *Tr.S(tam,tam,tam) ;
        //transf = transf *transf_toMult;
    }

    //se ponen los vertices en la nueva 
        for ( unsigned int i=0; i<index_vertices.size(); i++ ) {
            arma::fcolvec v = obj_vertices[index_vertices[i]].get_value_toTransform();
            arma::fcolvec vp = transf * v;
            Vertex rv = Vertex();
            rv.set_value(arma::trans(vp));
            new_vertexs.push_back(rv);
        }
        
        glColor4f(r,g,b,alpha);
        glBegin(GL_TRIANGLES);
        for ( unsigned int i=0; i<new_vertexs.size(); i++ ) {
            arma::frowvec vert = new_vertexs[i].get_value_3dView();
            glVertex3f(vert[0], vert[1], vert[2]);
            
        }
        glEnd();
        // Fin dibujado de la Tierra
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

