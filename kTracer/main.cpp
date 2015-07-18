/*

kTracer by kamiyo

*/

#include "utils.h"
#include "main.h"
#include "Scene.h"
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
	clock_t start = clock();
	std::string inFile, outFile;
	if (argc == 3) {
		inFile = argv[1];
		outFile = argv[2];
	}
	else if (argc == 2) {
		inFile = argv[1];
		outFile = inFile.substr(0, inFile.find_last_of('.')) + ".exr";
	}
	else {
		std::cerr << "Wrong number of arguments! usage:\nkTracer scene.yaml (out.exr)" << std::endl;
		return 0;
	}

	Scene* scene = new Scene(inFile);
	int width = scene->m_width, height = scene->m_height;

	MatrixRgba output;
	scene->render(output);
		
	writeRgba(outFile.c_str(), output.data(), width, height);

	clock_t end = clock();
	double runtime = (double) (end - start) / CLOCKS_PER_SEC;
	std::cout << "total runtime: " << runtime << " s" << std::endl;
	return 0;

}