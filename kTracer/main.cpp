/*

kTracer by kamiyo

*/

#include "utils.h"
#include "main.h"
#include "Parser.h"
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
	std::string filename(argv[1]);
	Parser* p = new Parser(filename);
	return 0;
}