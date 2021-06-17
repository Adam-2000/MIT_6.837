#ifndef _PLANE_H_
#define _PLANE_H_

#include "object3d.h"
#include "vectors.h"

class Plane: public Object3D{

public:
    Plane(Vec3f &normal, float d, Material* m): normal(normal), d(d){
        this->m = m;
    }
    ~Plane(){}

    virtual bool intersect(const Ray &r, Hit &h, float tmin){
        float t_ = (d - normal.Dot3(r.getOrigin())) / normal.Dot3(r.getDirection());
        if (t_ >= tmin && t_ < h.getT()){
            // if (normal.Dot3(r.getDirection()) <= 0){
            //     h.set(t_, m, normal, r);
            // } else {
            //     h.set(t_, m, Vec3f(0, 0, 0) - normal, r);
            // }
            h.set(t_, m, normal, r);
            return true;
        }
        return false;
    }

private:

    Vec3f normal;
    float d;

};

#endif /*_PLANE_H_*/