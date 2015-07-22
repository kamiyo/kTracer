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
		Random* rng = new Random();
		/*
		double accumulate = 0;
		for (int i = 0; i < 1000; i++) {
			accumulate += rng->discrete(0, 16);
		}
		std::cout << accumulate / 1000 << std::endl;*/
		
		Sampler* s = new PermutedHaltonSampler(rng);
		Sampler2d i; i.resize(4, 4); s->genPoints(i);
		std::cout << i << std::endl;
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