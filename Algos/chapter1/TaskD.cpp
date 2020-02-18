#include <stdio.h>
#include <list>

void ProcReqs (int num);
void AddNorm (std::list<int>* queue, std::list<int>::iterator* mid, int hNum);
void AddVip  (std::list<int>* queue, std::list<int>::iterator* mid, int hNum);
void Leave   (std::list<int>* queue, std::list<int>::iterator* mid);

int main () {

    int num = 0;
    scanf ("%d%*c", &num);
    ProcReqs (num);

    return 0;

}

void ProcReqs (int num) {

    std::list<int> hoblins;
    char com = 0;
    int hNum = 0;
    std::list<int>::iterator mid = hoblins.begin();

    for (int i = 0; i < num; i++) {

        scanf ("%c", &com);

        switch (com) {

        case '+': scanf ("%d%*c", &hNum); AddNorm (&hoblins, &mid, hNum);  break;
        case '*': scanf ("%d%*c", &hNum); AddVip  (&hoblins, &mid, hNum);  break;
        case '-': scanf ("%*c"); Leave (&hoblins, &mid);  break;

        }

    }

    return;

}

void AddNorm (std::list<int>* queue, std::list<int>::iterator* mid, int hNum) {

    queue->push_back(hNum);

    int size = queue->size();

    if (size == 1)
        *mid = queue->begin();

    else if (size < 4)
        (*mid)++;

    else if (size & 1)
        (*mid)++;

    return;

}

void AddVip (std::list<int>* queue, std::list<int>::iterator* mid, int hNum) {

    int size = queue->size();
    
    if (size <= 1) 
        queue->push_back(hNum);
    else
        queue->insert (*mid, hNum);

    size++;
    
    if (size == 1)
        *mid = queue->begin();

    else if (size == 2)
        (*mid)++;

    else if (size % 2 ==0)
        (*mid)--;

    return;

}

void Leave (std::list<int>* queue, std::list<int>::iterator* mid) {

    if (!queue->empty()) {

        printf ("%d ", queue->front());
        queue->pop_front();

    }

    int size = queue->size();

    if ((size > 1) && (size % 2 == 1))
        (*mid)++;

    return;

}

