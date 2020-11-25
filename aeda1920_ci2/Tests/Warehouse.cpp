#include "Warehouse.h"
#include <algorithm>

Warehouse::Warehouse()  {
}

queue<ThinTallBox> Warehouse::getBoxes() {
	return boxes;
}

queue<Object> Warehouse::getObjects() {
    return objects;
}

void Warehouse::setBoxes(queue<ThinTallBox> q) {
    while (!q.empty()) {
        boxes.push(q.front());
        q.pop();
    }
}

void Warehouse::addObject(Object o1) {
    objects.push(o1);
}

int Warehouse::InsertObjectsInQueue(vector <Object> obj){
    sort(obj.begin(), obj.end());
    for (vector<Object>::iterator it = obj.begin(); it != obj.end(); it++) {
        objects.push((*it));
    }
    obj.size();
}

Object Warehouse::RemoveObjectQueue(int maxSize){
     vector<Object> vec;
     Object obj("dummy", maxSize);
     bool first = true;
     while (!objects.empty()) {
         vec.push_back(objects.front());
         objects.pop();
     }
     for (vector<Object>::iterator it = vec.begin(); it != vec.end(); it++) {
         if ((*it).getSize() <= maxSize) {
             if (first) {
                 obj = (*it);
                 first = false;
             }
             else {
                 objects.push((*it));
             }
         }
         else {
             objects.push((*it));
         }
     }
     return obj;
}

int Warehouse::addPackOfBoxes(vector <ThinTallBox> boV) {
    for (vector<ThinTallBox>::iterator it = boV.begin(); it != boV.end(); it++) {
        boxes.push((*it));
    }
    return boxes.size();
}

vector<ThinTallBox> Warehouse::DailyProcessBoxes(){
    vector<ThinTallBox> vecBox;
    vector<ThinTallBox> dispatched;
    while (!boxes.empty()) {
        vecBox.push_back(boxes.front());
        boxes.pop();
    }
    for (vector<ThinTallBox>::iterator b = vecBox.begin(); b != vecBox.end(); b++) {
        if ((*b).full()) { //caixa cheia - despachada
            dispatched.push_back((*b));
        }
        else if ((*b).getDays() == 0) { //data de envio expirou - processada e despachada
            if ((*b).full()) {
                dispatched.push_back((*b));
            }
            else {
                vector<Object> vecObj;
                while (!objects.empty()) {
                    vecObj.push_back(objects.front());
                    objects.pop();
                }
                Object obj("dummy", 0);
                bool processed = false;
                for (vector<Object>::iterator o = vecObj.begin(); o != vecObj.end(); o++) {
                    if ((*b).getContentSize() + (*o).getSize() <= (*b).getCapacity()) {
                        if (!processed) {
                            (*b).insert((*o));
                            (*b).setContentSize((*b).getContentSize() + (*o).getSize());
                            processed = true;
                        }
                        else {
                            objects.push((*o));
                        }
                    }
                    else {
                        objects.push((*o));
                    }
                }
                if (!processed) {
                    (*b).insert(obj);
                }
                dispatched.push_back((*b));
            }
        }
        else { //processada e movida para o final da fila, decrementando o prazo de envio
            vector<Object> vecObj;
            while (!objects.empty()) {
                vecObj.push_back(objects.front());
                objects.pop();
            }
            Object obj("dummy", 0);
            bool processed = false;
            for (vector<Object>::iterator o = vecObj.begin(); o != vecObj.end(); o++) {
                if ((*b).getContentSize() + (*o).getSize() <= (*b).getCapacity()) {
                    if (!processed) {
                        (*b).insert((*o));
                        (*b).setContentSize((*b).getContentSize() + (*o).getSize());
                        processed = true;
                    }
                    else {
                        objects.push((*o));
                    }
                }
                else {
                    objects.push((*o));
                }
            }
            if (!processed) {
                (*b).insert(obj);
            }
            (*b).setDaysToSend((*b).getDays() - 1);
            boxes.push((*b));
        }
    }
    return dispatched;
}