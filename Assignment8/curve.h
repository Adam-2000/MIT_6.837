#ifndef _CURVE_H_
#define _CURVE_H_

#include "spline.h"
#include "glCanvas.h"
#include "matrix.h"
#include "arg_parser.h"

static float B_bezier_[16] = {-1, 3, -3, 1,
                        3, -6, 3, 0,
                        -3, 3, 0, 0,
                        1, 0, 0, 0};
static float B_spline_[16] = {-1.0/6.0, 0.5, -0.5, 1.0/6.0,
                        0.5, -1.0, 0, 4.0/6.0,
                        -0.5, 0.5, 0.5, 1.0/6.0,
                        1.0/6.0, 0, 0, 0};

static Matrix B_bezier(B_bezier_);
static Matrix B_spline(B_spline_);

class Curve: public Spline {

public:
    Curve(){}
    ~Curve(){}

    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args){}

    // FOR CONVERTING BETWEEN SPLINE TYPES
    // virtual void OutputBezier(FILE *file) = 0;
    // virtual void OutputBSpline(FILE *file) = 0;
    // FOR CONTROL POINT PICKING
    // virtual int getNumVertices();
    // virtual Vec3f getVertex(int i);

    // FOR EDITING OPERATIONS
    virtual void moveControlPoint(int selectedPoint, float x, float y){
        set(selectedPoint, Vec3f(x, y, 0));
    }
    virtual void addControlPoint(int selectedPoint, float x, float y){}
    virtual void deleteControlPoint(int selectedPoint){}
    virtual int getLabel() = 0;

    // FOR GENERATING TRIANGLES

protected:
    
};

class BezierCurve: public Curve {

public:
    BezierCurve(int n){
        n_vertices = n;
        vertices = new Vec3f[n];
        assert(n >= 4);
    }
    ~BezierCurve(){}

    int getLabel(){return 0;}
    virtual void Paint(ArgParser *args){
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
            float t;
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
            
        glEnd();

    }
    // FOR CONVERTING BETWEEN SPLINE TYPES
    void OutputBSpline(FILE *file){
        assert(n_vertices == 4);
        float G_[16] = {vertices[0].x(), vertices[1].x(), vertices[2].x(), vertices[3].x(),
                        vertices[0].y(), vertices[1].y(), vertices[2].y(), vertices[3].y(),
                        vertices[0].z(), vertices[1].z(), vertices[2].z(), vertices[3].z()};
        Matrix G(G_);
        Matrix Inv_B_spline(B_spline);
        Inv_B_spline.Inverse(0.0001);
        G = G * B_bezier * Inv_B_spline;
        fprintf (file, "bspline\n");
        fprintf (file, "num_vertices %d\n", n_vertices);
        for (int i = 0; i < n_vertices; i++) {
            fprintf (file, "%lf %lf %lf\n", G.Get(i, 0), G.Get(i, 1), G.Get(i, 2));
        }
    }
};

class BSplineCurve: public Curve {

public:
    BSplineCurve(int n){
        assert(n >= 4);
        n_vertices = n;
        vertices = new Vec3f[n];
    }
    ~BSplineCurve(){}

    int getLabel(){return 1;}
    virtual void Paint(ArgParser *args){
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
        glEnd();

    }
    // FOR CONVERTING BETWEEN SPLINE TYPES
    void OutputBezier(FILE *file){
        assert(n_vertices == 4);
        float G_[16] = {vertices[0].x(), vertices[1].x(), vertices[2].x(), vertices[3].x(),
                        vertices[0].y(), vertices[1].y(), vertices[2].y(), vertices[3].y(),
                        vertices[0].z(), vertices[1].z(), vertices[2].z(), vertices[3].z()};
        Matrix G(G_);
        Matrix Inv_B_bezier(B_bezier);
        Inv_B_bezier.Inverse(0.0001);
        G = G * B_spline * Inv_B_bezier;
        fprintf (file, "bezier\n");
        fprintf (file, "num_vertices %d\n", n_vertices);
        for (int i = 0; i < n_vertices; i++) {
            fprintf (file, "%lf %lf %lf\n", G.Get(i, 0), G.Get(i, 1), G.Get(i, 2));
        }
    }
    Vec3f getPoint(float t){
        t *= n_vertices - 3;
        int i = min((int)floor(t), n_vertices - 4);
        t = t - i;
        Vec3f ret(0, 0, 0);
        float t3 = pow(t, 3);
        float t2 = t * t;
        ret = pow(1-t, 3) / 6 * vertices[i] + (3*t3 - 6*t2 + 4) / 6 * vertices[i+1]
                + (-3*t3 + 3*t2 + 3*t + 1) / 6 * vertices[i+2] + t3 / 6 * vertices[i+3];
        return ret; 
    }
};



#endif

