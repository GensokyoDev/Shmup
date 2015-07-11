#include <SFML/Window.hpp>
#include <GL/glew.h>

// Shader macro
#define GLSL(src) "#version 150 core\n" #src

// Vertex shader
const GLchar* vertexShaderSrc2 = GLSL(
	in vec2 pos;
	in vec3 color;
	in float sides;

	out vec3 vColor;
	out float vSides;

	void main() {
		gl_Position = vec4(pos, 0.0, 1.0);
		vColor = color;
		vSides = sides;
	}
);

// Geometry shader
const GLchar* geometryShaderSrc = GLSL(
	layout(points) in;
	layout(line_strip, max_vertices = 64) out;

	in vec3 vColor[];
	in float vSides[];
	out vec3 fColor;

	const float PI = 3.1415926;

	void main() 
	{
		fColor = vColor[0];

		// Safem GLfloats can represent small integers exactly
		for (int i = 0; i <= vSides[0]; i++)
		{
			// Angle between each side in radians
			float ang = PI * 2.0 / vSides[0] * i;

			// Offset from center of pointer (0.3 for aspect ratio)
			vec4 offset = vec4(cos(ang) * 0.3, -sin(ang) * 0.4, 0.0, 0.0);
			gl_Position = gl_in[0].gl_Position + offset;

			EmitVertex();
		}
		
		EndPrimitive();
	}
);

// Fragment shader
const GLchar* fragmentShaderSrc = GLSL(
	in vec3 fColor;

	out vec4 outColor;
	void main() {
		outColor = vec4(fColor, 1.0);
	}
);

// Shader creation helper
GLuint createShader2(GLenum type, const GLchar* src) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	return shader;
}

int main2() {
	// Window
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;

	sf::Window window(sf::VideoMode(800, 600), "Circles", sf::Style::Titlebar | sf::Style::Close, settings);

	// Initialize GLEW
	glewInit();

	// Compile and activate shaders
	GLuint vertexShader = createShader2(GL_VERTEX_SHADER, vertexShaderSrc2);
	GLuint geometryShader = createShader2(GL_GEOMETRY_SHADER, geometryShaderSrc);
	GLuint fragmentShader = createShader2(GL_FRAGMENT_SHADER, fragmentShaderSrc);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Create VBO with point coordinates
	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLfloat points[] = {
		//  Coordinates  Color             Sides
		-0.45f, 0.45f, 1.0f, 0.0f, 0.0f, 4.0f,
		0.45f, 0.45f, 0.0f, 1.0f, 0.0f, 8.0f,
		0.45f, -0.45f, 0.0f, 0.0f, 1.0f, 16.0f,
		-0.45f, -0.45f, 1.0f, 1.0f, 0.0f, 32.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Create VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Specify layout of point data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "pos");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	GLint sidesAttrib = glGetAttribLocation(shaderProgram, "sides");
	glEnableVertexAttribArray(sidesAttrib);
	glVertexAttribPointer(sidesAttrib, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

	// Render loop
	while (window.isOpen()) {
		// Handle events
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
				break;
			}
		}

		// Render frame
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_POINTS, 0, 4);

		window.display();
	}

	return 0;
}