// в базовом классе объявить метод невиртуальный, а в классе-потомке объявить метод с таким же именем: какой метод будет
// вызываться при обращении к объекту через указатель на базовый класс, через указатель на класс-потомок?

#include <iostream>
#include <string>

using namespace std;

class Parent {
public:
    virtual string classname() const {
        return "Parent";
    }

    virtual bool isA(const string& className) const {
        return className == "Parent";
    }
};

class Child : public Parent {
public:
    string classname() const override {
        return "Child";
    }

    bool isA(const string& className) const override {
        return className == "Child" || Parent::isA(className);
    }
};

int main() {
    Parent* ParentPtr = new Child();
    cout << "Classname: " << ParentPtr->classname() << endl;
    cout << "Is Child? " << (ParentPtr->isA("Child") ? "Yes" : "No") << endl;
    delete ParentPtr;
    return 0;
}