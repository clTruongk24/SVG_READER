#ifndef DEFS_H
#define DEFS_H

#include <map>
#include <string>
#include "Gradient.h"

class Defs
{
private:
	static std::map<std::string, GradientBase *> gradients;
	static std::map<std::string, std::string> styles;

	Defs() = default;
	Defs(const Defs &) = delete;
	Defs &operator=(const Defs &) = delete;
	Defs(Defs &&) = delete;
	Defs &operator=(Defs &&) = delete;

public:
	static Defs &getInstance();

	~Defs();

	 void addGradient(GradientBase *);
	 GradientBase *getGradient(const std::string &) const;
	 bool hasGradient(const std::string &) const;

	 void addStyle(const std::string &, const std::string &);
	 std::string getStyle(const std::string &) const;
	 bool hasStyle(const std::string &) const;

	static std::map<std::string, std::string> parseStyleString(const std::string &);
	static std::string extractIDFromURL(const std::string &);

	void clear();
};

#endif // !DEFS_H
