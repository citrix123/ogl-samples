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
	glf::window Window("gl-420-image-store");

	char const * VERT_SHADER_SOURCE_SAVE("gl-420/image-store-write.vert");
	char const * FRAG_SHADER_SOURCE_SAVE("gl-420/image-store-write.frag");
	char const * VERT_SHADER_SOURCE_READ("gl-420/image-store-read.vert");
	char const * FRAG_SHADER_SOURCE_READ("gl-420/image-store-read.frag");

	namespace program
	{
		enum type
		{
			VERT,
			FRAG,
			MAX
		};
	}//namespace program

	namespace pipeline
	{
		enum type
		{
			READ,
			SAVE,
			MAX
		};
	}//namespace pipeline

	glm::uvec2 ImageSize(0);

	GLuint VertexArrayName(0);
	GLuint PipelineName[pipeline::MAX] = {0, 0};
	GLuint ProgramName[program::MAX] = {0, 0};
	GLuint TextureName(0);
}//namespace

bool initProgram()
{
	bool Validated(true);

	glGenProgramPipelines(pipeline::MAX, PipelineName);

	glBindProgramPipeline(PipelineName[pipeline::READ]);
	if(Validated)
	{
		std::string VertexSourceContent = glf::loadFile(glf::DATA_DIRECTORY + VERT_SHADER_SOURCE_READ);
		char const * VertexSourcePointer = VertexSourceContent.c_str();
		ProgramName[program::VERT] = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &VertexSourcePointer);
		Validated = Validated && glf::checkProgram(ProgramName[program::VERT]);
	}

	if(Validated)
	{
		std::string FragmentSourceContent = glf::loadFile(glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE_READ);
		char const * FragmentSourcePointer = FragmentSourceContent.c_str();
		ProgramName[program::FRAG] = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &FragmentSourcePointer);
		Validated = Validated && glf::checkProgram(ProgramName[program::FRAG]);
	}

	if(Validated)
	{
		glUseProgramStages(PipelineName[pipeline::READ], GL_VERTEX_SHADER_BIT, ProgramName[program::VERT]);
		glUseProgramStages(PipelineName[pipeline::READ], GL_FRAGMENT_SHADER_BIT, ProgramName[program::FRAG]);
		Validated = Validated && glf::checkError("initProgram - stage");
	}

	glBindProgramPipeline(PipelineName[pipeline::SAVE]);
	if(Validated)
	{
		std::string VertexSourceContent = glf::loadFile(glf::DATA_DIRECTORY + VERT_SHADER_SOURCE_SAVE);
		char const * VertexSourcePointer = VertexSourceContent.c_str();
		ProgramName[program::VERT] = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &VertexSourcePointer);
		Validated = Validated && glf::checkProgram(ProgramName[program::VERT]);
	}

	if(Validated)
	{
		std::string FragmentSourceContent = glf::loadFile(glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE_SAVE);
		char const * FragmentSourcePointer = FragmentSourceContent.c_str();
		ProgramName[program::FRAG] = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &FragmentSourcePointer);
		Validated = Validated && glf::checkProgram(ProgramName[program::FRAG]);
	}

	if(Validated)
	{
		glUseProgramStages(PipelineName[pipeline::SAVE], GL_VERTEX_SHADER_BIT, ProgramName[program::VERT]);
		glUseProgramStages(PipelineName[pipeline::SAVE], GL_FRAGMENT_SHADER_BIT, ProgramName[program::FRAG]);
		Validated = Validated && glf::checkError("initProgram - stage");
	}

	return Validated;
}

bool initTexture()
{
	bool Validated(true);

	glGenTextures(1, &TextureName);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexStorage2D(GL_TEXTURE_2D, GLint(1), GL_RGBA8, GLsizei(Window.Size.x), GLsizei(Window.Size.y));

	return Validated;
}

bool initVertexArray()
{
	bool Validated(true);

	glGenVertexArrays(1, &VertexArrayName);
	glBindVertexArray(VertexArrayName);
	glBindVertexArray(0);

	return Validated;
}

bool begin()
{
	bool Validated(true);

	glf::logImplementationDependentLimit(GL_MAX_IMAGE_UNITS, "GL_MAX_IMAGE_UNITS");
	glf::logImplementationDependentLimit(GL_MAX_VERTEX_IMAGE_UNIFORMS, "GL_MAX_VERTEX_IMAGE_UNIFORMS");
	glf::logImplementationDependentLimit(GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS, "GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS");
	glf::logImplementationDependentLimit(GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS, "GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS");
	glf::logImplementationDependentLimit(GL_MAX_GEOMETRY_IMAGE_UNIFORMS, "GL_MAX_GEOMETRY_IMAGE_UNIFORMS");
	glf::logImplementationDependentLimit(GL_MAX_FRAGMENT_IMAGE_UNIFORMS, "GL_MAX_FRAGMENT_IMAGE_UNIFORMS");
	glf::logImplementationDependentLimit(GL_MAX_COMBINED_IMAGE_UNIFORMS, "GL_MAX_COMBINED_IMAGE_UNIFORMS");
	glf::logImplementationDependentLimit(GL_MAX_ARRAY_TEXTURE_LAYERS, "GL_MAX_ARRAY_TEXTURE_LAYERS");
	glf::logImplementationDependentLimit(GL_MAX_TEXTURE_IMAGE_UNITS, "GL_MAX_TEXTURE_IMAGE_UNITS");
	glf::logImplementationDependentLimit(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS");
	glf::logImplementationDependentLimit(GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS, "GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS");
	//glf::logImplementationDependentLimit(GL_MAX_TEXTURE_UNITS, "GL_MAX_TEXTURE_UNITS");

	if(Validated)
		Validated = initTexture();
	if(Validated)
		Validated = initProgram();
	if(Validated)
		Validated = initVertexArray();

	return Validated;
}

bool end()
{
	bool Validated(true);

	glDeleteTextures(1, &TextureName);
	glDeleteVertexArrays(1, &VertexArrayName);
	glDeleteProgram(ProgramName[program::VERT]);
	glDeleteProgram(ProgramName[program::FRAG]);
	glDeleteProgramPipelines(pipeline::MAX, PipelineName);

	return Validated;
}

void display()
{
	glViewportIndexedf(0, 0, 0, float(Window.Size.x), float(Window.Size.y));
	glDrawBuffer(GL_BACK);
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.0f, 0.5f, 1.0f, 1.0f)[0]);

	// Renderer to image
	{
		glDrawBuffer(GL_NONE);

		glBindProgramPipeline(PipelineName[pipeline::SAVE]);
		glBindImageTexture(glf::semantic::image::DIFFUSE, TextureName, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
		glBindVertexArray(VertexArrayName);
		glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 3, 1, 0);
	}

	// Read from image
	{
		GLint Border(8);
		glEnable(GL_SCISSOR_TEST);
		glScissorIndexed(0, Border, Border, Window.Size.x - 2 * Border, Window.Size.y - 2 * Border);

		glDrawBuffer(GL_BACK);

		glBindProgramPipeline(PipelineName[pipeline::READ]);
		glBindImageTexture(glf::semantic::image::DIFFUSE, TextureName, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindVertexArray(VertexArrayName);
		glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 3, 1, 0);

		glDisable(GL_SCISSOR_TEST);
	}
}

int main(int argc, char* argv[])
{
	return glf::run(argc, argv, glf::CORE, 4, 2);
}

