///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Samples Pack (ogl-samples.g-truc.net)
///
/// Copyright (c) 2004 - 2014 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////////

#include <glf/glf.hpp>

namespace
{
	glf::window Window("gl-400-primitive-tessellation");

	std::string const SAMPLE_VERTEX_SHADER("gl-400/tess.vert");
	std::string const SAMPLE_CONTROL_SHADER("gl-400/tess.cont");
	std::string const SAMPLE_EVALUATION_SHADER("gl-400/tess.eval");
	std::string const SAMPLE_GEOMETRY_SHADER("gl-400/tess.geom");
	std::string const SAMPLE_FRAGMENT_SHADER("gl-400/tess.frag");

	GLsizei const VertexCount(4);
	GLsizeiptr const VertexSize = VertexCount * sizeof(glf::vertex_v2fc4f);
	glf::vertex_v2fc4f const VertexData[VertexCount] =
	{
		glf::vertex_v2fc4f(glm::vec2(-1.0f,-1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),
		glf::vertex_v2fc4f(glm::vec2( 1.0f,-1.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)),
		glf::vertex_v2fc4f(glm::vec2( 1.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),
		glf::vertex_v2fc4f(glm::vec2(-1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))
	};

	GLuint ProgramName(0);
	GLuint ArrayBufferName(0);
	GLuint VertexArrayName(0);
	GLint UniformMVP(0);
}//namespace

bool initProgram()
{
	bool Validated = true;
	
	if(Validated)
	{
		GLuint VertexShader = glf::createShader(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + SAMPLE_VERTEX_SHADER);
		GLuint ControlShader = glf::createShader(GL_TESS_CONTROL_SHADER, glf::DATA_DIRECTORY + SAMPLE_CONTROL_SHADER);
		GLuint EvaluationShader = glf::createShader(GL_TESS_EVALUATION_SHADER, glf::DATA_DIRECTORY + SAMPLE_EVALUATION_SHADER);
		GLuint GeometryShader = glf::createShader(GL_GEOMETRY_SHADER, glf::DATA_DIRECTORY + SAMPLE_GEOMETRY_SHADER);
		GLuint FragmentShader = glf::createShader(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + SAMPLE_FRAGMENT_SHADER);

		Validated = Validated && glf::checkShader(VertexShader, SAMPLE_VERTEX_SHADER);
		Validated = Validated && glf::checkShader(ControlShader, SAMPLE_CONTROL_SHADER);
		Validated = Validated && glf::checkShader(EvaluationShader, SAMPLE_EVALUATION_SHADER);
		Validated = Validated && glf::checkShader(GeometryShader, SAMPLE_GEOMETRY_SHADER);
		Validated = Validated && glf::checkShader(FragmentShader, SAMPLE_FRAGMENT_SHADER);

		ProgramName = glCreateProgram();
		glAttachShader(ProgramName, VertexShader);
		//glAttachShader(ProgramName, ControlShader);
		glAttachShader(ProgramName, EvaluationShader);
		glAttachShader(ProgramName, GeometryShader);
		glAttachShader(ProgramName, FragmentShader);
		glDeleteShader(VertexShader);
		glDeleteShader(ControlShader);
		glDeleteShader(EvaluationShader);
		glDeleteShader(GeometryShader);
		glDeleteShader(FragmentShader);
		glLinkProgram(ProgramName);
		Validated = Validated && glf::checkProgram(ProgramName);
	}

	if(Validated)
	{
		UniformMVP = glGetUniformLocation(ProgramName, "MVP");
	}

	return Validated && glf::checkError("initProgram");
}

bool initVertexArray()
{
	// Build a vertex array object
	glGenVertexArrays(1, &VertexArrayName);
	glBindVertexArray(VertexArrayName);
		glBindBuffer(GL_ARRAY_BUFFER, ArrayBufferName);
		glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fc4f), GLF_BUFFER_OFFSET(0));
		glVertexAttribPointer(glf::semantic::attr::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fc4f), GLF_BUFFER_OFFSET(sizeof(glm::vec2)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(glf::semantic::attr::POSITION);
		glEnableVertexAttribArray(glf::semantic::attr::COLOR);
	glBindVertexArray(0);

	return glf::checkError("initVertexArray");
}

bool initBuffer()
{
	glGenBuffers(1, &ArrayBufferName);
	glBindBuffer(GL_ARRAY_BUFFER, ArrayBufferName);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return glf::checkError("initBuffer");
}

bool begin()
{
	bool Validated = true;

	if(Validated)
		Validated = initProgram();
	if(Validated)
		Validated = initBuffer();
	if(Validated)
		Validated = initVertexArray();

	return Validated && glf::checkError("begin");
}

bool end()
{
	glDeleteVertexArrays(1, &VertexArrayName);
	glDeleteBuffers(1, &ArrayBufferName);
	glDeleteProgram(ProgramName);

	return glf::checkError("end");
}

void display()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Window.TranlationCurrent.y));
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Window.RotationCurrent.y, glm::vec3(1.f, 0.f, 0.f));
	glm::mat4 View = glm::rotate(ViewRotateX, Window.RotationCurrent.x, glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model;

	glViewport(0, 0, Window.Size.x, Window.Size.y);
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.0f)[0]);

	glUseProgram(ProgramName);
	glUniformMatrix4fv(UniformMVP, 1, GL_FALSE, &MVP[0][0]);

	glBindVertexArray(VertexArrayName);
	glPatchParameteri(GL_PATCH_VERTICES, VertexCount);
	glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, &glm::vec2(16.f)[0]);
	glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, &glm::vec4(16.f)[0]);
	glDrawArraysInstanced(GL_PATCHES, 0, VertexCount, 1);

	glf::checkError("display");

}

int main(int argc, char* argv[])
{
	return glf::run(argc, argv, glf::CORE, 4, 0);
}
