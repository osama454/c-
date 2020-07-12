

#include <iostream>
#include<cstring>
#include<string>
#include<sstream>
#ifndef FUN_H
#define FUN_h
#endif
#include<cassert>
#include<fstream>

void split(string &s1,string &s2,string in,bool qt)
{string in2=in;
//////remove spaces and taps from start and end of string
{int k=0,n=0;
for(int i=0;i<in2.size();i++)
{if(in[i]==' '||in[i]=='\t')k++;
else break;}
for(int i=in2.size()-1;i>=0;i--)
{if(in[i]==' '||in[i]=='\t')n++;
else break;}
in=in2.substr(k,in2.size()-k-n);}
/////if there is only one word
{int i=in.find(' ');//???
int j=in.find('\t');
if((i<0||i>in.size()-1)&&(j<0||j>in.size()-1)){s1=in;s2="\0";return;}}


//////if there is " "
int z=0;
if(qt)
{int i=in.find('"');
int j=in.find_last_of('"');
bool begn=false;
if((i!=j)) {
    for(int nnn=0;nnn<i;nnn++)
    {
       if(in[nnn]==' '||in[nnn]=='\t')
            begn=true;
    }
    if(!begn)z=j;
//in=in.substr(0,z+1)+' '+in.substr(z+1,in.size()-z-1);
}}


//cout<<in<<endl;
///////remove all spaces and taps after first word and put single space
{int n=0,j=0 ;
for(int i=z;i<in.size();i++)
{if((in[i]==' '||in[i]=='\t'))
{n++;
if(j==0)j=i;
if(in[i+1]!=' '&&in[i+1]!='\t')break;}}

s1=in.substr(0,j);
s2=in.substr(j+n,in.size()-j-n);}}


string padding(string z)
{
    string a,b;
    split(a,b,z,false);
    for (int i=a.size();i<3;i++)
        a='0'+a;
    for (int i=b.size();i<2;i++)
        b='0'+b;
    return a+b;
}

string unpadding(string in)
{   assert(in.size()==5);

    string a,b;
       int na=0,nb=0;
    for (int i=0;i<2;i++)
        {if (in[i]=='0')na++;
        else break;}
    for (int i=3;i<4;i++)
        {if (in[i]=='0')nb++;
        else break;}
    a=in.substr(na,3-na);
    b=in.substr(3+nb,2-nb);
    return a+" "+b;
}

int spacecount(string str)
{int count =1;
string s1,s2;
split(s1,s2,str,false);
if(s1=="\0")return 0;
if(s1=="ADD"||s1=="MOD")
    {
        split(s1,s2,s2,true);
  if(s1!="\0")count++;
else return count;  }
split(s1,s2,s2,false);
while(1)
{if(s1!="\0")count++;
else return count;
  split(s1,s2,s2,false);
}


	return count;
}


int todigit(string str)
{int n=0,p;
p=str.size();

int k=1;
   for (int i=p-1;i>=0;i--)
   {
      if(str[i]>='0'&&str[i]<='9') {n=n+(str[i]-'0')*k;k=k*10;}
      else {return -1;}
   }
   return n;
}
bool check(string str,ofstream &out)
{int flag=0;
string com,tx;
//cout<<F<<endl;



    split(com,tx,str,false);
if(com=="\0")return false;
   if(com!="ADD"&&com!="MOD"&&com!="DEL"&&com!="Print"&&com!="Find")
   {
      // cout<<"Invalid command"<<endl;
       out<<"Invalid command"<<endl;
       return false;
   }
   int count=spacecount(str);

if((com=="ADD"||com=="MOD")&&count!=4){//cout<<"Invalid arguments\n";
out<<"Invalid arguments\n";return false;}

if((com=="Find"||com=="DEL")&&count!=3){//cout<<"Invalid arguments\n";
out<<"Invalid arguments\n";return false;}

if(com=="Print"&&count!=1){//cout<<"Invalid arguments\n";
out<<"Invalid arguments\n";return false;}
string T1,T2,T3,T4;
if(count==4)
{

    split(T1,T2,str,false);
    split(T2,T3,T2,true);
    split(T3,T4,T3,false);
//    cout<<T1+" "+T2+" "+T3+" "+T4;
if(T2[0]!='"'||T2[T2.size()-1]!='"'){//cout<<"Invalid title\n";
out<<"Invalid title\n";flag=1;}
}
if(count==3)
{

    split(T1,T3,str,false);
    split(T3,T4,T3,false);

}
if(count==3||count==4)

{if(todigit(T3)>365||todigit(T3)<1){//cout<<"Invalid day\n";
out<<"Invalid day\n";flag=1;}
if(todigit(T4)>23||todigit(T4)<0){//cout<<"Invalid hour\n";
out<<"Invalid hour\n";flag=1;}
if (flag==1)return false;}

return true;

}

