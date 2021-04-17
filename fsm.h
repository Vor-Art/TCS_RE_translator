#ifndef FSM_H
#define FSM_H

#include <string>
#include <sstream>
#include <vector>
#include <exception>
#include <iostream>

#include "myerror.h"


using namespace std;

struct Trans{
    std::string from;
    std::string alpha;
    std::string to;
};

class FSM
{
    using Field = std::vector<std::string>;

    std::vector<Field> fsm;

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

public:
    explicit FSM (const std::vector<std::string> & input_str) noexcept(false){
        if (input_str.size()!=Attribute::SIZE) throw MyError(ErrorType::E0);
        fsm.reserve(Attribute::SIZE);

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
            while (getline(field_stream, line1, ',')){
                field.emplace_back(std::move(line1));
            }
            fsm.emplace_back(std::move(field));
        }
//        if (fsm[INITIAL].size() != 1) throw MyError(ErrorType::EX);
    }
private:

};

#endif // FSM_H
