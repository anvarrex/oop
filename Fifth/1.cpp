// в базовом классе объявить метод невиртуальный, а в классе-потомке объявить метод с таким же именем:
// какой метод будет вызываться при обращении к объекту через указатель на базовый класс, через указатель на класс-потомок?


// в базовом классе объявить метод виртуальный, а в классе-потомке объявить метод с таким же именем:
// какой метод будет вызываться при обращении к объекту через указатель на базовый класс, через указатель на класс-потомок?

#include <iostream>

using namespace std;

class Parent {
public:
    void method1() {
        cout << "Метод1 родителя" << endl;
    }

    virtual void method2() {
        cout << "Метод2 родителя" << endl;
    }
};

class Child : public Parent {
public:
    void method1() {
        cout << "Метод1 ребенка" << endl;
    }

    void method2() override 
    {
        cout << "Метод2 ребенка" << endl;
    }
};

int main() {
    Child* Childptr = new Child;
    Childptr->method1();
    Childptr->method2();
    delete Childptr;

    printf("--------\n");

    Parent* ParentPtr = new Child;
    ParentPtr->method1();  // Вызов метода родительского класса
    ParentPtr->method2();  // Вызов метода дочернего класса
    return 0;
}