#pragma once

#include <Grammar.h>

/* An easy way to convert char to
 * Symbol in order to use earley algorithm
 * implementation. If the char is capital
 * then the symbol gonna be non-terminal.
 */
Symbol CharToSymbol(char ch);

template <typename Iterator>
GrammarRule ParseGrammarRule(Iterator& input) {
    std::string initial = *input;
    ++input;

    assert(initial.size() == 1);
    GrammarRule rule;
    rule.initial = CharToSymbol(initial[0]);

    std::string result = *input;
    ++input;

    for(char ch : result) {
        rule.result.push_back(CharToSymbol(ch));
    }

    return rule;
}

template <typename Iterator>
Grammar ParseGrammar(Iterator& input) {
    /* first parameter -- grammar size */
    size_t grammarSize = std::stoi(*input);
    ++input;

    Grammar grammar;
    for(size_t i = 0; i < grammarSize; ++i) {
        GrammarRule rule = ParseGrammarRule(input);
        grammar.grammarSet.push_back(rule);
    }

    return grammar;
}
