
/* 
 * File:   material.h
 * Author: Julien
 *
 * Created on 11 février 2019, 22:36
 */

#ifndef MATERIAL_H
#define MATERIAL_H

struct hit_record;

#include "sphere.h"

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
private:

};

class lambertian : public material {
public:
    lambertian(const vec3& a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }
    
    vec3 albedo;
    
private:

};


class metal : public material {
public:
    metal(const vec3& a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        //vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        vec3 v = unit_vector(r_in.direction());
        vec3 n = rec.normal;
        vec3 reflected = v - 2*dot(v,n)*n;
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    
    vec3 albedo;
private:

};

#endif /* MATERIAL_H */

