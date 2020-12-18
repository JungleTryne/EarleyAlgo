#pragma once

#include <algorithm>
#include <sstream>

#include <Grammar.h>
#include <GrammarFetchers.h>

namespace EarleyGlobals {
    inline char const EPSILON = '#';
    inline char const CURRENT_POS_SYMBOL = '.';
    inline auto const ADDITIONAL_NONTERMINAL = 'K';
}

struct EarleySituation {
    const GrammarRule& rule;
    size_t alreadyProcessed;
    size_t position;

    bool operator==(const EarleySituation& other) const;
};

struct EarlySituationHasher {
    size_t operator()(const EarleySituation& situation) const noexcept;
};

class SituationsPool {
private:
    std::unordered_set<EarleySituation, EarlySituationHasher> container_;
public:
    void addSituation(EarleySituation situation);
    size_t getSize() const;
    std::vector<EarleySituation> getSituations() const;
};

class Earley {
private:
    Grammar currentGrammar_;
    std::string_view currentWord_;
    std::vector<SituationsPool> pool_;

    bool isNullable(const GrammarRule& rule) const;

    void preparePool(size_t poolSize);

    void scan(size_t depth);
    void predict(size_t depth);
    void complete(size_t depth);

    bool findFinalSituation() const;

public:
    Earley(const Grammar& grammar);
    Earley(Grammar&& grammar);

    bool accept(std::string_view word);
};
