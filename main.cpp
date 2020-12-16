#include <iostream>
#include <iterator>
#include <Earley.h>

int main() {
    std::istream_iterator<std::string> input(std::cin);
    Grammar grammar = ParseGrammar(input);
    Earley earleyHandler{std::move(grammar)};

    std::string word;
    word = *input;

    bool result = earleyHandler.accept(word);
    std::cout << (result ? "ACCEPTED" : "REJECTED");
    return 0;
}
