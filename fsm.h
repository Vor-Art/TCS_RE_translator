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


struct FSM
{
    enum Attribute{
        STATE,
        ALPHABET,
        INITIAL,
        ACCEPTING,
        TRANS,
        SIZE
    };


    inline static const std::string EMPTY = "{}";
    inline static const std::string EPS = "eps";
    using Field = std::vector<std::string>;
    using Graph = std::vector<std::vector<std::string>>;

private:
    struct Trans{
        std::string from;
        std::string alpha;
        std::string to;
    };

    inline static const std::string Attribute_msg [Attribute::SIZE]{
        "states=[",
        "alpha=[",
        "initial=[",
        "accepting=[",
        "trans=[",
    };

    std::vector<Field> fsm_raw;     //for intermediate calculations
    std::vector<Trans> transitions; //for intermediate calculations
    std::unordered_map<std::string, uint32_t> states_dictinary;
//    std::unordered_map<std::string, uint32_t> alphabet_dictinary;
    Graph fsm_graph;

public:
    explicit FSM (const std::vector<std::string> &input_str) noexcept(false);
    const Graph &getGraph() const;
private:
    void fillFsmRaw(const std::vector<std::string> &input_str);
    void fillDictionary();
    void initGraph(size_t N);
    void fillGraph();
    void checkCondition() const noexcept(false);
    bool isDeterm()       const;
    bool isNotDisjoint()  const;
    template <typename T>
    static bool beIn(const T & item, const std::vector<T>& array);
    static bool correctStr(const std::string & str,const std::vector<char> & extension = {});
    static bool isLetter(const char ch);
    static bool isNumber(const char ch);
};



template <typename T>
bool FSM::beIn(const T &item, const std::vector<T> &array){
    for (const T & state_name: array)
        if (item == state_name)
            return true;
    return false;
}



#endif // FSM_H
