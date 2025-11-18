#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <map>
#include "Shape.h"

class factory {
private:
	static void ApplyCommonAttributes(shape*, const map<string, string>&);
public:
	static shape* createShape(const string&, const map<string, string>&);
};

#endif // !FACTORY_H
