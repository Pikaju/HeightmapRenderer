#pragma once

namespace graphics {

	enum Type
	{
		TYPE_FLOAT, TYPE_DOUBLE, TYPE_UNSIGNED_BYTE, TYPE_UNSIGNED_INT
	};

	class Attribute
	{
	public:
		Type type;
		unsigned int amount;
		bool normalized;

		Attribute(const Type& type, unsigned int amount, bool normalized = false) : type(type), amount(amount), normalized(normalized) {  }
		~Attribute() {  }
	};

	class Model
	{
	public:
		Model();
		~Model();

		void initialize();
		void cleanup();

		void setData(const void* vertexData, unsigned int numVertices, const void* indexData, unsigned int numIndices, const Attribute* attributes, unsigned int numAttributes);

		void render() const;
	protected:
		static unsigned int getGLType(const Type& type);
		static unsigned int getGLTypeSize(const Type& type);
	private:
		unsigned int m_vbo;
		unsigned int m_ibo;
		unsigned int m_vao;

		unsigned int m_mode;
		unsigned int m_indexType;
		unsigned int m_numIndices;
	};

}