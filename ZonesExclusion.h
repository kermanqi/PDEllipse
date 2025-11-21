#pragma once

#include <cmath>
#include <vector>

#include "geometricalFeatures.h"
#include "EllipticalZone.h"
#include "globalConstants.h"

struct ZonesExclusion
{
    Rectangle subtractZone1;
    Rectangle subtractZone2;
    Rectangle subtractZone3;
    Rectangle subtractZone4;
    Rectangle subtractZone5;
    EllipticalZone subtractEllipse;

    /**
     * @brief Construct a new Zones Exclusion object
     * 
     * @param lengthX 
     * @param lengthY 
     */
    ZonesExclusion(double lengthX, double lengthY) 
    {
        subtractZone1 = 
        Rectangle
        (
            {0.0, 0.0}, 
            0.5*(domainX - span) - boundaryLayer, boundaryLayer
        );

        subtractZone2 = 
        Rectangle
        (
            {0.5*(domainX - span), 0.0},
            span, boundaryLayer
        );

        subtractZone3 = 
        Rectangle
        (
            {domainX - 0.5*(domainX - span) + boundaryLayer, 0.0},
            0.5*(domainX - span) - boundaryLayer, boundaryLayer
        );

        subtractZone4 = 
        Rectangle
        (
            {0.0, lengthY - boundaryLayer},
            0.5 * (lengthX - boundaryLayer), boundaryLayer
        );

        subtractZone5 = 
        Rectangle
        (
            {0.5 * lengthX + 0.5 * boundaryLayer, lengthY - boundaryLayer},
            0.5 * (lengthX - boundaryLayer), boundaryLayer
        );

        // 初始化斜椭圆：a=0.25*domainX, b=0.1*domainX, angle=30度, 中心=(0.5*domainX,0.5*domainY)
        double angleRad = 30.0 * M_PI / 180.0;
        subtractEllipse = EllipticalZone(
            Point{0.5 * domainX, 0.5 * domainY},
            0.25 * domainX,
            0.025 * domainX,
            angleRad
        );
    }

    // std::vector<Rectangle> toVector() const 
    // {
    //     return {subtractZone1, subtractZone2, subtractZone3, subtractZone4, subtractZone5};
    // }
    
    std::vector<Rectangle> toVector() const 
    {
        auto v = std::vector<Rectangle>{subtractZone1, subtractZone2, subtractZone3, subtractZone4, subtractZone5};

        // 将椭圆近似为一组小矩形并追加（用与网格一致的分辨率，避免分辨率/单位不匹配）
        double dx = gridSize;
        double dy = gridSize;
        auto ellipseRects = subtractEllipse.approximateAsAxisAlignedRectangles(dx, dy);
        v.insert(v.end(), ellipseRects.begin(), ellipseRects.end());

        return v;
    }
};