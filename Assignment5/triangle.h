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
    void insertIntoGrid(Grid *g, Matrix *m){
        // std::cout <<"triangle::insertintogrid::0" <<std::endl;
        BoundingBox* bb = g->getBoundingBox();
        int nx, ny, nz;
        g->get_n(nx, ny, nz);
        
        Vec3f vec_min_grid = bb->getMin();
        Vec3f vec_max_grid = bb->getMax();
        Vec3f vec_min, vec_max;
        Vec3f _vec_min = this->bbox->getMin();
        Vec3f _vec_max = this->bbox->getMax();
        if(m != NULL){
            Vec3f _a = a, _b = b, _c = c;
            // std::cout <<"triangle::insertintogrid::abc::" << _a << _b << _c <<std::endl;
            m->TransformPoint(_a);
            m->TransformPoint(_b);
            m->TransformPoint(_c);
            Vec3f::Min(_vec_min, _a, _b);
            Vec3f::Min(_vec_min, _vec_min, _c);
            Vec3f::Max(_vec_max, _a, _b);
            Vec3f::Max(_vec_max, _vec_max, _c);
            // std::cout <<"triangle::insertintogrid::_abc::" << _a << _b << _c <<std::endl;
        }
        
        // std::cout <<"triangle::insertintogrid::_vec_min::" << _vec_min <<std::endl;
        // std::cout <<"triangle::insertintogrid::_vec_max::" << _vec_max <<std::endl;
        Vec3f::Max(vec_min, _vec_min, vec_min_grid);
        Vec3f::Min(vec_max, _vec_max, vec_max_grid);
        Vec3f dxyz = g->getDxyz();
        Vec3f nxyz = Vec3f(nx, ny, nz);
        vec_min = (vec_min - vec_min_grid) / dxyz;
        vec_max = (vec_max - vec_min_grid) / dxyz;
        for (int i = 0; i < 3; i++){
            vec_min[i] = floor(vec_min[i]);
            if(vec_min[i] >= nxyz[i]){
                vec_min[i] = nxyz[i] - 1;
            }
            vec_max[i] = floor(vec_max[i]);
            if(vec_max[i] >= nxyz[i]){
                vec_max[i] = nxyz[i] - 1;
            }
        }
        
        float dx = dxyz.x();
        float dy = dxyz.y();
        float dz = dxyz.z();
        // Vec3f grid_corner;
        // std::cout <<"triangle::insertintogrid::vec_min_grid::" << vec_min_grid <<std::endl;
        // std::cout <<"triangle::insertintogrid::vec_max_grid::" << vec_max_grid <<std::endl;
        // std::cout <<"triangle::insertintogrid::vec_min::" << vec_min <<std::endl;
        // std::cout <<"triangle::insertintogrid::vec_max::" << vec_max <<std::endl;
        for (int x = (int)vec_min.x(); x <= (int)vec_max.x(); x++){
            for (int y = (int)vec_min.y(); y <= (int)vec_max.y(); y++){
                for(int z = (int)vec_min.z(); z <= (int)vec_max.z(); z++){
                    // std::cout<<x<<y<<z<<g->get_array(x, y, z)<<std::endl;
                    // std::cout <<"triangle::insertintogrid::xyz::" << x << y << z <<std::endl;
                    g->add_object(this, x, y, z);
                    // std::cout<<grid_center << " " << dist <<std::endl;
                    // std::cout<<x<<y<<z<<g->get_array(x, y, z)<<std::endl;
                }
            }
        }
        // g->setMaterial(this->m);
    }
private:

    Vec3f a;
    Vec3f b;
    Vec3f c;
    Vec3f normal;

};

#endif /*_TRIANGLE_H_*/