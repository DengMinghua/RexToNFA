#include"BasicNFA.h"
BasicNFA::BasicNFA(int i_start, int i_end) {
    start = i_start;
    end = i_end;
}
BasicNFA::~BasicNFA() {};
void BasicNFA::set(int i_start, int i_end) {
    start = i_start;
    end = i_end;
}
int BasicNFA::show_start() {
    return start;
}
int BasicNFA::show_end() {
    return end;
}