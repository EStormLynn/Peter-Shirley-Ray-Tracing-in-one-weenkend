#include<fstream>
#include<iostream>
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


hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n+1];
    list[0] =  new sphere(vec3(0,-700,0), 700, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            vec3 center(a+0.9*drand48(),0.2,b+0.9*drand48());
            if ((center-vec3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.8) {  // diffuse
                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
                }
                else if (choose_mat < 0.95) { // metal
                    list[i++] = new sphere(center, 0.2,
                                           new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())),  0.5*drand48()));
                }
                else {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(2.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(1, 1, 1), 0.0));

    return new hitable_list(list,i);
}


int main()
{
    string str = "";

    int nx = 1500;
    int ny = 900;
    // 采样数量ns
    int ns = 100;


    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;
    camera cam(lookfrom,lookat,vec3(0,1,0),20,float(nx)/float(ny),aperture,dist_to_focus);



    hitable *list[2];
    // 球1,2,3,4; 2个lambertian ，2个metal
    list[0] = new sphere(vec3(0,0,-1),0.5,new lambertian(vec3(0.8,0.3,0.3)));
    list[1] = new sphere(vec3(0,-100.5,-1),100,new lambertian(vec3(0.8,0.8,0.0)));
    list[2] = new sphere(vec3(1,0,-1),0.5,new metal(vec3(0.8,0.6,0.2),0.0));
    list[3] = new sphere(vec3(-1,0,-1),0.5,new dielectric(1.5));
    list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5));
    hitable *world = new hitable_list(list,5);
    world = random_scene();

    random_device rd;

    ofstream OutFile("Test.ppm"); //利用构造函数创建txt文本，并且打开该文本

    OutFile<<"P3\n"<<nx<<" "<<ny<<"\n255\n";

    for(int j=ny-1;j>=0;j--)
    {
        str = "";
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
            int ib=int(255.99* col[2]);
            string s = to_string(ir)+" "+to_string(ig)+" "+to_string(ib)+"\n";
            str+=s;
        }
        OutFile<<str;
    }

    OutFile.close();            //关闭Test.txt文件

}
