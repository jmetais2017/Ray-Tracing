
/* 
 * File:   sphere.cpp
 * Author: eleve
 * 
 * Created on 11 février 2019, 17:46
 */

#include "sphere.h"

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0){
        float temp = (-b -sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b*b-a*c))/a;
        if (temp < t_max && temp > t_min){
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

vec3 random_in_unit_sphere(){
    vec3 p;
    do{
        p = 2.0*vec3(float(rand() % RAND_MAX) / float(RAND_MAX), 
                float(rand() % RAND_MAX) / float(RAND_MAX),
                float(rand() % RAND_MAX) / float(RAND_MAX)) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
}
