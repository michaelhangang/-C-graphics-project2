#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp> 

#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include"Shader.h"
#include<fstream>
#include"VAOManager.h"
#include"Mesh.h"
#include"Light.h"
using namespace glm;


#pragma region  methods

//auto adjust window when window is changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//input control
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool isShiftDownAlone(int mods);
bool isCtrlDownAlone(int mods);
void SaveEverything(void);
bool isAltDownAlone(int mods);
void drawMesh(Mesh mesh,int shaderId);
void drawLightSphere(Mesh lightSphere, int shaderId);
void drawSpotLightSphere(Mesh lightSphere, int shaderId);
#pragma endregion

#pragma region global variable
//global variable
glm::vec3 cameraPos = glm::vec3(-13, -18.7998, 73.4994);
glm::vec3 cameraPos2 = glm::vec3(-15.8, 54.2003, 96.299);
glm::vec3 cameraPos3 = glm::vec3(-9.8, -24.9998, 26.2994);
glm::vec3 cameraTarget = glm::vec3(-8.8, -2.4, -3.1);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

vector<Mesh> MeshToDraw;
VAOManager VaoManager;
LightManager lights;
int indexSelectedMesh = 0;
int indexSelectedLight = 0;
int indexSelectedSpotLight = 0;
int view = 0;
//screen 
int screenWidth = 1680;
int creenHigh = 900;
#pragma endregion global variable

int main(void)
{
#pragma region initialize and configure
	//initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a window
	GLFWwindow* window = glfwCreateWindow(screenWidth, creenHigh, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Initialize  glad 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSwapInterval(1);
	//create shaders
	Shader vertexShader(Shader::shaderType::VERTEX,"vertex.glsl");
	vertexShader.createShader();
	
	Shader fragmentShader(Shader::shaderType::FRAGMENT, "fragment.glsl");
	fragmentShader.createShader();

	//create shader program
	int shaderProgram = glCreateProgram();
	//link 
	glAttachShader(shaderProgram, vertexShader.Id);
	glAttachShader(shaderProgram, fragmentShader.Id);
	glLinkProgram(shaderProgram);
	
	//delete shader object
	glDeleteShader(vertexShader.Id);
	glDeleteShader(fragmentShader.Id);
#pragma endregion 

#pragma region  Get location of view, projection  camera 

	// retrieve the matrix uniform locations
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	unsigned int cameraPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
#pragma endregion 

#pragma region model
	//load model 1
	Model arena("arena", "free_arena_ASCII_xyz.ply");
	VaoManager.loadModelToVAO(arena);
	Model dragon("dragon","xyzrgb_dragon_simple.ply");
	VaoManager.loadModelToVAO(dragon);
	Model flydragon("flydragon2", "Dragon 2.5_ply.ply");
	VaoManager.loadModelToVAO(flydragon);
	//load model 2
	Model warrior("warrior","BM_3d_Low.ply");
	VaoManager.loadModelToVAO(warrior);
	
	//load model 3
	Model sphere("sphere", "Sphere.ply");
	VaoManager.loadModelToVAO(sphere);
	//load model 4
	Model Thenatarius("Thenatarius", "Thenatarius.ply");
	VaoManager.loadModelToVAO(Thenatarius);

	//load model 4
	Model owlmonk("owlmonk", "owl-monk-fixed-mesh.ply");
	VaoManager.loadModelToVAO(owlmonk);
#pragma endregion  

#pragma region mesh
	//ligthphere
	Mesh lightSphere("light", sphere);
	lightSphere.pos = glm::vec3(-7.99999f, 0.f, 0);
	lightSphere.scale = 0.2f;
	lightSphere.isWireframe = true;
	//create arena
	Mesh arenaMesh("arena",arena);
	arenaMesh.pos = vec3(-3.5, -7.7, 0);
	arenaMesh.scale = 3.8f;
	arenaMesh.orientation = vec3(1.03, 0.02, -0.02);
	arenaMesh.colour = vec3(0.390001, 0.73, 1);
	MeshToDraw.push_back(arenaMesh);
	//create dragon
	Mesh dragon1("dragon1",dragon);
	dragon1.pos = glm::vec3(-28.8, -2.4, -3.1);
	dragon1.scale = 11.73f;
	dragon1.orientation = glm::vec3(0.99, 0.56, 0.69);
	dragon1.colour = vec3(1.0f, 0.5f, 0.31f);
	MeshToDraw.push_back(dragon1);
	
	Mesh dragon2("dragon2", flydragon);
	dragon2.pos = glm::vec3(-5.49996, 26.6, 4.7);
	dragon2.scale = 0.4f;
	dragon2.orientation = glm::vec3(0.0899999, 0.42, 1.32);
	dragon2.colour = vec3(0.0f, 1.f, 0.0f);
	MeshToDraw.push_back(dragon2);
	//
	Mesh warrior1("warrior1", warrior);
	warrior1.pos = glm::vec3(-1.80001, -1.6, 0.800001);
	warrior1.scale = 0.5f;
	warrior1.orientation = glm::vec3(0.969999, -0.43, -0.66);
	warrior1.colour = vec3(0.290001, 0.410001, 1);
	MeshToDraw.push_back(warrior1);
	//
	Mesh warrior2("warrior2", warrior);
	warrior2.pos = glm::vec3(16, -4.8, -1.6);
	warrior2.scale = 0.42f;
	warrior2.orientation = glm::vec3(1.27, -0.66, -1.05);
	warrior2.colour = vec3(0.290001, 0.410001, 1);
	MeshToDraw.push_back(warrior2);
	//
	Mesh warrior3("warrior3", warrior);
	warrior3.pos = glm::vec3(-18.4, -6.8, -16.8);
	warrior3.scale = 0.52f;
	warrior3.orientation = glm::vec3(-18.4, -6.8, -16.8);
	warrior3.colour = vec3(0.290001, 0.410001, 1);
	MeshToDraw.push_back(warrior3);
	//
	Mesh warrior4("warrior4", warrior);
	warrior4.pos = glm::vec3(-14, -5.4, -22.4);
	warrior4.scale = 0.5f;
	warrior4.orientation = glm::vec3(1.59, -0.45, -1.48);
	warrior4.colour = vec3(0.290001, 0.410001, 1);
	MeshToDraw.push_back(warrior4);
	//
	Mesh warrior5("warrior5", warrior);
	warrior5.pos = glm::vec3(17.7, 3, -25.7001);
	warrior5.scale = 0.38f;
	warrior5.orientation = glm::vec3(1.66, -0.33, -1.91);
	warrior5.colour = vec3(0.290001, 0.410001, 1);
	MeshToDraw.push_back(warrior5);
	//
	Mesh warrior6("warrior6", warrior);
	warrior6.pos = glm::vec3(-0.60003, -1.6, -29.5);
	warrior6.scale = 0.44f;
	warrior6.orientation = glm::vec3(1.66, -0.43, -1.84);
	warrior6.colour = vec3(0.290001, 0.410001, 1);
	MeshToDraw.push_back(warrior6);
	
	//
	Mesh Thenatarius1("Thenatarius1", Thenatarius);
	Thenatarius1.pos = glm::vec3(12.2, -2.89999, 5.19998);
	Thenatarius1.scale =(float)0.12;
	Thenatarius1.orientation = glm::vec3(0.97, 0.06, 0.12);
	Thenatarius1.colour = vec3(0.180001, 0.55, 1);
	MeshToDraw.push_back(Thenatarius1);
	//
	Mesh Thenatarius2("Thenatarius2", Thenatarius);
	Thenatarius2.pos = glm::vec3(15.8, -4.79999, 16.1);
	Thenatarius2.scale = (float)0.12;
	Thenatarius2.orientation = glm::vec3(0.95, 0.01, 0.12);
	Thenatarius2.colour = vec3(0.180001, 0.55, 1);
	MeshToDraw.push_back(Thenatarius2);
	//
	Mesh Thenatarius3("Thenatarius3", Thenatarius);
	Thenatarius3.pos = glm::vec3(-2.9, -2.99998, 9.49996);
	Thenatarius3.scale = (float)0.14;
	Thenatarius3.orientation = glm::vec3(0.95, 0.06, 0.12);
	Thenatarius3.colour = vec3(0.180001, 0.55, 1);
	MeshToDraw.push_back(Thenatarius3);
	//
	Mesh Thenatarius4("Thenatarius4", Thenatarius);
	Thenatarius4.pos = glm::vec3(19.6, -4.89998, 8.99998);
	Thenatarius4.scale = (float)0.12;
	Thenatarius4.orientation = glm::vec3(0.93, 7.45058e-09, 0.12);
	Thenatarius4.colour = vec3(0.180001, 0.55, 1);
	MeshToDraw.push_back(Thenatarius4);
	//
	Mesh Thenatarius5("Thenatarius5", Thenatarius);
	Thenatarius5.pos = glm::vec3(6.9, -2.09999, -6.80004);
	Thenatarius5.scale = (float)0.16;
	Thenatarius5.orientation = glm::vec3(0.98, 0.08, 0.18);
	Thenatarius5.colour = vec3(0.180001, 0.55, 1);
	MeshToDraw.push_back(Thenatarius5);
	//
	Mesh Thenatarius6("Thenatarius6", Thenatarius);
	Thenatarius6.pos = glm::vec3(2.7, -13.4, 20.6);
	Thenatarius6.scale = (float)0.12;
	Thenatarius6.orientation = glm::vec3(0.55, 0.06, 0.12);
	Thenatarius6.colour = vec3(0.180001, 0.55, 1);
	MeshToDraw.push_back(Thenatarius6);

	
	//
	Mesh owlmonk1("owlmonk1", owlmonk);
	owlmonk1.pos = glm::vec3(21.3, -5.7, 20.4);
	owlmonk1.scale = (float)0.0999999;
	owlmonk1.orientation = glm::vec3(0.85, -0.16, -0.469999);
	owlmonk1.colour = vec3(-0.259999, 1, 1);
	MeshToDraw.push_back(owlmonk1);
	//
	Mesh owlmonk2("owlmonk2", owlmonk);
	owlmonk2.pos = glm::vec3(9.9, 2.3, -23.0001);
	owlmonk2.scale = (float)0.16;
	owlmonk2.orientation = glm::vec3(1.01, 0.51, 0.68);
	owlmonk2.colour = vec3(-0.259999, 1, 1);
	MeshToDraw.push_back(owlmonk2);
	//
	Mesh owlmonk3("owlmonk3", owlmonk);
	owlmonk3.pos = glm::vec3(12.3, -3.2, -10.9);
	owlmonk3.scale = (float)0.14;
	owlmonk3.orientation = glm::vec3(1.14, 0.0200002, -0.0599996);
	owlmonk3.colour = vec3(-0.259999, 1, 1);
	MeshToDraw.push_back(owlmonk3);
	//
	Mesh owlmonk4("owlmonk4", owlmonk);
	owlmonk4.pos = glm::vec3(19.8, -1.9, -2.00001);
	owlmonk4.scale = (float)0.12;
	owlmonk4.orientation = glm::vec3(0.98, -0.0499998, -0.19);
	owlmonk4.colour = vec3(-0.259999, 1, 1);
	MeshToDraw.push_back(owlmonk4);
	//
	Mesh owlmonk5("owlmonk5", owlmonk);
	owlmonk5.pos = glm::vec3(22.6, -5.9, 16.5);
	owlmonk5.scale = (float)0.0999999;
	owlmonk5.orientation = glm::vec3(0.9, -0.23, -0.37);
	owlmonk5.colour = vec3(-0.259999, 1, 1);
	MeshToDraw.push_back(owlmonk5);
	//
	Mesh owlmonk6("owlmonk6", owlmonk);
	owlmonk6.pos = glm::vec3(5.99999, -2, 2.69999);
	owlmonk6.scale = (float)0.12;
	owlmonk6.orientation = glm::vec3(1.01, 0.0100002, -0.18);
	owlmonk6.colour = vec3(-0.259999, 1, 1);
	MeshToDraw.push_back(owlmonk6);
#pragma endregion 

#pragma region point light
	//light 
	Light lightone;
	lightone.position = vec3(1.00004, 22.8, -12.3);
	lightone.atten = vec3(1, 0.083, 2.22219e-19);
	lightone.diffuse = vec3(1, -0.0999993, 0.68);
	lights.Lights.push_back(lightone);

	//light2
	Light light2;
	light2.position = vec3(33.5001, -6.50009, 3.50001);
	light2.atten = vec3(1, 0.039, 3.12959e-30);
	light2.diffuse = vec3(1, 1, 1);
	lights.Lights.push_back(light2);
	//
	Light light3;
	light3.position = vec3(10, -22.4001, 13.6);
	light3.atten = vec3(1, 0.005, 0.0568953);
	light3.diffuse = vec3(-0.629999, 1.97, 1);
	lights.Lights.push_back(light3);
	//
	Light light4;
	light4.position = vec3(-28.3, -17.5, 18.5);
	light4.atten = vec3(1, 0.0320009, 4.22104e-12);
	light4.diffuse = vec3(3.33, 0.5, 0.31);
	lights.Lights.push_back(light4);
	//
	Light light5;
	light5.position = vec3(-15.1999, 2, 25.1001);
	light5.atten = vec3(1, 0.015, 0.00501816);
	light5.diffuse = vec3(1, 1.82, 1);
	lights.Lights.push_back(light5);
	lights.GetPointLightsUniformLocations(shaderProgram);
#pragma endregion 

#pragma region spot light
	//Spot
	SpotLight spotLight1;
	spotLight1.position = vec3(-15.1999, 2, 25.1001);
	//spotLight1.turnOff();
	spotLight1.atten = vec3(1, 0.015, 0.00501816);
	spotLight1.direction = vec3(-28.8, -2.4, -3.1);
	lights.SpotLights.push_back(spotLight1);
	lights.GetSpotLightsUniformLocations(shaderProgram);
#pragma endregion

//draw graphics
	while (!glfwWindowShouldClose(window))
	{
    #pragma	region basic setting

		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);

		//clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		
		//activate shader program
		glUseProgram(shaderProgram);
	
		//perspective 
		glm::mat4 projection =
			glm::perspective(glm::radians(45.0f), ratio, 0.1f, 1000.0f);

		//camera
		glm::mat4 view = glm::mat4(1.0f);	
		view = glm::lookAt(cameraPos,
			cameraTarget,
			cameraUp);

		//set up view and projection value in shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

#pragma endregion 	

		//draw every mesh
		for (vector<Mesh>::iterator iter = MeshToDraw.begin(); iter != MeshToDraw.end();iter++) {
			Mesh mesh = *iter;
			drawMesh(mesh,shaderProgram);
		}

		//draw light sphere
		drawLightSphere(lightSphere,shaderProgram);
		drawSpotLightSphere(lightSphere, shaderProgram);
		
		//draw color
		lights.CopyPointLightsValuesToShader();
		lights.CopySpotLightsValuesToShader();

#pragma region swap butters
		//swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
#pragma endregion 

    #pragma region stop 
	glfwDestroyWindow(window);
	glfwTerminate();
	std::exit(EXIT_SUCCESS);
#pragma endregion 
}



#pragma region input setting 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	const float cameraSpeed = 0.2f;
	//change mesh
	if (isShiftDownAlone(mods)) {
		//change postion
		
		if (key == GLFW_KEY_A) {
			MeshToDraw[indexSelectedMesh].pos.x += 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.x << endl;
		}
		if (key == GLFW_KEY_D) {
			MeshToDraw[indexSelectedMesh].pos.x -= 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.x << endl;
		}
		if (key == GLFW_KEY_W) {
			MeshToDraw[indexSelectedMesh].pos.z += 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.x << endl;
		}
		if (key == GLFW_KEY_S) {
			MeshToDraw[indexSelectedMesh].pos.z -= 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.x << endl;
		}

		if (key == GLFW_KEY_Q) {
			MeshToDraw[indexSelectedMesh].pos.y += 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.x << endl;
		}
		if (key == GLFW_KEY_E) {
			MeshToDraw[indexSelectedMesh].pos.y -= 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.x << endl;
		}

		//change scale
		if (key == GLFW_KEY_Z ) {
			MeshToDraw[indexSelectedMesh].scale += 0.01f;
			cout << MeshToDraw[indexSelectedMesh].scale << endl;
		}
		if (key == GLFW_KEY_X ) {
			MeshToDraw[indexSelectedMesh].scale -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].scale << endl;
		}

		//change orientation
		if (key == GLFW_KEY_T ) {
			MeshToDraw[indexSelectedMesh].orientation.x +=0.01f ;
			cout << MeshToDraw[indexSelectedMesh].orientation.x <<" "<< endl;
		}
		if (key == GLFW_KEY_G) {
			MeshToDraw[indexSelectedMesh].orientation.x -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].orientation.x << " " << endl;
		}
		if (key == GLFW_KEY_Y ) {
			MeshToDraw[indexSelectedMesh].orientation.y += 0.01f;
			cout << MeshToDraw[indexSelectedMesh].orientation.y << " "  << endl;
		}
		if (key == GLFW_KEY_H) {
			MeshToDraw[indexSelectedMesh].orientation.y -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].orientation.y << " " << endl;
		}
		if (key == GLFW_KEY_U ) {
			MeshToDraw[indexSelectedMesh].orientation.z += 0.01f;
			cout << MeshToDraw[indexSelectedMesh].orientation.z << " "  << endl;
		}
		if (key == GLFW_KEY_J) {
			MeshToDraw[indexSelectedMesh].orientation.z -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].orientation.z << " " << endl;
		}
		//change color
		if (key == GLFW_KEY_1) {
			MeshToDraw[indexSelectedMesh].colour.x +=0.01f ;
			cout << MeshToDraw[indexSelectedMesh].colour.x << endl;
		}
		if (key == GLFW_KEY_2) {
			MeshToDraw[indexSelectedMesh].colour.x -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].colour.x << endl;
		}
		if (key == GLFW_KEY_3) {
			MeshToDraw[indexSelectedMesh].colour.y += 0.01f;
			cout << MeshToDraw[indexSelectedMesh].colour.y << endl;
		}
		if (key == GLFW_KEY_4) {
			MeshToDraw[indexSelectedMesh].colour.y -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].colour.y << endl;
		}

		if (key == GLFW_KEY_5) {
			MeshToDraw[indexSelectedMesh].colour.y += 0.01f;
			cout << MeshToDraw[indexSelectedMesh].colour.y << endl;
		}
		if (key == GLFW_KEY_6) {
			MeshToDraw[indexSelectedMesh].colour.y -= 0.01f;
			cout << MeshToDraw[indexSelectedMesh].colour.y << endl;
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			indexSelectedMesh++;
			if (indexSelectedMesh == MeshToDraw.size())
				indexSelectedMesh = 0;
			cout << indexSelectedMesh << endl;
		}

		if (key == GLFW_KEY_O && action == GLFW_PRESS) {
			indexSelectedMesh--;
			if (indexSelectedMesh <0)
				indexSelectedMesh = 0;
			cout << indexSelectedMesh << endl;
		}
		if (key == GLFW_KEY_L && action == GLFW_PRESS)
			MeshToDraw[indexSelectedMesh].isWireframe = true;
		if (key == GLFW_KEY_K && action == GLFW_PRESS)
			MeshToDraw[indexSelectedMesh].isWireframe = false;
		return;
	}

    #pragma region point lights
	//adjust lights
	if (isAltDownAlone(mods)) {
		//change postion
		if (key == GLFW_KEY_A) {
			lights.Lights[indexSelectedLight].position.x -= 0.1f;
			cout << lights.Lights[indexSelectedLight].position.x << endl;
		}
		if (key == GLFW_KEY_D) {
			lights.Lights[indexSelectedLight].position.x += 0.1f;
			cout << lights.Lights[indexSelectedLight].position.x << endl;
		}
		if (key == GLFW_KEY_W) {
			lights.Lights[indexSelectedLight].position.z += 0.1f;
			cout << lights.Lights[indexSelectedLight].position.z << endl;
		}
		if (key == GLFW_KEY_S) {
			lights.Lights[indexSelectedLight].position.z -= 0.1f;
			cout << lights.Lights[indexSelectedLight].position.z << endl;
		}

		if (key == GLFW_KEY_Q) {
			lights.Lights[indexSelectedLight].position.y += 0.1f;
			cout << lights.Lights[indexSelectedLight].position.y << endl;
		}
		if (key == GLFW_KEY_E) {
			lights.Lights[indexSelectedLight].position.y -= 0.1f;
			cout << lights.Lights[indexSelectedLight].position.y << endl;
		}

		//change atten
		if (key == GLFW_KEY_1 == GLFW_PRESS) {
			lights.Lights[indexSelectedLight].atten.y -= 0.001f;
			cout << lights.Lights[indexSelectedLight].atten.y << endl;
		}
		if (key == GLFW_KEY_2 == GLFW_PRESS) {
			lights.Lights[indexSelectedLight].atten.y += 0.001f;
			cout << lights.Lights[indexSelectedLight].atten.y << endl;
		}

		if (key == GLFW_KEY_3 == GLFW_PRESS) {
			lights.Lights[indexSelectedLight].atten.z *= 0.9f;
			cout << lights.Lights[indexSelectedLight].atten.z << endl;
		}
		if (key == GLFW_KEY_4 == GLFW_PRESS) {
			lights.Lights[indexSelectedLight].atten.z *= 1.1f;
			cout << lights.Lights[indexSelectedLight].atten.z << endl;
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			indexSelectedLight++;
			if (indexSelectedLight == lights.Lights.size())
				indexSelectedLight = 0;
			cout << indexSelectedLight << endl;
		}

		if (key == GLFW_KEY_O && action == GLFW_PRESS) {
			indexSelectedLight--;
			if (indexSelectedLight <0)
				indexSelectedLight = 0;
			cout << indexSelectedLight << endl;
		}
		//light is on or off
		if (key == GLFW_KEY_L && action == GLFW_PRESS)
			lights.Lights[indexSelectedLight].turnOff();
		if (key == GLFW_KEY_K && action == GLFW_PRESS)
			lights.Lights[indexSelectedLight].turnOn();

		//change color
		if (key == GLFW_KEY_5) {
			lights.Lights[indexSelectedLight].diffuse.x += 0.01f;
			cout << lights.Lights[indexSelectedLight].diffuse.x << endl;
		}
		if (key == GLFW_KEY_6) {
			lights.Lights[indexSelectedLight].diffuse.x -= 0.01f;
			cout << lights.Lights[indexSelectedLight].diffuse.x << endl;
		}
		if (key == GLFW_KEY_7) {
			lights.Lights[indexSelectedLight].diffuse.y += 0.01f;
			cout << lights.Lights[indexSelectedLight].diffuse.y << endl;
		}
		if (key == GLFW_KEY_8) {
			lights.Lights[indexSelectedLight].diffuse.y -= 0.01f;
			cout << lights.Lights[indexSelectedLight].diffuse.y << endl;
		}
		if (key == GLFW_KEY_9) {
			lights.Lights[indexSelectedLight].diffuse.z += 0.01f;
			cout << lights.Lights[indexSelectedLight].diffuse.z << endl;
		}
		if (key == GLFW_KEY_0) {
			lights.Lights[indexSelectedLight].diffuse.z -= 0.01f;
			cout << lights.Lights[indexSelectedLight].diffuse.z << endl;
		}
		return;
	}
    #pragma endregion point lights

    #pragma region spot lights
	if( isCtrlDownAlone( mods)) {
		//change postion
		if (key == GLFW_KEY_A) {
			lights.SpotLights[indexSelectedSpotLight].position.x -= 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].position.x << endl;
		}
		if (key == GLFW_KEY_D) {
			lights.SpotLights[indexSelectedSpotLight].position.x += 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].position.x << endl;
		}
		if (key == GLFW_KEY_W) {
			lights.SpotLights[indexSelectedSpotLight].position.z += 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].position.z << endl;
		}
		if (key == GLFW_KEY_S) {
			lights.SpotLights[indexSelectedSpotLight].position.z -= 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].position.z << endl;
		}

		if (key == GLFW_KEY_Q) {
			lights.SpotLights[indexSelectedSpotLight].position.y += 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].position.y << endl;
		}
		if (key == GLFW_KEY_E) {
			lights.SpotLights[indexSelectedSpotLight].position.y -= 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].position.y << endl;
		}

		//change atten
		if (key == GLFW_KEY_1 == GLFW_PRESS) {
			lights.SpotLights[indexSelectedSpotLight].atten.y -= 0.001f;
			cout << lights.SpotLights[indexSelectedSpotLight].atten.y << endl;
		}
		if (key == GLFW_KEY_2 == GLFW_PRESS) {
			lights.SpotLights[indexSelectedSpotLight].atten.y += 0.001f;
			cout << lights.SpotLights[indexSelectedSpotLight].atten.y << endl;
		}

		if (key == GLFW_KEY_3 == GLFW_PRESS) {
			lights.SpotLights[indexSelectedSpotLight].atten.z *= 0.9f;
			cout << lights.SpotLights[indexSelectedSpotLight].atten.z << endl;
		}
		if (key == GLFW_KEY_4 == GLFW_PRESS) {
			lights.SpotLights[indexSelectedSpotLight].atten.z *= 1.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].atten.z << endl;
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			indexSelectedSpotLight++;
			if (indexSelectedSpotLight == lights.SpotLights.size())
				indexSelectedSpotLight = 0;
			cout << indexSelectedSpotLight << endl;
		}

		if (key == GLFW_KEY_O && action == GLFW_PRESS) {
			indexSelectedSpotLight--;
			if (indexSelectedSpotLight < 0)
				indexSelectedSpotLight = 0;
			cout << indexSelectedSpotLight << endl;
		}
		//light is on or off
		if (key == GLFW_KEY_L && action == GLFW_PRESS)
			lights.SpotLights[indexSelectedSpotLight].turnOff();
		if (key == GLFW_KEY_K && action == GLFW_PRESS)
			lights.SpotLights[indexSelectedSpotLight].turnOn();

		//change color
		if (key == GLFW_KEY_5) {
			lights.SpotLights[indexSelectedSpotLight].diffuse.x += 0.01f;
			cout << lights.SpotLights[indexSelectedSpotLight].diffuse.x << endl;
		}
		if (key == GLFW_KEY_6) {
			lights.SpotLights[indexSelectedSpotLight].diffuse.x -= 0.01f;
			cout << lights.SpotLights[indexSelectedSpotLight].diffuse.x << endl;
		}
		if (key == GLFW_KEY_7) {
			lights.SpotLights[indexSelectedSpotLight].diffuse.y += 0.01f;
			cout << lights.SpotLights[indexSelectedSpotLight].diffuse.y << endl;
		}
		if (key == GLFW_KEY_8) {
			lights.SpotLights[indexSelectedSpotLight].diffuse.y -= 0.01f;
			cout << lights.SpotLights[indexSelectedSpotLight].diffuse.y << endl;
		}
		if (key == GLFW_KEY_9) {
			lights.SpotLights[indexSelectedSpotLight].diffuse.z += 0.01f;
			cout << lights.SpotLights[indexSelectedSpotLight].diffuse.z << endl;
		}
		if (key == GLFW_KEY_0) {
			lights.SpotLights[indexSelectedSpotLight].diffuse.z -= 0.01f;
			cout << lights.SpotLights[indexSelectedSpotLight].diffuse.z << endl;
		}
		if (key == GLFW_KEY_Z) {
			lights.SpotLights[indexSelectedSpotLight].direction.x += 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].direction.x << endl;
		}
		if (key == GLFW_KEY_X) {
			lights.SpotLights[indexSelectedSpotLight].direction.x -= 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].direction.x << endl;
		}
		if (key == GLFW_KEY_C) {
			lights.SpotLights[indexSelectedSpotLight].direction.y += 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].direction.y << endl;
		}
		if (key == GLFW_KEY_V) {
			lights.SpotLights[indexSelectedSpotLight].direction.y -= 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].direction.y << endl;
		}
		if (key == GLFW_KEY_B) {
			lights.SpotLights[indexSelectedSpotLight].direction.z += 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].direction.z<< endl;
		}
		if (key == GLFW_KEY_N) {
			lights.SpotLights[indexSelectedSpotLight].direction.z -= 0.1f;
			cout << lights.SpotLights[indexSelectedSpotLight].direction.z << endl;
		}
		return;
	}
#pragma endregion  spot lights

#pragma region basic control
	//save 
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		SaveEverything();

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_W )
		cameraPos.z -= cameraSpeed;
	if (key == GLFW_KEY_S )
		cameraPos.z += cameraSpeed;
	if (key == GLFW_KEY_A)
		cameraPos.x -= cameraSpeed;
	if (key == GLFW_KEY_D )
		cameraPos.x += cameraSpeed;
	if (key == GLFW_KEY_Q )
		cameraPos.y += cameraSpeed;
	if (key == GLFW_KEY_E )
		cameraPos.y -= cameraSpeed;
	//change camera
	if (key == GLFW_KEY_C&&action== GLFW_PRESS) {
		view++;
		switch (view)
		{
		case 1:cameraPos = cameraPos2; break;
		case 2:cameraPos = cameraPos3; break;
		default:
			cameraPos = vec3(-13, -18.7998, 73.4994);
			view = 0;
			break;
		}
	}
#pragma endregion 
}
#pragma endregion 

#pragma region Save setting

void SaveEverything(void)
{
	std::ofstream saveFile("Save.txt");

	for (unsigned int index = 0;
		index != MeshToDraw.size(); index++)
	{
		saveFile
			<< MeshToDraw[index].name << "\n postion "
			<< MeshToDraw[index].pos.x << ", "
			<< MeshToDraw[index].pos.y << ", "
			<< MeshToDraw[index].pos.z << "\n\n orientation "
			<< MeshToDraw[index].orientation.x << ", "
			<< MeshToDraw[index].orientation.y << ","
			<< MeshToDraw[index].orientation.z << " \n\n scale "
			<< MeshToDraw[index].scale << "\n\n color "
			<< MeshToDraw[index].colour.x << ", "
			<< MeshToDraw[index].colour.y << ", "
			<< MeshToDraw[index].colour.z << "\n\n";

	}

	for (unsigned int index = 0;
		index < lights.Lights.size(); index++)
	{
		saveFile
			<< "Light # " << index + 1 << "\n postion "
			<< lights.Lights[index].position.x << ", "
			<< lights.Lights[index].position.y << ", "
			<< lights.Lights[index].position.z << "\n\n atten "
			<< lights.Lights[index].atten.x << ", "
			<< lights.Lights[index].atten.y << ","
			<< lights.Lights[index].atten.z << " \n\n color "
			<< lights.Lights[index].diffuse.x << ","
			<< lights.Lights[index].diffuse.y << ","
			<< lights.Lights[index].diffuse.z << " \n\n";
	}
	for (unsigned int index = 0;
		index < lights.SpotLights.size(); index++)
	{
		saveFile
			<< "SpotLight # " << index + 1 << "\n postion "
			<< lights.SpotLights[index].position.x << "  "
			<< lights.SpotLights[index].position.y << "  "
			<< lights.SpotLights[index].position.z << "\n\n atten "
			<< lights.SpotLights[index].atten.x << " "
			<< lights.SpotLights[index].atten.y << " "
			<< lights.SpotLights[index].atten.z << " \n\n color "
			<< lights.SpotLights[index].diffuse.x << ","
			<< lights.SpotLights[index].diffuse.y << ","
			<< lights.SpotLights[index].diffuse.z << " \n\n direction "
			<< lights.SpotLights[index].direction.x << " "
			<< lights.SpotLights[index].direction.y << " "
			<< lights.SpotLights[index].direction.z << " \n\n";
		
	}

	saveFile <<"Camera "<< cameraPos.x << "," << cameraPos.y << "," << cameraPos.z;

	saveFile.close();
	cout << "Saved...." << endl;
	return;
}
#pragma endregion 

#pragma region mod control method

bool isShiftDownAlone(int mods)
{
	if ((mods & GLFW_MOD_SHIFT)== GLFW_MOD_SHIFT)
	{
		return true;
	}
	return false;
}

bool isAltDownAlone(int mods)
{
	if ((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
	{
		return true;
	}
	return false;
}
bool isCtrlDownAlone(int mods) {
	if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
	{
		return true;
	}
	return false;

}
#pragma endregion 

#pragma region Draw meshes

void drawMesh(Mesh mesh,int shaderId) {
	//for (Mesh mesh : MeshToDraw) {
		// create transformations
		glm::mat4 model = glm::mat4(1.0f);

		glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
			mesh.orientation.z,
			glm::vec3(0.0f, 0.0, 1.0f));

		glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
			mesh.orientation.y,
			glm::vec3(0.0f, 1.0, 0.0f));

		glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
			mesh.orientation.x,
			glm::vec3(1.0f, 0.0, 0.0f));

		glm::mat4 move = glm::translate(glm::mat4(1.0f), mesh.pos);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(mesh.scale, mesh.scale, mesh.scale));
		model = rotateZ * rotateY * rotateX * move * scale;

		// pass them to the shaders 
		unsigned int modelLoc = glGetUniformLocation(shaderId, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//pass color to shaders
		unsigned int colorLoc = glGetUniformLocation(shaderId, "ourColor");
		glUniform3f(colorLoc, mesh.colour.x, mesh.colour.y, mesh.colour.z);

		// Is the object wireframe
		if (mesh.isWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		//get VAO Id 
		Model mod = VaoManager.dateVAO.find(mesh.modelType)->second;
		glBindVertexArray(mod.VAOId);
		glDrawElements(GL_TRIANGLES, mod.numberOfIndices, GL_UNSIGNED_INT, 0);
}

#pragma endregion

#pragma region Draw point light sphere

void drawLightSphere(Mesh mesh,int shaderId) {
	unsigned int sphereColourLoc = glGetUniformLocation(shaderId, "sphereColor");
	unsigned int  isLightLoc = glGetUniformLocation(shaderId, "isLightSphere");
	glUniform1f(isLightLoc, (float)GL_TRUE);
	glUniform3f(sphereColourLoc, 1.0f, 1.0f, 1.0f);
	mesh.pos = lights.Lights[indexSelectedLight].position;

	//draw lightphere 
	drawMesh(mesh, shaderId);
	glUniform1f(isLightLoc, (float)GL_FALSE);

}
#pragma endregion 

#pragma region Draw spot light sphere
void drawSpotLightSphere(Mesh lightSphere, int shaderId) {
	unsigned int sphereColourLoc = glGetUniformLocation(shaderId, "sphereColor");
	unsigned int  isLightLoc = glGetUniformLocation(shaderId, "isLightSphere");
	glUniform1f(isLightLoc, (float)GL_TRUE);
	glUniform3f(sphereColourLoc, 1.0f, 1.0f, 1.0f);
	lightSphere.pos = lights.SpotLights[indexSelectedSpotLight].position;

	//draw lightphere 
	drawMesh(lightSphere, shaderId);
	glUniform1f(isLightLoc, (float)GL_FALSE);

}
#pragma endregion 

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}