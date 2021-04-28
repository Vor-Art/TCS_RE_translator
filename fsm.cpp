#include "fsm.h"

FSM::FSM(const std::vector<string> &input_str) noexcept(false){
    if (input_str.size()!=Attribute::SIZE) throw MyError(ErrorType::E0);
    fsm_raw.reserve(Attribute::SIZE);

    for(uint8_t i = 0; i < input_str.size(); i++)
    {
        Field field;
        auto pos1 = input_str[i].find(Attribute_msg[i]);
        if (pos1 == std::string::npos || pos1 != 0) throw MyError(ErrorType::E0);

        auto pos2 = input_str[i].find("]");
        if (pos2 == std::string::npos || pos2+1 != input_str[i].size()) throw MyError(ErrorType::E0);

        auto s =input_str[i].begin();
        auto f =input_str[i].end();

        std::string line(s+Attribute_msg[i].size(),f-1);
        std::stringstream field_stream(line);
        std::string line1;
        while (getline(field_stream, line1, ','))
            field.emplace_back(std::move(line1));
        fsm_raw.emplace_back(std::move(field));
    }
    if (fsm_raw[INITIAL].size() > 1) throw MyError(ErrorType::E0);

    for (const auto & trans_raw: fsm_raw[TRANS])
    {
        Trans trans_parse;
        std::stringstream stream(trans_raw);

        bool success = true;

        success = success && getline(stream, trans_parse.from, '>');
        success = success && getline(stream, trans_parse.alpha, '>');
        success = success && getline(stream, trans_parse.to);

        if (!success) throw MyError(ErrorType::E0);

        fsm_graph[trans_parse.from].push_back(trans_parse);
        transitions.push_back(trans_parse);
    }
    checkCondition();
}

void FSM::checkCondition() noexcept(false){
    //E0
    for (const auto& str: fsm_raw[STATE])
        if (!correctStr(str)) throw MyError(ErrorType::E0);

    for (const auto& str: fsm_raw[ALPHABET])
        if (!correctStr(str,{'_'})) throw MyError(ErrorType::E0);

    //E1
    std::string initial = fsm_raw[INITIAL].front();
    if (!beIn(initial, fsm_raw[STATE])) throw MyError(ErrorType::E1);
    for (const auto & state : fsm_raw[ACCEPTING])
        if (!beIn(state, fsm_raw[STATE])) throw MyError(ErrorType::E1);

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
    if (fsm_raw[INITIAL].empty()) throw MyError(ErrorType::E4);

    //E5
    if (!isDeterm()) throw MyError(ErrorType::E5);

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
        for (auto & transitor: transitions) {

            std::string from = transitor.from;
            std::string to = transitor.to;

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

bool FSM::correctStr(const string &str, const std::vector<char> &extension){
    for (const char& symb: str)
    {
        if (isLetter(symb)) continue;
        if (isNumber(symb)) continue;
        if (beIn(symb,extension)) continue;
        return false;
    }
    return true;
}

bool FSM::isLetter(const char ch) {
    return ( ('a' <= ch) && (ch <= 'z') ) || ( ('A' <= ch) && (ch <= 'Z') );
}

bool FSM::isNumber(const char ch) {
    return ('0' <= ch) && (ch <= '9');
}

