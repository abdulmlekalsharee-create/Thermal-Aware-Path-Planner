#ifndef POINT3D_H
#define POINT3D_H

#include <iostream>

class Point3D
{
private:
    double x;
    double y;
    double z;

public:
    Point3D();
    Point3D(double x, double y, double z);

    double getX() const;
    double getY() const;
    double getZ() const;

    void setX(double x);
    void setY(double y);
    void setZ(double z);

    double distanceTo(const Point3D& other) const;

    bool operator==(const Point3D& other) const;
    bool operator!=(const Point3D& other) const;

    Point3D operator+(const Point3D& other) const;
    Point3D operator-(const Point3D& other) const;

    friend std::ostream& operator<<(
        std::ostream& os,
        const Point3D& point
    );
};

#endif