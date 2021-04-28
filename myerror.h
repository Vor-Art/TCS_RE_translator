#ifndef MYERROR_H
#define MYERROR_H
#include <string>
#include <stdexcept>
#include <memory>
enum class ErrorType
{
    E0,
    E1,
    E2,
    E3,
    E4,
    E5,
    SIZE
};

class MyError : public std::exception
{
    ErrorType err;

    const std::string MyError_msg [static_cast<size_t> (ErrorType::SIZE)]
    {
        "E0: Input file is malformed",
        "E1: A state '%s' is not in the set of states",
        "E2: Some states are disjoint",
        "E3: A transition '%s' is not represented in the alphabet",
        "E4: Initial state is not defined",
        "E5: FSA is nondeterministic"
    };
    std::string complement;
public:

    explicit MyError (ErrorType err_t, const std::string &complement = "")
        : err(err_t), complement(complement)
    {}

    const char* what() const noexcept
    {
        static std::string str;
        str = MyError_msg [static_cast<size_t> (err)];
        size_t n = str.find("%s");
        if (complement.size() && n != std::string::npos)
             str.replace(n,2,complement);
        return str.c_str();
    }

};

#endif // MYERROR_H
