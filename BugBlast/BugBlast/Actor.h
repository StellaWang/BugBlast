#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, StudentWorld* p)
    :GraphObject(imageID, startX, startY), m_status(true), m_world(p)
    {
        setVisible(true);
    }
    
    // Action to perform each tick
    virtual void doSomething() = 0;
    
    bool isAlive() const
    {
        return m_status;
    }
    
    void setDead()
    {
        m_status = false;
        setVisible(false);
    }
    
    StudentWorld* getWorld() const
    {
        return m_world;
    }
    
    void revealit();
    
    virtual ~Actor()
    {
        setVisible(false);
    }
    
private:
    bool m_status;
    StudentWorld* m_world;
};



class Player: public Actor
{
public:
    Player(int startX, int startY, StudentWorld* p)
    :Actor(IID_PLAYER, startX, startY, p), m_WalkThroughWallTicks(0), m_SimultaneousBugSprayerTicks(0),
    m_CanWalkThroughWalls(false), m_CanDropMoreSprayers(false)
    {}
    
    virtual void doSomething();
    
    // Turn on ability to walk through destroyable bricks
    void activateWalkThroughWalls();
    
    // Can player walk through destroyable bricks?
    bool canWalkThroughWalls() const
    {
        return m_CanWalkThroughWalls;
    }
    
    // Turn on ability to drop more sprayers
    void increaseSimultaneousSprayers();
    
    //Can player drop more sprayers?
    bool canDropMoreSprayers() const
    {
        return m_CanDropMoreSprayers;
    }
    
private:
    bool m_CanWalkThroughWalls;
    bool m_CanDropMoreSprayers;
    unsigned int m_WalkThroughWallTicks;
    unsigned int m_SimultaneousBugSprayerTicks;
};



class Brick: public Actor
{
public:
    Brick(int imageID, int startX, int startY, StudentWorld* p)
    :Actor(imageID, startX, startY, p)
    {}
    
    virtual void doSomething()
    {}
};


class PermaBrick: public Brick
{
public:
    PermaBrick(int startX, int startY, StudentWorld* p)
    :Brick(IID_PERMA_BRICK, startX, startY, p)
    {}
};



class DestroyableBrick: public Brick
{
public:
    DestroyableBrick(int startX, int startY, StudentWorld* p)
    :Brick(IID_DESTROYABLE_BRICK, startX, startY, p)
    {}
};



class Exit: public Actor
{
public:
    Exit(int startX, int startY, StudentWorld* p)
    :Actor(IID_EXIT, startX, startY, p)
    {
        setVisible(false);
    }
    
    virtual void doSomething()
    {}
    
private:
};



class TimedLifetimeActor: public Actor
{
public:
    TimedLifetimeActor(StudentWorld* p, int startX, int startY, int imageID, unsigned int lifetime)
    :Actor(imageID, startX, startY, p), m_lifetime(lifetime)
    {
    }
    virtual void doSomething() = 0;
    
    void decLifetime()
    {
        m_lifetime --;
    }
    
    int timeRemaining()
    {
        return m_lifetime;
    }
    
    // Set remining lifetime to 0
    void expireImmediately();
private:
    int m_lifetime;
};



class BugSpray: public TimedLifetimeActor
{
public:
    BugSpray(StudentWorld* p, int startX, int startY)
    :TimedLifetimeActor(p, startX, startY, IID_BUGSPRAY, 3)
    {
    }
    virtual void doSomething();
private:
};



class BugSprayer: public TimedLifetimeActor
{
public:
    BugSprayer(StudentWorld* p, int startX, int startY)
    :TimedLifetimeActor(p, startX, startY, IID_BUGSPRAYER, 40)
    {
    }
    virtual void doSomething();
private:
};



class Goodie: public TimedLifetimeActor
{
public:
    Goodie(StudentWorld* p, int startX, int startY, int imageID, int lifetime)
    :TimedLifetimeActor(p, startX, startY, imageID, lifetime)
    {
    }
private:
};



class ExtraLifeGoodie: public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* p, int startX, int startY, int lifetime)
    :Goodie(p, startX, startY, IID_EXTRA_LIFE_GOODIE, lifetime)
    {
    }
    virtual void doSomething();
private:
};



class WalkThroughWallsGoodie: public Goodie
{
public:
    WalkThroughWallsGoodie(StudentWorld* p, int startX, int startY, int lifetime)
    :Goodie(p, startX, startY, IID_WALK_THRU_GOODIE, lifetime)
    {
    }
    virtual void doSomething();
private:
};



class IncreaseSimultaneousSprayersGoodie: public Goodie
{
public:
    IncreaseSimultaneousSprayersGoodie(StudentWorld* p, int startX, int startY, int lifetime)
    :Goodie(p, startX, startY, IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE, lifetime)
    {
    }
    virtual void doSomething();
private:
};



class Zumi: public Actor
{
public:
    Zumi(int imageID, int startX, int startY, StudentWorld*p, unsigned int ticksPerMove)
    :Actor(imageID, startX, startY, p), m_ticks(ticksPerMove), m_ticksPerMove(ticksPerMove)
    {
        decideCurrentDirection();
    }
    void decTicks()
    {
        m_ticks --;
    }
    bool resetTicks();
    void decideCurrentDirection();
    int getCurrentDirection()
    {
        return m_currentDirection;
    }
    void damage(int x);
    bool willdrop(int x);
private:
    unsigned int m_ticksPerMove;
    unsigned int m_ticks;
    int m_currentDirection;
};



class SimpleZumi: public Zumi
{
public:
    SimpleZumi(int startX, int startY, StudentWorld* p, unsigned int ticksPerMove)
    :Zumi(IID_SIMPLE_ZUMI, startX, startY, p, ticksPerMove)
    {
    }
    virtual void doSomething();
private:
    
};



class ComplexZumi: public Zumi
{
public:
    ComplexZumi(int startX, int startY, StudentWorld* p, unsigned int ticksPerMove)
    :Zumi(IID_COMPLEX_ZUMI, startX, startY, p, ticksPerMove)
    {
    }
    virtual void doSomething();
    void computeDistance(int& horizontal, int& vertical);
    int breadthfirstscearch(int sr, int sc, int er, int ec);
private:
    class Coord
    {
    public:
        Coord(int rr, int cc, int direction) : m_r(rr), m_c(cc), m_direction(direction) {}
        int r() const { return m_r; }
        int c() const { return m_c; }
        int d() const { return m_direction;}
        
    private:
        int m_r;
        int m_c;
        int m_direction;
    };
};

#endif // ACTOR_H_
