#pragma once
class BasicNFA {
protected:
    int start;
    int end;
public:
    BasicNFA(int i_start, int i_end);
    ~BasicNFA();
    void set(int i_start, int i_end);
    int show_start();
    int show_end();
};