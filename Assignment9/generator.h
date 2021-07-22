#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "vectors.h"
#include "forcefield.h"
#include "particle.h"
#include "random.h"

class Generator{

public:
    Generator(){
        rand_lifespan = new Random();
        rand_color = new Random();
        rand_mass = new Random();
        color = Vec3f(1,1,1);
        dead_color= Vec3f(0.1,0.1,0.1);
        color_randomness = 0;
        lifespan = 10;
        desired_num_particles = 1000;
        lifespan_randomness = 0;
        mass = 1;
        mass_randomness = 0;
    }    
    ~Generator(){
        if(rand_lifespan != NULL){
            delete rand_lifespan;
            rand_lifespan = NULL;
        }
        if(rand_color != NULL){
            delete rand_color;
            rand_color = NULL;
        }
        if(rand_mass != NULL){
            delete rand_mass;
            rand_mass = NULL;
        }
    }

    void SetColors(Vec3f color, Vec3f dead_color, float color_randomness){
        this->color = color;
        this->dead_color = dead_color;
        this->color_randomness = color_randomness;
        // rand_color = new Random();
    }

    void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles){
        this->lifespan = lifespan;
        this->desired_num_particles = desired_num_particles;
        this->lifespan_randomness = lifespan_randomness;
        // rand_lifespan = new Random();
    }
    void SetMass(float mass, float mass_randomness){
        this->mass = mass;
        this->mass_randomness = mass_randomness;
        // rand_mass = new Random();
    }

    // on each timestep, create some particles
    int numNewParticles(float current_time, float dt) const{
        return (int)ceil(dt * desired_num_particles / lifespan);
    }
    virtual Particle* Generate(float current_time, int i){
        Particle* part = new Particle(Vec3f(0, 0, 0), Vec3f(0, 0, 0), color + color_randomness * rand_color->randomVector(),
                    dead_color + color_randomness * rand_color->randomVector(), mass + 2 * mass_randomness * (rand_mass->next() - 0.5), 
                    lifespan + lifespan_randomness * 2 * (rand_lifespan->next() - 0.5));
        return part;
    }

    // for the gui
    virtual void Paint() const{}
    virtual void Restart(){
        if(rand_lifespan != NULL){
            *rand_lifespan = Random();
        }
        if(rand_color != NULL){
            *rand_color = Random();
        }
        if(rand_mass != NULL){
            *rand_mass = Random();
        }
        // rand = Random();
    }

protected:
    Vec3f color;
    Vec3f dead_color;
    float color_randomness;
    float lifespan;
    int desired_num_particles;
    float lifespan_randomness;
    float mass;
    float mass_randomness;
    // Random rand = Random();
    Random* rand_lifespan;
    Random* rand_color;
    Random* rand_mass;

};

class HoseGenerator: public Generator{

public:
    HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness):
                    position(position), position_randomness(position_randomness),
                    velocity(velocity), velocity_randomness(velocity_randomness){
        rand_position = new Random();
        rand_velocity = new Random();
    }
    ~HoseGenerator(){
        delete rand_position;
        rand_position = NULL;
        delete rand_velocity;
        rand_velocity = NULL;
    }

    Particle* Generate(float current_time, int i){
        Particle* part = new Particle(position + position_randomness * rand_position->randomVector(), velocity + velocity_randomness * rand_velocity->randomVector(), 
                            color + color_randomness * rand_color->randomVector(), dead_color + color_randomness * rand_color->randomVector(), 
                            mass + 2 * mass_randomness * (rand_mass->next() - 0.5), lifespan + lifespan_randomness * 2 * (rand_lifespan->next() - 0.5));
        return part;
    }

    // for the gui
    void Paint() const{}

    void Restart(){
        if(rand_lifespan != NULL){
            *rand_lifespan = Random();
        }
        if(rand_color != NULL){
            *rand_color = Random();
        }
        if(rand_mass != NULL){
            *rand_mass = Random();
        }
        *rand_position = Random();
        *rand_velocity = Random();
    }

private:
    Vec3f position;
    float position_randomness;
    Vec3f velocity;
    float velocity_randomness;
    Random* rand_position;
    Random* rand_velocity;

};

class RingGenerator: public Generator{

public:
    RingGenerator(float position_randomness, Vec3f velocity, float velocity_randomness):
                    position_randomness(position_randomness),
                    velocity(velocity), velocity_randomness(velocity_randomness){
        rand_position = new Random();
        rand_velocity = new Random();
    }
    ~RingGenerator(){
        delete rand_position;
        rand_position = NULL;
        delete rand_velocity;
        rand_velocity = NULL;
    }

    Particle* Generate(float current_time, int i){
        Particle* part = new Particle(position_randomness * rand_position->randomVector(), velocity + velocity_randomness * rand_velocity->randomVector(), 
                            color + color_randomness * rand_color->randomVector(), dead_color + color_randomness * rand_color->randomVector(), 
                            mass + 2 * mass_randomness * (rand_mass->next() - 0.5), lifespan + lifespan_randomness * 2 * (rand_lifespan->next() - 0.5));
        return part;
    }

    // for the gui
    void Paint() const{}

    void Restart(){
        if(rand_lifespan != NULL){
            *rand_lifespan = Random();
        }
        if(rand_color != NULL){
            *rand_color = Random();
        }
        if(rand_mass != NULL){
            *rand_mass = Random();
        }
        *rand_position = Random();
        *rand_velocity = Random();
    }

private:
    float position_randomness;
    Vec3f velocity;
    float velocity_randomness;
    Random* rand_position;
    Random* rand_velocity;

};


#endif

