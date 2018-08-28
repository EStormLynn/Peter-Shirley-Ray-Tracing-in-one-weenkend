#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "random"
#include "material.h"
#define random(a,b) (rand()%(b-a+1)+a)

using namespace std;

vec3 color(const ray& r,hitable *world, int depth)
{
    hit_record rec;
    if(world->hit(r,0.0,MAXFLOAT,rec))
    {
        // 散射后的光线
        ray scattered;
        // 衰减
        vec3 attenuation;

        if(depth<50 && rec.mat_ptr->scatter(r,rec,attenuation,scattered))
        {
            // 递归 衰减
            return attenuation * color(scattered, world, depth+1);
        } else
        {
            return vec3(0,0,0);
        }
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 *(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0,1.0,1.0) + t*vec3(0.5,0.7,1.0);
    }
}

int main()
{
    int nx =200;
    int ny =100;
    // 采样数量ns
    int ns = 100;
    cout<<"P3\n"<<nx<<" "<<ny<<"\n255\n";

    camera cam;

    hitable *list[2];
    // 球1,2,3,4
    list[0] = new sphere(vec3(0,0,-1),0.5,new lambertian(vec3(0.8,0.3,0.3)));
    list[1] = new sphere(vec3(0,-100.5,-1),100,new lambertian(vec3(0.8,0.8,0.0)));
    list[2] = new sphere(vec3(1,0,-1),0.5,new metal(vec3(0.8,0.6,0.2),1));
    list[3] = new sphere(vec3(-1,0,-1),0.5,new metal(vec3(0.8,0.8,0.8),1));

    hitable *world = new hitable_list(list,4);
    random_device rd;

    for(int j=ny-1;j>=0;j--)
    {
        for(int i=0;i<nx;i++)
        {
            vec3 col(0,0,0);

            for(int s = 0; s<ns; s++)
            {
                float u = (float(i)+float(random(0,100))/100.0f)/float(nx);
                float v = (float(j)+float(random(0,100))/100.0f)/float(ny);

                ray r = cam.get_ray(u,v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r,world,0);
            }
            // color 取均值
            col /= float(ns);
            col = vec3(sqrt(col[0]),sqrt(col[1]),sqrt(col[2]));

            int ir=int(255.99* col[0]);
            int ig=int(255.99* col[1]);
            int ib=int(255.99* col[2]);;
            cout<<ir<<" "<<ig<<" "<<ib<<"\n";
        }
    }

}