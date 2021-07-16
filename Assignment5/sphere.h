#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object3d.h"
#include "vectors.h"
#include <cmath>
#include "grid.h"
#define PI 3.141592653589793238462 
extern int theta_steps;
extern int phi_steps;
extern bool gouraud_flag;
class Sphere: public Object3D{

public:
    Sphere(Vec3f &center, float radius, Material* m): center(center), radius(radius){
        this->m = m;
        Vec3f vec_min = center - Vec3f(radius, radius, radius);
        Vec3f vec_max = center + Vec3f(radius, radius, radius);

        this->bbox = new BoundingBox(vec_min, vec_max);
    }
    ~Sphere(){delete this->bbox; this->bbox = NULL;}

    bool intersect(const Ray &r, Hit &h, float tmin){
        Vec3f Ro = center - r.getOrigin();
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
        Vec3f n;
        if (inside){
            t_temp = tp + std::sqrt(r2 - d2);
            if (t_temp < h.getT() && t_temp >= tmin){
                // n = center - r.pointAtParameter(t_temp);
                n = r.pointAtParameter(t_temp) - center;
                n.Normalize();
                h.set(t_temp, m, n, r);
                return true;
            }
        } else {
            t_temp = tp - std::sqrt(r2 - d2);
            if (t_temp < h.getT() && t_temp >= tmin){
                n = r.pointAtParameter(t_temp) - center;
                n.Normalize();
                h.set(t_temp, m, n, r);
                return true;
            }     
        }
        return false;
    }
    void insertIntoGrid(Grid *g, Matrix *m){
        BoundingBox* bb = g->getBoundingBox();
        int nx, ny, nz;
        Vec3f vec_min = bb->getMin();
        Vec3f vec_max = bb->getMax();
        Vec3f dxyz = g->getDxyz();
        g->get_n(nx, ny, nz);
        float dx = dxyz.x();
        float dy = dxyz.y();
        float dz = dxyz.z();
        float big_radius = Vec3f(dx/2, dy/2, dz/2).Length() + radius;
        float small_radius = -Vec3f(dx/2, dy/2, dz/2).Length() + radius;
        float dist;
        Vec3f grid_center;
        for (int x = 0; x < nx; x++){
            for (int y = 0; y < ny; y++){
                for(int z = 0; z < nz; z++){
                    grid_center = vec_min + Vec3f((x + 0.5) * dx, (y + 0.5) * dy, (z + 0.5) * dz);
                    dist = (grid_center - center).Length();
                    // std::cout<<x<<y<<z<<g->get_array(x, y, z)<<std::endl;
                    if(dist <= big_radius && dist >= small_radius){
                        g->add_object(this, x, y, z);
                    }
                    // std::cout<<grid_center << " " << dist <<std::endl;
                    // std::cout<<x<<y<<z<<g->get_array(x, y, z)<<std::endl;
                }
            }
        }
        // g->setMaterial(this->m);
    }

    void paint();
private:

    Vec3f center;
    float radius;

};

#endif /*_SPHERE_H_*/
