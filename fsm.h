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
    struct Trans{
        std::string from;
        std::string alpha;
        std::string to;
    };

    enum Attribute{
        STATE,
        ALPHABET,
        INITIAL,
        ACCEPTING,
        TRANS,
        SIZE
    };

    const std::string Attribute_msg [Attribute::SIZE]{
        "states=[",
        "alpha=[",
        "initial=[",
        "accepting=[",
        "trans=[",
    };

    using Field = std::vector<std::string>;

    std::vector<Field> fsm_raw;
    std::vector <Trans> transitions;
    std::unordered_map<std::string, std::vector<Trans>> fsm_graph;

    const std::string EPS = "eps";
public:
    explicit FSM (const std::vector<std::string> & input_str) noexcept(false){
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
            std::cout << line << std::endl;
            std::string line1;
            std::stringstream field_stream(line);
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
private:
    void checkCondition() noexcept(false);

    template <typename T>
    static bool beIn(const T & item, const std::vector<T>& array){
        for (const T & state_name: array)
            if (item == state_name)
                return true;
        return false;
    }

    bool isDeterm();
    bool isNotDisjoint();
    static bool correctStr(const std::string & str,const std::vector<char> & extension = {});
    static bool isLetter(const char ch);
    static bool isNumber(const char ch);
};



#endif // FSM_H
