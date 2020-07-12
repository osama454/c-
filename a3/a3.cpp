#include <iostream>
#include "stack.h"
#include<cstring>
#include<string>

using namespace std;

int main(int argc , char**argv)
{
    if (argc!=2)
    {cout<<"Invalid input\n";
    return 0;}

    string str=argv[1];
    string str1;
    string str2;
    int carry=0;
    char op='?';
    stack num1;
    stack num2;
    stack result;
    //stack float1;
    //stack float2;



int p=-1;//+ index
int n=-1;//- index
int k=-1;// operation index
//check for + or - and separate 2 strings
{
    p=str.find('+');
    n=str.find('-');
       //check that this sign in the range of the string
       if(p<str.size()-1)

    {
       op='+';
       k=p;
    }
    if(n<str.size()-1)
    {
        if(op=='?')
        {
            op='-';
            k=n;
        }
        else{cout<<"Invalid input\n";
        return 0;}
    }
    if(op=='?')
    {
       cout<<"Invalid input\n";
        return 0;
    }
  str1=str.substr(0,k);
  str2=str.substr(k+1,str.size()-str1.size()-1) ;

//  cout<<"str1:"<<str1<<endl;
  //cout<<"str2:"<<str2<<endl;

}
//check if there is more than 1 + or -
{
    p=str2.find('+');
    n=str2.find('-');
       //check that this sign in the range of the string
       if(p<str2.size()+1)
    {
    cout<<"Invalid input\n";
    return 0;
    }
    if(n<str2.size()+1)
    {
    cout<<"Invalid input\n";
    return 0;
    }
//cout<<"check input :123+456+ :"<<str1<<" + or - "<<str2<<endl;

}

int f1,f2,n1=0,n2=0,k1,k2;//number of digits after and before the floating point
//check the floating point for the  str1
{
    k=str1.find('.');
    if(k<str1.size()+1);
    else{
        str1=str1+".";
        n1=1;
        goto L1;

    }

    //check if there is more than 1 floating point

       if(k!=str1.find_last_of('.') )
       {
           cout<<"Invalid input\n";
           return 0;
       }



   L1:;

   // check if both sides of floating point are integer
   {
    for(int i=0;i<str1.size();i++)
    {
        if((str1[i]>='0'&&str1[i]<='9')||str1[i]=='.');
        else{
            cout<<"Invalid input\n";
           return 0;
        }

    }


   }
 k=str1.find('.');
 f1=str1.size()-k-1;
 k1=str1.size()-f1-1;
 if((f1<=0&&n1==0)||k1<=0)
 {
    cout<<"Invalid input\n";
    return 0;
 }

// cout<<"str1 before padding: "<<str1<<endl;

}


//check the floating point for the str 2
{
    k=str2.find('.');
    if(k<str.size()+1);
    else{
        str2=str2+".";
        n2=1;
        goto L2;

    }

    //check if there is more than 1 floating point

       if(k!=str2.find_last_of('.') )
       {
           cout<<"Invalid input\n";
           return 0;
       }



   L2:;
   // check if both sides of floating point are integer
   {
 for(int i=0;i<str2.size();i++)
    {
        if((str2[i]>=48&&str2[i]<=57)||str2[i]=='.');
        else{
            cout<<"Invalid input\n";
           return 0;
        }

    }

   }
 k=str2.find('.');
 f2=str2.size()-k-1;
 k2=str2.size()-f2-1;
 if((f2<=0&&n2==0)||k2<=0)
 {
    cout<<"Invalid input\n";
    return 0;
 }
// cout<<"str2 before padding: "<<str2<<endl;

}


//padding the smallest floating point of the 2 strings
{
    if(f1>f2)
    {
        for(int i=0;i<f1-f2;i++)
        {
            str2=str2+"0";
        }
    }
    else{
        for(int i=0;i<f2-f1;i++)
        {
            str1=str1+"0";
        }
    }
    if(k1>k2)
    {
        for(int i=0;i<k1-k2;i++)
        {
            str2="0"+str2;
        }
    }
    else{
        for(int i=0;i<k2-k1;i++)
        {
            str1="0"+str1;
        }
    }
//cout<<"str1 after padding :"<<str1<<"- str2 :"<<str2<<endl;;

}
//push the 2 strings int the 2 stacks
{
    for(int i=0;i<str1.size();i++)
    {
       num1.push(str1[i]) ;

    }
        for(int i=0;i<str2.size();i++)
    {
       num2.push(str2[i]) ;

    }

}

int z=0;
// pop and add
char ch1;
char ch2;
int nm1;
int nm2;
int rs;
char res;
int checkNum=0;
if(op=='+')
{   carry=0;
    for(int i=0;i<str1.size();i++)
    {ch1=num1.pop();
    ch2=num2.pop();
        //digit
        if(ch1!='.')
        {
            nm1=ch1-48;
            nm2=ch2-48;
            rs=nm1+nm2+carry;
            carry=0;
            if(rs>=10)
            {
                carry=1;
                rs=rs-10;


            }

            //if(rs!=0||checkNum==1)
                 result.push(rs+48);
                z++;

        }
        // .
        else
        {   //if(checkNum==1)
           result.push('.') ;
           z++;
        }
   }
   if(carry>0){result.push(carry+48);
                z++;
                carry=0;}

}
carry=0;


//pop and substruct
if(op=='-')
{for(int i=0;i<str1.size();i++)
    {ch1=num1.pop();
    ch2=num2.pop();
        //digit
        if(ch1!='.')
        {
            nm1=ch1-48;
            nm2=ch2-48;
            rs=nm1-nm2+carry;
            carry=0;
            if(rs<0)
            {
                carry=-1;
                rs=rs+10;

            }
                 result.push(rs+48);
                z++;


        }
        // .
        else
        {
           result.push('.') ;
           z++;
        }
   }}


   while(1)
   {
    ch1=result.pop();
    z--;
   if(ch1!='0'){
          if(ch1=='.')cout<<'0';
       if(z!=0) cout<<ch1;
        break;}

   }
   int Z=z;

   for(int i=0;i<z-1;i++)
    {ch1=result.pop();
    Z--;
    cout<<ch1;
    }
    if(Z!=0)
    {ch1=result.pop();
    if(ch1!='.')cout<<ch1;}
cout<<endl;

}





















