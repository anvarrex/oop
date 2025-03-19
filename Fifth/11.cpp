#include <iostream>

using namespace std;


class Base {
private:
    int *x;
public:
    Base():x(new int(10)){
        printf("Base()\n");
    }

    void method(){
        printf("Base method %d\n", *x);
    }

    // Base(Base *obj){
    //     printf("Base(Base *obj)\n");
    // }

    Base(const Base &obj){
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

Base func1() {
    Base obj;
    return obj;
};

Base* func2() {
    Base obj;
    return &obj;
};

Base& func3() {
    Base obj;
    return obj;
};

Base func4() {
    Base* obj  = new Base();
    return *obj;
};

Base* func5() {
    Base*  obj = new Base();
    return obj;
};

Base& func6() {
    Base* obj = new Base();
    return *obj;
};

int main() {
    {
        Base obj1 = func1();
        obj1.method();
        
    }
    printf("-----------\n");
    {
        Base* obj2 = func2();
        obj2->method();
    }
    printf("-----------\n");
    {
        Base& obj3 = func3();
    }

    printf("-----------\n");
    {
        Base& obj3 = func3();
    }
    printf("-----------\n");
    {
        Base obj4 = func4();
    }
    printf("-----------\n");
    {
        Base* obj5 = func5();
        delete obj5;
    }
    printf("-----------\n");
    {
        Base& obj6 = func6();
    }
    printf("-----------\n");
    {
        Base& obj6 = func6();
    }

    return 0;
}