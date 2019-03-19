#pragma once
#include "utils.h"

class ShaderLoader
{
public:

	ShaderLoader(void);
	~ShaderLoader(void);
	GLuint create_program(char* vertex_shader_filename,
	                      char* fragment_shader_filename);
private:

	std::string read_shader(char* filename);
	GLuint create_shader(GLenum shader_type,
	                     std::string source,
	                     char* shader_name);
};
