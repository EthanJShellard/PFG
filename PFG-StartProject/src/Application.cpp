#include "Application.h"
#include "SceneLoader.h"
#include "glew.h"

#include "Scene.h"
#include <iostream>
#include <string>

/**
* Application class which defines the main sequences for the application (Init, Update and Exit, etc.)
* This is only an example class. There are many different ways of designing this class.
* For your physics computation, you may not need to modify this class.
* @file: Application.cpp
*/

// Define a fixed step length for stable physics simulations
static float STEP_LENGTH = 0.02;

Application::Application()
{
	/** Default parameters for SDL
	*/
	m_window = nullptr;
	m_renderer = nullptr;

	// Engine defaults
	m_running = false;

	//Timing defaults
	m_lastTime = 0;
	m_currentTime = 0;
	m_deltaTime = 0.0166666667f; // Default deltatime to 1/60 for first frame
	
}

Application::~Application()
{}


bool Application::Run()
{
	// Runs the application in the Init->Update->Exit order with error checking
	// Checks each run to ensure it returned true. If it didnt this will return false to main
	if (Init())
	{
		if (Update())
		{
			if (Exit())
			{
				// Returns true to main stating that everything ran okay and we can exit now.
				return true;
			}
		}
	}
	return false;
}

bool Application::Init()
{

	//GET SCENE TO BE LOADED
	bool found = false;
	int sceneIndex = -1;

	while (!found)
	{
		std::cout << "Please input the index of the scene you wish to load: ";

		cin >> sceneIndex;

		if (sceneIndex > 0 && sceneIndex <= 3)
		{
			found = true;
		}
		else 
		{
			std::cout << "\nInvalid Scene Index\n";
			//Reset input buffer
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cin.sync();
		}

	}
	////////////////////////


	// Init SDL with video mode
	if (SDL_Init(SDL_INIT_VIDEO) !=0)
	{
		// Failed to init SDL
		std::cerr << "Failed to init SDL!\n";
		return false;
	}

	// This is how we set the context profile through SDL to setup the OpenGL drawing contxt
	// Major version number (4):
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	// Minor version number (3):
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Then we say whether we want the core profile or the compatibility profile
	// Flag options are either: SDL_GL_CONTEXT_PROFILE_CORE   or   SDL_GL_CONTEXT_PROFILE_COMPATIBILITY
	// We'll go for the core profile
	// This means we are using the latest version and cannot use the deprecated functions
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	// Create the SDL window
	m_window = SDL_CreateWindow("Physics Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	// Check the window was made okay
	if (m_window == NULL)
	{
		std::cerr << "Failed to create SDL window!\n";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Error", "Failed to create SDL Window!", NULL);
		return false;
	}

	// Create the SDL renderer
	m_renderer = SDL_CreateRenderer(m_window, -1, 0);
	// This will allow us to actually use OpenGL to draw to the window
	m_glcontext = SDL_GL_CreateContext(m_window);

	// Call our initialisation function to set up GLEW and print out some GL info to console
	if (!InitGL())
	{
		return false;
	}

	// Enable the depth test to make sure triangles in front are always in front no matter the order they are drawn
	// When you do this, don't forget to clear the depth buffer at the start of each frame - otherwise you just get an empty screen!
	glEnable(GL_DEPTH_TEST);
	//Enable multisampling to smooth jagged edges
	glEnable(GL_MULTISAMPLE);
	

	//LOAD SCENE HERE
	// The scene contains all the objects etc
	SceneLoader sl;
	std::string path = "assets/Scenes/Scene";
	path.append(std::to_string(sceneIndex)); //Amend path to fit scene index to be loaded
	path.append(".txt");

	m_myScene = sl.LoadScene(path.c_str());
	//Provide scene with performance monitor
	m_myScene->SetPerformanceMonitor(&m_performanceMonitor);

	SDL_ShowCursor(true);
	m_input = new Input();

	// Get the last time to calculate deltatime for the first runthrough.
	/* No more code should be under this line in the init function unless its
	timing related or enabling the update loop */

	m_lastTime = SDL_GetTicks();
	m_running = true;
	
	return true;
}

bool Application::InitGL()
{
	// GLEW has a problem with loading core OpenGL
	// See here: https://www.opengl.org/wiki/OpenGL_Loading_Library
	// The temporary workaround is to enable its 'experimental' features
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: GLEW failed to initialise with message: " << glewGetErrorString(err) << std::endl;
		return false;
	}
	std::cout << "INFO: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

	std::cerr << "INFO: OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cerr << "INFO: OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cerr << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cerr << "INFO: OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return true;
}

bool Application::Update()
{
	// Game loop
	while (m_running)
	{
		// Calculate deltatime
		m_currentTime = SDL_GetTicks();
		m_performanceMonitor.FrameBegin();
		m_deltaTime = (float)(m_currentTime - m_lastTime) / 1000.0f;
		m_lastTime = m_currentTime;

		// Update the scene
	    m_input->update();
		if (m_input->m_quit)
			m_running = false;

		// Update scene and physics simulation
		// Run simulation: 
		// We target a real-time simulation, say 60 fps.
		// Therefore multiple updates to achieve our goal.

		m_myScene->Update(STEP_LENGTH, m_input);
		
		// Specify the colour to clear the framebuffer to
		glClearColor(0.25f, 0.25f, 0.25f, 0.0f);
		// This writes the above colour to the colour part of the framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Draw the scene
		m_myScene->Draw();

		// This tells the renderer to actually show its contents to the screen
		SDL_GL_SwapWindow(m_window);
		
		//caps framerate
		m_lastTime = m_currentTime;		
		SDL_Delay(10);

		//Tell the performance monitor that the frame has ended
		m_performanceMonitor.FrameEnd();
	}
	//Ouput performance metrics
	m_performanceMonitor.OutputResults();

	return true;
}


bool Application::Exit()
{
	
	// Destroy everything, clean up!
	SDL_GL_DeleteContext(m_glcontext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

	return true;
}

/** STATIC IMPLEMENTS 
* The static instance variable that stores the one instance of itself
*/
Application* Application::g_instance = nullptr;

/** The Instance getter method. Returns the static instance of itself (or makes one if it doesn't yet exist
*/
Application* Application::Instance()
{
	// Check if the instance is null
	if (g_instance == nullptr)
	{
		// Instance is null, we need to make one
		g_instance = new Application();
	}

	// Return the instance of the application
	return g_instance;
}