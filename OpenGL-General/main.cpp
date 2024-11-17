//بِسْمِ ٱللَّٰهِ ٱلرَّحْمَٰنِ ٱلرَّحِيمِ //

/*Includes*/
#include <iostream> 
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include "glm.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
  #include <glm/gtc/matrix_transform.hpp>
  #include <glm/gtc/quaternion.hpp>
  #include <glm/gtx/quaternion.hpp>

/*Local-Header_Files*/

#include "texture.h"
#include "rWindow.h"
#include "shaderClass.h"
#include "coord.h"
#include "camera.h"
#include "render.h"

   using namespace rwindow;
   using namespace texCoord;
   using namespace camera;

// Screen dimensions
const int screenWidth = 1920;
const int screenHeight = 1080;

float screen_aspectRatio = 16 / 9;

int main() {

	//Start ! 

	glfwInit();
	
	GLFWwindow* window;

	//OpenGL 4.6 is Being used//
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  ///Major #Version 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);  ///Minor #Version 4.6 // 4.0::0.6 //
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
	Shader framebufferProgram("Abuffer.vert", "framebufferA.frag");
	

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

		// Create Frame Buffer Object
		unsigned int FBO;
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// Create Frame buffer Texture
		GLuint framebufferTexture;
		glGenTextures(1, &framebufferTexture);
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding


		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

		///Color buffer for PostProccessing ?//
		unsigned int fragmentBufferColor;

		glGenTextures(1, &fragmentBufferColor);
		glBindTexture(GL_TEXTURE_2D, fragmentBufferColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Create Render Buffer Object
		unsigned int RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


		// Error checking frame buffer
		auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Frame buffer error: " << fboStatus << std::endl;


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

			glBindFramebuffer(GL_FRAMEBUFFER, framebufferTexture);

			glClearColor(1,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			
	
			shaderProgram.Activate();
			glBindImageTexture(0, framebufferTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
			glDispatchCompute(width / 16, height / 16, 1); // Or render if using fragment shader


			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

			//framebufferProgram.Activate();

	

			glUniform1i(glGetUniformLocation(framebufferProgram.ID, "bufferB"), 0);
			glActiveTexture(GL_TEXTURE0);

			glBindTexture(GL_TEXTURE_2D, framebufferTexture);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			frameCount++;

			//Uniform always true // if is only for Group purpose 
			if (true){
			///Window Function ?/ 
			bool firstMouse = true;
			bool isClicked = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);


			if (isClicked == true) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}


			//Uniform Data Fetching //
			float currentTime = static_cast<float>(glfwGetTime()); // Time in seconds

			deltaTime = currentTime - lastFrame;
			lastFrame = currentTime;

			int timeLoc = glGetUniformLocation(shaderProgram.ID, "time");


			int rwidth, rheight;
			glfwGetFramebufferSize(window, &rwidth, &rheight);

			int resolutionLoc = glGetUniformLocation(shaderProgram.ID, "resolution");

			glUniform2f(resolutionLoc, static_cast<float>(rwidth), static_cast<float>(rheight));
			glUniform1f(timeLoc, currentTime);



			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);
			mouseX = mouseX - rwidth / 2;
			mouseY = rheight / 2 - mouseY;



			int isMouseClicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ? 1 : 0;
			int isRightMouseClicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS ? 1 : 0;

			int mouseUnifPos = glGetUniformLocation(shaderProgram.ID, "mouse");

			glUniform4f(mouseUnifPos, mouseX, mouseY, isMouseClicked, isRightMouseClicked);

			int frameLoc = glGetUniformLocation(shaderProgram.ID, "frame");
			glUniform1i(frameLoc, frameCount);

			glUniform1i(glGetUniformLocation(shaderProgram.ID, "renderSample"), renderImage::workspaceRender::sample);
			glUniform1i(glGetUniformLocation(shaderProgram.ID, "eyeRenderBouncesTotality"), renderImage::workspaceRender::eyeRayTotalityBounce);
			/*Camera_Functionallity_struction*/


			int threshold = 60; //pixels 

			static glm::vec2 mousePosition;

			glm::vec2 chachedPosition = m_LastMousePosition;

			if (isClicked == true) {


				mousePosition = glm::vec2(mouseX, mouseY);

				chachedPosition = mousePosition;



			}
			//mousePosition -= chachedPosition;


			glm::vec2 deltaMousePos = (mousePosition) * 0.002f;






			constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
			glm::vec3 rightDirection = glm::cross(glm::vec3(0, 0, 1), upDirection);


			float ts = currentTime / 100.0f;
			ts = glm::clamp(ts, 0.0f, speed / 100.0f);



			float pitchDelta = deltaMousePos.y * speed;
			float yawDelta = deltaMousePos.x * speed;

			glm::vec3 direction;


			direction.x = cos(yawDelta) * cos(pitchDelta);
			direction.y = sin(pitchDelta);
			direction.z = sin(yawDelta) * cos(pitchDelta);
			direction = glm::normalize(direction);

			glm::vec3 RelativerightDirection = glm::cross(direction, upDirection);

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { m_Position += direction * ts * speed; }   /// W Forward 
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { m_Position -= direction * ts * speed; }   /// S Backward 
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { m_Position += RelativerightDirection * ts * speed; }  /// D Right
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { m_Position -= RelativerightDirection * ts * speed; }   /// A Left

			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { m_Position += upDirection * ts * speed; }   /// E Upward
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { m_Position -= upDirection * ts * speed; }   /// Q DownWard


			glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection),
				glm::angleAxis(-yawDelta, glm::vec3(0.f, 1.0f, 0.0f))));


			m_ForwardDirection = glm::rotate(q, m_ForwardDirection);


			glm::mat4 m_Projection = glm::perspectiveFov(glm::radians(m_VerticalFOV), (float)rwidth, (float)rheight, m_NearClip, m_FarClip);

			glm::mat4 m_View = glm::lookAt(glm::vec3(0), glm::vec3(0) + direction, upDirection);

			int ViewLoc = glGetUniformLocation(shaderProgram.ID, "view_Matrix");
			int ProjectLoc = glGetUniformLocation(shaderProgram.ID, "projection_Matrix");
			int CameraPosLoc = glGetUniformLocation(shaderProgram.ID, "cameraPosition");


			glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, glm::value_ptr(m_View));
			glUniformMatrix4fv(ProjectLoc, 1, GL_FALSE, glm::value_ptr(m_Projection));
			glUniform3f(CameraPosLoc, m_Position.x, m_Position.y, m_Position.z);

		}    
		//buffer*/
		

		noisetex.Bind();  //Didn't work :( 


		
		//Continue Draw Buffer From Index ///
		
		glDisable(GL_DEPTH_TEST);
	
		//glDrawArrays(GL_TRIANGLES, 0, 0);


		glBindVertexArray(VertexBufferOriantation);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VertexBufferOriantation);
	glDeleteBuffers(1, &VertexBufferOriantation);
	shaderProgram.Delete();
	framebufferProgram.Delete();
	glDeleteFramebuffers(1, &FBO);
	noisetex.Delete();
	
	glfwDestroyWindow(window);

  
	glfwTerminate();
	return 0;
}
