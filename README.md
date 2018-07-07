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

![]()

## Chapter2:The vec3 class
用于几何向量计算和颜色计算，
