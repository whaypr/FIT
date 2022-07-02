#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <variant>
#include <vector>

/** @brief Helper templates for visitors. See https://en.cppreference.com/w/cpp/utility/variant/visit */
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

/** @brief Helper templates for visitors. See https://en.cppreference.com/w/cpp/utility/variant/visit */
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;


namespace alphabet {
using Symbol = char;
}

namespace automaton {
using State = int;

/* @brief Represents a nondeterministic finite automaton (a standard 5-tuple (Q, \Sigma, \delta, q0, F)).
 *
 * Note that this structure *does not validate the automaton*. It is possible to store invalid automata in the struct.
 */
struct NFA {
    std::set<State> m_States;
    std::set<alphabet::Symbol> m_Alphabet;
    std::map<std::pair<State, alphabet::Symbol>, std::set<State>> m_Transitions;
    State m_InitialState;
    std::set<State> m_FinalStates;
};
}

namespace regexp {
struct Alternation;
struct Concatenation;
struct Iteration;
struct Symbol;
struct Epsilon;
struct Empty;

/** @brief Represents a RegExp node */
using RegExp = std::variant<std::shared_ptr<Alternation>, std::shared_ptr<Concatenation>, std::shared_ptr<Iteration>, std::shared_ptr<Symbol>, std::shared_ptr<Epsilon>, std::shared_ptr<Empty>>;

/** @brief Represents an alternation of two regexps */
struct Alternation {
    Alternation(RegExp left, RegExp right);
    RegExp m_left, m_right;
};

/** @brief Represents a concatenation of two regexps */
struct Concatenation {
    Concatenation(RegExp left, RegExp right);
    RegExp m_left, m_right;
};

/** @brief Represents an iteration regexp */
struct Iteration {
    Iteration(RegExp node);
    RegExp m_node;
};

/** @brief Represents a symbol regexp */
struct Symbol {
    Symbol(alphabet::Symbol symb);
    alphabet::Symbol m_symbol;
};

/** @brief Represents an empty string regexp */
struct Epsilon {
};

/** @brief Represents an empty regexp */
struct Empty {
};

}
