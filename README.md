# Peter Shirley-Ray Tracing in One Weekend (2016)

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

![](http://oo8jzybo8.bkt.clouddn.com/Chapter01_Output%20an%20image.png)

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

![](http://oo8jzybo8.bkt.clouddn.com/3.1.png)

p(t)称为点A关于t的函数。Ray tracing的本质是通过发射射线，计算像素点的颜色。在ray tracing之前需要有个摄像机，建立坐标系，显示背景色，以及ray hit的点的颜色。

假设摄像机的位置就是眼睛位置，看到的内容为ppm显示的东西，简历坐标系，z轴正方向，垂直平面向外，x向右，y向上，

![](http://oo8jzybo8.bkt.clouddn.com/3.2.png)

计算公式：

    blended_value = (1-t)*start_value + t*end_value

![](http://oo8jzybo8.bkt.clouddn.com/Screen%20Shot%202018-08-05%20at%2011.57.37%20PM.png)

## Chapter4:Adding a sphere
球的公式：
    x*x + y*y +z*z = R*R
对于任意xyz，如果满足球面公式，(x,y,z)为球面的一个点。

如果球心位置为（cx,cy,cz）,公式为
    (x-cx)*(x-cx) + (y-cy)*(y-cy) + (z-cz)*(z-cz) = R*R

用向量表示，球面点P，球心点C，半径可以表示为向量PC
    dot((p-C)(p-C)) = (x-cx)*(x-cx) + (y-cy)*(y-cy) + (z-cz)*(z-cz) 



