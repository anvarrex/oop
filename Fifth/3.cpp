// в методе1 базового класса вызывается метод2, который определен в этом же классе
// как невиртуальный, у класса-потомка метод2 переопределен: что происходит при вызове метода1 у класса-потомка?

#include <iostream>

using namespace std;

class Parent {
public:
    void method1() {
        cout << "Метод1 родителя" << endl;
        method2();
    }

    void method2() {
        cout << "Метод2 родителя" << endl;
    }
};

class Child : public Parent {
public:
    void method2() 
    {
        cout << "Метод2 ребенка" << endl;
    }
};

int main() {
    Child* childptr = new Child;
    childptr->method1();
    return 0;
}