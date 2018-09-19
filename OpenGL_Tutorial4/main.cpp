#include "main.h"

const glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

int main()
{
	initGLFW(); // Initialise GLFW
	GLFWwindow* window = getDefaultWindow();
	if (window == NULL) return -1; // Check the window was created sucessfully
	if (initGLEW() == -1) return -1; // Initialise GLEW and return -1 if there is an error

	std::vector<vertex> vertices = getCubeVertices();
	unsigned int cubeVAO = getVAO(vertices);

	shaderProgram shaders = shaderProgram("vertexShader.vrt", "fragmentShader.frg");

	unsigned int projLoc = shaders.getUniformLocation("projection");
	unsigned int viewLoc = shaders.getUniformLocation("view");
	unsigned int modelLoc = shaders.getUniformLocation("model");
	
	// VIEW matrix
	glm::vec3 camPosition = glm::vec3(2.0f, 1.0f, -1.0f);
	glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::lookAt(camPosition, camTarget, camUp);

	// MODEL matrix
	glm::mat4 model = glm::mat4(1.0f); //Identity matrix
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, glm::vec3(0.5));

	// Main render loop
	while (!glfwWindowShouldClose(window))
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		model = glm::rotate(model, glm::radians(0.03f), glm::vec3(0.0, 0.0, 1.0));

		shaders.use(); // activates the shader program - do before setting uniforms
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(cubeVAO); // binds the triangle's VAO
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Exit OpenGL
	glfwTerminate();
	return 0;
}

void initGLFW()
{
	// Initilise GLFW with OpenGL version 3.3 core
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

// Must be called after first window is created
int initGLEW()
{
	GLenum errorCode = glewInit();
	if (errorCode != GLEW_OK)
	{
		std::cout << "ERROR initialising GLEW";
		glfwTerminate();
		return -1;
	}

	return 0;
}

GLFWwindow* getDefaultWindow()
{

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

	return window;
}

// Window resize callback
void onWindowResize(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

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