#include "fsm.h"

void FSM::checkCondition() noexcept(false){
    //E1
    std::string initial = fsm_raw[INITIAL].front();
    if (!beIn(initial, fsm_raw[STATE])) throw MyError(ErrorType::E1);
    for (const auto & state : fsm_raw[ACCEPTING])
    {
        if (!beIn(state, fsm_raw[STATE])) throw MyError(ErrorType::E1);
    }
    //E2
    if (!isNotDisjoint()) throw MyError(ErrorType::E2);

    //E3
    for (const auto & [from,alpha,to] : transitions)
    {
        if (!beIn(from, fsm_raw[STATE])) throw MyError(ErrorType::E1);
        if (!beIn(alpha, fsm_raw[ALPHABET])) throw MyError(ErrorType::E3);
        if (!beIn(to, fsm_raw[STATE])) throw MyError(ErrorType::E1);
    }
    //E4
    if (fsm_raw[INITIAL].size() == 0) throw MyError(ErrorType::E4);

    //E5
    if (!isDeterm()) throw MyError(ErrorType::E5);

}

bool FSM::beIn(const string &item, const std::vector<string> &array)
{
    for (const auto & state_name: array)
        if (item == state_name)
            return true;
    return false;
}

bool FSM::isDeterm()
{
    for (const std::string & from: fsm_raw[STATE])
    {
        std::vector <Trans> trans = fsm_graph[from];
        for (size_t i = 0; i < trans.size(); ++i)
        {
            if (trans[i].alpha == EPS) return false;
            for (size_t j = i+1; j < trans.size(); ++j)
                if (trans[i].alpha == trans[j].alpha) return false;
        }
    }
    return true;
}

bool FSM::isNotDisjoint() {
    std::queue <std::string> q;
    q.push(fsm_raw[INITIAL].front());
    std::unordered_map<std::string, bool> used;
    used[fsm_raw[INITIAL].front()] = true;

    while (!q.empty()) {
        std::string state = q.front();
        q.pop();
        for (size_t i=0; i < transitions.size(); ++i) {

            std::string from = transitions[i].from;
            std::string to = transitions[i].to;

            if (to != state && from != state) continue;
            if (used.find(to) == used.end()) {
                used[to] = true;
                q.push (to);
            }
            if (used.find(from) == used.end()) {
                used[from] = true;
                q.push (from);
            }
        }
    }
    return used.size() == fsm_raw[STATE].size();
}

