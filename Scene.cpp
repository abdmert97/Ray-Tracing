#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"
#include "tinyxml2.h"
#include "Image.h"
#include <thread> 
using namespace tinyxml2;
extern  int shadowcount;


void Scene::threading(Camera* camera, Image* image)
{
	std::thread td0(&Scene::renderImagePart, this, 0, camera, image);
	std::thread td1(&Scene::renderImagePart, this, 1, camera, image);
	std::thread td2(&Scene::renderImagePart, this, 2, camera, image);
	std::thread td3(&Scene::renderImagePart, this, 3, camera, image);
	std::thread td4(&Scene::renderImagePart, this, 4, camera, image);
	std::thread td5(&Scene::renderImagePart, this, 5, camera, image);
	std::thread td6(&Scene::renderImagePart, this, 6, camera, image);
	std::thread td7(&Scene::renderImagePart, this, 7, camera, image);
		
	td0.join();
	//cout << "part1 rendered" << endl;
	td1.join();
	//cout << "part2 rendered" << endl;
	td2.join();
	//cout << "part3 rendered" << endl;
	td3.join();
	//cout << "part4 rendered" << endl;
	td4.join();
	//cout << "part5 rendered" << endl;
	td5.join();
	//cout << "part6 rendered" << endl;
	td6.join();
	//cout << "part7 rendered" << endl;
	td7.join();
	//cout << "part8 rendered" << endl;
}

/*
 * Must render the scene from each camera's viewpoint and create an image.
 * You can use the methods of the Image class to save the image as a PPM file.
 */
void Scene::renderScene(void)
{
	for (int i = 0; i < cameraCount; i++)
	{
		Camera* camera = cameras[i];
		ImagePlane imagePlane = camera->imgPlane;
		Image* image = new Image(imagePlane.nx, imagePlane.ny);
	
		threading(camera, image);
		//renderImage(camera, image);
		image->saveImage(cameras[i]->imageName);
	}

}
void Scene::renderImage(Camera* camera, Image* image)
{
	ImagePlane imagePlane = camera->imgPlane;
	ReturnVal returnValue;
	Shape* shape;
	Color color = { 0,0,0 };
	int maxWidth = (imagePlane.nx);
	for (int w = 0; w < maxWidth; w++)
	{

		for (int h = 0; h < imagePlane.ny; h++)
		{

			Ray ray = camera->getPrimaryRay(w, h);

			// Selecting Closest object to the camera
			ReturnVal closestObjectReturnVal = rayIntersection->closestObject(ray);
			if (closestObjectReturnVal.objectID == -1) // ray hits nothing
			{
				// Set background Color 
				Color color2 = { (unsigned char)backgroundColor.r,(unsigned char)backgroundColor.g,(unsigned char)backgroundColor.b };
				image->setPixelValue(w, h, color2);
			}
			else // if ray hits an object
			{
				color = { 0,0,0 };
				// start Shading a object

				shape = objects[closestObjectReturnVal.objectID];
				Color color2 = shading->shading(maxRecursionDepth, shape, closestObjectReturnVal, ray);

				//  cout<<(int)color.red<< " "<<(int)color.grn<< " "<<(int)color.blu<< " "<<endl;
				image->setPixelValue(w, h, color2);
			}

		}
	}
}
void Scene::renderImagePart(int part, Camera* camera, Image* image)
{

		ImagePlane imagePlane = camera->imgPlane;
		ReturnVal returnValue;
		Shape* shape;
		Color color = { 0,0,0 };
		int maxWidth = (imagePlane.nx / 8) * (part + 1);
		for (int w = (imagePlane.nx / 8) * part; w < maxWidth; w++)
		{

			for (int h = 0; h < imagePlane.ny; h++)
			{

				Ray ray = camera->getPrimaryRay(w, h);
				
				// Selecting Closest object to the camera
				ReturnVal closestObjectReturnVal = rayIntersection->closestObject(ray);
				if (closestObjectReturnVal.objectID == -1) // ray hits nothing
				{
					// Set background Color 
					Color color2 = { (unsigned char)backgroundColor.r,(unsigned char)backgroundColor.g,(unsigned char)backgroundColor.b };
					image->setPixelValue(w, h, color2);
				}
				else // if ray hits an object
				{
					color = { 0,0,0 };
					// start Shading a object

					shape = objects[closestObjectReturnVal.objectID];
					Color color2 = shading->shading(maxRecursionDepth, shape, closestObjectReturnVal, ray);

					//  cout<<(int)color.red<< " "<<(int)color.grn<< " "<<(int)color.blu<< " "<<endl;
					image->setPixelValue(w, h, color2);
				}
			
			}
		}
}


// Parses XML file.

void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}

void Scene::readXML(const char* xmlPath)
{
	const char* str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement* pElement;

	maxRecursionDepth = 1;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode* pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if (pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%f %f %f", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if (pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if (pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement* pCamera = pElement->FirstChildElement("Camera");
	XMLElement* camElement;
	while (pCamera != nullptr)
	{
		int id;
		char imageName[64];
		Vector3f pos, gaze, up;
		ImagePlane imgPlane;

		eResult = pCamera->QueryIntAttribute("id", &id);
		const char* type = pCamera->Attribute("type");
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &pos.x, &pos.y, &pos.z);
		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &up.x, &up.y, &up.z);
		camElement = pCamera->FirstChildElement("NearPlane");
		str = camElement->GetText();
		sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
		camElement = pCamera->FirstChildElement("NearDistance");
		eResult = camElement->QueryFloatText(&imgPlane.distance);
		camElement = pCamera->FirstChildElement("ImageResolution");
		str = camElement->GetText();
		sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
		camElement = pCamera->FirstChildElement("ImageName");
		str = camElement->GetText();
		strcpy(imageName, str);

		cameras.push_back(new Camera(id, imageName, pos, gaze, up, imgPlane));

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement* pMaterial = pElement->FirstChildElement("Material");
	XMLElement* materialElement;
	while (pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;
	
		eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		const char* type = pMaterial->Attribute("type");
		if (type == nullptr)
			materials[curr]->materialType = Default;
		else if(type[0] == 'm')
			materials[curr]->materialType = Mirror;
		else if (type[0] == 'c')
			materials[curr]->materialType = Conductor;
		else if (type[0] == 'd')
			materials[curr]->materialType = Dialectic;
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef.r, &materials[curr]->ambientRef.g, &materials[curr]->ambientRef.b);
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef.r, &materials[curr]->diffuseRef.g, &materials[curr]->diffuseRef.b);
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef.r, &materials[curr]->specularRef.g, &materials[curr]->specularRef.b);
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if (materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef.r, &materials[curr]->mirrorRef.g, &materials[curr]->mirrorRef.b);
		}
		else
		{
			materials[curr]->mirrorRef.r = 0.0;
			materials[curr]->mirrorRef.g = 0.0;
			materials[curr]->mirrorRef.b = 0.0;
		}

		materialElement = pMaterial->FirstChildElement("AbsorptionCoefficient");
		if (materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->absorptionCoefficient.r, &materials[curr]->absorptionCoefficient.g, &materials[curr]->absorptionCoefficient.b);
		}
		else
		{
			materials[curr]->absorptionCoefficient.r = 0.0;
			materials[curr]->absorptionCoefficient.g = 0.0;
			materials[curr]->absorptionCoefficient.b = 0.0;
		}
		
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if (materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		materialElement = pMaterial->FirstChildElement("RefractionIndex");
		if (materialElement != nullptr)
			materialElement->QueryFloatText(&materials[curr]->refractionIndex);

		materialElement = pMaterial->FirstChildElement("AbsorptionIndex");
		if (materialElement != nullptr)
			materialElement->QueryFloatText(&materials[curr]->absorptionIndex);

		
		pMaterial = pMaterial->NextSiblingElement("Material");
	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	int cursor = 0;
	Vector3f tmpPoint;
	str = pElement->GetText();
	while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
		cursor++;
	while (str[cursor] != '\0')
	{
		for (int cnt = 0; cnt < 3; cnt++)
		{
			if (cnt == 0)
				tmpPoint.x = atof(str + cursor);
			else if (cnt == 1)
				tmpPoint.y = atof(str + cursor);
			else
				tmpPoint.z = atof(str + cursor);
			while (str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
				cursor++;
			while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
		}
		vertices.push_back(tmpPoint);
	}

	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");

	// Parse spheres
	XMLElement* pObject = pElement->FirstChildElement("Sphere");
	XMLElement* objElement;
	while (pObject != nullptr)
	{
		int id;
		int matIndex;
		int cIndex;
		float R;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		objects.push_back(new Sphere(id, matIndex, cIndex, R, &vertices,SphereType));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while (pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		objects.push_back(new Triangle(id, matIndex, p1Index, p2Index, p3Index, &vertices,TriangleShape));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	pObject = pElement->FirstChildElement("Mesh");
	while (pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		vector<Triangle> faces;
		vector<int>* meshIndices = new vector<int>;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");
		objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
		str = objElement->GetText();
		while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while (str[cursor] != '\0')
		{
			for (int cnt = 0; cnt < 3; cnt++)
			{
				if (cnt == 0)
					p1Index = atoi(str + cursor) + vertexOffset;
				else if (cnt == 1)
					p2Index = atoi(str + cursor) + vertexOffset;
				else
					p3Index = atoi(str + cursor) + vertexOffset;
				while (str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++;
				while (str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			faces.push_back(*(new Triangle(-1, matIndex, p1Index, p2Index, p3Index, &vertices,TriangleShape)));
			meshIndices->push_back(p1Index);
			meshIndices->push_back(p2Index);
			meshIndices->push_back(p3Index);
		}

		objects.push_back(new Mesh(id, matIndex, faces, meshIndices, &vertices,MeshType));

		pObject = pObject->NextSiblingElement("Mesh");
	}

	// Parse lights
	int id;
	Vector3f position;
	Vector3f intensity;
	pElement = pRoot->FirstChildElement("Lights");

	XMLElement* pLight = pElement->FirstChildElement("AmbientLight");
	XMLElement* lightElement;
	str = pLight->GetText();
	sscanf(str, "%f %f %f", &ambientLight.r, &ambientLight.g, &ambientLight.b);

	pLight = pElement->FirstChildElement("PointLight");
	while (pLight != nullptr)
	{
		eResult = pLight->QueryIntAttribute("id", &id);
		lightElement = pLight->FirstChildElement("Position");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
		lightElement = pLight->FirstChildElement("Intensity");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &intensity.r, &intensity.g, &intensity.b);

		lights.push_back(new PointLight(position, intensity));

		pLight = pLight->NextSiblingElement("PointLight");
	}
}

Scene::Scene(const char* xmlPath)
{
	readXML(xmlPath);
	initObjects();
}
void Scene::setScene()
{
	lightCount = lights.size();
	objectCount = objects.size();
	ambientLightList = new Color[materials.size()];
	for (int i = 0; i < materials.size(); i++)
	{
		Material mat = *materials[i];
		ambientLightList[i].red = (unsigned char)(mat.ambientRef.x * ambientLight.x);
		ambientLightList[i].grn = (unsigned char)(mat.ambientRef.y * ambientLight.y);
		ambientLightList[i].blu = (unsigned char)(mat.ambientRef.z * ambientLight.z);
	}
	cameraCount = cameras.size();
}
void Scene::initObjects()
{
	setScene();
	rayIntersection = new RayIntersection(objects, objectCount);
	reflection = new Reflection(shadowRayEps,objects,backgroundColor);
	shading = new Shading(shadowRayEps,materials,ambientLightList,lightCount,objectCount,lights,objects);
	shading->reflection = reflection;
	reflection->shading = shading;
	reflection->rayIntersection = rayIntersection;
	refraction = new Refraction(reflection, shading, objects, rayIntersection);
	shading->refraction = refraction;
	
	boundingVolume = new BoundingVolume(objects);
	
	rayIntersection->boundingVolume = boundingVolume;
}

