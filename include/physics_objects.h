#ifndef PHYSICS_OBJECTS_H_INCLUDED
#define PHYSICS_OBJECTS_H_INCLUDED
#include <raylib.h>

struct PhysicsRectangle : public Rectangle{
    float mass;

    //override of rectangle that assigns mass
    PhysicsRectangle(float x, float y, float width, float height, float mass){
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->mass = mass;
    }
};



#endif