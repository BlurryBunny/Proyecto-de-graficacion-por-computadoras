#include <stdio.h>
#include<bits/stdc++.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <armadillo>
#include <string>
#include <vector>

#include "include/Vertex.hpp"
#include "include/Edge.hpp"
#include "include/Face.hpp"
#include "include/Object.hpp"
#include "include/Transform.hpp"
#include "include/Bezier.hpp"
#include "include/ObjectToMove.hpp"


int view_type=1;
int last_view_type=1;

using namespace std;


int main()
{
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

    window = glfwCreateWindow(1024, 768, "Fish in a fishbowl Animation", NULL, NULL);
    if( window == NULL ) {
        fprintf( stderr, "Fallo al abrir la ventana de GLFW.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

//  Proyecciones
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    float ar = width / height;

    //  Proyección en paralelo
    glViewport(0, 0, width, height);

    //Vista desde el frente
    glOrtho(-15, 15, -0.5, 6.0, -30.0, 30.0);
    //glOrtho(-10, 10, -6.5, 6.5, -30.0, 30.0);
    //glRotatef(90,1,0,0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //coleccion que almacena todos los objetos
    std::vector<ObjectToMove> my_objs;

    ObjectToMove fish = ObjectToMove("objetos/Pez_Final/fish_Ratio_01.obj","Fish");
    fish.create_obj();
    my_objs.push_back(fish);

    ObjectToMove skull = ObjectToMove("objetos/fishbowl_things/skull_005.obj","Skull");
    skull.create_obj();
    my_objs.push_back(skull);

    ObjectToMove treasure = ObjectToMove("objetos/fishbowl_things/treasure_003.obj","Treasure");
    treasure.create_obj();
    my_objs.push_back(treasure);

    ObjectToMove wood = ObjectToMove("objetos/fishbowl_things/wood_01.obj","Wood");
    wood.create_obj();
    my_objs.push_back(wood);

    ObjectToMove fishbowl = ObjectToMove("objetos/Pecera_MIA/fishbowl_10.obj","Fishbowl");
    fishbowl.create_obj();
    my_objs.push_back(fishbowl);

    /*-------------------------- Fish --------------------------*/
    my_objs[0].set_color_rgb(1.0,1.0,0.0,1.0);
    my_objs[0].set_aceleration(1.0);
    my_objs[0].set_position(-25,35,0.0);
    my_objs[0].set_tam(0.5);
    my_objs[1].set_angleToRotateX(-35.0);
    my_objs[1].set_angleToRotateZ(-35.0);


    /*-------------------------- Skull --------------------------*/
    my_objs[1].set_color_rgb(1.0,1.0,0.9,0.8);
    my_objs[1].set_position(-15.0,0.0,-3.0);
    my_objs[1].set_tam(1.2);
    my_objs[1].set_angleToRotateX(-55.0);
    my_objs[1].set_angleToRotateY(-45.0);
    my_objs[1].set_angleToRotateZ(45.0);

    /*-------------------------- Treasure --------------------------*/
    my_objs[2].set_color_rgb(1.0,0.5,0.0,0.6);
    my_objs[2].set_position(-20,3.0,10.0);
    my_objs[2].set_tam(1.5);
    my_objs[2].set_angleToRotateX(0.0);
    my_objs[1].set_angleToRotateY(45.0);
    my_objs[1].set_angleToRotateZ(45.0);

    /*-------------------------- wood --------------------------*/
    my_objs[3].set_color_rgb(0.1,0.0,0.0,1.0);
    my_objs[3].set_position(30,9.0,-10.0);
    my_objs[3].set_tam(1.5);
    my_objs[3].set_angleToRotateX(0.0f);
    my_objs[1].set_angleToRotateY(90.0);
    my_objs[1].set_angleToRotateZ(35.0);

    /*-------------------------- Fishbowl --------------------------*/
    my_objs[4].set_color_rgb(0.0,0.5,1.0,0.1);
    my_objs[4].set_position(0.0,0.0,0.0);
    my_objs[4].set_tam(1.0);

     /*---------------------------- init the move of fish -----------------------------------*/
    /*Stablish the list of vertex and index vertex from fishbowl to fish*/
    cout<<"\nPonemos la lista de vertices en Pez de Pecera"<<endl;
    my_objs[0].set_value_list_vertex_fishbowl(my_objs[4].get_list_index_vertices(),my_objs[4].get_list_obj_vertices());
    /*Stablish an endpoint to the fish so it will be move to that point in a Bezier curve*/
    cout<<"\nPonemos nuevos puntos para curva de bezier"<<endl;
    my_objs[0].set_new_points_Bezier();


    //Animacion
    do {
        glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //check if a key was press
        if( glfwGetKey(window, GLFW_KEY_UP ) ==  GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS && last_view_type!=view_type){
            
            last_view_type=view_type;

            //set the rotation to origin depending each view type
            switch(view_type){
                case 1:
                    glOrtho(0, 0, 0, 0, 0, 0);
                    break;
                case 2:
                    glOrtho(0, 0, 0, 0, 0, 0);
                    glRotatef(-90,1,0,0);
                    break;
                default:
                    break;
            }

            //front view
            if( glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_KEY_DOWN ){
                glOrtho(-15, 15, -1.0, 6.0, -30.0, 30.0);
                view_type=1;
            }

            //view up
            if( glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_KEY_UP ){
                 glOrtho(-10, 10, -5.0, 5.0, -30.0, 30.0);
                 glRotatef(90,1,0,0);
                 view_type=2;
            }
        }

        //move, rotation for each object
        for(unsigned int i=0; i<my_objs.size();i++){
            my_objs[i].act();  
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        sleep(0.16);

    } while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    glfwTerminate();

    return 0;
}

