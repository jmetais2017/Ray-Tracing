
/* 
 * File:   hitable.h
 * Author: eleve
 *
 * Created on 11 février 2019, 17:43
 */

#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"

struct hit_record{
    float t;
    vec3 p;
    vec3 normal;
};

class hitable {
public:
    virtual bool hit(const ray& r,  float t_min, float t_max, hit_record& rec) const = 0;
    
private:

};

#endif /* HITABLE_H */

