//
//  physics.hpp
//  Lab04 -Apollo-11-Visuals
//
//  Created by Samuel on 1/31/23.
//

#ifndef physics_hpp
#define physics_hpp

#include <stdio.h>

class physics {
private:
    int timer = 0;
    double aRadians = 0;
    double accelerationThrust = 0;
    double ddxThrust = 0;
    double ddyThrust = 0;
    double ddx = 0;
    double ddy = 0;
    double v = 0;
    double aDegrees = 0;
    
public:
    // Getters
    double getTimer() const;
    double getRadians() const;
    double getAccelerationThrust() const;
    double getDdxThrust() const;
    double getDdyThrust() const;
    double getDdx() const;
    double getDdy() const;
    double getV() const;
    double getDegrees() const;

    // Setters
    void setTimer(double t);
    void setRadians(double r);
    void setAccelerationThrust(double at);
    void setDdxThrust(double dt);
    void setDdyThrust(double dt);
    void setDdx(double d);
    void setDdy(double d);
    void setV(double vel);
    void setDegrees(double d);
    
    double computeDistance(double s, double v, double a, double t);
    double computeAcceleration(double f, double m);
    double computeVelocity(double v, double a, double t);
    double computeVerticalComponent(double a, double total);
    double computeHorizontalComponent(double a, double total);
    double computeTotalComponent(double x, double y);
    double radiansFromDegrees(double d);
};

#endif /* physics_hpp */
