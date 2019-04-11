#include "../Header/ShaderLoader.h"

ShaderLoader::ShaderLoader(void)
{
}

ShaderLoader::~ShaderLoader(void)
{
}

std::string ShaderLoader::read_shader(char* filename)
{
	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);
	if (!file.good())
	{
		std::cout << "Can't read file " << filename << std::endl;
		std::terminate();
	}
	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

GLuint ShaderLoader::create_shader(GLenum shader_type, std::string source, char* shader_name)
{
	int compile_result = 0;
	GLuint shader = glCreateShader(shader_type);
	const char* shader_code_ptr = source.c_str();
	const int shader_code_size = source.size();
	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	//check for errors
	if (compile_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(shader, info_log_length, nullptr, &shader_log[0]);
		std::cout << "ERROR compiling shader: " << shader_name << std::endl << &shader_log[0] << std::endl;
		return 0;
	}
	return shader;
}

GLuint ShaderLoader::create_program(char* vertexShaderFilename, char* fragmentShaderFilename)
{
	//read the shader files and save the code
	std::string vertex_shader_code = read_shader(vertexShaderFilename);
	std::string fragment_shader_code = read_shader(fragmentShaderFilename);
	GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_code, (char*)"vertex shader");
	GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_code, (char*)"fragment shader");
	int link_result = 0;
	//create the program handle, attach the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, nullptr, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return 0;
	}
	return program;
}
