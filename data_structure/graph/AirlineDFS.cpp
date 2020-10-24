#include <iostream>
#include <string>
#include "graphtypes.h"
#include <set>
#include <vector>
#include <stack>
using namespace std;

void depthFirstSearch(Node *node);
void visitUsingDFS(Node *node, set<Node *> & visited);
void visit(Node *node);
void initAirlineGraph(SimpleGraph & airline);
void addFlight(SimpleGraph & airline, string c1, string c2, int miles);
void addNode(SimpleGraph & g, string name);
void addArc(SimpleGraph & g, Node *n1, Node *n2, double cost);

void depthFirstSearch2(Node *node);//use stack

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
   depthFirstSearch(airline.nodeMap[name]);
   
   cout << "\ndepthFirstSearch2: \n";
   depthFirstSearch2(airline.nodeMap[name]);
   return 0;
}

void depthFirstSearch(Node *node) {
   set<Node *> visited;
   visitUsingDFS(node, visited);
}

void visitUsingDFS(Node *node, set<Node *> & visited) {
   if (visited.count(node)) return;
   visit(node);
   visited.insert(node);
   for(Arc *arc: node->arcs) {
      visitUsingDFS(arc->finish, visited);
   }
}

void visit(Node *node) {
   cout << node->name << endl;
}

void depthFirstSearch2(Node *node){
	set<Node *> visited;
	stack<Node *> s;//用栈而不是递归函数实现
	s.push(node);
	while(!s.empty()){
		node = s.top();
		s.pop();
		if (!visited.count(node)){
			visited.insert(node);
			visit(node);
		}else{
			continue;
		}
		for(Arc *arc: node->arcs) {
			s.push(arc->finish);
		}
	}
}

# include "AirlineGraph.inc"
