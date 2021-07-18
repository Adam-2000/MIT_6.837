#ifndef _GRID_H_
#define _GRID_H_

#include "object3d.h"
#include "vectors.h"
#include "ray.h"
#include "rayTree.h"
#include "object3dvector.h"
#define EPSILON_GRID 0.001
static PhongMaterial m_static = PhongMaterial(Vec3f(1,1,1), Vec3f(1,1,1), 100, Vec3f(0.1,0.1,0.1), Vec3f(0.1,0.1,0.1), 1.0);


class MarchingInfo {

public:
    MarchingInfo(){}
    ~MarchingInfo(){}

    void setTmin(float t){tmin = t;}
    void setIjk(int idx, int val){ijk[idx] = val;}
    void setIjk(Vec3f vec){ijk = vec;}
    void setTnext(int idx, float val){t_next[idx] = val;}
    void setTnext(Vec3f vec){t_next = vec;}
    void setDt(int idx, int val){d_t[idx] = val;}
    void setDt(Vec3f vec){d_t = vec;}
    void setSign(Vec3f vec){sign = vec;}
    void setNormal(Vec3f vec){normal = vec;}
    void setHitidx(int val){hit_idx = val;}
    float getTmin()const{return tmin;}
    int getIjk(int idx)const{return (int)ijk[idx];}
    Vec3f getIjk()const{return ijk;}
    float getTnext(int idx)const{return t_next[idx];}
    Vec3f getTnext()const{return t_next;}
    float getDt(int idx)const{return d_t[idx];}
    Vec3f getDt()const{return d_t;}
    Vec3f getSign()const{return sign;}
    Vec3f getNormal()const{return normal;}
    int getHitidx(){return hit_idx;}
    void nextCell(){
        // std::cout<<"nextcell:0"<<ijk<<std::endl;
        int next_idx = 0;
        float temp_t;
        temp_t = t_next[0];
        if((int)sign[0] == 0){
            if((int)sign[1] == 0){
                if((int)sign[2] == 0){
                    // std::cout<<"grid:nextCell: Why all 0?" << std::endl;
                }
                next_idx = 2;
                temp_t = t_next[2];
            } else {
                next_idx = 1;
                temp_t = t_next[1];
            }
        }
        
        if (t_next[1] < temp_t && (int)sign[1] != 0){
            temp_t = t_next[1];
            next_idx = 1;
        }
        if (t_next[2] < temp_t && (int)sign[2] != 0){
            temp_t = t_next[2];
            next_idx = 2;
        }
        hit_idx = (sign[next_idx] > 0) ? next_idx : next_idx + 3;
        ijk[next_idx] += sign[next_idx];
        t_next[next_idx] += d_t[next_idx];
        normal = Vec3f(0, 0, 0);
        normal[next_idx] = -sign[next_idx];
        // std::cout<<"nextcell:0.5"<<hit_idx<<std::endl;
        // std::cout<<"nextcell:1"<<ijk<<std::endl;
    }

private:
    float tmin;
    int hit_idx;
    Vec3f ijk;
    Vec3f t_next;
    Vec3f d_t;
    Vec3f sign;
    Vec3f normal;
};

class Grid: public Object3D{

public:
    Grid(BoundingBox *bb, int nx, int ny, int nz): nx(nx), ny(ny), nz(nz) {
        this->bbox = bb;
        occ_array = new Object3DVector[nx*ny*nz];
        // for (int i = 0; i < nx*ny*nz; i++){
        //     occ_array[i] = NULL;
        // }
        Vec3f vec_min = bbox->getMin();
        Vec3f vec_max = bbox->getMax();
        float dx = (vec_max.x() - vec_min.x()) / nx;
        float dy = (vec_max.y() - vec_min.y()) / ny;
        float dz = (vec_max.z() - vec_min.z()) / nz;
        dxyz = Vec3f(dx, dy, dz);
        // m = new PhongMaterial(Vec3f(1,1,1), Vec3f(1,1,1), 100, Vec3f(0.1,0.1,0.1), Vec3f(0.1,0.1,0.1), 1.0);
        m = &m_static;
        visualize_grid_flag = true;
        // std::cout<<"grid constructor:" << nx << ny << nz << bb->getMin() << bb->getMax() <<std::endl;
    }
    ~Grid(){delete [] occ_array;}

    // void set_array(Object3D* val, int x, int y, int z){
    //     occ_array[x * ny * nz + y * nz + z] = val;
    // }
    void add_object(Object3D* val, int x, int y, int z){
        occ_array[x * ny * nz + y * nz + z].addObject(val);
    }
    Object3DVector* get_array(int x, int y, int z){
        return &occ_array[x * ny * nz + y * nz + z];
    }
    void ChangeColor(int n){
        Vec3f color_gradient = Vec3f(1,1,1);
        if (n > 1){
            color_gradient[((n-2)/3)%3] -= ((n-2) % 3 + 1) * (1.0 / 3.0);
        }
        m->setDiffuseColor(color_gradient);
    };
    void get_n(int &x, int &y, int &z){
        x = nx;
        y = ny;
        z = nz;
    }
    Vec3f getDxyz(){return dxyz;}
    void setMaterial(Material* m){
        this->m = m;
    }
    bool intersect(const Ray &r, Hit &h, float tmin){
        // std::cout << "grid::intersect::0.0" << std::endl;
        MarchingInfo mi;
        initializeRayMarch(mi, r, tmin);
        if (mi.getHitidx() == -1){
            return false;
            // std::cout << "grid::intersect::return:1" << std::endl;
        }
        
        Vec3f ijk = mi.getIjk();
        // std::cout << "grid::intersect::mi.getHitidx()"<<mi.getHitidx()<<ijk << std::endl;
        Vec3f vec_min = bbox->getMin();

        bool flag;
        int cnt = 0;
        Vec3f normal;
        Vec3f p_grid;
        Vec3f vec1, vec2, vec3, vec4, vec5, vec6, vec7;
        Vec3f _vec1 = Vec3f(0, 0, dxyz.z());
        Vec3f _vec2 = Vec3f(0, dxyz.y(), 0);
        Vec3f _vec3 = Vec3f(0, dxyz.y(), dxyz.z());
        Vec3f _vec4 = Vec3f(dxyz.x(), 0, 0);
        Vec3f _vec5 = Vec3f(dxyz.x(), 0, dxyz.z());
        Vec3f _vec6 = Vec3f(dxyz.x(), dxyz.y(), 0);
        Vec3f _vec7 = Vec3f(dxyz.x(), dxyz.y(), dxyz.z());
        Vec3f tnext_xyz;
        float temp_t;
        flag = false;
        while(ijk.x() >= 0 && ijk.x() < nx && ijk.y() >= 0 && ijk.y() < ny && ijk.z() < nz && ijk.z() >= 0){
            // std::cout <<cnt<< "   grid::ijk:  " <<ijk<< std::endl;
            p_grid = vec_min + dxyz * ijk;
            vec1 = p_grid + _vec1;
            vec2 = p_grid + _vec2;
            vec3 = p_grid + _vec3;
            vec4 = p_grid + _vec4;
            vec5 = p_grid + _vec5;
            vec6 = p_grid + _vec6;
            vec7 = p_grid + _vec7;
            RayTree::AddHitCellFace(p_grid, vec1, vec3, vec2, Vec3f(-1, 0, 0), m);
            RayTree::AddHitCellFace(p_grid, vec1, vec5, vec4, Vec3f(0, -1, 0), m);
            RayTree::AddHitCellFace(p_grid, vec2, vec6, vec4, Vec3f(0, 0, 1), m);
            RayTree::AddHitCellFace(vec7, vec6, vec4, vec5, Vec3f(1, 0, 0), m);
            RayTree::AddHitCellFace(vec7, vec6, vec2, vec3, Vec3f(0, 1, 0), m);
            RayTree::AddHitCellFace(vec7, vec5, vec1, vec3, Vec3f(0, 0, -1), m);
            // std::cout<<"grid::intersect:res"<<mi.getHitidx()<<std::endl;
            switch (mi.getHitidx()){
                case 6:
                    goto LOOP_TAIL;
                    break;
                case 0: 
                    normal.Set(-1, 0, 0);
                    RayTree::AddEnteredFace(p_grid, vec1, vec3, vec2, Vec3f(-1, 0, 0), m);
                    break;
                case 1: 
                    normal.Set(0, -1, 0);
                    RayTree::AddEnteredFace(p_grid, vec1, vec5, vec4, Vec3f(0, -1, 0), m);
                    break;
                case 2: 
                    normal.Set(0, 0, -1);
                    RayTree::AddEnteredFace(p_grid, vec2, vec6, vec4, Vec3f(0, 0, 1), m);
                    break;
                case 3: 
                    normal.Set(1, 0, 0);
                    RayTree::AddEnteredFace(vec7, vec6, vec4, vec5, Vec3f(1, 0, 0), m);
                    break;
                case 4: 
                    normal.Set(0, 1, 0);
                    RayTree::AddEnteredFace(vec7, vec6, vec2, vec3, Vec3f(0, 1, 0), m);
                    break;
                case 5: 
                    normal.Set(0, 0, 1);
                    RayTree::AddEnteredFace(vec7, vec5, vec1, vec3, Vec3f(0, 0, -1), m);
                    break;
                default : assert(0);
            }
            // std::cout<<"grid::intersect:ijk"<<ijk<<std::endl;
            // if(visualize_grid_flag){
            // std::cout<<"grid::intersect:visualize_grid_flag:s"<<visualize_grid_flag<<std::endl;
            if(visualize_grid_flag){
                if(get_array(ijk.x(), ijk.y(), ijk.z())->getNumObjects() > 0){
                    ChangeColor(get_array(ijk.x(), ijk.y(), ijk.z())->getNumObjects());
                    h.set(tmin, m, normal, r);
                    // std::cout << "grid::intersect::return:2" << std::endl;
                    return true;
                }
            } else{
                
                for (int i = 0; i < get_array(ijk.x(), ijk.y(), ijk.z())->getNumObjects(); i++){
                    flag |= get_array(ijk.x(), ijk.y(), ijk.z())->getObject(i)->intersect(r, h, tmin);
                }
                tnext_xyz = mi.getTnext();
                temp_t = min(tnext_xyz[0], tnext_xyz[1]);
                temp_t = min(tnext_xyz[2], temp_t);
                if(flag){
                    // if(cnt < 5){
                    //     std::cout<<"grid::intersect::2"<< h.getIntersectionPoint() << " " << h.getT()<<std::endl;
                    //     std::cout<<"grid::intersect::2.1"<< ijk << " " << p_grid << vec7 << temp_t<<std::endl;
                    // }
                    
                    if(temp_t + EPSILON_GRID > h.getT())
                    return true;
                    
                }
            }
            
            
            
            LOOP_TAIL:
            mi.nextCell();
            ijk = mi.getIjk();
            cnt++;
            // assert(cnt < 5);
        }
        // std::cout << "grid::intersect::return:4" << std::endl;
        return false;



    }
    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const{
        // std::cout<<"initializeRayMarch::nxyz"<<nx<<ny<<nz<<""<<tmin<<std::endl;
        Vec3f vec_min = bbox->getMin();
        Vec3f vec_max = bbox->getMax();
        // std::cout<<"initializeRayMarch::vec_max"<<vec_max<<std::endl;
        // std::cout<<"initializeRayMarch::vec_min"<<vec_min<<std::endl;
        Vec3f txyz_min;
        Vec3f dir = r.getDirection();
        // std::cout<<"initializeRayMarch::dir"<<dir<<std::endl;
        Vec3f orig = r.getOrigin();
        // std::cout<<"initializeRayMarch::orig"<<orig<<std::endl;
        Vec3f sign;
        Vec3f nxyz = Vec3f(nx, ny, nz);
        int hit_idx;
        // bool hit_xyz[3] = {0, 0, 0};
        Vec3f ijk;
        // divide zero case ignored
        for(int i = 0; i < 3; i++){
            if (dir[i] > 0){
                sign[i] = 1;
            } else if (dir[i] == 0){
                sign[i] = 0;
                // sign[i] = -1;
            } else if (dir[i] < 0){
                sign[i] = -1;
            }
        }
        
        mi.setSign(sign);
        mi.setDt(sign * dxyz / dir);
        Vec3f norm_vec = Vec3f(0, 0, 0);
        mi.setNormal(Vec3f(0, 0, 0));
        Vec3f txyz_temp = (vec_min - orig) / dir;
        Vec3f txyz_max = (vec_max - orig) / dir;
        Vec3f::Min(txyz_min, txyz_temp, txyz_max);
        Vec3f::Max(txyz_max, txyz_temp, txyz_max);
        Vec3f int_p;
        float t_min = txyz_min.x(); hit_idx = 0;
        if (t_min < txyz_min.y()){
            t_min = txyz_min.y();
            hit_idx = 1;
        }
        if (t_min < txyz_min.z()){
            t_min = txyz_min.z(); 
            hit_idx = 2;
        }
        float t_max = min(txyz_max.x(), txyz_max.y());
        t_max = min(txyz_max.z(), t_max);
        // std::cout<<"initializeRayMarch::tminmax"<<tmin << "  " <<t_min<< "  " <<t_max<< "  " <<std::endl;
        // std::cout<<"initializeRayMarch::sign"<<sign<<std::endl;
        // std::cout<<"initializeRayMarch::dt"<<mi.getDt()<<std::endl;
        
        // std::cout<<"initializeRayMarch::int_p"<<int_p<<std::endl;
        mi.setHitidx(-1);
        if (t_max < tmin || t_max < t_min){
            mi.setTmin(t_max);
            return;
        } else if (t_min >= tmin){
            int_p = r.pointAtParameter(t_min);
            // std::cout<<"initializeRayMarch::int_p"<<int_p<<std::endl;
            
            int_p -= vec_min;
            int_p = int_p / dxyz;
            for(int i = 0; i < 3; i++){
                if(int_p[i] < 0) int_p[i] = 0;
                if(int_p[i] >= nxyz[i]) int_p[i] = nxyz[i] - 1;
            }
            mi.setTmin(t_min);
            for (int i = 0; i < 3; i++){
                if (int_p[i] >= 0){
                    ijk[i] = (int)floor(int_p[i]);
                    if (sign[i] > 0){
                        mi.setTnext(i, t_min + (ceil(int_p[i]) - int_p[i]) * dxyz[i] / dir[i]);
                    } else {
                        mi.setTnext(i, t_min + (floor(int_p[i]) - int_p[i]) * dxyz[i] / dir[i]);
                    }
                }
                if (hit_idx == i){
                    mi.setHitidx((sign[i] > 0) ? i : i + 3);
                    ijk[i] = (sign[i] > 0) ? 0 : nxyz[i] - 1;
                    mi.setTnext(i, t_min + mi.getDt(i));
                    norm_vec[i] = -sign[i];
                    mi.setNormal(norm_vec);
                }
            }
        } else {
            int_p = r.pointAtParameter(tmin);
            // std::cout<<"initializeRayMarch::int_p"<<int_p<<std::endl;
            
            int_p -= vec_min;
            int_p = int_p / dxyz;
            for(int i = 0; i < 3; i++){
                if(int_p[i] < 0) int_p[i] = 0;
                if(int_p[i] >= nxyz[i]) int_p[i] = nxyz[i] - 1;
            }
            mi.setHitidx(6);
            mi.setTmin(tmin);
            for (int i = 0; i < 3; i++){
                ijk[i] = (int)floor(int_p[i]);
                if (sign[i] > 0){
                    mi.setTnext(i, tmin + (ceil(int_p[i]) - int_p[i]) * dxyz[i] / dir[i]);
                } else {
                    mi.setTnext(i, tmin + (floor(int_p[i]) - int_p[i]) * dxyz[i] / dir[i]);
                }
            }
        }
        mi.setIjk(ijk);
        // std::cout<<"initializeRayMarch::Hitidx"<<mi.getHitidx()<<std::endl;
        // std::cout<<"initializeRayMarch::ijk"<<ijk<<std::endl;
        // std::cout<<"initializeRayMarch::norm_vec"<<norm_vec<<std::endl;
    }
    void paint();
    void setVisualizeGridFlag(bool val){visualize_grid_flag = val;};
    bool getVisualizeGridFlag() const {return visualize_grid_flag;};
private:

    int nx; 
    int ny; 
    int nz;
    Vec3f dxyz;
    Object3DVector* occ_array;
    bool visualize_grid_flag;

};




#endif
