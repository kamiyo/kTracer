/*

kTracer by kamiyo

*/

#include "utils.h"
#include "main.h"
#include "Scene.h"
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
	std::string filename(argv[1]);
	Scene* scene = new Scene(filename);
	//Parser* p = new Parser(filename);
	return 0;

}