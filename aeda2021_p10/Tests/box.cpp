#include "box.h"
#include <sstream>

Object::Object(unsigned i, unsigned w): id(i), weight(w) {}

unsigned Object::getID() const { return id; }

unsigned Object::getWeight() const { return weight; }

bool Object::operator<(const Object& o1) const { return weight < o1.weight; }

ostream& operator<<(ostream& os, Object obj) {
    os << "O" << obj.id << ":" << obj.weight;
    return os;
}


unsigned Box::lastId = 1;

Box::Box(unsigned cap): id(lastId++), capacity(cap), free(cap) {}

unsigned Box::getID() const { return id; }

unsigned Box::getFree() const { return free; }

unsigned Box::getSize() const { return objects.size(); }

void Box::addObject(Object& obj) {
    free -= obj.getWeight();
    objects.push(obj);
}

void Box::resetID(){ lastId = 1; }

bool Box::operator<(const Box& b1) const { return free > b1.free; } //the box with highest priority is the one with less free space

string Box::printContent() const {
    stringstream out;
    if (free == capacity) {
        out << "Box " << id << " empty!\n";
        return out.str();
    }
    StackObj temp = objects;
    out << "Box " << id << " [ ";
    while (!temp.empty()) {
        out << temp.top() << " ";
        temp.pop();
    }
    out << "]";
    return out.str();
}