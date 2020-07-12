#include <iostream>
#include<cstring>
#include<string>
#include<sstream>
#include<cassert>
#include<fstream>
#include "BST.h"
#include "stack.h"
#include "fun.h"
using namespace std;


int main(int argc , char**argv)

{
    if(argc!=3)
    {
        cout<<"wrong number of files";

        return 0;
    }

    string s1,s2;
//cout<<spacecount("MD   \"asd asd\"   3    a  ");
//cout<<s1+" + "+s2+"-\n";

     char* IFile,*OFile;
    IFile=argv[1],OFile="temp.txt";

//    cout<<OFile<<endl;
    string com,met,day,hour,dh,line;
    BST<string>  *bst= new BST<string>;
    ofstream out;
    out.open(OFile);
//    out<<"hello worlk"<<endl;
    ifstream in;
    in.open(IFile);
    if(in.is_open()==false)
        {
        cout<<"input file not found";
        out<<"input file not found";
        return 0;
    }
int indx;

    while(!in.eof())
    {
        getline(in,line);
indx=line.find("\r");
if(!(indx==-1||indx>line.size()+2))
{line=line.substr(0,line.size()-1);}

        if(check(line,out))
        {
            split(com,met,line,false);

    if(strcmp(com.c_str(),"ADD")==0)
        {split(met,dh,met,true);

if(!bst->insert(padding(dh),met)){//cout<<"Conflict "+unpadding(padding(dh))<<endl;
out<<"Conflict "+unpadding(padding(dh))<<endl;}}
else if(strcmp(com.c_str(),"Find")==0)
    {if(bst->retrieve(padding(met))->isEmpty())
    {//cout<<"Empty "+unpadding(padding(met))<<endl;
    out<<"Empty "+unpadding(padding(met))<<endl;}
    else {//cout<<bst->retrieve(padding(met))->getData(day)<<endl;
    out<<bst->retrieve(padding(met))->getData(day)<<endl;}}
    else if(strcmp(com.c_str(),"MOD")==0)
    {
        split(met,dh,met,true);
       if(bst->retrieve(padding(dh))->isEmpty()||bst->getData(day)=="x")
       {
          // cout<<"Empty "+unpadding(padding(dh))<<endl;
           out<<"Empty "+unpadding(padding(dh))<<endl;
       }
       else{
        bst->retrieve(padding(dh))->change(met);
       }
    }
    else if(strcmp(com.c_str(),"DEL")==0)
    {
        if(bst->retrieve(padding(met))->isEmpty()||bst->retrieve(padding(met))->getData(day)=="x")
        {
           // cout<<"Empty "+unpadding(padding(met))<<endl;
            out<<"Empty "+unpadding(padding(met))<<endl;
        }
        else{BST<string> *bst2= bst->retrieve(padding(met));
            bst2->del();
        }
    }
    else if(strcmp(com.c_str(),"Print")==0)
    {
        bst->Print(out);
    }
        }

    }
    out.close();


    char*OFile1;
    string line1;
    OFile1=argv[2];
    ofstream out1;
    out1.open(OFile1);

    ifstream in1;
    in1.open("temp.txt");
    getline(in1,line1);
    out1<<line1;
    cout<<line1;
    while(!in1.eof())
    {
        getline(in1,line1);
        if(line1.size()<=0)break;
        out1<<endl<<line1;
        cout<<endl<<line1;

    }
    out1.close();
    remove("temp.txt");


    }

