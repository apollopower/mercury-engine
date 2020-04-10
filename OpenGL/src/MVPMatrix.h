#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define DEFAULT glm::mat4(1.0f)

struct MVPMatrix
{
	glm::mat4 model, view, proj;

	MVPMatrix(glm::mat4 m=DEFAULT, glm::mat4 v=DEFAULT, glm::mat4 p=DEFAULT)
	{
		model = m;
		view = v;
		proj = p;
	}

	glm::mat4 matrix()
	{
		return model * view * proj;
	}

	void translate(glm::vec3 translation)
	{
		model = glm::translate(glm::mat4(1.0f), translation);
	}

};