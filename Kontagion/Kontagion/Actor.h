#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject {
public:
    Actor(int imageID, double x, double y, Direction dir, int depth, int hitPoints, bool damageable, StudentWorld* sw);
    virtual ~Actor();
    virtual void doSomething() {}
    bool isAlive();
    void died();
    void decHP(int hp);
    void restoreHP();
    bool isDamageable();
    virtual void attacked(int hp);
    int getHP();
    StudentWorld* getWorld();
    
private:
    bool m_isAlive;
    int m_hitPoints;
    StudentWorld* m_world;
    bool m_damageable;
};

class DirtPile: public Actor {
public:
    DirtPile(double x, double y, StudentWorld* sw);
    ~DirtPile() {}
};

class Socrates: public Actor {
public:
    Socrates(StudentWorld* sw);
    ~Socrates() {}
    void doSomething();
    void attacked(int hp);
    void incFlameThrowerCharges();
private:
    int m_sprayCharges;
    int m_flameThrowerCharges;
    int m_positionAngle;
};

class Pit: public Actor {
public:
    Pit(double x, double y, StudentWorld* sw);
    ~Pit() {}
private:
    int m_regSal;
    int m_aggSal;
    int m_eColi;
};

class Food: public Actor {
public:
    Food(double x, double y, StudentWorld* sw);
    ~Food() {}
};

class Spray: public Actor {
public:
    Spray(int imageID, double x, double y, Direction dir, int travelDist, StudentWorld* sw);
    ~Spray() {}
    int getTravelDist();
    bool decTravelDist(int dist);
    void doSomething();
    virtual void attack(Actor* a) = 0;
private:
    int m_travelDistance;
};

class Flame: public Spray {
public:
    Flame(double x, double y, Direction dir, StudentWorld* sw);
    ~Flame() {}
    void attack(Actor* a);
};

class Disinfectant: public Spray {
public:
    Disinfectant(double x, double y, Direction dir, StudentWorld* sw);
    ~Disinfectant() {}
    void attack(Actor* a);
};

class Bacteria: public Actor {
public:
    Bacteria(int imageID, double x, double y, int hp, StudentWorld* sw);
    ~Bacteria() {}
    virtual void attacked(int hp);
    int getMovePlanDist();
    void setMovePlanDist(int mpd);
    int getFoodEaten();
    void setFoodEaten(int food);
private:
    int m_movePlanDist;
    int m_foodEaten;
};

class RegularSalmonella: public Bacteria {
public:
    RegularSalmonella(double x, double y, StudentWorld* sw);
    ~RegularSalmonella() {}
    void doSomething();
};

class AggressiveSalmonella: public Bacteria {
public:
    AggressiveSalmonella(double x, double y, StudentWorld* sw);
    ~AggressiveSalmonella() {}
};

class EColi: public Bacteria {
public:
    EColi(double x, double y, StudentWorld* sw);
    ~EColi() {}
    void attacked(int hp);
};

class Goodie: public Actor {
public:
    Goodie(int imageID, double x, double y, StudentWorld* sw);
    ~Goodie() {}
    bool decLifetime();
    void doSomething();
    virtual void useGoodie() = 0;
private:
    int m_lifetime;
};

class FlameThrower: public Goodie {
public:
    FlameThrower(double x, double y, StudentWorld* sw);
    ~FlameThrower() {}
    void useGoodie();
};

class RestoreHealth: public Goodie {
public:
    RestoreHealth(double x, double y, StudentWorld* sw);
    ~RestoreHealth() {}
    void useGoodie();
};

class ExtraLife: public Goodie {
public:
    ExtraLife(double x, double y, StudentWorld* sw);
    ~ExtraLife() {}
    void useGoodie();
};

class Fungus: public Goodie {
public:
    Fungus(double x, double y, StudentWorld* sw);
    ~Fungus() {}
    void useGoodie();
};

#endif // ACTOR_H_
