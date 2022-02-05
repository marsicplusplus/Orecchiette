#include "core/framebuffer.hpp"

namespace{
	const char *vertexShaderSource = "#version 450 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aTexturePos;\n"
		"out vec2 TexCoord;\n"
		"void main()\n"
		"{\n"
		"   TexCoord = aTexturePos;"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char *fragmentShaderSource = "#version 450 core\n"
		"uniform sampler2D text;\n"
		"in vec2 TexCoord;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
			"FragColor = texture(text, TexCoord);\n"
		"}\0";
};

