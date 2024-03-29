#include "raytracer.h"
#include "vectors.h"
#include "group.h"
#include "light.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "rayTree.h"
#include "raytracing_stats.h"
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
    // std::cout<<"transmitted::"<<normal<<incoming<<index_i<<" "<<index_t<<std::endl;
    float yita = index_i / index_t;
    float crit = 1 - yita * yita * (1 - pow(normal.Dot3(incoming), 2));
    // std::cout<<crit<<index_t<<std::endl;
    if (crit <= 0){
        return false;
    }
    transmitted = yita * incoming - (incoming.Dot3(normal) * yita + sqrt(crit)) * normal;
    transmitted.Normalize();
    // std::cout<<"output"<<transmitted<<std::endl;
    return true; 
}

Vec3f RayTracer::traceRay(Ray &r, float tmin, int bounces, float weight, 
                 float indexOfRefraction, Hit &h) const{
    // std::cout << "traceRay:0.0" << std::endl;
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

    Vec3f color_light;
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
    float dis2light = INFINITY;
    int k;
    float index_out;
    bool int_flag = false;
    // std::cout << "traceRay:0.2:"<<visualize_grid_flag << grid_flag<< std::endl;
    Object3D* obj_boss;
    if(grid_flag){
        obj_boss = this->g;
    } else {
        obj_boss = g;
    }
    RayTracingStats::IncrementNumNonShadowRays();

    if (obj_boss->intersect(r, h, tmin)){
        
        p_insct = h.getIntersectionPoint();
        // std::cout<<p_insct<<std::endl;
        m = h.getMaterial();

        index_out = m->getIndexOfRefraction();
        // std::cout << r.getDirection() << std::endl;
        // std::cout << h.getNormal()<<indexOfRefraction << " " << index_out <<std::endl;
        if (h.getNormal().Dot3(r.getDirection()) > 0){
            if(shade_back_flag){
                h.set(h.getT(), m, -h.getNormal(), r);
            }
            index_out = 1.0;
        }
        normal = h.getNormal();
        // std::cout << normal << " " << index_out <<std::endl;
        for (k = 0; k < n_lights; k++){
            light_ptr = s->getLight(k);
            light_ptr->getIllumination(p_insct, dir_light, color_light, dis2light);
            if (shadows & (!visualize_grid_flag)) {
                ray_shadow = Ray(p_insct, dir_light);
                hit_shadow = Hit(N_LARGE, NULL, Vec3f());
                // std::cout<<dir_light;
                RayTracingStats::IncrementNumShadowRays();
                if (obj_boss->intersect(ray_shadow, hit_shadow, EPSILON)){
                    // std::cout<<"1";
                    RayTree::AddShadowSegment(ray_shadow, 0, hit_shadow.getT());
                    // if (hit_shadow.getT() + EPSILON < dis2light){
                        // std::cout<<"1";
                        continue;
                    // }
                }
                RayTree::AddShadowSegment(ray_shadow, 0, LONG_RAY);
            }

            ret_color += m->Shade(r, h, dir_light, color_light);
        }
        ret_color += m->Shade(r, h, Vec3f(0, 0, 0), ambient_light_color);
        // ret_color = color_diffused + ambient_light_color * color_obj + color_specular;
        ReflectiveColor = m->getReflectiveColor();
        if(ReflectiveColor.Length() > EPSILON & (!visualize_grid_flag)){
            ray_reflect = Ray(p_insct, mirrorDirection(normal, r.getDirection()));
            color_reflected = ReflectiveColor * traceRay(ray_reflect, EPSILON, bounces + 1, 
                            ReflectiveColor.Length() * weight, indexOfRefraction, hit_reflect); 
            if (hit_reflect.getT() + EPSILON < N_LARGE){
                RayTree::AddReflectedSegment(ray_reflect, 0, hit_reflect.getT());
            } else {
                RayTree::AddReflectedSegment(ray_reflect, 0, LONG_RAY);
            }
        }
        TransparentColor = m->getTransparentColor();
        if(TransparentColor.Length() > EPSILON & (!visualize_grid_flag)){
            if(transmittedDirection(normal, r.getDirection(), indexOfRefraction, index_out, dir_transmitted)){
                ray_transmit = Ray(p_insct, dir_transmitted);
                // std::cout<<"send transmit ray"<<std::endl;
                color_transmitted = TransparentColor * traceRay(ray_transmit, EPSILON, bounces + 1, 
                            TransparentColor.Length() * weight, index_out, hit_transmit);
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
    // std::cout << "traceRay:end" << ret_color<<std::endl;
    return ret_color;
}


