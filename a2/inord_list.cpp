#include "inord_list.h"
#include<string.h>
inord_list::inord_list()
{
    head=0;
    pred=0;
    current=0;
}

bool inord_list::insertUL(const ListElemType &e,const int &l)
{
    Node* addedNode=new Node;

    if(addedNode==0)
        return false;
    addedNode->elem=e;
    addedNode->line_number=l;

    if(head==0||l>=head->line_number)
    {
        addedNode->next=head;
        head=addedNode;

    }

    else{
            pred=head;

    while(pred->next!=0&&pred->next->line_number>=l)
        pred=pred->next;
        addedNode->next=pred->next;
        pred->next=addedNode;
    }

    return true;
}


 bool inord_list::insertLU(const ListElemType &e,const int &l)
 {
     Node* addedNode=new Node;

    if(addedNode==0)
        return false;
    addedNode->elem=e;
    addedNode->line_number=l;
    // cout<<"2bl al if"<<head->elem<<endl;
    if(head==0||l<head->line_number)
    {
        addedNode->next=head;
        head=addedNode;
        // cout<<"lma if true:  "<<head->elem<<endl;
    }

    else{    //cout<<"2bl al pred:  "<<head->elem<<endl;
            pred=head;
     //cout<<"b3d al else :   "<<head->elem<<endl;
    while(pred->next!=0&&pred->next->line_number<=l)
        pred=pred->next;
        addedNode->next=pred->next;
        pred->next=addedNode;
    }
    // cout<<"b3d al while :  "<<head->elem<<endl;

    return true;
 }


bool inord_list::insertSort(const ListElemType &e,const int &l)
{
    Node* addedNode=new Node;

    if(addedNode==0)
        return false;
    addedNode->elem=e;
    addedNode->line_number=l;

    if(head==0||strcmp(head->elem.c_str(),e.c_str())>0)
    {
        addedNode->next=head;
        head=addedNode;

    }

    else{
            pred=head;

    while(pred->next!=0&&strcmp(pred->next->elem.c_str(),e.c_str())<=0)
        pred=pred->next;
        addedNode->next=pred->next;
        pred->next=addedNode;
    }

    return true;
}


bool inord_list::first( ListElemType &e, int &l)
{
    if(head==0)return false;
    current=head;
    e=current->elem;
    l=current->line_number;

    return true;
}


bool inord_list::next( ListElemType &e, int &l)
{
    if(current==0||current->next==0)return false;
  current= current->next;
  e=current->elem;
  l=current->line_number;
  return true;
}


int inord_list::wordCount()
{   if(head==0){cout<<"0 words\n";
                return 0;}
    int i=0;
    current=head;
if(head!=0)

{   i++;
    while(current->next!=0)
        {i++;
        current=current->next;}
    }
    cout<<i<<" words\n";
    return i;
}


int inord_list::distWords()
{   if(head==0){cout<<"0 distinct words\n";
                return 0;}
    int i=0;
   Link pred=head;

if(head!=0)

{   i++;
    while(pred->next!=0)
        {
        current=head;
        pred=pred->next;
        while(current!=pred)
        {
            if(strcmp(pred->elem.c_str(),current->elem.c_str())==0)goto L;
            current=current->next;
        }


        i++;
        L:;
    }}
    cout<<i<<" distinct words\n";
    return i;
}


int inord_list::charCount(const int &n)
{   if(head==0){cout<<n<<" characters\n";
                return 0;}

    int i=0;
    current=head;
    if(current!=0){
            i=current->elem.size();
    while(current->next!=0){
        current=current->next;
        i=i+current->elem.size();
    }
}
i=i+n;
cout<<i<<" characters\n";
return i;
}


string inord_list::frequentWord()
{
    if(head==0)if(head==0){cout<<"Most frequent word is: \n";
                return "0";}

    inord_list lis;
    int n=0;
    int nmax=0;
    Link st=new Node;
    pred=head;
    current=head;
    while(pred!=0)
    {   st->elem=pred->elem;
    n++;
    if(strcmp(st->elem.c_str(),"a")!=0&&strcmp(st->elem.c_str(),"an")!=0&&strcmp(st->elem.c_str(),"the")!=0&&strcmp(st->elem.c_str(),"in")!=0&&strcmp(st->elem.c_str(),"on")!=0&&strcmp(st->elem.c_str(),"of")!=0&&strcmp(st->elem.c_str(),"and")!=0&&strcmp(st->elem.c_str(),"is")!=0&&strcmp(st->elem.c_str(),"are")) //al7rof al mtkrrh kter
        {

        while(current->next!=0)
        {   current=current->next;
            if(strcmp(st->elem.c_str(),current->elem.c_str())==0)n++;

        }
        if(nmax<=n)
        {lis.insertUL(st->elem,n);
        nmax=n;

        }
        }
        n=0;
        pred=pred->next;
        current=pred;
    }



if(lis.head==0){
    cout<<"Most frequent word is: \n";
    return "File not found";
}

    inord_list list2;
    lis.pred=lis.head;

while(lis.pred!=0&&lis.pred->line_number== lis.head->line_number)

       {
           list2.insertSort(lis.pred->elem,0);

   lis.pred=lis.pred->next;
   }


list2.pred=list2.head;

cout<<"Most frequent word is: ";
while(list2.pred!=0&&list2.pred->line_number== list2.head->line_number)

       {

           cout<<list2.pred->elem<<" ";


   list2.pred=list2.pred->next;
   }
   cout<<'\n';

return list2.head->elem;

}
int inord_list::countWord(const ListElemType &e)
{
        if(head==0){//cout<<"Word not found\n";
                return 0;}
      int n=0;
      current=head;

      while(current!=0)
        { if(strcmp(e.c_str(),current->elem.c_str())==0)n++;
          current=current->next;}


    return n;
}

void inord_list::starting(const ListElemType &e)
{   int n=0;
    Link temp;
    inord_list list3;
   if(head==0){cout<<"Word not found\n";
                return ;}


      temp=head;

      while(temp!=0)
        {if(temp->elem.size()<e.size())goto L;
             if(strncmp(e.c_str(),temp->elem.c_str(),e.size())==0){
           n=countWord(temp->elem) ;

           list3.insertSort(temp->elem,n);//


          }
          L:
          temp=temp->next;}



          if(list3.head==0){cout<<"Word not found\n";
          return;}

            list3.pred=list3.head;

          while(list3.pred!=0)
          {
              cout<<list3.pred->elem<<": "<<list3.pred->line_number<<"\t";
              for(int i=0;list3.pred!=0&&i<list3.pred->line_number;i++)
                list3.pred=list3.pred->next;
          }
          cout<<endl;
}

void inord_list::containing(const ListElemType &e)
{   if(head==0)
{
    cout<<"Word not found\n";
    return;
    }

        int n=0;
       Link Temp=head;
       inord_list lis;
    while(Temp!=0)
    {if(Temp->elem.size()<e.size())goto L;
           for(int i=0;i<(Temp->elem.size()-e.size()+1);i++)
        {if(strcmp(Temp->elem.substr(i,e.size()).c_str(),e.c_str())==0)
        {n=countWord(Temp->elem);
        lis.insertSort(Temp->elem,n);
        break;
        }
        }
        L:
    Temp=Temp->next;
}
if(lis.head==0){cout<<"Word not found\n";
                return;}

Temp=lis.head;
while(Temp!=0)
{
    cout<<Temp->elem<<": "<<Temp->line_number<<'	';
    for(int i=0;Temp!=0&&i<Temp->line_number;i++)Temp=Temp->next;
}
cout<<endl;
}

void inord_list::search(const ListElemType &e)
{
    if(head==0)
{
    cout<<"Word not found\n";
    return;
    }

        int n=0;
       Link Temp=head;
       inord_list lis;
    while(Temp!=0)
    {   if(Temp->elem.size()<e.size())goto L;
           for(int i=0;i<(Temp->elem.size()-e.size()+1);i++)
        {if(strcmp(Temp->elem.substr(i,e.size()).c_str(),e.c_str())==0)
        {
            n=Temp->line_number;
        lis.insertSort(Temp->elem,n);
        break;
        }
        }
        L:
    Temp=Temp->next;
}

if(lis.head==0){cout<<"Word not found\n";
                return;}


Temp=lis.head;

while(Temp!=0)
{       cout<<Temp->elem<<":\tlines";
       cout<<" "<<Temp->line_number;
   // cout<<Temp->elem<<": lines ";

    while(Temp->next!=0&&strcmp(Temp->next->elem.c_str(),Temp->elem.c_str())==0)
    {   if(Temp->line_number!=Temp->next->line_number)
            cout<<" "<<Temp->next->line_number;
        Temp=Temp->next;}

                 if(Temp->next==0)break;
                 Temp=Temp->next;
                 cout<<endl;


            }
            cout<<endl;
}




void inord_list::command(inord_list lis,int &n)
{int k;
string st;
    if(head==0){cout<<"Word not found\n"; return;}


    Link current1=head;
    Link temp;
   while(current1!=0){ //im in a new line
       //if there is more than 2 elements in line
       temp=current1->next;
    if(current1->next!=0&&temp->next!=0&&current1->line_number==current1->next->line_number&&current1->next->line_number==temp->next->line_number)
    {
  if(strcmp(current1->elem.c_str(),"countWord")==0
     ||strcmp(current1->elem.c_str(),"starting")==0
     ||strcmp(current1->elem.c_str(),"containing")==0
     ||strcmp(current1->elem.c_str(),"search")==0
     ||strcmp(current1->elem.c_str(),"wordCount")==0
     ||strcmp(current1->elem.c_str(),"distWords")==0
     ||strcmp(current1->elem.c_str(),"charCount")==0
     ||strcmp(current1->elem.c_str(),"frequentWord")==0)
            cout<<"Incorrect number of arguments\n";
  else cout<<"Undefined command\n";

    while(current1!=0&&current1->next!=0&&current1->line_number==current1->next->line_number)
    {current1=current1->next;}
    if(current1!=0)current1=current1->next;


    } //2 elem
    else if(current1->next!=0&&current1->line_number==current1->next->line_number) {

        if(strcmp(current1->elem.c_str(),"countWord")==0)
            {k=lis.countWord(current1->next->elem);
            if(k!=-1)
                cout<<current1->next->elem<<" is repeated "<<k<<" times\n";
            }
       else if(strcmp(current1->elem.c_str(),"starting")==0)
            lis.starting(current1->next->elem);
       else if(strcmp(current1->elem.c_str(),"containing")==0)
            lis.containing(current1->next->elem);
        else if(strcmp(current1->elem.c_str(),"search")==0)
            lis.search(current1->next->elem);

        else {if(strcmp(current1->elem.c_str(),"countWord")==0
     ||strcmp(current1->elem.c_str(),"starting")==0
     ||strcmp(current1->elem.c_str(),"containing")==0
     ||strcmp(current1->elem.c_str(),"search")==0
     ||strcmp(current1->elem.c_str(),"wordCount")==0
     ||strcmp(current1->elem.c_str(),"distWords")==0
     ||strcmp(current1->elem.c_str(),"charCount")==0
     ||strcmp(current1->elem.c_str(),"frequentWord")==0)
            cout<<"Incorrect number of arguments\n";
  else cout<<"Undefined command\n";}

        while(current1!=0&&current1->next!=0&&current1->line_number==current1->next->line_number)
    {current1=current1->next;}
    if(current1!=0)current1=current1->next;

    }
    //1 elem
    else{
            if(strcmp(current1->elem.c_str(),"wordCount")==0)
            lis.wordCount();
       else if(strcmp(current1->elem.c_str(),"distWords")==0)
            lis.distWords();
       else if(strcmp(current1->elem.c_str(),"charCount")==0)
            lis.charCount(n);
        else if(strcmp(current1->elem.c_str(),"frequentWord")==0)
            lis.frequentWord();
        else {if(strcmp(current1->elem.c_str(),"countWord")==0
     ||strcmp(current1->elem.c_str(),"starting")==0
     ||strcmp(current1->elem.c_str(),"containing")==0
     ||strcmp(current1->elem.c_str(),"search")==0
     ||strcmp(current1->elem.c_str(),"wordCount")==0
     ||strcmp(current1->elem.c_str(),"distWords")==0
     ||strcmp(current1->elem.c_str(),"charCount")==0
     ||strcmp(current1->elem.c_str(),"frequentWord")==0)
            cout<<"Incorrect number of arguments\n";
  else cout<<"Undefined command\n";}

    if(current1!=0)current1=current1->next;
    }

   }

   return;

}





