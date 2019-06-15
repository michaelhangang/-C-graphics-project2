#pragma once

#include <string>
#include <map>
#include"Model.h"
using namespace std;
class VAOManager {

public:
	 void loadModelToVAO(Model &name);
	 map<string, Model> dateVAO;

};