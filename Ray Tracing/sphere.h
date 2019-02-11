
/* 
 * File:   sphere.h
 * Author: eleve
 *
 * Created on 11 février 2019, 17:46
 */

#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"

class sphere : public hitable {
public:
    sphere() {}
    sphere(vec3 cen, float r, material *mat) : center(cen), radius(r), mat_ptr(mat) {};
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    vec3 center;
    float radius;
    material *mat_ptr;
    
    
private:

};

vec3 random_in_unit_sphere();

#endif /* SPHERE_H */

