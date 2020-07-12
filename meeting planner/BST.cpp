
#include "BST.h"
#include "stack.h"

#include<cstring>
#include<string>
#include<cassert>
#include <iostream>
#include<fstream>

string unpadding2(string inn)
{
//      assert(inn.size()==5);

    string a,b;
       int na=0,nb=0;
    for (int i=0;i<2;i++)
        {if (inn[i]=='0')na++;
        else break;}
    for (int i=3;i<4;i++)
        {if (inn[i]=='0')nb++;
        else break;}
    a=inn.substr(na,3-na);
    b=inn.substr(3+nb,2-nb);
    return a+" "+b;
}

using namespace std;
//constructor
template < class btElementType >
BST < btElementType > :: BST()
{
nullTree = true;
leftTree = NULL;
rightTree = NULL;
}
//insert
template < class btElementType >
bool BST < btElementType >::insert(const btElementType & d,const btElementType &m)
{
//    cout<<"inesert "+meeting+"+"+treeData<<endl;
if (nullTree) {
nullTree = false;
leftTree = new BST;
rightTree = new BST;
treeData = d;
meeting=m;

return true;
}
else if(strcmp(d.c_str() ,treeData.c_str())==0&&meeting=="x"){meeting=m;return true;}
else if (strcmp(d.c_str() ,treeData.c_str())==0&&meeting!="x"){return false ; }// do nothing -- it's already here!
else if (strcmp(d.c_str(),treeData.c_str())<0)
{return leftTree->insert(d,m); }// insert in left subtree
else
{return rightTree->insert(d,m);} // insert in right subtree

}

template < class btElementType >BST < btElementType > *//????
BST < btElementType > :: retrieve(const btElementType & d)
{
if ((nullTree || d == treeData)&&meeting!="x")
// return pointer to tree for which retrieve was called
return this;
else if (d < treeData)
return leftTree->retrieve(d); // recurse left
else
return rightTree->retrieve(d); // recurse right
}


template < class btElementType >
bool
BST < btElementType > :: isEmpty() const
{
return nullTree;
}

template < class btElementType >
btElementType
BST < btElementType > :: getData(btElementType &d) const
{
//    cout<<"at get data : "+treeData+" + "+meeting<<endl;
assert(!isEmpty());
d=treeData;
return meeting;
}

template < class btElementType >
BST < btElementType > *
BST < btElementType > :: left()
{
assert(!isEmpty());
return leftTree;
}

template < class btElementType >
BST < btElementType > *
BST < btElementType > :: right()
{
assert(!isEmpty());
return rightTree;
}

template < class btElementType >
void BST < btElementType >
:: makeLeft(BST * T1)
{
assert(!isEmpty());
assert(left()->isEmpty());
delete left(); // could be nullTree true, w/data
leftTree = T1;
}

template < class btElementType >
void BST < btElementType >
:: makeRight(BST * T1)
{
assert(!isEmpty());
assert(right()->isEmpty());
delete right();
rightTree = T1;
}

template < class btElementType >
void BST < btElementType >
:: change(const btElementType &m)
{
    assert(!isEmpty());
    meeting=m;

}

template < class btElementType >
void BST < btElementType >
:: del()
{meeting="x";}

template < class btElementType >
void BST < btElementType >
:: Print(ofstream &out)
{
    string t;
    bool flage=false;



BST *b,*c;
    if(isEmpty()){
       // cout<<"Empty Planner\n";
        out<<"Empty Planner\n";return;}
    stack<BST<string>*>st;
    b=this;

    while(true)//go to deep lift and print it and push the path
    {
//        cout<<b->treeData<<endl;
        st.push(b);
        L3:
//       cout<<"\n el data "<<b->getData(t)<<":"<<t<<endl;
//           cout<<t<<endl;
        if(b->left()->isEmpty())
        {
           c=st.pop();
        c->getData(t);
//        cout<<endl<<t+" : "+unpadding2(t)<<endl;
        if(c->getData(t)!="x")
           {
               //cout<<c->getData(t)+" "+unpadding2(t)<<endl;
               out<<c->getData(t)+" "+unpadding2(t)<<endl;
               flage = true;}
        goto L;
        }

        b=b->left();
    }

    {L:;//then go one step right

    if(b->right()->isEmpty())goto L2;
    b=b->right();
    st.push(b);
    goto L3;//then go to deep lift

    }
    //if not exist//go back one step and print and go one step right
    {L2:if(st.isEmpty()){
        if (flage==false)
    {
        //cout<<"Empty Planner\n";
        out<<"Empty Planner\n";
    }
return;}
    b=st.pop();
    b->getData(t);
    if(b->getData(t)!="x")
    {//cout<<b->getData(t)+" "+unpadding2(t)<<endl;
    out<<b->getData(t)+" "+unpadding2(t)<<endl;
    flage=true;}
    goto L;}

    }


