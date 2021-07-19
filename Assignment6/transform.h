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
        this->bbox = NULL;
        del_flag = true;
        if (obj->getBoundingBox() != NULL){
            Vec3f vec_min = obj->getBoundingBox()->getMin();
            Vec3f vec_max = obj->getBoundingBox()->getMax();
            Vec3f vec_min_new = vec_min;
            Vec3f vec_max_new = vec_max;
            mat.Transform(vec_min_new);
            mat.Transform(vec_max_new);
            Vec3f vec_new;
            for (int i = 0; i < 8; i++){
                vec_new = Vec3f(i & 4 ? vec_max.x() : vec_min.x(), 
                                i & 2 ? vec_max.y() : vec_min.y(),
                                i & 1 ? vec_max.z() : vec_min.z());
                mat.Transform(vec_new);
                Vec3f::Min(vec_min_new, vec_min_new, vec_new);
                Vec3f::Max(vec_max_new, vec_max_new, vec_new);
            }
            this->bbox = new BoundingBox(vec_min_new, vec_max_new);
            
        } else {
            bbox = NULL;
        }
        
    }
    ~Transform(){
        if(this->bbox != NULL) {
            delete this->bbox; 
            this->bbox = NULL;
        }
        if(del_flag){
            delete obj;
        }
    }

    void clearDelflag(){del_flag = false;}

    bool intersect(const Ray &r, Hit &h, float tmin){
        Vec3f orig = r.getOrigin();
        Vec3f dir = r.getDirection();
        inv_m.Transform(orig);
        inv_m.TransformDirection(dir);
        float ratio = dir.Length();
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
        h.set(h.getT() * ratio);
        if (obj->intersect(r_tran, h, tmin * ratio)){
            new_normal = h.getNormal();
            inv_t_m.TransformDirection(new_normal);
            new_normal.Normalize();
            h.set(h.getT() / ratio, h.getMaterial(), new_normal, r);
            // h.set(h.getT(), h.getMaterial(), new_normal, r);
            return true;
        }
        h.set(h.getT() / ratio);
        return false;
    }
    void paint(){
        glPushMatrix();
        GLfloat *glMatrix = mat.glGet();
        glMultMatrixf(glMatrix);
        delete[] glMatrix;
        obj->paint();
        glPopMatrix();
    }
    void insertIntoGrid(Grid *g, Matrix *m){
        // std::cout<<"Transform::insertIntoGrid:0"<<std::endl;
        if (m == NULL){
            m = new Matrix;
            *m = mat;
            // std::cout<<"Transform::insertIntoGrid::mnew::"<<*m<<std::endl;
            obj->insertIntoGrid(g, m);
            delete m;
            m = NULL;
        } else {
            // std::cout<<"Transform::insertIntoGrid::mold::"<<*m<<std::endl;
            *m = (*m) * mat;
            // std::cout<<"Transform::insertIntoGrid::mnew::"<<*m<<std::endl;
            obj->insertIntoGrid(g, m);
            *m = (*m) * inv_m;
            // std::cout<<"Transform::insertIntoGrid::mafter::"<<*m<<std::endl;
        }
        
    }
private:

    Object3D* obj;
    Matrix mat;
    Matrix inv_m;
    Matrix inv_t_m;
    float det;
    bool del_flag;

};

#endif /*_TRANSFORM_H_*/