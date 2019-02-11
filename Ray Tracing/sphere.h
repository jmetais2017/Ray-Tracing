
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
    sphere(vec3 cen, float r) : center(cen), radius(r) {};
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    vec3 center;
    float radius;
    
    
private:

};

#endif /* SPHERE_H */

