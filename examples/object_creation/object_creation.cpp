#include <iostream>

struct myClass {
    myClass() : number(10) {
        std::cout << "Constructor called" << std::endl;
    }

    ~myClass() {
        std::cout << "Destructor called" << std::endl;
    }

    void displayNumber() const {
        std::cout << "Number: " << number << std::endl;
    }

    private:
    int number;
};

int main(){
    myClass obj;
    obj.displayNumber();
    return 0;
}