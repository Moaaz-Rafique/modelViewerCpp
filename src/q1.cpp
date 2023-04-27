#define GLEW_STATIC


#include "Light.h"


#include <iostream>
#include <string>
#include "object.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


GLfloat vertices[] = {
	// box vertex	
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,


	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,


	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,


	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,


	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,


	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,

};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;


int main(int argc, char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment: 4", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
	{
		std::cout << "error" << std::endl;
	}

	glfwSetErrorCallback((GLFWerrorfun)[](int error_code, const char* description) {
		fprintf(stderr, "Error: %s\n", description);
		});

	if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
	{
		std::cout << "error" << std::endl;
	}


	glfwSetErrorCallback((GLFWerrorfun)[](int error_code, const char* description) {
		fprintf(stderr, "Error: %s\n", description);
		});

	 Light pointLight;
	 
	

	//...........................................................
	glm::mat4 projection = glm::infinitePerspective(glm::radians(160.0f), 16.0f/ 9.0f, 0.1f);
	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 1.0f, 0.25f)),
		view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -8.0f));
	//.............................................................
	shaderCompilation lightShader = shaderCompilation("shaders/q1vertex.glsl", "shaders/q1fragment.glsl", "Lighting Shader"); //diffuse shader
	lightShader.Bind();

	shaderCompilation lamp = shaderCompilation("shaders/lampV.glsl", "shaders/lamp.glsl", "lamp"); //diffuse shader
	lamp.Bind();
	
	pointLight = Light(glm::vec3(1.0f), "light", 1.0f, 0.0925f, 1.0f);
	pointLight.set_position(glm::vec3(2.2f, 0.1, 2.2));


	lightShader.Bind();
	lightShader.setUniformMat4("projection", projection);

	lamp.Bind();
	lamp.setUniformMat4("projection", projection);
	lamp.setUniformMat4("model", glm::mat4(1.0f));

	float deltaTime, currentFrame, lastFrame = 0;

	Models::assimpModel backPack = Models::assimpModel(lightShader, "resources/objects/backpack/backpack.obj", "resources/objects/backpack/diffuse.jpg", "resources/objects/backpack/specular.jpg");


	float dragDistance = 5.0f;
	float zoomAngle = 90.0f;
	glm::vec3 lampPosition(2.2, 0.1, 2.2);
	glm::vec3 objectPos(0.0f, 0.0f, 0.0f);

	unsigned int lamp_array;

	glGenVertexArrays(1, &lamp_array);
	glBindVertexArray(lamp_array);

	Buffer lamp_buffer = Buffer(vertices, sizeof(vertices));
	lamp_buffer.Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	float  object_rotation = 0.0f; bool is_rotating = 0;
	float  lamp_roataion = 0.0f; bool isLamp = 0;
	while (!glfwWindowShouldClose(window))
	{
		

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			zoomAngle -= 0.5f;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			zoomAngle -= 0.5f;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			objectPos.x -= 25.0f * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			objectPos.x += 25.0f * deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		{
			lampPosition.y += 0.01f;
		}

		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			lampPosition.y -= 0.01f;
		}

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			is_rotating = true;
		}


		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{
			isLamp = true;
		}

		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		{
			is_rotating = false;
			isLamp = false;
		}


		// render
		// ------

		object_rotation -= (is_rotating ? 0.1f : 0.0f);
		lamp_roataion -= (isLamp ? 0.1f : 0.0f);

		projection = glm::infinitePerspective(glm::radians(zoomAngle), 16.0f / 9.0f, 0.1f);
		lightShader.Bind();

		model = glm::translate(glm::mat4(1.0f), objectPos) * glm::rotate(glm::mat4(1.0f), glm::radians(object_rotation), glm::vec3(0, 1, 0));

		lightShader.setUniformMat4("view", view);
		lightShader.setUniformMat4("projection", projection);
		
		
		pointLight.set_position(lampPosition);
		pointLight.set_ambient_intensity(0.000000000001f);
		pointLight.set_specular_intensity(20.0f);
		pointLight.set_view_position(glm::vec3(0.0f, 0.0f, -8.0f));
		pointLight.render(lightShader.return_object());
		pointLight.render(lightShader.return_object());


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		backPack.Draw_call(3, model);

		lamp.Bind();
		glBindVertexArray(lamp_array);
		lamp_buffer.Bind();
		lamp.setUniformMat4("projection", projection);
		lamp.setUniformMat4("view", view);

		glm::mat4 lamp_model = glm::translate(glm::mat4(1.0f), objectPos);
		lamp_model = glm::rotate(lamp_model, glm::radians(lamp_roataion), glm::vec3(0, 1, 0));
		lamp_model = glm::translate(lamp_model, lampPosition) * glm::scale(glm::mat4(1.0f), glm::vec3(0.25f));

		lamp.setUniformMat4("model", lamp_model);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//if (firstMouse)
	//{
	//    lastX = xpos;
	//    lastY = ypos;
	//    firstMouse = false;
	//}

	//float xoffset = xpos - lastX;
	//float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	//lastX = xpos;
	//lastY = ypos;

	//camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//camera.ProcessMouseScroll(yoffset);
}
