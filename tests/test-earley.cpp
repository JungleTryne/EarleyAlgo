#include <catch2/catch.hpp>

#include <iostream>
#include <fstream>
#include <iterator>
#include <Earley.h>

Earley LoadGrammarFromFile(const std::string& path) {
    std::fstream file(path);
    CHECK(file.is_open());

    std::istream_iterator<std::string> input(file);
    Grammar grammar = ParseGrammar(input);
    Earley earleyHandler{std::move(grammar)};
    return earleyHandler;
}

TEST_CASE("ArithmeticGrammarCheck") {
    Earley earleyHandler = LoadGrammarFromFile("grammars/test1.txt");

    std::string firstWord = "(a+a)";
    std::string secondWord = "a";
    std::string thirdWord = "(a+a)*(a+a)*(a+a)*a";

    std::string wordToReject = "()";

    bool firstResult = earleyHandler.accept(firstWord);
    REQUIRE(firstResult);

    bool secondResult = earleyHandler.accept(secondWord);
    REQUIRE(secondResult);

    bool thirdResult = earleyHandler.accept(thirdWord);
    REQUIRE(thirdResult);

    bool fourthResult = earleyHandler.accept(wordToReject);
    REQUIRE_FALSE(fourthResult);
}

TEST_CASE("HomeworkGrammarCheck") {
    Earley earleyHandler = LoadGrammarFromFile("grammars/test2.txt");

    std::string wordToAccept = "caabba";
    std::string wordToReject = "ccccc";

    bool firstResult = earleyHandler.accept(wordToAccept);
    REQUIRE(firstResult);

    bool secondResult = earleyHandler.accept(wordToReject);
    REQUIRE_FALSE(secondResult);
}

TEST_CASE("EpsilonGrammarCheck") {
    Earley earleyHandler = LoadGrammarFromFile("grammars/test3.txt");

    std::string wordToAccept = "bbcca";
    bool firstResult = earleyHandler.accept(wordToAccept);
    REQUIRE(firstResult);

    std::string wordToDecline = "aaaaaa";
    bool secondResult = earleyHandler.accept(wordToDecline);
    REQUIRE_FALSE(secondResult);

    std::string epsilonWord = "";
    bool thirdResult = earleyHandler.accept(epsilonWord);
    REQUIRE(thirdResult);
}

TEST_CASE("StressTestParentheses") {
    Earley earleyHandler = LoadGrammarFromFile("grammars/test4.txt");

    std::string longString;

    std::fstream file("words/longPar.txt");
    file >> longString;

    bool result = earleyHandler.accept(longString);
    REQUIRE(result);
}
