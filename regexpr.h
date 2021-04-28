#ifndef REGEXPR_H
#define REGEXPR_H
#include "fsm.h"

class RegExpr
{
    std::string expression;
public:
    RegExpr(const FSM& fsm);
};

#endif // REGEXPR_H
