#ifndef FSM_H
#define FSM_H

#include <string>
#include <sstream>
#include <vector>
#include <exception>
#include <iostream>
#include <unordered_map>
#include <queue>

#include "myerror.h"


using namespace std;


class FSM
{
public:
    enum Attribute{
        STATE,
        ALPHABET,
        INITIAL,
        ACCEPTING,
        TRANS,
        SIZE
    };

private:
    struct Trans{
        std::string from;
        std::string alpha;
        std::string to;
    };

    const std::string Attribute_msg [Attribute::SIZE]{
        "states=[",
        "alpha=[",
        "initial=[",
        "accepting=[",
        "trans=[",
    };

    using Field = std::vector<std::string>;

    std::vector <Field> fsm_raw;
    std::vector <Trans> transitions;
    std::unordered_map<std::string, std::vector<Trans>> fsm_graph;

    const std::string EPS = "eps";
public:
    explicit FSM (const std::vector<std::string> & input_str) noexcept(false);
private:
    template <typename T>
    static bool beIn(const T & item, const std::vector<T>& array);
    void checkCondition() noexcept(false);
    bool isDeterm();
    bool isNotDisjoint();
    static bool correctStr(const std::string & str,const std::vector<char> & extension = {});
    static bool isLetter(const char ch);
    static bool isNumber(const char ch);
};

template <typename T>
bool FSM::beIn(const T &item, const std::vector<T> &array){
    for (const T & state_name: array)
        if (item == state_name)
            return true;
    return false;
}



#endif // FSM_H
