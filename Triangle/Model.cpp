#include"Model.h"



Model::Model() {

}
Model::Model(string name,string fileName):name(name),fileName(fileName){
	
	this->VAOId=0;
    this-> numberOfIndices=0;
	this->numberOfVertices = 0;
}