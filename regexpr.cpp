#include "regexpr.h"



RegExpr::RegExpr(const FSM &fsm)
    :N(fsm.getGraph().size())
{
    const auto fsm_graph = fsm.getGraph();
    R.reserve(N+1);
    for (int32_t k = -1; k < int32_t(N); ++k)
    {
        R[k].reserve(N);
        for (size_t i = 0; i < N; ++i)
            R[k][i].reserve(N);
    }
    initialStep(fsm_graph);
}

void RegExpr::initialStep(const FSM::Graph& graph)
{
    const int32_t LAYER = -1;
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            R[LAYER][i][j];
        }
    }
}
