#include "Renderer.h"

#include <glew.h>

namespace graphics {

	void Renderer::initialize()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::cleanup()
	{

	}

}