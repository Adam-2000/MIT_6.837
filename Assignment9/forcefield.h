#ifndef _FORCEFIELD_H_
#define _FORCEFIELD_H_

#include "vectors.h"

class ForceField{

public:
    ForceField(){}
    ~ForceField(){}

    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;



};

class GravityForceField: public ForceField{

public:
    GravityForceField(Vec3f gravity):gravity(gravity){}
    ~GravityForceField(){}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const{
        return gravity;
    }

private:
    Vec3f gravity;

};

class ConstantForceField: public ForceField{

public:
    ConstantForceField(Vec3f force):force(force){}
    ~ConstantForceField(){}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const{
        return force / mass;
    }

private:
    Vec3f force;

};

class RadialForceField: public ForceField{

public:
    RadialForceField(float magnitude):magnitude(magnitude){}
    ~RadialForceField(){}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const{
        return magnitude * (-position) / mass;
    }

private:
    float magnitude;

};

class VerticalForceField: public ForceField{

public:
    VerticalForceField(float magnitude):magnitude(magnitude){}
    ~VerticalForceField(){}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const{
        Vec3f ret;
        ret.Set(0, -position.y(), 0);
        ret *= magnitude / mass;
        return ret;
    }

private:
    float magnitude;

};

class WindForceField: public ForceField{

public:
    WindForceField(float magnitude):magnitude(magnitude){}
    ~WindForceField(){}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const{
        Vec3f ret;
        ret.Set(0, -position.y(), 0);
        ret *= magnitude / mass;
        return ret;
    }

private:
    float magnitude;

};

#endif

