#ifndef INORD_LIST_H
#define INORD_LIST_H
#include <iostream>
using namespace std;
typedef string ListElemType;

class inord_list
{
    public:
        inord_list();
        bool insertUL(const ListElemType &e,const int &l);
         bool insertLU(const ListElemType &e,const int &l);
         bool insertSort(const ListElemType &e,const int &l);
        bool first (ListElemType &e,int &l);
        bool next(ListElemType &e,int &l);

        int wordCount();
        int distWords();
        int charCount(const int &n);
        string frequentWord();
        int countWord(const ListElemType &e);
        void starting(const ListElemType &e);
        void containing(const ListElemType &e);
        void search(const ListElemType &e);
        void command(inord_list lis,int &n);



    private:
        struct Node;
        typedef Node*Link;


        struct Node
        {
            ListElemType elem;
            int line_number;
            Link next;


        };
Link head;
Link current;
Link pred;
};

#endif // INORD_LIST_H
