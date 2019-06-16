#pragma once

#include <string>
#include <map>
#include"Model.h"
#include <glm/glm.hpp>
using namespace std;
class VAOManager {

public:
	 void loadModelToVAO(Model &name);
	 enum enumTEXCOORDBIAS
	 {
		 POSITIVE_X, POSITIVE_Y, POSITIVE_Z
	 };
	 void m_GenTextureCoordsSpherical(Model& drawInfo, enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias, bool basedOnNormals, float scale, bool fast);

	 map<string, Model> dateVAO;

};