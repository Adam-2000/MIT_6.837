#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "object3d.h"
#include "vectors.h"
#include "matrix.h"

class Transform: public Object3D{

public:
    Transform(Matrix &m, Object3D *o): mat(m), obj(o){
        inv_m = mat;
        inv_m.Inverse();
        inv_m.Transpose(inv_t_m);
        det = mat.get_det();
    }
    ~Transform(){}

    bool intersect(const Ray &r, Hit &h, float tmin){
        Vec3f orig = r.getOrigin();
        Vec3f dir = r.getDirection();
        inv_m.Transform(orig);
        inv_m.TransformDirection(dir);
        dir.Normalize();
        Ray r_tran = Ray(orig, dir);
        Vec3f new_normal;
        // Hit h_temp;
        // r is problemetic
        // std::cout<<"?"<<std::endl;
        // std::cout<<h.getT()<<std::endl;
        // std::cout<<h.getNormal()<<std::endl;
        // h_temp.set(h.getT() / std::pow(det, 1.0/3), h.getMaterial(), h.getNormal(), r_tran);
        // if (obj->intersect(r_tran, h, tmin)){
        h.set(h.getT() / std::pow(det, 1.0/3));
        if (obj->intersect(r_tran, h, tmin / std::pow(det, 1.0/3))){
            new_normal = h.getNormal();
            inv_t_m.TransformDirection(new_normal);
            new_normal.Normalize();
            h.set(h.getT() * std::pow(det, 1.0/3), h.getMaterial(), new_normal, r);
            // h.set(h.getT(), h.getMaterial(), new_normal, r);
            return true;
        }
        h.set(h.getT() * std::pow(det, 1.0/3));
        return false;
    }

private:

    Object3D* obj;
    Matrix mat;
    Matrix inv_m;
    Matrix inv_t_m;
    float det;

};

#endif /*_TRANSFORM_H_*/