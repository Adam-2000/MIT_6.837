#include "sphere.h"
#include "glCanvas.h"
int theta_steps;
int phi_steps;
bool gouraud_flag;
// #define A2R(degree) ((PI * degree) / 180)
void Sphere::paint(){
    // Vec3f vec0, vec1, vec2, vec3;
    // Vec3f normal;
    float x0, x1, x2, x3, y0, y1, z0, z1, z2, z3;
    // int iPhi_next, iTheta_next;
    float d_Phi = PI / phi_steps;
    float d_Theta = 2 * PI / theta_steps;
    float rPhi, rTheta, rPhi_next, rTheta_next;
    this->m->glSetMaterial();
    glBegin(GL_QUADS);
        rPhi_next = -PI / 2 + d_Phi;
        y1 = sin(rPhi_next);
        for (int iPhi = 1; iPhi < phi_steps - 1; iPhi++){
            // std::cout << iPhi;
            rPhi = rPhi_next;
            rPhi_next = rPhi + d_Phi;
            y0 = y1;
            y1 = sin(rPhi_next);
            x1 = cos(rPhi);
            x2 = cos(rPhi_next);
            z1 = 0;
            z2 = 0;
            rTheta_next = 0;
            for (int iTheta = 0; iTheta < theta_steps; iTheta ++) {
                rTheta = rTheta_next;
                rTheta_next = rTheta + d_Theta;
                // compute appropriate coordinates & normals
                x0 = x1;
                x1 = cos(rPhi) * cos (rTheta_next);
                x3 = x2;
                x2 = cos(rPhi_next) * cos (rTheta_next);
                z0 = z1;
                z1 = -cos(rPhi) * sin (rTheta_next);
                z3 = z2;
                z2 = -cos(rPhi_next) * sin (rTheta_next);
                if (gouraud_flag) {
                    // send gl vertex commands
                    glNormal3f(x0, y0, z0);
                    glVertex3f(x0 * radius + center.x(), y0 * radius + center.y(), z0 * radius + center.z());
                    glNormal3f(x1, y0, z1);
                    glVertex3f(x1 * radius + center.x(), y0 * radius + center.y(), z1 * radius + center.z());
                    glNormal3f(x2, y1, z2);
                    glVertex3f(x2 * radius + center.x(), y1 * radius + center.y(), z2 * radius + center.z());
                    glNormal3f(x3, y1, z3);
                    glVertex3f(x3 * radius + center.x(), y1 * radius + center.y(), z3 * radius + center.z());
                } else {
                    // send gl vertex commands
                    glNormal3f(cos(rPhi + d_Phi * 0.5) * cos (rTheta + d_Theta * 0.5), 
                                    sin(rPhi + d_Phi * 0.5), -cos(rPhi + d_Phi * 0.5) * sin (rTheta + d_Theta * 0.5));
                    glVertex3f(x0 * radius + center.x(), y0 * radius + center.y(), z0 * radius + center.z());
                    glVertex3f(x1 * radius + center.x(), y0 * radius + center.y(), z1 * radius + center.z());
                    glVertex3f(x2 * radius + center.x(), y1 * radius + center.y(), z2 * radius + center.z());
                    glVertex3f(x3 * radius + center.x(), y1 * radius + center.y(), z3 * radius + center.z());
                }
            }
        }
    glEnd();
    glBegin(GL_TRIANGLES);
        rPhi = -PI / 2 + d_Phi;
        y0 = -1;
        y1 = sin(rPhi);
        x0 = 0;
        z0 = 0;
        x2 = cos(rPhi);
        z2 = 0;
        rTheta_next = 0;
        for (int iTheta = 0; iTheta < theta_steps; iTheta ++) {
            rTheta = rTheta_next;
            rTheta_next = rTheta + d_Theta;
            // compute appropriate coordinates & normals
            x1 = x2;
            x2 = cos(rPhi) * cos (rTheta_next);
            z1 = z2;
            z2 = -cos(rPhi) * sin (rTheta_next);
            // send gl vertex commands
            if (!gouraud_flag) {
                glNormal3f(cos(-PI / 2 + d_Phi * 0.5) * cos (rTheta + d_Theta * 0.5), 
                                    sin(-PI / 2 + d_Phi * 0.5), -cos(-PI / 2 + d_Phi * 0.5) * sin (rTheta + d_Theta * 0.5));
                glVertex3f(center.x(), -radius + center.y(), center.z());
                glVertex3f(x1 * radius + center.x(), y1 * radius + center.y(), z1 * radius + center.z());
                glVertex3f(x2 * radius + center.x(), y1 * radius + center.y(), z2 * radius + center.z());
                glNormal3f(cos(PI / 2 - d_Phi * 0.5) * cos (rTheta + d_Theta * 0.5), 
                                    sin(PI / 2 - d_Phi * 0.5), -cos(PI / 2 - d_Phi * 0.5) * sin (rTheta + d_Theta * 0.5));
                glVertex3f(center.x(), radius + center.y(), center.z());
                glVertex3f(x1 * radius + center.x(), -y1 * radius + center.y(), z1 * radius + center.z());
                glVertex3f(x2 * radius + center.x(), -y1 * radius + center.y(), z2 * radius + center.z());
            } else {
                glNormal3f(0, -1, 0);
                glVertex3f(center.x(), -radius + center.y(), center.z());
                glNormal3f(x1, y1, z1);
                glVertex3f(x1 * radius + center.x(), y1 * radius + center.y(), z1 * radius + center.z());
                glNormal3f(x2, y1, z2);
                glVertex3f(x2 * radius + center.x(), y1 * radius + center.y(), z2 * radius + center.z());
                glNormal3f(0, 1, 0);
                glVertex3f(center.x(), radius + center.y(), center.z());
                glNormal3f(x1, -y1, z1);
                glVertex3f(x1 * radius + center.x(), -y1 * radius + center.y(), z1 * radius + center.z());
                glNormal3f(x2, -y1, z2);
                glVertex3f(x2 * radius + center.x(), -y1 * radius + center.y(), z2 * radius + center.z());
            }
        }
    glEnd();
}