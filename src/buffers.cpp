#include "buffers.h"


Buffer::Buffer(void* data, unsigned int size)
{
	this->initialize_buffer_(data, size);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &Object_ID_);
}


void Buffer::Bind(GLenum bufferType)
{
	glBindBuffer(GL_ARRAY_BUFFER, Object_ID_);
}

void Buffer::unBind(GLenum bufferType)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


Element_buffer::Element_buffer(unsigned int* data, unsigned int size)
{
	
	glGenBuffers(1, &Object_ID_);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Object_ID_);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

Element_buffer::~Element_buffer()
{
	if(Object_ID_ != 0)	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Object_ID_);	
	glDeleteBuffers(1, &Object_ID_);

}


void Element_buffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Object_ID_);
}

void Element_buffer::unBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
