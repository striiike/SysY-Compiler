//
// Created by hiccup on 2023/10/20.
//

#ifndef COMPILER_LLVM_IR_TYPE_H
#define COMPILER_LLVM_IR_TYPE_H

#include <string>

class Type {
public:
	virtual std::string toString() { return "!ub!"; }
	virtual bool isVoid() { return false; }
	virtual bool isInt1() { return false; }
	virtual bool isInt32() { return false; }
	virtual bool isFunction() { return false; }
	virtual bool isArray() { return false; }
	virtual bool isPointer() { return false; }

	virtual Type *getTargetType() { return nullptr; }
	virtual Type *getEleType() { return nullptr; }
};

class IntegerType : public Type {
	int bitWidth;
public:
	explicit IntegerType(int width) : bitWidth(width) {}

	bool isVoid() override { return bitWidth == 0; }

	bool isInt1() override { return bitWidth == 1; }

	bool isInt32() override { return bitWidth == 32; }

	static IntegerType *VOID;
	static IntegerType *INT1;
	static IntegerType *INT32;

	std::string toString() override {
		if (bitWidth == 0)
			return "void";
		else if (bitWidth == 32)
			return "i32";
		return "!ub!";
	}
};

class ArrayType : public Type {
	int num;
	Type *type;
public:
	ArrayType(int num, Type *type) : num(num), type(type) {}

	int getNum() {
		return num;
	}

	Type *getEleType() override {
		return type;
	}

	bool isArray() override { return true; }

	std::string toString() override {
		return "[" + std::to_string(num) + " x " + type->toString() + "]";
	}
};

class PointerType : public Type {
	Type *targetType;
public:
	explicit PointerType(Type *type) : targetType(type) {}

	Type *getTargetType() override {
		return targetType;
	}

	bool isPointer() override { return true; }

	std::string toString() override {
		return targetType->toString() + "*";
	}
};

class FunctionType : public Type {
public:
	FunctionType() = default;

	bool isFunction() override { return true; }
};

#endif //COMPILER_LLVM_IR_TYPE_H
