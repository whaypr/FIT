#ifndef __PROGTEST__
#include "sample.h"

namespace regexp {

Alternation::Alternation(RegExp left, RegExp right)
    : m_left(std::move(left))
    , m_right(std::move(right))
{
}
Concatenation::Concatenation(RegExp left, RegExp right)
    : m_left(std::move(left))
    , m_right(std::move(right))
{
}
Iteration::Iteration(RegExp node)
    : m_node(std::move(node))
{
}
Symbol::Symbol(alphabet::Symbol symb)
    : m_symbol(std::move(symb))
{
}


#if 1
/** @brief Example usage of std::variant visitor using overloaded struct and lambda functions */
void to_string(const regexp::RegExp& r, std::ostream& os)
{
    std::visit(overloaded{
                   [&os](const std::shared_ptr<regexp::Alternation>& arg) { os << '('; to_string(arg->m_left, os); os << '+'; to_string(arg->m_right, os); os << ')'; },
                   [&os](const std::shared_ptr<regexp::Concatenation>& arg) { os << '('; to_string(arg->m_left, os); os << ' '; to_string(arg->m_right, os); os << ')'; },
                   [&os](const std::shared_ptr<regexp::Iteration>& arg) { os << '('; to_string(arg->m_node, os); os << ")*"; },
                   [&os](const std::shared_ptr<regexp::Symbol>& arg) { os << arg->m_symbol; },
                   [&os](const std::shared_ptr<regexp::Epsilon>& arg) { os << "#E"; },
                   [&os](const std::shared_ptr<regexp::Empty>& arg) { os << "#0"; },
               },
               r);
}
#else
/** @brief Example usage of std::visitor without overloaded struct with lambdas, using std::holds_alternative and std::get instead */
void to_string(const regexp::RegExp& r, std::ostream& os)
{
    if (std::holds_alternative<std::shared_ptr<regexp::Alternation>>(r)) {
        const auto& node = std::get<std::shared_ptr<regexp::Alternation>>(r);
        os << '(';
        to_string(node->m_left, os);
        os << ' ';
        to_string(node->m_right, os);
        os << ')';
    } else if (std::holds_alternative<std::shared_ptr<regexp::Concatenation>>(r)) {
        const auto& node = std::get<std::shared_ptr<regexp::Concatenation>>(r);
        os << '(';
        to_string(node->m_left, os);
        os << ' ';
        to_string(node->m_right, os);
        os << ')';
    } else if (std::holds_alternative<std::shared_ptr<regexp::Iteration>>(r)) {
        const auto& node = std::get<std::shared_ptr<regexp::Iteration>>(r);
        os << '(';
        to_string(node->m_node, os);
        os << ")*";
    } else if (std::holds_alternative<std::shared_ptr<regexp::Symbol>>(r)) {
        os << std::get<std::shared_ptr<regexp::Symbol>>(r)->m_symbol;
    } else if (std::holds_alternative<std::shared_ptr<regexp::Epsilon>>(r)) {
        os << "#E";
    } else if (std::holds_alternative<std::shared_ptr<regexp::Empty>>(r)) {
        os << "#0";
    } else {
        __builtin_unreachable();
    }
}
#endif

/** @brief Convenience function for converting RegExps into the ALT string format (usable in the ALT tool) */
std::ostream& operator<<(std::ostream& os, const regexp::RegExp& regexp)
{
    to_string(regexp, os);
    return os;
}
}

namespace automaton {
/** @brief Convenience function for converting NFAs into the ALT string format (usable in the ALT tool) */
std::ostream& operator<<(std::ostream& os, const automaton::NFA& nfa)
{
    os << "NFA ";
    for (const auto& symb : nfa.m_Alphabet)
        os << symb << " ";
    os << '\n';

    for (const auto& state : nfa.m_States) {
        os << (nfa.m_InitialState == state ? ">" : " ") << (nfa.m_FinalStates.count(state) ? "<" : " ") << state;

        for (const auto& symbol : nfa.m_Alphabet) {
            if (auto iter = nfa.m_Transitions.find({state, symbol}); iter != nfa.m_Transitions.end()) {
                os << " ";
                for (auto i = iter->second.begin(); i != iter->second.end(); ++i) {
                    os << (i != iter->second.begin() ? "|" : "") << *i;
                }
            } else {
                os << " -";
            }
        }
        os << std::endl;
    }
    return os;
}

bool operator==(const automaton::NFA& a, const automaton::NFA& b)
{
    return std::tie(a.m_States, a.m_Alphabet, a.m_Transitions, a.m_InitialState, a.m_FinalStates) == std::tie(b.m_States, b.m_Alphabet, b.m_Transitions, b.m_InitialState, b.m_FinalStates);
}
}
#endif
//-----------------------------------------------------------------------------------------------------------------
using TKEY = size_t;

std::set<std::pair<TKEY, TKEY>> cartesian(const std::set<TKEY>& left, const std::set<TKEY>& right)
{
    std::set<std::pair<TKEY, TKEY>> res;
    for (const auto & l : left)
        for (const auto & r : right)
            res.insert( std::make_pair(l, r) );
    return res;
}

void createTables(const regexp::RegExp& regexp, std::map<TKEY, automaton::State>& tstates, std::map<TKEY, alphabet::Symbol>& tsymbols)
{
    std::visit( overloaded{
            [&tstates, &tsymbols](const std::shared_ptr<regexp::Alternation>& arg)   { createTables(arg->m_left, tstates, tsymbols); createTables(arg->m_right, tstates, tsymbols); },
            [&tstates, &tsymbols](const std::shared_ptr<regexp::Concatenation>& arg) { createTables(arg->m_left, tstates, tsymbols); createTables(arg->m_right, tstates, tsymbols); },
            [&tstates, &tsymbols](const std::shared_ptr<regexp::Iteration>& arg)     { createTables(arg->m_node, tstates, tsymbols); },
            [&tstates, &tsymbols](const std::shared_ptr<regexp::Symbol>& arg)        { tsymbols[(TKEY)(arg.get())] = arg->m_symbol; tstates[(TKEY)(arg.get())] = tstates.size() + 1; },
                               [](const std::shared_ptr<regexp::Epsilon>& arg)       { return; },
                               [](const std::shared_ptr<regexp::Empty>& arg)         { return; },
    }, regexp );
}

bool hasEpsilon(const regexp::RegExp& regexp)
{
    bool res;

    std::visit( overloaded{
            [&res](const std::shared_ptr<regexp::Alternation>& arg)   { res = hasEpsilon(arg->m_left) || hasEpsilon(arg->m_right); },
            [&res](const std::shared_ptr<regexp::Concatenation>& arg) { res = hasEpsilon(arg->m_left) && hasEpsilon(arg->m_right); },
            [&res](const std::shared_ptr<regexp::Iteration>& arg)     { res = true; },
            [&res](const std::shared_ptr<regexp::Symbol>& arg)        { res = false; },
            [&res](const std::shared_ptr<regexp::Epsilon>& arg)       { res = true; },
            [&res](const std::shared_ptr<regexp::Empty>& arg)         { res = false; },
    }, regexp );

    return res;
}

std::set<TKEY> findStarts(const regexp::RegExp& regexp)
{
    std::set<TKEY> res;

    std::visit( overloaded{
        [&res](const std::shared_ptr<regexp::Alternation>& arg)   {
            res = findStarts(arg->m_left);
            std::set<TKEY> r = findStarts(arg->m_right);
            res.insert(r.begin(), r.end());
        },
        [&res](const std::shared_ptr<regexp::Concatenation>& arg) {
            if (hasEpsilon(arg->m_left))
                res = findStarts( regexp::RegExp{ std::make_shared<regexp::Alternation>(arg->m_left, arg->m_right) } );
            else
                res = findStarts(arg->m_left);
        },
        [&res](const std::shared_ptr<regexp::Iteration>& arg)     { res = findStarts(arg->m_node); },
        [&res](const std::shared_ptr<regexp::Symbol>& arg)        { res = std::set<TKEY>{(TKEY)(arg.get())}; },
        [&res](const std::shared_ptr<regexp::Epsilon>& arg)       { res = std::set<TKEY>(); },
        [&res](const std::shared_ptr<regexp::Empty>& arg)         { res = std::set<TKEY>(); },
    }, regexp );

    return res;
}

std::set<TKEY> findEnds(const regexp::RegExp& regexp)
{
    std::set<TKEY> res;

    std::visit( overloaded{
        [&res](const std::shared_ptr<regexp::Alternation>& arg)   {
            res = findEnds(arg->m_left);
            std::set<TKEY> r = findEnds(arg->m_right);
            res.insert(r.begin(), r.end());
        },
        [&res](const std::shared_ptr<regexp::Concatenation>& arg) {
            if (hasEpsilon(arg->m_right))
                res = findEnds( regexp::RegExp{ std::make_shared<regexp::Alternation>(arg->m_left, arg->m_right) } );
            else
                res = findEnds(arg->m_right);
        },
        [&res](const std::shared_ptr<regexp::Iteration>& arg)     { res = findEnds(arg->m_node); },
        [&res](const std::shared_ptr<regexp::Symbol>& arg)        { res = std::set<TKEY>{(TKEY)(arg.get())}; },
        [&res](const std::shared_ptr<regexp::Epsilon>& arg)       { res = std::set<TKEY>(); },
        [&res](const std::shared_ptr<regexp::Empty>& arg)         { res = std::set<TKEY>(); },
    }, regexp );

    return res;
}

std::set<std::pair<TKEY, TKEY>> findNeighbors(const regexp::RegExp& regexp)
{
    std::set<std::pair<TKEY, TKEY>> res;

    std::visit( overloaded{
        [&res](const std::shared_ptr<regexp::Alternation>& arg)   {
            res = findNeighbors(arg->m_left);
            std::set<std::pair<TKEY, TKEY>> r = findNeighbors(arg->m_right);
            res.insert(r.begin(), r.end());
        },
        [&res](const std::shared_ptr<regexp::Concatenation>& arg) {
            res = findNeighbors( regexp::RegExp{ std::make_shared<regexp::Alternation>(arg->m_left, arg->m_right) } );
            std::set<std::pair<TKEY, TKEY>> cartes = cartesian( findEnds(arg->m_left), findStarts(arg->m_right) );
            res.insert(cartes.begin(), cartes.end());
        },
        [&res](const std::shared_ptr<regexp::Iteration>& arg)     {
            res = findNeighbors(arg->m_node);
            std::set<std::pair<TKEY, TKEY>> cartes = cartesian( findEnds(arg->m_node), findStarts(arg->m_node) );
            res.insert(cartes.begin(), cartes.end());
        },
        [&res](const std::shared_ptr<regexp::Symbol>& arg)        { res = std::set<std::pair<TKEY, TKEY>>(); },
        [&res](const std::shared_ptr<regexp::Epsilon>& arg)       { res = std::set<std::pair<TKEY, TKEY>>(); },
        [&res](const std::shared_ptr<regexp::Empty>& arg)         { res = std::set<std::pair<TKEY, TKEY>>(); },
    }, regexp );

    return res;
}

/** @brief Function for converting RegExps into NFAs using Glushkov's construction algorithm */
automaton::NFA convert(const regexp::RegExp& regexp)
{
    automaton::NFA res;
    const automaton::State INIT_STATE = 0;

    // translation tables for states and symbols
    std::map<TKEY, automaton::State> tstates;
    std::map<TKEY, alphabet::Symbol> tsymbols;
    createTables(regexp, tstates, tsymbols);

    // states
    res.m_States = {INIT_STATE};
    for (const auto & item : tstates)
        res.m_States.insert(item.second);

    // alphabet
    for (const auto & item : tsymbols)
        res.m_Alphabet.insert(item.second);

    // init state
    res.m_InitialState = INIT_STATE;

    // final states
    if ( hasEpsilon(regexp) ) res.m_FinalStates.insert(INIT_STATE);
    for ( const auto & id : findEnds(regexp) )
        res.m_FinalStates.insert( tstates[id] );

    // transitions
    for ( const auto & state : res.m_States ) // prepare empty sets
        for (const auto & symbol : res.m_Alphabet)
            res.m_Transitions[ std::make_pair(state, symbol) ] = std::set<automaton::State>();

    for ( const auto & id : findStarts(regexp) ) // insert transitions from start
        res.m_Transitions[ std::make_pair(INIT_STATE, tsymbols[id]) ].insert( tstates[id] );

    for ( const auto & pair : findNeighbors(regexp) ) // insert other transitions
        res.m_Transitions[ std::make_pair(tstates[pair.first], tsymbols[pair.second]) ].insert( tstates[pair.second] );

    return res;
}
//-----------------------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__
regexp::RegExp tests[] = {
    std::make_shared<regexp::Iteration>(
        std::make_shared<regexp::Concatenation>(
            std::make_shared<regexp::Iteration>(
                std::make_shared<regexp::Alternation>(
                    std::make_shared<regexp::Symbol>('a'),
                    std::make_shared<regexp::Symbol>('b'))),
            std::make_shared<regexp::Concatenation>(
                std::make_shared<regexp::Symbol>('a'),
                std::make_shared<regexp::Concatenation>(
                    std::make_shared<regexp::Symbol>('b'),
                    std::make_shared<regexp::Iteration>(
                        std::make_shared<regexp::Alternation>(
                            std::make_shared<regexp::Symbol>('a'),
                            std::make_shared<regexp::Symbol>('b'))))))),

    std::make_shared<regexp::Iteration>(
        std::make_shared<regexp::Alternation>(
            std::make_shared<regexp::Alternation>(
                std::make_shared<regexp::Symbol>('a'),
                std::make_shared<regexp::Epsilon>()),
            std::make_shared<regexp::Alternation>(
                std::make_shared<regexp::Symbol>('b'),
                std::make_shared<regexp::Empty>()))),

    std::make_shared<regexp::Concatenation>(
        std::make_shared<regexp::Iteration>(
            std::make_shared<regexp::Concatenation>(
                std::make_shared<regexp::Symbol>('a'),
                std::make_shared<regexp::Concatenation>(
                    std::make_shared<regexp::Alternation>(
                        std::make_shared<regexp::Empty>(),
                        std::make_shared<regexp::Epsilon>()),
                    std::make_shared<regexp::Concatenation>(
                        std::make_shared<regexp::Iteration>(
                            std::make_shared<regexp::Symbol>('b')),
                        std::make_shared<regexp::Symbol>('a'))))),
        std::make_shared<regexp::Alternation>(
            std::make_shared<regexp::Concatenation>(
                std::make_shared<regexp::Symbol>('b'),
                std::make_shared<regexp::Iteration>(
                    std::make_shared<regexp::Symbol>('c'))),
            std::make_shared<regexp::Concatenation>(
                std::make_shared<regexp::Iteration>(
                    std::make_shared<regexp::Symbol>('a')),
                std::make_shared<regexp::Alternation>(
                    std::make_shared<regexp::Epsilon>(),
                    std::make_shared<regexp::Concatenation>(
                        std::make_shared<regexp::Symbol>('b'),
                        std::make_shared<regexp::Empty>()))))),
};

// Your solution might give another (equivalent) answers. See task details.
automaton::NFA results[] = {
    {{0, 1, 2, 3, 4, 5, 6},
     {'a', 'b'},
     {
         {{0, 'a'}, {1, 3}},
         {{0, 'b'}, {2}},
         {{1, 'a'}, {1, 3}},
         {{1, 'b'}, {2}},
         {{2, 'a'}, {1, 3}},
         {{2, 'b'}, {2}},
         {{3, 'a'}, {}},
         {{3, 'b'}, {4}},
         {{4, 'a'}, {1, 3, 5}},
         {{4, 'b'}, {2, 6}},
         {{5, 'a'}, {1, 3, 5}},
         {{5, 'b'}, {2, 6}},
         {{6, 'a'}, {1, 3, 5}},
         {{6, 'b'}, {2, 6}},
     },
     0,
     {0, 4, 5, 6}},

    {{0, 1, 2},
     {'a', 'b'},
     {
         {{0, 'a'}, {1}},
         {{0, 'b'}, {2}},
         {{1, 'a'}, {1}},
         {{1, 'b'}, {2}},
         {{2, 'a'}, {1}},
         {{2, 'b'}, {2}},
     },
     0,
     {0, 1, 2}},

    {{0, 1, 2, 3, 4, 5, 6, 7},
     {'a', 'b', 'c'},
     {
         {{0, 'a'}, {1, 6}},
         {{0, 'b'}, {4, 7}},
         {{0, 'c'}, {}},
         {{1, 'a'}, {3}},
         {{1, 'b'}, {2}},
         {{1, 'c'}, {}},
         {{2, 'a'}, {3}},
         {{2, 'b'}, {2}},
         {{2, 'c'}, {}},
         {{3, 'a'}, {1, 6}},
         {{3, 'b'}, {4, 7}},
         {{3, 'c'}, {}},
         {{4, 'a'}, {}},
         {{4, 'b'}, {}},
         {{4, 'c'}, {5}},
         {{5, 'a'}, {}},
         {{5, 'b'}, {}},
         {{5, 'c'}, {5}},
         {{6, 'a'}, {6}},
         {{6, 'b'}, {7}},
         {{6, 'c'}, {}},
         {{7, 'a'}, {}},
         {{7, 'b'}, {}},
         {{7, 'c'}, {}},
     },
     0,
     {0, 3, 4, 5, 6}},
};

int main()
{
    assert(convert(tests[0]) == results[0]);
    assert(convert(tests[1]) == results[1]);
    assert(convert(tests[2]) == results[2]);
}
#endif
