#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "scene_parser.h"
#include "ray.h"
#include "hit.h"

class RayTracer {

public:

    RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows):
                s(s), max_bounces(max_bounces), cutoff_weight(cutoff_weight), shadows(shadows){};
    ~RayTracer(){}
    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, 
                 float indexOfRefraction, Hit &hit) const;
private:
    SceneParser *s;
    int max_bounces; 
    float cutoff_weight; 
    bool shadows;
};




#endif 
