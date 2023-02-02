//
//  gameState.hpp
//  Lab04 -Apollo-11-Visuals
//
//  Created by Samuel on 2/1/23.
//

#ifndef gameState_hpp
#define gameState_hpp

#include <stdio.h>

class gameState {
    
private:
    bool crashed;
    bool landed;
    bool fuel;
    double altitude;
    double speed;
public:
    gameState(double a);
    
    bool getCrashed()const;
    bool getLanded()const;
    bool getFuel()const;
    double getAltitude()const;
    double getSpeed()const;
    bool setCrashed();
    bool setLanded();
    bool setFuel();
    double setAltitude();
    double setSpeed();
    
    void checkCrashed();
    void checkLanded();
    void updateGamestate();
    void getLanded();
    void getCrashed();
};

#endif /* director_hpp */
