#pragma once
#include"EdgeSet.h"
class DFA {
protected:
    int start;
    int end;
    std::string rex;
    std::vector<EdgeSet> EdgeSet_collection;
    int Number;
public:
    DFA();
    ~DFA();
    void add_EdgeSet(EdgeSet i_EdgeSet);
};