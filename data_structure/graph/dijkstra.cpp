#include <iostream>
#include <fstream>
#include "graphtypes.h"

#include <queue>
#include <set>
#include <vector>

using namespace std;

vector<Arc *> findShortestPath(Node *start, Node *finish);
double getPathCost(const vector<Arc *> & path);
void printPath(const vector<Arc *> & path, int n);
void inputAirlineToSerch(string & start, string & finish, SimpleGraph & airline);


void initAirlineGraph(SimpleGraph & airline);
void addFlight(SimpleGraph & airline, string c1, string c2, int miles);
void addNode(SimpleGraph & g, string name);
void addArc(SimpleGraph & g, Node *n1, Node *n2, double cost);

//优先级队列规则,路径较小的优先
struct cmp{
	bool operator ()(vector<Arc *> & a, vector<Arc *> & b){
		return getPathCost(a) > getPathCost(b);
	}
};



int main() {
   SimpleGraph airline;
   initAirlineGraph(airline);
   
   string name;
   string name2;
   inputAirlineToSerch(name,name2,airline);
   
   vector<Arc *> path = findShortestPath(airline.nodeMap[name], airline.nodeMap[name2]);
   cout << "The shortest path : \n";
   printPath(path,0);
   
   return 0;
}

vector<Arc *> findShortestPath(Node *start, Node *finish) {
   vector<Arc *> path;
   //priority_queue< Arc *, vector<Arc *>, cmp > queue;//error
   priority_queue< vector<Arc *>, vector< vector<Arc *> >, cmp > queue;
   map<std::string, double> fixed;
   while (start != finish) {
      if (!fixed.count(start->name)) {
		 
         //fixed.insert(start->name, getPathCost(path));//error
		 fixed[start->name] = getPathCost(path);
        
         for (Arc *arc : start->arcs) {
            if (fixed.count(arc->finish->name)) {
				cout << arc->start->name << " -> " << arc->finish->name << " 忽略\n";
            } else {
				path.push_back(arc);
			    printPath(path,1);
	   
				queue.push(path);
				path.pop_back();
            }
         }
      }
      if (queue.empty()) {
         path.clear();
         return path;
      }
      path = queue.top();
	  printPath(path,2);
	  
	  queue.pop();
      start = path[path.size() - 1]->finish;
   }
   return path;
}

double getPathCost(const vector<Arc *> & path) {
   double cost = 0;
   for (Arc *arc : path) {
      cost += arc->cost;
   }
   return cost;
}

void printPath(const vector<Arc *> & path, int n){
	if(path.size() > 0){//
		//cout << path.size() << " (path.size)\n";
		for(int i = 0; i < path.size(); i++)
			cout << path[i]->start->name << " -> " ;
		cout << path[path.size() - 1]->finish->name << " (" <<  getPathCost(path) << ")";
		if (n == 0)
			cout << " \n";
		else if (n == 1)
			cout << " (enqueue)\n";
		else 
			cout << " (dequeue)\n";

	}
}

void inputAirlineToSerch(string & start, string & finish, SimpleGraph & airline){
	int i = 0;
	vector<string> names;
	cout << "City codes: \n";
	for(Node * city : airline.nodes){
		cout << "[" << i++ << "]" << city->name << "\n";
		names.push_back(city->name) ;
	}
	//cout << "\n";
	cout << "Start city code: ";
	cin >> i;
	start = names[i];
	cout << "Finish city code: ";
	cin >> i;
	finish = names[i];
}

# include "AirlineGraph.inc"
