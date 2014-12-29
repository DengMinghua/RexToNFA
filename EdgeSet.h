#pragma once
#include<vector>
#include<map>
#include<set>
typedef std::pair<int, char> Edge;
class NFA;
class EdgeSet {
protected:    
    int start_point;
    std::vector<Edge> Edges;
public:
    EdgeSet(int i_start_point);
    void addEdge(Edge edge);
    char if_transable(int i_next_point);
    std::vector<Edge> show_edges();
    void show_me(bool Epsilon_or_not);
    std::set<int> alpha_edge();
};