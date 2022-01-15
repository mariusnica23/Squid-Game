#ifndef USERS_H
#define USERS_H
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class User
{
protected:
    string first_name;
    string last_name;
    string city;
    unsigned weight;
    int debt;
public:
    User(string first_name, string last_name, string city, unsigned weight, int debt);
    virtual void print();
};

enum mask_type
{
    triangle,
    circle,
    rectangle
};

class Supervisor:public User
{
private:
    vector<int> comp_id;
    mask_type figure;
public:
    Supervisor(string first_name, string last_name, string city, unsigned weight, int debt, mask_type figure, vector<int> comp_id);
    virtual void print();
    void decrease_debt(int val);
    vector<int> &get_comp_id()
    {
        return comp_id;
    }
    long get_debt()
    {
        return abs(this->debt);
    }
    mask_type get_figure()
    {
        return this->figure;
    }
};

class Competitors:public User
{
    int id_number;
public:
    Competitors(string first_name, string last_name, string city, unsigned weight, int debt, int id_number);
    virtual void print() ;
    int get_id_number()
    {
        return id_number;
    }
    long get_debt()
    {
        return abs(this->debt);
    }
    void find_supervisor(vector<Supervisor*> supervisor);
    int get_weight()
    {
        return weight;
    }
};

#endif