#include "node.hpp"
#include <cmath>
#include <cstdlib>
using namespace std;

queue <Gate> Q, Qmask;
queue <Gate> Q1,Q2;
vector <string> vin, vout;  //vectors for storing the inputs/outputs

void create_ini_solution(string filename)
{
  for(int i = 0; i < (signed)vin.size(); i++)
  {
    Gate g;
    g.ID = vin[i];
    g.size = 1;
    Q.push(g);
  }
  while(!Q.empty())
  {
    if(rand()%2 == 0)
      Q1.push(Q.front());
    else
      Q2.push(Q.front());
    Q.pop();
  }
}

bool acceptMove(int dt, double temperature)
{
  double const e = 2.7182818;
  double const k = 50;
  if(dt < 0)
    return true;
  else
  {
    double boltz = pow(e, -dt/k/temperature);
    double r = (double)rand()/RAND_MAX;
    if(r < boltz)
      return true;
    else
      return false;
  }
}

struct MySets perturb(queue<Gate> a, queue<Gate> b)
{
  struct Gate temp;
  struct MySets myset;
  if(a.size() > b.size())
  {
    temp = a.front();
    a.pop();
    myset.q1 = a;
    b.push(temp);
    myset.q2 = b;
  }
  else
  {
    temp = b.front();
    b.pop();
    myset.q1 = b;
    a.push(temp);
    myset.q2 = a;
  }
  return myset;
}

int cost(queue<Gate> q, queue<Gate> p)
{
  int sum = 0;
  vector<Gate> vg;
  while(!q.empty())
  {
    vg = SearchinQueue(q.front(), p);
    sum += vg.size();
    q.pop();
  }
  return sum;
}

int find_size(queue<Gate> q)
{
  int sum = 0;
  while(!q.empty())
  {
    sum += q.front().size;
    q.pop();
  }
  return sum;
}
void node(string filename)
{
  ifstream in;
  ofstream out;
  char c;
  int len = 0, pos;
  string s, s1, s2, s3[2], first_input, sc;
  struct Gate g,child_g, output_g;
  vector<Gate>::iterator it;
  vector<Gate> vg;
  in.open (filename.c_str());
  do
  {
    in >> std::ws;
    getline(in,s); //get rid of comments
  } while(s[0] == '#');

  //at this point, s contain the first input line
  pos = s.find("(") + 1;
  first_input = s.substr(pos,s.find(")")-pos);
  vin.push_back(first_input);

  while(s[0] == 'I')  //parse the inputs
  {
      in >> std::ws;
      if((c = in.peek()) != 'I') break;
    getline(in,s,'('); //s="input"
    getline(in,s1,')');  //s1="n1"
    vin.push_back(s1);
  }

  do   //parse the outputs
  {
      in >> std::ws;
      if((c = in.peek()) != 'O') break;
    getline(in,s,'('); //s="output"
    getline(in,s1,')'); //s1="U35"
    vout.push_back(s1);
  } while(s[0] == 'O');

  //parse the gate information
  do
  {
    in >> std::ws;
    g.input.clear();
    if(in.eof()) break;
    getline(in,s,'=');   //s = U34
    for (int i = s.length()-1; i >= 0; --i) {
       if(s[i] == ' ')
          s.erase(i, 1);
    }
    in >> std::ws;
    g.ID = s;
    getline(in,s1,'(');   //s1 = NAND
    g.type = s1;
    in >> std::ws;
    if(s1 == "NOT")  //is a one-input gate
    {
      getline(in,s2,')');   //s2 = gate inputs
      child_g.ID = s2;
      g.input.push_back(child_g); //add the input node to vector
      g.size = 1;
      Q.push(g);
    }
    else  //is a n-input gate
    {
      getline(in,s,')');  //out << s << endl;
      sc = s;len =0;
      do
      {
        in >> std::ws;
        s2 = s.substr(0,s.find(","));  //s2 = U38
        s.erase(0,s2.length()+2);
        len += s2.length()+2;
        child_g.ID = s2;
        g.input.push_back(child_g);
      }  while(len < (signed)sc.length());
      g.size = g.input.size();
      Q.push(g);
    }

  } while(!in.eof());

  in.close();
}

vector<Gate> SearchinQueue(Gate target, queue<Gate> q)
{
  Gate g;
  std::vector<Gate> dummy;
  while(!q.empty())
  {
    g = q.front();
    for(int i = 0; i < (signed)g.input.size();i++)
    {
      if(target.ID == g.input[i].ID)  //found target in the input vector
      {
        dummy.push_back(g);
      }
    }
    q.pop();
  }
  return dummy;
}
