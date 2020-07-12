#ifndef STACK_H
#define STACK_H
#include <iostream>

using namespace std;

typedef char stackElementType;

class stack
{

    public:
        stack();
        void push(stackElementType item);
        stackElementType pop();
        stackElementType top();
        bool isEmpty();
        int n;




    private:

        int topIndex;
        struct node;
        typedef node*link;
        struct node
        {
            link next;
            stackElementType data;
        };
        link head;
};

#endif // STACK_H
