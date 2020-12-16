#include <catch2/catch.hpp>

#include <algorithm>
#include <fstream>
#include <iterator>
#include <Earley.h>

TEST_CASE("TestGrammarRuleParsing") {
    std::fstream file("grammars/test1.txt");
    INFO("File is open: " << file.is_open());

    std::istream_iterator<std::string> input(file);
    auto grammarFetched = ParseGrammar(input);
    REQUIRE(grammarFetched.grammarSet.size() == 6);

    Symbol S = CharToSymbol('S');
    Symbol T = CharToSymbol('T');
    Symbol PLUS = CharToSymbol('+');
    Symbol MULT = CharToSymbol('*');
    Symbol F = CharToSymbol('F');
    Symbol A_TERM = CharToSymbol('a');
    Symbol LEFT = CharToSymbol('(');
    Symbol RIGHT = CharToSymbol(')');

    std::list<GrammarRule> grammarRules = {
            {S, {T, PLUS, S}},
            {S, {T}},
            {T, {F, MULT, T}},
            {T, {F}},
            {F, {LEFT, S, RIGHT}},
            {F, {A_TERM}}
    };

    bool equal = std::equal(grammarFetched.grammarSet.begin(),
                            grammarFetched.grammarSet.end(),
                            grammarRules.begin());
    REQUIRE(equal);
}
