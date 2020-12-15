#include "packagingMachine.h"
#include <sstream>

PackagingMachine::PackagingMachine(int boxCap): boxCapacity(boxCap) {}

unsigned PackagingMachine::numberOfBoxes() { return boxes.size(); }

unsigned PackagingMachine::addBox(Box& b1) {
    boxes.push(b1);
    return boxes.size();
}

HeapObj PackagingMachine::getObjects() const { return this->objects; }

HeapBox PackagingMachine::getBoxes() const { return this->boxes; }

unsigned PackagingMachine::loadObjects(vector<Object> &objs) {
    unsigned counter = 0;
	for (vector<Object>::iterator it = objs.begin(); it != objs.end(); it++) {
	    if ((*it).getWeight() <= boxCapacity) {
            objects.push(*it);
            counter++;
            objs.erase(it);
            it--; //erase returns an iterator pointing to the new location of the element that followed the last element erased by the function call
	    }
	}
	return counter;
}

Box PackagingMachine::searchBox(Object& obj) {
	Box b;
	bool found = false;
    vector<Box> temp;
	while (!boxes.empty()) {
	    if (!found && boxes.top().getFree() >= obj.getWeight()) {
            b = boxes.top();
            boxes.pop();
            found = true;
        }
	    else {
	        temp.push_back(boxes.top());
	        boxes.pop();
	    }
	}
	if (!found) {
	    b = Box(boxCapacity);
	}
	for (vector<Box>::iterator it = temp.begin(); it != temp.end(); it++) {
	    boxes.push(*it);
	} temp.clear();
	return b;
}

unsigned PackagingMachine::packObjects() {
    unsigned counter = 0;
    vector<Object> vecObjects;
    while (!objects.empty()) {
        vecObjects.push_back(objects.top());
        objects.pop();
    }
    for (vector<Object>::iterator o = vecObjects.begin(); o != vecObjects.end(); o++) {
        Box b = searchBox(*o);
        if (b.getFree() == boxCapacity) { //otherwise it means we are packing on a box where has already been packed an object
            counter++;
        }
        b.addObject(*o);
        boxes.push(b);
        vecObjects.erase(o); //packed objects are removed from the queue
        o--;
    }
    for (vector<Object>::iterator o = vecObjects.begin(); o != vecObjects.end(); o++) {
        objects.push(*o);
    } vecObjects.clear();
	return counter;
}

string PackagingMachine::printObjectsNotPacked() const {
    if (objects.empty()) {
        return "No objects!\n";
    }
    stringstream out;
    HeapObj temp = objects;
    while (!temp.empty()) {
        out << temp.top() << '\n';
        temp.pop();
    }
    return out.str();
}

Box PackagingMachine::boxWithMoreObjects() const {
	if (boxes.empty()) {
	    throw MachineWithoutBoxes();
	}
	HeapBox temp = boxes;
	Box res;
	while (!temp.empty()) {
	    if (temp.top().getSize() > res.getSize()) {
	        res = temp.top();
	    }
        temp.pop();
	}
	return res;
}