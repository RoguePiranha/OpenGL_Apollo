/**********************************************************************
 * GL Demo
 * Just a simple program to demonstrate how to create an Open GL window, 
 * draw something on the window, and accept simple user input
 **********************************************************************/

#include "point.h"
#include "uiInteract.h"
#include "uiDraw.h"
#include "ground.h"
#include <vector>
#include <iostream>
using namespace std;


/*************************************************************************
 * Demo
 * Test structure to capture the LM that will move around the screen
 *************************************************************************/
class LMState {
    LMState(): gravity(-1.625), thrust(45000.000), weight(15103.000)  {}
private:
    double altitude;
    double position;
    double horizontalVelocity;
    double verticalVelocity;
    double horizontalAcceleration;
    double verticalAcceleration;
    double angles;
    
    double degreesFromRadians(double r);
    double radiansFromDegrees(double d);
    double totalVelocity();
public:
    const double gravity;
    const double thrust;
    const double weight;
    
    void retrieveUserInput();
    void fuelConsumption();
    void applyInertia();
    void applyGravity();
    void applyThrust();
    void updateAngle(double d);
    
};

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
    double getTimer() {
        return timer;
    }

    double getRadians() {
        return aRadians;
    }

    double getAccelerationThrust() {
        return accelerationThrust;
    }

    double getDdxThrust() {
        return ddxThrust;
    }

    double getDdyThrust() {
        return ddyThrust;
    }

    double getDdx() {
        return ddx;
    }

    double getDdy() {
        return ddy;
    }

    double getV() {
        return v;
    }

    double getDegrees() {
        return aDegrees;
    }

    // setters

    void setTimer(double t) {
        timer = t;
    }

    void setRadians(double r) {
        aRadians = r;
    }

    void setAccelerationThrust(double at) {
        accelerationThrust = at;
    }

    void setDdxThrust(double dt) {
        ddxThrust = dt;
    }

    void setDdyThrust(double dt) {
        ddyThrust = dt;
    }

    void setDdx(double d) {
        ddx = d;
    }

    void setDdy(double d) {
        ddy = d;
    }

    void setV(double vel) {
        v = vel;
    }

    void setDegrees(double d) {
        aDegrees = d;
    }

    
    double computeDistance(double s, double v, double a, double t) {
        return s + v * t + 0.5 * a * t * t;
    }

    double computeAcceleration(double f, double m) {
        return f / m;
    }
    double computeVelocity(double v, double a, double t) {
        return v + a * t;
    }
    double computeVerticalComponent(double a, double total) {
        return total * cos(a);
    }
    double computeHorizontalComponent(double a, double total) {
        return total * sin(a);
    }
    double computeTotalComponent(double x, double y) {
        return sqrt((x * x) + (y * y));
    }
    double radiansFromDegrees(double d) {
        double pi = 2 * asin(1.0);
        return d * 2 * pi / 360;
    }
};


class Demo
{
public:
   Demo(const Point& ptUpperRight) :
          angle(0.0),
          ptStar(ptUpperRight.getX() - 20.0, ptUpperRight.getY() - 20.0),
          ptLM(ptUpperRight.getX() / 2.0, ptUpperRight.getY() / 2.0),
          ptLM2(ptLM),
          ground(ptUpperRight), fuel(5000), altitude(0), speed(0)
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
   Point ptLM;           // location of the LM on the screen
   Point ptLM2;          // Location of previous LM on screen
    int ptLMx;
    int ptLMy;
   Point ptUpperRight;   // size of the screen
   double angle;         // angle the LM is pointing
   unsigned char phase;  // phase of the star's blinking
   Ground ground;
   Point ptStar;
   vector<Point> stars;
    
   int fuel;
   int altitude;
   double speed;
};

/*************************************
 * All the interesting work happens here, when
 * I get called back from OpenGL to draw a frame.
 * When I am finished drawing, then the graphics
 * engine will wait until the proper amount of
 * time has passed and put the drawing on the screen.
 **************************************/

// set the ptLMx and ptLMy to the ptLM
void setPtLM(Demo *pDemo){
    pDemo->ptLMx = pDemo->ptLM.getX();
    pDemo->ptLMy = pDemo->ptLM.getY();
    
}

double velocity(Point ptLM, Point ptLM2)
{
    double velocity = sqrt(pow(ptLM.getX() - ptLM2.getX(), 2) + pow(ptLM.getY() - ptLM2.getY(), 2));
    return velocity;
}

void callBack(const Interface *pUI, void * p)
{
   ogstream gout;

   // the first step is to cast the void pointer into a game object. This
   // is the first step of every single callback function in OpenGL. 

   Demo * pDemo = (Demo *)p;

   // move the ship around
    if (pUI->isRight()) {
        pDemo->ptLM.addX(1.0);
        pDemo->fuel-=1;
    }
       
    if (pUI->isLeft()) {
        pDemo->ptLM.addX(-1.0);
        pDemo->fuel-=1;
    }
       
    if (pUI->isUp()) {
        pDemo->ptLM.addY(-1.0);
    }
      
    if (pUI->isDown()) {
        pDemo->ptLM.addY(1.0);
        pDemo->fuel-=10;
    }
    
    
     
   // draw the ground
   pDemo->ground.draw(gout);

   // draw the lander and its flames
   gout.drawLander(pDemo->ptLM /*position*/, pDemo->angle /*angle*/);
   gout.drawLanderFlames(pDemo->ptLM, pDemo->angle, /*angle*/
                    pUI->isDown(), pUI->isLeft(), pUI->isRight());

   //Calculate the altitude
    pDemo-> altitude = pDemo -> ground.getElevation(pDemo->ptLM);
    

   //Calculate the speed
   pDemo->speed = velocity(pDemo->ptLM, pDemo->ptLM2);
    
   pDemo->ptLM2 = pDemo->ptLM;
    
   // put some text on the screen
   gout.setPosition(Point(20.0, 375.0));
   gout << "Demo (" << (int)pDemo->ptLM.getX() << ", " << (int)pDemo->ptLM.getY() << ")" << "\n";
    gout << "Fuel: " << pDemo->fuel << "\n";
    gout << "Altitude: " << pDemo-> altitude << "\n";
    gout << "Speed: " << pDemo-> speed << "\n";
    

   // draw our little star
   gout.drawStar(pDemo->ptStar, pDemo->phase++);
   
   // draw a bunch of stars
   for (int i = 0; i < pDemo->stars.size(); i++)
	   gout.drawStar(pDemo->stars[i], pDemo->phase++);

    if (pDemo->fuel <= 0) {
       gout << "Game Over" << "\n";
       cin.ignore();
       exit(0);
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
#else // !_WIN32

int main(int argc, char ** argv)
#endif // !_WIN32
{
   // Initialize OpenGL
   Point ptUpperRight(400.0, 400.0);
   Interface ui(0, NULL, 
                "Open GL Demo", 
                 ptUpperRight);

   // Initialize the game class
   Demo demo(ptUpperRight);

   // set everything into action
   ui.run(callBack, &demo);             

   return 0;
}
