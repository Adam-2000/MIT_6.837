#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "scene_parser.h"
#include "image.h"
#include "camera.h"
#include "hit.h"
#include "group.h"

#define N_LARGE 10000

int main(int argc, char** argv){

    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;

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
        } else if (!strcmp(argv[i],"-depth")) {
            i++; assert (i < argc); 
            depth_min = atof(argv[i]);
            i++; assert (i < argc); 
            depth_max = atof(argv[i]);
            i++; assert (i < argc); 
            depth_file = argv[i];
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }

    SceneParser scene = SceneParser(input_file);
    Image img_output = Image(width, height);
    Image img_depth = Image(width, height);
    Camera* cam = scene.getCamera();
    int i, j;
    Vec2f point;
    Vec3f back_color = scene.getBackgroundColor();
    Group* g = scene.getGroup();
    Ray r;
    Hit h;
    float tmin = cam->getTMin();
    float t;
    Vec3f white = Vec3f(1, 1, 1);
    for (i = 0; i < height; i++){
        for (j = 0; j < width; j++){
            h = Hit(N_LARGE, NULL);
            // point.Set((j+0.5)/width, 1 - (i + 0.5) / height);
            point.Set((j+0.5)/width, (i + 0.5) / height);
            r = cam->generateRay(point);
            if (g->intersect(r, h, tmin)){
                img_output.SetPixel(j, i, h.getMaterial()->getDiffuseColor());
                t = h.getT();
                if (t >= depth_min && t <= depth_max){
                    img_depth.SetPixel(j, i, white *  ((depth_max - t) / (depth_max - depth_min)));
                }
            } else {
                img_output.SetPixel(j, i, back_color);
            }
        }
    }
    img_output.SaveTGA(output_file);
    img_depth.SaveTGA(depth_file);

    return 0;

}