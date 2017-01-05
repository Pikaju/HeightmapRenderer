#include "Model.h"

#include <glew.h>

#include "../../debug/Debug.h"

namespace graphics {

	Model::Model() : m_vbo(0), m_ibo(0), m_vao(0), m_mode(GL_TRIANGLES), m_indexType(GL_UNSIGNED_INT), m_numIndices(0)
	{
	}
	
	Model::~Model()
	{
	}

	void Model::initialize()
	{
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);
		glGenVertexArrays(1, &m_vao);
	}

	void Model::cleanup()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ibo);
		glDeleteVertexArrays(1, &m_vao);
	}

	void Model::setData(const void* vertexData, unsigned int numVertices, const void* indexData, unsigned int numIndices, const Attribute* attributes, unsigned int numAttributes)
	{
		m_numIndices = numIndices;

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		unsigned int stride = 0;
		for (unsigned int i = 0; i < numAttributes; i++) {
			stride += getGLTypeSize(attributes[i].type) * attributes[i].amount;
		}
		glBufferData(GL_ARRAY_BUFFER, stride * numVertices, vertexData, GL_STATIC_DRAW);
		
		unsigned int pointer = 0;
		for (unsigned int i = 0; i < numAttributes; i++) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, attributes[i].amount, getGLType(attributes[i].type), attributes[i].normalized, stride, (const void*)pointer);
			pointer += getGLTypeSize(attributes[i].type) * attributes[i].amount;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_numIndices, indexData, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void Model::render() const
	{
		glBindVertexArray(m_vao);
		glDrawElements(m_mode, m_numIndices, m_indexType, nullptr);
		glBindVertexArray(0);
	}

	unsigned int Model::getGLType(const Type& type)
	{
		switch (type) {
		case TYPE_FLOAT: return GL_FLOAT; break;
		case TYPE_DOUBLE: return GL_DOUBLE; break;
		case TYPE_UNSIGNED_BYTE: return GL_UNSIGNED_BYTE; break;
		case TYPE_UNSIGNED_INT: return GL_UNSIGNED_INT; break;
		default: debug::crash("Invalid type: " + std::to_string(type)); break;
		}
		return 0;
	}
	
	unsigned int Model::getGLTypeSize(const Type& type)
	{
		switch (type) {
		case TYPE_FLOAT: return sizeof(float); break;
		case TYPE_DOUBLE: return sizeof(double); break;
		case TYPE_UNSIGNED_BYTE: return sizeof(unsigned char); break;
		case TYPE_UNSIGNED_INT: return sizeof(unsigned int); break;
		default: debug::crash("Invalid type: " + std::to_string(type)); break;
		}
		return 0;
	}

}