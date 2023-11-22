//
// Created by hiccup on 2023/9/18.
//
//#include "./llvm-ir/Function.h"



#include "frontend/parser/Parser.h"
#include "frontend/parser/Exception.h"
#include "frontend/lexer/Lexer.h"
#include "config.h"
//#include "midend/MidEnd.h"
#include "backend/MipsParser.h"
#include "llvm-ir/IrBuilder.h"
#include "midend/MidEnd.h"

using namespace std;

bool PARSER_DISPLAY = true;
bool PARSER_SWITCH = false;

std::ofstream outfile("./output.txt");
std::ofstream errfile("./error.txt");
std::ofstream llvmfile("./llvm_ir.txt");
std::ofstream llvmfile_nop("./llvm_ir_nop.txt");
std::ofstream llvmfile_m2r("./llvm_ir_m2r.txt");
std::ofstream llvmfile_killPhi("./llvm_ir_killPhi.txt");
std::ofstream mipsfile("./mips.txt");

void parseLog(const std::string &str) {
	if (PARSER_DISPLAY && PARSER_SWITCH) {
		outfile << str << std::endl;
	}
}

map<Exception, char> exceptionToString = {
	{Exception::INVALID_CHARACTER, 'a'},
	{Exception::REDEFINED_IDENT, 'b'},
	{Exception::UNDEFINED_IDENT, 'c'},
	{Exception::PARAMS_NUM_UNMATCHED, 'd'},
	{Exception::PARAM_TYPE_UNMATCHED, 'e'},
	{Exception::VOID_RETURN_INT, 'f'},
	{Exception::INT_RETURN_LACKED, 'g'},
	{Exception::CONST_ASSIGNED, 'h'},
	{Exception::SEMICN_LACKED, 'i'},
	{Exception::RPARENT_LACKED, 'j'},
	{Exception::RBRACK_LACKED, 'k'},
	{Exception::FORMAT_CHAR_UNMATCHED, 'l'},
	{Exception::BREAK_CONTINUE_OUT_LOOP, 'm'},
	{Exception::UNDEFINED_ERROR, 'n'}
};

int main() {

	std::ifstream infile("testfile.txt");
//	std::ifstream infile("testfile.c");
	std::string code((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());

	Lexer lexer(code);
	std::vector<Token> tokens = lexer.analyze();

	TokenStream tokenStream(tokens);
	Parser parser(tokenStream);

	auto AST = parser.parseCompUnit();

	{
		auto ctx = make_shared<ErrorCtx>();
		auto ret = make_shared<ErrorRet>();
		AST.checkError(ctx, ret);
		std::sort(errorList.begin(), errorList.end(),
				  [](const auto &left, const auto &right) {
					  return left.second < right.second;
				  });
		errorList.erase(std::unique(errorList.begin(), errorList.end()), errorList.end());
		for (const auto &pair : errorList) {
			errfile << pair.second << " " << exceptionToString[pair.first] << std::endl;
		}
	}
	symbol.clear();

	AST.llvmIr();
//	cout << irBuilder.getModule()->toString() << endl;
	llvmfile_nop << irBuilder.getModule()->toString() << endl;

//	auto *mid = new MidEnd();
//	mid->run(irBuilder.getModule());

	(new DFBuilder(&(irBuilder.getModule()->functionList)))->run();
//	cout << irBuilder.getModule()->toString() << endl;
	(new Mem2Reg())->run(irBuilder.getModule());


	llvmfile << irBuilder.getModule()->toString() << endl;


	llvmfile_m2r << irBuilder.getModule()->toString() << endl;
//	cout << irBuilder.getModule()->toString() << endl;




	(new RemovePhi())->run(irBuilder.getModule());
	llvmfile_killPhi << irBuilder.getModule()->toString() << endl;

	auto mipsParser = new MipsParser(irBuilder.getModule());
	mipsParser->parseModule();

	mipsParser->mipsBuilder->mipsModule->print(cout);
	mipsParser->mipsBuilder->mipsModule->print(mipsfile);

	return 0;
}