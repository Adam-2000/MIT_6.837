#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object3d.h"
#include "vectors.h"

class Triangle: public Object3D{

public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material* m): a(a), b(b), c(c){
        this->m = m;
        Vec3f vec_min, vec_max;
        Vec3f::Min(vec_min, a, b);
        Vec3f::Min(vec_min, vec_min, c);
        Vec3f::Max(vec_max, a, b);
        Vec3f::Max(vec_max, vec_max, c);
        this->bbox = new BoundingBox(vec_min, vec_max);
        Vec3f::Cross3(normal, b - a, c - b);
        normal.Normalize();
    }
    ~Triangle(){ delete this->bbox; this->bbox = NULL;}

    bool intersect(const Ray &r, Hit &h, float tmin){
        Matrix mat, mat1, mat2, mat3;
        int i, j;
        float detA;
        Vec3f Ro = r.getOrigin();
        Vec3f Rd = r.getDirection(); 
        // Vec3f barycentric = a - r.getOrigin();
        float beta, gamma, t;
        for (i = 0; i < 3; i++){
            mat.Set(0, i, a[i] - b[i]);
            mat.Set(1, i, a[i] - c[i]);
            mat.Set(2, i, Rd[i]);
        }
        mat.Set(3, 3, 1); 
        for (i = 0; i < 3; i++){
            mat1.Set(0, i, a[i] - Ro[i]);
            mat1.Set(1, i, a[i] - c[i]);
            mat1.Set(2, i, Rd[i]);
        }
        mat1.Set(3, 3, 1);
        for (i = 0; i < 3; i++){
            mat2.Set(0, i, a[i] - b[i]);
            mat2.Set(1, i, a[i] - Ro[i]);
            mat2.Set(2, i, Rd[i]);
        }
        mat2.Set(3, 3, 1);
        for (i = 0; i < 3; i++){
            mat3.Set(0, i, a[i] - b[i]);
            mat3.Set(1, i, a[i] - c[i]);
            mat3.Set(2, i, a[i] - Ro[i]);
        }
        mat3.Set(3, 3, 1);
        detA = mat.get_det();
        beta = mat1.get_det() / detA;
        gamma = mat2.get_det() / detA;
        t = mat3.get_det() / detA;

        if (beta > 0 && gamma > 0 && beta + gamma < 1 
                    && t > tmin && t < h.getT()){

            h.set(t, m, normal, r);
            return true;
        }
        return false;
    }
    void paint(){
        this->m->glSetMaterial();
        glBegin(GL_TRIANGLES);
        glNormal3f(normal.x(), normal.y(), normal.z());
        glVertex3f(a.x(), a.y(), a.z());
        glVertex3f(b.x(), b.y(), b.z());
        glVertex3f(c.x(), c.y(), c.z());
        glEnd();
    }
private:

    Vec3f a;
    Vec3f b;
    Vec3f c;
    Vec3f normal;

};

#endif /*_TRIANGLE_H_*/