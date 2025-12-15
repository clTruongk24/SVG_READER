#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <map>
#include "Shape.h"

class factory {
private:
public:
	static shape* createShape(const string&, const map<string, string>&);
	static void ApplyCommonAttributes(shape*, const map<string, string>&);

};

#endif // !FACTORY_H
