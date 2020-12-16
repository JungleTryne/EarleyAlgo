#pragma once

#include <Grammar.h>
#include <GrammarFetchers.h>

struct EarlySituation {
    const GrammarRule& rule;
    size_t alreadyProcessed;
    size_t position;

    bool operator==(const EarlySituation& other) const;
};

/* This is D_j */
//using SituationsPool = std::vector<EarlySituation>;

class SituationsPool {
private:
    std::vector<EarlySituation> container_;
public:
    void addSituation(EarlySituation situation);
    size_t getSize() const;
    std::vector<EarlySituation> getSituations() const;
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
