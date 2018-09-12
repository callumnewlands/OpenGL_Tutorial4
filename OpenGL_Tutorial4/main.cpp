#include "main.h"

int main()
{
	GLFWwindow* window = setupFirstWindow();
	if (window == NULL) return -1;

	std::vector<vertex> vertices = getCubeVertices();

	unsigned int VAO = getVAO(vertices);

	unsigned int shaderProgram = getShaderProgram("vertexShader.vrt", "fragmentShader.frg");

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
	
	glm::vec3 camPosition = glm::vec3(2.0f, 1.0f, -1.0f);
	glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::lookAt(camPosition, camTarget, camUp);
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");

	glm::mat4 model = glm::mat4(1.0f); //Identity matrix
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, glm::vec3(0.5));
	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");

	// Main render loop
	while (!glfwWindowShouldClose(window))
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram); // activates the shader program

		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO); // binds the triangle's VAO
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Exit OpenGL
	glfwTerminate();
	return 0;
}

// Call first to setup openGL and return a new window
GLFWwindow* setupFirstWindow()
{
	// Initilise GLFW with OpenGL version 3.3 core
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "windowTitle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "ERROR creating window";
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, onWindowResize); // Set window resize callback

	// Initialise GLEW
	// Has to be called after window is created
	GLenum errorCode = glewInit();
	if (errorCode != GLEW_OK)
	{
		std::cout << "ERROR initialising GLEW";
		glfwTerminate();
		return NULL;
	}

	return window;
}

// Window resize callback
void onWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Create a shader program from the 2 shaders passed as arguments
unsigned int getShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// shader Program
	unsigned int id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);
	checkCompileErrors(id, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return id;
}

// Check shader compilation errors
void checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

// Generate a VAO from the vertex data passed as an argument
unsigned int getVAO(std::vector<vertex> vertices)
{
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO first before the buffers it will contain
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Bind buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW); // Set buffer data

	// Configure poistion attribute #0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Configure colour attribute #1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, colour));
	glEnableVertexAttribArray(1);

	return VAO;
}

std::vector<vertex> getTriangleVertices()
{
	return { vertex(-0.5f, -0.5f, 0.0f),
			 vertex(0.5f, -0.5f, 0.0f),
			 vertex(0.0f,  0.5f, 0.0f) };
}

std::vector<vertex> getCubeVertices()
{
	return { vertex(-0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f),
			 vertex( 0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f),
			 vertex( 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f),
			 vertex( 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f),
			 vertex(-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f),
		 	 vertex(-0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f),

			 vertex(-0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 1.0f),
			 vertex( 0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 1.0f),
			 vertex( 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f),
			 vertex( 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f),
			 vertex(-0.5f,  0.5f,  0.5f,    0.5f, 0.5f, 0.5f),
			 vertex(-0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 1.0f),

			 vertex(-0.5f,  0.5f,  0.5f),
			 vertex(-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f),
			 vertex(-0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f),
			 vertex(-0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f),
			 vertex(-0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 1.0f),
			 vertex(-0.5f,  0.5f,  0.5f,    0.5f, 0.5f, 0.5f),

			 vertex( 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f),
			 vertex( 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f),
			 vertex( 0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f),
			 vertex( 0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f),
			 vertex( 0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 1.0f),
			 vertex( 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f),

			 vertex(-0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f),
			 vertex( 0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f),
			 vertex( 0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 1.0f),
			 vertex( 0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 1.0f),
			 vertex(-0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 1.0f),
			 vertex(-0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f),

			 vertex(-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f),
			 vertex( 0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f),
			 vertex( 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f),
			 vertex( 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f),
			 vertex(-0.5f,  0.5f,  0.5f,    0.5f, 0.5f, 0.5f),
			 vertex(-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f)};
}