#version 460 core 



in vec2 texCoord;

layout(location = 0) uniform sampler2D bufferB ;


out vec4 FragColor;

void main (){
  vec3 col = texture(bufferB , texCoord).rgb;


  FragColor = vec4(vec3(col),1.0);
}