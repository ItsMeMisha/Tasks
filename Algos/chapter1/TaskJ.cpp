#include <stdio.h>
#include <vector>
#include <math.h>

struct Tshirt {

    int price;
    short colorA;
    short colorB;

    Tshirt (const int price = 0, const int colorA = 0, const int colorB = 0) {

        this->price = price;
        this->colorA  = colorA;
        this->colorB  = colorB;

    };

    bool operator< (const Tshirt& rightop) const {
        return (price < rightop.price);
    };

    bool operator> (const Tshirt& rightop) const {
        return (price > rightop.price);
    };

    bool operator== (const Tshirt& rightop) const {
        return (price == rightop.price);
    };

    bool operator!= (const Tshirt& rightop) const {
        return (price != rightop.price);
    };

    bool operator<= (const Tshirt& rightop) const{
        return !(price > rightop.price);
    };

    bool operator>= (const Tshirt& rightop) const {
        return !(price < rightop.price);
    };

    Tshirt& operator= (const Tshirt& rightop) {

        this->price = rightop.price;
        this->colorA = rightop.colorA;
        this->colorB = rightop.colorB;
        return (*this);

    }

};

struct Customer {

    short color;
    int price;

    Customer(const int color = 0, const short price = -1) {

        this->price = price;
        this->color = color;

    };

};

const Tshirt NegInf(-1);
const Tshirt PosInf(2000000000);

template <typename T>
class SkipList {

public:

    int size;
    const int maxHeight = 32;

    SkipList(const T& Neginf, const T& Posinf) {

        PosInf = new Node(Posinf, maxHeight);
        NegInf = new Node(Neginf, maxHeight);

        for (int i = 0; i < this->maxHeight; ++i)
            NegInf->forward[i] = PosInf;

        this->size = 0;

    }

    void insert (const T& elem);
    void remove (const T& elem);
    T getFront ();
    void popFront ();

    bool empty () {

        if (size <= 0)
            return true;

        return false;

    }

    ~SkipList() {    

        clear ();
    
    }

private:
 
    struct Node {

        Node(const T& val, const int& Size) : data(val){

            this->size = Size;

            for (int i = 0; i < size; ++i)
                forward.push_back (nullptr);

        }

        std::vector<Node*> forward;
        const T& data;
        int size;

    };


    Node* NegInf;
    Node* PosInf;

    int newHeight (const int& maxHeight) {

        int newHeight = 1;

        while ((rand() % 2) && newHeight < maxHeight)
            newHeight++;

        return newHeight;

    }
    
    Node* createObj (const T& elem) {

        int height = newHeight (this->maxHeight);
        Node* obj = new Node(elem, height);

        return obj;

    }

    void clear ();

    std::vector<Node*>* findPath (const T& elem) {

        std::vector<Node*>* path = new std::vector<Node*>;

        Node* cur = this->NegInf;  

        for (int height = this->maxHeight-1; height >= 0; --height) {

                while (cur->forward[height]->data < elem)
                cur = cur->forward[height];
                
                path->push_back(cur);

        }

        return path;

    }

};


void SortShirts (std::vector<SkipList<Tshirt>>& colors, int n, const std::vector<Tshirt>& shirts);
void SellStuff (std::vector<SkipList<Tshirt>>& colors, int m, std::vector<Customer>& customer);
void ReadTshirts (const int size, std::vector<Tshirt>& shirts); 
void ReadCustomers (const int num, std::vector<Customer>& customers);
void PrintResults (const int num, const std::vector<Customer>& customers);
 
int main () {

    std::vector<SkipList<Tshirt>> color;

    for (int i = 0; i < 3; i++)
        color.push_back ({NegInf, PosInf});

    int n = 0;
    scanf("%d", &n);
    std::vector<Tshirt> shirts;
    ReadTshirts (n, shirts);
  
    int m = 0;
    scanf("%d", &m);
    std::vector<Customer> customers;
    ReadCustomers (m, customers);

    SortShirts (color, n, shirts);
    SellStuff (color, m, customers);

    PrintResults (m, customers);

    return 0;

}

void SortShirts (std::vector<SkipList<Tshirt>>& colors, int n, const std::vector<Tshirt>& shirts) {

    for (int i = 0; i < n; ++i) {
      
        colors[shirts[i].colorA - 1].insert (shirts[i]);

        if (shirts[i].colorA != shirts[i].colorB)
            colors[shirts[i].colorB - 1].insert (shirts[i]);

    }

    return;

}

void SellStuff (std::vector<SkipList<Tshirt>>& colors, int m, std::vector<Customer>& customer) {

    Tshirt selled;
    short color = 0;

    for (int i = 0; i < m; ++i) {

        color = customer[i].color - 1;
        selled = colors[color].getFront();
        customer[i].price = selled.price;
        colors[color].popFront();

        if (selled.price > 0) {

            if (color + 1 != selled.colorA)
                colors[selled.colorA - 1].remove(selled);

            if (color + 1 != selled.colorB)
                colors[selled.colorB - 1].remove(selled);

        }

    }

    return;

}

void ReadTshirts (const int size, std::vector<Tshirt>& shirts) {

    int price = 0;
    short colorA = 0;
    short colorB = 0;
    Tshirt newTshirt;

    for (int i = 0; i < size; ++i) {

        scanf ("%d", &price);
        newTshirt.price = price;
        shirts.push_back(newTshirt);
    }

    for (int i = 0; i < size; ++i) {

        scanf ("%hd", &colorA);
        shirts[i].colorA = colorA;

    }

    for (int i = 0; i < size; ++i) {

        scanf ("%hd", &colorB);
        shirts[i].colorB = colorB;

    }

    return;

}

void ReadCustomers (const int num, std::vector<Customer>& customers) {

    short color = 0;
    Customer newCust;

    for (int i = 0; i < num; ++i) {

        scanf ("%hd", &color);
        newCust.color = color;
        customers.push_back(newCust);

    }
        
    return;

}

void PrintResults (const int num, const std::vector<Customer>& customers) {

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

    Node* newobj = createObj (elem);   

    std::vector<Node*>* path = findPath (elem);
    int psize = path->size();

    for (int i = 0; i < newobj->size; ++i) {

        newobj->forward[i] = (*path)[psize-i-1]->forward[i];
        (*path)[psize-i-1]->forward[i] = newobj;
       
    }
   
    delete path;

    return;

}

template <typename T>
void SkipList<T>::remove (const T& elem) {

    if (elem <= this->NegInf->data || elem >= this->PosInf->data) { 

        printf ("!!!ERROR Removing elem out of range\n");//out of range
        return; 

    }

    if (empty())
        return;   
 
    Node* cur = this->NegInf;  
    Node* remElem = nullptr;

    for (int height = this->maxHeight-1; height >= 0; --height) {

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

    return;

}

template <typename T>
T SkipList<T>::getFront () {

    if (empty()) 
        return NegInf->data;

    return NegInf->forward[0]->data;

}

template <typename T>   
void SkipList<T>::popFront () {

    if (empty())
        return;

    Node* popping = NegInf->forward[0];

    for (int i = 0; i < popping->size; i++)
        this->NegInf->forward[i] =  popping->forward[i];

    this->size--;

    delete popping;
    
    return;
    
}

template <typename T>
void SkipList<T>::clear () {

        while (!empty()) {

            popFront();

        }

        return;
}


