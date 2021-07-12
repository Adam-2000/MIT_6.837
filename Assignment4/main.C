#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "scene_parser.h"
#include "image.h"
#include "camera.h"
#include "hit.h"
#include "group.h"
#include "light.h"
#include "ray.h"
#include "glCanvas.h"
#include "raytracer.h"
#include "render.h"
#include "sphere.h"
#define N_LARGE 10000

int main(int argc, char** argv){
    
    argc_glb = argc;
    argv_glb = argv;
    char *input_file = NULL;
    bool gui_flag = false;
    // sample command line:
    // raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

    for (int i = 1; i < argc_glb; i++) {
        if (!strcmp(argv_glb[i],"-input")) {
            i++; assert (i < argc_glb); 
            input_file = argv_glb[i];
        } else if (!strcmp(argv_glb[i],"-gui")) {
            gui_flag = true;
        } else if (!strcmp(argv_glb[i],"-tessellation")) {
            i++; assert (i < argc_glb);
            theta_steps = atof(argv_glb[i]);
            i++; assert (i < argc_glb);
            phi_steps = atof(argv_glb[i]);
        } else if (!strcmp(argv_glb[i],"-gouraud")) {
            gouraud_flag = true;
        } else {
            ;   
        }
    }

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc); 
            input_file = argv[i];
        } else if (!strcmp(argv[i],"-gui")) {
            gui_flag = true;
        }
    }
    if (gui_flag){
        SceneParser scene = SceneParser(input_file);
        glutInit(&argc, argv);
        GLCanvas glc = GLCanvas();
        glc.initialize(&scene, render, traceRayFunction);
        // ;
    } else {
        render();
    }

    return 0;

}