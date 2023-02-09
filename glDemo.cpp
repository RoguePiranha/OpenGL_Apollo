/**********************************************************************
 * GL Demo
 * Just a simple program to demonstrate how to create an Open GL window,
 * draw something on the window, and accept simple user input
 **********************************************************************/

// Created by Isaac Radford, Andrew Swayze, Samuel Casellas

#include "physics.h"
#include "point.h"
#include "uiInteract.h"
#include "uiDraw.h"
#include "ground.h"
#include <vector>
#include <iostream>
using namespace std;

const double gravity = -1.625;
const double thrust = 45000.000;
const double weight = 15103.000;
const double tTime = 0.1;

/*************************************************************************
 * Demo
 * Test structure to capture the LM that will move around the screen
 *************************************************************************/

class Demo
{
public:
    Demo(const Point &ptUpperRight) : angle(0.0),
                                      ptStar(ptUpperRight.getX() - 20.0, ptUpperRight.getY() - 20.0),
                                      ptLM(ptUpperRight.getX() - 100.0, ptUpperRight.getY() - 80.0),
                                      ptLM2(ptLM),
                                      ground(ptUpperRight), fuel(5000), altitude(0), speed(0),
                                      dx(0), dy(0),
                                      y(ptUpperRight.getY() - 80.0),
                                      x(ptUpperRight.getX() - 100.0),
                                      down(gravity)
    {
        // create a bunch of stars
        for (int i = 0; i < 49; i++)
        {
            Point pt(random(0, (int)ptUpperRight.getX()), random(0, (int)ptUpperRight.getY()));

            if (ground.getElevation(pt) > 0)
            {
                stars.push_back(pt);
            }
        };
        phase = random(0, 255);
    }

    // this is just for test purposes.  Don't make member variables public!
    Point ptLM;  // location of the LM on the screen
    Point ptLM2; // Location of previous LM on screen
    int ptLMx;
    int ptLMy;
    Point ptUpperRight;  // size of the screen
    double angle;        // angle the LM is pointing
    unsigned char phase; // phase of the star's blinking
    Ground ground;
    Point ptStar;
    vector<Point> stars;

    int fuel;
    int altitude;
    double speed;
    bool gameEnd = false;
    double dx;
    double dy;

    double x;
    double y;

    double down;

    void setDown(double d)
    {
        down = d;
    }
};

/*************************************
 * All the interesting work happens here, when
 * I get called back from OpenGL to draw a frame.
 * When I am finished drawing, then the graphics
 * engine will wait until the proper amount of
 * time has passed and put the drawing on the screen.
 **************************************/

physics *phys = new physics();

void callBack(const Interface *pUI, void *p)
{
    ogstream gout;

    // the first step is to cast the void pointer into a game object. This
    // is the first step of every single callback function in OpenGL.

    Demo *pDemo = (Demo *)p;

    //  1. Compute the net force acting on the Lunar Module: This is equal to the gravitational force acting in the downward direction, minus the force due to the thrusters acting in the upward direction.

    // Compute the horizontal and vertical components of the acceleration
    double ddxThrust = phys->computeHorizontalComponent(pDemo->angle, phys->getAccelerationThrust());
    double ddyThrust = phys->computeVerticalComponent(pDemo->angle, phys->getAccelerationThrust());
    // Compute the total acceleration
    phys->setDdx(ddxThrust);
    phys->setDdy(ddyThrust + gravity);

    // move the ship around
    if (pDemo->gameEnd == false)
    {

        if (pUI->isRight())
        {
            pDemo->fuel -= 1;
            pDemo->angle -= 0.06;
        }

        if (pUI->isLeft())
        {
            pDemo->fuel -= 1;
            pDemo->angle += 0.06;
        }

        if (pUI->isUp())
        {
            // TODO
            //             double moveSide = phys->computeHorizontalComponent(pDemo->angle, 2);
            //             double moveUp = phys->computeVerticalComponent(pDemo->angle, 2);
            //             pDemo->ptLM.addY(ddyThrust);
            //             pDemo->ptLM.addX(-ddxThrust);
            pDemo->fuel -= 10;

            //
            //  2.  Compute the acceleration of the Lunar Module: The acceleration is given by the net force divided by the mass of the Lunar Module.
            // What about when accelerating up at an angle and not straight up?
            phys->setAccelerationThrust(phys->computeAcceleration(thrust, weight));
        }
        else
        {
            phys->setAccelerationThrust(0.0);
        }

        // Compute the new velocity
        phys->dx = phys->computeVelocity(phys->dx, phys->getDdx(), tTime);
        phys->dy = phys->computeVelocity(phys->dy, phys->getDdy(), tTime);

        //    Use the velocity to update the position of the Lunar Module: The position is given by the integral of the velocity with respect to time.
        // Compute the new position
        pDemo->x = phys->computeDistance(pDemo->x, -phys->dx, phys->getDdx(), tTime);
        pDemo->y = phys->computeDistance(pDemo->y, phys->dy, phys->getDdy(), tTime);

        pDemo->ptLM.setX(pDemo->x);
        pDemo->ptLM.setY(pDemo->y);

        pDemo->speed = phys->computeTotalComponent(phys->dx, phys->dy);
    }

    // draw the ground
    pDemo->ground.draw(gout);

    // draw the lander and its flames
    gout.drawLander(pDemo->ptLM /*position*/, pDemo->angle /*angle*/);
    gout.drawLanderFlames(pDemo->ptLM, pDemo->angle, pUI->isUp(), pUI->isLeft(), pUI->isRight());

    // Calculate the altitude
    pDemo->altitude = pDemo->ground.getElevation(pDemo->ptLM);

    // put some text on the screen
    gout.setPosition(Point(20.0, 960.0));
    gout << "Fuel: " << pDemo->fuel << "\n";
    gout << "Altitude: " << pDemo->altitude << "\n";
    gout << "Speed: " << pDemo->speed << "\n";

    // draw our little star
    gout.drawStar(pDemo->ptStar, pDemo->phase++);

    // draw a bunch of stars
    for (int i = 0; i < pDemo->stars.size(); i++)
        gout.drawStar(pDemo->stars[i], pDemo->phase++);

    // Game over if you run out of fuel.
    if (pDemo->fuel <= 0)
    {
        gout.setPosition(Point(160.0, 200.0));
        gout << "Game Over"
             << "\n";
        gout.setPosition(Point(130, 170.0));
        gout << "You ran out of fuel!"
             << "\n";
        pDemo->gameEnd = true;

        pDemo->setDown(0);
    }

    if (pDemo->ground.hitGround(pDemo->ptLM, 20))
    {
        gout.setPosition(Point(160.0, 200.0));
        gout << "Game Over"
             << "\n";
        gout.setPosition(Point(152.0, 170.0));
        gout << "You crashed!"
             << "\n";
        pDemo->gameEnd = true;
        pDemo->setDown(0);
    }

    if (pDemo->ground.onPlatform(pDemo->ptLM, 20.0) && phys->dy <= 4 && phys-> dx <= 2)
    {
        gout.setPosition(Point(137.0, 300.0));
        gout << "You landed safely"
             << "\n";
        pDemo->gameEnd = true;
        pDemo->setDown(0);
    }
    else if (pDemo->ground.onPlatform(pDemo->ptLM, 20.0) && pDemo->speed > 4)
    {
        gout.setPosition(Point(160.0, 200.0));
        gout << "Game Over"
             << "\n";
        gout.setPosition(Point(130.0, 170.0));
        gout << "You crashed into the ground"
             << "\n";
        pDemo->gameEnd = true;
        pDemo->setDown(0);
    }
}

/*********************************
 * Main is pretty sparse.  Just initialize
 * my Demo type and call the display engine.
 * That is all!
 *********************************/
#ifdef _WIN32_X
#include <windows.h>
int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ PWSTR pCmdLine,
    _In_ int nCmdShow)
#else  // !_WIN32

int main(int argc, char **argv)
#endif // !_WIN32
{
    // Initialize OpenGL
    Point ptUpperRight(1000.0, 1000.0);
    Interface ui(0, NULL,
                 "Open GL Demo",
                 ptUpperRight);

    // Initialize the game class
    Demo demo(ptUpperRight);

    // set everything into action
    ui.run(callBack, &demo);

    return 0;
}
