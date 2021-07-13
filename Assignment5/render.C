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
#include "render.h"
#include "sphere.h"
#include "raytracer.h"
#include "rayTree.h"
int argc_glb;
char** argv_glb;
#define N_LARGE 10000
#define EPSILON 0.001
void traceRayFunction(float x, float y){
    // std::cout << "render:" << std::endl;
    char *input_file = NULL;

    bool shade_back_flag = false;
    gouraud_flag = false;
    bool shadow_flag = false;
    int bounces = 0;
    float weight = 0.1;
    bool grid_flag = false;
    int nxyz[3];
    bool visual_grid_flag = false;

    // sample command line:
    // raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

    for (int i = 1; i < argc_glb; i++) {
        if (!strcmp(argv_glb[i],"-input")) {
            i++; assert (i < argc_glb); 
            input_file = argv_glb[i];
        } else if (!strcmp(argv_glb[i],"-shade_back")) {
            shade_back_flag = true;
        } else if (!strcmp(argv_glb[i],"-tessellation")) {
            i++; assert (i < argc_glb);
            theta_steps = atof(argv_glb[i]);
            i++; assert (i < argc_glb);
            phi_steps = atof(argv_glb[i]);
        } else if (!strcmp(argv_glb[i],"-gouraud")) {
            gouraud_flag = true;
        } else if (!strcmp(argv_glb[i],"-shadows")) {
            shadow_flag = true;
        } else if (!strcmp(argv_glb[i],"-bounces")) {
            i++; assert (i < argc_glb); 
            bounces = atof(argv_glb[i]);
        } else if (!strcmp(argv_glb[i],"-weight")) {
            i++; assert (i < argc_glb); 
            weight = atof(argv_glb[i]);
        } else if (!strcmp(argv_glb[i],"-grid")) {
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

    std::cout << "render:0" << std::endl;
    SceneParser scene = SceneParser(input_file);
    RayTracer rtracer = RayTracer(&scene, bounces, weight, shadow_flag, shade_back_flag, grid_flag, nxyz);

    Camera* cam = scene.getCamera();
    std::cout << "render:1" << std::endl;
    Ray r = cam->generateRay(Vec2f(x, y));
    Hit h = Hit(N_LARGE, NULL, Vec3f());;
    float tmin = cam->getTMin();

    rtracer.traceRay(r, tmin, 0, 1.0, 1.0, h);
    if (h.getT() + EPSILON >= N_LARGE){
        RayTree::SetMainSegment(r, 0, 20);
    } else {
        RayTree::SetMainSegment(r, 0, h.getT());
    }
    

}
void render(){
    // std::cout << "render:" << std::endl;
    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    char *normal_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;
    bool shade_back_flag = false;
    gouraud_flag = false;
    bool shadow_flag = false;
    int bounces = 0;
    float weight = 0.1;
    bool grid_flag = false;
    int nxyz[3];
    bool visual_grid_flag = false;
    // sample command line:
    // raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

    for (int i = 1; i < argc_glb; i++) {
        if (!strcmp(argv_glb[i],"-input")) {
            i++; assert (i < argc_glb); 
            input_file = argv_glb[i];
        } else if (!strcmp(argv_glb[i],"-size")) {
            i++; assert (i < argc_glb); 
            width = atoi(argv_glb[i]);
            i++; assert (i < argc_glb); 
            height = atoi(argv_glb[i]);
        } else if (!strcmp(argv_glb[i],"-output")) {
            i++; assert (i < argc_glb); 
            output_file = argv_glb[i];
        } else if (!strcmp(argv_glb[i],"-normals")) {
            i++; assert (i < argc_glb); 
            normal_file = argv_glb[i];
        } else if (!strcmp(argv_glb[i],"-depth")) {
            i++; assert (i < argc_glb); 
            depth_min = atof(argv_glb[i]);
            i++; assert (i < argc_glb); 
            depth_max = atof(argv_glb[i]);
            i++; assert (i < argc_glb); 
            depth_file = argv_glb[i];
        } else if (!strcmp(argv_glb[i],"-shade_back")) {
            shade_back_flag = true;
        } else if (!strcmp(argv_glb[i],"-gui")) {
            ;
        } else if (!strcmp(argv_glb[i],"-tessellation")) {
            i++; assert (i < argc_glb);
            theta_steps = atof(argv_glb[i]);
            i++; assert (i < argc_glb);
            phi_steps = atof(argv_glb[i]);
        } else if (!strcmp(argv_glb[i],"-gouraud")) {
            gouraud_flag = true;
        } else if (!strcmp(argv_glb[i],"-shadows")) {
            shadow_flag = true;
        } else if (!strcmp(argv_glb[i],"-bounces")) {
            i++; assert (i < argc_glb); 
            bounces = atof(argv_glb[i]);
        } else if (!strcmp(argv_glb[i],"-weight")) {
            i++; assert (i < argc_glb); 
            weight = atof(argv_glb[i]);
        } else if (!strcmp(argv_glb[i],"-grid")) {
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
            printf ("whoops error with command line argument %d: '%s'\n",i,argv_glb[i]);
            assert(0);
        }
    }

    SceneParser scene = SceneParser(input_file);
    // std::cout << shadow_flag << std::endl;
    RayTracer rtracer = RayTracer(&scene, bounces, weight, shadow_flag, shade_back_flag, grid_flag, nxyz);

    Image img_output = Image(width, height);
    Image img_depth = Image(width, height);
    Image img_normal = Image(width, height);
    Camera* cam = scene.getCamera();
    int i, j, k, l;
    Vec2f point_screen;

    Ray r;
    Hit h;
    float tmin = cam->getTMin();
    float t;
    Vec3f raytrace_color;
    Vec3f white = Vec3f(1, 1, 1);

    for (i = 0; i < height; i++){
        for (j = 0; j < width; j++){
            h = Hit(N_LARGE, NULL, Vec3f());
            // point.Set((j+0.5)/width, 1 - (i + 0.5) / height);
            point_screen.Set((j+0.5)/width, (i + 0.5) / height);
            r = cam->generateRay(point_screen);
            raytrace_color = rtracer.traceRay(r, tmin, 0, 1.0, 1.0, h);
            img_output.SetPixel(j, i, raytrace_color);
            img_normal.SetPixel(j, i, h.getNormal());
            t = h.getT();
            if (t >= depth_min && t <= depth_max){
                img_depth.SetPixel(j, i, white *  ((depth_max - t) / (depth_max - depth_min)));
            }
        }
    }
    img_output.SaveTGA(output_file);
    if (depth_file != NULL){
        img_depth.SaveTGA(depth_file);
    }
    if (normal_file != NULL){
        img_normal.SaveTGA(normal_file);
    }
}