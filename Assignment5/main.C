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
    bool visualize_grid_flag = false;
    bool shadow_flag = false;
    int bounces = 0;
    float weight = 0.1;
    bool grid_flag = false;
    int nxyz[3];
    bool visual_grid_flag = false;
    bool shade_back_flag = false;
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
        } else if (!strcmp(argv_glb[i],"-shade_back")) {
            shade_back_flag = true;
        } else if (!strcmp(argv_glb[i],"-visualize_grid_flag")) {
            visualize_grid_flag = true;
        } else if (!strcmp(argv_glb[i],"-shadows")) {
            shadow_flag = true;
        } else if (!strcmp(argv_glb[i],"-bounces")) {
            i++; assert (i < argc_glb); 
            bounces = atof(argv_glb[i]);
        } else if (!strcmp(argv_glb[i],"-weight")) {
            i++; assert (i < argc_glb); 
            weight = atof(argv_glb[i]);
        } else if (!strcmp(argv_glb[i],"--grid")) {
            grid_flag = true;
            i++; assert (i < argc_glb); 
            nxyz[0] = atof(argv_glb[i]);
            i++; assert (i < argc_glb); 
            nxyz[1] = atof(argv_glb[i]);
            i++; assert (i < argc_glb); 
            nxyz[2] = atof(argv_glb[i]);
        } else if (!strcmp(argv_glb[i],"-visualize_grid")) {
            visual_grid_flag = true;
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
        RayTracer rtracer = RayTracer(&scene, bounces, weight, shadow_flag, shade_back_flag, grid_flag, nxyz);
        GLCanvas glc = GLCanvas();
        glc.initialize(&scene, render, traceRayFunction, rtracer.getGrid(), visualize_grid_flag);
        // ;
    } else {
        render();
    }

    return 0;

}