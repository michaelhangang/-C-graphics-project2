#include "Light.h"
#include <sstream>
Light::Light() {
	position = vec3(0.0, 0.0, 0.0);
	diffuse = vec3(0.8f, 0.8f, 0.8f);
	specular = 0.5f;
	atten = vec3(1.0f, 0.09f, 0.0032f);
	isOn = 0.0f;

	lightPosLoc = -1;
	lightSpeLoc = -1;
	lightAttenLoc = -1;
	lightDiffLoc = -1;
	lightIsONLoc = -1;;

	return;
}
void Light::turnOff() {
	isOn = 0.0f;
}

void Light::turnOn() {
	isOn = 1;
}

SpotLight::SpotLight() {
	position = vec3(0.0, 0.0, 0.0);
	diffuse = vec3(1.f, 1.f, 1.f);
	specular = 1.f;
	atten = vec3(1.0f, 0.09f, 0.0032f);
	isOn = 1.0f;

	direction = vec3(0.0, 0.0, 0.0);
	cutOff = cos(radians(12.5f));
	outerCutOff = cos(radians(17.5f));

	SpotlightPosLoc = -1;
	SpotlightSpeLoc = -1;
	SpotlightAttenLoc = -1;
	SpotlightDiffLoc = -1;
	SpotlightIsONLoc = -1;;
	SpotlightDirLoc = -1;
	SpotlightcutOffLoc = -1;
	SpotlightouterCutOffLoc = -1;
	return;
}
void SpotLight::turnOff() {
	isOn = 0.0f;
}

void SpotLight::turnOn() {
	isOn = 1;
}
LightManager::LightManager() {


}

void LightManager::GetPointLightsUniformLocations(int shaderID) {
	for (size_t i = 0; i < Lights.size(); i++) {
		ostringstream iss;
		iss << "theLights[" << i << "].position";
		Lights[i].lightPosLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		iss.str("");
		iss << "theLights[" << i << "].specular";
		Lights[i].lightSpeLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		iss.str("");
		iss << "theLights[" << i << "].atten";
		Lights[i].lightAttenLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		iss.str("");
		iss << "theLights[" << i << "].diffuse";
		Lights[i].lightDiffLoc = glGetUniformLocation(shaderID, iss.str().c_str());

		iss.str("");
		iss << "theLights[" << i << "].isOn";

		Lights[i].lightIsONLoc = glGetUniformLocation(shaderID, iss.str().c_str());

	}
}

void LightManager::CopyPointLightsValuesToShader(void) {
	for (size_t i = 0; i < Lights.size(); i++) {

		glUniform3f(Lights[i].lightPosLoc, Lights[i].position.x, Lights[i].position.y, Lights[i].position.z);

		glUniform3f(Lights[i].lightAttenLoc, Lights[i].atten.x, Lights[i].atten.y, Lights[i].atten.z);

		glUniform1f(Lights[i].lightSpeLoc, Lights[i].specular);

		glUniform1f(Lights[i].lightIsONLoc, Lights[i].isOn);

		glUniform3f(Lights[i].lightDiffLoc, Lights[i].diffuse.x, Lights[i].diffuse.y, Lights[i].diffuse.z);
	}
}


void LightManager::GetSpotLightsUniformLocations(int shaderID) {
	for (size_t i = 0; i < SpotLights.size(); i++) {
		ostringstream iss;
		iss << "theSpotLights[" << i << "].position";
		SpotLights[i].SpotlightPosLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		iss.str("");
		iss << "theSpotLights[" << i << "].specular";
		SpotLights[i].SpotlightSpeLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		iss.str("");
		iss << "theSpotLights[" << i << "].atten";
		SpotLights[i].SpotlightAttenLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		iss.str("");
		iss << "theSpotLights[" << i << "].diffuse";
		SpotLights[i].SpotlightDiffLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		iss.str("");
		iss << "theSpotLights[" << i << "].direction";
		SpotLights[i].SpotlightDirLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		iss.str("");
		iss << "theSpotLights[" << i << "].cutOff";
		SpotLights[i].SpotlightouterCutOffLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		iss.str("");
		iss << "theSpotLights[" << i << "].outerCutOff";
		SpotLights[i].SpotlightouterCutOffLoc = glGetUniformLocation(shaderID, iss.str().c_str());
		iss.str("");

		iss << "theSpotLights[" << i << "].isOn";

		SpotLights[i].SpotlightIsONLoc = glGetUniformLocation(shaderID, iss.str().c_str());

	}
}
void LightManager::CopySpotLightsValuesToShader(void) {
	for (size_t i = 0; i < SpotLights.size(); i++) {

		glUniform3f(SpotLights[i].SpotlightPosLoc, SpotLights[i].position.x, SpotLights[i].position.y, SpotLights[i].position.z);

		glUniform3f(SpotLights[i].SpotlightAttenLoc, SpotLights[i].atten.x, SpotLights[i].atten.y, SpotLights[i].atten.z);

		glUniform1f(SpotLights[i].SpotlightSpeLoc, SpotLights[i].specular);

		glUniform1f(SpotLights[i].SpotlightIsONLoc, SpotLights[i].isOn);

		glUniform3f(SpotLights[i].SpotlightDiffLoc, SpotLights[i].diffuse.x, SpotLights[i].diffuse.y, SpotLights[i].diffuse.z);

		glUniform3f(SpotLights[i].SpotlightDirLoc, SpotLights[i].direction.x, SpotLights[i].direction.y, SpotLights[i].direction.z);

		glUniform1f(SpotLights[i].SpotlightcutOffLoc, SpotLights[i].cutOff);

		glUniform1f(SpotLights[i].SpotlightouterCutOffLoc, SpotLights[i].outerCutOff);

	}
}