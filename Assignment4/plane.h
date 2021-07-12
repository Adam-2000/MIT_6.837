#ifndef _PLANE_H_
#define _PLANE_H_

#include "object3d.h"
#include "vectors.h"

#define NUM_BIG 1000
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
    virtual void paint(){
        this->m->glSetMaterial();
        Vec3f vec_t = Vec3f(1, 0, 0);
        if (normal.x() == 1){
            vec_t.Set(0, 1, 0);
        }
        Vec3f vec_b;
        Vec3f::Cross3(vec_b, normal, vec_t);
        vec_b.Normalize();
        Vec3f::Cross3(vec_t, normal, vec_b);
        Vec3f vec_1, vec_2, vec_3, vec_4;
        Vec3f vec_center = normal * d;
        vec_1 = vec_center + NUM_BIG * vec_b + NUM_BIG * vec_t;
        vec_2 = vec_center - NUM_BIG * vec_b + NUM_BIG * vec_t;
        vec_3 = vec_center + NUM_BIG * vec_b - NUM_BIG * vec_t;
        vec_4 = vec_center - NUM_BIG * vec_b - NUM_BIG * vec_t;
        glBegin(GL_QUADS);
        glNormal3f(normal.x(), normal.y(), normal.z());
        glVertex3f(vec_1.x(), vec_1.y(), vec_1.z());
        glVertex3f(vec_2.x(), vec_2.y(), vec_2.z());
        glVertex3f(vec_3.x(), vec_3.y(), vec_3.z());
        glVertex3f(vec_4.x(), vec_4.y(), vec_4.z());
        glEnd();
    }

private:

    Vec3f normal;
    float d;

};

#endif /*_PLANE_H_*/