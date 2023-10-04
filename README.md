### TinyRasterizer

实现一个基于CPU的软光栅渲染器。

----

### 图像操作

光栅化的过程中涉及到大量的矩阵操作以及将像素转换为图片的操作。对于矩阵运算，采用Eigen矩阵运算库，图像操作采用opencv;

```cpp
//This function is used to convert the pixels' rgb value into opencv::Mat
void write_Mat(cv::Mat& img, std::vector<Eigen::Vector3f>& frame_buf)
{
    int channels = img.channels(), rows = img.rows, cols = img.cols * channels;

    //the Mat may be stored in a one dimension formation
    if (img.isContinuous())
    {
        cols = rows * cols;
        rows = 1;
    }

    uchar* p;
    int idx = 0;
    for (int i = 0; i < rows; ++i)
    {
        p = img.ptr<uchar>(i);
        for (int j = 0; j < cols; j += 3, idx += 1)
        {
            //the value in Mat is not RGB but BGR
            p[j] = frame_buf[idx].z();
            p[j + 1] = frame_buf[idx].y();
            p[j + 2] = frame_buf[idx].x();
        }
    }
}
```

例如对frame_buf中的数值初始化为1/3 red,1/3 blue and 1/3 green

```cpp
int step = height / 3;
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int idx = i * width + j;
            if (i < step)
                frame_buf[idx] = Eigen::Vector3f(255, 0, 0);
            else if (i < 2 * step)
                frame_buf[idx] = Eigen::Vector3f(0, 255, 0);
            else
                frame_buf[idx] = Eigen::Vector3f(0, 0, 255);

        }
    }
```

得到的结果就是：

![img](./img/1.PNG)

----

### line drawing ： mid point algorithm

<img src="./img/2.jpg" title="" alt="img" width="461">

如图所示，竖直和水平分开绘制之后一共有四种不同的斜率:(0,1],(1,inf),[-1,0),(-inf,-1)

实际上需要实现的是两种斜率的绘制，一个是(0,1]，一个是(1,inf)即可，并且给定的两个点的标准都是(x0,y0)  < (x1,y1),其余两种可以直接通过轴对称的方式实现计算。

期间省略一堆数学推导推理：

直线的方程为：

$$
f(x,y) = (y_0 - y_1)x + (x_1 - x_0)y + x_0y_1 - x_1y_0= 0
$$

对于斜率(0,1]的直线，it runs more than rises，也就是需要保证每个x都有一个数值！

```python
x = x0
y = y0
d = f(x0 + 1,y0 + 0.5)
for x in (x0,x1):
    setpixel(x,y)
    if (d < 0):
        d += (x1 - x0) + (y0 - y1)
        y += 1 
    else:
        d += (y0 - y1)
```

对于斜率为(1,inf)的直线，it rises more than runs，也就是需要保证每个y都要有一个数值！

```python
x = x0
y = y0
d = f(x0 + 0.5,y0 + 1)
for y in (y0,y1):
    setpixel(x,y)
    if(d < 0):
        d += (x1 - x0)
    else:
        x += 1
        d += (x1 - x0) + (y0 - y1)
```

what’s more ,opencv的图像的y轴是向下的，最后画出来的图像是上下颠倒的，比如y=x本来是斜向上的，结果出来是斜向下的。

也就是需要将frame_buf中的第一行写到opencv的mat中的最后一行，修改一下write_mat即可。

```cpp
//This function is used to convert the pixels' rgb value into opencv::Mat
void write_Mat(cv::Mat& img, std::vector<Eigen::Vector3f> frame_buf)
{
	int channels = img.channels(), rows = img.rows, cols = img.cols * channels;
	
	//the Mat may be stored in a one dimension formation
	//if (img.isContinuous())
	//{
	//	cols = rows * cols;
	//	rows = 1;
	//}

	uchar* p;
	int idx = 0;
	for (int i = rows - 1; i >= 0; --i)
	{
		p = img.ptr<uchar>(i);
		for (int j = 0; j < cols; j += 3, idx += 1)
		{
			//the value in Mat is not RGB but BGR
			p[j] = frame_buf[idx].z();
			p[j + 1] = frame_buf[idx].y();
			p[j + 2] = frame_buf[idx].x();
		}
	}
}
```

尝试绘制一个三角形：

![img](./img/4.PNG)

有比较明显的走样。。。

----

### 模型读取






























