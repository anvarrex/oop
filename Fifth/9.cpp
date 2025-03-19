// Продемонстрировать опасное приведение типов и предварительную проверку типа с помощью реализованного метода isA.
// Продемонстрировать использование стандартных средств языка (dynamic_cast в c++ или аналог на используемом языке).

// Зачем нужны виртуальные методы? Зачем может понадобиться хранить объект не в указателе на свой собственный класс,
//а в указателе на класс-предок?
// Зачем нужна проверка на принадлежность некоторому классу?
// Зачем и в каких случаях требуется производить безопасное приведение типов?
//В каком случае может понадобиться вызвать метод потомка для объекта, который лежит в переменной предка?

#include <iostream>

using namespace std;

class Animal { 
private :
    string name;
public :
    virtual void sound() {
    printf ("\n") ; }
};

class Cat : public Animal { 
private:
    int skillCatchMouse = 10;
public :
    void sound () override {
        printf ("Mew-mew\n");
    }
    void catchMouse(){
        printf("skillcatchMouse = %d\n", skillCatchMouse);
    }
};

class Dog : public Animal { 
public :
    void sound () override {
        printf ("Wow-wow\n");
    }
    void chaseCat(){
        printf("chaseCat");
    } 
};

int main(){
    const int ZOO_SIZE = 10; 
    Animal* zoo[ZOO_SIZE];
    for (int i = 0; i < ZOO_SIZE; i++) {
        zoo[i] = new Cat();
        if (dynamic_cast<Cat*>(zoo[i])){
            dynamic_cast<Cat*>(zoo[i])->catchMouse();
        }
    }

    

    
    
    return 0;
}