#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>


#include "fsm.h"
#include "myerror.h"

using namespace std;





int main()
{

    std::string path = "/home/vorart/workspace/qt projects/TCS_RE_translator/input2.txt";

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
        cout << i << endl;

    try {
        FSM fsm(input_str);
    } catch (const MyError& err) {
        std::cout << "Error:\n\t";
        auto tmp = err.what();
        std::cout << tmp << std::endl;
    }

    return 0;
}
