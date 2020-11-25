// CSimpleList - Implementacao Simples de Lista Ligada
#include <iostream>
#include <string>

using namespace std;

class CNode{ 
private: 
    int d_data;   
    CNode *d_next; 
public:
    CNode(int data, CNode *next){ d_data=data; d_next=next; }
    void setData(int data) { d_data=data; }
    void setNext(CNode *next) { d_next=next; }
    int data() const { return d_data;}
    CNode *next() const { return d_next;}
    void print() const { cout << d_data << " ";}
};

class CSimpleList {
private: 
	 CNode *first; 
public:
    CSimpleList() { first=0; }
    ~CSimpleList() { 
        CNode *aux=first; CNode *cur;
        while(aux!=0) { cur = aux; aux = aux->next(); delete cur; }
    }
    bool empty() const { return (first==0); }
    string toStr() const {
    	stringstream oss;
    	CNode *aux=first;
        while(aux!=0) { oss << aux->data() << " "; aux=aux->next(); }
        return oss.str();
     }
    void print() const {   
        CNode *aux=first;
        cout << "List: ";
        while(aux!=0) { aux->print(); aux=aux->next(); }
        cout << endl;
    }
	CNode *find(int elem) { 
      CNode *aux = first;
      while(aux!=0 && aux->data()!=elem) aux=aux->next();
      if(aux!=0) return aux;
      else { cerr << "Elem is not in list\n"; return 0;}
  	}
	void insert_head(int elem){ 
      CNode *res = new CNode(elem, first);
      first = res;
	}
	void insert_end(int elem){ 
      CNode *node, *aux;
      node = new CNode(elem,0); 
      aux = first;
      if(aux==0) first=node;
      else { 
          while(aux->next()!=0) aux = aux->next();
          aux->setNext(node);
      }
	}	
	void insert_sort(int elem) { 
    	CNode *prev, *node, *aux;
    	node = new CNode(elem,0); prev = 0; aux = first;
    	while(aux!=0 && aux->data()<elem) { prev = aux; aux = aux->next();}
    	node->setNext(aux);
    	if(prev==NULL) first=node; else prev->setNext(node);
  	}
	void intercalar(const CSimpleList &lst) { //Grupo 2 c)
        CNode* b = lst.first;
        CNode* a = first;
        //if *this is empty, *this will end up equal to lst
        //if lst is empty, *this will remain equal
        while (true) {
            if (b != 0) {
                CNode* toInsert = new CNode(b->data(), a->next()); //the node to be inserted has the same data as the one from lst and points to the next of the original *this
                a->setNext(toInsert);
                b = b->next(); //'b' now points to the next element of lst that we will be inserting in *this
            }
            if (a != 0) {
                a = a->next(); //'a' now points to the element from lst that we have just inserted, 'b'
                if (a != 0) {
                    a = a->next(); //'a' now points to the next element of the original *this
                }
            }
            if (a == 0 && b == 0) { //we finished traversing both lists so we can't do anything else
                break;
            }
        }
	}
	int zipar() { //Grupo 2 d)
        CNode* node = first;
        CNode* nextNode = node;
        int counter = 0;
        while (true) {
            if (node != 0) {
                while (true) {
                    nextNode = nextNode->next();
                    if (nextNode != 0) {
                        if (nextNode->data() == node->data()) counter++;
                        else break;
                    } else break;
                }
                node->setNext(nextNode);
                node = node->next();
                if (node == 0) break;
            } else break;
        }
        return counter;
    }
}; 

