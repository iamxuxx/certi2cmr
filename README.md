# certi2cmr
标定双目模组
通过25点圆靶和正三角形靶标计算出相机的成像距离id（像素单位）。实际就是相机的水平视角。
然后计算出每个正三角形顶点的空间坐标。
计算72点点云坐标系。
在此坐标系下重构圆靶靶点坐标。
验证两相机坐标系的空间变换矩阵U
在标定景深范围内精度比较好。