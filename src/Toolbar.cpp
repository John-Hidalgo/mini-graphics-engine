#include "Toolbar.h"

Toolbar::Toolbar() : canvasRef(nullptr) {}

void Toolbar::setup(Canvas* canvas) {
	canvasRef = canvas;

	// On charge les possibles curseurs au debut
	loadCursorIcons();

	dessinez.setup("Dessinez");
	dessinez.setPosition(220,0);
	dessinez.setSize(200, 0);
	
	dessinez.add(rectangleToggle.setup("Rectangle", false));
	rectangleToggle.addListener(this, &Toolbar::rectangleToggleChanged);
	
	dessinez.add(circleToggle.setup("Cercle", false));
	circleToggle.addListener(this, &Toolbar::circleToggleChanged);
	
	dessinez.add(lineToggle.setup("Ligne", false));
	lineToggle.addListener(this, &Toolbar::lineToggleChanged);

	dessinez.add(pointToggle.setup("Point", false));
	pointToggle.addListener(this, &Toolbar::pointToggleChanged);

	dessinez.add(squareToggle.setup("Carre", false));
	squareToggle.addListener(this, &Toolbar::squareToggleChanged);

	dessinez.add(triangleToggle.setup("Triangle", false));
	triangleToggle.addListener(this, &Toolbar::triangleToggleChanged);
	
	dessinez.add(freeformToggle.setup("Libre", false));
	freeformToggle.addListener(this, &Toolbar::freeformToggleChanged);

	dessinez.add(houseToggle.setup("Maison", false));
	houseToggle.addListener(this, &Toolbar::houseToggleChanged);

	dessinez.add(treeToggle.setup("Arbre", false));
	treeToggle.addListener(this, &Toolbar::treeToggleChanged);

	dessinez.add(targetToggle.setup("Cible", false));
	targetToggle.addListener(this, &Toolbar::targetToggleChanged);
	
	dessinez.add(selectColourToggle.setup("Couleur du canevas", false));

	selectColourToggle.addListener(this, &Toolbar::selectColourToggleChanged);
	
	dessinez.add(redBtn.setup("Rouge"));
	dessinez.add(greenBtn.setup("Vert"));
	dessinez.add(blueBtn.setup("Bleue"));
	dessinez.add(yellowBtn.setup("Jaune"));

	currentColour.setup("Couleur Courant", "");
	dessinez.add(&currentColour);
	currentColour.setBackgroundColor(ofColor(255, 255, 255));

	redBtn.setBackgroundColor(ofColor(150, 50, 50));
	greenBtn.setBackgroundColor(ofColor(50, 150, 50));
	blueBtn.setBackgroundColor(ofColor(50, 50, 150));
	yellowBtn.setBackgroundColor(ofColor(150, 150, 50));

	redBtn.setTextColor(ofColor(255, 200, 200));
	greenBtn.setTextColor(ofColor(200, 255, 200));
	blueBtn.setTextColor(ofColor(200, 200, 255));
	yellowBtn.setTextColor(ofColor(255, 255, 200));
	
	redBtn.addListener(this, &Toolbar::onRedPressed);
	greenBtn.addListener(this, &Toolbar::onGreenPressed);
	blueBtn.addListener(this, &Toolbar::onBluePressed);
	yellowBtn.addListener(this, &Toolbar::onYellowPressed);

	currentColor = ofColor(255, 255, 255);
	currentColorName = "";
	
	dessinez.add(undoButton.setup("defaire"));
	undoButton.addListener(this, &Toolbar::undoButtonPressed);
	
	dessinez.add(histogramToggle.setup("Affichez l'histogramme", false));
	histogramToggle.addListener(this, &Toolbar::histogramToggleChanged);

	dessinez.minimize();

	echantillonage.setup("Echantillonage");
	echantillonage.setPosition(440,0);
	echantillonage.setSize(200,0);

	// Deux modes d'échantillonnage
	echantillonage.add(echantillonageUniqueToggle.setup("Unique Image", true));
	echantillonage.add(echantillonageMultipleToggle.setup("Image Folder", false));

	echantillonage.add(echantillonageButton.setup("Echantillonez Image(s)"));
	echantillonageButton.addListener(this, &Toolbar::echantillonagePressed);

	echantillonage.add(sampleXSlider.setup("Sample Pos X", 100, 1, 1200));
	echantillonage.add(sampleYSlider.setup("Sample Pos Y", 100, 1, 1200));
	echantillonage.add(sampleWidthSlider.setup("Sample Width", 200, 1, 1200));
	echantillonage.add(sampleHeightSlider.setup("Sample Height", 200, 1, 1200));

	// Paramètres pour le mode multiple (pour les dossiers d'images)
	echantillonage.add(gridColumnsSlider.setup("Colonnes Grille", 2, 1, 8));
	echantillonage.add(gridRowsSlider.setup("Lignes Grille", 2, 1, 8));

	// Pour le display
	echantillonage.add(imageDisplayScaleSlider.setup("Taille affichage UI", 1.0f, 0.1f, 3.0f));
	echantillonage.add(maintainAspectRatioToggle.setup("Garder Aspect Ratio", true));

	imageDisplayScaleSlider.addListener(this, &Toolbar::imageDisplayScaleChanged);
	maintainAspectRatioToggle.addListener(this, &Toolbar::maintainAspectRatioToggled);

	echantillonage.minimize();
	
	importation.setup("Importation");
	importation.setPosition(660,0);
	importation.setSize(200,0);

	importation.add(importImageButton.setup("Importez Image"));
	importImageButton.addListener(this, &Toolbar::importImagePressed);

	importation.add(importModelButton.setup("Importez Model 3D"));
	importModelButton.addListener(this, &Toolbar::importModelPressed);

	// --- Variant selector group ---
	variantGroup.setup("3D model lighting");

	variantNoneButton.setup("Lambert");
	variantMetallicButton.setup("Gouraud");
	variantPlasticButton.setup("Phong");
	variantWireframeButton.setup("Blinn-Phong");
	variantTransparentButton.setup("Cell-Shaded");

	variantNoneButton.addListener(this, &Toolbar::variantNonePressed);
	variantMetallicButton.addListener(this, &Toolbar::variantMetallicPressed);
	variantPlasticButton.addListener(this, &Toolbar::variantPlasticPressed);
	variantWireframeButton.addListener(this, &Toolbar::variantWireframePressed);
	variantTransparentButton.addListener(this, &Toolbar::variantTransparentPressed);

	variantGroup.add(&variantNoneButton);
	variantGroup.add(&variantMetallicButton);
	variantGroup.add(&variantPlasticButton);
	variantGroup.add(&variantWireframeButton);
	variantGroup.add(&variantTransparentButton);

	importation.add(&variantGroup);
	
	
	importation.minimize();

	// 3D Primitives Group
	primitives3DGroup.setup("Primitives 3D");
	primitives3DGroup.setPosition(880, 0);
	primitives3DGroup.setSize(200, 0);

	primitives3DGroup.add(sphereToggle.setup("Sphere", false));
	sphereToggle.addListener(this, &Toolbar::sphereToggleChanged);

	primitives3DGroup.add(cubeToggle.setup("Cube", false));
	cubeToggle.addListener(this, &Toolbar::cubeToggleChanged);

	primitives3DGroup.add(cylinderToggle.setup("Cylinder", false));
	cylinderToggle.addListener(this, &Toolbar::cylinderToggleChanged);

	primitives3DGroup.add(coneToggle.setup("Cone", false));
	coneToggle.addListener(this, &Toolbar::coneToggleChanged);

	primitives3DGroup.add(torusToggle.setup("Torus", false));
	torusToggle.addListener(this, &Toolbar::torusToggleChanged);

	primitives3DGroup.add(pyramidToggle.setup("Pyramid", false));
	pyramidToggle.addListener(this, &Toolbar::pyramidToggleChanged);

	primitives3DGroup.minimize();

	updateVariantButtonColors();
	updateCursorIcon();
}

void Toolbar::setSelectedVariant(ModelVariant variant) {
	selectedVariant = variant;
	updateVariantButtonColors();
}

void Toolbar::updateVariantButtonColors() {
	auto resetColor = ofColor(80);
	variantNoneButton.setBackgroundColor(resetColor);
	variantMetallicButton.setBackgroundColor(resetColor);
	variantPlasticButton.setBackgroundColor(resetColor);
	variantWireframeButton.setBackgroundColor(resetColor);
	variantTransparentButton.setBackgroundColor(resetColor);

	ofColor highlight(0, 120, 255);

	switch (selectedVariant) {
	case ModelVariant::None:
		variantNoneButton.setBackgroundColor(highlight);
		break;
	case ModelVariant::Metallic:
		variantMetallicButton.setBackgroundColor(highlight);
		break;
	case ModelVariant::Plastic:
		variantPlasticButton.setBackgroundColor(highlight);
		break;
	case ModelVariant::Wireframe:
		variantWireframeButton.setBackgroundColor(highlight);
		break;
	case ModelVariant::Transparent:
		variantTransparentButton.setBackgroundColor(highlight);
		break;
	}
}

void Toolbar::variantNonePressed() {
	setSelectedVariant(ModelVariant::None);
	lighting = Lighting::LAMBERT;
}

void Toolbar::variantMetallicPressed() {
	setSelectedVariant(ModelVariant::Metallic);
	lighting = Lighting::GOURAUD;
}

void Toolbar::variantPlasticPressed() {
	setSelectedVariant(ModelVariant::Plastic);
	lighting = Lighting::PHONG;
}

void Toolbar::variantWireframePressed() {
	setSelectedVariant(ModelVariant::Wireframe);
	lighting = Lighting::BLINNPHONG;
}

void Toolbar::variantTransparentPressed() {
	setSelectedVariant(ModelVariant::Transparent);
	lighting = Lighting::CELL;
}

void Toolbar::draw() {
	dessinez.draw();
	importation.draw();
	echantillonage.draw();
	primitives3DGroup.draw();

	// Si on dessine une forme/primitive on draw le curseur
	if (isDrawingActive && selectedCursor.isAllocated()) {
		ofSetColor(255);
		selectedCursor.draw(ofGetMouseX() - 1.0f, ofGetMouseY(), 20, 20);
	}
}

void Toolbar::rectangleToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::RECTANGLE);
	else if (canvasRef->getCurrentMode() == ShapeMode::RECTANGLE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::circleToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::CIRCLE);
	else if (canvasRef->getCurrentMode() == ShapeMode::CIRCLE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::lineToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::LINE);
	else if (canvasRef->getCurrentMode() == ShapeMode::LINE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::houseToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::HOUSE);
	else if (canvasRef->getCurrentMode() == ShapeMode::HOUSE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::treeToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::TREE);
	else if (canvasRef->getCurrentMode() == ShapeMode::TREE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::targetToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::TARGET);
	else if (canvasRef->getCurrentMode() == ShapeMode::TARGET) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::pointToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::POINT);
	else if (canvasRef->getCurrentMode() == ShapeMode::POINT) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::squareToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::SQUARE);
	else if (canvasRef->getCurrentMode() == ShapeMode::SQUARE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::triangleToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::TRIANGLE);
	else if (canvasRef->getCurrentMode() == ShapeMode::TRIANGLE) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::freeformToggleChanged(bool & val) {
	if (val) setExclusiveToggle(ShapeMode::FREEFORM);
	else if (canvasRef->getCurrentMode() == ShapeMode::FREEFORM) {
		canvasRef->setCurrentMode(ShapeMode::NONE);
	}

	updateCursorIcon();
}

void Toolbar::selectColourToggleChanged(bool & val) {
	pickingColour = val;

	if (val) {
		rectangleToggle = false;
		circleToggle    = false;
		lineToggle      = false;
		freeformToggle  = false;
		pointToggle = false;
		triangleToggle = false;
		squareToggle    = false;
		houseToggle     = false;
		treeToggle      = false;
		targetToggle   = false;
		if (canvasRef) canvasRef->setCurrentMode(ShapeMode::NONE);
	}
}

void Toolbar::undoButtonPressed() {
	if (canvasRef) {
		canvasRef->undo();
	}
}
void Toolbar::importImagePressed() {
	ofFileDialogResult result = ofSystemLoadDialog("Choissisez une image");
	if (result.bSuccess) {
		std::string path = result.getPath();
		if (canvasRef) {
			canvasRef->loadImage(path);
		}
	}
}

void Toolbar::setExclusiveToggle(ShapeMode mode) {
	rectangleToggle = (mode == ShapeMode::RECTANGLE);
	circleToggle    = (mode == ShapeMode::CIRCLE);
	lineToggle      = (mode == ShapeMode::LINE);
	squareToggle    = (mode == ShapeMode::SQUARE);
	pointToggle     = (mode == ShapeMode::POINT);
	triangleToggle  = (mode == ShapeMode::TRIANGLE);
	freeformToggle  = (mode == ShapeMode::FREEFORM);
	houseToggle     = (mode == ShapeMode::HOUSE);
	treeToggle      = (mode == ShapeMode::TREE);
	targetToggle    = (mode == ShapeMode::TARGET);
	selectColourToggle = false;

	// On désactive toutes toggle de primitives 3D primitive
	sphereToggle = false;
	cubeToggle = false;
	cylinderToggle = false;
	coneToggle = false;
	torusToggle = false;
	pyramidToggle = false;

	if (canvasRef) {
		canvasRef->setCurrentMode(mode);
		// Pour s'assurer que le mode 3D mode est désactivé
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
		canvasRef->setDrawingColor(currentColor);
	}
}

void Toolbar::colorChanged(ofColor& col) {
	if (canvasRef) {
		canvasRef->setDrawingColor(col);
	}
}

void Toolbar::importModelPressed() {
	std::string defaultPath = ofToDataPath("", true);
	ofFileDialogResult result = ofSystemLoadDialog(
		"Select a 3D model (.obj)",
		false,
		defaultPath
	);

	if (result.bSuccess) {
		auto newModel = std::make_unique<Model3D>();
		newModel->setup();
		newModel->loadModel(result.getPath());

		newModel->color_background = canvasRef->color_picker_background;
		newModel->color_ambient = canvasRef->color_picker_ambient;
		newModel->color_diffuse = canvasRef->color_picker_diffuse;
		//newModel->applyVariant(selectedVariant);
		newModel->setShader(lighting);
		canvasRef->models.push_back(std::move(newModel));

		ofLogNotice() << "Imported model: " << result.getPath()
					  << " | Variant: " << static_cast<int>(selectedVariant);
	}
}

void Toolbar::echantillonagePressed() {
	if (echantillonageUniqueToggle) {
		echantillonageUniquePressed();
	} else {
		echantillonageMultiplePressed();
	}
}

void Toolbar::echantillonageUniquePressed() {
	ofFileDialogResult result = ofSystemLoadDialog("Choisissez une image a echantillonner");
	if (!result.bSuccess) return;

	ofImage sourceImage;
	if (!sourceImage.load(result.getPath())) {
		ofLogError() << "Impossible de charger l'image : " << result.getPath();
		return;
	}

	int sampleWidth = sampleWidthSlider;
	int sampleHeight = sampleHeightSlider;
	int sampleX = sampleXSlider;
	int sampleY = sampleYSlider;

	// Controle / validation des params
	sampleX = ofClamp(sampleX, 0, sourceImage.getWidth() - 1);
	sampleY = ofClamp(sampleY, 0, sourceImage.getHeight() - 1);
	sampleWidth = ofClamp(sampleWidth, 1, sourceImage.getWidth() - sampleX);
	sampleHeight = ofClamp(sampleHeight, 1, sourceImage.getHeight() - sampleY);

	// On creer l'image
	ofImage destination;
	destination.cropFrom(sourceImage, sampleX, sampleY, sampleWidth, sampleHeight);

	// Sauvegarde
	std::string outputPath = "echantillon_unique_" + ofGetTimestampString() + ".png";
	destination.save(outputPath);
	ofLogNotice() << "Échantillon unique sauvegardé: " << outputPath;

	// Load de l'image dans le canvas
	if (canvasRef) {
		canvasRef->loadImage(outputPath);
	}
}

void Toolbar::echantillonageMultiplePressed() {
	// Puisque ofSystemLoadDialog ne permet pas la selection de plusieurs images paths,
	// On load un path de dossier avec des images
	ofFileDialogResult result = ofSystemLoadDialog("Sélectionnez un dossier d'images", true); // true = select folder
	if (!result.bSuccess) return;

	std::string imageFolderPath = result.getPath();
	processEchantillonMultiple(imageFolderPath);
}


void Toolbar::processEchantillonMultiple(const std::string& folderPath) {
	// On chasrge toutes les images du dossier en limitant aux types permis par ofImage
	ofDirectory dir(folderPath);
	dir.allowExt("jpg");
	dir.allowExt("jpeg");
	dir.allowExt("png");
	dir.allowExt("gif");
	dir.listDir();

	// Si on a aucune image
	if (dir.size() == 0) {
		ofLogError() << "Aucune image trouvée dans le dossier: " << folderPath;
		return;
	}

	std::vector<std::string> imagePaths;
	for (int i = 0; i < dir.size(); i++) {
		imagePaths.push_back(dir.getPath(i));
	}

	ofLogNotice() << "Chargement de " << imagePaths.size() << " images depuis: " << folderPath;

	int gridCols = gridColumnsSlider;
	int gridRows = gridRowsSlider;
	int nbCells = gridCols * gridRows;

	// pour imiter le nombre d'images utilisées à la capacité de la grille
	int numImagesToUse = std::min((int)imagePaths.size(), nbCells);

	int sampleWidth = sampleWidthSlider;
	int sampleHeight = sampleHeightSlider;

	// Calcul des dimensions de l'echantillon final
	int finalWidth = sampleWidth * gridCols;
	int finalHeight = sampleHeight * gridRows;

	// Création de l'image finale avec un FBO
	ofFbo finalFbo;
	finalFbo.allocate(finalWidth, finalHeight, GL_RGBA);

	finalFbo.begin();
	// Transparent background au cas ou pour certain type de fichier
	ofClear(0, 0, 0, 0);

	int currentImageIndex = 0;

	// Pour chaque cellule de notre "collage" en grid
	for (int row = 0; row < gridRows; row++) {
		for (int col = 0; col < gridCols; col++) {
			if (currentImageIndex >= numImagesToUse) break;

			std::string imagePath = imagePaths[currentImageIndex];
			ofImage sourceImage;

			if (sourceImage.load(imagePath)) {
				// On redimensionne l'image pour qu'elle fit avec la cellule
				sourceImage.resize(sampleWidth, sampleHeight);

				// Position dans l'image finale
				int destX = col * sampleWidth;
				int destY = row * sampleHeight;

				// Dessiner l'image resized
				sourceImage.draw(destX, destY);

				ofLogNotice() << "Image " << currentImageIndex + 1 << " ajoutée à la position (" << col << "," << row << ")";
			} else {
				ofLogError() << "Impossible de charger l'image: " << imagePath;
			}

			currentImageIndex++;
		}
	}

	finalFbo.end();

	// Sauvegarde de l'echantillon multiple fini
	ofImage finalImage;
	finalFbo.readToPixels(finalImage.getPixels());
	finalImage.update();

	std::string outputPath = "echantillon_multiple_" + ofGetTimestampString() + ".png";
	finalImage.save(outputPath);
	ofLogNotice() << "Collage multiple sauvegardé: " << outputPath;
	ofLogNotice() << "Dimensions: " << finalWidth << "x" << finalHeight;
	ofLogNotice() << "Images utilisées: " << numImagesToUse << "/" << imagePaths.size();

	// Chargement dans le canvas
	if (canvasRef) {
		canvasRef->loadImage(outputPath);
	}
}

// Le display scale est la responsabilité du canvas
void Toolbar::imageDisplayScaleChanged(float &val) {
	if (canvasRef) {
		canvasRef->setImageDisplayScale(val);
	}
}

// Le aspect ratio est la responsabilité du canvas
void Toolbar::maintainAspectRatioToggled(bool &val) {
	if (canvasRef) {
		canvasRef->setMaintainAspectRatio(val);
	}
}

void Toolbar::onRedPressed(){
	currentColor = ofColor(255, 0, 0);
	currentColour = "Rouge";
	currentColour.setBackgroundColor(currentColor);
	if (canvasRef) {
		canvasRef->setDrawingColor(currentColor);
	}
}

void Toolbar::onGreenPressed(){
	currentColor = ofColor(0, 255, 0);
	currentColour = "Vert";
	currentColour.setBackgroundColor(currentColor);
	if (canvasRef) {
		canvasRef->setDrawingColor(currentColor);
	}
}

void Toolbar::onBluePressed(){
	currentColor = ofColor(0, 0, 255);
	currentColour = "Bleue";
	currentColour.setBackgroundColor(currentColor);
	if (canvasRef) {
		canvasRef->setDrawingColor(currentColor);
	}
}

void Toolbar::onYellowPressed(){
	currentColor = ofColor(255, 255, 0);
	currentColour = "Jaune";
	currentColour.setBackgroundColor(currentColor);
	if (canvasRef) {
		canvasRef->setDrawingColor(currentColor);
	}
}

void Toolbar::setColorFromCanvas(ofColor& color,std::string& name){
	currentColour.setBackgroundColor(color);
	currentColour = name;
	currentColor = color;
		currentColour = name;
		currentColour.setBackgroundColor(currentColor);

		redBtn.setBackgroundColor(ofColor(150, 50, 50));
		greenBtn.setBackgroundColor(ofColor(50, 150, 50));
		blueBtn.setBackgroundColor(ofColor(50, 50, 150));
		yellowBtn.setBackgroundColor(ofColor(150, 150, 50));
		if (color == ofColor(255, 0, 0)) {
			redBtn.setBackgroundColor(ofColor(255, 100, 100));
		} else if (color == ofColor(0, 255, 0)) {
			greenBtn.setBackgroundColor(ofColor(100, 255, 100));
		} else if (color == ofColor(0, 0, 255)) {
			blueBtn.setBackgroundColor(ofColor(100, 100, 255));
		} else if (color == ofColor(255, 255, 0)) {
			yellowBtn.setBackgroundColor(ofColor(255, 255, 100));
		}
		
		if (canvasRef) {
			canvasRef->setDrawingColor(currentColor);
		}
};


void Toolbar::sphereToggleChanged(bool &val) {
	if (val) setExclusivePrimitiveToggle(Primitive3DType::SPHERE);
	else if (canvasRef->getCurrentPrimitiveMode() == Primitive3DType::SPHERE) {
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
	}

	updateCursorIcon();
}

void Toolbar::cubeToggleChanged(bool &val) {
	if (val) setExclusivePrimitiveToggle(Primitive3DType::CUBE);
	else if (canvasRef->getCurrentPrimitiveMode() == Primitive3DType::CUBE) {
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
	}

	updateCursorIcon();
}

void Toolbar::cylinderToggleChanged(bool &val) {
	if (val) setExclusivePrimitiveToggle(Primitive3DType::CYLINDER);
	else if (canvasRef->getCurrentPrimitiveMode() == Primitive3DType::CYLINDER) {
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
	}

	updateCursorIcon();
}

void Toolbar::coneToggleChanged(bool &val) {
	if (val) setExclusivePrimitiveToggle(Primitive3DType::CONE);
	else if (canvasRef->getCurrentPrimitiveMode() == Primitive3DType::CONE) {
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
	}

	updateCursorIcon();
}

void Toolbar::torusToggleChanged(bool &val) {
	if (val) setExclusivePrimitiveToggle(Primitive3DType::TORUS);
	else if (canvasRef->getCurrentPrimitiveMode() == Primitive3DType::TORUS) {
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
	}

	updateCursorIcon();
}

void Toolbar::pyramidToggleChanged(bool &val) {
	if (val) setExclusivePrimitiveToggle(Primitive3DType::PYRAMID);
	else if (canvasRef->getCurrentPrimitiveMode() == Primitive3DType::PYRAMID) {
		canvasRef->setCurrentPrimitiveMode(Primitive3DType::NONE);
	}

	updateCursorIcon();
}

void Toolbar::setExclusivePrimitiveToggle(Primitive3DType mode) {
	// On désactive toutes toggle de primitives 2D primitive
	rectangleToggle = false;
	circleToggle = false;
	lineToggle = false;
	freeformToggle = false;
	pointToggle = false;
	triangleToggle = false;
	squareToggle = false;
	selectColourToggle = false;

	// On active la bonne toggle sélectionné
	sphereToggle = (mode == Primitive3DType::SPHERE);
	cubeToggle = (mode == Primitive3DType::CUBE);
	cylinderToggle = (mode == Primitive3DType::CYLINDER);
	coneToggle = (mode == Primitive3DType::CONE);
	torusToggle = (mode == Primitive3DType::TORUS);
	pyramidToggle = (mode == Primitive3DType::PYRAMID);

	if (canvasRef) {
		canvasRef->setCurrentPrimitiveMode(mode);
		// Pour s'assurer que le mode 2D mode est désactivé
		canvasRef->setCurrentMode(ShapeMode::NONE);
  }
}

void Toolbar::histogramToggleChanged(bool& val) {
	if (canvasRef) {
		canvasRef->setShowHistogram(val);
	}
}

void Toolbar::loadCursorIcons() {
	std::filesystem::path cwd = std::filesystem::current_path();
	std::cout << "Current working directory: " << cwd << std::endl;

	std::string icons2DPath = cwd.string() + "/data/resources/icons/2D/";
	std::string icons3DPath = cwd.string() + "/data/resources/icons/3D/";

	// Shape 2D
	availableCursorIcons["rectangle"] = ofImage(icons2DPath + "rectangle.png");
	availableCursorIcons["circle"] = ofImage(icons2DPath + "circle.png");
	availableCursorIcons["line"] = ofImage(icons2DPath + "line1.png");
	availableCursorIcons["freeform"] = ofImage(icons2DPath + "pen1.png");
	availableCursorIcons["point"] = ofImage(icons2DPath + "point1.png");
	availableCursorIcons["square"] = ofImage(icons2DPath + "square.png");
	availableCursorIcons["triangle"] = ofImage(icons2DPath + "triangle.png");

	// Primitives 3D
	availableCursorIcons["sphere"] = ofImage(icons3DPath + "sphere1.png");
	availableCursorIcons["cube"] = ofImage(icons3DPath + "cube.png");
	availableCursorIcons["cylinder"] = ofImage(icons3DPath + "cylinder.png");
	availableCursorIcons["cone"] = ofImage(icons3DPath + "cone.png");
	availableCursorIcons["torus"] = ofImage(icons3DPath + "torus-2D.png");
	availableCursorIcons["pyramid"] = ofImage(icons3DPath + "pyramid-square-base.png");

	for (auto& cursor : availableCursorIcons) {
		cursor.second.setUseTexture(true);
	}
}

void Toolbar::setCursor(const std::string& iconName) {
	// Si on dessine on cache le curseur OS defaut
	if (availableCursorIcons.find(iconName) != availableCursorIcons.end()) {
		ofHideCursor();
		selectedCursor = availableCursorIcons[iconName];
	} else {
		ofShowCursor();
	}
}

void Toolbar::updateCursorIcon() {
	isDrawingActive = (rectangleToggle || circleToggle || lineToggle ||
					pointToggle || squareToggle || triangleToggle || freeformToggle ||
					sphereToggle || cubeToggle || cylinderToggle ||
					coneToggle || torusToggle || pyramidToggle || selectColourToggle);

	if (isDrawingActive) {
		//On cache le curseur par defaut de l'OS
		ofHideCursor();

		if (rectangleToggle) {
			setCursor("rectangle");
		} else if (circleToggle) {
			setCursor("circle");
		} else if (lineToggle) {
			setCursor("line");
		} else if (freeformToggle) {
			setCursor("freeform");
		} else if (pointToggle) {
			setCursor("point");
		} else if (squareToggle) {
			setCursor("square");
		} else if (triangleToggle) {
			setCursor("triangle");
		} else if (sphereToggle) {
			setCursor("sphere");
		} else if (cubeToggle) {
			setCursor("cube");
		} else if (cylinderToggle) {
			setCursor("cylinder");
		} else if (coneToggle) {
			setCursor("cone");
		} else if (torusToggle) {
			setCursor("torus");
		} else if (pyramidToggle) {
			setCursor("pyramid");
		} else {
			setCursor("default");
		}
	}
	else {
		ofShowCursor();
	}
}
