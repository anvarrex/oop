#include <iostream>

using namespace std;

class Parent {
public:
    Parent() {
        printf("Parent()\n");
    }
    virtual void method1() {
        printf("Parent method1\n");
    }
    void method2() {
        printf("Parent method2\n");
    }
    virtual ~Parent() { // виртульный деструктор
        printf("~Parent()\n");
    }
};

class Child : public Parent { 
private:
    int* arr;
public:
    Child() {
        arr = new int[10];
        printf("Child()\n");
    }
    void method1() override {
        printf("Child method1\n");
    }
    void method2() {
        printf("Child method2\n");
    }
    ~Child() override { // переопределение деструктора
        delete arr;
        printf("~Child()\n");
    }
};

int main() {
    {
        Child child1;
        child1.method1(); //переопределенный метод1
        child1.method2(); //переопределенный метод2   
    }
    printf("------\n");

    Child* ChildPtr = new Child();

    ChildPtr->method1();
    ChildPtr->method2();

    delete ChildPtr;

    printf("------\n");

    Parent* ParentPtr = new Child();

    ParentPtr->method1();
    ParentPtr->method2();
    delete ParentPtr;

    return 0;
}