// базовый виртуальный метод bool isA(string classname), перекрыть его в потомках и показать отличие от метода classname.
#include <iostream>

using namespace std;


class Animal { public :
    virtual bool isA(const string &who) { 
        return who == "Animal";
    } 
};

class Cat : public Animal { public :
    bool isA(const string &who) override {
        return (Animal::isA(who)) || (who == "Cat");
    } 
    void catchMouse(){
        printf("skillcatchMouse\n");
    }
};

class Dog : public Animal { 
public :
    bool isA(const string &who) override {
        return (who == "Animal") || (who == "Dog"); 
    }
};

class Manul : public Cat { public :
    bool isA(const string &who) override {
    return (who == "Manul") || (Cat::isA(who)); 
    }
};


int main(){
    const int ZOO_SIZE = 10; 
    Animal* zoo[ZOO_SIZE];
    for (int i = 0; i < ZOO_SIZE; i++) {
        zoo[i] = new Manul();
        if (zoo[i]->isA("Cat")){
            static_cast<Cat*>(zoo[i])->catchMouse();
        }
        
    }
    return 0;
}
