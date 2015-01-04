#include"EdgeSet.h"
#include<iostream>
EdgeSet::EdgeSet(int i_start_point) {
    start_point = i_start_point;
}
void EdgeSet::addEdge(Edge edge) {
    Edges.push_back(edge);
}
char EdgeSet::if_transable(int i_next_point) {
    for (auto x : Edges)
        if (x.first = i_next_point) return x.second;
    return -2;
    //See if the start node can transfer to the next point and return the transer.
}
void EdgeSet::show_me(bool Epsilon_or_not) {
    std::cout << "Point " << start_point << std::endl;
    for (auto x : Edges) {
        if (x.second == -1 && !Epsilon_or_not) std::cout << "To point " << x.first << " by " << "Epsilon" << ";" << std::endl;
            else if (x.second != -1) std::cout << "To point " << x.first << " by \'" << x.second << "\'; " << std::endl;
    }
    std::cout << std::endl;
}
std::set<int> EdgeSet::alpha_edge() {
    std::set<int> temp;
    for (auto x : Edges)
        if (x.second != -1) temp.insert(x.first);
    return temp;
};
std::vector<Edge> EdgeSet::show_edges() {
    std::vector<Edge> temp;
    for (auto x : Edges)
        temp.push_back(x);
    return temp;
}