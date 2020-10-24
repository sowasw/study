#ifndef _GRAPHTYPES_H
#define _GRAPHTYPES_H

#include <string>
#include <map>
#include <set>

using namespace std;

struct Node;     //城市节点
struct Arc;      //城市之间的航线距离

//整个图包含城市的集合和航线距离的集合，以及字符串和节点的映射
struct SimpleGraph {
   set<Node *> nodes;
   set<Arc *> arcs;
   map<std::string,Node *> nodeMap;
};

struct Node {
   std::string name;
   set<Arc *> arcs;
};

struct Arc {
   Node *start;
   Node *finish;
   double cost;
};

#endif
