#include <Grammar.h>

bool Symbol::operator==(const Symbol &other) const {
    return this->terminal == other.terminal &&
        this->symbol == other.symbol;
}

bool Symbol::operator!=(const Symbol &other) const {
    return !(*this == other);
}

bool GrammarRule::operator==(const GrammarRule &other) const {
    return initial == other.initial && result == other.result;
}
