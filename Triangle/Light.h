#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

using namespace glm;
using namespace std;
class Light {
public:
	vec3 position;
	vec3 diffuse;
	float specular;
	vec3 atten;
	float isOn;
	unsigned int lightPosLoc;
	unsigned int lightSpeLoc;
	unsigned int lightAttenLoc;
	unsigned int lightDiffLoc;
	unsigned int lightIsONLoc;

	Light();
	void turnOff();
	void turnOn();
};

class SpotLight {
public:
	vec3 position;
	vec3 diffuse;
	float specular;
	vec3 atten;
	float isOn;

	vec3 direction;
	float cutOff;
	float outerCutOff;

	unsigned int SpotlightPosLoc;
	unsigned int SpotlightSpeLoc;
	unsigned int SpotlightAttenLoc;
	unsigned int SpotlightDiffLoc;
	unsigned int SpotlightIsONLoc;

	unsigned int SpotlightDirLoc;
	unsigned int SpotlightcutOffLoc;
	unsigned int SpotlightouterCutOffLoc;

	SpotLight();
	void turnOff();
	void turnOn();
};

class LightManager
{
public:
	//static const unsigned int NUMBEROFLIGHTS = 5;

	vector<Light> Lights;
	vector<SpotLight> SpotLights;
	LightManager();

	void GetPointLightsUniformLocations(int shaderID);
	void CopyPointLightsValuesToShader(void);

	void GetSpotLightsUniformLocations(int shaderID);
	void CopySpotLightsValuesToShader(void);

};