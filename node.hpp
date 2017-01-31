#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
using namespace std;
struct Gate
{
  string ID;  //u23
  string type;  //AND
  int size;
  vector<Gate> input;
};
struct MySets
{
  queue<Gate> q1;
  queue<Gate> q2;
};
extern queue <Gate> Q1,Q2;
void node(string name);
int find_size(queue<Gate> q);
struct MySets perturb(queue<Gate>, queue<Gate>);
int cost(queue<Gate> q, queue<Gate> q1);
bool acceptMove(int dt, double temperature);
void create_ini_solution(string filename);
vector<Gate> SearchinQueue(Gate g, queue<Gate> v);
