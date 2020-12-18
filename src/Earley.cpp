#include <Earley.h>


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

            nextPool.addSituation(EarleySituation{
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
            currentPool.addSituation(EarleySituation{rule, 0, depth});

            if(isNullable(rule)) {  //handling a rule [A -> \eps]
                EarleySituation shiftedCurrent = situation;
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

            currentPool.addSituation(EarleySituation{
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

    pool_[0].addSituation(EarleySituation{*currentGrammar_.grammarSet.begin(), 0, 0});
}

bool Earley::findFinalSituation() const {
    EarleySituation situationToFind{*currentGrammar_.grammarSet.begin(), 1, 0};

    auto lastPoolSituations = pool_.back().getSituations();
    return std::find(lastPoolSituations.begin(), lastPoolSituations.end(), situationToFind) !=
           lastPoolSituations.end();
}

Earley::Earley(const Grammar &grammar)  : currentGrammar_(grammar) {
    /* adding rule S' -> S */
    Symbol surTerminal{true, EarleyGlobals::ADDITIONAL_NONTERMINAL};
    Symbol startSymbol = currentGrammar_.grammarSet.begin()->initial;

    currentGrammar_.grammarSet.push_front(GrammarRule{
        surTerminal, {startSymbol}
    });
}

Earley::Earley(Grammar&& grammar) : currentGrammar_(std::move(grammar)) {
    /* adding rule S' -> S */
    Symbol surTerminal{true, EarleyGlobals::ADDITIONAL_NONTERMINAL};
    Symbol startSymbol = currentGrammar_.grammarSet.begin()->initial;

    currentGrammar_.grammarSet.push_front(GrammarRule{
            surTerminal, {startSymbol}
    });
}

bool Earley::isNullable(const GrammarRule &rule) const {
    return rule.result.size() == 1 && rule.result[0] == CharToSymbol(EarleyGlobals::EPSILON);
}

bool EarleySituation::operator==(const EarleySituation &other) const {
    return rule == other.rule && alreadyProcessed == other.alreadyProcessed &&
        position == other.position;
}

size_t EarlySituationHasher::operator()(const EarleySituation &situation) const noexcept{
    //We will hash it as a string
    std::stringstream hashStream;
    hashStream << situation.rule.initial.symbol;

    size_t counter = 0;
    for(auto symbolObj : situation.rule.result) {
        if(counter == situation.alreadyProcessed) {
            hashStream << EarleyGlobals::CURRENT_POS_SYMBOL;
        }
        hashStream << symbolObj.symbol;
        ++counter;
    }

    hashStream << ':' << std::to_string(situation.position);
    std::string toHash = hashStream.str();
    return std::hash<std::string>()(std::move(toHash));
}

void SituationsPool::addSituation(EarleySituation situation) {
    container_.insert(situation);
}

size_t SituationsPool::getSize() const {
    return container_.size();
}

std::vector<EarleySituation> SituationsPool::getSituations() const {
    std::vector<EarleySituation> toBeReturned = {};
    for(const auto& elem : container_) {
        toBeReturned.push_back(elem);
    }
    return toBeReturned;
}
