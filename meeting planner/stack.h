#ifndef STACK_H
#define STACK_H
#include <iostream>
#include "BST.h"
using namespace std;


template < class stackElementType >
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
template class stack<string>;
template class stack<BST<string>*>;
#endif // STACK_H
