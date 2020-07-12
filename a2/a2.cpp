    #include <iostream>
    #include<fstream>
    #include<cstring>
    #include "inord_list.h"
    using namespace std;

    int main(int argc,char**argv)
    {
if(argc!=3){
cout<<"Incorrect number of arguments";
return 0;}


        /*define variables*/
   char* FileName=argv[1];
   char* ComandsFile=argv[2];
   string line;
   int line_number=1;
   inord_list MyList;
   inord_list CommandList;
   int i;//element index in line
    int j=-1;//begin of a word
    int n=0;// number of elements in a word
    int NumSpace=0;//number of separators

    /*insert data in the class*/
   {


       ifstream in;
        in.open(FileName);
if(in.is_open()==false){
     cout<<"File not found";
     return 0;
}


   while( in.eof()==0)
   {    getline(in,line);
       for(i=0;i<line.size();i++)
       {
       if(line[i]<=90&&line[i]>=65)// A to Z
            {line[i]=line[i]+32;
                if(j==-1){j=i; n=0;}
                n++;            }






            else if(line[i]==' '||line[i]=='\t'||line[i]=='\n'||line[i]==','
                    ||line[i]=='.') //spaces //separators

        {   if(n>0)
                {MyList.insertLU(line.substr(j,n),line_number);
            }
            NumSpace++;
            j=-1;
            n=0;}
                else //a  to z , 0 to 9

            {
                if(j==-1){j=i;n=0;}
                n++;
            }


           }


           if(n>0)
        {
           MyList.insertLU(line.substr(j,n),line_number);}

           j=-1;
           n=0;
           line_number++;
           NumSpace++;}





   in.close();}
    /*read the command file*/
    line_number=1;
    {ifstream in;

    in.open(ComandsFile);
if(in.is_open()==false){
     cout<<"File not found";
     return 0;
}


int z;
   while( in.eof()==0)
    {    getline(in,line);
    z=0;
    string str1;

       for(i=0;i<line.size();i++)
       {

            if(line[i]==' '||line[i]=='\t')

            {
              if(n>0)
                {str1=line.substr(j,n);
                    if(z!=0){
                    for(int i=0;i<str1.size();i++)
                    {if(str1[i]<='Z'&&str1[i]>='A')
                    {str1[i]=str1[i]+32;}}

                    }
                    //cout<<str1<<endl;

                    CommandList.insertLU(str1,line_number);
                    z++;
            }

            j=-1;
            n=0;
            }
            else

        {  if(j==-1){j=i;n=0;}
                n++; }


           }


           if(n>0)
        {str1=line.substr(j,n);
                    if(z!=0){
                    for(int i=0;i<str1.size();i++)
                    {if(str1[i]<='Z'&&str1[i]>='A')
                    {str1[i]=str1[i]+32;}}

                    }
                    //cout<<str1<<endl;
           CommandList.insertLU(str1,line_number);
           z++;
           }


           j=-1;
           n=0;
           line_number++;}

    in.close();}
int k;
string st;
    if(!CommandList.first(st,k))return 0;



   NumSpace= NumSpace-1  ;
    CommandList.command(MyList,NumSpace);



    return 0;
    }
