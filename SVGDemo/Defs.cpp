#include "stdafx.h"
#include "Defs.h"
#include <sstream>
#include <algorithm>

using namespace std;

map<string, GradientBase*> Defs::gradients;
map<string, string> Defs::styles;

Defs::~Defs() {
	clear();
}
 
Defs& Defs::getInstance() {
	static Defs instance;
	return instance;
}

void Defs::addGradient(GradientBase* gradient) {
	if (gradient && !gradient->getId().empty()) {
		if (gradients.count(gradient->getId())) {
			delete gradients[gradient->getId()];
		}
		gradients[gradient->getId()] = gradient;
	}
}

GradientBase* Defs::getGradient(const string& id) const {
	if (gradients.count(id)) {
		return gradients.at(id);
	}
	return nullptr;
}

bool Defs::hasGradient(const string& id) const {
	return gradients.count(id) > 0;
}

void Defs::addStyle(const string& id, const string& style) {
	styles[id] = style;
}

string Defs::getStyle(const string& id) const {
	if (styles.count(id)) {
		return styles.at(id);
	}
	return "";
}	

bool Defs::hasStyle(const string& id) const {
	return styles.count(id) > 0;
}

map<string, string> Defs::parseStyleString(const string& styleStr) {
	map<string, string> attributes;

	if (styleStr.empty()) {
		return attributes;
	}

	stringstream ss(styleStr);
	string item;

	while (getline(ss, item, ';')) {
		item.erase(0, item.find_first_not_of(" \t\n\r"));
		item.erase(item.find_last_not_of(" \t\n\r") + 1);

		if (item.empty()) {
			continue;
		}

		size_t colonPos = item.find(':');
		if (colonPos == string::npos) {
			continue;
		}

		string key = item.substr(0, colonPos);
		string value = item.substr(colonPos + 1);

		key.erase(0, key.find_first_not_of(" \t\n\r"));
		key.erase(key.find_last_not_of(" \t\n\r") + 1);
		value.erase(0, value.find_first_not_of(" \t\n\r"));
		value.erase(value.find_last_not_of(" \t\n\r") + 1);

		if (!key.empty() && !value.empty()) {
			attributes[key] = value;
		}
	}
	return attributes;
}

string Defs::extractIDFromURL(const string& url) {
	
	size_t start = url.find("#");
	if (start == string::npos) {
		return "";
	}

	start++;
	size_t end = url.find(")", start);
	if (end == string::npos) {
		end = url.length();
	}

	string id = url.substr(start, end - start);

	id.erase(0, id.find_first_not_of(" \t\n\r"));
	id.erase(id.find_last_not_of(" \t\n\r") + 1);

	return id;
}

void Defs::clear() {
	if (!gradients.empty()) {
		for (auto& p : gradients) {
			delete p.second;
			p.second = nullptr;
		}
		gradients.clear();
	}

	styles.clear();
}