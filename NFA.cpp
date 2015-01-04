#include"NFA.h"
#include<iostream>
#include<queue>
#include<algorithm>
#include<string>
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
    //Pop out two basic NFA from NFA stack
    int new_start = ++Number;
    int new_end = ++Number;
    EdgeSet * new_ES = new EdgeSet(new_start);
    Edge new_Edge1(first.show_start(), -1);
    Edge new_Edge2(second.show_start(), -1);
    new_ES->addEdge(new_Edge1);
    new_ES->addEdge(new_Edge2);
    EdgeSet_collection.push_back(*new_ES);
    //Link the new start node to both the start of the first and the second NFA
    Edge new_Edge3(new_end, -1);
    EdgeSet_collection[first.show_end()].addEdge(new_Edge3);
    EdgeSet_collection[second.show_end()].addEdge(new_Edge3);
    new_ES = new EdgeSet(new_end);
    EdgeSet_collection.push_back(*new_ES);
    //Link both the end of the first and the second NFA to the new end node
    BasicNFA * new_BNFA = new BasicNFA(new_start, new_end);
    NFA_stack.push(*new_BNFA);
    //Put the new NFA back to the NFA stack
    delete new_ES;
    delete new_BNFA;
    //Release temporary variables.
}
void NFA::connect() {
    BasicNFA second = NFA_stack.top();
    NFA_stack.pop();
    BasicNFA first = NFA_stack.top();
    NFA_stack.pop();
    //Pop out two basic NFA from NFA stack
    Edge new_Edge1(second.show_start(), -1);
    EdgeSet_collection[first.show_end()].addEdge(new_Edge1);
    //Link the end of the first to the start of the second
    BasicNFA * new_NFA = new BasicNFA(first.show_start(), second.show_end());
    NFA_stack.push(*new_NFA);
    //Put the new NFA back to the NFA stack
    delete new_NFA;
    //Release temporary variables.
}
void NFA::repeat() {
    BasicNFA temp = NFA_stack.top();
    NFA_stack.pop();
    //Pop out one basic NFA from NFA stack
    int start = ++Number;
    int end = ++Number;
    EdgeSet * New_ES = new EdgeSet(start);
    EdgeSet_collection.push_back(*New_ES);
    New_ES = new EdgeSet(end);
    EdgeSet_collection.push_back(*New_ES);
    //Create two new edgesets
    Edge edge1(temp.show_start(), -1);
    EdgeSet_collection[start].addEdge(edge1);
    //Link the new start to the old start
    Edge edge2(temp.show_start(), -1);
    EdgeSet_collection[temp.show_end()].addEdge(edge2);
    //Link the old end to the old start
    Edge edge3(end, -1);
    EdgeSet_collection[temp.show_end()].addEdge(edge3);
    //Link the old end to the new end
    Edge edge4(end, -1);
    EdgeSet_collection[temp.show_start()].addEdge(edge4);
    //Link the old start to the new end
    BasicNFA * new_BNFA = new BasicNFA(start, end);
    NFA_stack.push(*new_BNFA);
    //Put the new NFA back to the NFA stack
    delete New_ES;
    delete new_BNFA;
    //Release temporary variables.
}
void NFA::left_brace() {
    Char_stack.push('(');
    //push left brace into character stack directely
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
    //pop the character stack and operate until match '(' 
    //and push right brace into character stack
}
void NFA::element(char i_transer) {
    int start, end;
    start = ++Number;
    end = ++Number;
    EdgeSet * new_ES = new EdgeSet(start);
    Edge new_Edge(end, i_transer);
    //Link the start to the end using the transer character;
    new_ES->addEdge(new_Edge);
    EdgeSet_collection.push_back(*new_ES);
    new_ES = new EdgeSet(end);
    EdgeSet_collection.push_back(*new_ES);
    //Add a empty edgeset to the edgeset collection for end node
    BasicNFA * new_BNFA = new BasicNFA(start, end);
    NFA_stack.push(*new_BNFA);
    //Put the new NFA back to the NFA stack
    delete new_ES;
    delete new_BNFA;
    //Release temporary variables.
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
    //Eclosure set
    std::stack<int> WtD;
    //Wait to do stack
        std::vector<Edge> temp_Edge = EdgeSet_collection[i_start_point].show_edges();
        //Get the edgeset of the start node
        for (auto x : temp_Edge)
        if (x.second == -1) {
            Ecl.insert(x.first);
            WtD.push(x.first);
        }
        //push all the Epsilon edge to the Eclosure and the wait to do stack
    while (!WtD.empty()) {
        int x = WtD.top();
        WtD.pop();
        //Pop a node from the wait to do stack
        std::vector<Edge> temp_Edge = EdgeSet_collection[x].show_edges();
        //Get the edgeset of the node
        for (auto y : temp_Edge)         
            if (y.second == -1) {
            for (auto z : Ecl) if (z == y.first) continue;
            if (y.first == i_start_point) continue;
            Ecl.insert(y.first);
            WtD.push(y.first);
            //If the node is not in the Eclosure
            //Add it into the Eclosure and the wait to do list
        }
    }
    return Ecl;
}
void NFA::delete_E() {
    if (Epsilon_NFA == 1) return;
    int original_end = NFA_stack.top().show_end();
    std::set<int> temp;
    for (auto x : EdgeSet_collection) {
        temp = x.alpha_edge();
        std::set_union(temp.begin(), temp.end(), alpha_point.begin(), alpha_point.end(), inserter(alpha_point, alpha_point.begin()));
    }
    alpha_point.insert(NFA_stack.top().show_start());
    //Find all the available nodes
    //for (auto x : alpha_point)
    //if (x == original_end)
    end.insert(original_end);
    //Find all the node link to the original end
    for (auto x : alpha_point) {
        temp = E_closure(x);
        for (auto y : temp) {
            if (y == original_end) end.insert(x);
            //Find all the node link to the original end
            std::vector<Edge> temp_Edge = EdgeSet_collection[y].show_edges();
            for (auto z : temp_Edge)
                if (z.second != -1) EdgeSet_collection[x].addEdge(z);
        }
    }
    //Link all the available nodes from one start node to the start node
    Epsilon_NFA = 1;
}
void NFA::search(int i_start, int i_index, int i_point) {
    if (i_index < (int)input.length()) {
        for (auto x : EdgeSet_collection[i_point].show_edges())
            if (x.second == input[i_index]) {
                search(i_start,i_index + 1, x.first);
                for (auto y : end) 
                    if (x.first == y) {
                    record temp_string;
                    temp_string.first.append(input, i_start, i_index - i_start + 1);
                    bool flag = 0;
                    for (int z = 0; z < (int) records.size();z++)
                        if (records[z].first == temp_string.first) {
                        records[z].second++;
                        flag = 1;
                        }
                    if (flag == 0) {
                        temp_string.second = 1;
                        records.push_back(temp_string);
                    }
                    break;
                    }
                break;
            }
    }
}
bool NFA::test(std::string i_target) {
    input = i_target;
    for (int i = 0; i < (int)i_target.length(); i++)
        search(i, i, start);
    for (auto x : records) std::cout << x.first << " has appeared " << x.second << " times\n";
    records.clear();
    return 1;
}