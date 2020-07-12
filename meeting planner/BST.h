#ifndef BST_H
#define BST_H
#include<cstring>
#include<string>
#include <iostream>

using namespace std;



template < class btElementType >

class BST
{
    public:
        bool insert(const btElementType & d,const btElementType &m);
        BST();
        bool isEmpty() const;
        btElementType getData(btElementType &d) const;
        BST * left();
        BST * right();
        void makeLeft(BST * T1);
        void makeRight(BST * T1);
        BST *retrieve(const btElementType & d);//???
        void change(const btElementType &m);
        void del();
        void Print(ofstream &out);

    private:
        bool nullTree;
        btElementType treeData;
        btElementType meeting;
        BST * leftTree;
        BST * rightTree;
};

template class BST<string>;

#endif // BST_H





