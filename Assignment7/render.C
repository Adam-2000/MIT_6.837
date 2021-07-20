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
#include "raytracing_stats.h"
#include "film.h"
#include "filter.h"

int argc_glb;
char** argv_glb;
#define N_LARGE 10000
#define EPSILON 0.001
void traceRayFunction(float x, float y){
    // std::cout << "traceRayFunction:0: "<<x << " "<< y << std::endl;
    char *input_file = NULL;

    bool shade_back_flag = false;
    gouraud_flag = false;
    bool shadow_flag = false;
    int bounces = 0;
    float weight = 0.1;
    bool grid_flag = false;
    int nxyz[3];
    bool visualize_grid_flag = false;
    bool stats_flag = false;
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
            visualize_grid_flag = true;
        // } else if (!strcmp(argv_glb[i],"-stats")) {
            // stats_flag = true;
        } else {
            ;
        }
    }

    // std::cout << "render:0" << std::endl;
    SceneParser scene = SceneParser(input_file);
    // std::cout<<"tracerayfunc"<<std::endl;
    RayTracer rtracer = RayTracer(&scene, bounces, weight, shadow_flag, shade_back_flag, grid_flag, nxyz, visualize_grid_flag);

    Camera* cam = scene.getCamera();
    // std::cout << "render:1" << std::endl;
    Ray r = cam->generateRay(Vec2f(x, y));
    Hit h = Hit(N_LARGE, NULL, Vec3f());;
    float tmin = cam->getTMin();

    rtracer.traceRay(r, tmin, 0, 1.0, 1.0, h);
    // std::cout << "traceRayFunction:1" << std::endl;
    if (h.getT() + EPSILON >= N_LARGE){
        RayTree::SetMainSegment(r, 0, 20);
    } else {
        RayTree::SetMainSegment(r, 0, h.getT());
    }
    // std::cout << "traceRayFunction:end" << std::endl;

}
void render(){
    // std::cout << "render:" << std::endl;
    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    char *normal_file = NULL;
    int zoom_factor_samples = 0;
    char *samples_file = NULL;
    int zoom_factor_filter = 0;
    char *filter_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;
    bool shade_back_flag = false;
    gouraud_flag = false;
    bool shadow_flag = false;
    int bounces = 0;
    float weight = 0.1;
    bool grid_flag = false;
    int nxyz[3] = {0, 0, 0};
    bool visualize_grid_flag = false;
    bool stats_flag = false;
    Sampler* sampler = NULL; 
    int num_samples = 1;
    Filter* filter = NULL;
    float radius = 0;
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
        } else if (!strcmp(argv_glb[i],"-render_samples")) {
            i++; assert (i < argc_glb); 
            samples_file = argv_glb[i];
            i++; assert (i < argc_glb); 
            zoom_factor_samples = atof(argv_glb[i]);
        } else if (!strcmp(argv_glb[i],"-render_filter")) {
            i++; assert (i < argc_glb); 
            filter_file = argv_glb[i];
            i++; assert (i < argc_glb); 
            zoom_factor_filter = atof(argv_glb[i]);
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
            visualize_grid_flag = true;
        } else if (!strcmp(argv_glb[i],"-stats")) {
            stats_flag = true;
        } else if (!strcmp(argv_glb[i],"-random_samples")) {
            i++; assert (i < argc_glb); 
            num_samples = atof(argv_glb[i]);
            sampler = new RandomSampler(num_samples);
        } else if (!strcmp(argv_glb[i],"-uniform_samples")) {
            i++; assert (i < argc_glb); 
            num_samples = atof(argv_glb[i]);
            sampler = new UniformSampler(num_samples);
        } else if (!strcmp(argv_glb[i],"-jittered_samples")) {
            i++; assert (i < argc_glb); 
            num_samples = atof(argv_glb[i]);
            sampler = new JitteredSampler(num_samples);
        } else if (!strcmp(argv_glb[i],"-box_filter")) {
            i++; assert (i < argc_glb); 
            radius = atof(argv_glb[i]);
            filter = new BoxFilter(radius);   
        } else if (!strcmp(argv_glb[i],"-tent_filter")) {
            i++; assert (i < argc_glb); 
            radius = atof(argv_glb[i]);
            filter = new TentFilter(radius);
        } else if (!strcmp(argv_glb[i],"-gaussian_filter")) {
            i++; assert (i < argc_glb); 
            radius = atof(argv_glb[i]);
            filter = new GaussianFilter(radius);
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv_glb[i]);
            assert(0);
        }
    }
    
    SceneParser scene = SceneParser(input_file);
    RayTracingStats::Initialize(width, height, scene.getGroup()->getBoundingBox(), nxyz[0], nxyz[1], nxyz[2]);
    RayTracer rtracer = RayTracer(&scene, bounces, weight, shadow_flag, shade_back_flag, grid_flag, nxyz, visualize_grid_flag);

    Image img_output = Image(width, height);
    // Image img_depth = Image(width, height);
    // Image img_normal = Image(width, height);
    Camera* cam = scene.getCamera();
    int i, j, k, l;
    Vec2f point_corner;
    Vec2f point_screen;
    Vec2f sample_offset;
    Ray r;
    Hit h;
    float tmin = cam->getTMin();
    float t;
    Vec3f raytrace_color;
    Vec3f white = Vec3f(1, 1, 1);
    int bigger_size = max(height, width);
    int height_offset = (bigger_size - height) >> 1;
    int width_offset = (bigger_size - width) >> 1;
    int cnt = 0;
    Film fm(width, height, num_samples);
    if(sampler == NULL){
        sampler = new UniformSampler(1);
    }
    for (i = 0; i < height; i++){
        for (j = 0; j < width; j++){
            point_corner.Set(j + width_offset, i + height_offset);
            for(k = 0; k < num_samples; k++){
                sample_offset = sampler->getSamplePosition(k);
                Vec2f::Add(point_screen, point_corner, sample_offset);
                point_screen /= width;
                h = Hit(N_LARGE, NULL, Vec3f());
                r = cam->generateRay(point_screen);
                raytrace_color = rtracer.traceRay(r, tmin, 0, 1.0, 1.0, h);
                fm.setSample(j, i, k, sample_offset, raytrace_color);
            }
            if(!filter){
                img_output.SetPixel(j, i, raytrace_color);
            }
            
            // if(normal_file) img_normal.SetPixel(j, i, h.getNormal());
            // if(depth_file){
            //     t = h.getT();
            //     if (t >= depth_min && t <= depth_max){
            //         img_depth.SetPixel(j, i, white *  ((depth_max - t) / (depth_max - depth_min)));
            //     }
            // }
        }
    }
    if(filter){
        for (i = 0; i < height; i++){
            for (j = 0; j < width; j++){
                img_output.SetPixel(j, i, filter->getColor(j, i, &fm));
            }
        }
    }
    
    // std::cout<<"render:2"<<std::endl;
    if(stats_flag){
        RayTracingStats::PrintStatistics();
    }
    if(output_file!= NULL){
        img_output.SaveTGA(output_file);
    }
    if(samples_file != NULL){
        fm.renderSamples(samples_file, zoom_factor_samples);
    }
    if(filter_file != NULL){
        fm.renderFilter(filter_file, zoom_factor_filter, filter);
    }
    if(sampler != NULL){
        delete sampler;
        sampler = NULL;
    }
    if(filter != NULL){
        delete filter;
        filter = NULL;
    }
    // if (depth_file != NULL){
    //     img_depth.SaveTGA(depth_file);
    // }
    // if (normal_file != NULL){
    //     img_normal.SaveTGA(normal_file);
    // }
}