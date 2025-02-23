#include <iostream>

using namespace std;

class Point
{
protected:
    int x, y;
public:
    Point(): x(0), y(0){
        printf("Point() ");
        cout << this;
        printf(" (%d, %d)\n", x, y);
    }
    Point(int x, int y);
    Point(const Point &other){
        this->x = other.x;
        this->y = other.y;
        printf("Point(const Point &other) ");
        cout << this;
        printf(" (%d, %d)\n", x, y);
    }
    ~Point();
    void move_point(int dx, int dy);
    friend void reset(Point &obj);
    void printCoo(){
        printf("(%d, %d)\n", x, y);
    }
};

void reset(Point &obj){
    printf("reset(Point &obj) ");
    cout << &obj << " ";
    obj.x = 0;
    obj.y = 0;
}

void Point::move_point(int dx, int dy){
    printf("move_point(int dx, int dy) ");
    cout << this;
    x = x + dx;
    y = y + dy;
    printf(" (%d, %d)\n", x, y);
}


//Инициализация деструктора вне класса
Point::Point(int x, int y){
    this->x = x;
    this->y = y;
    printf("Point(int x, int y) ");
    cout << this;
    printf(" (%d, %d)\n", x, y);
}

Point::~Point()
{   
    printf("~Pointer() ");
    cout << this;
    printf(" (%d, %d)\n", x, y);
}


class ColoredPoint: public Point{
    private:
        string color;
    public:
        ColoredPoint(){
            printf("ColoredPoint()\n");
            color = "white";
        }
        ColoredPoint(int x, int y, string color){
            this->x = x;
            this->y = y;
            printf("ColoredPoint(int x, int y, string color): Point(x, y)\n");
            this->color = color;
        }

        ColoredPoint(const ColoredPoint &other){
            printf("ColoredPoint(const ColoredPoint &other)");
            color = other.color;
            x = other.x;
            y = other.y;
        }

        void SetColor(string color){
            this->color = color;
        }
        void GetColor(){
            cout << color << endl;
        }
        ~ColoredPoint(){
            printf("~ColoredPoint()\n");
        }
};

class Section{
    protected:
        Point *p1;
        Point *p2;
    public:
        Section(){
            printf("Section()\n");
            p1 = new Point;
            p2 = new Point;
        }
        Section(const Section &s){
            printf("Section(const Section &s)\n");
            p1 = s.p1;
            p2 = s.p2;
        }
        ~Section(){
            delete p1;
            delete p2;
            printf("~Section()\n");
        }
};

int main(){
    // {
    //     Point a;
    //     a.move_point(10, 5);
    //     Point b(1, 2);
    //     reset(b);
    //     b.printCoo();
    //     Point c(a);
    // }

    // cout << "============================" << endl;

    // Point *a = new Point;
    // Point *b = new Point(1, 2);
    // Point *c = new Point(*b);

    // delete a;
    // delete b;
    // delete c;

    // cout << "============================" << endl;

    // {
    //     ColoredPoint z(1, 2, "black");
    //     //z.GetColor();
    //     //z.SetColor("Red");
    //     //z.GetColor();
    // }

    // cout << "============================" << endl;

    // {
    //     Point *p = new ColoredPoint(1, 2, "black");
    //     delete p;
    // }

    Section *s1 = new Section;
    Section *s2 = new Section(*s1);

    delete s1;
    delete s2;

    cout << "Конец работы программы\n";
    return 0;
}