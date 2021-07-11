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
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    char *normal_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;
    bool shade_back_flag = false;
    bool gui_flag = false;
    gouraud_flag = false;
    // sample command line:
    // raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc); 
            input_file = argv[i];
        } else if (!strcmp(argv[i],"-size")) {
            i++; assert (i < argc); 
            width = atoi(argv[i]);
            i++; assert (i < argc); 
            height = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-output")) {
            i++; assert (i < argc); 
            output_file = argv[i];
        } else if (!strcmp(argv[i],"-normals")) {
            i++; assert (i < argc); 
            normal_file = argv[i];
        } else if (!strcmp(argv[i],"-depth")) {
            i++; assert (i < argc); 
            depth_min = atof(argv[i]);
            i++; assert (i < argc); 
            depth_max = atof(argv[i]);
            i++; assert (i < argc); 
            depth_file = argv[i];
        } else if (!strcmp(argv[i],"-shade_back")) {
            shade_back_flag = true;
        } else if (!strcmp(argv[i],"-gui")) {
            gui_flag = true;
        } else if (!strcmp(argv[i],"-tessellation")) {
            i++; assert (i < argc);
            theta_steps = atof(argv[i]);
            i++; assert (i < argc);
            phi_steps = atof(argv[i]);
        } else if (!strcmp(argv_glb[i],"-gouraud")) {
            gouraud_flag = true;
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }
    // SceneParser scene = SceneParser(input_file);
    if (gui_flag){
        SceneParser scene = SceneParser(input_file);
        glutInit(&argc, argv);
        // std::cout<<"main:0"<<std::endl;
        // std::cout<<"main:1"<<std::endl;
        GLCanvas glc = GLCanvas();
        // std::cout<<"main:2"<<std::endl;
        glc.initialize(&scene, render);
        // std::cout<<"main:3"<<std::endl;
    } else {
        render();
        // Image img_output = Image(width, height);
        // Image img_depth = Image(width, height);
        // Image img_normal = Image(width, height);
        // Camera* cam = scene.getCamera();
        // int i, j, k, l;
        // Vec2f point_screen;
        // Vec3f back_color = scene.getBackgroundColor();
        // Vec3f ambient_light_color = scene.getAmbientLight();
        // Group* g = scene.getGroup();
        // int n_lights = scene.getNumLights();
        // Light* light_ptr;
        // Ray r;
        // Hit h;
        // float tmin = cam->getTMin();
        // float t;
        // Vec3f white = Vec3f(1, 1, 1);
        // Vec3f color_diffused;
        // Vec3f color_diffused__;
        // Vec3f color_light;
        // Vec3f color_obj;
        // Vec3f dir_light;
        // Vec3f p_insct;
        // Vec3f normal;
        // float cos;
        // bool flag_lit;
        // float dis2light = INFINITY;

        // for (i = 0; i < height; i++){
        //     for (j = 0; j < width; j++){
        //         h = Hit(N_LARGE, NULL, Vec3f());
        //         // point.Set((j+0.5)/width, 1 - (i + 0.5) / height);
        //         point_screen.Set((j+0.5)/width, (i + 0.5) / height);
        //         r = cam->generateRay(point_screen);
        //         if (g->intersect(r, h, tmin)){
        //             p_insct = h.getIntersectionPoint();
        //             normal = h.getNormal();
        //             color_obj = h.getMaterial()->getDiffuseColor();
        //             color_diffused.Set(0, 0, 0);
        //             flag_lit = false;
        //             if (normal.Dot3(r.getDirection()) <= 0){
        //                 flag_lit = true;
        //             }
        //             for (k = 0; k < n_lights; k++){
        //                 light_ptr = scene.getLight(k);
        //                 light_ptr->getIllumination(p_insct, dir_light, color_light, dis2light);
        //                 cos = dir_light.Dot3(normal);
        //                 if (shade_back_flag && cos < 0){
        //                     cos = - cos;
        //                 }
        //                 if(cos > 0){
        //                     // flag_lit = true;
        //                     Vec3f::Mult(color_diffused__, color_light, color_obj);
        //                     color_diffused += color_diffused__ * cos;
        //                 }
        //             }
        //             if (flag_lit){
        //                 color_diffused += ambient_light_color * color_obj;
        //             }
        //             img_output.SetPixel(j, i, color_diffused);
        //             normal.Set(fabs(normal.x()), fabs(normal.y()), fabs(normal.z()));
        //             img_normal.SetPixel(j, i, normal);
        //             t = h.getT();
        //             if (t >= depth_min && t <= depth_max){
        //                 img_depth.SetPixel(j, i, white *  ((depth_max - t) / (depth_max - depth_min)));
        //             }
        //         } else {
        //             img_output.SetPixel(j, i, back_color);
        //         }
        //     }
        // }
        // img_output.SaveTGA(output_file);
        // if (depth_file != NULL){
        //     img_depth.SaveTGA(depth_file);
        // }
        // if (normal_file != NULL){
        //     img_normal.SaveTGA(normal_file);
        // }
    }

    return 0;

}