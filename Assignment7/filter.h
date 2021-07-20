#ifndef _FILTER_H_
#define _FILTER_H_

#include "film.h"
#include "vectors.h"

class Filter{

public:
    Filter(){}
    ~Filter(){}

    virtual Vec3f getColor(int i, int j, Film *film) = 0;
    virtual float getWeight(float x, float y) = 0;
    virtual int getSupportRadius() = 0;

};

class BoxFilter: public Filter{

public:
    BoxFilter(float radius):radius(radius){
        support_radius = (int)ceil(radius - 0.5);
    }
    ~BoxFilter(){}

    Vec3f getColor(int i, int j, Film *film){
        float weight_sum = 0;
        Vec3f color_sum = Vec3f(0, 0, 0);
        float x, y;
        float weight;
        Sample sample;
        for(int pixel_x = max(0, i - support_radius); pixel_x < min(film->getWidth(), i + support_radius + 1); pixel_x++){
            for(int pixel_y = max(0, j - support_radius); pixel_y < min(film->getHeight(), j + support_radius + 1); pixel_y++){
                for(int k = 0; k < film->getNumSamples(); k++){
                    sample = film->getSample(pixel_x, pixel_y, k);
                    weight_sum += weight = getWeight(pixel_x - i + sample.getPosition().x() - 0.5, pixel_y - j + sample.getPosition().y() - 0.5);
                    color_sum += weight * sample.getColor();
                }
            }
        }
        color_sum /= weight_sum;
        return color_sum;
    }
    float getWeight(float x, float y){
        if(max(abs(x), abs(y)) <= radius){
            return 1.0;
        }
        return 0.0;
    }
    int getSupportRadius(){return support_radius;}

private:
    float radius;
    int support_radius;

};

class TentFilter: public Filter{

public:
    TentFilter(float radius):radius(radius){
        support_radius = (int)ceil(radius - 0.5);
    }
    ~TentFilter(){}

    Vec3f getColor(int i, int j, Film *film){
        float weight_sum = 0;
        Vec3f color_sum = Vec3f(0, 0, 0);
        float x, y;
        float weight;
        Sample sample;
        for(int pixel_x = max(0, i - support_radius); pixel_x < min(film->getWidth(), i + support_radius + 1); pixel_x++){
            for(int pixel_y = max(0, j - support_radius); pixel_y < min(film->getHeight(), j + support_radius + 1); pixel_y++){
                for(int k = 0; k < film->getNumSamples(); k++){
                    sample = film->getSample(pixel_x, pixel_y, k);
                    weight_sum += weight = getWeight(pixel_x - i + sample.getPosition().x() - 0.5, pixel_y - j + sample.getPosition().y() - 0.5);
                    color_sum += weight * sample.getColor();
                }
            }
        }
        color_sum /= weight_sum;
        return color_sum;
    }
    float getWeight(float x, float y){
        return max(1 - sqrt(x*x + y*y) / radius, float(0));
    }
    int getSupportRadius(){return support_radius;}

private:
    float radius;
    int support_radius;

};

class GaussianFilter: public Filter{

public:
    GaussianFilter(float sigma):sigma(sigma){
        support_radius = (int)ceil(2 * sigma - 0.5);
    }
    ~GaussianFilter(){}

    Vec3f getColor(int i, int j, Film *film){
        float weight_sum = 0;
        Vec3f color_sum = Vec3f(0, 0, 0);
        float x, y;
        float weight;
        Sample sample;
        static int cnt = 0;
        for(int pixel_x = max(0, i - support_radius); pixel_x < min(film->getWidth(), i + support_radius + 1); pixel_x++){
            for(int pixel_y = max(0, j - support_radius); pixel_y < min(film->getHeight(), j + support_radius + 1); pixel_y++){
                for(int k = 0; k < film->getNumSamples(); k++){
                    sample = film->getSample(pixel_x, pixel_y, k);
                    weight_sum += weight = getWeight(pixel_x - i + sample.getPosition().x() - 0.5, pixel_y - j + sample.getPosition().y() - 0.5);
                    color_sum += weight * sample.getColor();
                }
            }
        }
        cnt++;
        color_sum /= weight_sum;
        return color_sum;
    }
    float getWeight(float x, float y){
        float l = sqrt(x*x + y*y);
        if(l <= 2 * sigma){
            return exp(- l * l / (2 * sigma * sigma));
        }
        return 0.0;
    }
    int getSupportRadius(){return support_radius;}

private:
    float sigma;
    int support_radius;

};
#endif