#ifndef GAMES_H
#define GAMES_H
#include <iostream>
#include <string>
#include <vector>
#include "Users.h"

using namespace std;

class Games
{

public:
    virtual void play(vector<Competitors*>&, vector<Supervisor*>)=0;
    int get_debt_by_id(vector<Competitors*>, int);
    int get_weight(vector<Competitors*>);
};

class Game_1:public Games

{
private:
    
public:
    void play(vector<Competitors*>&, vector<Supervisor*>) override;

};


class Game_2:public Games

{
private:
    
public:
    void play(vector<Competitors*>&, vector<Supervisor*>) override;

};


class Game_3:public Games

{
private:
    
public:
    void play(vector<Competitors*>&, vector<Supervisor*>) override;

};


class Game_4:public Games

{
private:
    
public:
    void play(vector<Competitors*>&, vector<Supervisor*>) override;

};

#endif 