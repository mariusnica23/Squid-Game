//Partea de final imi da cateva erori, nu inteleg unde si de ce, insa 90% este functional si stabil(voi incerca sa il fac rulabil pana luni)
//Am facut 2 headere in care am clasele pentru partea de utilizatori si in al doilea header am clasele pentru jocuri
//Am incercat sa formez random din 2 fisiere text(unul cu peste 100 de nume, iar altul peste 100 de orase)concurentii si supraveghetorii
//pentru a fi cat mai diferit totul la fiecare rulare si cat mai dinamic
#include <iostream>
#include "Users.h"
#include "Games.h"
#include <time.h>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

int S=0;

User::User(string first_name, string last_name, string city, unsigned weight, int debt)
{
    this->first_name=first_name;
    this->last_name=last_name;
    this->city=city;
    this->weight=weight;
    this->debt=debt;

}

Competitors::Competitors(string first_name, string last_name, string city, unsigned weight, int debt, int id_number):User(first_name, last_name, city, weight, debt)
{
    this->id_number=id_number;

}

Supervisor::Supervisor(string first_name, string last_name, string city, unsigned weight, int debt, mask_type figure, vector<int> comp_id):User(first_name, last_name, city, weight, debt)
{
    this->figure=figure;
    this->comp_id=comp_id;
}

vector<string> read_txt(string path)
{
    ifstream f(path);
    vector<string> lines;
    string line;
    for(int i=0; i<108; i++)
    {
        f>>line;
        lines.push_back(line);
    }
    f.close();
    return lines;
}

void User::print()
{
    cout<<first_name<<" "<<last_name<<" "<<city<<" "<<weight<<" "<<abs(debt)<<" ";
}
void Competitors::print()
{
    User::print();
    cout<<id_number<<"\n";
}
//Functie pentru a gasi supraveghetorul aferent concurentului
void Competitors::find_supervisor(vector<Supervisor*> supervisor)
{
    try
    {
        for(auto x:supervisor)
        {
            for(auto y:x->get_comp_id())
                if(y==id_number)
                    cout<<x->get_debt();
        }
        throw "Current player don't have a supervisor";
    }
    catch(string e)
    {
        cout<<e<<"\n";
    }

}
//Functie pentru a retine castigurile supraveghetorilor
void Supervisor::decrease_debt(int val)
{
    debt=-debt+val;
}
void Supervisor::print()
{
    User::print();
    switch (figure)
    {
        case triangle:
            cout<<"triangle"<<"\n";
            break;
        case rectangle:
            cout<<"rectangle"<<"\n";
            break;
        case circle:
            cout<<"circle"<<"\n";
            break;
        default:
            break;
    }

    for(auto x:comp_id)
    {
        cout<<x<<" ";
    }
}
//O functie de acces la datoriile acestora pentru a le putea modifica in timpul sau dupa terminarea jocurilor
int Games::get_debt_by_id(vector<Competitors*> competitor, int id)
{
    for(auto x:competitor)
    {
        if(x->get_id_number()==id)
            return x->get_debt();
    }
    return 0;
}
//Functie pentru stergerea jucatorilor eliminati in functie de Id-ul acestora
void delete_comp_by_id(vector<Competitors*> &competitor, int id)
{
    for(auto itr=competitor.begin(); itr !=competitor.end();)
    {
        if((*itr)->get_id_number()==id)
            itr=competitor.erase(itr);
        else
            itr++;

    }
}

void Game_1::play(vector<Competitors*> &competitor, vector<Supervisor*> supervisor)
{   // delete from supervisor
    for(auto x:supervisor)
    {   vector<int> &comp=x->get_comp_id();
        for(auto itr=comp.begin(); itr !=comp.end();)
        {   // daca are id-ul de eliminat il elimin
            if((*itr)%2==0)
            {
                int debt=get_debt_by_id(competitor, *itr);
                S+=debt;
                x->decrease_debt(debt);
                delete_comp_by_id(competitor, *itr);
                itr=comp.erase(itr);

            }
            else
            {
                itr++;
            }
        }
    }

}
int Games::get_weight(vector<Competitors*> competitor)
{
    for(auto x:competitor)
    {
        int weight=x->get_weight();
        return weight;
    }
    return 0;
}
void Game_2::play(vector<Competitors*> &competitor, vector<Supervisor*> supervisor)
{
    map<int,vector<Competitors *>> teams;
    map<int,int> team_weights;

    // map : echipa -> lista de competitori
    // pregatesc pentru fiecare echipa lista goala de competitori
    for(int i = 1; i <=4; i++) {
        teams.insert(pair<int, vector<Competitors *> >(i, vector<Competitors *>()));
        team_weights[i] = 0;
    }


    // ultimii competitor.size() % 4 nu intra in echipa deci sar peste runda
    int competitors_that_skip_round = competitor.size() % 4;

    // aici adun pentru fiecare echipa weighturile
    // am pornit de la 1 ca sa se potriveasca cu grupele
    for(int i = 1; i <= competitor.size() - competitors_that_skip_round; i++) 
    {
        teams[i % 4+1].push_back(competitor[i]);
        team_weights[i % 4+1] += competitor[i]->get_weight();
    }
    // analizam care echipe sunt eliminate 
    bool already_eliminated[4] = {false,false,false,false};

                if(team_weights[0] < team_weights[1])
                    already_eliminated[0] = true;
                else
                    already_eliminated[1] = true;
                if(team_weights[2] < team_weights[3])
                    already_eliminated[2] = true;
                else
                    already_eliminated[3] = true;
    // stergem din vector competitorii si dupa ii eliminam si de la supervisors

    for(int i = 1;i<=4;i++)
        if(already_eliminated[i - 1]==true) 
        {

            cout<< "Team "<< i <<" eliminated\n";
            // delete from supervisor
            for (auto c: teams[i]) 
            {
                for(auto x:supervisor)
                {
                    vector<int> &comp=x->get_comp_id();
                    for(auto itr=comp.begin(); itr !=comp.end();)
                    {
                        // daca are id-ul de eliminat il elimin
                        if((*itr) == c->get_id_number())
                        {
                            int debt=get_debt_by_id(competitor, *itr);
                            S+=debt;
                            x->decrease_debt(debt);
                            delete_comp_by_id(competitor, *itr);
                            itr=comp.erase(itr);

                        }
                        else
                            itr++;  
                    }
                }

            }

        }
    
    }
//Acelasi algoritm in principiu, insa de aceasta data separ in 2 echipe jucatorii care au castigat si cei care au pierdut
void Game_3::play(vector<Competitors*> &competitor, vector<Supervisor*> supervisor)
{   srand(time(NULL));
    int number1, number2;
    map<int,vector<Competitors *>> teams;

    for(int i = 1; i <=2; i++) 
    {
        teams.insert(pair<int, vector<Competitors *> >(i, vector<Competitors *>()));
        
    }

    
    for(int i = 1; i <= competitor.size(); i=i+2) 
    {   number1=rand();
        number2=rand();
        if(number1<number2)
        {
            teams[1].push_back(competitor[i]);
            teams[2].push_back(competitor[i-1]);
        }
        else
        {
            teams[1].push_back(competitor[i-1]);
            teams[2].push_back(competitor[i]);
        }
    }
    
    for (auto c: teams[2]) 
            {
                for(auto x:supervisor)
                {
                    vector<int> &comp=x->get_comp_id();
                    for(auto itr=comp.begin(); itr !=comp.end();)
                    {
                        // daca are id-ul de eliminat il elimin
                        if((*itr) == c->get_id_number())
                        {
                            int debt=get_debt_by_id(competitor, *itr);
                            S+=debt;
                            x->decrease_debt(debt);
                            delete_comp_by_id(competitor, *itr);
                            itr=comp.erase(itr);
                        }
                        else
                            itr++;  
                    }
                }

            }

}
//In functia aferenta ultimului joc am bagat intr-un map fiecare jucator care a pierdut, iar cel ramas a fost afisat
void Game_4::play(vector<Competitors*> &competitor, vector<Supervisor*> supervisor)
{
    string moves[3] = {"piatra", "foarfeca", "hartie"};
    string pick_1, pick_2;
    map<int,vector<Competitors *>> teams;
    teams.insert(pair<int, vector<Competitors *> >(1, vector<Competitors *>()));
    srand(time(NULL));
    for(int i = competitor.size()-1; i >= 1; i--)
    {
        pick_1=moves[rand()%3];
        pick_2=moves[rand()%3];
        while(pick_1 == pick_2)
        {
            pick_1 = moves[rand() % 3];
            pick_2 = moves[rand() % 3];
        }
        if((pick_1 == "piatra" && pick_2 == "foarfeca") || (pick_1 == "hartie" && pick_2 == "piatra") || (pick_1 == "foarfeca" && pick_2 == "hartie"))
         {
            teams[1].push_back(competitor[i-1]);
            for (auto c: teams[1]) 
            {
                for(auto x:supervisor)
                {
                    vector<int> &comp=x->get_comp_id();
                    for(auto itr=comp.begin(); itr !=comp.end();)
                    {
                        // daca are id-ul de eliminat il elimin
                        if((*itr) == c->get_id_number())
                        {
                            int debt=get_debt_by_id(competitor, *itr);
                            S+=debt;
                            x->decrease_debt(debt);
                            delete_comp_by_id(competitor, *itr);
                            itr=comp.erase(itr);
                        }
                        else
                            itr++;  
                    }
                }
            }
         }
        else
        {
            teams[1].push_back(competitor[i]);
            for (auto c: teams[1]) 
            {
                for(auto x:supervisor)
                {
                    vector<int> &comp=x->get_comp_id();
                    for(auto itr=comp.begin(); itr !=comp.end();)
                    {
                        // daca are id-ul de eliminat il elimin
                        if((*itr) == c->get_id_number())
                        {
                            int debt=get_debt_by_id(competitor, *itr);
                            S+=debt;
                            x->decrease_debt(debt);
                            delete_comp_by_id(competitor, *itr);
                            itr=comp.erase(itr);
                        }
                        else
                            itr++;  
                    }
                }
            }
        }
         
    }
}
//Functie pentru crearea concurentilor
vector<Competitors*> make_competitors()
{
    vector<string> names = read_txt("Names.txt");
    vector<string> cities = read_txt("Cities.txt");
    vector<Competitors*> competitors;

    srand(time(NULL));

    for(int i=1;i<=99;i++)
    {
        int fn_index=rand()%108;
        int ln_index=rand()%108;
        int ct_index=rand()%108;
        int weight = rand()%50+50;
        int debt = rand()%90000+10000;
        int id_number=i;
        competitors.push_back(new Competitors(names[fn_index], names[ln_index], cities[ct_index], weight, debt, id_number));
    }
    return competitors;

}
//Functie pentru crearea supraveghetorilor
vector<Supervisor*> make_supervisor(vector<Competitors*> competitors)
{
    vector<string> names = read_txt("Names.txt");
    vector<string> cities = read_txt("Cities.txt");
    vector<Supervisor*> supervisor;

    srand(time(NULL));

    mask_type v[3]={triangle, circle, rectangle};
    for(int i=0;i<9;i++)
    {  vector<int> comp_id;
        int fn_index=rand()%108;
        int ln_index=rand()%108;
        int ct_index=rand()%108;
        int weight = rand()%50+50;
        int debt = rand()%90000+10000;
        mask_type figure=v[i/3];

        for(int j=i*11+1;j<=(i+1)*11;j++)
            comp_id.push_back(j);

        supervisor.push_back(new Supervisor(names[fn_index], names[ln_index], cities[ct_index], weight, debt, figure, comp_id));
    }
    return supervisor;

}

int main()
{    
    int i, S1=0, S2=0, S3=0;
    vector<Competitors*> competitors = make_competitors();
    vector<Supervisor*> supervisor = make_supervisor(competitors);
    Supervisor* aux;
    cout<<"Welcome to Squid Game"<<"\n";
    cout<<"-------------------------------------------------\n";
    cout<<"The list of competitors:"<<"\n\n";
    cout<<"F_Name L_Name Town Weight Debt Number\n";

    for( i=0;i<99;i++)
    {
        competitors[i]->print();
        cout<<"\n";
    }
    cout<<"-------------------------------------------------\n";
    cout<<"The list of supervisors:"<<"\n";
    cout<<"F_Name L_Name Town Weight Debt Team\n\n";
    //iar mai jos, sub detaliile supravechetorului se afiseaza si concurentii aferenti(doar id-urile)
    for( i=0;i<9;i++)
    {
        supervisor[i]->print();
        cout<<"\n\n";
    }
    cout<<"-------------------------------------------------\n";
    cout<<"List of remaining competitors after game 1:\n\n";
    Game_1 game_1;
    game_1.play(competitors, supervisor);

    for( i=0;i<99;i++)
    { if(competitors[i]!=competitors[i+1])
        {
            competitors[i]->print();
            cout<<"\n";
        }
        else
        {
            competitors[i]->print();
            cout<<"\n";
            break;
        }
    }
    cout<<"The amount accumulated after the first game is:"<<" "<<S<<"\n";
    cout<<competitors.size()<<" left\n\n";
    cout<<"-------------------------------------------------\n";

    cout<<"List of remaining competitors after game 2:\n\n";
    Game_2 game_2;
    game_2.play(competitors, supervisor);
    for( i=0;i<99;i++)
    { if(competitors[i]!=competitors[i+1])
        {
            competitors[i]->print();
            cout<<"\n";
        }
        else
        {
            competitors[i]->print();
            cout<<"\n";
            break;
        }
    }
    cout<<"The amount accumulated after the second game is:"<<" "<<S<<"\n";
    cout<<competitors.size()<<" left\n\n";
    cout<<"-------------------------------------------------\n";

    cout<<"List of remaining competitors after game 3:\n\n";
    Game_3 game_3;
    game_3.play(competitors, supervisor);
    for( i=0;i<99;i++)
    { if(competitors[i]!=competitors[i+1])
        {
            competitors[i]->print();
            cout<<"\n";
        }
        else
        {
            competitors[i]->print();
            cout<<"\n";
            break;
        }
    }
    cout<<"The amount accumulated after third game is:"<<" "<<S<<"\n";
    cout<<competitors.size()<<" left\n\n";
    cout<<"-------------------------------------------------\n";

    Game_4 game_4;
    game_4.play(competitors, supervisor);
    for( i=0;i<99;i++)
    { if(competitors[i]!=competitors[i+1])
        {
            competitors[i]->print();
            cout<<"\n";
        }
        else
        {
            competitors[i]->print();
            cout<<"\n";
            break;
        }
    }
    cout<<"The amount accumulated after fourth game is:"<<" "<<S<<"\n";
    cout<<competitors.size()<<" left\n\n";
    cout<<"The winner received "<<S<<"\n";
    long v[9];
    // for(i=0;i<supervisor.size();i++)
    // v[i]=supervisor[i]->get_debt();

    // for(i=supervisor.size();i>=0;i--)
    //     for(int j=supervisor.size(); j>supervisor.size()-i; j--)
    // {
    //         if(v[j]>v[j-1])
    //         {
    //             aux=supervisor[j];
    //             supervisor[j]=supervisor[j-1];
    //             supervisor[j-1]=aux;
    //         }
    // }
    cout<<"Earnings for every supervisor:\n";
    for(i=0;i<supervisor.size();i++)
        supervisor[i]->print();
        cout<<"\n";
    
    // for(i=0;i<supervisor.size();i++)
    // {
    //     if(supervisor[i]->get_figure() == 0)
    //      S1=S1+abs(supervisor[i]->get_debt());
        
    //     else if(supervisor[i]->get_figure() == 1)
    //      S2=S2+abs(supervisor[i]->get_debt());
        
    //     else if(supervisor[i]->get_figure() == 2)
    //      S3=S3+abs(supervisor[i]->get_debt());
    // }
    // if(S1>S2 && S1>S3)
    // cout<<"Team Triangle got the most money\n";
    // if(S2>S1 && S2>S3)
    // cout<<"Team Cercle got the most money\n";
    // if(S3>S2 && S3>S1)
    // cout<<"Team Square got the most money\n";
    // return 0;
}