#include <iostream>
#include <string>
#include <list>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include<bits/stdc++.h>
#include <stdlib.h>
#include <vector>
#include <armadillo>

#include "include/Vertex.hpp"
#include "include/Edge.hpp"
#include "include/Face.hpp"
#include "include/Object.hpp"
#include "include/Transform.hpp"


using namespace std;

int main(){
    
    arma::frowvec eye = {0.0, 1.0, 20.0};
    arma::frowvec camera = {0.0, 1.0, 20.0};
    arma::frowvec viewup = {0.0, 1.0, 0.0};

    GLFWwindow* window;

    if( !glfwInit() )
    {
        fprintf( stderr, "Fallo al inicializar GLFW\n" );
        getchar();
        return -1;
    }

    window = glfwCreateWindow(1024, 768, "Pez en pecera", NULL, NULL);
    if( window == NULL ) {
        fprintf( stderr, "Fallo al abrir la ventana de GLFW.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LIGTHING);
    //glEnable(GL_LIGHT0);
   // glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

//  Proyecciones
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

//  Proyección en paralelo
    glViewport(0, 0, width, height);
    glOrtho(-30, 30, -4.0, 4.0, -30.0, 30.0);
    //glRotatef(90,0,1,0);
    glRotatef(40,0,1,0);
    glRotatef(20,1,0,1);


//  Proyección en perspectiva
//    glFrustum(-ar, ar, -ar, ar, 2.0, 4.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    /*------------ Declaracion de objetos usados para animacion -----------------*/

    std::vector<Object>my_objs;
    cout<<"hola"<<endl;
    //Pez
    Object fish=Object("objetos/roca.obj", "Fish");
	my_objs.push_back(fish);
	 cout<<"hola"<<endl;
    my_objs[0].read_for_split();
     cout<<"ahhhh"<<endl;
	
    my_objs[0].set_color_rgb(1.0,1.0,0.0,1.0);
	my_objs[0].set_position(1.0,1.5,-0.5); //Un poco a la derecha del origen.
	my_objs[0].set_tam(1.0);
    cout<<"hola";
    
	//Pecera
	my_objs.push_back( Object("objetos/Pecera_MIA/fishbowl_10.obj", "Fishbowl") );
	my_objs[my_objs.size()-1].read_for_split();
	my_objs[my_objs.size()-1].set_color_rgb(1.0,1.0,0.0,0.1);
	my_objs[my_objs.size()-1].set_position(0.0,1.0,0.0); //Un poco a la derecha del origen.
	my_objs[my_objs.size()-1].set_tam(2.0);

	/*Objetos dentro de pecera.*/
	//Madera decorativa
	my_objs.push_back( Object("objetos/Fishbowl_things/SeaWood_01.obj", "Wood") );
	my_objs[my_objs.size()-1].read_for_split();
	my_objs[my_objs.size()-1].set_color_rgb(0.1,0.0,0.0,0.8);
	my_objs[my_objs.size()-1].set_position(-0.5,0.0,-1.0); //Un poco a la derecha del origen.
	my_objs[my_objs.size()-1].set_tam(0.1);

	//Calavera decorativa
	my_objs.push_back( Object("objetos/Fishbowl_things/skull_01.obj", "Skull") );
	my_objs[my_objs.size()-1].read_for_split();
	my_objs[my_objs.size()-1].set_color_rgb(0.1,0.0,0.0,0.8);
	my_objs[my_objs.size()-1].set_position(0.5,0.0,-1.0); //Un poco a la derecha del origen.
	my_objs[my_objs.size()-1].set_tam(0.1);

	//Cofre del tesoro decorativo
	my_objs.push_back( Object("objetos/Fishbowl_things/treasure_001.obj", "Treasure") );
	my_objs[my_objs.size()-1].read_for_split();
	my_objs[my_objs.size()-1].set_color_rgb(0.1,0.0,0.0,0.8);
	my_objs[my_objs.size()-1].set_position(0.0,0.0,-1.0); //Un poco a la derecha del origen.
	my_objs[my_objs.size()-1].set_tam(0.1);


	do {
        glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //visibilidad
        /*gluLookAt(eye[0], eye[1], eye[2], 
                camera[0], camera[1], camera[2], 
                viewup[0], viewup[1], viewup[2])*/;
    	
    	for (int i = 0; i < my_objs.size(); ++i)
    	{
    		my_objs[i].move_and_paint();
    	}
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

	return 0;
}







