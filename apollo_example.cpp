#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>
using namespace std;

/*******************************************************
 * LM
 * Represents both the position and velocity of the LM
*******************************************************/
class LM
{
public:
    LM(double altitude, double position,
        double verticalVelocity,
        double horizontalVelocity,
        double degrees) :
        WEIGHT(15103.0), THRUST(45000.0), GRAVITY(-1.625)
    {
        x = position;
        y = altitude;
        dx = horizontalVelocity;
        dy = verticalVelocity;
        ddx = 0.0;
        ddy = 0.0;

        updateAngle(degrees);
    }
    
    friend ostream & operator << (ostream & out, const LM & rhs);

    void updateAngle(double degrees)
    {
        angle = radiansFromDegrees(degrees);
    }

    /***************************************************
     * APPLY INERTIA
     * Add an inertia component to the current position:
     *   s = s_0 + v * t
     * Since the time is 1 second, we can simplify to:
     *  s = s_0 + v
     * This is the same as
     * s += v
     * x += dx;
     **************************************************/
    void applyInertia()
    {
        x += dx + .05 * ddx;
        y += dy + .05 * (ddy + GRAVITY);
    }

    /***************************************************
     * APPLY GRAVITY
     * Force vectors add. This means we can simply add
     * the force of gravity to the current velocity:
     * Note the time unit is 1 second.
     **************************************************/
    void applyGravity()
    {
        dy += GRAVITY;
    }

    /***************************************************
     * APPLY THRUST
     * Compute the acceleration using Newton's second law of motion:
     *  F = m * a
     * Since we need to compute acceleration, we can divide by mass:
     * a = F / m
     * Using the kinematics equation to find the new velocity:
     * v = v_0 + a * t
     * Since the time unit is 1 second:
     * v = v_0 + f / m * 1
     * v += f / m
     * Note that the thrust will be applied to both the x and y
     * according to the angle of the moon lander where:
     * 
     *      dx
     *    +---->
     *    |   /
     * dy |  / v
     *    |a/
     *    |/
     *
     **************************************************/
    void applyThrust()
    {
        ddx += sin(angle) * THRUST / WEIGHT;
        ddy += cos(angle) * THRUST / WEIGHT;
    }


private:

    double x;               // horizontal position
    double y;               // vertical position
    double dx;              // horizontal speed in meters per second
    double dy;              // vertical speed in meters per second
    double ddx;             // horizontal acceleration in meters per second
    double ddy;             // vertical acceleration in meters per second
    double angle;           // the angle of the LM with 0 pointing up (radians)
    const double WEIGHT;    // the weight of the LM in pounds
    const double THRUST;    // the thrust of the LM in pounds
    const double GRAVITY;   // the gravity on the moon in (m/s^2)

    /***************************************************
     * RADIANS FROM DEGREES and DEGREES FROM RADIANS
     * Convert degrees to radians and vice-versa
     **************************************************/
    double degreesFromRadians(double radians) const
    {
        return 360 * (radians / (2.0 * 3.14159265));
    }

    double radiansFromDegrees(double degrees) const
    {
        return (2.0 * 3.14159265) * (degrees / 360.0);
    }


    /***************************************************
     * COMPUTE TOTAL VELOCITY
     * Given the horizontal and vertical components of velocity,
     * compute the total velocity. To do this, use the pythagorean
     * theorem:
     *     a^2 + b^2 = c^2
     * Since we are solving for c, the equation becomes:
     *    c = sqrt(a^2 + b^2)
     **************************************************/
    double totalVelocity() const
    { 
        return sqrt(dx * dx + dy * dy);
    }
};

/*********************************************
 * Display a vector on the screen 
 ********************************************/
ostream & operator << (ostream & out, const LM & rhs)
{
    out.setf(ios::fixed | ios::showpoint);
    out.precision(2);
    out << "x,y: (" << rhs.x << ", " << rhs.y << ")m  "
        << "dx,dy: (" << rhs.dx << ", " << rhs.dy << ")m/s  "
        << "speed: " << rhs.totalVelocity() << "m/s  "
        << "angle: " << rhs.degreesFromRadians(rhs.angle) << "deg";
    return out;
}

/*********************************************
 * PROMPT
 * A generic prompt function
 ********************************************/
double prompt(const char * output)
{
    double input;
    cout << output;
    cin >> input;
    return input;
}



/**********************************************************
 * SIMULATION
 * Run the simulation for a specified number of seconds
 * as specified by tStart and tEnd
 *********************************************************/
void simulation(LM & lm, int tStart, int tEnd)
{
    for (int t = tStart; t < tEnd; t++)
    {
        lm.applyInertia();
        lm.applyGravity();
        lm.applyThrust();
        cout << setw(2) << t << "s - " << lm << endl;
    }
}




/**********************************************************
 * MAIN
 * Prompt for input, compute landing velocity, and display
 * the results
 *********************************************************/

int main()
{
    //prompt for input
    double vVelocity = prompt("What is your vertical velocity (m/s)? ");
    double hVelocity = prompt("What is your horizontal velocity (m/s)? ");
    double altitude = prompt("What is your altitude (m)? ");
    double angle = prompt("What is the angle of the LM where 0 is up (degrees)? ");
    double position = 0.0;

    LM lm(altitude,
          position,
          vVelocity,
          hVelocity,
          angle);

    // simulation
    cout << "\nFor the next 5 seconds with the main engine on, the position of the LM is:\n";
    simulation(lm, 1, 5);

    // now we will turn the lander
    angle = prompt("What is the new angle of the LM where 0 is up (degrees)? ");
    lm.updateAngle(angle);

    // final simulation
    cout << "\nFor the next 5 seconds with the main engine on, the position of the LM is:\n";
    simulation(lm, 6, 10);

    return 0;
}