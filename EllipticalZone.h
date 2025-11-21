#pragma once

#include <cmath>
#include <vector>

#include "geometricalFeatures.h" // 假定这里定义了 Point, Rectangle

struct EllipticalZone
{
    Point center;
    double a;      // 半长轴
    double b;      // 半短轴
    double angle;  // 以弧度为单位，椭圆相对于 x 轴的旋转角（逆时针为正）

    EllipticalZone() = default;
    EllipticalZone(const Point &c, double halfMajor, double halfMinor, double ang)
        : center(c), a(halfMajor), b(halfMinor), angle(ang) {}

    // 判断点是否在旋转椭圆内部（包含边界）
    bool contains(const Point &p) const
    {
        double dx = p.x - center.x;
        double dy = p.y - center.y;
        double ca = std::cos(angle);
        double sa = std::sin(angle);
        // 将点逆旋转到椭圆局部坐标（使椭圆轴与坐标轴对齐）
        double xr =  ca * dx + sa * dy;
        double yr = -sa * dx + ca * dy;
        double v = (xr*xr) / (a*a) + (yr*yr) / (b*b);
        return v <= 1.0;
    }

    // 将椭圆近似为若干个轴对齐的小矩形（分辨率由 dx, dy 控制）
    // 返回的 Rectangle 假定构造函数为 Rectangle(Point lowerLeft, double width, double height)
    std::vector<Rectangle> approximateAsAxisAlignedRectangles(double dx, double dy) const
    {
        std::vector<Rectangle> rects;
        if (dx <= 0.0 || dy <= 0.0) return rects;

        // 估算世界坐标下的轴对齐包围盒半宽/半高
        double ca = std::cos(angle);
        double sa = std::sin(angle);
        double hx = std::sqrt(a*a * ca*ca + b*b * sa*sa);
        double hy = std::sqrt(a*a * sa*sa + b*b * ca*ca);

        double minX = center.x - hx;
        double maxX = center.x + hx;
        double minY = center.y - hy;
        double maxY = center.y + hy;

        for (double x = minX; x < maxX; x += dx)
        {
            for (double y = minY; y < maxY; y += dy)
            {
                // 采样矩形中心判断是否在椭圆内
                Point sample{ x + 0.5*dx, y + 0.5*dy };
                if (contains(sample))
                {
                    rects.emplace_back(Point{x, y}, dx, dy);
                }
            }
        }
        return rects;
    }
};