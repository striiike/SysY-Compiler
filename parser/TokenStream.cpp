//
// Created by hiccup on 2023/9/20.
//

#include <optional>
#include "TokenStream.h"

TokenStream::TokenStream(std::vector<Token>& tokens) : current(tokens.begin()), end(tokens.end()) {}

std::optional<Token> TokenStream::next() {
    if (current != end) {
        return *(current++);
    }
    return {};
}

std::optional<Token> TokenStream::peek(int n) {
    auto temp = current;
    std::advance(temp, n);
    if (temp != end) {
        return *temp;
    }
    return {};
}

bool TokenStream::reachEnd() const {
    return current == end;
}
