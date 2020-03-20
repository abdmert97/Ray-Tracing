#include "defs.h"
#include "Scene.h"
#include <time.h>
#include "Camera.h"
Scene *pScene; // definition of the global scene variable (declared in defs.h)

int main(int argc, char *argv[])
{
	

	/* Do your stuff here */
	const char *xmlPath ;
	xmlPath = "hw2/scienceTree.xml";

	clock_t tStart = clock();
	for (int i = 0; i < 1; i++) 
	{

		pScene = new Scene(xmlPath);
		//pScene->readPly("hw2/ply/dragon_remeshed.ply");
		float time = (double)(clock() - tStart) / CLOCKS_PER_SEC;
		printf("Time taken for construction: %.5fs\n", time);
		 tStart = clock();
		pScene->renderScene();
		 time = (double)(clock() - tStart) / CLOCKS_PER_SEC;
		printf("Time taken: %.5fs\n", time);
		pScene->convertPPMToPNG(pScene->cameras[0]->imageName, 2);
	}

	return 0;
}
