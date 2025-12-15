#ifndef GROUP_H
#define GROUP_H

#include "Shape.h"
#include "rapidxml.hpp"
#include <map>
#include <vector>

class group : public shape {
private:
	std::vector<shape*> children;

	std::map<string, string> groupAttributes;

	void ApplyInheritAttribute(shape*);
public:
	group();
	virtual ~group();

	void draw(Graphics&) override;
	void addChild(shape*);

	void setGroupAttributes(const std::map<std::string, std::string>&);


	std::map<std::string, std::string> getGroupAttributes() const;
};

#endif // !GROUP_H