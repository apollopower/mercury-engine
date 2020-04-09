#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"


// This matrix struct needs to be abstracted later...
struct MVPMatrix
{
	glm::mat4 model, view, proj;
	
	MVPMatrix(glm::mat4 m, glm::mat4 v, glm::mat4 p)
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


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	/* Create a windowed mode window and its OpenGL context */
	// Default window dimensions: 640, 480
	window = glfwCreateWindow(1024, 768, "Mercury Engine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		// First 2 floats represent COORDINATES of vertices
		// Last 2 floats are 'projection' coordinates for where texture starts/stops
		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, // 0
			 0.5f, -0.5f, 1.0f, 0.0f, // 1
			 0.5f,  0.5f, 1.0f, 1.0f, // 2
			-0.5f,  0.5f, 0.0f, 1.0f, // 3
		};

		float positionsOLD[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, // 0
			 1.0f, -1.0f, 1.0f, 0.0f, // 1
			 1.0f,  1.0f, 1.0f, 1.0f, // 2
			-1.0f,  1.0f, 0.0f, 1.0f, // 3
		};
		 
		// index buffers representing the index of each position
		// (currently to draw a sqaure AKA 2 triangles)
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// Blending to be refactored...
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2); // for the texture 'coordinates' (q for self: why cant we just do one Push<float>(4)???)
		va.AddBuffer(vb, layout);

		// index buffers, in order to 'reuse' positions and not duplicate them in memory
		IndexBuffer ib(indices, 6);

		// MVP Matrix
		struct MVPMatrix mvp = MVPMatrix(
			// Model (the object)
			glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)),
			// View (the 'camera')
			glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)),
			// Projection (the normalization of vertices on screen)
			glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f)
		);

		struct MVPMatrix mvp2 = MVPMatrix(
			// Model (the object)
			glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0)),
			// View (the 'camera')
			glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)),
			// Projection (the normalization of vertices on screen)
			glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f)
		);


		//~~~HARD CODED MATRIX VALUES~~~
		//
		// projection matrix
		//glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

		// view matrix
		//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		// model matrix
		//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.5, 0));

		// MVP matrix
		//glm::mat4 mvp = model * view * proj;
		//
		//~~~HARD CODED MATRIX VALUES~~~


		// parsing and loading in our shader functions from the shader file provided
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		

		Texture texture("res/textures/id_logo.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;


		// ImGUI (OpenGL GUI framework)
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		// some sample values to test ImGUI
		glm::vec3 transA(0, 0, 0);
		glm::vec3 transB(0.5, 0.5, 0);


		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();


			ImGui_ImplGlfwGL3_NewFrame();


			va.Bind();
			ib.Bind(); 
			shader.Bind();
			shader.SetUniformMat4f("u_MVP", mvp.matrix());

			// Rendering Pipeline that does actual draw calls
			renderer.Draw(va, ib, shader);	

			// Rendering another image... (not optimal)
			shader.SetUniformMat4f("u_MVP", mvp2.matrix());
			renderer.Draw(va, ib, shader);


			// GUI Window
			{
				ImGui::Text("Engine UI");
				ImGui::SliderFloat3("Trans A", &transA.x, -1.0f, 1.0f); 
				ImGui::SliderFloat3("Trans B", &transB.x, -1.0f, 1.0f);
				mvp.translate(transA);
				mvp2.translate(transB);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}


			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}


	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}