#include "clsController.h"

clsController oController = clsController();

void setup() 
{
	Serial.begin(115200);
	Serial.println("Starting Initialisation");
	oController.InitialiseController();
}

void loop() 
{
	oController.RunProgram();
}