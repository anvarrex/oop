#include <iostream>

using namespace std;


class Base {
public:
    Base(){
        printf("Base()\n");
    }

    Base(Base *obj){
        printf("Base(Base *obj)\n");
    }

    Base(Base &obj){
        printf("Base(Base &obj)\n");
    }

    virtual ~Base(){
        printf("~Base()\n");
    }
};

class Desc: public Base {
    public:
        Desc() {
            printf("Desc()\n");
        }
    
        Desc(Desc *obj){
            printf("Desc(Desc *obj)\n");
        }
    
        Desc(Desc &obj){
            printf("Desc(Desc &obj)\n");
        }
    
        ~Desc() override{
            printf("~Desc()\n");
        }
    };

void func1(Base obj) {
    printf ("inside function func1\n") ;
    Desc* c = dynamic_cast<Desc*>(&obj);
    if (c){
        printf("obj является объектом Desc");
    }
};

void func2(Base *obj) {
    printf ("inside function func2\n");
    Desc* c = dynamic_cast<Desc*>(obj);
    if (c){
        printf("obj является объектом Desc\n");
    }
};

void func3(Base &obj) {
    printf ("inside function func3\n");
    Desc* c = dynamic_cast<Desc*>(&obj);
    if (c){
        printf("obj является объектом Desc\n");
    }
};

int main() {
    Base obj1;
    Base *obj2 = new Base();
    Desc obj3;
    Desc *obj4 = new Desc();
    Base *obj5 = new Desc();

    printf("-------\n");

    func1(obj1);
    func2(&obj1);
    func3(obj1);

    printf("-------\n");

    func1(*obj2);
    func2(obj2);
    func3(*obj2);

    printf("-------\n");

    func1(obj3);
    func2(&obj3);
    func3(obj3);

    printf("-------\n");

    func1(*obj4);
    func2(obj4);
    func3(*obj4);

    printf("-------\n");

    func1(*obj5);
    func2(obj5);
    func3(*obj5);

    delete obj2;
    delete obj4;
    delete obj5;

    return 0;
}