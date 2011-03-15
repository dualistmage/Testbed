#include <iostream>

class virtualClass
{
public:
    virtualClass() {printf("hello virtual class\n");};
    virtual ~virtualClass();
    void run() { print(); }
protected:
    virtual void print();
public:
private:
}; // end - class virtualClass

class virtualInheritClass : public virtualClass
{
public:
    ~virtualInheritClass() {}
protected:
    void print() { printf("hello virtual inherit\n"); }
public:
private:
}; // end - class virtualInheritClass

virtualClass::~virtualClass() { printf("hello virtual original destructor---------\n"); }
void virtualClass::print() { printf("hello virtual original---------\n"); }
