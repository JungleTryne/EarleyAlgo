#include <Earley.h>
#include <algorithm>

void Earley::scan(size_t depth) {
    if(depth < 1) return;

    assert(pool_.size() > depth);
    auto& currentPool = pool_[depth - 1];
    auto& nextPool = pool_[depth];

    auto currentSituations = currentPool.getSituations();
    for(const auto& situation : currentSituations) {
        auto ruleResult = situation.rule.result;
        Symbol nextSymbol = ruleResult[situation.alreadyProcessed];

        if(nextSymbol.terminal && nextSymbol.symbol == currentWord_[depth - 1]) {

            nextPool.addSituation(EarlySituation{
                situation.rule,
                situation.alreadyProcessed + 1,
                situation.position
            });
        }
    }
}

void Earley::predict(size_t depth) {
    auto& currentPool = pool_[depth];

    auto currentSituations = currentPool.getSituations();
    for(const auto& situation : currentSituations) {
        auto ruleResult = situation.rule.result;
        Symbol nextSymbol = ruleResult[situation.alreadyProcessed];

        if(nextSymbol.terminal) continue;   //we are looking for non-terminal

        for(const GrammarRule& rule : currentGrammar_.grammarSet) {
            if(rule.initial != nextSymbol) continue;
            currentPool.addSituation(EarlySituation{rule, 0, depth});

            if(isNullable(rule)) {  //handling a rule [A -> \eps]
                EarlySituation shiftedCurrent = situation;
                ++shiftedCurrent.alreadyProcessed;
                currentPool.addSituation(shiftedCurrent);
            }
        }
    }
}

void Earley::complete(size_t depth) {
    auto& currentPool = pool_[depth];

    auto currentSituations =  currentPool.getSituations();
    for(const auto& situation : currentSituations) {
        auto ruleResult = situation.rule.result;

        if(situation.alreadyProcessed != ruleResult.size()) continue;

        auto currentUpperSituations = pool_[situation.position].getSituations();
        for(const auto& upperSit : currentUpperSituations) {
            auto upperRuleResult = upperSit.rule.result;
            Symbol upperNextSymbol = upperRuleResult[upperSit.alreadyProcessed];

            if(upperNextSymbol != situation.rule.initial) continue;

            currentPool.addSituation(EarlySituation{
                    upperSit.rule,
                    upperSit.alreadyProcessed + 1,
                    upperSit.position
            });
        }
    }
}

bool Earley::accept(std::string_view word) {
    currentWord_ = word;
    preparePool(word.size() + 1);

    for(size_t depth = 0; depth < word.size() + 1; ++depth) {
        scan(depth);
        size_t currentPoolSize = pool_[depth].getSize();
        size_t prevPoolSize;

        do {
            prevPoolSize = currentPoolSize;
            predict(depth);
            complete(depth);

            currentPoolSize = pool_[depth].getSize();
        } while(prevPoolSize != currentPoolSize);
    }

    return findFinalSituation();
}

void Earley::preparePool(size_t poolSize) {
    pool_.clear();
    pool_.resize(poolSize);

    pool_[0].addSituation(EarlySituation{*currentGrammar_.grammarSet.begin(), 0, 0});
}

bool Earley::findFinalSituation() const {
    EarlySituation situationToFind{*currentGrammar_.grammarSet.begin(), 1, 0};

    auto lastPoolSituations = pool_.back().getSituations();
    return std::find(lastPoolSituations.begin(), lastPoolSituations.end(), situationToFind) !=
           lastPoolSituations.end();
}

Earley::Earley(const Grammar &grammar)  : currentGrammar_(grammar) {
    /* adding rule S' -> S */
    Symbol surTerminal{true, 'K'};
    Symbol startSymbol = currentGrammar_.grammarSet.begin()->initial;

    currentGrammar_.grammarSet.push_front(GrammarRule{
        surTerminal, {startSymbol}
    });
}

Earley::Earley(Grammar&& grammar) : currentGrammar_(std::move(grammar)) {
    /* adding rule S' -> S */
    Symbol surTerminal{true, 'K'};
    Symbol startSymbol = currentGrammar_.grammarSet.begin()->initial;

    currentGrammar_.grammarSet.push_front(GrammarRule{
            surTerminal, {startSymbol}
    });
}

bool Earley::isNullable(const GrammarRule &rule) const {
    return rule.result.size() == 1 && rule.result[0] == CharToSymbol(EPSILON);
}

bool EarlySituation::operator==(const EarlySituation &other) const {
    return rule == other.rule && alreadyProcessed == other.alreadyProcessed &&
        position == other.position;
}

void SituationsPool::addSituation(EarlySituation situation) {
    if(std::find(container_.begin(), container_.end(), situation) != container_.end()) return;
    container_.push_back(std::move(situation));
}

size_t SituationsPool::getSize() const {
    return container_.size();
}

std::vector<EarlySituation> SituationsPool::getSituations() const {
    return container_;
}
