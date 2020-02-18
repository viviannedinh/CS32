#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath) {
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath) {
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init() {
    m_numBacteria = 0;
    m_bacteriaKilled = 0;
    
    player = new Socrates(this);
    
    int l = getLevel();
    
    // create pits
    for (int i = 0; i < l; i++) {
        int randAngle = randInt(0, 360);
        int randRadius = randInt(0, 120);
        double x = randRadius * cos(randAngle) + 128;
        double y = randRadius * sin(randAngle) + 128;
        
        vector<Actor*>::iterator it;
        bool overlaps = false;
        for (it = actorsVector.begin(); it != actorsVector.end(); it++){
            if (checkOverlap(x, y, (*it)->getX(), (*it)->getY())) {
                overlaps = true;
            }
        }
        
        if (overlaps)
            i--;
        else
            actorsVector.push_back(new Pit(x, y, this));
    }
    
    // create food
    int k = min(5 * l, 25);
    for (int i = 0; i < k; i++) {
        int randAngle = randInt(0, 360);
        int randRadius = randInt(0, 120);
        double x = randRadius * cos(randAngle) + 128;
        double y = randRadius * sin(randAngle) + 128;
        
        vector<Actor*>::iterator it;
        bool overlaps = false;
        for (it = actorsVector.begin(); it != actorsVector.end(); it++){
            if (checkOverlap(x, y, (*it)->getX(), (*it)->getY())) {
                overlaps = true;
            }
        }
        
        if (overlaps)
            i--;
        else
            actorsVector.push_back(new Food(x, y, this));
    }
    
    //create dirtpiles
    int j = max(180 - 20 * l, 20);
    for (int i = 0; i < j; i++) {
        int randAngle = randInt(0, 360);
        int randRadius = randInt(0, 120);
        double x = randRadius * cos(randAngle) + 128;
        double y = randRadius * sin(randAngle) + 128;

        vector<Actor*>::iterator it;
        bool overlaps = false;
        int n = 0;
        for (it = actorsVector.begin(); n < l+k; it++, i++) {
            if (checkOverlap(x, y, (*it)->getX(), (*it)->getY())) {
                overlaps = true;
            }
        }
        
        if (overlaps)
            i--;
        else {
            DirtPile* newDirt = new DirtPile(x, y, this);
            actorsVector.push_back(newDirt);
            dirtVector.push_back(newDirt);
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    // socrates do something
    player->doSomething();
    
    // other actors do something
    vector<Actor*>::iterator it;
    it = actorsVector.begin();
    while (it != actorsVector.end()) {
        (*it)->doSomething();
        it++;
    }
    
    // delete dead actors
    it = actorsVector.begin();
    while (it != actorsVector.end()) {
        if (!(*it)->isAlive()) {
            delete *it;
            actorsVector.erase(it);
        }
        else
            it++;
    }
    
    
    
    decLives();
    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp() {
    delete player;
    player = nullptr;
    
    vector<Actor*>::iterator it;
    it = actorsVector.begin();
    while (it != actorsVector.end()) {
        delete (*it);
        it++;
    }
    actorsVector.clear();
}

Socrates* StudentWorld::getPlayer() {
    return player;
}

Actor* StudentWorld::getOverlapedActor(Actor* a) {
    vector<Actor*>::iterator it;
    it = actorsVector.begin();
    while (it != actorsVector.end()) {
        if (checkOverlap(a->getX(), a->getY(), (*it)->getX(), (*it)->getY()) && (*it)->isAlive() && (*it)->isDamageable())
            return (*it);
        else
            it++;
    }
    return nullptr;
}

bool StudentWorld::blocksMovement(Actor *a) {
    double x = a->getX(), y = a->getY();
    a->getPositionInThisDirection(a->getDirection(), 3, x, y);
    
    vector<DirtPile*>::iterator it;
    it = dirtVector.begin();
    while (it != dirtVector.end()) {
        double dist = pow(pow((*it)->getX() - x, 2) + pow((*it)->getY() - y, 2), .5);
        if (dist <= SPRITE_RADIUS  && (*it)->isAlive()) {
            return true;
        }
    }
    
    double distFromCenter = pow(pow(VIEW_WIDTH/2-x, 2) + pow(VIEW_HEIGHT/2-y, 2), .5);
    if (distFromCenter >= VIEW_RADIUS)
        return true;
    
    return false;
}

bool StudentWorld::checkOverlap(double x1, double y1, double x2, double y2) {
    double x = x1 - x2;
    double y = y1 - y2;
    double dist = sqrt(pow(x, 2) + pow(y, 2));
    
    return (dist <= 64);
}

void StudentWorld::addActor(Actor *newActor) {
    actorsVector.push_back(newActor);
}
