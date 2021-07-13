#include "raytracer.h"
#include "vectors.h"
#include "group.h"
#include "light.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "rayTree.h"

#define EPSILON 0.001
#define LONG_RAY 20
#define N_LARGE 10000
Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming){
    Vec3f reflect_out = incoming - 2 * incoming.Dot3(normal) * normal;
    reflect_out.Normalize();
    return reflect_out;
}

bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming, 
            float index_i, float index_t, Vec3f &transmitted){
    float yita = index_i / index_t;
    float crit = 1 - yita * yita * (1 - pow(normal.Dot3(incoming), 2));
    if (crit <= 0){
        return false;
    }
    transmitted = yita * incoming - (incoming.Dot3(normal) + sqrt(crit)) * normal;
    transmitted.Normalize();
    return true; 
}

Vec3f RayTracer::traceRay(Ray &r, float tmin, int bounces, float weight, 
                 float indexOfRefraction, Hit &h) const{
                
    Vec3f back_color = s->getBackgroundColor();
    Vec3f ambient_light_color = s->getAmbientLight();
    Group* g = s->getGroup();
    int n_lights = s->getNumLights();
    Material* m;
    Light* light_ptr;
    Vec3f ret_color = Vec3f(0, 0, 0);
    if (bounces > max_bounces){
        return ret_color;
    }
    if (weight < cutoff_weight){
        return ret_color;
    }
    Vec3f color_diffused;
    Vec3f color_diffused__;
    Vec3f color_specular;
    Vec3f color_light;
    Vec3f color_obj;
    Vec3f color_reflected, color_transmitted;
    Vec3f dir_light;
    Vec3f p_insct;
    Vec3f normal;
    Ray ray_shadow;
    Hit hit_shadow;
    Ray ray_reflect;
    Hit hit_reflect = Hit(N_LARGE, NULL, Vec3f());
    Ray ray_transmit;
    Hit hit_transmit = Hit(N_LARGE, NULL, Vec3f());
    Vec3f ReflectiveColor;
    Vec3f TransparentColor;
    Vec3f dir_transmitted;
    float cos;
    float dis2light = INFINITY;
    int k;
    bool inside = false;
    float index_out, index_in;
    // float tstop = LONG_RAY;
    if (g->intersect(r, h, tmin)){
        p_insct = h.getIntersectionPoint();
        m = h.getMaterial();
        color_obj = m->getDiffuseColor();
        color_diffused.Set(0, 0, 0);
        color_specular.Set(0, 0, 0);
        index_out = m->getIndexOfRefraction();
        // tstop = h.getT();
        // flag_lit = false;
        if (h.getNormal().Dot3(r.getDirection()) > 0){
            // flag_lit = true;
            h.set(h.getT(), m, -h.getNormal(), r);
            inside = true;
            index_out = 1.0;
        }
        normal = h.getNormal();
        for (k = 0; k < n_lights; k++){
            light_ptr = s->getLight(k);
            light_ptr->getIllumination(p_insct, dir_light, color_light, dis2light);
            if (shadows) {
                ray_shadow = Ray(p_insct, dir_light);
                hit_shadow = Hit(N_LARGE, NULL, Vec3f());
                // std::cout<<dir_light;
                if (g->intersect(ray_shadow, hit_shadow, EPSILON)){
                    // std::cout<<"1";
                    RayTree::AddShadowSegment(ray_shadow, 0, hit_shadow.getT());
                    // if (hit_shadow.getT() + EPSILON < dis2light){
                        // std::cout<<"1";
                        continue;
                    // }
                }
                RayTree::AddShadowSegment(ray_shadow, 0, LONG_RAY);
            }
            cos = dir_light.Dot3(normal);
            if (shade_back_flag && cos < 0){
                cos = - cos;
            }
            if(cos > 0){
                // flag_lit = true;
                Vec3f::Mult(color_diffused__, color_light, color_obj);
                color_diffused += color_diffused__ * cos;
                color_specular += m->Shade(r, h, dir_light, color_light);
            }
        }

        ret_color = color_diffused + ambient_light_color * color_obj + color_specular;
        ReflectiveColor = m->getReflectiveColor();
        if(ReflectiveColor.Length() > EPSILON){
            ray_reflect = Ray(p_insct, mirrorDirection(normal, r.getDirection()));
            color_reflected = ReflectiveColor * traceRay(ray_reflect, EPSILON, bounces + 1, 
                            ReflectiveColor.Length(), index_out, hit_reflect); 
            if (hit_reflect.getT() + EPSILON < N_LARGE){
                RayTree::AddReflectedSegment(ray_reflect, 0, hit_reflect.getT());
            } else {
                RayTree::AddReflectedSegment(ray_reflect, 0, LONG_RAY);
            }
        }
        TransparentColor = m->getTransparentColor();
        if(TransparentColor.Length() > EPSILON){
            if(transmittedDirection(normal, r.getDirection(), indexOfRefraction, index_out, dir_transmitted)){
                ray_transmit = Ray(p_insct, dir_transmitted);
                color_transmitted = TransparentColor * traceRay(ray_transmit, EPSILON, bounces + 1, 
                            TransparentColor.Length(), index_out, hit_transmit);
                if (hit_transmit.getT() + EPSILON < N_LARGE){
                    RayTree::AddTransmittedSegment(ray_transmit, 0, hit_transmit.getT());
                } else {
                    RayTree::AddTransmittedSegment(ray_transmit, 0, LONG_RAY);
                }
            }
        }
        ret_color += color_transmitted + color_reflected;
        normal.Set(fabs(normal.x()), fabs(normal.y()), fabs(normal.z()));
    } else {
        ret_color =  back_color;
        normal.Set(0, 0, 0);
    }
    h.set(h.getT(), h.getMaterial(), normal, r);
    // if (bounces == 0){
    //     RayTree::SetMainSegment(r, 0, tstop);
    // }
    
    return ret_color;
}


