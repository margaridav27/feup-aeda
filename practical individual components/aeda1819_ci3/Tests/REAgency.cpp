#include "REAgency.h"

REAgency::REAgency(): catalogItems(PropertyTypeItem("", "","", 0)) {
	//do nothing!
}

REAgency::REAgency(vector<Property*> properties): catalogItems(PropertyTypeItem("", "","", 0)) {
	this->properties = properties;
}

void REAgency::addProperty(Property* property) {
	this->properties.push_back(property);
}

void REAgency::addProperties(vector<Property*> properties) {
	this->properties = properties;
}

vector<Property*> REAgency::getProperties() const{
	return this->properties;
}

PropertyTypeItem REAgency::getTypeItem(string address, string postalCode, string typology) {
	PropertyTypeItem itemNotFound("", "", "", 0);
	BSTItrIn<PropertyTypeItem> it(catalogItems);
	while (!it.isAtEnd())
	{
		if( it.retrieve().getAddress() == address && it.retrieve().getPostalCode() == postalCode && it.retrieve().getTypology() == typology) {
			PropertyTypeItem pti(it.retrieve().getAddress(), it.retrieve().getPostalCode(), it.retrieve().getTypology(), 0);
			pti.setItems(it.retrieve().getItems());
			return pti;
		}
		it.advance();
	}
	return itemNotFound;
}

void REAgency::addTypeItem(Property* property) {
	PropertyTypeItem itemNotFound("", "","", 0);
	PropertyTypeItem pti(property->getAddress(), property->getPostalCode(), property->getTypology(), property->	getPrice());
	PropertyTypeItem ptiX = catalogItems.find(pti);
	if(ptiX == itemNotFound) {
		pti.addItems(property);
		this->catalogItems.insert(pti);
	}
	else {
		this->catalogItems.remove(ptiX);
		ptiX.addItems(property);
		this->catalogItems.insert(ptiX);
	}
	properties.push_back(property);
}

BST<PropertyTypeItem> REAgency::getTypeItems() const {
	return this->catalogItems;
}

vector<ClientRecord> REAgency::getClientRecords() const {
	vector<ClientRecord> records;
	HashTabClientRecord::const_iterator it1 = this->listingRecords.begin();
	HashTabClientRecord::const_iterator it2 = this->listingRecords.end();
	for(; it1 != it2; it1++) {
			records.push_back(*it1);
	}
	return records;
}

void REAgency::setClientRecords(vector<ClientRecord>& crs) {
	for(unsigned int i = 0; i < crs.size(); i++) {
		listingRecords.insert(crs[i]);
	}

}

priority_queue<Client> REAgency::getClientProfiles() const {
	return clientProfiles;
}

void REAgency::setClientProfiles(priority_queue<Client>& profiles) {
	clientProfiles = profiles;
}

//
// TODO: Part I   - BST
//
void REAgency::generateCatalog() {
    bool found;

	for (auto it = properties.begin(); it != properties.end(); it++) {
	    found = false;
	    PropertyTypeItem newCatalog((*it)->getAddress(),(*it)->getPostalCode(), (*it)->getTypology(), (*it)->getPrice());
        newCatalog.addItems(*it);

        BSTItrIn<PropertyTypeItem> iter(catalogItems);
        while (!iter.isAtEnd()) {
            if (iter.retrieve() == newCatalog) {
                iter.retrieve().addItems(*it);
                found = true;
                break;
            }
            iter.advance();
        }

        if (!found) {
            catalogItems.insert(newCatalog);
        }
	}
}

vector<Property*> REAgency::getAvailableProperties(Property* property) const {
	vector<Property*> available;
    for (auto itr = properties.begin(); itr != properties.end(); itr++) {
        if ((*itr)->getAddress() == property->getAddress() && (*itr)->getPostalCode() == property->getPostalCode() && (*itr)->getTypology() == property->getTypology() && (*itr)->getReservation() == tuple<Client*, int>()) {
            available.push_back((*itr));
        }
    }
	return available;
}

bool REAgency::reservePropertyFromCatalog(Property* property, Client* client, int percentage) {
    PropertyTypeItem catalogToSearch(property->getAddress(), property->getPostalCode(), property->getTypology(), property->getPrice());
	BSTItrIn<PropertyTypeItem> itr(catalogItems);
	while(!itr.isAtEnd()) {
        if (itr.retrieve() == catalogToSearch) {
            for (auto p = itr.retrieve().getItems().begin(); p != itr.retrieve().getItems().end(); p++) {
                if (get<0>((*p)->getReservation()) == NULL) {
                    int offer = (*p)->getPrice() * (1.0 - ((float)percentage / 100.0));
                    tuple<Client*, int> reservation(client, offer);
                    (*p)->setReservation(reservation);
                    client->addVisiting((*p)->getAddress(), (*p)->getPostalCode(), (*p)->getTypology(), to_string((*p)->getPrice()));
                    return true;
                }
            }
        }
        itr.advance();
	}
	return false;
}

//
// TODO: Part II  - Hash Table
//
void REAgency::addClientRecord(Client* client) {
	listingRecords.insert(client);

}

void REAgency::deleteClients() {
    auto it = listingRecords.begin();
	while (it != listingRecords.end()) {
	    if (it->getClientPointer()->getVisitedProperties().empty()) {
	        it = listingRecords.erase(it);
	    }
	    it++;
	}
}

//
// TODO: Part III - Priority Queue
//
void REAgency::addBestClientProfiles(const vector<Client>candidates, int min) {
	for (auto c = candidates.begin(); c != candidates.end(); c++) {
        float ratio = 0;
        for (auto p = properties.begin(); p != properties.end(); p++) {
	        if (get<0>((*p)->getReservation())->getEMail() == c->getEMail()) {
	            ratio++;
	        }
	    }
	    ratio = ratio / (*c).getVisitedProperties().size();
	    if (ratio > min) {
	        clientProfiles.push((*c));
	    }
	}

}

vector<Property*> REAgency::suggestProperties() {
	vector<Property*> suggestedProperties;
    priority_queue<Client> tempProfiles = clientProfiles;

    while (!tempProfiles.empty()) {
        Client currProfile = tempProfiles.top();
        tempProfiles.pop();

        if (currProfile.getVisiting() == tuple<string, string, string, string>()) {
            continue; //client has never visited any property
        }

        int minPCDiff = 9999999;
        int lastVisitedPC = stoi(get<1>(currProfile.getVisiting()));
        Property* propertyToSuggest;

        for (auto it = properties.begin(); it != properties.end(); it++) {
            if (get<0>((*it)->getReservation()) == NULL) { //the property to be suggested can not be reserved
                int currPCDiff = abs(stoi((*it)->getPostalCode()) - lastVisitedPC);
                if (currPCDiff != 0 && currPCDiff < minPCDiff) { //need to make sure currPCDiff != 0 otherwise could be suggesting the last visited property
                    propertyToSuggest = *it;
                    minPCDiff = currPCDiff;
                }
            }
        }
        suggestedProperties.push_back(propertyToSuggest);
    }
	return suggestedProperties;
}
