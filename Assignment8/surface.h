#ifndef _SURFACE_H_
#define _SURFACE_H_

#include "spline.h"
#include "glCanvas.h"
#include "matrix.h"
#include "curve.h"
#include "triangle_mesh.h"

class Surface: public Spline {

public:
    Surface(){}
    ~Surface(){}

    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args){}
    virtual void OutputBezier(FILE *file){
        fprintf (file, "surface_of_revolution\n");
        fprintf (file, "bezier\n");
        fprintf (file, "num_vertices %d\n", n_vertices);
        for (int i = 0; i < n_vertices; i++) {
            fprintf (file, "%lf %lf %lf\n", vertices[i].x(), vertices[i].y(), vertices[i].z());
        }
    }
    virtual void OutputBSpline(FILE *file){
        fprintf (file, "surface_of_revolution\n");
        fprintf (file, "bspline\n");
        fprintf (file, "num_vertices %d\n", n_vertices);
        for (int i = 0; i < n_vertices; i++) {
            fprintf (file, "%lf %lf %lf\n", vertices[i].x(), vertices[i].y(), vertices[i].z());
        }
    }
    // FOR EDITING OPERATIONS
    virtual void moveControlPoint(int selectedPoint, float x, float y){
        set(selectedPoint, Vec3f(x, y, 0));
    }
    virtual void addControlPoint(int selectedPoint, float x, float y){}
    virtual void deleteControlPoint(int selectedPoint){}

    // FOR GENERATING TRIANGLES
    virtual TriangleMesh* OutputTriangles(ArgParser *args){}

protected:
    Curve* c;
    int label;
};

class SurfaceOfRevolution: public Surface {

public:

    SurfaceOfRevolution(Curve* c){
        n_vertices = c->getNumVertices();
        vertices = new Vec3f[n_vertices];
        for(int i = 0; i < n_vertices; i++){
            vertices[i] = c->getVertex(i);
        }
        label = c->getLabel();
        assert(n_vertices >= 4);
        delete c;
    }
    ~SurfaceOfRevolution(){}
    void Paint(ArgParser *args){
        glPointSize(5);
        glBegin(GL_POINTS);
            glColor3f(1,0,0);
            for(int i = 0; i < n_vertices; i++){
                glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
            }
        glEnd();
        glLineWidth(1);
        glBegin(GL_LINES);
            glColor3f(0,0,1);
            for(int i = 0; i < n_vertices - 1; i++){
                glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
                glVertex3f(vertices[i+1].x(), vertices[i+1].y(), vertices[i+1].z());
            }
        glEnd();
        glLineWidth(3);
        glBegin(GL_LINES);
            glColor3f(0,1,0);
            float dt = 1.0 / args->curve_tessellation;
            Vec3f temp;
            float t, t2, t3;
            if(label == 0){
                for(int i = 0; i < (n_vertices - 1) / 3; i++){
                    t = 0;
                    temp = vertices[3*i];
                    for(int j = 0; j < args->curve_tessellation; j++){
                        glVertex3f(temp.x(), temp.y(), temp.z());
                        t += dt;
                        temp = pow(1-t, 3)* vertices[3*i] + 3 * t * pow(1-t, 2) * vertices[3*i+1]
                                + 3 * t * t * (1 - t) * vertices[3*i+2] + t * t * t * vertices[3*i+3];
                        glVertex3f(temp.x(), temp.y(), temp.z());
                    }
                }
            } else {
                for(int i = 0; i < n_vertices - 3; i++){
                    t = 0;
                    temp = 1.0 / 6 * vertices[i] + 4.0 / 6 * vertices[i+1]
                                + 1.0 / 6 * vertices[i+2];
                    for(int j = 0; j < args->curve_tessellation; j++){
                        glVertex3f(temp.x(), temp.y(), temp.z());
                        t += dt;
                        t3 = pow(t, 3);
                        t2 = t * t;
                        temp = pow(1-t, 3) / 6 * vertices[i] + (3*t3 - 6*t2 + 4) / 6 * vertices[i+1]
                                + (-3*t3 + 3*t2 + 3*t + 1) / 6 * vertices[i+2] + t3 / 6 * vertices[i+3];
                        glVertex3f(temp.x(), temp.y(), temp.z());
                    }
                }
            }
            
        glEnd();
    }
    TriangleMesh* OutputTriangles(ArgParser *args){
        int n_lines; 
        float t, t2, t3, dt;
        dt = 1.0 / args->curve_tessellation;
        int idx;
        Vec3f* vert;
        if(label == 0){        
            n_lines = args->curve_tessellation * (n_vertices - 1) / 3;
            vert = new Vec3f[n_lines + 1];
            idx = 0;
            for(int i = 0; i < (n_vertices - 1) / 3; i++){
                t = 0;
                for(int j = 0; j < args->curve_tessellation; j++){
                    vert[idx++] = pow(1-t, 3)* vertices[3*i] + 3 * t * pow(1-t, 2) * vertices[3*i+1]
                            + 3 * t * t * (1 - t) * vertices[3*i+2] + t * t * t * vertices[3*i+3];
                    t += dt;
                }
            }   
            vert[idx] = vertices[n_vertices - 1];
        } else {
            n_lines = args->curve_tessellation * (n_vertices - 3);
            vert = new Vec3f[n_lines + 1];
            idx = 0;
            for(int i = 0; i < n_vertices - 3; i++){
                t = 0;
                for(int j = 0; j < args->curve_tessellation; j++){                   
                    t3 = pow(t, 3);
                    t2 = t * t;
                    vert[idx++] = pow(1-t, 3) / 6 * vertices[i] + (3*t3 - 6*t2 + 4) / 6 * vertices[i+1]
                            + (-3*t3 + 3*t2 + 3*t + 1) / 6 * vertices[i+2] + t3 / 6 * vertices[i+3];
                    t += dt;
                }
            }
            vert[idx] = 1.0 / 6 * vertices[n_vertices - 3]
                            + 4.0 / 6 * vertices[n_vertices - 2] + 1.0 / 6 * vertices[n_vertices - 1];
        }
        float d_angle = 2*M_PI / args->revolution_tessellation;
        float angle = 0;
        TriangleMesh* tria = new TriangleMesh(args->revolution_tessellation * (n_lines + 1), 2 * args->revolution_tessellation * n_lines);
        for(int i = 0; i < args->revolution_tessellation; i++){
            for(int j = 0; j < n_lines + 1; j++){
                tria->SetVertex(i * (n_lines + 1) + j, Vec3f(vert[j].x()*cos(angle), vert[j].y(), vert[j].x()*sin(angle)));
            }
            angle += d_angle;
        }
        idx = 0;
        for(int i = 0; i < args->revolution_tessellation; i++){
            for(int j = 0; j < n_lines; j++){
                tria->SetTriangle(idx++, i * (n_lines + 1) + j, i * (n_lines + 1) + j + 1,  (i + 1) % args->revolution_tessellation * (n_lines + 1) + j);
                tria->SetTriangle(idx++, (i + 1) % args->revolution_tessellation * (n_lines + 1) + j, i * (n_lines + 1) + j + 1,  (i + 1) % args->revolution_tessellation * (n_lines + 1) + j + 1);
            }
        }
        delete [] vert;
        return tria;
    }

};

class BezierPatch: public Surface {

public:

    BezierPatch(){}
    BezierPatch(Curve* c){}
    ~BezierPatch(){}
};

#endif
