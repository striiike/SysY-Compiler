//
// Created by hiccup on 2023/9/18.
//


#include <iostream>
#include <memory> // For std::shared_ptr
#include <vector>

class InitVal;
class ExpInitVal;
class ArrayInitVal;

using InitValPtr = std::shared_ptr<InitVal>;
using ExpInitValPtr = std::shared_ptr<ExpInitVal>;

class InitVal {
public:
    virtual void print(){

    }
};

class ExpInitVal : public InitVal {
private:
    bool isConst;
    int expPtr;
public:
    ExpInitVal(bool isConst, int expPtr) :
            isConst(isConst), expPtr((expPtr)) {}
//    void print() override {}
};

class ArrayInitVal : public InitVal {
private:
    bool isConst;
    std::vector<InitValPtr> initValPtrs;
public:
    explicit ArrayInitVal(bool isConst, std::vector<InitValPtr> initValPtrs) :
            isConst(isConst), initValPtrs(std::move(initValPtrs)) {}
};




//int main() {
//    ExpInitValPtr obb = std::make_shared<ExpInitVal>(true, 234);
//
//    InitValPtr obj = obb;
//    return 0;
//}




//
#include "driver/Driver.h"

    int main() {

    Driver driver;
    driver.runCompiler();
    return 0;
}