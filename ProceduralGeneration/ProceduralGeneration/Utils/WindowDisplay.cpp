#include "WindowDisplay.h"

WindowDisplay::WindowDisplay(int width, int height, const std::string& title)
{
	// Set up the variables so can be accesed later
	this->m_widthScreen = width;
	this->m_heightScreen = height;

	// Initialize all the components from GLFW
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Set whether the windowed mode window will be resizable by the user
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	// Set the desired bit depths of the various component
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_REFRESH_RATE, INT_MAX);

	// Set whether the framebuffer should be double buffered
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

	// Create a window
	this->m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	// If the window is null terminate the process
	if (this->m_window == NULL)
	{
		std::cout << "ERROR: GLFW wasn't able to create the window!!" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(this->m_window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: Failed to initialize GLAD" << std::endl;
		return;
	}

	// Init the keyboard input
	this->m_keyboard = new Keyboard(this);

	// Init the fps with the window
	this->m_timer = new Timer();

	// Initialise the guide on how to interact with the scene
	std::cout << "---------------------------------------------" << "\n";
	std::cout << "\tControls:" << "\n";
	std::cout << "\t\tWS: \tForward/Back" << "\n";
	std::cout << "\t\tAD: \tRotate - Left/Right" << "\n";
	std::cout << "\t\tSpace: \tRotate Up" << "\n";;
	std::cout << "\t\tLCtrl: \tRotate Down" << "\n";
	std::cout << "---------------------------------------------" << "\n";
	std::cout << "\tDetails:" << "\n";
	std::cout << "\t\tR: \tRe-generate content" << "\n";
	std::cout << "---------------------------------------------" << "\n";
}

WindowDisplay::~WindowDisplay()
{
	// Clean the context and destroy de window
	glfwDestroyWindow(this->m_window);

	// Stops initialising everything
	glfwTerminate();
}

void WindowDisplay::UpdateWindow()
{
	// Update the frameRate
	this->m_timer->CalculateFrameRate();

	// Resize the window and the viewport
	this->ResizeWindow();

	// Update the keyboard input based on the event pool
	this->m_keyboard->KeyboardAction(/*this->m_sceneManager*/);

	// Run update for all generated objects.
	for (std::vector<GeneratedObjects*>::size_type i = 0; i != this->generatedObjects.size(); i++) {
		this->generatedObjects[i]->DrawScene(this->m_timer->GetPassedTime() / 20.f);
		if (i == 0) {
			// Run update on the skybox cube map
			this->windowSkyBox->DrawSkyBox();
		}
	}

	// Tells to the operating system to swap the windows on those 2 buffers and so there are no moments when nothing is drawn on the screen
	glfwSwapBuffers(this->m_window);
}

void WindowDisplay::DrawWindow()
{
	// Build up the application data
	Transform* initPos = new Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	/* ----- TERRAIN ----- */ // [0]
	GeneratedData genDataTerrain = Generator(513, -50.0f, 0.1f).DiamondSquareAlgorithmCalculation();
	this->generatedObjects.push_back(new GeneratedObjects(initPos, genDataTerrain.vertexData, genDataTerrain.vertexIndices, GL_STATIC_DRAW, "GroundShader", 
		{ "UnderWater-low.jpg", "Sand-low.jpg", "Grass-low.jpg", "Snow-low.jpg" }, 512));

	/* ----- WATER ----- */ // [1]
	Transform* initPosWater = new Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(8.0f, 8.0f, 8.0f));
	GeneratedData genDataWater = Generator(65, -2.0f, 0.05f).DiamondSquareAlgorithmCalculation();
	this->generatedObjects.push_back(new GeneratedObjects(initPosWater, genDataWater.vertexData, genDataWater.vertexIndices, GL_STATIC_DRAW, "WaterShader", { "Water.jpg" }, 1024));

	/* ----- CLOUDS ----- */ // [2]
	Transform* initPosClouds = new Transform(glm::vec3(0.0f, 70.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(8.0f, 8.0f, 8.0f));
	GeneratedData genDataClouds = Generator(65, -2.0f, 0.001f).DiamondSquareAlgorithmCalculation();
	std::vector<glm::vec3> genDataCloudsTexture = Generator(17, -4.0f, 0.0f).PerlinNoise();

	/* ----- TREES ----- */ // [>=3]
	GeneratedData genDataTrees = Generator(genDataTerrain.vertexData, 1.0f).LSystemPGTrees(3);
	for (unsigned int i = 0; i < genDataTerrain.vertexData.size(); i += 300) {
		if (genDataTerrain.vertexData[i].pos.y >= 18.0f && genDataTerrain.vertexData[i].pos.y <= 33.0f) {
			this->generatedObjects.push_back(
				new GeneratedObjects(
					new Transform(genDataTerrain.vertexData[i].pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
					genDataTrees.vertexData,
					genDataTrees.vertexIndices,
					GL_STATIC_DRAW, "SkyBoxShader", { "Default.jpg" }, 16, true));
		}
	}
	
	/* ----- CLOUDS TEXTURE ----- */
	std::vector<glm::vec4> textureColour;
	for (unsigned int i = 0; i < genDataCloudsTexture.size(); i++) {
		textureColour.push_back(glm::vec4(genDataCloudsTexture[i].y, 0.0f, 0.0f, 1.0f));
	}

	std::vector<void*> cloudTextureData = { textureColour.data() };
	this->generatedObjects.push_back(new GeneratedObjects(initPosClouds, genDataClouds.vertexData, genDataClouds.vertexIndices, GL_STATIC_DRAW, cloudTextureData, "CloudsShader", 16));

	/* ----- SKYBOX ----- */
	this->windowSkyBox = new SkyBox(500);

	// Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(this->m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(this->m_window) == 0)
	{
		// Check every frame if the window is focused
		this->FocusedWindow();

		if (this->m_windowFocusedState) {
			// Clear the window every frame
			this->ClearWindow();

			// Render what is on the window and update the instances of all the subsystems
			this->UpdateWindow();
		}

		// Check every frame if there have been any events
		glfwPollEvents();
	}
}

void WindowDisplay::ResizeWindow()
{
	glfwGetFramebufferSize(this->m_window, &this->m_widthScreen, &this->m_heightScreen);
	glViewport(0, 0, this->m_widthScreen, this->m_heightScreen);

	// If the window size is seen as being 0, don't change the aspect ratio unless initialised
	if(this->m_heightScreen > 0 || this->m_widthScreen > 0)
		Camera::s_camera->SetProjection(glm::perspective(70.0f, (float)this->m_widthScreen / (float)this->m_heightScreen, 0.01f, 1000.0f));
}

void WindowDisplay::FocusedWindow() {
	if (this->m_windowFocusedState != glfwGetWindowAttrib(this->m_window, GLFW_FOCUSED))
		this->m_windowFocusedState = glfwGetWindowAttrib(this->m_window, GLFW_FOCUSED);
}

void WindowDisplay::ClearWindow()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);
}