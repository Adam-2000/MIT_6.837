#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object3d.h"
#include "vectors.h"
#include <math.h>

class Sphere: public Object3D{

public:
    Sphere(Vec3f center, float radius, Material* m): center(center), radius(radius){
        this->m = m;
    }
    ~Sphere(){}

    virtual bool intersect(const Ray &r, Hit &h, float tmin){
        Vec3f Ro = center - r.pointAtParameter(tmin);
        float Ro2 = Ro.Dot3(Ro);
        float r2 = radius * radius;
        bool inside = false;
        if (Ro2 <= r2) inside = true;
        float tp = Ro.Dot3(r.getDirection());
        if (!inside && tp < 0){
            return false;
        }
        float d2 = Ro2 - tp * tp;
        if (d2 > r2){
            return false;
        }
        float t_temp;
        if (inside){
            t_temp = tmin + tp + sqrt(r2 - d2);
            if (t_temp < h.getT()){
                h.set(t_temp, m, r);
                return true;
            }
        } else {
            t_temp = tmin + tp - sqrt(r2 - d2);
            if (t_temp < h.getT()){
                h.set(t_temp, m, r);
                return true;
            }     
        }
        return false;
    }

private:

    Vec3f center;
    float radius;

};

#endif /*_SPHERE_H_*/