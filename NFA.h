#pragma once
#include"Edgeset.h"
#include"BasicNFA.h"
#include<string>
#include<vector>
#include<stack>
#include"DFA.h"
typedef std::pair<std::string, int> record;
class NFA {
protected:
    bool possible;
    bool Epsilon_NFA;
    std::string rex;
    std::string input;
    std::vector<record> records;
    std::vector<EdgeSet> EdgeSet_collection;
    std::stack<BasicNFA> NFA_stack;
    std::stack<char> Char_stack;
    std::set<int> alpha_point;
    int Number;
    int start;
    std::set<int> end;
    std::set<int> E_closure(int i_start_point);
    void search(int i_start, int i_index, int i_point);
    void select();
    void connect();
    void repeat();
    void left_brace();
    void right_brace();
    void element(char i_transer);
public:
    NFA(std::string i_rex);
    ~NFA();
    void show_me();
    void delete_E();
    bool test(std::string i_target);
};