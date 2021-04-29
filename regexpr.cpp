#include "regexpr.h"

#include <cassert>

RegExpr::RegExpr(const FSM &fsm)
    :N(fsm.getGraph().size())
{
    R.reserve(N+1);
    for (int32_t k = -1; k < int32_t(N); ++k)
    {
        R[k].resize(N, Vector(N,""));
    }
    initialStep(fsm.getGraph());

    // iteratively pumping our expressions
    // steps 0..n
    for(size_t k=0; k < N; k++)
        for(size_t i=0; i < N; i++)
            for(size_t j=0; j < N; j++)
                R[k][i][j] = '('+R[k-1][i][k]+")("+R[k-1][k][k]+")*("+R[k-1][k][j]+")|("+R[k-1][i][j]+')';

    uint32_t initial = fsm.getInitial();
    for(uint32_t final: fsm.getAccepting())
        expression += R[N-1][initial][final]+'|';
    if (!expression.size())
        expression += FSM::EMPTY + "|";
    expression.pop_back();
}



void RegExpr::initialStep(const FSM::Graph& graph)
{
    const int32_t LAYER = -1;
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {

            for(const auto& alpha: graph[i][j])
                R[LAYER][i][j] += alpha + "|";

            if (i == j)
                R[LAYER][i][j] += FSM::EPS + "|";

            if (!R[LAYER][i][j].size()) // doesn't exist transition
                R[LAYER][i][j] += FSM::EMPTY + "|";

            assert( R[LAYER][i][j].size() && "expr must not be empty");
            R[LAYER][i][j].pop_back();
        }
    }
}

const RegExpr::Expression &RegExpr::getExpression() const
{
    return expression;
}
