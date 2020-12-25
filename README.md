# Earley parsing algorithm

With given context-free grammar the class is 
to accept or reject the given word.

### How to use the class?

```c++
#include <iostream>
#include <Earley.h>

int main() {
    std::istream_iterator<std::string> input(std::cin);

    Grammar grammar = ParseGrammar(input); //gets input iterator in order to fetch the grammar
    Earley earleyHandler{std::move(grammar)}; //grammar handler using Earley algorithm 
    
    ...

    std::string word = "abacaba";
    bool accepted = earleyHandler.accept(word); // <-- here you see if the word accepted by the grammar

    return 0;
}
```

### Description

The complexity of algorithm is ~~O(n^4)~~ O(n^3) yet. ~~It can be easily improved by modifying 'Pool' class,
as 'insert' method works with complexity O(n)~~. Now insert method is optimized to O(1) complexity

### How to build

```bash
./build_and_install.sh && ./EarleyTest
```
