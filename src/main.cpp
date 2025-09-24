#include "ofMain.h"
#include "Application.h"

//========================================================================
int main( ){

//	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
//	ofGLWindowSettings settings;
//	settings.setSize(1024, 768);
//	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN
//
//	auto window = ofCreateWindow(settings);
//
//	ofRunApp(window, std::make_shared<Application>());
//	ofRunMainLoop();
	
	// paramètres du contexte de rendu
	ofGLFWWindowSettings windowSettings;

	// résolution de la fenêtre d'affichage
	windowSettings.setSize(1028, 768);

	// sélection de la version de OpenGL
	windowSettings.setGLVersion(3, 3);

	// création de la fenêtre
	ofCreateWindow(windowSettings);

	// démarrer l'exécution de l'application
	ofRunApp(new Application());

}
