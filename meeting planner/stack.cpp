#include "stack.h"
#include <iostream>
#include<cassert>
using namespace std;

template < class stackElementType >
stack <stackElementType> ::stack()
{

    head=0;
    n=0;
}

template < class stackElementType >
void stack<stackElementType>::push(stackElementType item)
{
    link addedNode = new node;
    assert(addedNode!=NULL);
    addedNode->data=item;
    addedNode->next=head;
    head=addedNode;
    n++;


}
template < class stackElementType >
stackElementType stack<stackElementType>:: pop()
{
    assert(head!=NULL);


    stackElementType temp=head->data;
    link temp2=head;
    head=head->next;
    delete temp2;
    n--;

    return temp;
}

template < class stackElementType >
bool stack<stackElementType>:: isEmpty()
{
    if(n==0)return true;
    return false;
}












