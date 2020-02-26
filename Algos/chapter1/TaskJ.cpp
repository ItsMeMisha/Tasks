#include <stdio.h>
#include <vector>
#include <math.h>

struct Tshirt_t {

    int price;
    short clrA;
    short clrB;

    Tshirt_t() {};

    Tshirt_t (const int price, const int clrA = 0, const int clrB = 0) {

        this->price = price;
        this->clrA  = clrA;
        this->clrB  = clrB;

    };

    bool operator< (const Tshirt_t& rightop) const {
        return (price < rightop.price);
    };

    bool operator> (const Tshirt_t& rightop) const {
        return (price > rightop.price);
    };

    bool operator== (const Tshirt_t& rightop) const {
        return (price == rightop.price);
    };

    bool operator!= (const Tshirt_t& rightop) const {
        return (price != rightop.price);
    };

    bool operator<= (const Tshirt_t& rightop) const{
        return !(price > rightop.price);
    };

    bool operator>= (const Tshirt_t& rightop) const {
        return !(price < rightop.price);
    };

    Tshirt_t& operator= (const Tshirt_t& rightop) {

        this->price = rightop.price;
        this->clrA = rightop.clrA;
        this->clrB = rightop.clrB;
        return (*this);

    }

};

struct Customer_t {

    short clr;
    int price;

    Customer_t(const int clr = 0, const short price = -1) {

        this->price = price;
        this->clr = clr;

    };

};

const Tshirt_t NegInf(-1);
const Tshirt_t PosInf(2000000000);

template <typename T>
struct Node;

template <typename T>
class SkipList {

public:

    int size;
    int maxHei = 1;

    SkipList(const T& Neginf, const T& Posinf) {

        PosInf = new Node<T>(Posinf, maxHei);
        NegInf = new Node<T>(Neginf, maxHei);

        NegInf->forward[0] = PosInf;
        this->size = 0;

    }

    void insert (const T& elem);
    void remove (const T& elem);
    T getfront ();
    void popfront ();

    bool empty () {

        if (size <= 0)
            return true;

        return false;

    }

    ~SkipList() {    
        erase ();
    }

private:

    Node<T>* NegInf;
    Node<T>* PosInf;

    Node<T>* createObj (const T& elem) {

        int height = rand() % this->maxHei + 1;
        Node<T>* obj = new Node<T>(elem, height);

        return obj;

    }

    void erase ();
    std::vector<Node<T>*>* findPath (const T& elem); 

};

template <typename T>
struct Node {

    friend SkipList<T>;

    Node(const T& val, const int& Size) : data(val){

        this->size = Size;

        for (int i = 0; i < size; ++i)
            forward.push_back (nullptr);

    }

    std::vector<Node<T>*> forward;
    const T& data;
    int size;

};


void SortShirts (std::vector<SkipList<Tshirt_t>>& color, int n, const std::vector<Tshirt_t>& shirts);
void SellStuff (std::vector<SkipList<Tshirt_t>>& color, int m, std::vector<Customer_t>& customer);
void ReadTshirts (const int size, std::vector<Tshirt_t>& shirts); 
void ReadCustomers (const int num, std::vector<Customer_t>& customers);
void PrintResults (const int num, const std::vector<Customer_t>& customers);
 
int main () {

    std::vector<SkipList<Tshirt_t>> color;

    for (int i = 0; i < 3; i++)
        color.push_back ({NegInf, PosInf});

    int n = 0;
    scanf("%d", &n);
    std::vector<Tshirt_t> shirts;
    ReadTshirts (n, shirts);
  
    int m = 0;
    scanf("%d", &m);
    std::vector<Customer_t> customers;
    ReadCustomers (m, customers);

    SortShirts (color, n, shirts);
    SellStuff (color, m, customers);

    PrintResults (m, customers);

    return 0;

}

void SortShirts (std::vector<SkipList<Tshirt_t>>& color, int n, const std::vector<Tshirt_t>& shirts) {

    for (int i = 0; i < n; ++i) {
      
        color[shirts[i].clrA - 1].insert (shirts[i]);

        if (shirts[i].clrA != shirts[i].clrB)
            color[shirts[i].clrB - 1].insert (shirts[i]);

    }

    return;

}

void SellStuff (std::vector<SkipList<Tshirt_t>>& color, int m, std::vector<Customer_t>& customer) {

    Tshirt_t selled;
    short clr = 0;

    for (int i = 0; i < m; ++i) {

        clr = customer[i].clr - 1;
        selled = color[clr].getfront();
        customer[i].price = selled.price;
        color[clr].popfront();

        if (selled.price > 0) {

            if (clr + 1 != selled.clrA)
                color[selled.clrA - 1].remove(selled);

            if (clr + 1 != selled.clrB)
                color[selled.clrB - 1].remove(selled);

        }

    }

    return;

}

void ReadTshirts (const int size, std::vector<Tshirt_t>& shirts) {

    int price = 0;
    short clrA = 0;
    short clrB = 0;

    for (int i = 0; i < size; ++i) {

        scanf ("%d", &price);
        shirts.push_back(*(new Tshirt_t (price)));
    }

    for (int i = 0; i < size; ++i) {

        scanf ("%hd", &clrA);
        shirts[i].clrA = clrA;

    }

    for (int i = 0; i < size; ++i) {

        scanf ("%hd", &clrB);
        shirts[i].clrB = clrB;

    }

    return;

}

void ReadCustomers (const int num, std::vector<Customer_t>& customers) {

    short clr = 0;

    for (int i = 0; i < num; ++i) {

        scanf ("%hd", &clr);
        customers.push_back(*(new Customer_t (clr)));

    }
        
    return;

}

void PrintResults (const int num, const std::vector<Customer_t>& customers) {

    for (int i = 0; i < num; ++i)
        printf ("%d ", customers[i].price);

    return;

}

//======================================================================================================

template <typename T>
void SkipList<T>::insert (const T& elem) {

    if (elem <= this->NegInf->data || elem >= this->PosInf->data) { 
        
        printf ("!!!ERROR Inserting elem out of range\n");//out of range
        return; 
    }

    this->size++;
    int nextHei = static_cast<int>(std::log2(this->size));

    if (nextHei > this->maxHei) {

        this->maxHei = nextHei;
        this->NegInf->forward.push_back(this->PosInf);
        this->NegInf->size++;

    }

    Node<T>* newobj = createObj (elem);   

    std::vector<Node<T>*>* path = findPath (elem);
    int psize = path->size();

    for (int i = 0; i < newobj->size; ++i) {

        newobj->forward[i] = (*path)[psize-i-1]->forward[i];
        (*path)[psize-i-1]->forward[i] = newobj;
       
    }
   
    delete path;

    return;

}

template <typename T>
std::vector<Node<T>*>* SkipList<T>::findPath (const T& elem) {

    std::vector<Node<T>*>* path = new std::vector<Node<T>*>;

    Node<T>* cur = this->NegInf;  

    for (int height = this->maxHei-1; height >= 0; --height) {

        while (cur->forward[height]->data < elem)
           cur = cur->forward[height];

        path->push_back(cur);

    }

    return path;

}

template <typename T>
void SkipList<T>::remove (const T& elem) {

    if (elem <= this->NegInf->data || elem >= this->PosInf->data) { 

        printf ("!!!ERROR Removing elem out of range\n");//out of range
        return; 

    }

    if (empty())
        return;   
 
/*    std::vector<Node<T>*>* path = findPath (elem);
    int psize = path->size();
    
    if (path->back()->forward[0]->data != elem)
        return;

    for (int i = 0; i < remElem->size; ++i)
        (*path)[psize-i-1]->forward[i] = remElem->forward[i]; */

    Node<T>* cur = this->NegInf;  
    Node<T>* remElem = nullptr;

    for (int height = this->maxHei-1; height >= 0; --height) {

        while (cur->forward[height]->data < elem)
           cur = cur->forward[height];

        if (height == 0)
            remElem = cur->forward[height];

        if (cur->forward[height]->data == elem)
            cur->forward[height] = cur->forward[height]->forward[height];

    }  

    if (remElem == nullptr)
        return;

    this->size--; 
    delete remElem;
//    delete path;

    return;

}

template <typename T>
T SkipList<T>::getfront () {

    if (empty()) 
        return NegInf->data;

    return NegInf->forward[0]->data;

}

template <typename T>   
void SkipList<T>::popfront () {

    if (empty())
        return;

    Node<T>* popping = NegInf->forward[0];

    for (int i = 0; i < popping->size; i++)
        this->NegInf->forward[i] =  popping->forward[i];

    this->size--;

    delete popping;
    
    return;
    
}

template <typename T>
void SkipList<T>::erase () {

        while (!empty()) {

            popfront();

        }

        return;
}


