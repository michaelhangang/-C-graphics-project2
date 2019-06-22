#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <glm/gtc/type_ptr.hpp> 
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include"Shader.h"
#include<fstream>
#include"VAOManager.h"
#include"Mesh.h"
#include"Light.h"
#include "Texture.h"
#include "./BasicTextureManager/TextureManager.h"
using namespace glm;
#pragma region method
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

void sortMeshes();
#pragma endregion 

#pragma region global variable
glm::vec3 cameraPos = glm::vec3(10, 3.1, 124.3);   //question 1
//glm::vec3 cameraPos = glm::vec3(-37.4001, -49.1001, -115.5);  //question 2
glm::vec3 cameraPos3 = glm::vec3(-9.8, -24.9998, 26.2994);
glm::vec3 cameraTarget = glm::vec3(0, 1, 0);   //question 1
//glm::vec3 cameraTarget = glm::vec3(0, 1,-224);  //question 2
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

vector<Mesh> MeshToDraw;
VAOManager VaoManager;
TextureManager textureManager;
LightManager lights;
int indexSelectedMesh = 0;
int indexSelectedLight = 0;
int indexSelectedSpotLight = 0;
int view = 0;
//screen 
int screenWidth = 1680;
int creenHigh = 900;

#pragma endregion

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
	
	
#pragma region light mesh

	Model sphere("sphere", "Sphere.ply");
	VaoManager.loadModelToVAO(sphere);
	Mesh lightSphere("light", sphere);
	lightSphere.pos = glm::vec3(-7.99999f, 0.f, 0);
	lightSphere.scale = 0.2f;
	lightSphere.isWireframe = true;

#pragma endregion light mesh

#pragma region texture
	Texture rock_Textures ("rock_Textures","_original.jpg");
	textureManager.Create2DTexture(rock_Textures,true);
	Texture rock_Textures1("rock_Textures1", "depositphotos_15659203-Seamless-texture-surface-of-the.jpg");
	textureManager.Create2DTexture(rock_Textures1, true);
	Texture rock_Textures2("rock_Textures2", "Aerographe albedo.jpg");
	textureManager.Create2DTexture(rock_Textures2, true);
	Texture rock_Textures3("rock_Textures3", "Dsc.jpg");
	textureManager.Create2DTexture(rock_Textures3, true);
	Texture rock_Textures4("rock_Textures4","earth_moon_texture_pack_by_zephroth.jpg");
	textureManager.Create2DTexture(rock_Textures4, true);
	Texture rock_Textures5("rock_Textures5","Explosion.bmp");
	textureManager.Create2DTexture(rock_Textures5, true);
	Texture rock_Textures6("rock_Textures6","Dsc2.jpg");
	textureManager.Create2DTexture(rock_Textures6, true);

#pragma endregion 
#pragma region  load Model
	

	Model imposter_Mod("imposter","Flat_Quad_xyz_n_uv.ply");
	imposter_Mod.GenerateSphericalUVs = false;
	VaoManager.loadModelToVAO(imposter_Mod);

	Model Starboard_Panel_mod("Starboard_Panel", "Starboard_Panel_only_03492.ply");
	VaoManager.loadModelToVAO(Starboard_Panel_mod);
	
	Model Port_Panel_mod("Port_Panel", "Port_Panel_only_03479.ply");
	VaoManager.loadModelToVAO(Port_Panel_mod);

	Model Body_mod("Body", "Body_Only_07328_vertx.ply");
	VaoManager.loadModelToVAO(Body_mod);
	
	Model Asteroid_001("Asteroid_001", "Asteroid_001.ply");
	VaoManager.loadModelToVAO(Asteroid_001);
	
	//load model 2
	Model Asteroid_002("Asteroid_002","Asteroid_002.ply");
	VaoManager.loadModelToVAO(Asteroid_002);
	
	Model Asteroid_003("Asteroid_003", "Asteroid_003.ply");
	VaoManager.loadModelToVAO(Asteroid_003);
	
	Model Asteroid_004("Asteroid_004", "Asteroid_004.ply");
	VaoManager.loadModelToVAO(Asteroid_004);
	
	Model Asteroid_005("Asteroid_005", "Asteroid_005.ply");
	VaoManager.loadModelToVAO(Asteroid_005);

	Model Asteroid_006("Asteroid_006", "Asteroid_006.ply");
	VaoManager.loadModelToVAO(Asteroid_006);
	
	Model Asteroid_007("Asteroid_007", "Asteroid_007.ply");
	VaoManager.loadModelToVAO(Asteroid_007);

	


#pragma endregion 
	
#pragma region mesh

	//create falcon1
	Mesh imposter("imposter", imposter_Mod, "rock_Textures5");
	imposter.pos = glm::vec3(-46.1999, -0.4, 6.69997);
	imposter.isUseAlphaTexture = true;
	imposter.alphaTextureName = "rock_Textures5";
	imposter.scale = 8.50011;
	imposter.colour = glm::vec4(1, 1, 1,0.8);
	MeshToDraw.push_back(imposter);
	
	Mesh body("body", Body_mod);
	body.pos = glm::vec3 (-48.5999, -1.7, 4.79998);
	body.scale = 1.08;
	//body.orientation = glm::vec3(0.26, 0.02, 0.12);
	MeshToDraw.push_back(body);
	

	Mesh panel("panel", Port_Panel_mod);
	panel.pos = glm::vec3(-46.4999, 7.4, 3.19998);
	panel.scale = 0.370001;
	//panel.orientation = glm::vec3(0.26, 0.02, 0.12);
	MeshToDraw.push_back(panel);

	Mesh starboardpanel("starboardpanel", Starboard_Panel_mod);
	starboardpanel.pos = glm::vec3(-51.3998, -6.4, 2.79998);
	starboardpanel.scale = 0.65;
	//starboardpanel.orientation = glm::vec3(0.26, 0.02, 0.12);
	MeshToDraw.push_back(starboardpanel);


	Mesh Asteroid8("Asteroid_008", Asteroid_001, "rock_Textures2");
	Asteroid8.pos = glm::vec3(162.601, 13.1, -189.498);
	Asteroid8.orientation = vec3(0.02, 0.08, -7.45058e-08);
	Asteroid8.scale = 40.1002;
	MeshToDraw.push_back(Asteroid8);

	Mesh Asteroid1("Asteroid_001", Asteroid_001, "rock_Textures1");
	Asteroid1.pos = glm::vec3(0.500027, -6.09999, 9.79999);
	Asteroid1.scale = 45.33f;
	Asteroid1.orientation = glm::vec3(0.6, -0.2, 1.17);
	Asteroid1.colour = vec4(1.0f, 1.f, 1.f, 1.f);
	MeshToDraw.push_back(Asteroid1);


	Mesh Asteroid2("Asteroid_002", Asteroid_002, "rock_Textures2");
	Asteroid2.pos = glm::vec3(16.1001, 6.7, 81.2993);
	Asteroid2.scale = 20.11f;
	Asteroid2.colour = vec4(1.0f, 1.f, 1.f, 1.f);
	//Asteroid2.orientation = glm::vec3(-3.07, -3.05, 1.68);
	MeshToDraw.push_back(Asteroid2);

	Mesh Asteroid3("Asteroid_003", Asteroid_003,"rock_Textures3" );
	Asteroid3.pos = glm::vec3(-43.9999, -5.50004, -7.4);
	Asteroid3.scale = 58.83f;	
	Asteroid3.colour = vec4(1.0f, 1.f, 1.f, 1.f);

	//Asteroid3.orientation = glm::vec3(-3.07, -3.05, 1.68);
	MeshToDraw.push_back(Asteroid3);

	Mesh Asteroid4("Asteroid_004", Asteroid_004, "rock_Textures4");
	Asteroid4.pos = glm::vec3(-80.9994, 26.9001, -17.8);
	Asteroid4.scale = 20.25;
	Asteroid4.colour = vec4(1.0f, 0.5f, 1.f, 1.f);

	//Asteroid4.orientation = glm::vec3(-3.07, -3.05, 1.68);
	MeshToDraw.push_back(Asteroid4);
	
	Mesh Asteroid5("Asteroid_005", Asteroid_005, "rock_Textures");
	Asteroid5.pos = glm::vec3(-119.499, -52.7998, -28.6001);
	Asteroid5.scale = 13.1002;
	Asteroid5.colour = vec4(0.4f, 1.f, 1.f, 1.f);
	//Asteroid5.orientation = glm::vec3(-3.07, -3.05, 1.68);
	MeshToDraw.push_back(Asteroid5);
	
	//Mesh Asteroid6("Asteroid_006", Asteroid_006, "rock_Textures6");
	//Asteroid6.pos = glm::vec3(64.4996, 8.79995, -28.6001);
	//Asteroid6.scale = 3.1002;
	//Asteroid6.colour = vec4(0.4f, 1.f, 1.f, 1.f);	
	//MeshToDraw.push_back(Asteroid6);
	//
	//Mesh Asteroid7("Asteroid_007", Asteroid_007, "rock_Textures4");
	//Asteroid7.pos = glm::vec3(-52.2998, -19.2002, -28.6001);
	//Asteroid7.scale = 3.1002;
	////Asteroid7.colour = vec4(0.4f, 1.f, 1.f, 1.f);	
	//MeshToDraw.push_back(Asteroid7);

	
	
#pragma endregion 

#pragma region point light

	//light 
	Light lightone;
	lightone.position = vec3(19.5001, 35.8, 24.8);
	lightone.atten = vec3(1, 0.00800001, 2.74103e-21);
	lightone.diffuse = vec3(1);
	lights.Lights.push_back(lightone);

	//light2
	Light light2;
	light2.position = vec3(6.40004, 2.19991, 115.499);
	light2.atten = vec3(1, 0.0459997, 4.53491e-31);
	light2.diffuse = vec3(1, 1, 1);
	lights.Lights.push_back(light2);
	//
	Light light3;
	light3.position = vec3(0.199998, 11.4, 5.39998);
	light3.atten = vec3(1, 0.039, 0.0340892);
	light3.diffuse = vec3( 1);
	lights.Lights.push_back(light3);
	//
	Light light4;
	light4.position = vec3(-44.6999, -29.8001, -227.402);
	light4.atten = vec3(1, 0.0020009, 2.24323e-12);
	lights.Lights.push_back(light4);
	//
	Light light5;
	light5.position = vec3(-15.1999, 2, 25.1001);
	light5.atten = vec3(1, 0.015, 0.00501816);
	light5.turnOff();
	light5.diffuse = vec3(1, 1.82, 1);
	lights.Lights.push_back(light5);
	lights.GetPointLightsUniformLocations(shaderProgram);
#pragma endregion 

#pragma region spot light

	//Spot
		//
	
	

	SpotLight spotLight1;
	spotLight1.position = vec3(-48.5999, -1.7, 13.7);
	
	spotLight1.atten = vec3(1, -0.455998, 0.0634822);
	spotLight1.direction = vec3(-1, 0, 0);
	lights.SpotLights.push_back(spotLight1);
	
	SpotLight spotLight2;
	spotLight2.position = vec3(-52.9998, -10.7, 5.69998);
	
	spotLight2.atten = vec3 (1, -0.745994, 0.151885 );
	spotLight2.diffuse = vec3(1,0.5,0);
	spotLight2.direction = vec3(-1, -1, 0);
	lights.SpotLights.push_back(spotLight2);
	
	SpotLight spotLight3;
	spotLight3.position = vec3(-44.5999, 9.7, 7.59998);
	spotLight3.diffuse = vec3(1, 0.5, 0);
	spotLight3.atten = vec3(1, -0.805993, 0.185762);
	spotLight3.direction = vec3(-1, 0, 0);
	lights.SpotLights.push_back(spotLight3);

	SpotLight spotLight4;
	spotLight4.position = vec3(-34.9001, -13.6, -215.101);
	spotLight4.atten = vec3(1, -0.770993, 0.102771);
	spotLight4.direction = vec3(-28.8, 0, 0);
	lights.SpotLights.push_back(spotLight4);
	//
	SpotLight spotLight5;
	spotLight5.position = vec3(-31.3001, -12.4, -218.701);
	spotLight5.atten = vec3(1, -0.622995, 0.0977671);
	spotLight5.direction = vec3(-28.8, 0, 0);
	lights.SpotLights.push_back(spotLight5);

	lights.GetSpotLightsUniformLocations(shaderProgram);
#pragma endregion
	sortMeshes();
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
		////draw light sphere
		drawLightSphere(lightSphere, shaderProgram);
		drawSpotLightSphere(lightSphere, shaderProgram);

        //draw mesh
			for (vector<Mesh>::iterator iter = MeshToDraw.begin(); iter != MeshToDraw.end(); iter++) {
				Mesh mesh = *iter;
			
				drawMesh(mesh, shaderProgram);
			}

		//light up 
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

//input 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	const float cameraSpeed = 0.2f;
  #pragma region change mesh
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
			cout << MeshToDraw[indexSelectedMesh].pos.z << endl;
		}
		if (key == GLFW_KEY_S) {
			MeshToDraw[indexSelectedMesh].pos.z -= 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.z << endl;
		}

		if (key == GLFW_KEY_Q) {
			MeshToDraw[indexSelectedMesh].pos.y += 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.y << endl;
		}
		if (key == GLFW_KEY_E) {
			MeshToDraw[indexSelectedMesh].pos.y -= 0.1f;
			cout << MeshToDraw[indexSelectedMesh].pos.y << endl;
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
#pragma endregion 

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
		//case 1:cameraPos = cameraPos2, cameraTarget=cameraTarget2; break;
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
			<< MeshToDraw[index].colour.z << ", "
			<< MeshToDraw[index].colour.a << "\n\n";

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
			<< lights.SpotLights[index].position.x << " , "
			<< lights.SpotLights[index].position.y << " , "
			<< lights.SpotLights[index].position.z << "\n\n atten "
			<< lights.SpotLights[index].atten.x << ", "
			<< lights.SpotLights[index].atten.y << ", "
			<< lights.SpotLights[index].atten.z << " \n\n color "
			<< lights.SpotLights[index].diffuse.x << ","
			<< lights.SpotLights[index].diffuse.y << ","
			<< lights.SpotLights[index].diffuse.z << " \n\n direction "
			<< lights.SpotLights[index].direction.x << " ,"
			<< lights.SpotLights[index].direction.y << " ,"
			<< lights.SpotLights[index].direction.z << " \n\n";

	}
	saveFile <<"Camera "<< cameraPos.x << "," << cameraPos.y << "," << cameraPos.z;

	saveFile.close();
	cout << "Saved...." << endl;
	return;
}
#pragma endregion 

void sortMeshes() {
	vector<Mesh> transparentMesh;
	vector<Mesh> temp;
	map<float, Mesh> sorted;
	for (vector<Mesh>::iterator iter = MeshToDraw.begin(); iter != MeshToDraw.end(); iter++) {
		Mesh mesh = *iter;
		if (mesh.colour.a < 1.0f||mesh.isUseAlphaTexture) {
			transparentMesh.push_back(mesh);

		}
		else
			temp.push_back(mesh);
	}
	MeshToDraw.clear();
	MeshToDraw.swap(temp);
	//sort transparent mesh
	for (unsigned int i = 0; i < transparentMesh.size(); i++)
	{
		float distance = length(cameraPos - transparentMesh[i].pos);
		sorted[distance] = transparentMesh[i];
	}
	//put back to original container
	for (map<float, Mesh>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{

		MeshToDraw.push_back(it->second);
	}


}
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

void drawMesh(Mesh mesh,int shaderId) {
	//open blend function
	if (mesh.colour.a < 1.0f||mesh.isUseAlphaTexture) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

#pragma region texture binding 
	

#pragma endregion

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
		glUniform4f(colorLoc, mesh.colour.x, mesh.colour.y, mesh.colour.z, mesh.colour.a);

		// Is the object wireframe
		if (mesh.isWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}


		GLint bUseTexture_UniLoc = glGetUniformLocation(shaderId, "bUseTexture");
		GLint isUseOneTexture_UniLoc = glGetUniformLocation(shaderId, "isUseOneTexture");
		GLint isUseTwoTexture_UniLoc = glGetUniformLocation(shaderId, "isUseTwoTexture");
		//get VAO Id 
		if (mesh.textureName.size() > 0) {

			glUniform1f(bUseTexture_UniLoc, (float)GL_TRUE);

			if (mesh.textureName.size() == 1) {
			
				glUniform1f(isUseOneTexture_UniLoc, (float)GL_TRUE);
				//unit 00
				GLuint texture00Unit = 0;			// Texture unit go from 0 to 79
				glActiveTexture(texture00Unit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984

				GLuint	 textureId = textureManager.getTextureIdFromName(mesh.textureName[0]);
				glBindTexture(GL_TEXTURE_2D, textureId);
				GLint texSamp2D_00_UniLoc
					= glGetUniformLocation(shaderId, "texSamp2D_00");
				glUniform1i(texSamp2D_00_UniLoc, texture00Unit);
			}
		
			else if (mesh.textureName.size() == 2) {
				glUniform1f(isUseTwoTexture_UniLoc, (float)GL_TRUE);
				//unit 00
				GLuint texture00Unit = 0;			// Texture unit go from 0 to 79
				glActiveTexture(texture00Unit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984

				GLuint	 textureId = textureManager.getTextureIdFromName(mesh.textureName[0]);
				glBindTexture(GL_TEXTURE_2D, textureId);
				GLint texSamp2D_00_UniLoc
					= glGetUniformLocation(shaderId, "texSamp2D_00");
				glUniform1i(texSamp2D_00_UniLoc, texture00Unit);

				//unit 01
				GLuint texture01Unit = 1;			// Texture unit go from 0 to 79
				glActiveTexture(texture01Unit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984

				GLuint	 textureId2 = textureManager.getTextureIdFromName(mesh.textureName[1]);
				glBindTexture(GL_TEXTURE_2D, textureId2);
				GLint texSamp2D_01_UniLoc
					= glGetUniformLocation(shaderId, "texSamp2D_01");
				glUniform1i(texSamp2D_01_UniLoc, texture01Unit);
			}
		
		}
		//check weather to use alphatexture
		GLint bUseAlphaTexture_UniLoc = glGetUniformLocation(shaderId, "bUseAlphaTexture");
		if (mesh.isUseAlphaTexture) {
			GLint texSamp2D_Alpha_UniLoc = glGetUniformLocation(shaderId, "texSamp2D_Alpha");
		
			glUniform1f(bUseAlphaTexture_UniLoc, (float)GL_TRUE);
			GLuint	 alphaModTextureNumber = textureManager.getTextureIdFromName(mesh.alphaTextureName);
			glActiveTexture(GL_TEXTURE31);	// Pick the last one
			glBindTexture(GL_TEXTURE_2D, alphaModTextureNumber);
			glUniform1i(texSamp2D_Alpha_UniLoc, 31);	// NOT GL_TEXTURE0, but the actual 0
		}

		Model mod = VaoManager.dateVAO.find(mesh.modelType)->second;
		glBindVertexArray(mod.VAOId);
		glDrawElements(GL_TRIANGLES, mod.numberOfIndices, GL_UNSIGNED_INT, 0);
		glUniform1f(bUseTexture_UniLoc, (float)GL_FALSE);
		glUniform1f(isUseOneTexture_UniLoc, (float)GL_FALSE);
		glUniform1f(isUseTwoTexture_UniLoc, (float)GL_FALSE);
		glUniform1f(bUseAlphaTexture_UniLoc, (float)GL_FALSE);

		if (mesh.colour.a < 1.0f || mesh.isUseAlphaTexture) {
			glDisable(GL_BLEND);

		}
		
}
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}