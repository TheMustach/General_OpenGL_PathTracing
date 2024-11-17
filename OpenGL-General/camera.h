#pragma once


#include <vector>
#include <glm.hpp>

using namespace glm ;





namespace camera {

	const float speed = 5.0 * 0.3 ;

	static  float m_VerticalFOV = 45.0f;
	static  float m_NearClip = 0.1f;
	static  float m_FarClip = 100.0f;

	glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_ForwardDirection{ 0.0f, 0.0f, 0.0f };
    glm::vec2 m_LastMousePosition{ 0.0f, 0.0f };

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame


	class Camera
	{
	public: 
		void mouseCallback(GLFWwindow* window, double xpos, double ypos , double mouseX, double mouseY)
		{

			int screenWidth, screenHeight;
			glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

			mouseX = xpos;
			mouseY = ypos;

			// Check if the mouse position is out of bounds and wrap around
			if (mouseX < 0)
				glfwSetCursorPos(window, screenWidth - 1, mouseY);
			else if (mouseX >= screenWidth)
				glfwSetCursorPos(window, 0, mouseY);

			if (mouseY < 0)
				glfwSetCursorPos(window, mouseX, screenHeight - 1);
			else if (mouseY >= screenHeight)
				glfwSetCursorPos(window, mouseX, 0);
		}

		
	};
		

		// Cached ray directions
		std::vector<glm::vec3> m_RayDirections;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
 }