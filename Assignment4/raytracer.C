#include "raytracer.h"
#include "vectors.h"
#include "group.h"
#include "light.h"
#include "ray.h"
#include "hit.h"

#define EPSILON 0.01
// #define N_LARGE 10000
Vec3f RayTracer::traceRay(Ray &r, float tmin, int bounces, float weight, 
                 float indexOfRefraction, Hit &h) const{
                
    Vec3f back_color = s->getBackgroundColor();
    Vec3f ambient_light_color = s->getAmbientLight();
    Group* g = s->getGroup();
    int n_lights = s->getNumLights();
    Light* light_ptr;
    Vec3f ret_color = Vec3f(0, 0, 0);
    Vec3f color_diffused;
    Vec3f color_diffused__;
    Vec3f color_specular;
    Vec3f color_light;
    Vec3f color_obj;
    Vec3f dir_light;
    Vec3f p_insct;
    Vec3f normal;
    Ray ray_shadow;
    Hit hit_shadow;
    float cos;
    float dis2light = INFINITY;
    int k;
    if (g->intersect(r, h, tmin)){
        p_insct = h.getIntersectionPoint();
        color_obj = h.getMaterial()->getDiffuseColor();
        color_diffused.Set(0, 0, 0);
        color_specular.Set(0, 0, 0);
        // flag_lit = false;
        if (h.getNormal().Dot3(r.getDirection()) > 0){
            // flag_lit = true;
            h.set(h.getT(), h.getMaterial(), -h.getNormal(), r);
        }
        normal = h.getNormal();
        for (k = 0; k < n_lights; k++){
            light_ptr = s->getLight(k);
            light_ptr->getIllumination(p_insct, dir_light, color_light, dis2light);
            if (shadows) {
                ray_shadow = Ray(p_insct, dir_light);
                if (g->intersect(ray_shadow, hit_shadow, EPSILON)){
                    if (hit_shadow.getT() + EPSILON < dis2light){
                        continue;
                    }
                }
            }
            cos = dir_light.Dot3(normal);
            if (shade_back_flag && cos < 0){
                cos = - cos;
            }
            if(cos > 0){
                // flag_lit = true;
                Vec3f::Mult(color_diffused__, color_light, color_obj);
                color_diffused += color_diffused__ * cos;
                color_specular += h.getMaterial()->Shade(r, h, dir_light, color_light);
            }
        }
        // if (flag_lit){
        ret_color = color_diffused + ambient_light_color * color_obj + color_specular;
        // }
    
        normal.Set(fabs(normal.x()), fabs(normal.y()), fabs(normal.z()));
    } else {
        ret_color =  back_color;
        normal.Set(0, 0, 0);
    }
    h.set(h.getT(), h.getMaterial(), normal, r);
    return ret_color;
}


