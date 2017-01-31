#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include "node.hpp"
#define NUM_MOVE_PER_TIMESTEP 50
using namespace std;
int main(int argc, char *argv[])
{
	string input, output;
	ifstream fin;
	ofstream fout, out;
  string s;
	double temperature = 400, freeze_temp = 0.1;
	int dt;
	struct MySets set;
	srand(time(0));
	if(argc != 2)
	{
		cout << "error in command... must be in format ./bipartition + [filename]...\n";
		exit(1);
	}
	input = argv[1];
	output = "output.txt";
	fout.open (output.c_str());
	node(input);

	create_ini_solution(input);
	while(temperature > freeze_temp)
	{
		for(int i = 1; i < NUM_MOVE_PER_TIMESTEP; i ++)
		{
			double cost1 = cost(Q1,Q2);
			set = perturb(Q1,Q2);
			dt = cost(set.q1,set.q2) - cost1;

			if(acceptMove(dt, temperature))
			{
				Q1 = set.q1;
				Q2 = set.q2;
			}
		}
		out << cost(Q1,Q2) << '\t' << temperature << endl;
		temperature = 0.9 * temperature; //cout << "temperature is " << temperature << endl;
	}
	fout << "for circuit file " << input << endl;
	fout << "the cut size of the final partition is " << cost(Q1,Q2);
	fout << " ,size ratio is "<<(double)Q1.size()/Q2.size() << endl;
	fout << "size of partition 1 is "<< find_size(Q1) << " ,size of partition 2 is "<<find_size(Q2) << endl;
  fout << "in partition 1, the gates are listed as follows:\n";
	while(!Q1.empty())
	{
		fout << Q1.front().ID << '\t';
		Q1.pop();
	}
	fout << "\nin partition 2, the gates are listed as follows:\n";
	while(!Q2.empty())
	{
		fout << Q2.front().ID << '\t';
		Q2.pop();
	}
	fout << endl;
	fout.close();
  return 0;
}
