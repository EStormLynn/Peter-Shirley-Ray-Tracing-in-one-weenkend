#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "random"
#define random(a,b) (rand()%(b-a+1)+a)
#define random1 (float((rand()%(100)))/100.0f)

using namespace std;

// 单位cube随机取点,返回一个在球内的点
vec3 random_in_unit_sphere()
{
    vec3 p;
    do{
        p = 2.0*vec3(random1,random1,random1) - vec3(1,1,1);
    }while (dot(p,p) >= 1.0);
    return p;
}

vec3 color(const ray& r,hitable *world)
{
    hit_record rec;
    if(world->hit(r,0.0,MAXFLOAT,rec))
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5* color(ray(rec.p, target - rec.p), world);
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
    int ny=100;
    // 采样数量ns
    int ns = 100;
    cout<<"P3\n"<<nx<<" "<<ny<<"\n255\n";

    camera cam;

    hitable *list[2];
    // 球1
    list[0] = new sphere(vec3(0,0,-1),0.5);
    // 球2
    list[1] = new sphere(vec3(0,-100.5,-1),100);

    hitable *world = new hitable_list(list,2);
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
                col += color(r,world);
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