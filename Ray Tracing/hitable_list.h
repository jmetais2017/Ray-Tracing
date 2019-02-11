
/* 
 * File:   hitable_list.h
 * Author: eleve
 *
 * Created on 11 février 2019, 17:56
 */

#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "hitable.h"

class hitable_list : public hitable{
public:
    hitable_list() {}
    hitable_list(hitable **l, int n) {list = l; list_size = n;}
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    hitable **list;
    int list_size;
    
private:

};

#endif /* HITABLE_LIST_H */

