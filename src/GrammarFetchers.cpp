#include <GrammarFetchers.h>

Symbol CharToSymbol(char ch) {
    return Symbol{!static_cast<bool>(std::isupper(ch)), ch};
}