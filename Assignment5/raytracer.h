#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "scene_parser.h"
#include "ray.h"
#include "hit.h"
#include "grid.h"
#include "group.h"
#include "vectors.h"

class RayTracer {

public:

    RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows, bool shade_back_flag, bool grid_flag, int* nxyz, bool visualize_grid_flag):
                s(s), max_bounces(max_bounces), cutoff_weight(cutoff_weight), shadows(shadows), shade_back_flag(shade_back_flag){
        this->visualize_grid_flag = false;
        if (grid_flag){
            g = new Grid(s->getGroup()->getBoundingBox(), nxyz[0], nxyz[1], nxyz[2]);
            s->getGroup()->insertIntoGrid(g, NULL);
            this->visualize_grid_flag = visualize_grid_flag;
            g->setVisualizeGridFlag(visualize_grid_flag);
        }
        // mi = new MarchingInfo;

    };
    ~RayTracer(){
        if (g != NULL){
            delete g;
            g = NULL;
        }
        // delete mi;
    }

    Grid* getGrid(){
        return g;
    }
    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, 
                 float indexOfRefraction, Hit &hit) const;
private:
    SceneParser *s;
    int max_bounces; 
    float cutoff_weight; 
    bool shadows;
    bool shade_back_flag;
    bool visualize_grid_flag;
    Grid* g;
    // MarchingInfo* mi;
};

#endif 
