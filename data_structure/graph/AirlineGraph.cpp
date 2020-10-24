#include <iostream>
#include <string>
#include "graphtypes.h"
#include <set>
using namespace std;

void printAdjacencyLists(SimpleGraph & g);
void initAirlineGraph(SimpleGraph & airline);
void addFlight(SimpleGraph & airline, string c1, string c2, int miles);
void addNode(SimpleGraph & g, string name);
void addArc(SimpleGraph & g, Node *n1, Node *n2, double cost);

int main() {
   SimpleGraph airline;
   initAirlineGraph(airline);
   printAdjacencyLists(airline);
   return 0;
}

void printAdjacencyLists(SimpleGraph & g) {
   for(Node *node : g.nodes) {
      cout << node->name << " -> ";
      bool first = true;
      for(Arc *arc : node->arcs) {
         if (!first) cout << ", ";
         cout << arc->finish->name;
         first = false;
      }
      cout << endl;
   }
}

# include "AirlineGraph.inc"
