#pragma once
#include<string>
#include<vector>
using namespace std;


class Shader 
{
public:
	enum shaderType {
		VERTEX,
		FRAGMENT,
        };
	shaderType type;
	unsigned int Id;
	string glslFile;
	vector<string> vecSource;

	string getShaderTypeString() { 
		if (this->type == VERTEX) {
			return "VERTEX_SHADER";
		}

		else if (this->type == FRAGMENT) {
			return "FRAGMENT_SHADER";
		}
		else
			return "Unknown ";
	}
	
	Shader(shaderType type,string filename);

	void  createShader();
	bool loadSourceFromFile();
	bool compileShaderFromSource(Shader &shader);
	void checkShaderCompile(Shader shader);
};