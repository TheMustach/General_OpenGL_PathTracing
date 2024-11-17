#pragma once

#include <GLFW/glfw3.h>
#include <vector>



namespace rwindow {

	
	static int width = 800;
	static int height = 600;

	static float aspect_ratio;


	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	 	aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
  		
		glViewport(0, 0, width, height);
	}
    

}