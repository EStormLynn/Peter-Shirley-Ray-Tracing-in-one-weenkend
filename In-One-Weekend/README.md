# Peter Shirley-Ray Tracing in One Weekend (2016)

原著：Peter Shirley

本书是Peter Shirley ray tracing系列三部曲的第一本，也是学习ray tracing 入门比较容易的一本书，自己照着书上的内容，抄了一遍，Github有完整的代码，和每一章学习过程的代码，部分代码加了注释。

[英文原著地址](https://pan.baidu.com/s/1b5CvAdElCcXAO2R4lNFgkA)  密码: urji

[Github地址](https://github.com/EStormLynn/Peter-Shirley-Ray-Tracing-in-one-weenkend)



## 目录：
- [x] [Chapter1:Output an image](https://github.com/EStormLynn/Peter-Shirley-Ray-Tracing-in-one-weenkend#chapter1output-an-image)
- [x] [Chapter2:The vec3 class](https://github.com/EStormLynn/Peter-Shirley-Ray-Tracing-in-one-weenkend#chapter2the-vec3-class)
- [x] [Chapter3:Rays, a simple camera, and background](https://github.com/EStormLynn/Peter-Shirley-Ray-Tracing-in-one-weenkend#chapter3rays-a-simple-camera-and-background)
- [x] [Chapter4:Adding a sphere](https://github.com/EStormLynn/Peter-Shirley-Ray-Tracing-in-one-weenkend#chapter4adding-a-sphere)
- [x] [Chapter5:Surface normals and multiple objects](https://github.com/EStormLynn/Peter-Shirley-Ray-Tracing-in-one-weenkend#chapter5surface-normals-and-multiple-objects)
- [x] [Chapter6:Antialiasing](https://github.com/EStormLynn/Peter-Shirley-Ray-Tracing-in-one-weenkend#chapter6antialiasing)
- [x] [Chapter7:Diffuse Materials](https://github.com/EStormLynn/Peter-Shirley-Ray-Tracing-in-one-weenkend#chapter7diffuse-materials)
- [x] [Chapter8:Metal](https://github.com/EStormLynn/Peter-Shirley-Ray-Tracing-in-one-weenkend#chapter8metalADD)
- [x] [Chapter9:Dielectrics](https://github.com/EStormLynn/Peter-Shirley-Ray-Tracing-in-one-weenkend#chapter9dielectrics)
- [x] [Chapter10:Positionable camera](https://github.com/EStormLynn/Peter-Shirley-Ray-Tracing-in-one-weenkend#chapter10positionable-camera)
- [x] [Chapter11:Defocus](https://github.com/EStormLynn/Peter-Shirley-Ray-Tracing-in-one-weenkend#chapter11defocus-blur)
- [x] [Chapter12:Where next?](https://github.com/EStormLynn/Peter-Shirley-Ray-Tracing-in-one-weenkend#chapter12where-next)


## Chapter1:Output an image
使用ppm渲染到图片

```c++
#include <iostream>

using namespace std;


int main()
{
    int nx =200;
    int ny=100;
    cout<<"P3\n"<<nx<<" "<<ny<<"\n255\n";
    for(int j=ny-1;j>=0;j--)
    {
        for(int i=0;i<nx;i++)
        {
            float r=float(i)/float(nx);
            float g=float(j)/float(ny);
            float b=0.2;

            int ir=int(255.99*r);
            int ig=int(255.99*g);
            int ib=int(255.99*b);
            cout<<ir<<" "ig<<" "<<ib<<"\n";
        }
    }
}
```
说明：
* 像素从左往右打印
* 从上向下打印
* 这个例子中RGB计算出来在[0,1]之间，输出之前映射到一个高范围空间
* 红+绿=黄
* 打印的内容保存成.ppm格式即可预览

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Chapter01_Output%20an%20image.png" width="400" height="200" alt=""/></div>

## Chapter2:The vec3 class
用于几何向量计算和颜色计算，包含颜色，向量，位置坐标，偏移，主要包含重写操作符，以及点乘、叉乘等操作。
```c++
class vec3  {
public:
    vec3() {}
    vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const vec3& operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; };

    inline vec3& operator+=(const vec3 &v2);
    inline vec3& operator-=(const vec3 &v2);
    inline vec3& operator*=(const vec3 &v2);
    inline vec3& operator/=(const vec3 &v2);
    inline vec3& operator*=(const float t);
    inline vec3& operator/=(const float t);

    inline float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
    inline float squared_length() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
    inline void make_unit_vector();

    float e[3];
};

```

## Chapter3:Rays, a simple camera, and background
所有的ray tracers 都是以ray类为基础，计算颜色
    p(t) = A + t*B
其中A是光源点，B是ray的方向，t是具体float值，空间中确定一条线，不同的t，可以到达不同地方。

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/3.1.png" width="400" height="200" alt=""/></div>


p(t)称为点A关于t的函数。Ray tracing的本质是通过发射射线，计算像素点的颜色。在ray tracing之前需要有个摄像机，建立坐标系，显示背景色，以及ray hit的点的颜色。

假设摄像机的位置就是眼睛位置，看到的内容为ppm显示的东西，简历坐标系，z轴正方向，垂直平面向外，x向右，y向上，


<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/3.2.png" width="400" height="200" alt=""/></div>


计算公式：

    blended_value = (1-t)*start_value + t*end_value

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-05%20at%2011.57.37%20PM.png" width="400" height="200" alt=""/></div>


## Chapter4:Adding a sphere

球的公式：

    x*x + y*y +z*z = R*R

对于任意xyz，如果满足球面公式，(x,y,z)为球面的一个点。

如果球心位置为（cx,cy,cz）,公式为

    (x-cx)*(x-cx) + (y-cy)*(y-cy) + (z-cz)*(z-cz) = R*R

用向量表示，球面点P，球心点C，半径可以表示为向量PC

    dot((p-C)(p-C)) = (x-cx)*(x-cx) + (y-cy)*(y-cy) + (z-cz)*(z-cz) 

等价于

    dot((A + t*B - C),(A + t*B - C)) = R*R

展开之后

    t*t*dot(B,B) + 2*t*dot(A-C,A-C) + dot(C,C) - R*R = 0

ABC已知，这里是一个关于t的一元二次方程，对于t无解，有一个解，有两个解的情况，即为下图

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/roots.png" width="400" height="200" alt=""/></div>


通过打印颜色，利用红色的射线，ray hit 圆，hit到的地方显示红色

```c++
bool hit_sphere(const vec3 & center, float radius,const ray& r)
{
    vec3 oc = r.origin() -center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc,r.direction());
    float c = dot(oc,oc) -radius*radius;
    float discriminant = b*b - 4*a*c;
    return (discriminant>0);
}

vec3 color(const ray& r)
{
    if(hit_sphere(vec3(0,0,-1),0.5,r))
        return vec3(1.0,0,0);

    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 *(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0,1.0,1.0) + t*vec3(0.5,0.7,1.0);
}
```


<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-07%20at%2012.29.04%20AM.png" width="400" height="200" alt=""/></div>


## Chapter5:Surface normals and multiple objects

法线是垂直与物体表面的一个向量，对于上一节提到的球，他的法线方向是，从球心出发，射向hitpoint的。就像在地球上，地面的法向是从地心出发，射向你站立的点的。

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-10%20at%201.07.39%20AM.png" width="350" height="200" alt=""/></div>


假设N是长度在[-1，1]之间的单位向量，映射到去见[0,1]之间，再映射x/y/z到r/g/b，通常除了须要知道是否hit点，还要拿到hit point的数据。

```C++
// 本章 hit_Sphere的返回值改为float了
float hit_sphere(const vec3 & center, float radius,const ray& r)
{
    vec3 oc = r.origin() -center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc,r.direction());
    float c = dot(oc,oc) -radius*radius;
    float discriminant = b*b - 4*a*c;
    if(discriminant<0)
        return -1.0;
    else
        return (-b-sqrt(discriminant))/(2.0*a);
}

vec3 color(const ray& r)
{
    float t = hit_sphere(vec3(0,0,-1),0.5,r);
    if(t>0.0)
    {
        // 球心到hitpoint的单位法向量
        vec3 N = unit_vector(r.point_at_parameter(t)-vec3(0,0,-1));
        return 0.5*vec3(N.x() +1,N.y()+1,N.z()+1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 *(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0,1.0,1.0) + t*vec3(0.5,0.7,1.0);
}

```


<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-10%20at%201.07.29%20AM.png" width="400" height="200" alt=""/></div>

当场景中有多个可以被击中的物体的时候，需要一个Hitable的抽象类，包含抽象方法hit 是否击中，以及记录hit到的数据，包括hit的位置，hit点的法向，以及距离t

通过距离t
    tmin< t < tmax 
来控制hit到物体的距离远近，因为hit到之后将不再往后ray tracing。

```C++
#include "ray.h"

struct hit_record
{
    float t;
    vec3 p;
    vec3 normal;
};

class hitable
{
public:
    virtual bool hit(const ray& r,float t_min,float t_max,hit_record & rec)const =0;
};
```

对于sphere类基础hitable抽象类，实现自己的hit方法，去判断是否击中了球的对象

```C++

#include "hitable.h"

class sphere: public hitable  {
public:
    sphere() {}
    sphere(vec3 cen, float r) : center(cen), radius(r)  {};
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    vec3 center;
    float radius;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}
```

还需要一个hitable list去记录击中所有的物体，也是继承hitable类，实现hit方法，去找出最近的物体。

```C++

#include "hitable.h"

class hitable_list: public hitable  {
public:
    hitable_list() {}
    hitable_list(hitable **l, int n) {list = l; list_size = n; }
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    hitable **list;
    int list_size;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size; i++) {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}
```

本章新的main函数如下
```C++
#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"

using namespace std;


vec3 color(const ray& r,hitable *world)
{
    hit_record rec;
    if(world->hit(r,0.0,MAXFLOAT,rec))
        return 0.5*vec3(rec.normal.x()+1,rec.normal.y()+1,rec.normal.z()+1);
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
    cout<<"P3\n"<<nx<<" "<<ny<<"\n255\n";
    vec3 lower_left_corner(-2.0,-1.0,-1.0);
    vec3 horizontal(4.0,0.0,0.0);
    vec3 vertical(0.0,2.0,0.0);
    vec3 origin(0.0,0.0,0.0);

    hitable *list[2];
    // 球1
    list[0] = new sphere(vec3(0,0,-1),0.5);
    // 球2
    list[1] = new sphere(vec3(0,-100.5,-1),100);

    hitable *world = new hitable_list(list,2);
    for(int j=ny-1;j>=0;j--)
    {
        for(int i=0;i<nx;i++)
        {
            float u = float(i)/float(nx);
            float v = float(j)/float(ny);

            ray r(origin,lower_left_corner + u*horizontal +v * vertical);

            vec3 p = r.point_at_parameter(2.0);
            vec3 col = color(r,world);

            int ir=int(255.99* col[0]);
            int ig=int(255.99* col[1]);
            int ib=int(255.99* col[2]);;
            cout<<ir<<" "<<ig<<" "<<ib<<"\n";
        }
    }
}
```

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-10%20at%201.35.11%20AM.png" width="400" height="200" alt=""/></div>

## Chapter6:Antialiasing

真实世界中，照相机拍照时，一边边缘部分没有锯齿，因为每个像素，前景和背景在边缘的地方进行的混合。我们可以通过平均多个像素的值，达到一样的效果。我们的做法是，抽象camera类，后面再写颜色的部分。

还需要写个随机数的生成器，用来控制采样点的位置，范围是在[0,1]之间。这里我定义了一个宏
    
    #define random(a,b) (rand()%(b-a+1)+a)

使用rand()程序运行时每次生成的随机数和上一次相同，便于调试。

对于给的一个像素，我们有好几个采样点在像素内，对每个采样点进行ray tracer，再平均每个采样点的color。


<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/average%20sample.png" width="300" height="200" alt=""/></div>


camera类
```c++
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
```

main函数
```C++
#include <iostream>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "random"
#define random(a,b) (rand()%(b-a+1)+a)

using namespace std;

vec3 color(const ray& r,hitable *world)
{
    hit_record rec;
    if(world->hit(r,0.0,MAXFLOAT,rec))
        return 0.5*vec3(rec.normal.x()+1,rec.normal.y()+1,rec.normal.z()+1);
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

            int ir=int(255.99* col[0]);
            int ig=int(255.99* col[1]);
            int ib=int(255.99* col[2]);;
            cout<<ir<<" "<<ig<<" "<<ib<<"\n";
        }
    }

}
```

最后达到的效果如下

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/AA.png" width="400" height="400" alt=""/></div>


## Chapter7:Diffuse Materials
之前已经实现了多个object 和每个像素多个采样，本章将实现漫反射材质。首先需要明确的一点是，物体和材质的关系，我们假设球体有一个自己的材质，通常在渲染中，每个物体都有自己的材质。

不发光的物体，漫反射是吸收周围的颜色，显示出来，物体表面反射周围的光线的方向是随机的，如下图，在2个不同的物体的漫反射表面间，发射了3条光线，三条光线的漫反射之后的路径各不相同：

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-12%20at%202.02.10%20PM.png" width="400" height="200" alt=""/></div>

漫反射物体的表面，也可能会吸收部分光线，表面越暗，吸收的光线越多，吸收之后看起来就像一个哑光的表面。

选择一个随机的点切一个单位半径的球，这个点就是hitpoint，在球上选个随机点s，从p到s做一条线，作为漫反射的方向，这个球的球心是（p + N），N是hitpoitn的法向。

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-12%20at%202.12.28%20PM.png" width="400" height="200" alt=""/></div>

关于球面上s点如何区，这里的做法是，在单位cube中，选一个点，x、y、z都在[-1,1]之间，如果这个点不在球内，继续选点，直到满足在球内的这个条件。

```C++
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
```

得到的图像如下：
<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-12%20at%202.33.09%20PM.png" width="400" height="200" alt=""/></div>

球和地板的交界处的颜色可能不明显，是因为吸收的光太多了，可以通多将颜色开放的方法，来提高物体表面的亮度，减少吸收的光

    col = vec3(sqrt(col[0]),sqrt(col[1]),sqrt(col[2]));

这样就可以看清楚交界处的阴影效果了，如下图：

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-12%20at%202.39.00%20PM.png" width="400" height="200" alt=""/></div>

## Chapter8:Metal
对于不同的物体，可能有不同的材质，所以就需要设计一个材质抽象类，包含一些参数。对于程序而言，材质需要做的事情包括

* 1.产生一个散射体（或者表示吸收了多少光线）
* 2.如果发生散射，表达出光线应该衰减多少

抽象类如下：
```C++
class material  {
public:
    // 散射虚函数
    // 参数：r_in 入射的光线， rec hit的记录， attenuation v3的衰减，scattered 散射后的光线
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};
```

hitables 和material须要知道对方的数据，所以在c++代码中，hit_record中加了一个指针 * mat_ptr 指向material这个类。

```C++
struct hit_record
{
    float t;
    vec3 p;
    vec3 normal;
    material *mat_ptr;
};
```

lambertian 材质，主要是漫反射，通过attenuation衰减，来控制散射之后的光线强度，散射的方向用random_in_unit_sphere()控制，albedo表示反射率

```C++
class lambertian : public material {
public:
    lambertian(const vec3& a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const  {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target-rec.p);
        attenuation = albedo;
        return true;
    }

    vec3 albedo;    // 反射率
};
```

对于光滑表面的物体，ray不会随机的散射，物理规律是反射角等于入射角，会发生镜面反射，向量的说明如下：

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-13%20at%2011.53.40%20PM.png" width="260" height="200" alt=""/></div>

红色的是反射光线，向量表示是(v+2B)，N是单位法向量，v是入射光线的方向向量，B的模是v和N的点乘 dot(v,N)。公式为：
	
	vec3 reflect(const vec3& v, const vec3& n) {
    	return v - 2*dot(v,n)*n;
	}

metal材质只反射光线，代码如下：
```C++
class metal : public material {
public:
    metal(const vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const  {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    vec3 albedo;
    float fuzz;
};
```

修改color方法，对散射进行递归，求color
```C++

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
```

再在场景中添加2个metal材质的球，main函数如下
注意因为加了hitrecord添加了material ，sphere的hit函数须要将hit_record的引用传出来，须要在函数内形参的指针指向material的matptr。
```C++
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
```

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-14%20at%2012.42.39%20AM.png" width="400" height="200" alt=""/></div>

关于metal的反射，也可以用一个随机性的反射方向，来做微量的偏移，相当于一个小球上选endpoint。fuzzinss就相当于这个小球的半径，可以决定反射偏移的多少。fuzz取值在[0,1]之间

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-14%20at%201.02.35%20AM.png" width="300" height="200" alt=""/></div>

最后效果如下，注意两边的metal sphere中反射的边界模糊。

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-14%20at%201.02.45%20AM.png" width="400" height="200" alt=""/></div>

## Chapter9:Dielectrics

透明的物体，比如水，玻璃，钻石是电介质，当光射入的时候，不仅发生反射，还会发生折射。折射光线是ray tracer中比较难debug的部分。本章节在场景中放入了2个玻璃球，渲染出来的画面是这样的：

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-16%20at%2012.04.21%20AM.png" width="400" height="200" alt=""/></div>

光从一种介质进入另一种介质时，实际上，有一部分光会折射进入另一种介质，有另一部分光则会反射回来。反射系数=反射光振幅（能量）/入射光振幅（能量）。

反射系数的求解是是一个非常复杂的过程，Christophe Schlick这个人提供一个逼近公式，这个公式被称为“ChristopheSchlick’s Approximation”。Wiki链接：

https://en.wikipedia.org/wiki/Schlick%27s_approximation

当反射系数为0，只有折射，没有反射。

折射满足 斯涅尔定律（Snell law）

	n * sin(theta) = n' * sin(theat')

折射系数（air= 1,glass = 1.3-1.7, diamond = 2.4）

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-16%20at%2012.05.54%20AM.png" width="250" height="200" alt=""/></div>

折射部分的代码如下：

```C++
bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0) {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else
        return false;
}
```

电解质材质总是会发生折射，所以材质类中派生dielectric类。

```C++

class dielectric : public material {
public:
    dielectric(float ri) : ref_idx(ri) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const  {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_prob;
        float cosine;
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            //         cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
            cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
            cosine = sqrt(1 - ref_idx*ref_idx*(1-cosine*cosine));
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
            reflect_prob = schlick(cosine, ref_idx);
        else
            reflect_prob = 1.0;

        // 随机数小与反射系数，设为反射光线，反之为折射光线
        if (drand48() < reflect_prob)
            scattered = ray(rec.p, reflected);
        else
            scattered = ray(rec.p, refracted);
        return true;
    }

    float ref_idx;
};
```
衰减始终是1，玻璃表面不吸收任何光线。

当场景中添加4个球，渲染出来的画面是这样的
```c++
    list[0] = new sphere(vec3(0,0,-1),0.5,new lambertian(vec3(0.8,0.3,0.3)));
    list[1] = new sphere(vec3(0,-100.5,-1),100,new lambertian(vec3(0.8,0.8,0.0)));
    list[2] = new sphere(vec3(1,0,-1),0.5,new metal(vec3(0.8,0.6,0.2),0.3));
    list[3] = new sphere(vec3(-1,0,-1),0.5,new dielectric(1.5));
```

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-16%20at%2012.18.55%20AM.png" width="400" height="200" alt=""/></div>

如果对于电介质的球内部再加一个半径为 负的球，得到的效果如下（感觉是不同介质之间负负得正了）：

```c++
    list[0] = new sphere(vec3(0,0,-1),0.5,new lambertian(vec3(0.8,0.3,0.3)));
    list[1] = new sphere(vec3(0,-100.5,-1),100,new lambertian(vec3(0.8,0.8,0.0)));
    list[2] = new sphere(vec3(1,0,-1),0.5,new metal(vec3(0.8,0.6,0.2),0.3));
    list[3] = new sphere(vec3(-1,0,-1),0.5,new dielectric(1.5));
    list[4] = new sphere(vec3(-1,0,-1),-0.45,new dielectric(1.5));
```

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-16%20at%2012.24.39%20AM.png" width="400" height="200" alt=""/></div>

## Chapter10:Positionable camera

自由位置的camera，首先有了解FOV（Field of view）视场的概念，相当于视力看到的一定角度的内容。

从射线源点的位置，射向z=-1的平面，可以看到的高度h，满足：

	h = tan(theta/2)

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-18%20at%2012.29.32%20AM.png" width="250" height="200" alt=""/></div>

修改camera部分的代码，增加fov 和aspect来控制可以看到的宽和高。

设置好camera的viewpoint之后，viewpoint就是lookfrom的点，看向的点就是lookat，还需要确定看过去水平方向的视野宽度，和竖直方向的视野宽度，camera所在平面竖直向上的向量"view up” vup，通过叉乘，拿到uvw，刚好相当于一个相机的坐标系。

```C++
class camera
{
    vec3 origin;
    vec3 horizontal;
    vec3 vertical;
    vec3 lower_left_corner;

public :
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect)
    {
        vec3 u,v,w;
        float theta = vfov*M_PI/180;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        origin = lookfrom;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w,u);

        lower_left_corner = vec3 (-half_width,-half_height,-1.0);
        lower_left_corner = origin - half_width*u - half_height*v - w;
        horizontal = 2*half_width*u;
        vertical = 2*half_height*v;
    }

    ray get_ray(float u,float v)
    {
        return ray(origin,lower_left_corner+u*horizontal + v*vertical - origin);
    }

};

```



<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-18%20at%2012.26.23%20AM.png" width="700" height="200" alt=""/></div>

设置新的摄像机，fov分别设置90和30°，得到的画面如下：


<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-18%20at%2012.38.15%20AM.png" width="400" height="200" alt=""/></div>

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-18%20at%2012.34.45%20AM.png" width="400" height="200" alt=""/></div>



## Chapter11:Defocus Blur

散焦模糊（虚化），拍照的时候，我们经常会制造出虚化的效果，主题清晰，背景或者前景模糊，这是因为摄像机具有焦距，会有一个成像面，在有效焦距内的物体才能清晰成像，通过光圈控制进光量也可以控制虚化的范围。大光圈和长焦端，都可以制造出浅景深的效果。

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-18%20at%201.14.05%20AM.png" width="300" height="200" alt=""/></div>

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-18%20at%201.14.00%20AM.png" width="400" height="200" alt=""/></div>

本章引入aperture（光圈）,focus_dist(焦距) 2个参数，来实现画面的虚化效果。
```C++

class camera
{
    vec3 origin;
    vec3 u,v,w;
    vec3 horizontal;
    vec3 vertical;
    vec3 lower_left_corner;
    float len_radius;

public :
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist)
    {
        len_radius = aperture/2;
        float theta = vfov*M_PI/180;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        origin = lookfrom;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w,u);

        lower_left_corner = origin - half_width*focus_dist*u - half_height*focus_dist*v - focus_dist*w;
        horizontal = 2*half_width*focus_dist*u;
        vertical = 2*half_height*focus_dist*v;
    }

    ray get_ray(float s,float t)
    {
        vec3 rd = len_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() +v*rd.y();
        return ray(origin + offset,lower_left_corner+s*horizontal + t*vertical - origin - offset);
    }

    vec3 random_in_unit_disk()
    {
        vec3 p;
        do{
            p = 2.0*vec3(drand48(),drand48(),0)-vec3(1,1,0);
        }while (dot(p,p)>=1.0);
        return p;
    }

};
```


改变camera的参数，设置光圈和焦距
```C++
    vec3 lookfrom(3,3,2);
    vec3 lookat(0,0,-1);
    float dist_to_focus = (lookfrom-lookat).length();
    float aperture = 2.0;
    camera cam(lookfrom,lookat,vec3(0,1,0),20,float(nx)/float(ny),aperture,dist_to_focus);
```

拿到的效果如下：

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-18%20at%201.12.43%20AM.png" width="400" height="200" alt=""/></div>

## Chapter12:Where next?

这张主要运用本书学到的知识，完成封面上的图片的渲染。

通过添加一个随机生成的世界，达到很多个小球的效果
```C++

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
```

<div align=center><img src="http://oo8jzybo8.bkt.clouddn.com/QQ20180819-0.png" width="800" height="480" alt=""/></div>

下一本书Peter Shirley-Ray Tracing The Next Week (2016) 将会从以下几个方面继续学习光追。

* Motion Blur
	
	运动模糊。跟上面的DOF一样，暴力多渲几帧，每帧用不同的位置就行。
* Bounding Volume Hierarchies

	包围盒树。加速相交检测计算的。这方面另外有一大把更好的资料。
* 贴图
	
	形状里定义好UV，然后用UV来采样贴图。当然采样方法有多种。
* Perlin Noise
	
	柏林噪声。这里重点对采样结果做了Filtering，已经简单介绍了Turb（N重噪声产生的大理石纹理）
* 光源
	
	把光源当成Emissive材质就搞定了。
* 物件的摆放
	
	之前都是写绝对坐标的，这里在形状的体系里面加了一层Transform，只要通过那个Transform把Ray变换到物体的局部空间就可以像之前一样进行相交检测了。
* 体（Volume / Participating Media）
	
	依然暴力的把Ray一点点插进Volume里面进行采样，每个采样点上取得Volume自身的颜色，并把Ray四散开去即可。
