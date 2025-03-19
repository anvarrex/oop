#include <iostream>
using namespace std;

// Базовый класс
class Grandparent {
public:
    virtual void method() {
        cout << "Grandparent method" << endl;
    }
};

// Дочерний класс (наследник Grandparent)
class Parent : public Grandparent {
public:
    void method() override {
        cout << "Parent method" << endl;
    }
};

// Внучатый класс (наследник Parent)
class Child : public Parent {
public:
    void method() override {
        cout << "Child method" << endl;
    }
};

int main() {
    Grandparent* ptr1 = new Grandparent();
    Grandparent* ptr2 = new Parent();
    Grandparent* ptr3 = new Child();

    ptr1->method();
    ptr2->method(); 
    ptr3->method(); 

    delete ptr1;
    delete ptr2;
    delete ptr3;

    printf("---------\n");

    Parent* parentptr = new Child;
    parentptr->method();

    return 0;
}