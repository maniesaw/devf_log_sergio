#ifndef C_A_H
#define C_A_H

class A{
public:
    A(int val);
    ~A();
    bool a;
    bool* array;

    void Clone(A* other);
    void Print();
};
#endif
