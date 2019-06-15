#include"Shader.h"
#include"globe.h"
#include<fstream>
#include<sstream>
#include<iostream>
Shader::Shader(shaderType type,string filename):type(type),Id(0),glslFile(filename){}

void  Shader::createShader() {
	shaderType type =this->type;
	if (type == VERTEX) {
		this->Id = glCreateShader(GL_VERTEX_SHADER);	
	}
	else if (type == FRAGMENT) {
		this->Id = glCreateShader(GL_FRAGMENT_SHADER);
	}
	
	loadSourceFromFile();
	compileShaderFromSource(*this);
	checkShaderCompile(*this);
}

// Returns bool if didn't load
bool Shader::loadSourceFromFile()
{
	string fullFileName =  this->glslFile;

	ifstream theFile(fullFileName);
	if (!theFile.is_open())
	{
		cout << "Opening shader file failed";
		return false;
	}

	this->vecSource.clear();
	
	string temp;
	while (getline(theFile, temp))
	{		
		this->vecSource.push_back(temp);
	}

	theFile.close();
	return true;		// Return the string (from the sstream)
}



bool Shader::compileShaderFromSource(Shader &shader)
{
	const unsigned int MAXLINESIZE = 8 * 1024;	// About 8K PER LINE, which seems excessive

	unsigned int numberOfLines = static_cast<unsigned int>(shader.vecSource.size());

	// This is an array of pointers to strings. aka the lines of source
	char** arraySource = new char*[numberOfLines];
	// Clear array to all zeros (i.e. '\0' or null terminator)
	memset(arraySource, 0, numberOfLines);

	for (unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++)
	{
		unsigned int numCharacters = (unsigned int)shader.vecSource[indexLine].length();
		// Create an array of chars for each line
		arraySource[indexLine] = new char[numCharacters + 2];		// For the '\n' and '\0' at end
		memset(arraySource[indexLine], 0, numCharacters + 2);

		// Copy line of source into array
		for (unsigned int indexChar = 0; indexChar != shader.vecSource[indexLine].length(); indexChar++)
		{
			arraySource[indexLine][indexChar] = shader.vecSource[indexLine][indexChar];
		}//for ( unsigned int indexChar = 0...

		// At a '\0' at end (just in case)
		arraySource[indexLine][numCharacters + 0] = '\n';
		arraySource[indexLine][numCharacters + 1] = '\0';

	}
		glShaderSource(shader.Id, numberOfLines, arraySource, NULL);
		glCompileShader(shader.Id);

		// Get rid of the temp source "c" style array
		for (unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++)
		{	// Delete this line
			delete[] arraySource[indexLine];
		}
		// And delete the original char** array
		delete[] arraySource;
		return true;
	}

void Shader::checkShaderCompile(Shader shader) {
	int success;
	char infoLog[512];
	glGetShaderiv(shader.Id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader.Id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}