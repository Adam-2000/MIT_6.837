#include "grid.h"
#include "boundingbox.h"
#include "GL/gl.h"

static void setColor(int n){

    GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };

    Vec3f color_gradient = Vec3f(1,1,1);
    if (n > 1){
        color_gradient[((n-2)/3)%3] -= ((n-2) % 3 + 1) * (1.0 / 3.0);
    }

    GLfloat diffuse[4] = {
      color_gradient.r(),
      color_gradient.g(),
      color_gradient.b(),
      1.0};
    
    // NOTE: GL uses the Blinn Torrance version of Phong...      
    float glexponent = 100;
      
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, one);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
}

void Grid::paint(){

    // std::cout<<"grid paint"<<std::endl;

    glBegin(GL_QUADS);

    float coord_x, coord_y, coord_z;
    Vec3f vec_min = this->bbox->getMin();
    Vec3f vec_max = this->bbox->getMax();
    float dx = (vec_max.x() - vec_min.x())/nx;
    float dy = (vec_max.y() - vec_min.y())/ny;
    float dz = (vec_max.z() - vec_min.z())/nz;
    int n_obj;
    Vec3f color_gradient;
    // std::cout<<"grid:paint"<<std::endl;
    // std::cout<< Vec3f(dx,dy,dz) << std::endl;
    // std::cout<< Vec3f(nx,ny,nz) << std::endl;
    // std::cout<< vec_min<<vec_max<<std::endl;
    coord_x = vec_min.x();
    for(int x = 0; x < nx; x++, coord_x += dx){  
        coord_y = vec_min.y();
        for(int y = 0; y < ny; y++, coord_y += dy){
            coord_z = vec_min.z();
            for(int z = 0; z < nz; z++, coord_z += dz){
                // std::cout<< Vec3f(x,y,z)<<Vec3f(coord_x, coord_y, coord_z)<<get_array(x, y, z)<<std::endl;
                n_obj = get_array(x, y, z).getNumObjects();
                if (n_obj > 0){
                    // color_gradient = Vec3f(1,1,1);
                    // if (n_obj > 1){
                    //     color_gradient[(n_obj-2)/3] -= ((n_obj-2) % 3 + 1) * (1.0 / 3.0);
                    // }
                    
                    // GLfloat diffuse[4] = {color_gradient[0],color_gradient[1],color_gradient[2],1.0};
                    // glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
                    setColor(n_obj);
                    // std::cout<< color_gradient<<n_obj<<std::endl;
                    // get_array(x,y,z).getObject(0)->getMaterial()->glSetMaterial();
                    if (x == 0){
                        goto DRAW_X_1;
                    } else if(!get_array(x - 1, y, z).getNumObjects()){
                        // std::cout<<"grid:paint:1.111"<<std::endl;
                        DRAW_X_1:
                        glNormal3f(-1, 0, 0);
                        glVertex3f(coord_x, coord_y, coord_z);
                        glVertex3f(coord_x, coord_y + dy, coord_z);
                        glVertex3f(coord_x, coord_y + dy, coord_z + dz);
                        glVertex3f(coord_x, coord_y, coord_z + dz);
                    }
                    // std::cout<<"grid:paint:1.11"<<std::endl;
                    if(x == nx - 1){
                        goto DRAW_X_2;
                    } else if(!get_array(x + 1, y, z).getNumObjects()){
                        // std::cout<<"grid:paint:1.12"<<std::endl;
                        DRAW_X_2:
                        glNormal3f(1, 0, 0);
                        glVertex3f(coord_x + dx, coord_y, coord_z);
                        glVertex3f(coord_x + dx, coord_y + dy, coord_z);
                        glVertex3f(coord_x + dx, coord_y + dy, coord_z + dz);
                        glVertex3f(coord_x + dx, coord_y, coord_z + dz);
                    }
                    // std::cout<<"grid:paint:1.13"<<std::endl;
                    if (y == 0){
                        goto DRAW_Y_1;
                    } else if(!get_array(x, y - 1, z).getNumObjects()){
                        DRAW_Y_1:
                        glNormal3f(0, -1, 0);
                        glVertex3f(coord_x, coord_y, coord_z);
                        glVertex3f(coord_x + dx, coord_y, coord_z);
                        glVertex3f(coord_x + dx, coord_y, coord_z + dz);
                        glVertex3f(coord_x, coord_y, coord_z + dz);
                    }
                    if(y == ny - 1){
                        goto DRAW_Y_2;
                    } else if(!get_array(x, y + 1, z).getNumObjects()){
                        DRAW_Y_2:
                        glNormal3f(0, 1, 0);
                        glVertex3f(coord_x, coord_y + dy, coord_z);
                        glVertex3f(coord_x + dx, coord_y + dy, coord_z);
                        glVertex3f(coord_x + dx, coord_y + dy, coord_z + dz);
                        glVertex3f(coord_x, coord_y + dy, coord_z + dz);
                    }
                    if (z == 0){
                        goto DRAW_Z_1;
                    } else if(!get_array(x, y, z - 1).getNumObjects()){
                        DRAW_Z_1:
                        glNormal3f(0, 0, 1);
                        glVertex3f(coord_x, coord_y, coord_z);
                        glVertex3f(coord_x, coord_y + dy, coord_z);
                        glVertex3f(coord_x + dx, coord_y + dy, coord_z);
                        glVertex3f(coord_x + dx, coord_y, coord_z);
                    }
                    if(z == nz - 1){
                        goto DRAW_Z_2;
                    } else if(!get_array(x, y, z + 1).getNumObjects()){
                        DRAW_Z_2:
                        glNormal3f(0, 0, -1);
                        glVertex3f(coord_x, coord_y, coord_z + dz);
                        glVertex3f(coord_x, coord_y + dy, coord_z + dz);
                        glVertex3f(coord_x + dx, coord_y + dy, coord_z + dz);
                        glVertex3f(coord_x + dx, coord_y, coord_z + dz);
                    }
                    // std::cout<<"grid:paint:1.2"<<std::endl;
                }

            }
            
        }
    }
    glEnd();
    // std::cout<<"grid:paint:2"<<std::endl;
}
