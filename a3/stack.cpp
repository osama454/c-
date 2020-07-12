#include "stack.h"
#include <iostream>
#include<cassert>
using namespace std;
stack::stack()
{

    head=0;
    n=0;
}


void stack::push(stackElementType item)
{
    link addedNode = new node;
    assert(addedNode!=NULL);
    addedNode->data=item;
    addedNode->next=head;
    head=addedNode;
    n++;


}

stackElementType stack:: pop()
{
    assert(head!=NULL);


    int temp=head->data;
    link temp2=head;
    head=head->next;
    delete temp2;


    return temp;
}















