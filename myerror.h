#ifndef MYERROR_H
#define MYERROR_H
#include <string>
#include <stdexcept>

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
        "E1: A state 's' is not in the set of states",
        "E2: Some states are disjoint",
        "E3: A transition 'a' is not represented in the alphabet",
        "E4: Initial state is not defined",
        "E5: FSA is nondeterministic"
    };
public:

    explicit MyError (ErrorType err_t)
        : err(err_t)
    {}

    const char* what() const noexcept
    {
        return MyError_msg [static_cast<size_t> (err)].c_str();
    }

};

#endif // MYERROR_H
