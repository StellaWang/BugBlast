#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include <vector>
#include <iostream>
#include <sstream>
#include "GameWorld.h"
#include "Level.h"

class Actor;
class Player;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld()
    {
    }
    
    virtual ~StudentWorld()
    {
        for (int i = 0; i < Actors.size(); i++)
        {
            delete Actors[i];
            Actors.erase(Actors.begin() + i);
            i--;
        }
        
        Actors.clear();
        delete m_player;
    }

	virtual int move();
	virtual int init();
	virtual void cleanUp();
    
    //check if there's a brick and which kind of brick it is at location (x,y)
    int checkBricks(int x, int y);
    // Are there any living Zumi left?
    bool anyLiveZumi() const;
    // Drop a sprayer here if allowed
    void addSprayerIfPossible(int x, int y);
    // Increase the maximum simultaneous sprayers that can be dropped
    void increasePlayerSimultaneousSprayers(int x)
    {
        m_SimultaneousSprayers += x;
    }
    //check if there is any living zumi at where the player is
    bool AnyZumiAtSpecificLocation(int x, int y);
    //check if there is any bugsprayer at this position
    bool CheckBugSprayer(int x, int y);
    // Add an actor to the world
    void addActor(Actor* a);
    //Get the pointer at that position
    Actor* getPointer(int x, int y);
    // Get the player's coordinates
    void getPlayerLocation(int& x, int& y) const;

    
    // Report the value of the level data file options
    unsigned int getProbOfGoodieOverall() const;
    unsigned int getProbOfExtraLifeGoodie() const;
    unsigned int getProbOfWalkThruGoodie() const;
    unsigned int getProbOfMoreSprayersGoodie() const;
    unsigned int getGoodieLifetimeInTicks() const;
    unsigned int getWalkThruLifetimeTicks() const;
    unsigned int getMaxBoostedSprayers() const;
    unsigned int getBoostedSprayerLifetimeTicks() const;
    unsigned int getComplexZumiSearchDistance() const;

private:
    std::vector<Actor*> Actors;
    Player* m_player;
    unsigned int m_bonus;
    unsigned int m_SimultaneousSprayers;
    
    unsigned int m_ProbOfGoodieOverall;
    unsigned int m_ProbOfExtraLifeGoodie;
    unsigned int m_ProbOfWalkThruGoodie;
    unsigned int m_ProbOfMoreSprayersGoodie;
    unsigned int m_TicksPerSimpleZumiMove;
    unsigned int m_TicksPerComplexZumiMove;
    unsigned int m_GoodieLifetimeInTicks;
    unsigned int m_WalkThruLifetimeTicks;
    unsigned int m_BoostedSprayerLifetimeTicks;
    unsigned int m_MaxBoostedSprayers;
    unsigned int m_ComplexZumiSearchDistance;
};

#endif // STUDENTWORLD_H_WXY

