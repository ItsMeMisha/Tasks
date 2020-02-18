#include <stdio.h>
#include <stack>

typedef unsigned long long ull;

void ReadRects (ull num, ull* recs);
ull FindMaxSq (ull num, ull* recs);
ull NewMaxSq (std::stack<ull>* heights, std::stack<ull>* pos, ull newHei, ull newPos, ull maxSq);
ull LastMaxCheck (std::stack<ull>* heights, std::stack<ull>* pos, ull maxSq);

int main () {

    ull num = 0;
    scanf ("%llu", &num);
    ull* recs = new ull[num];
    ReadRects (num, recs);
    ull maxSq = FindMaxSq (num, recs);
    delete[] recs;

    printf ("%llu", maxSq);

    return 0;

}

void ReadRects (ull num, ull* recs) {

    for (ull i = 0; i < num; i++)
        scanf ("%llu", recs+i);

    return;

}

ull FindMaxSq (ull num, ull* recs) {

    std::stack<ull> heights;
    std::stack<ull> pos;

    ull maxSq = 0;

    for (ull i = 0; i < num; ++i) {

        if (heights.empty() || heights.top() <= recs[i]) {

            heights.push(recs[i]);
            pos.push(i);

        } else
            maxSq = NewMaxSq (&heights, &pos, recs[i], i, maxSq);

    }

    maxSq = LastMaxCheck (&heights, &pos, maxSq);

    return maxSq;

}

ull NewMaxSq (std::stack<ull>* heights, std::stack<ull>* pos, ull newHei, ull newPos, ull maxSq) {

    ull curSq = newHei;
    maxSq = (curSq > maxSq) ? curSq : maxSq;
    ull numOfSkips = 0;
    while (heights->top() >= newHei) {

        curSq = heights->top() * (newPos - pos->top());
        maxSq = (curSq > maxSq) ? curSq : maxSq;
        heights->pop();
        numOfSkips = newPos - pos->top();
        pos->pop();

        if (heights->empty())
            break;

    }

    pos->push(newPos - numOfSkips);
    heights->push(newHei);
    return maxSq;

}

ull LastMaxCheck (std::stack<ull>* heights, std::stack<ull>* pos, ull maxSq) {

    ull curSq = 0;
    ull lastPos = pos->top();

    while (!heights->empty()) {

        curSq = heights->top() * (lastPos - pos->top() + 1);
        maxSq = (curSq > maxSq) ? curSq : maxSq;
        heights->pop();
        pos->pop();

    }

    return maxSq;
   
} 
