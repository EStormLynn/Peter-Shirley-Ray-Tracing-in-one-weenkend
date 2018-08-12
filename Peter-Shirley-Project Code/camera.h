//
// Created by SeeKHit on 2018/8/12.
//

#ifndef PETER_SHIRLEY_PROJECT_CODE_CAMERA_H
#define PETER_SHIRLEY_PROJECT_CODE_CAMERA_H

#include "ray.h"

class camera
{
    vec3 origin;
    vec3 horizontal;
    vec3 vertical;
    vec3 lower_left_corner;

public :
    camera()
    {
       lower_left_corner = vec3 (-2.0,-1.0,-1.0);
       horizontal = vec3(4.0,0.0,0.0);
       vertical = vec3(0.0,2.0,0.0);
       origin = vec3(0.0,0.0,0.0);
    }

    ray get_ray(float u,float v)
    {
        return ray(origin,lower_left_corner+u*horizontal + v*vertical - origin);
    }

};

#endif //PETER_SHIRLEY_PROJECT_CODE_CAMERA_H
