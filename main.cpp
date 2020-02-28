#include "defs.h"
#include "Scene.h"
#include <time.h>
#include "Camera.h"
Scene *pScene; // definition of the global scene variable (declared in defs.h)

int main(int argc, char *argv[])
{
	

	/* Do your stuff here */
	const char *xmlPath = "hw2/cornellbox_recursive.xml" ;
    cout<< xmlPath<<endl;
	clock_t tStart = clock();
	for (int i = 0; i < 1; i++) 
	{

		pScene = new Scene(xmlPath);

		pScene->renderScene();
		pScene->convertPPMToPNG(pScene->cameras[0]->imageName, 2);
	}
	float time = (double)(clock() - tStart) / CLOCKS_PER_SEC;
	printf("Time taken: %.5fs\n",time);
	return 0;
}
