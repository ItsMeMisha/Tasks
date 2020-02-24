#include <stdio.h>
#include <vector>
#include <math.h>

struct Tshirt_t {

    int price;
    short clrA;
    short clrB;

    Tshirt_t (const int price = 0, const int clrA = 0, const int clrB = 0) {

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

    Customer_t(const int price = -1, const short clr = 0) {

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

    SkipList(const T& Neginf, const T& Posinf) : NegInf {Neginf, maxHei}, PosInf {Posinf, maxHei} {

//        maxHei = 1;
        size = 0;

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
        maxHei = 0;
        NegInf.~Node<T>();
        PosInf.~Node<T>();            

    }

private:

    Node<T> NegInf;
    Node<T> PosInf;

    Node<T>* createObj (const T& elem) {

        Node<T>* obj = new Node<T>(elem, this->maxHei);
        return obj;

    }

    void erase ();
    std::vector<Node<T>*>* findPath (const T& elem); 

};

template <typename T>
struct Node {

    friend SkipList<T>;

    Node(const T& val, const int maxsize) : data(val){
        this->size = rand() % maxsize + 1;        
    };

    ~Node() {
        size = 0;
        forward.~vector();
    };

    typename std::vector<Node<T>*> forward;
    const T& data;
    int size;

};


void SortShirts (std::vector<SkipList<Tshirt_t>> color, int n, std::vector<Tshirt_t> shirts);
void SellStuff (std::vector<SkipList<Tshirt_t>> color, int m, std::vector<Customer_t> customer);
void ReadTshirts (const int size, std::vector<Tshirt_t> shirts); 
void ReadCustomers (const int num, std::vector<Customer_t> customers);
void PrintResults (const int num, std::vector<Customer_t> customers);
 
int main () {

    std::vector<SkipList<Tshirt_t>> color (3, SkipList<Tshirt_t>(NegInf, PosInf));

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
    
    for (int i = 0; i < 3; ++i)
        color[i].~SkipList();

    shirts.~vector();
    PrintResults (m, customers);
    customers.~vector();
    

    return 0;

}

void SortShirts (std::vector<SkipList<Tshirt_t>> color, int n, std::vector<Tshirt_t> shirts) {

    for (int i = 0; i < n; ++i) {
        
        color[shirts[i].clrA - 1].insert (shirts[i]);
        
        if (shirts[i].clrA != shirts[i].clrB)
            color[shirts[i].clrB - 1].insert (shirts[i]);

    }

    return;

}

void SellStuff (std::vector<SkipList<Tshirt_t>> color, int m, std::vector<Customer_t> customer) {

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

void ReadTshirts (const int size, std::vector<Tshirt_t> shirts) {

    for (int i = 0; i < size; ++i)
        scanf ("%d", &(shirts[i].price));

    for (int i = 0; i < size; ++i)
        scanf ("%hd", &(shirts[i].clrA));

    for (int i = 0; i < size; ++i)
        scanf ("%hd", &(shirts[i].clrB));

    return;

}

void ReadCustomers (const int num, std::vector<Customer_t> customers) {

    for (int i = 0; i < num; ++i)
        scanf ("%hd", &(customers[i].clr));
        
    return;

}

void PrintResults (const int num, std::vector<Customer_t> customers) {

    for (int i = 0; i < num; ++i)
        printf ("%d ", customers[i].price);

    return;

}

//======================================================================================================

template <typename T>
void SkipList<T>::insert (const T& elem) {

    if (elem <= this->NegInf.data || elem >= this->PosInf.data) { 
        
        printf ("!!!ERROR Inserting elem out of range\n");//out of range
        return; 
    }

    this->size++;
    int nextHei = static_cast<int>(std::log2(this->size));

    if (nextHei > this->maxHei) {

        this->maxHei =  nextHei;
        this->NegInf.forward.push_back(&this->PosInf);

    }

    Node<T>* newobj = createObj (elem);    
    std::vector<Node<T>*>* path = findPath (elem);

    for (int i = newobj->size-1; i >= 0; --i) {

        newobj->forward.push_back((*path)[i]);
        (*path)[i]->forward[i] = newobj;
       
    }
    
    delete path;

    return;

}

template <typename T>
std::vector<Node<T>*>* SkipList<T>::findPath (const T& elem) {

    std::vector<Node<T>*>* path = new std::vector<Node<T>*>;

    Node<T>* cur = &NegInf;    
    
    for (int height = this->maxHei-1; height >= 0; --height) {

        while (cur->forward[height]->data < elem)
            cur = cur->forward[height];

        path->push_back(cur);

    }

    return path;

}

template <typename T>
void SkipList<T>::remove (const T& elem) {

    if (elem <= this->NegInf.data || elem >= this->PosInf.data || this->empty()) { 

        printf ("!!!ERROR Inserting elem out of range\n");//out of range
        return; 

    }

    
    std::vector<Node<T>*>* path = findPath (elem);
    
    if (path->back()->data != elem)
        return;

    Node<T>* remElem = path->back();

    for (int i = remElem->size-1; i >= 0; --i)
        (*path)[i]->forward[i] = remElem->forward[i];
    
    delete remElem;
    delete path;

    return;

}

template <typename T>
T SkipList<T>::getfront () {

    if (empty()) 
        return NegInf.data;

    return NegInf.forward[0]->data;

}

template <typename T>   
void SkipList<T>::popfront () {

    if (empty())
        return;

    Node<T>* popping = NegInf.forward[0];

    for (int i = 0; i < popping->size; i++)
        this->NegInf.forward[i] =  popping->forward[i];

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


