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
#include "include/Bezier_M.hpp"
#include "include/ObjectToMove.hpp"


int view_type=1;    //Values from 1-2
                    // 1.- front view    || 2.- up view
int last_view_type=1;

using namespace std;


int main()
{
    
    GLFWwindow* window;

    if( !glfwInit() )
    {
        fprintf( stderr, "Fallo al inicializar GLFW\n" );
        getchar();
        return -1;
    }

    window = glfwCreateWindow(1000, 900, "Fish in a fishbowl Animation", NULL, NULL);
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
    glOrtho(-12, 12, -8.0, 8.0, -20.0, 20.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //coleccion que almacena todos los objetos
    std::vector<ObjectToMove> my_objs;

    /*-------------------------- Fish --------------------------*/

    ObjectToMove fish = ObjectToMove("objetos/my_fish/fish_Ratio_01.obj","Fish");

    fish.set_color_rgb(1.0,1.0,0.0,1.0);
    fish.set_aceleration(1.0);

    //set the center of the object 
    fish.set_position(-25,-45,10.0);
    fish.set_tam(0.5);
    fish.set_angleToRotateX(-5.0);
    fish.set_angleToRotateZ(5.0);
    fish.create_obj();

    my_objs.push_back(fish);

    /*-------------------------- Skull --------------------------*/

    ObjectToMove skull = ObjectToMove("objetos/fishbowl_things/skull_005.obj","Skull");

    skull.set_color_rgb(1.0,1.0,0.9,0.8);
    skull.set_position(-15.0,-35.0,-3.0);
    skull.set_tam(1.2);
    skull.set_angleToRotateX(-10.0f);
    skull.set_angleToRotateY(2.0f);
    skull.set_angleToRotateZ(2.0f);
    skull.create_obj();

    my_objs.push_back(skull);

    /*-------------------------- Treasure --------------------------*/

    ObjectToMove treasure = ObjectToMove("objetos/fishbowl_things/treasure_003.obj","Treasure");
    
    treasure.set_color_rgb(1.0,0.5,0.0,0.6);
    treasure.set_position(-20,-28.0,10.0);
    treasure.set_tam(1.5);
    treasure.set_angleToRotateX(0.0f);
    treasure.set_angleToRotateY(5.0f);
    treasure.set_angleToRotateZ(5.0f);
    treasure.create_obj();

    my_objs.push_back(treasure);

    /*-------------------------- wood --------------------------*/

    ObjectToMove wood = ObjectToMove("objetos/fishbowl_things/wood_01.obj","Wood");
    
    wood.set_color_rgb(0.1,0.0,0.0,1.0);
    wood.set_position(10.0,-25.0,-10.0);
    wood.set_tam(1.5);
    wood.set_angleToRotateX(0.0f);
    wood.set_angleToRotateY(1.0f);
    wood.set_angleToRotateZ(1.0f);
    wood.create_obj();

    my_objs.push_back(wood);

    /*-------------------------- Fishbowl --------------------------*/

    ObjectToMove fishbowl = ObjectToMove("objetos/my_fishbowl/fishbowl_10.obj","Fishbowl");
    
    fishbowl.set_color_rgb(0.0,0.5,1.0,0.1);
    fishbowl.set_position(0.0,-2.0,0.0);
    fishbowl.set_tam(1.0);
    fishbowl.create_obj();

    my_objs.push_back(fishbowl);

    /*-------------------------- Transform every object according to the dimentions --------------------------*/
    
    Transform Tr = Transform();

    //Tranformation which every object inside the fishbolw needs to reduce their size.
    arma::fmat transform_fishbowl= Tr.S(0.15, 0.15, 0.15);

    //If the object is the fishbowl then it rescalate it size to 1.5 
    arma::fmat trans_size = Tr.S(5.0,3.5,3.0);

    /*---------------------------- init elements for dinamic objects -----------------------------------*/

    for (int i = 0; i < my_objs.size(); ++i)
    {
        if(my_objs[i].get_dinamic_obj()!=false){
            my_objs[i].set_new_points_Bezier();
        }

        //put the transform of each object inside the animation so it will put in screen.
        (my_objs[i].get_objName()!="Fishbowl") ? my_objs[i].set_value_transform_SizeWorld(transform_fishbowl) : my_objs[i].set_value_transform_SizeWorld(trans_size);
        my_objs[i].init();   
    }

    //Animation, reset the objects each frame
    do {
        glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


        /* --------------------- CHANGE VIEW --------------------- */

        //check if a key was press
        if( glfwGetKey(window, GLFW_KEY_UP ) ==  GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS ){

            last_view_type = view_type;

            //front view
            if( glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS ){
                view_type = 1;
            }

            //view up
            if( glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS ){
                view_type = 2;
            }
        }

        switch(view_type){
            case 1: 
                break;
            case 2:
                glRotatef(90,1,0,0);
                break;
        }

        /*---------------------  ACTION FOR EVERY OBJECT EACH FRAME --------------------- */
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
