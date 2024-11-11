//بِسْمِ ٱللَّٰهِ ٱلرَّحْمَٰنِ ٱلرَّحِيمِ //


#include <iostream> 
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>


#include "texture.h"
#include "rWindow.h"
#include "shaderClass.h"
#include "coord.h"

using namespace rwindow;
using namespace texCoord;

int main() {



	glfwInit();
	GLFWwindow* window;

	

	//OpenGL 4.6 is Being used//
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  ///Major #Version 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);  ///Minor #Version 4.6 // ::0.6 //
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Package identifier //

	  
	window = glfwCreateWindow(width,height, "Path-Tracer By Mohammad [DirectX 12]", NULL, NULL);
	if (window == NULL)
	{

		std::cout << "Error_Render" << std::endl;
		glfwTerminate();

		return -1;
	}
	glfwMakeContextCurrent(window);
	
    
	gladLoadGL();
	
//	glViewport(0, 0, width, height);  /// FrameBuffer Updates the the Window Dimantion/Resolution //

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	Shader shaderProgram("Abuffer.vert", "Abuffer.frag");
 
	//Screen Draw Data //
		// Vertices coordinates
		GLfloat vertices[] =
		{
			// positions         // texture_Coords
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f, // top left
			 1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // top right
			 1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom right
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f  // bottom left
		};

		GLuint indices[] = {
	    0, 1, 2, // First triangle
	    2, 3, 0  // Second triangle
		                           };

		//vertex Store Buffer Oriantation// !

			//VertexBufferOriantation = is the same as = VertexBufferObject //
		GLuint VertexBufferOriantation, VAO, EBO; //VBO
		if (true) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VertexBufferOriantation);
		glGenBuffers(1, &EBO);


		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferOriantation);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Bind and set EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);

		///texture layout ///


		//std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
		//std::string texPath = "/";
		Texture noisetex("T_Noise_Spots.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
		noisetex.texUnit(shaderProgram, "tex0", 0);

		///Intialize//
		int frameCount = 0;

	/// Final ///

	while (!glfwWindowShouldClose(window))
	{



		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		

		shaderProgram.Activate();

		frameCount++;

		///Window Function ?/ 
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}


    //Uniform Data Fetching //
		float currentTime = static_cast<float>(glfwGetTime()); // Time in seconds
		int timeLoc = glGetUniformLocation(shaderProgram.ID, "time");

		int rwidth, rheight;
		glfwGetFramebufferSize(window, &rwidth, &rheight);

		int resolutionLoc = glGetUniformLocation(shaderProgram.ID, "resolution");

		glUniform2f(resolutionLoc, static_cast<float>(rwidth), static_cast<float>(rheight));
		glUniform1f(timeLoc, currentTime);
		


		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		mouseY = rheight - mouseY;
		int isMouseClicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? 1 : 0;

		int mouseUnifPos = glGetUniformLocation(shaderProgram.ID, "mouse");
		 
		glUniform3f(mouseUnifPos, mouseX, mouseY, isMouseClicked);

		int frameLoc = glGetUniformLocation(shaderProgram.ID, "frame");
		glUniform1i(frameLoc, frameCount);





		noisetex.Bind();

		//Continue Draw Buffer From Index ///
		glBindVertexArray(VertexBufferOriantation);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VertexBufferOriantation);
	glDeleteBuffers(1, &VertexBufferOriantation);
	shaderProgram.Delete();

	noisetex.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
