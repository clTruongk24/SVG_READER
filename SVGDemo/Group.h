#ifndef GROUP_H
#define GROUP_H

#include "Shape.h"
#include "rapidxml.hpp"
#include <map>
#include <vector>

class group : public shape {
private:
	std::vector<shape*> children;

	static void ApplyCommonAttributes(shape*, const std::map<std::string, std::string>&);
public:
	group();
	virtual ~group();
	void draw(Graphics&) override;
	void addChild(shape*);

	void ApplyAttributes(const std::map<std::string, std::string>&);
};

#endif // !GROUP_H