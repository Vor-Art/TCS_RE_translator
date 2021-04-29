#include "fsm.h"

FSM::FSM(const std::vector<string> &input_str) noexcept(false)
{
    if (input_str.size()!=Attribute::SIZE) throw MyError(ErrorType::E0);

    fillFsmRaw(input_str);
    fillDictionary();
    fillGraph();
    checkCondition();
}



void FSM::fillFsmRaw(const std::vector<string> &input_str)
{
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
}

void FSM::fillDictionary()
{
    states_dictinary.reserve(fsm_raw[STATE].size());
    for (const auto& state_name: fsm_raw[STATE])
        states_dictinary.emplace(state_name, states_dictinary.size());

//    alphabet_dictinary.reserve(fsm_raw[ALPHABET].size());
//    for (const auto& alpha_name: fsm_raw[ALPHABET])
//            alphabet_dictinary.emplace(alpha_name, alphabet_dictinary.size());
}
void FSM::initGraph(size_t N)
{
    fsm_graph.resize(N, Field(N, EMPTY));
}

void FSM::fillGraph()
{
    initGraph(fsm_raw[STATE].size());
    transitions.reserve(fsm_raw[TRANS].size());
    for (const auto & trans_raw: fsm_raw[TRANS])
    {
        Trans trans_parse;
        std::stringstream stream(trans_raw);

        bool success = true;
        success = success && getline(stream, trans_parse.from, '>');
        success = success && getline(stream, trans_parse.alpha, '>');
        success = success && getline(stream, trans_parse.to);
        if (!success) throw MyError(ErrorType::E0);

        size_t indx_from = states_dictinary.at(trans_parse.from);
        size_t indx_to = states_dictinary.at(trans_parse.to);
        fsm_graph[indx_from][indx_to] = trans_parse.alpha;
        transitions.push_back(trans_parse);
    }
}

void FSM::checkCondition() const noexcept(false)
{

    //E0: Input file is malformed
    if (fsm_raw[INITIAL].size() > 1) throw MyError(ErrorType::E0);

    for (const auto& str: fsm_raw[STATE])
        if (!correctStr(str)) throw MyError(ErrorType::E0);

    for (const auto& str: fsm_raw[ALPHABET])
        if (!correctStr(str,{'_'})) throw MyError(ErrorType::E0);

    //E1: A state '%s' is not in the set of states
    std::string initial = fsm_raw[INITIAL].front();
    if (!beIn(initial, fsm_raw[STATE])) throw MyError(ErrorType::E1, initial);

    for (const auto & state : fsm_raw[ACCEPTING])
        if (!beIn(state, fsm_raw[STATE])) throw MyError(ErrorType::E1, state);

    //E2: Some states are disjoint
    if (!isNotDisjoint()) throw MyError(ErrorType::E2);

    //E3: A transition '%s' is not represented in the alphabet
    for (const auto & [from,alpha,to] : transitions)
    {
        if (!beIn(from, fsm_raw[STATE])) throw MyError(ErrorType::E1, from);
        if (!beIn(alpha, fsm_raw[ALPHABET])) throw MyError(ErrorType::E3, alpha);
        if (!beIn(to, fsm_raw[STATE])) throw MyError(ErrorType::E1, to);
    }
    //E4: Initial state is not defined
    if (fsm_raw[INITIAL].empty()) throw MyError(ErrorType::E4);

    //E5: FSA is nondeterministic
    if (!isDeterm()) throw MyError(ErrorType::E5);

}

bool FSM::isDeterm() const
{
    for (const std::string & from_name: fsm_raw[STATE])
    {
        size_t indx_from = states_dictinary.at(from_name);
        std::vector <std::string> trans = fsm_graph[indx_from];
        for (size_t i = 0; i < trans.size(); ++i)
        {
            if (trans[i] == EPS) return false;
            for (size_t j = i+1; j < trans.size(); ++j)
                if (trans[i] == trans[j]) return false;
        }
    }
    return true;
}

bool FSM::isNotDisjoint() const
{
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

bool FSM::correctStr(const string &str, const std::vector<char> &extension)
{
    for (const char& symb: str)
    {
        if (isLetter(symb)) continue;
        if (isNumber(symb)) continue;
        if (beIn(symb,extension)) continue;
        return false;
    }
    return true;
}

bool FSM::isLetter(const char ch)
{
    return ( ('a' <= ch) && (ch <= 'z') ) || ( ('A' <= ch) && (ch <= 'Z') );
}

bool FSM::isNumber(const char ch)
{
    return ('0' <= ch) && (ch <= '9');
}

const FSM::Graph &FSM::getGraph() const { return fsm_graph;}



