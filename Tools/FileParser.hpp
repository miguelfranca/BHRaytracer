/*
Usage:
	FileParser fp("params.txt",true);

	type t;
	//option 1
	fp("name",t,default_value);
	//option 2
	t = fp.get<type>("name");
	//option 2 if known default
	t = fp.get("name",default_value);
*/

#ifndef FILEPARSER_
#define FILEPARSER_

#include <fstream>
#include <string>
#include <vector>
#include "Tools.hpp"

class FileParser
{
public:
	FileParser(const std::string& p_name, bool p_verbose = false, char p_setter = '='):
		name(p_name), open(true), setter(p_setter), verbose(p_verbose)
	{
		file.open(name, std::ios::in);

		if (!file) {
			print("File ", name, " not found");
			open = false;
		}
	}

	~FileParser() { file.close(); }

	template <typename T>
	bool get(const std::string& id, T& var);
	template<typename T>
	bool get(const std::string& id, std::vector<T>& var);
	template <typename T>
	bool operator()(const std::string& id, T& var);

	template <typename T>
	bool get(const std::string& id, T& var, const T& def);
	template <typename T>
	bool operator()(const std::string& id, T& var, const T& def);

	//pro: no need to send variable as argument
	//con: returns default value if not found
	template <typename T>
	T get(const std::string& id, const T& def = T());

	inline bool isOpen() { return open; }

private:
	std::string name;	//save name of file (for printing purposes)
	std::ifstream file;	//file opened
	bool open;			//flag if file was correctly opened
	char setter;		//character separating ID from its value
	bool verbose;		//whether or not to print stuff when making requests
};

#include "FileParser.impl.hpp"

#endif /* FILEPARSER_ */