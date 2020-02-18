#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <vector>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////////
// Actor Implemetation //
////////////////////////////////////////////

Actor::Actor(int imageID, double x, double y, Direction dir, int depth, int hitPoints, bool damageable, StudentWorld* sw)
: GraphObject(imageID, x, y, dir, depth) {
    m_isAlive = true;
    m_hitPoints = hitPoints;
    m_world = sw;
    m_damageable = damageable;
}

Actor::~Actor() {}

bool Actor::isAlive() {
    return m_isAlive;
}

int Actor::getHP() {
    return m_hitPoints;
}

void Actor::died() {
    m_isAlive = false;
}

bool Actor::isDamageable() {
    return m_damageable;
}

void Actor::attacked(int hp) {
    died();
}

void Actor::restoreHP() {
    m_hitPoints = 100;
}

void Actor::decHP(int hp) {
    m_hitPoints -= hp;
}

StudentWorld* Actor::getWorld() {
    return m_world;
}

/////////////////////////////////////////////////
// Socrates Implemetation //
/////////////////////////////////////////////////

Socrates::Socrates(StudentWorld* sw)
: Actor(IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, 100, false, sw) {
    m_sprayCharges = 20;
    m_flameThrowerCharges = 5;
    m_positionAngle = 180;
}

void Socrates::doSomething() {
    if (getHP() <= 0)
        return;
    
    int pk;
    if (getWorld()->getKey(pk)) {
        switch (pk) {
            case KEY_PRESS_SPACE: {
                if (m_sprayCharges >= 1) {
                    double x = 2 * SPRITE_RADIUS * cos(getDirection()) + getX();
                    double y = 2 * SPRITE_RADIUS * sin(getDirection()) + getY();;
                    getWorld()->addActor(new Disinfectant(x, y, getDirection(), getWorld()));
                    m_sprayCharges--;
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                }
                break;
            }
            case KEY_PRESS_ENTER: {
                if (m_flameThrowerCharges >= 1) {
                    for (int i = 0, k = 0; i < 16; i++, k += 22) {
                        Direction d = getDirection() + k;
                        double x = 2 * SPRITE_RADIUS * cos(d) + getX();
                        double y = 2 * SPRITE_RADIUS * sin(d) + getY();
                        getWorld()->addActor(new Flame(x, y, d, getWorld()));
                    }
                    m_flameThrowerCharges--;
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                break;
            }
            case KEY_PRESS_LEFT: {
                m_positionAngle -= 5;
                if (m_positionAngle == -5)
                    m_positionAngle += 360;
                int x = 128 * cos(m_positionAngle) + 128;
                int y = 128 * sin(m_positionAngle) + 128;
                moveTo(x, y);
                setDirection(m_positionAngle + 180);
                break;
            }
            case KEY_PRESS_RIGHT: {
                m_positionAngle += 5;
                if (m_positionAngle == 365)
                    m_positionAngle -= 360;
                int x = 128 * cos(m_positionAngle);
                int y = 128 * sin(m_positionAngle);
                moveTo(x, y);
                setDirection(m_positionAngle+180);
                break;
            }
        }
    }
    else
        if (m_sprayCharges < 20)
            m_sprayCharges++;
}

void Socrates::attacked(int hp) {
    decHP(hp);
    if (getHP() <= 0) {
        died();
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
    else
        getWorld()->playSound(SOUND_PLAYER_HURT);
}

void Socrates::incFlameThrowerCharges() {
    m_flameThrowerCharges += 5;
}

/////////////////////////////////////////////////
// DirtPile Implemetation //
/////////////////////////////////////////////////

DirtPile::DirtPile(double x, double y, StudentWorld* sw)
: Actor(IID_DIRT, x, y, 90, 1, 0, true, sw) {}

////////////////////////////////////////
// Pit Implemetation //
////////////////////////////////////////

Pit::Pit(double x, double y, StudentWorld* sw)
: Actor(IID_PIT, x, y, 0, 1, 0, false, sw) {
    m_regSal = 5;
    m_aggSal = 3;
    m_eColi = 2;
}

//////////////////////////////////////////
// Food Implemetation //
//////////////////////////////////////////

Food::Food(double x, double y, StudentWorld *sw)
: Actor(IID_FOOD, x, y, 90, 1, 0, false, sw) {}

////////////////////////////////////////////
// Spray Implemetation //
////////////////////////////////////////////

Spray::Spray(int imageID, double x, double y, Direction dir, int travelDist, StudentWorld* sw)
: Actor(imageID, x, y, dir, 1, 0, false, sw) {
    m_travelDistance = travelDist;
}

int Spray::getTravelDist() {
    return m_travelDistance;
}

bool Spray::decTravelDist(int dist) {
    m_travelDistance -= dist;
    return (m_travelDistance <= 0);
}

void Spray::doSomething() {
    if (!isAlive())
        return;
    
    Actor* a = getWorld()->getOverlapedActor(this);
    if (a != nullptr) {
        attack(a);
        died();
        return;
    }
    else {
        double x = 2 * SPRITE_RADIUS * cos(getDirection()) + getX();
        double y = 2 * SPRITE_RADIUS * sin(getDirection()) + getY();
        moveTo(x,y);
        if (decTravelDist(2 * SPRITE_RADIUS))
            died();
    }
}

////////////////////////////////////////////
// Flame Implemetation //
////////////////////////////////////////////

Flame::Flame(double x, double y, Direction dir, StudentWorld* sw)
: Spray(IID_FLAME, x, y, dir, 32, sw) {}

void Flame::attack(Actor* a) {
    a->attacked(5);
}

/////////////////////////////////////////////////////////
// Disinfectant Implemetation //
/////////////////////////////////////////////////////////

Disinfectant::Disinfectant(double x, double y, Direction dir, StudentWorld* sw)
: Spray(IID_SPRAY, x, y, dir, 112, sw) {}

void Disinfectant::attack(Actor* a) {
    a->attacked(2);
}

/////////////////////////////////////////////////
// Bacteria Implemetation //
/////////////////////////////////////////////////

Bacteria::Bacteria(int imageID, double x, double y, int hp, StudentWorld* sw)
: Actor(imageID, x, y, 90, 0, hp, true, sw) {
    m_movePlanDist = 0;
    m_foodEaten = 0;
}

void Bacteria::attacked(int hp) {
    decHP(hp);
    if (getHP() > 0)
        getWorld()->playSound(SOUND_SALMONELLA_HURT);
    else {
        died();
        getWorld()->playSound(SOUND_SALMONELLA_DIE);
        getWorld()->increaseScore(100);
        int rn = randInt(1, 2);
        if (rn == 1) {
            getWorld()->addActor(new Food(getX(), getY(), getWorld()));
        }
    }
}

int Bacteria::getMovePlanDist() {
    return m_movePlanDist;
}

void Bacteria::setMovePlanDist(int mpd) {
    m_movePlanDist = mpd;
}

int Bacteria::getFoodEaten() {
    return m_foodEaten;
}

void Bacteria::setFoodEaten(int food) {
    m_foodEaten = food;
}

////////////////////////////////////////////////////////////////////
// Regular Salmonella Implemetation //
////////////////////////////////////////////////////////////////////

RegularSalmonella::RegularSalmonella(double x, double y, StudentWorld* sw)
: Bacteria(IID_SALMONELLA, x, y, 4, sw) {}

void RegularSalmonella::doSomething() {
    if (!isAlive())         // 1
        return;
    
    Socrates* soc = getWorld()->getPlayer();        // 2
    if (soc->isAlive() && getWorld()->checkOverlap(getX(), getY(), soc->getX(), soc->getY()))
        soc->attacked(1);
    else if (getFoodEaten() >= 3) {                 // 3
        double newx = getX(), newy = getY();
        if (newx < VIEW_WIDTH/2)
            newx += SPRITE_RADIUS;
        else if (newx > VIEW_WIDTH/2)
            newx -= SPRITE_RADIUS;
        
        if (newy < VIEW_HEIGHT/2)
            newy += SPRITE_RADIUS;
        else if (newy > VIEW_HEIGHT/2)
            newy -= SPRITE_RADIUS;
        
        getWorld()->addActor(new RegularSalmonella(newx, newy, getWorld()));
        
        setFoodEaten(0);
    }
//    else if () {                                // 4
//
//    }
    
    
    if (getMovePlanDist() > 0) {                // 5
        setMovePlanDist(getMovePlanDist() - 1);
        if (!getWorld()->blocksMovement(this))
            moveAngle(getDirection(), 3);
        else {
            setDirection(randInt(0, 359));
            setMovePlanDist(10);
            return;
        }
    }
//    else {                                      // 6
//
//    }
        
}

/////////////////////////////////////////////////////////////////////////
// Aggressive Salmonella Implemetation //
/////////////////////////////////////////////////////////////////////////

AggressiveSalmonella::AggressiveSalmonella(double x, double y, StudentWorld* sw)
: Bacteria(IID_SALMONELLA, x, y, 10, sw) {}

////////////////////////////////////////////
// EColi Implemetation //
////////////////////////////////////////////

EColi::EColi(double x, double y, StudentWorld* sw)
: Bacteria(IID_ECOLI, x, y, 5, sw) {}

void EColi::attacked(int hp) {
    decHP(hp);
    if (getHP() > 0)
        getWorld()->playSound(SOUND_ECOLI_HURT);
    else {
        died();
        getWorld()->playSound(SOUND_ECOLI_DIE);
        getWorld()->increaseScore(100);
        int rn = randInt(1, 2);
        if (rn == 1) {
            getWorld()->addActor(new Food(getX(), getY(), getWorld()));
        }
    }
}

//////////////////////////////////////-//////
// Goodie Implemetation //
///////////////////////////////////////-/////

Goodie::Goodie(int imageID, double x, double y, StudentWorld* sw)
: Actor(imageID, x, y, 0, 1, 0, true, sw) {
    m_lifetime = std::max(rand() % (300 - 10 * sw->getLevel()), 50);
}

bool Goodie::decLifetime() {
    m_lifetime--;
    return (m_lifetime <= 0);
}

void Goodie::doSomething() {
    if (!isAlive())
        return;
    
    Socrates* soc = getWorld()->getPlayer();
    if (soc->isAlive() && getWorld()->checkOverlap(getX(), getY(), soc->getX(), soc->getY())) {
        useGoodie();
        died();
        return;
    }
    else
        if (decLifetime())
            died();
}

//////////////////////////////////////////////////////////
// Flame Thrower Implemetation //
//////////////////////////////////////////////////////////

FlameThrower::FlameThrower(double x, double y, StudentWorld* sw)
: Goodie(IID_FLAME_THROWER_GOODIE, x, y, sw) {}

void FlameThrower::useGoodie() {
    getWorld()->getPlayer()->incFlameThrowerCharges();
    getWorld()->increaseScore(300);
    getWorld()->playSound(SOUND_GOT_GOODIE);
}

////////////////////////////////////////////////////////////
// Restore Health Implemetation //
////////////////////////////////////////////////////////////

RestoreHealth::RestoreHealth(double x, double y, StudentWorld* sw)
: Goodie(IID_RESTORE_HEALTH_GOODIE, x, y, sw) {}

void RestoreHealth::useGoodie() {
    getWorld()->getPlayer()->restoreHP();
    getWorld()->increaseScore(250);
    getWorld()->playSound(SOUND_GOT_GOODIE);
}

/////////////////////////////////////////////////////
// Extra Life Implemetation //
/////////////////////////////////////////////////////

ExtraLife::ExtraLife(double x, double y, StudentWorld* sw)
: Goodie(IID_EXTRA_LIFE_GOODIE, x, y, sw) {}

void ExtraLife::useGoodie() {
    getWorld()->incLives();
    getWorld()->increaseScore(500);
    getWorld()->playSound(SOUND_GOT_GOODIE);
}

/////////////////////////////////////////////
// Fungus Implemetation //
/////////////////////////////////////////////

Fungus::Fungus(double x, double y, StudentWorld* sw)
: Goodie(IID_FUNGUS, x, y, sw) {}

void Fungus::useGoodie() {
    getWorld()->increaseScore(-50);
    getWorld()->getPlayer()->attacked(20);
}
