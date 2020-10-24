#include <iostream>
#include <string>
#include "graphtypes.h"
#include <queue>
#include <set>
#include <vector>

using namespace std;

void breadthFirstSearch(Node *node);
void visit(Node *node);
void initAirlineGraph(SimpleGraph & airline);
void addFlight(SimpleGraph & airline, string c1, string c2, int miles);
void addNode(SimpleGraph & g, string name);
void addArc(SimpleGraph & g, Node *n1, Node *n2, double cost);



int main() {
   SimpleGraph airline;
   initAirlineGraph(airline);
   
   int i = 0;
	vector<string> names;
	cout << "City codes: \n";
	for(Node * city : airline.nodes){
		cout << "[" << i++ << "]" << city->name << "\n";
		names.push_back(city->name) ;
	}
   
   cout << "Starting city code: ";
   string name;
   cin >> i;
   name = names[i];
   
   cout << "\n";
   breadthFirstSearch(airline.nodeMap[name]);
   return 0;
}



void breadthFirstSearch(Node *node) {
   set<Node *> visited;//记录访问过的城市节点
   queue<Node *> que;
   que.push(node);
   while (!que.empty()) {
      node = que.front();
	  que.pop();
      if (!visited.count(node)) {
         visit(node);
         visited.insert(node);
         for (Arc *arc : node->arcs) {
            que.push(arc->finish);
         }
      }
   }
}



void visit(Node *node) {
   cout << node->name << endl;
}

# include "AirlineGraph.inc"
