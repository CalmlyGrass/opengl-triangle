#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>
#include<string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

std::string readFile(const char* filePath);
GLuint loadShader(const char* vertex_path, const char* fragment_path);

int main() {
	//Initialize glfw
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create window and check for error
	GLFWwindow* window = glfwCreateWindow(800, 800, "Hello Triangle by @Pieceofsoil", NULL, NULL);
	if (!window) {
		std::cerr << "ERROR: Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	std::cout << "GLFW window has been created successfully\n";

	glfwMakeContextCurrent(window);

	//initialize glad
	gladLoadGL();

	glViewport(0, 0, 800, 800);

	//// Prepare data ////

	GLfloat vertices[] = { // Our triangle vertices
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	GLuint shaderProgram = loadShader("src/shaders/basicVertexShader.glsl", "src/shaders/basicFragmentShader.glsl");

	GLuint vao, vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glClearColor(0.015, 0.172, 0.384, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	//// Main loop ////

	float previousTime = glfwGetTime();
	int frameCount = 0;
	while (!glfwWindowShouldClose(window))
	{
		// Log fps
		float currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - previousTime >= 1) {
			std::cout << "Fps: " << frameCount << '\n';

			frameCount = 0;
			previousTime = currentTime;
		}

		glClearColor(0.015, 0.172, 0.384, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Finally draw our triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Delete Gl objects at the end
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

GLuint loadShader(const char* vertex_path, const char* fragment_path) {
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shaders
	std::string vertShaderStr = readFile(vertex_path);
	std::string fragShaderStr = readFile(fragment_path);
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	// Compile vertex shader
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);

	// Compile fragment shader
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	// Check is program was linked successfully
	char log[512];
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		glGetProgramInfoLog(program, 512, NULL, log);
		std::cerr << "ERROR: Failed to link shader program " << &program << ". Check log for more information: \n" << log << "\n";
		return -1;
	}
	std::cout << "Program " << &program << " has been successfully linked.\n";
	return program;
}

std::string readFile(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "ERROR: Could not read file " << filePath << ". File does not exist.\n";
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}
