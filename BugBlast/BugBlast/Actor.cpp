#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <queue>
#include <string>
#include <iostream>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
void Player::doSomething()
{
    if (isAlive() == false)
        return;
    
    int x = getX();
    int y = getY();
    
    if (getWorld()->AnyZumiAtSpecificLocation(x, y) == true)
    {
        setDead();
        getWorld()->playSound(SOUND_PLAYER_DIE);
        return;
    }
    
    if (canWalkThroughWalls() == false && getWorld()->checkBricks(x, y) != 0)
    {
        setDead();
        getWorld()->playSound(SOUND_PLAYER_DIE);
        return;
    }
    
    if (canWalkThroughWalls() == true)
    {
        if (m_WalkThroughWallTicks == 0)
            m_CanWalkThroughWalls = false;
        m_WalkThroughWallTicks--;
    }
    
    if (canDropMoreSprayers() == true) {
        if (m_SimultaneousBugSprayerTicks == 0)
            m_CanDropMoreSprayers = false;
        m_SimultaneousBugSprayerTicks--;
    }
    
    int ch;
    if (getWorld()->getKey(ch))
    {
        if (canWalkThroughWalls() == true)
        {
            switch (ch) {
                case KEY_PRESS_LEFT:
                    if (getWorld()->checkBricks(x-1, y) != -1)
                        moveTo(x-1, y);
                    break;
                case KEY_PRESS_RIGHT:
                    if (getWorld()->checkBricks(x+1, y) != -1)
                        moveTo(x+1, y);
                    break;
                case KEY_PRESS_UP:
                    if (getWorld()->checkBricks(x, y+1) != -1)
                        moveTo(x, y+1);
                    break;
                case KEY_PRESS_DOWN:
                    if (getWorld()->checkBricks(x, y-1) != -1)
                        moveTo(x, y-1);
                    break;
                case KEY_PRESS_SPACE:
                    getWorld()->addSprayerIfPossible(x, y);
                    break;
                default: 
                    break;
            }

        }
        else
        {
            switch (ch) {
                case KEY_PRESS_LEFT:
                    if (getWorld()->checkBricks(x-1, y) == 0)
                        moveTo(x-1, y);
                    break;
                case KEY_PRESS_RIGHT:
                    if (getWorld()->checkBricks(x+1, y) == 0)
                        moveTo(x+1, y);
                    break;
                case KEY_PRESS_UP:
                    if (getWorld()->checkBricks(x, y+1) == 0)
                        moveTo(x, y+1);
                    break;
                case KEY_PRESS_DOWN:
                    if (getWorld()->checkBricks(x, y-1) == 0)
                        moveTo(x, y-1);
                    break;
                case KEY_PRESS_SPACE:
                    getWorld()->addSprayerIfPossible(x, y);
                    break;
                default:
                    break;
            }
        }
    }
}

void Player::activateWalkThroughWalls()
{
    m_CanWalkThroughWalls = true;
    m_WalkThroughWallTicks = getWorld()->getWalkThruLifetimeTicks();
}

void Player::increaseSimultaneousSprayers()
{
    m_CanDropMoreSprayers = true;
    int n = getWorld()->getMaxBoostedSprayers();
    getWorld()->increasePlayerSimultaneousSprayers(n-2);
    m_SimultaneousBugSprayerTicks = getWorld()->getBoostedSprayerLifetimeTicks();
}

void Actor::revealit()
{
    setVisible(true);
    getWorld()->playSound(SOUND_REVEAL_EXIT);
}

void BugSprayer::doSomething()
{
    if (isAlive() == false)
        return;
    decLifetime();
    if (timeRemaining() <= 0)
    {
        setDead();
        int BX = getX();
        int BY = getY();
        Actor* BSpray = new BugSpray(getWorld(), BX, BY);
        getWorld()->addActor(BSpray);
        
        if (getWorld()->checkBricks(BX+1,BY) != -1)
        {
            Actor* BSpray = new BugSpray(getWorld(), BX+1, BY);
            getWorld()->addActor(BSpray);
            if (getWorld()->checkBricks(BX+1,BY) == 0 && getWorld()->checkBricks(BX+2,BY) != -1)
            {
                Actor* BSpray = new BugSpray(getWorld(), BX+2, BY);
                getWorld()->addActor(BSpray);
            }
        }
        if (getWorld()->checkBricks(BX-1,BY) != -1)
        {
            Actor* BSpray = new BugSpray(getWorld(), BX-1, BY);
            getWorld()->addActor(BSpray);
            if (getWorld()->checkBricks(BX-1,BY) == 0 && getWorld()->checkBricks(BX-2,BY) != -1)
            {
                Actor* BSpray = new BugSpray(getWorld(), BX-2, BY);
                getWorld()->addActor(BSpray);
            }
        }
        if (getWorld()->checkBricks(BX,BY+1) != -1)
        {
            Actor* BSpray = new BugSpray(getWorld(), BX, BY+1);
            getWorld()->addActor(BSpray);
            if (getWorld()->checkBricks(BX,BY+1) == 0 && getWorld()->checkBricks(BX,BY+2) != -1)
            {
                Actor* BSpray = new BugSpray(getWorld(), BX, BY+2);
                getWorld()->addActor(BSpray);
            }
        }
        if (getWorld()->checkBricks(BX,BY-1) != -1)
        {
            Actor* BSpray = new BugSpray(getWorld(), BX, BY-1);
            getWorld()->addActor(BSpray);
            if (getWorld()->checkBricks(BX,BY-1) == 0 && getWorld()->checkBricks(BX,BY-2) != -1)
            {
                Actor* BSpray = new BugSpray(getWorld(), BX, BY-2);
                getWorld()->addActor(BSpray);
            }
        }
        getWorld()->increasePlayerSimultaneousSprayers(1);
        getWorld()->playSound(SOUND_SPRAY);
    }
}

void BugSpray::doSomething()
{
    if (isAlive() == false)
        return;
    decLifetime();
    if (timeRemaining() <= 0)
    {
        setDead();
        int BX = getX();
        int BY = getY();
        if (getWorld()->checkBricks(BX, BY) == -2)
        {
            Actor* DB = getWorld()->getPointer(BX, BY);
            DB->setDead();
            return;
        }
        
        Actor* Z = getWorld()->getPointer(BX, BY);
        Player* P = dynamic_cast<Player*>(Z);
        SimpleZumi* SZ = dynamic_cast<SimpleZumi*>(Z);
        ComplexZumi* CZ = dynamic_cast<ComplexZumi*>(Z);
        BugSprayer* B = dynamic_cast<BugSprayer*>(Z);
        
        if (P != nullptr)
        {
            P->setDead();
            P->getWorld()->playSound(SOUND_PLAYER_DIE);
        }

        if (SZ != nullptr)
        {
            SZ->damage(100);
        }
        
        if (CZ != nullptr)
        {
            CZ->damage(500);
        }
        
        if (B != nullptr)
        {
            B->expireImmediately();
        }
    }
}

void ExtraLifeGoodie::doSomething()
{
    if (isAlive() == false)
        return;
    decLifetime();
    if (timeRemaining() == 0)
        setDead();

    int x = getX();
    int y = getY();
    int px, py;
    getWorld()->getPlayerLocation(px, py);
    if (x == px && y == py)
    {
        getWorld()->incLives();
        getWorld()->increaseScore(1000);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}

void WalkThroughWallsGoodie::doSomething()
{
    if (isAlive() == false)
        return;
    decLifetime();
    if (timeRemaining() == 0)
        setDead();
    
    int x = getX();
    int y = getY();
    int px, py;
    getWorld()->getPlayerLocation(px, py);
    if (x == px && y == py)
    {
        Actor* P = getWorld()->getPointer(px, py);
        Player* p = dynamic_cast<Player*>(P);
        p->activateWalkThroughWalls();
        getWorld()->increaseScore(1000);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}

void IncreaseSimultaneousSprayersGoodie::doSomething()
{
    if (isAlive() == false)
        return;
    decLifetime();
    if (timeRemaining() == 0)
        setDead();

    int x = getX();
    int y = getY();
    int px, py;
    getWorld()->getPlayerLocation(px, py);
    if (x == px && y == py)
    {
        Actor* P = getWorld()->getPointer(px, py);
        Player* p = dynamic_cast<Player*>(P);
        p->increaseSimultaneousSprayers();
        getWorld()->increaseScore(1000);
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
}

bool Zumi::resetTicks()
{
    if (m_ticks == 0)
    {
        m_ticks = m_ticksPerMove;
        return true;
    }
    return false;
}

void Zumi::decideCurrentDirection()
{
    m_currentDirection = rand() %4;
}

void Zumi::damage(int x)
{
    setDead();
    getWorld()->playSound(SOUND_ENEMY_DIE);
    getWorld()->increaseScore(x);
    if (willdrop(getWorld()->getProbOfGoodieOverall()) == true)
    {
        int x = getX();
        int y = getY();
        int prob1 = getWorld()->getProbOfExtraLifeGoodie();
        int prob2 = prob1 + getWorld()->getProbOfWalkThruGoodie();
        int n = rand() %100;
        
        if (0 <= n && n < prob1)
        {
            Actor* ExtraLife = new ExtraLifeGoodie(getWorld(), x, y,getWorld()-> getGoodieLifetimeInTicks());
            getWorld()->addActor(ExtraLife);
        }
        else if (prob1 <= n && n < prob2)
        {
            Actor* WalkThru = new WalkThroughWallsGoodie(getWorld(), x, y,getWorld()->getGoodieLifetimeInTicks());
            getWorld()->addActor(WalkThru);
        }
        else
        {
            Actor* MoreSprayers = new IncreaseSimultaneousSprayersGoodie(getWorld(), x, y,getWorld()->getGoodieLifetimeInTicks());
            getWorld()->addActor(MoreSprayers);
        }
    }
}

bool Zumi::willdrop(int x)
{
    int number = rand()%100;
    if (number < x) {
        return true;
    }
    return false;
}

void SimpleZumi::doSomething()
{
    if (isAlive() == false)
        return;
    decTicks();
    if (resetTicks() == true)
    {
        int direction = getCurrentDirection();
        int x = getX();
        int y = getY();
        switch (direction)
        {
            case 0:             //move up
            {
                if (getWorld()->checkBricks(x,y+1) == 0 && getWorld()->CheckBugSprayer(x,y+1) == false)
                    moveTo(x, y+1);
                else
                    decideCurrentDirection();
                break;
            }
            case 1:             //move down
            {
                if (getWorld()->checkBricks(x,y-1) == 0 && getWorld()->CheckBugSprayer(x,y-1) == false)
                    moveTo(x, y-1);
                else
                    decideCurrentDirection();
                break;
            }
            case 2:             //move left
            {
                if (getWorld()->checkBricks(x-1,y) == 0 && getWorld()->CheckBugSprayer(x-1,y) == false)
                    moveTo(x-1, y);
                else
                    decideCurrentDirection();
                break;
            }
            case 3:             //move right
            {
                if (getWorld()->checkBricks(x+1,y) == 0 && getWorld()->CheckBugSprayer(x+1,y) == false)
                    moveTo(x+1, y);
                else
                    decideCurrentDirection();
                break;
            }
            default:
                break;
        }
    }
}

void ComplexZumi::doSomething()
{
    if (isAlive() == false)
        return;
    decTicks();
    if (resetTicks() == true)
    {
        int smellDistance = getWorld()->getComplexZumiSearchDistance();
        int horizontal = getX();
        int vertical = getY();
        computeDistance(horizontal, vertical);
        if (horizontal <= smellDistance && vertical <= smellDistance)
        {
            int x = getX();
            int y = getY();
            int px, py;
            getWorld()->getPlayerLocation(px, py);
            int dir = breadthfirstscearch(px, py, x, y);
            if(dir != -1)
            {
                switch (dir) {
                    case 1:
                        moveTo(x+1, y);
                        break;
                    case 2:
                        moveTo(x, y-1);
                        break;
                    case 3:
                        moveTo(x-1, y);
                        break;
                    case 4:
                        moveTo(x, y+1);
                        break;
                    default:
                        break;
                }
                return;
            }
        }
        
        int direction = getCurrentDirection();
        int x = getX();
        int y = getY();
        switch (direction)
        {
            case 0:             //move up
            {
                if (getWorld()->checkBricks(x,y+1) == 0 && getWorld()->CheckBugSprayer(x,y+1) == false)
                    moveTo(x, y+1);
                else
                    decideCurrentDirection();
                break;
            }
            case 1:             //move down
            {
                if (getWorld()->checkBricks(x,y-1) == 0 && getWorld()->CheckBugSprayer(x,y-1) == false)
                    moveTo(x, y-1);
                else
                    decideCurrentDirection();
                break;
            }
            case 2:             //move left
            {
                if (getWorld()->checkBricks(x-1,y) == 0 && getWorld()->CheckBugSprayer(x-1,y) == false)
                    moveTo(x-1, y);
                else
                    decideCurrentDirection();
                break;
            }
            case 3:             //move right
            {
                if (getWorld()->checkBricks(x+1,y) == 0 && getWorld()->CheckBugSprayer(x+1,y) == false)
                    moveTo(x+1, y);
                else
                    decideCurrentDirection();
                break;
            }
            default:
                break;
        }
    }
}

void ComplexZumi::computeDistance(int& horizontal, int& vertical)
{
    int px, py;
    getWorld()->getPlayerLocation(px, py);
    horizontal = abs(px - horizontal);
    vertical = abs(py - vertical);
}

int ComplexZumi::breadthfirstscearch(int sr, int sc, int er, int ec)
{
    char maze[VIEW_HEIGHT][VIEW_WIDTH];
    for (int i = 0; i < VIEW_HEIGHT; i++)
    {
        for (int j = 0; j < VIEW_WIDTH; j++)
        {
            if (getWorld()->checkBricks(j, i) != 0 || getWorld()->CheckBugSprayer(j, i) == true)
                maze[i][j] = 'X';
            else
                maze[i][j] = '.';
        }
    }
    
        queue<Coord> CoordQueue;       //decalre a stack of Coords
        
        Coord start(sr,sc, 0);
        CoordQueue.push(start);
        maze[sr][sc] = '*';
    
        while (!CoordQueue.empty())
        {
            Coord current = CoordQueue.front();
            CoordQueue.pop();
            
            int row = current.r();
            int col = current.c();
            int direction = current.d();
            if (row == er && col == ec)
                return direction;
            if (maze[row-1][col] == '.')
            {
                Coord update(row-1, col, 1);
                CoordQueue.push(update);
                maze[row-1][col] = '*';
            }
            if (maze[row][col+1] == '.')
            {
                Coord update(row, col+1, 2);
                CoordQueue.push(update);
                maze[row][col+1] = '*';
            }
            if (maze[row+1][col] == '.')
            {
                Coord update(row+1,col, 3);
                CoordQueue.push(update);
                maze[row+1][col] = '*';
            }
            if (maze[row][col-1] == '.')
            {
                Coord update(row, col-1, 4);
                CoordQueue.push(update);
                maze[row][col-1] = '*';
            }
        }
        return -1;
}

void TimedLifetimeActor::expireImmediately()
{
    m_lifetime = 0;
}
