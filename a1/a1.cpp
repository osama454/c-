#include <iostream>
#include<cstring>
#include <sstream>
#include <stdlib.h>
#include <ctype.h>


using namespace std;

int main(int argc,char**argv)
{
  const char* a=argv[1];

   //print
   if(strcmp(a,"print")==0)
        {   if(argc<3)
                {cout<<"Incorrect Number of Arguments";
                goto L;}
            for(int i=2;i<argc;i++)
                {cout<<argv[i]<<" ";}}

   // reverse
   else if(strcmp(a,"reverse")==0)
        {if(argc<3)
                {cout<<"Incorrect Number of Arguments";
                goto L;}
        char *str=argv[2];
      /*  if(str[0]>=65 && str[0]<=92)
            {str[0]=str[0]+32; } */
        for(int i=argc-1;i>2;i--)
            cout<<argv[i]<<" ";
        cout<<str;}

    //upper
     else if(strcmp(a,"upper")==0)
     {      if(argc<3)
                {cout<<"Incorrect Number of Arguments";
                goto L;}
           for(int j=2;j<argc;j++)
         {  char* str=argv[j];
            for(int i=0;i<strlen(str);i++)
      {         if(str[i]>=97 && str[i]<=122)
                    str[i]=str[i]-32; }
             cout<<str<<" ";}}

       //shuffle
      else if(strcmp(a,"shuffle")==0)
      {

               if(argc!=3)
               {    cout<<"Incorrect Number of Arguments";
                    goto L;}
               for(int j=2;j<argc;j++)
               {char* str=argv[j];
               char t;
               for(int i=0;i<=strlen(str)-2;i=i+2)
                    {t=str[i+1];
                    str[i+1]=str[i];
                    str[i]=t;}
                cout<<str<<" ";}}

      //shuffleStatement
      else if(strcmp(a,"shuffleStatement")==0)
{               if(argc<3)
               {    cout<<"Incorrect Number of Arguments";
                    goto L;}
              char* t;
           for(int i=2;i<=argc-2;i+=2)
                {t=argv[i+1];
                argv[i+1]=argv[i];
                argv[i]=t; }
            for(int i=2;i<argc;i++)
            cout<<argv[i]<<" ";}

        //delete
        else if(strcmp(a,"delete")==0)
{               if(argc<3)
               {    cout<<"Incorrect Number of Arguments";
                    goto L;}
            const   char*str=argv[2];
            if(str[0]!=43&&str[0]!='+'&& isdigit(str[0])==false) //<==
                            {cout<<"Incorrect Data Type";
                            goto L;}
            for (int j = 1; j < strlen(str); j++)
                            if (isdigit(str[j]) == false)
                                {cout<<"Incorrect Data Type";
                                goto L;}

            stringstream geek(str);
            int v;
            geek >> v;
            if(v>argc-3)
            {   cout<<"Incorrect Number of Arguments";
                goto L;

            }
            if(v<=0)
            {   cout<<"Incorrect Data Type";
                goto L;

            }

            for(int i=3;i<v+2;i++)
                cout<<argv[i]<<" ";
            for(int i=v+3;i<argc;i++)
                cout<<argv[i]<<" ";}

        //Middle
         else if(strcmp(a,"middle")==0)
{               if(argc<3)
               {    cout<<"Incorrect Number of Arguments";
                    goto L;}
                   if(argc%2==0)
                    cout<<argv[argc/2]<<" "<<argv[argc/2+1];
                else  cout<<argv[(argc+1)/2]<<" ";    }

        //Add
         else if(strcmp(a,"add")==0)
         {
               if(argc<3)
               {    cout<<"Incorrect Number of Arguments";
                    goto L;}
                 int sum=0;

              for(int i=2;i<argc;i++)
                    {   char *str=argv[i];
                         if(str[0]!=43 && str[0]!=45 && isdigit(str[0]) == false)
                            {cout<<"Incorrect Data Type";
                            goto L;}
                        for (int j = 1; j < strlen(str); j++)
                            if (isdigit(str[j]) == false)
                                {cout<<"Incorrect Data Type";
                                goto L;}
                        stringstream geek(str);
                        int v;
                        geek >> v;
                        sum+=v; }
              cout<<sum;
               }


        //Random
         else if(strcmp(a,"random")==0)
        {   if(argc!=6)  //if number of input not equal 4
                    {cout<<"Incorrect Number of Arguments";
                    goto L;}
            int v[4];
            char *test=argv[2];
            if(test[0]==45 || strcmp(test,"0")==0) // if first digit is -ve              //<==
                {cout<<"Incorrect Data Type";
                goto L;}
            for(int i=2;i<argc;i++)
                    {   char *str=argv[i];
                        if(str[0]!=43 && str[0]!=45 && isdigit(str[0]) == false)
                            {cout<<"Incorrect Data Type";
                            goto L;}
                        for (int j = 1; j < strlen(str); j++)
                            if (isdigit(str[j]) == false)
                                {cout<<"Incorrect Data Type";
                                goto L;}
                         stringstream geek(str);
                         geek >> v[i-2];}
            if(v[1]>=v[2])
                {cout<<"Incorrect Number of Arguments";
                 goto L;}
	   // cout<<v[0]<<" "<<v[1]<<" "<<v[2]<<" "<<v[3]<<" ";
            int n=v[2]-v[1]+1;

            srand(v[3]);
            for(int i=1;i<=v[0];i++)
                cout<<rand()%n +v[1]<<" ";}

           else cout<<"Undefined Command";
           L: ;

return 0;

}
