#include "ExpUniform.h"

MulExp::MulExp(UnaryExpPtr leftOperand, vector<TokenType> operators, vector<UnaryExpPtr> operands)
        : ExpUniform<UnaryExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
    name = "<MulExp>";
    print();
}

AddExp::AddExp(MulExpPtr leftOperand, vector<TokenType> operators, vector<MulExpPtr> operands)
        : ExpUniform<MulExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
    name = "<AddExp>";
    print();
}

RelExp::RelExp(AddExpPtr leftOperand, vector<TokenType> operators, vector<AddExpPtr> operands)
        : ExpUniform<AddExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
    name = "<RelExp>";
    print();
}

LAndExp::LAndExp(EqExpPtr leftOperand, vector<TokenType> operators, vector<EqExpPtr> operands)
        : ExpUniform<EqExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
    name = "<LAndExp>";
    print();
}

LOrExp::LOrExp(LAndExpPtr leftOperand, vector<TokenType> operators, vector<LAndExpPtr> operands)
        : ExpUniform<LAndExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
    name = "<LOrExp>";
    print();
}

EqExp::EqExp(RelExpPtr leftOperand, vector<TokenType> operators, vector<RelExpPtr> operands)
        : ExpUniform<RelExpPtr>(std::move(leftOperand), std::move(operators), std::move(operands)) {
    name = "<EqExp>";
    print();
}
