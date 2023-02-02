//
//  gameState.cpp
//  Lab04 -Apollo-11-Visuals
//
//  Created by Samuel on 2/1/23.
//

#include "gameState.hpp"

gameState::gameState(double a) : crashed(false), landed(false),
fuel(5000.0), altitude(a), speed(0.0)  {}

    
bool gameState::getCrashed()const {
    return false;
}

bool gameState::getLanded()const {
    return true;
}
bool gameState::getFuel()const {
    return true;
}
double gameState::getAltitude()const {
    return 0.0;
}
double gameState::getSpeed()const {
    return 0.0;
}
bool gameState::setCrashed() {
    return true;
}
bool gameState::setLanded() {
    return true;
}
bool gameState::setFuel() {
    return true;
}
double gameState::setAltitude() {
    return true;
}
double gameState::setSpeed() {
    return true;
}

void gameState::checkCrashed() {
    
}
void gameState::checkLanded() {
    
}
void gameState::updateGamestate() {
    
}
void gameState::getLanded() {
    
}
void gameState::getCrashed() {
    
}
