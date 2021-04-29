#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include <sstream>

#include "fsm.h"
#include "regexpr.h"
#include "myerror.h"


int main()
{

    std::string path = "/home/vorart/workspace/qt projects/TCS_RE_translator/input1.txt";

    std::vector<std::string> input_str;
    input_str.reserve(FSM::Attribute::SIZE);
    std::string	line;
    std::ifstream in(path); // opening the file for reading
    assert(in.is_open() && "file is not found");
    {
        for (int i=0; getline(in, line,'\n'); i++)
            input_str.emplace_back(std::move(line));
    }
    in.close();     // closing the file

    for (const auto& i: input_str)
        std::cout << i << endl;

    std::stringstream stream;
    try {
        FSM fsm(input_str);
        RegExpr regexpr(fsm);
        stream << regexpr.getExpression()<< std::endl;
    } catch (const MyError& err) {
        std::cout << "Error:\n\t";
        std::cout << err.what() << std::endl;
    } catch (...) {
        std::cout << "somthing go wrong";
    }
    std::ofstream fout("/home/vorart/workspace/qt projects/TCS_RE_translator/output.txt");
    fout << stream.str();
    fout.close();
    return 0;
}
