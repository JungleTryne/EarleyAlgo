#pragma once

#include <cassert>
#include <string>
#include <list>
#include <vector>
#include <unordered_set>

struct Symbol {
    bool terminal;
    char symbol;

    bool operator==(const Symbol& other) const;
    bool operator!=(const Symbol& other) const;
};

/* Grammar rule of CONTEXT FREE grammar
 * initial -> result[List]
 */
struct GrammarRule {
    Symbol initial;
    std::vector<Symbol> result;

    bool operator==(const GrammarRule& other) const;
};

struct Grammar {
    std::list<GrammarRule> grammarSet;
};

