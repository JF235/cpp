#include <iostream>

class myAbstractClass {
    public:

    virtual void pureVirtualFunction() = 0;

    virtual void virtualFunction() {
        std::cout << "myAbstractClass::virtualFunction()" << std::endl;
    }
};

class myDerivedClass : public myAbstractClass {
    public:

    void pureVirtualFunction() override {
        std::cout << "myDerivedClass::pureVirtualFunction()" << std::endl;
    }

    void virtualFunction() override {
        std::cout << "myDerivedClass::virtualFunction()" << std::endl;
    }
};

struct myOtherDerivedClass : public myAbstractClass {
    // public:
    // Struct não precisa da palavra chave public

    void pureVirtualFunction() override {
        std::cout << "myOtherDerivedClass::pureVirtualFunction()" << std::endl;
    }
};

int main() {
    myDerivedClass derived;
    derived.pureVirtualFunction();
    derived.virtualFunction();

    std::cout << std::endl;

    myOtherDerivedClass otherDerived;
    otherDerived.pureVirtualFunction();
    otherDerived.virtualFunction();

    /* Output >>>
myDerivedClass::pureVirtualFunction()
myDerivedClass::virtualFunction()
myOtherDerivedClass::pureVirtualFunction()
myAbstractClass::virtualFunction()
    */
    return 0;
}
 