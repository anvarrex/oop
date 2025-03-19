#include <iostream>
#include <memory>

using namespace std;

class Point { 
private :
    int x;
    int y;
public :
    Point() : x(0), y(0) { 
        printf("Point ()\n") ;
}
Point(int x, int y) : x(x), y(y) {
    printf("Point(int x, int y) : x(x), y(y)\n");
}
Point(const Point &t) : x(t.x), y(t.y) { 
   printf("Point(const Point &t)\n");
}
void dump(){
    printf("%d, %d\n", x, y); }

~Point (){
    printf ("~Point ()\n");
    } 
};

unique_ptr<Point> pass_object(unique_ptr<Point> p){
    p->dump();
    return move(p);
}

shared_ptr<Point> pass_object(shared_ptr<Point> p){
    p->dump();
    return move(p);
}



int main() {
    {
        unique_ptr<Point> p1 = make_unique<Point>(1, 2);
        p1 = pass_object(move(p1)); 
        // unique_ptr<Point> p2 = p1;
        p1->dump();
    }
    
    {
        shared_ptr<Point> p = make_shared<Point>(1, 2);
        p = pass_object(p);
        shared_ptr<Point> p2 = p;
        p2->dump();
    }

    return 0;
}