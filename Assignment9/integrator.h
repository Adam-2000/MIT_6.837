#ifndef _INTERGRATOR_H_
#define _INTERGRATOR_H_

#include "vectors.h"
#include "forcefield.h"
#include "particle.h"

class Integrator{

public:
    Integrator(){}
    ~Integrator(){}

    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;
    virtual Vec3f getColor() = 0;

};

class EulerIntegrator : public Integrator{

public:
    EulerIntegrator (){}
    ~EulerIntegrator (){}

    void Update(Particle *particle, ForceField *forcefield, float t, float dt){
        Vec3f old_pos = particle->getPosition();
        Vec3f old_vel = particle->getVelocity();
        particle->setPosition(old_pos + old_vel * dt);
        particle->setVelocity(old_vel + forcefield->getAcceleration(old_pos, particle->getMass(), t) * dt);
        particle->increaseAge(dt);
    }
    Vec3f getColor(){
        return Vec3f(1,1,1);
    }

};

class MidpointIntegrator  : public Integrator{

public:
    MidpointIntegrator  (){}
    ~MidpointIntegrator  (){}

    void Update(Particle *particle, ForceField *forcefield, float t, float dt){
        Vec3f old_pos = particle->getPosition();
        Vec3f old_vel = particle->getVelocity();
        Vec3f mid_pos = old_pos + old_vel * dt / 2;
        Vec3f mid_vel = old_vel + forcefield->getAcceleration(old_pos, particle->getMass(), t) * dt / 2;
        particle->setPosition(old_pos + mid_vel * dt);
        particle->setVelocity(old_vel + forcefield->getAcceleration(mid_pos, particle->getMass(), t + dt / 2) * dt);
        particle->increaseAge(dt);
    }
    Vec3f getColor(){
        return Vec3f(1,1,1);
    }

};

class RungeKuttaIntegrator  : public Integrator{

public:
    RungeKuttaIntegrator  (){}
    ~RungeKuttaIntegrator  (){}

    void Update(Particle *particle, ForceField *forcefield, float t, float dt){
        Vec3f old_pos = particle->getPosition();
        Vec3f old_vel = particle->getVelocity();
        Vec3f mid_pos = old_pos + old_vel * dt / 2;
        Vec3f mid_vel = old_vel + forcefield->getAcceleration(old_pos, particle->getMass(), t) * dt / 2;
        particle->setPosition(old_pos + mid_vel * dt);
        particle->setVelocity(old_vel + forcefield->getAcceleration(mid_pos, particle->getMass(), t + dt / 2) * dt);
        particle->increaseAge(dt);
    }
    Vec3f getColor(){
        return Vec3f(1,1,1);
    }

};

#endif

