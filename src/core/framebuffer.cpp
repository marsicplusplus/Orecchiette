#include "core/framebuffer.hpp"

#include "glad/glad.h"

#include <iostream>
#include <algorithm>

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

void Framebuffer::init(uint64_t w, uint64_t h){
	this->width = w;
	this->height = h;

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	this->shader = glCreateProgram();
	glAttachShader(this->shader, vertexShader);
	glAttachShader(this->shader, fragmentShader);
	glLinkProgram(this->shader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(this->shader);

	/* Quad */
	float vertices[] = {
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // top right
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // top left
	};
	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Texture Coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/* Texture */
	glGenTextures(1, &this->texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
	this->buffer = new uint32_t[this->width * this->height];
	this->accumulator = new Color[this->width * this->height];
}

void Framebuffer::putPixel(uint64_t idx, const glm::vec3 &color, uint64_t nFrames) {
	Color newCol {
			color.x + accumulator[idx].x * nFrames,
			color.y + accumulator[idx].y * nFrames,
			color.z + accumulator[idx].z * nFrames
	};
	newCol /= nFrames+1;
	accumulator[idx] = newCol;

	unsigned char r = static_cast<unsigned char>(std::clamp(newCol.r, 0.0f, 0.999f) * 255.999f);
	unsigned char g = static_cast<unsigned char>(std::clamp(color.g, 0.0f, 0.999f) * 255.999f);
	unsigned char b = static_cast<unsigned char>(std::clamp(color.b, 0.0f, 0.999f) * 255.999f);
	buffer[idx] = r << 16 | g << 8 | b << 0;
}

void Framebuffer::present() {
		glBindTexture(GL_TEXTURE_2D, this->texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Framebuffer::~Framebuffer(){
	glDeleteProgram(this->shader);
	glDeleteTextures(1, &this->texture);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
	glDeleteVertexArrays(1, &this->VAO);
	delete buffer;
}
