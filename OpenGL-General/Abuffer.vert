#version 460 core



layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 uv;

  out vec2 texCoord;

void main() {
  
   texCoord = uv;

   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}