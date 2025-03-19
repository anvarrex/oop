// Для проверки на принадлежность некоторому классу необходимо реализовать: 
// базовый виртуальный метод string classname(), перекрыть его в потомках, проверить работу и показать, 
// какие проблемы возникают при его использовании;
// базовый виртуальный метод bool isA(string classname), перекрыть его в потомках и показать отличие от метода classname.

// Продемонстрировать опасное приведение типов и предварительную проверку типа с помощью реализованного метода isA.
// Продемонстрировать использование стандартных средств языка (dynamic_cast в c++ или аналог на используемом языке).

#include <iostream>

using namespace std;

class Animal { 
private :
    string name;
public :
    virtual string classname(){
        return "Animal";
    }
};

class Cat : public Animal { 
private:
    int skillCatchMouse = 10;
public :
    string classname() override{
        return "Cat";
    }
    void catchMouse(){
        printf("skillcatchMouse = %d\n", skillCatchMouse);
    }
};

class Dog : public Animal { 
public :
    string classname() override {
        return "Dog";
    }
};

class Manul : public Cat { public :
    string classname () override { 
        return "Manul";
    } 
};

int main(){
    const int ZOO_SIZE = 10; 
    Animal* zoo[ZOO_SIZE];
    for (int i = 0; i < ZOO_SIZE; i++) {
        zoo[i] = new Cat();
        if ((zoo[i]->classname() == "Cat") || (zoo[i]->classname() == "Manul")){
            static_cast<Cat*>(zoo[i])->catchMouse();
        }
        
    }

    

    return 0;
}