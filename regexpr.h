#ifndef REGEXPR_H
#define REGEXPR_H
#include <string>
#include <vector>
#include <unordered_map>

#include "fsm.h"

class RegExpr
{
    struct Hash {
        std::size_t operator()(const int32_t& index) const {
            if (index < -1)
                throw std::runtime_error("index for hash in out of range [-1,∞)");
            return size_t(index + 1);
        }
    };
    using Expression = std::string;
    using Vector  = std::vector<Expression>;
    using Layer = std::vector<Vector>;
    using Matrix3D = std::unordered_map<int32_t,Layer,Hash>;
    Matrix3D R;
    const size_t N;
public:
    RegExpr(const FSM &fsm);
private:
    void initialStep(const FSM::Graph& graph);
};

#endif // REGEXPR_H
