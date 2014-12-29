#include"NFA.h"
#include<iostream>
#include<queue>
#include<algorithm>
NFA::NFA(std::string i_rex) {
    Epsilon_NFA = 0;
    possible = false;
    Number = -1;
    rex = i_rex;
    int len = i_rex.length();
    for (int i = 0; i < len; i++) {
        char x = i_rex[i];
        switch (x) {
        case '-': {
            connect();
            break;
        }
        case '(': {
            left_brace();
            break;
        }
        case ')': {
            right_brace();
            if ((i < len - 1) && (i_rex[i + 1] != ')') && (i_rex[i + 1] != '*') && (i_rex[i + 1] != '+'))
                Char_stack.push('-');
            break;
        }
        case '*': {
            repeat();
            if (i < len - 1 && i_rex[i + 1] != ')' && i_rex[i + 1] != '+')  Char_stack.push('-');
            break;
        }
        case '+': { 
            std::stack<char> temp;

            while (!Char_stack.empty() && Char_stack.top() != '(' ) {
                if (Char_stack.top() == '+') {
                    select();                 
                }
                if (Char_stack.top() == '-') {
                    connect();
                }
                Char_stack.pop();
            }
            Char_stack.push('+');
            break; 
        }
        default: {
            element(x);
            if ((i < len - 1) && (i_rex[i + 1] != ')') && (i_rex[i + 1] != '*') && (i_rex[i + 1] != '+')) 
                Char_stack.push('-');
            break;
        }
        }
    }

    while (!Char_stack.empty()) {
        if (Char_stack.top() == '-') connect();
        if (Char_stack.top() == '+') select();
        Char_stack.pop();
    }
    start = NFA_stack.top().show_start();
}
NFA::~NFA() {}
void NFA::select() {
    BasicNFA second = NFA_stack.top();
    NFA_stack.pop();
    BasicNFA first = NFA_stack.top();
    NFA_stack.pop();

    int new_start = ++Number;
    int new_end = ++Number;
    EdgeSet * new_ES = new EdgeSet(new_start);
    Edge new_Edge1(first.show_start(), -1);
    Edge new_Edge2(second.show_start(), -1);

    new_ES->addEdge(new_Edge1);
    new_ES->addEdge(new_Edge2);
    EdgeSet_collection.push_back(*new_ES);
    Edge new_Edge3(new_end, -1);

    EdgeSet_collection[first.show_end()].addEdge(new_Edge3);
    EdgeSet_collection[second.show_end()].addEdge(new_Edge3);

    new_ES = new EdgeSet(new_end);
    EdgeSet_collection.push_back(*new_ES);
    BasicNFA * new_BNFA = new BasicNFA(new_start, new_end);
    NFA_stack.push(*new_BNFA);
    delete new_ES;
    delete new_BNFA;

}
void NFA::connect() {
    BasicNFA second = NFA_stack.top();
    NFA_stack.pop();
    BasicNFA first = NFA_stack.top();
    NFA_stack.pop();
    Edge temp(second.show_start(), -1);
    EdgeSet_collection[first.show_end()].addEdge(temp);
    BasicNFA * new_NFA = new BasicNFA(first.show_start(), second.show_end());
    NFA_stack.push(*new_NFA);
    delete new_NFA;
}
void NFA::repeat() {

    BasicNFA temp = NFA_stack.top();
    NFA_stack.pop();
    int start = ++Number;
    int end = ++Number;
    EdgeSet * New_ES = new EdgeSet(start);
    EdgeSet_collection.push_back(*New_ES);
    New_ES = new EdgeSet(end);
    EdgeSet_collection.push_back(*New_ES);
    Edge edge1(temp.show_start(), -1);
    EdgeSet_collection[start].addEdge(edge1);
    Edge edge2(temp.show_start(), -1);
    EdgeSet_collection[temp.show_end()].addEdge(edge2);
    Edge edge3(end, -1);
    EdgeSet_collection[temp.show_end()].addEdge(edge3);
    Edge edge4(end, -1);
    EdgeSet_collection[temp.show_start()].addEdge(edge4);
    BasicNFA * new_BNFA = new BasicNFA(start, end);
    NFA_stack.push(*new_BNFA);
    delete New_ES;
    delete new_BNFA;
}
void NFA::left_brace() {
    Char_stack.push('(');
}
void NFA::right_brace() {
    char temp = Char_stack.top();
    Char_stack.pop();
    while (temp != '(') {
        if (temp == '-') connect();
        if (temp == '*') repeat();
        if (temp == '+') select();
        temp = Char_stack.top();
        Char_stack.pop();
    }
}
void NFA::element(char i_transer) {
    int start, end;
    start = ++Number;
    EdgeSet * new_ES = new EdgeSet(start);
    end = ++Number;
    Edge new_Edge(end, i_transer);
    new_ES->addEdge(new_Edge);
    EdgeSet_collection.push_back(*new_ES);
    new_ES = new EdgeSet(end);
    EdgeSet_collection.push_back(*new_ES);
    BasicNFA * new_BNFA = new BasicNFA(start, end);
    NFA_stack.push(*new_BNFA);
    delete new_ES;
    delete new_BNFA;
}
void NFA::show_me() {

    std::cout << "Start: " << NFA_stack.top().show_start() << std::endl;
    std::cout << "End: ";
    for (auto x : end) std::cout << x <<" ";
    std::cout << std::endl;
    std::cout << std::endl;
    for (auto x : alpha_point) {
        EdgeSet_collection[x].show_me(Epsilon_NFA);
    }
    std::cout << std::endl;
}
std::set<int> NFA::E_closure(int i_start_point) {
    std::set<int> Ecl;
    std::stack<int> WtD;
        std::vector<Edge> temp_Edge = EdgeSet_collection[i_start_point].show_edges();
        for (auto x : temp_Edge)
        if (x.second == -1) {
            Ecl.insert(x.first);
            WtD.push(x.first);
        }
    while (!WtD.empty()) {
        int x = WtD.top();
        WtD.pop();
        std::vector<Edge> temp_Edge = EdgeSet_collection[x].show_edges();
        for (auto y : temp_Edge)         
            if (y.second == -1) {
            for (auto z : Ecl) if (z == y.first) continue;
            if (y.first == i_start_point) continue;
            Ecl.insert(y.first);
            WtD.push(y.first);
        }
    }
    return Ecl;
}
void NFA::delete_E() {
    int ends = NFA_stack.top().show_end();
    std::set<int> temp;
    for (auto x : EdgeSet_collection) {
        temp = x.alpha_edge();
        std::set_union(temp.begin(), temp.end(), alpha_point.begin(), alpha_point.end(), inserter(alpha_point, alpha_point.begin()));
    }
    for (auto x : alpha_point)
        if (x == ends) end.insert(ends);
    alpha_point.insert(NFA_stack.top().show_start());
    for (auto x : alpha_point) {
        temp = E_closure(x);
        for (auto y : temp) {
            if (y == ends) end.insert(x);
            std::vector<Edge> temp_Edge = EdgeSet_collection[y].show_edges();
            for (auto z : temp_Edge)
                if (z.second != -1) EdgeSet_collection[x].addEdge(z);
        }
    }
    Epsilon_NFA = 1;
}
void NFA::search(int i_index, int i_point) {

    if (possible == true) return;
    if (i_index < input.length() ) {
        for (auto x : EdgeSet_collection[i_point].show_edges())
            if (x.second == input[i_index]) {
            if (i_index == input.length() - 1)
                for (auto y : end)

                    if (x.first == y) {
                possible = true;
                return;
            }
            search(i_index + 1, x.first);
            }
    }
}
bool NFA::run(std::string i_target) {
    possible = false;
    input = i_target;

    search(0, start);
    return possible;
}