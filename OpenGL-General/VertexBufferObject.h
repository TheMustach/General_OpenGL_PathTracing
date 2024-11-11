

#ifndef  VERTEXBUFFEROBJECT_CLASS_H

#define VERTEXBUFFEROBJECT_CLASS_H

#include <glad/glad.h>


class VertexBufferObject
{
public:
	GLuint ID;
	VertexBufferObject(GLfloat* vertices, GLsizeiptr size);


	void bind();
	void unbind();
	void Delete();
};

#endif