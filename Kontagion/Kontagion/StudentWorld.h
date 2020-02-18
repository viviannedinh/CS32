#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <vector>
#include <string>

class Socrates;
class Actor;
class DirtPile;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool checkOverlap(double x1, double y1, double x2, double y2);
    bool blocksMovement(Actor* a);
    Actor* getOverlapedActor(Actor* a);
    Socrates* getPlayer();
    void addActor(Actor* newActor);
    
private:
    int m_numBacteria;
    int m_bacteriaKilled;
    Socrates* player;
    std::vector<Actor*> actorsVector;
    std::vector<DirtPile*> dirtVector;
};

#endif // STUDENTWORLD_H_
