#pragma once
class Edge {
protected:
    int next_point;
    char transer;
public:
    Edge(int i_next_point, char i_transer);
    ~Edge()
};