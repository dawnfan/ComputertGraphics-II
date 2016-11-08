#pragma once

#include <iostream>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "bezier.h"

bool IS_ROTATE = true;
GLfloat ROTATION = 0.0f;
GLfloat SCALAR = 0.0f;
GLfloat TRANSITION_X = 0.0f;
GLfloat TRANSITION_Y = 0.0f;


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	Shader ourShader("shaders/default.vs", "shaders/default.frag");

	// bezier
	bezier bezier_in;
	std::vector<MyVertex>* vertices = bezier_in.getBezierPoints();
	int* indices = bezier_in.getBezierEdges();
	int faces_num = bezier_in.getEdgesNum();
	// for bezier points
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(MyVertex), vertices->data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces_num * 3 * sizeof(float), indices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO

	// for control points
	std::vector<MyVertex>* ctrl_vertices = bezier_in.getControlPoints();
	int* ctrl_indices = bezier_in.getCtrlEdges();
	int ctrl_faces_num = bezier_in.getCtrlEdgesNum();
	GLuint VBO1, VAO1, EBO1;
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &EBO1);

	glBindVertexArray(VAO1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, ctrl_vertices->size() * sizeof(MyVertex), ctrl_vertices->data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ctrl_faces_num * 3 * sizeof(float), ctrl_indices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate shader
		ourShader.Use();

		// Create transformations
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		if (IS_ROTATE) {
			//ROTATION = (GLfloat)glfwGetTime() * 1.0f;
			ROTATION += 0.0003f;
		}
		model = glm::rotate(model, ROTATION, glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f+TRANSITION_X, -0.1f+TRANSITION_Y, -3.5f+SCALAR));
		// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get their uniform location
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint vertexColorLoc = glGetUniformLocation(ourShader.Program, "ourColor");
		// Pass them to the shaders
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Draw bazier
		glUniform4f(vertexColorLoc, 1.0f, 0.5f, 0.3f, 1.0f);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, faces_num*3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// Draw controller
		glUniform4f(vertexColorLoc, 0.0f, 0.8f, 0.0f, 1.0f);
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, ctrl_faces_num * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &EBO1);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case  GLFW_KEY_SPACE:
			IS_ROTATE = !IS_ROTATE;
			std::cout <<"Is rotating? "<< !IS_ROTATE << std::endl;
			break;
		case GLFW_KEY_UP:
			TRANSITION_Y += 0.1f;
			std::cout << "Current transition_y: " << TRANSITION_Y << std::endl;
			break;
		case GLFW_KEY_DOWN:
			TRANSITION_Y -= 0.1f;
			std::cout << "Current transition_y: " << TRANSITION_Y << std::endl;
			break;
		case GLFW_KEY_RIGHT:
			TRANSITION_X += 0.1f;
			std::cout << "Current transition_x: " << TRANSITION_X << std::endl;
			break;
		case GLFW_KEY_LEFT:
			TRANSITION_X -= 0.1f;
			std::cout << "Current transition_x: " << TRANSITION_X << std::endl;
			break;
		default:
			break;
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	SCALAR += 0.5f*yoffset;
	std::cout << "Current scalar: " << SCALAR << std::endl;
}