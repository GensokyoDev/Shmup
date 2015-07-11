#include <iostream>	

#include "Window.h"
#include "FrameCounter.h"
#include "EventHandler.h"
#include "InputHandler.h"
#include "ArtManager.h"
#include "Stage.h"
#include "Reimu.h"

#define DEBUG 1

#if DEBUG
	#define LOG(x) std::cout << x << std::endl
#else
	#define LOG(x)
#endif

// Shader macro
#define GLSL(src) "#version 330 core\n" #src

extern const int fps = 60;
extern bool running = true;

enum Gamestates { GAME, MENU };
enum Character { REIMU, MARISA, SANAE };
/*
int main()
{

	Window window(1024, 768, "muh touhou clone");
	window.init();
	window.setFramerateLimit(fps);
	window.setKeyRepeatEnabled(false);
	
	EventHandler eventHandler;

	FrameCounter frameCounter;

	InputHandler inputHandler;

	ArtManager artManager; //load texture and then separate functions to get separate sprites (to not load textures multiple times)
	artManager.loadTexture("Resources/images/playercharacters/2husprites.png");
	sf::Sprite* characterSprite = artManager.getSprite(0, 0, 26, 44);

	Stage stage;

	Reimu reimu;
	reimu.setSprite(characterSprite);
	reimu.setPosition(200, 200);

	int gameState = GAME; //define states
	while (running) {	
		frameCounter.incrementFrames();
		if (frameCounter.getFrames() % fps == 0) {
			LOG("current fps = " << frameCounter.getFPS()/fps << " Intended fps = " << fps);
		}
		frameCounter.restartClock();

		stage.update();

		// Handle all possible events		
		eventHandler.handleEvents(window.getWindow());
		inputHandler.handleInputs(gameState, eventHandler.getKeys(), reimu); //generalize for all characters

		window.clear();
		window.draw(*characterSprite);
		window.display();
	}
	return 0;
}*/

// Header files
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <iostream>
#include <ctime>
#include <chrono>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* using sfml to load  images:
#include <sfml graphics.hpp="">

sf::Image image;
image.loadFromFile("myImage.jpg");

glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
*/

// Shader sources
const GLchar* sceneVertexSource = GLSL(
	in vec3 position;
in vec3 color;
in vec2 texcoord;
out vec3 Color;
out vec2 Texcoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 overrideColor;
void main() {
	Color = overrideColor * color;
	Texcoord = texcoord;
	gl_Position = proj * view * model * vec4(position, 1.0);
}
);

const GLchar* sceneFragmentSource = GLSL(
	in vec3 Color;
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texKitten;
uniform sampler2D texPuppy;
uniform float time;
//"uniform vec3 triangleColor;"
void main() {
	//"	outColor = texture(texPuppy, Texcoord) * vec4(Color, 1.0);"

	float factor = (sin(time * 1.0) + 1.0) / 2.0;
	//"	outColor = mix(texture(texKitten, Texcoord), texture(texPuppy, Texcoord), factor);"

	//"	if(Texcoord.y < 0.5)"
	//"		outColor = texture(texKitten, Texcoord);"
	//"	else"
	//"		outColor = texture(texKitten, vec2(Texcoord.x + sin(Texcoord.y * 60.0 + time * 2.0) / 60.0, 1.0 - Texcoord.y)) * vec4(0.7, 0.7, 1.0, 1.0);"
	//"	outColor =  texture(texKitten, vec2(Texcoord.x, 1-Texcoord.y));"

	outColor = vec4(Color, 1.0) * mix(texture(texKitten, Texcoord), texture(texPuppy, Texcoord), factor);
}
);

const GLchar* screenVertexSource = GLSL(
	in vec2 position;
in vec2 texcoord;
out vec2 Texcoord;
void main() {
	Texcoord = texcoord;
	gl_Position = vec4(position, 0.0, 1.0);
}
);

const GLchar* screenFragmentSource = GLSL(
	const float blurSizeH = 1.0 / 300.0;
const float blurSizeV = 1.0 / 200.0;
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;
void main() {
	outColor = texture(texFramebuffer, Texcoord);
	// inverse colors: 
	//vec4(1.0, 1.0, 1.0, 1.0) - texture
	//grayscale:
	//"float avg = (outColor.r + outColor.g + outColor.b) / 3.0;"
	//"outColor = vec4(avg, avg, avg, 1.0);"
	//grayscale weighted:
	//"	float avg = 0.2126 * outColor.r + 0.7152 * outColor.g + 0.0722 * outColor.b;"
	//"	outColor = vec4(avg, avg, avg, 1.0); "
	//box blur (9x9):
	//"	vec4 sum = vec4(0.0);"
	//"	for (int x = -4; x <= 4; x++)"
	//"		for (int y = -4; y <= 4; y++)"
	//"			sum += texture("
	//"			texFramebuffer,"
	//"			vec2(Texcoord.x + x * blurSizeH, Texcoord.y + y * blurSizeV)"
	//"			) / 81.0;"
	//"	outColor = sum;"
	//Sobel:
	//"vec4 s1 = texture(texFramebuffer, Texcoord - 1.0 / 300.0 - 1.0 / 200.0);"
	//"vec4 s2 = texture(texFramebuffer, Texcoord + 1.0 / 300.0 - 1.0 / 200.0);"
	//"vec4 s3 = texture(texFramebuffer, Texcoord - 1.0 / 300.0 + 1.0 / 200.0);"
	//"vec4 s4 = texture(texFramebuffer, Texcoord + 1.0 / 300.0 + 1.0 / 200.0);"
	//"vec4 sx = 4.0 * ((s4 + s3) - (s2 + s1));"
	//"vec4 sy = 4.0 * ((s2 + s4) - (s1 + s3));"
	//"vec4 sobel = sqrt(sx * sx + sy * sy);"
	//"outColor = sobel;"
}
);

// Cube vertices
GLfloat cubeVertices[] = {
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

	-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};

// Quad vertices
GLfloat quadVertices[] = {
	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,

	1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 1.0f
};

// Create a texture from an image file
GLuint loadTexture(const GLchar* path)
{
	GLuint texture;
	glGenTextures(1, &texture);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_2D, texture);
	image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

// Shader creation helper
GLuint createShader(GLenum type, const GLchar* src) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	return shader;
}

void createShaderProgram(const GLchar* vertSrc, const GLchar* fragSrc, GLuint& vertexShader, GLuint& fragmentShader, GLuint& shaderProgram)
{
	// Create and compile the vertex shader
	vertexShader = createShader(GL_VERTEX_SHADER, vertSrc);
	// Create and compile the fragment shader
	fragmentShader = createShader(GL_FRAGMENT_SHADER, fragSrc);

	// Link the vertex and fragment shader into a shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
}

void specifySceneVertexAttributes(GLuint shaderProgram)
{
	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
}

void specifyScreenVertexAttributes(GLuint shaderProgram)
{
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}

// Initialize OpenGL
//glEnable(GL_DEPTH_TEST);
int main()
{
	auto t_start = std::chrono::high_resolution_clock::now();

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 1; // Optional

	sf::Window window(sf::VideoMode(1024, 768, 32), "OpenGL tut", sf::Style::Titlebar | sf::Style::Close, settings);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Create Vertex Array Object (to hold VBOs)
	GLuint vaoCube, vaoQuad;
	glGenVertexArrays(1, &vaoCube);
	glGenVertexArrays(1, &vaoQuad);

	// Load vertex data
	GLuint vboCube, vboQuad;
	glGenBuffers(1, &vboCube);
	glGenBuffers(1, &vboQuad);

	glBindBuffer(GL_ARRAY_BUFFER, vboCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	GLuint sceneVertexShader, sceneFragmentShader, sceneShaderProgram;
	createShaderProgram(sceneVertexSource, sceneFragmentSource, sceneVertexShader, sceneFragmentShader, sceneShaderProgram);

	GLuint screenVertexShader, screenFragmentShader, screenShaderProgram;
	createShaderProgram(screenVertexSource, screenFragmentSource, screenVertexShader, screenFragmentShader, screenShaderProgram);

	// Specify the layout of the vertex data

	glBindVertexArray(vaoCube);
	glBindBuffer(GL_ARRAY_BUFFER, vboCube);
	specifySceneVertexAttributes(sceneShaderProgram);

	glBindVertexArray(vaoQuad);
	glBindBuffer(GL_ARRAY_BUFFER, vboQuad);
	specifyScreenVertexAttributes(screenShaderProgram);

	//	glActiveTexture(GL_TEXTURE0);
	// 	glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);
	// Load textures

	GLuint texKitten = loadTexture("Resources/images/playercharacters/sample.png");
	GLuint texPuppy = loadTexture("Resources/images/playercharacters/sample2.png");

	/*GLfloat vertices[] = {
	// X      Y     Z     R     G     B     U     V
	-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};*/


	/*GLfloat vertices[] = {
	//  Position   Color             Texcoords
	-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
	-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
	};*/

	// Create an element array
	/*GLuint ebo;
	glGenBuffers(1, &ebo);

	GLuint elements[] = {
	0, 1, 2,
	2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	*/

	glUseProgram(sceneShaderProgram);
	glUniform1i(glGetUniformLocation(sceneShaderProgram, "texKitten"), 0);
	glUniform1i(glGetUniformLocation(sceneShaderProgram, "texPuppy"), 1);

	glUseProgram(screenShaderProgram);
	glUniform1i(glGetUniformLocation(screenShaderProgram, "texFramebuffer"), 0);

	GLint uniModel = glGetUniformLocation(sceneShaderProgram, "model");

	// Create frame buffer
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// Create texture to hold color buffer
	GLuint texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	// Create Renderbuffer Object to hold depth and stencil buffers
	GLuint rboDepthStencil;
	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 768);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);

	// Set up projection
	glm::mat4 view = glm::lookAt(
		glm::vec3(2.5f, 2.5f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
		);

	GLint uniView = glGetUniformLocation(sceneShaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 1.0f, 10.0f);
	GLint uniProj = glGetUniformLocation(sceneShaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	// glGenerateMipmap(GL_TEXTURE_2D); ?

	// Get the location of the color uniform
	// GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");

	GLint uniColor = glGetUniformLocation(sceneShaderProgram, "overrideColor");
	GLint uniTime = glGetUniformLocation(sceneShaderProgram, "time");

	//GLfloat angle = -45.0f;
	//GLfloat speed = 0.0f;

	while (running)
	{
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				running = false;
				break;
			case sf::Event::KeyPressed:
				if (windowEvent.key.code == sf::Keyboard::Escape)
				{
					running = false;
					break;
				}
				else if (windowEvent.key.code == sf::Keyboard::Space)
				{
					std::cout << "hello";
					//speed = 180.0f;
				}
				break;
			}
		}

		/*// Render frame
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_POINTS, 0, 4);

		window.display();*/

		//glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

		// Bind our framebuffer and draw 3D scene (spinning cube)
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glBindVertexArray(vaoCube);
		glEnable(GL_DEPTH_TEST);
		glUseProgram(sceneShaderProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texKitten);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texPuppy);

		// CLear the screen to white
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set the time uniform    or		// Set the color of the triangle
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

		// Calculate transformation
		glm::mat4 model;
		model = glm::rotate(
			model,
			time / 3 * glm::radians(180.0f), // * time for rotatoes // * angle for variable angle
			glm::vec3(0.0f, 0.0f, 1.0f)
			);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

		//angle += speed / 60;//(GLfloat)CLOCKS_PER_SEC;
		//speed /= 1.0f + 0.2f / 60;// / (GLfloat)CLOCKS_PER_SEC;

		//GLfloat s = sin(time * 5.0f) * 0.25f + 0.75f;
		//model = glm::scale(model, glm::vec3(s, s, s));

		glUniform1f(uniTime, time);

		// Draw
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Draw cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glEnable(GL_STENCIL_TEST);

		// draw floor
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF); // Write to stencil buffer
		glDepthMask(GL_FALSE); // Don't write to depth buffer
		glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer, 0 by default

		glDrawArrays(GL_TRIANGLES, 36, 6);

		// Draw cube reflection
		glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
		glStencilMask(0x00); // Don't write anything to the stencil buffer
		glDepthMask(GL_TRUE); // Write to depth buffer

		model = glm::scale(
			glm::translate(model, glm::vec3(0, 0, -1)),
			glm::vec3(1, 1, -1)
			);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniform3f(uniColor, 0.3f, 0.3f, 0.3f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);

		glDisable(GL_STENCIL_TEST);

		// Bind default framebuffer and draw contents of our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(vaoQuad);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(screenShaderProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Swap buffers
		window.display();
	}
end:

	glDeleteRenderbuffers(1, &rboDepthStencil);
	glDeleteTextures(1, &texColorBuffer);
	glDeleteFramebuffers(1, &frameBuffer);

	glDeleteTextures(1, &texKitten);
	glDeleteTextures(1, &texPuppy);

	glDeleteProgram(screenShaderProgram);
	glDeleteShader(screenFragmentShader);
	glDeleteShader(screenVertexShader);

	glDeleteProgram(sceneShaderProgram);
	glDeleteShader(sceneFragmentShader);
	glDeleteShader(sceneVertexShader);

	//glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vboCube);
	glDeleteBuffers(1, &vboQuad);

	glDeleteVertexArrays(1, &vaoCube);
	glDeleteVertexArrays(1, &vaoQuad);

	return 0;
}