// Must be before GLFW.
#include <glad/glad.h>
// Must be after GLAD.
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <ktx.h>
#include <mathfu/constants.h>
#include <mathfu/glsl_mappings.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include "astc.h"

namespace {
template <typename T>
T DegreesToRadians(T degrees) {
  return degrees * static_cast<T>(0.01745329251994329576923690768489);
}

void RenderCube() {
  static unsigned int cube_vao = 0;
  static unsigned int cube_vbo = 0;
  if (cube_vao == 0) {
    float vertices[] = {
        // Back face.
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top-right
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,   // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top-right
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,   // top-left
        // Front face.
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // bottom-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top-right
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // top-left
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
        // Left face.
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-right
        -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
        -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-right
        // right face
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-left
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-right
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-left
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
        // Bottom face.
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top-right
        1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,   // top-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // bottom-left
        1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // bottom-left
        -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top-right
        // Top face.
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,    // bottom-right
        1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,    // bottom-right
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f    // bottom-left
    };
    glGenVertexArrays(1, &cube_vao);
    glGenBuffers(1, &cube_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Link vertex attributes.
    glBindVertexArray(cube_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(6 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  // Render the cube.
  glBindVertexArray(cube_vao);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

bool LoadFile(const char* path, char** mem, size_t* size) {
  assert(size);
  assert(mem);

  std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    return false;
  }

  std::streampos pos = file.tellg();
  *mem = new char[pos];
  *size = pos;

  file.seekg(0, std::ios::beg);
  file.read(*mem, pos);
  file.close();

  return true;
}

GLuint CompileShader(const GLenum shader_type, const char* source,
                     const int length) {
  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &source, &length);

  // Compile the shader
  glCompileShader(shader);

  GLint isCompiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    std::cout << "Shader compilation failed" << std::endl;
    GLint max_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

    // The max_length includes the NULL character
    GLchar* info_log = new GLchar[max_length + 1];
    glGetShaderInfoLog(shader, max_length, &max_length, info_log);
    info_log[max_length] = 0;

    // We don't need the shader anymore.
    glDeleteShader(shader);

    // Print the info_log.
    std::cout << info_log << std::endl;

    return 0;
  }
  return shader;
}

unsigned int LoadShader(const char* shader) {
  size_t frag_size, vert_size;
  char *frag, *vert;
  const std::string vertex_path = std::string(shader) + ".glslv";
  const std::string fragment_path = std::string(shader) + ".glslf";

  if (!LoadFile(vertex_path.c_str(), &vert, &vert_size)) {
    assert(false);
    return 0;
  }
  if (!LoadFile(fragment_path.c_str(), &frag, &frag_size)) {
    assert(false);
    return 0;
  }

  GLuint program = glCreateProgram();

  GLuint vertex_shader = CompileShader(GL_VERTEX_SHADER, vert, vert_size);
  GLuint fragment_shader = CompileShader(GL_FRAGMENT_SHADER, frag, frag_size);
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  glLinkProgram(program);
  glUseProgram(program);
  glUniform1i(glGetUniformLocation(program, "sampler0"), 0);
  glUseProgram(0);

  delete[] frag;
  delete[] vert;

  return program;
}
}  // namespace

GLFWwindow* InitWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window =
      glfwCreateWindow(512, 512, "Environment Map Tool", NULL, NULL);
  glfwMakeContextCurrent(window);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }
  // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // glfwSetCursorPosCallback(window, mouse_callback);
  // glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to capture our mouse
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Initializer GLAD.
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  return window;
}

GLenum NumComponentsToGlFormat(int num_components) {
  if (num_components == 1) {
    return GL_R;
  }
  if (num_components == 2) {
    return GL_RG;
  }
  if (num_components == 3) {
    return GL_RGB;
  }
  if (num_components == 4) {
    return GL_RGBA;
  }
  assert(false);
  return GL_RGB;
}

void RenderTextureToCubemap(unsigned int fbo, unsigned int fbo_texture,
                            int width, int height, unsigned int shader,
                            int mip = 0) {
  GLint old_fbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);

  const mathfu::mat4 projection = mathfu::mat4::Perspective(
      DegreesToRadians(90.0f), 1.0f, 0.1f, 10.0f, -1.0f);
  const mathfu::mat4 views[] = {
      mathfu::mat4::LookAt(mathfu::vec3(-1.0f, 0.0f, 0.0f),
                           mathfu::vec3(0.0f, 0.0f, 0.0f),
                           mathfu::vec3(0.0f, -1.0f, 0.0f)),
      mathfu::mat4::LookAt(mathfu::vec3(1.0f, 0.0f, 0.0f),
                           mathfu::vec3(0.0f, 0.0f, 0.0f),
                           mathfu::vec3(0.0f, -1.0f, 0.0f)),
      mathfu::mat4::LookAt(mathfu::vec3(0.0f, 1.0f, 0.0f),
                           mathfu::vec3(0.0f, 0.0f, 0.0f),
                           mathfu::vec3(0.0f, 0.0f, 1.0f)),
      mathfu::mat4::LookAt(mathfu::vec3(0.0f, -1.0f, 0.0f),
                           mathfu::vec3(0.0f, 0.0f, 0.0f),
                           mathfu::vec3(0.0f, 0.0f, -1.0f)),
      mathfu::mat4::LookAt(mathfu::vec3(0.0f, 0.0f, 1.0f),
                           mathfu::vec3(0.0f, 0.0f, 0.0f),
                           mathfu::vec3(0.0f, -1.0f, 0.0f)),
      mathfu::mat4::LookAt(mathfu::vec3(0.0f, 0.0f, -1.0f),
                           mathfu::vec3(0.0f, 0.0f, 0.0f),
                           mathfu::vec3(0.0f, -1.0f, 0.0f))};

  glUseProgram(shader);

  glViewport(0, 0, width, height);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  for (unsigned int i = 0; i < 6; ++i) {
    const mathfu::mat4 mat_projection_view = projection * views[i];
    glUniformMatrix4fv(glGetUniformLocation(shader, "uMatViewProjection"), 1,
                       false, &mat_projection_view[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, fbo_texture,
                           mip);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render a 1x1 cube.
    RenderCube();
  }
  glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
}

GLuint LoadHDRTexture(const char* file, int* out_width, int* out_height) {
  int width, height, num_components;
  float* data = stbi_loadf(file, &width, &height, &num_components, 0);
  if (!data) {
    std::cout << "Failed to load HDR image." << std::endl;
    return 1;
  }

  unsigned int gl_texture;
  glGenTextures(1, &gl_texture);
  glBindTexture(GL_TEXTURE_2D, gl_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0,
               NumComponentsToGlFormat(num_components), GL_FLOAT, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  stbi_image_free(data);

  if (out_width) {
    *out_width = width;
  }
  if (out_height) {
    *out_height = height;
  }
  return gl_texture;
}

unsigned int ConvertEquirectangularToCubemap(const char* file,
                                             int cubemap_width,
                                             int cubemap_height) {
  int width, height;
  GLuint equirectangular_texture = LoadHDRTexture(file, &width, &height);
  if (!equirectangular_texture) {
    return 0;
  }

  // Create framebuffer.
  GLint old_fbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);

  GLuint fbo, rbo;
  glGenFramebuffers(1, &fbo);
  glGenRenderbuffers(1, &rbo);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cubemap_width,
                        cubemap_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  // Generate the textures for the framebuffer.
  unsigned int cubemap;
  glGenTextures(1, &cubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                 cubemap_width, cubemap_height, 0, GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);

  // Draw each face of the cubemap, sampling from the equirectangular texture to
  // generate the cubemap.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, equirectangular_texture);
  RenderTextureToCubemap(fbo, cubemap, cubemap_width, cubemap_height,
                         LoadShader("data/equirectangular_to_cubemap"));

  // Generate mipmaps for the cubemap.
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

  return cubemap;
}

unsigned int GenerateIrradianceMap(unsigned int texture, int cubemap_width,
                                   int cubemap_height) {
  // Create framebuffer.
  GLint old_fbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);

  GLuint fbo, rbo;
  glGenFramebuffers(1, &fbo);
  glGenRenderbuffers(1, &rbo);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cubemap_width,
                        cubemap_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  // Generate the textures for the framebuffer.
  unsigned int cubemap;
  glGenTextures(1, &cubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                 cubemap_width, cubemap_height, 0, GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);

  // Draw each face of the cubemap, sampling from the equirectangular texture to
  // generate the cubemap.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
  RenderTextureToCubemap(fbo, cubemap, cubemap_width, cubemap_height,
                         LoadShader("data/irradiance_convolution"));

  return cubemap;
}

unsigned int GeneratePreFilteredMap(unsigned int texture, int cubemap_width,
                                    int cubemap_height) {
  GLint old_fbo;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
  unsigned int shader = LoadShader("data/prefilter");

  // Generate fbo.
  GLuint fbo;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  // Generate the textures for the framebuffer.
  unsigned int cubemap;
  glGenTextures(1, &cubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                 cubemap_width, cubemap_height, 0, GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

  const int num_mips =
      1 + std::floor(std::log2(std::max(cubemap_width, cubemap_height)));
  for (int mip = 0; mip < num_mips; ++mip) {
    const float roughness = (float)mip / (float)(num_mips - 1);
    glUniform1f(glGetUniformLocation(shader, "roughness"), roughness);
    unsigned int width = cubemap_width * std::pow(0.5, mip);
    unsigned int height = cubemap_height * std::pow(0.5, mip);

    // Draw each face of the cubemap, sampling from the equirectangular texture
    // to generate the cubemap.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    RenderTextureToCubemap(fbo, cubemap, width, height, shader, mip);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
  return cubemap;
}

void WriteCubemapToFile(std::string file, unsigned int texture,
                        int cubemap_width, int cubemap_height, int mip = 0) {
  // int stbi_write_hdr(char const* filename, int w, int h, int comp,
  //                   const float* data);
  // int stbi_write_png(char const* filename, int w, int h, int comp,
  //                   const void* data, int stride_in_bytes);

  std::string filenames[] = {
      file + "_right",  file + "_left",  file + "_top",
      file + "_bottom", file + "_front", file + "_back",
  };
  std::string extension = ".png";

  float* pixels = new float[cubemap_width * cubemap_height * 3];
  unsigned char* pixels_bytes =
      new unsigned char[cubemap_width * cubemap_height * 3];
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
  for (int i = 0; i < 6; ++i) {
    glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip, GL_RGB, GL_FLOAT,
                  static_cast<void*>(pixels));

    for (int i = 0; i < (cubemap_width * cubemap_height * 3); ++i) {
      float color = std::min(1.0f, std::max(0.0f, pixels[i]));
      // float color = pixels[i];
      pixels_bytes[i] = static_cast<unsigned char>(color * 255.0f);
    }

    stbi_write_png((filenames[i] + extension).c_str(), cubemap_width,
                   cubemap_height, 3, pixels_bytes, 0);

    // stbi_write_hdr(filenames[i].c_str(), cubemap_width, cubemap_height, 3,
    //             pixels);
  };
  delete[] pixels;
  delete[] pixels_bytes;
}

void WriteCubemapToKtx(std::string file, unsigned int texture,
                       int cubemap_width, int cubemap_height,
                       int num_mips = 1) {
  static const std::string kExtension = ".ktx";
  ktx::KtxHeader header;
  header.gl_type = GL_FLOAT;
  header.gl_format = GL_RGB;
  header.gl_internal_format = GL_RGB16F;
  header.gl_base_internal_format = GL_RGB;
  header.pixel_width = cubemap_width;
  header.pixel_height = cubemap_height;
  header.pixel_depth = 0;
  header.number_of_array_elements = 0;
  header.number_of_faces = 6;
  header.number_of_mipmap_levels = num_mips;
  header.bytes_of_key_value_data = 0;

  std::ofstream fstream;
  fstream.open((file + kExtension).c_str(),
               std::ios::out | std::ios::trunc | std::ios::binary);
  if (!fstream.is_open()) {
    return;
  }

  fstream.write(reinterpret_cast<const char*>(&header), sizeof(ktx::KtxHeader));

  char* pixels =
      new char[cubemap_width * cubemap_height * 3 * 6 * sizeof(float)];
  for (int mip = 0; mip < num_mips; ++mip) {
    // Image size for all 6 faces of this mip.
    unsigned int mip_width = cubemap_width * std::pow(0.5, mip);
    unsigned int mip_height = cubemap_height * std::pow(0.5, mip);
    uint32_t image_size = mip_width * mip_height * 3 * sizeof(float);
    fstream.write(reinterpret_cast<const char*>(&image_size), sizeof(uint32_t));

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    for (int i = 0; i < 6; ++i) {
      size_t offset = image_size * i;
      glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip, GL_RGB, GL_FLOAT,
                    static_cast<void*>(pixels + offset));
    };

    fstream.write(pixels, image_size * 6);
  }

  delete[] pixels;
  fstream.close();
}

GLenum GetTextureFormatForAstc(int footprint_x, int footprint_y) {
  switch (footprint_x) {
    case 4: {
      switch (footprint_y) {
        case 4: {
          return GL_COMPRESSED_RGBA_ASTC_4x4;
        }
      }
    } break;
    case 5: {
      switch (footprint_y) {
        case 4: {
          return GL_COMPRESSED_RGBA_ASTC_5x4;
        }
        case 5: {
          return GL_COMPRESSED_RGBA_ASTC_5x5;
        }
      }
    } break;
    case 6: {
      switch (footprint_y) {
        case 5: {
          return GL_COMPRESSED_RGBA_ASTC_6x5;
        }
        case 6: {
          return GL_COMPRESSED_RGBA_ASTC_6x6;
        }
      }
    } break;
    case 8: {
      switch (footprint_y) {
        case 5: {
          return GL_COMPRESSED_RGBA_ASTC_8x5;
        }
        case 6: {
          return GL_COMPRESSED_RGBA_ASTC_8x6;
        }
        case 8: {
          return GL_COMPRESSED_RGBA_ASTC_8x8;
        }
      }
    } break;
    case 10: {
      switch (footprint_y) {
        case 5: {
          return GL_COMPRESSED_RGBA_ASTC_10x5;
        }
        case 6: {
          return GL_COMPRESSED_RGBA_ASTC_10x6;
        }
        case 8: {
          return GL_COMPRESSED_RGBA_ASTC_10x8;
        }
        case 10: {
          return GL_COMPRESSED_RGBA_ASTC_10x10;
        }
      }
    } break;
    case 12: {
      switch (footprint_y) {
        case 10: {
          return GL_COMPRESSED_RGBA_ASTC_12x10;
        }
        case 12: {
          return GL_COMPRESSED_RGBA_ASTC_12x12;
        }
      }
    } break;
    default: {
      // Invalid format?
      assert(false);
    }
  }
  return GL_COMPRESSED_RGBA_ASTC_4x4;
}

void WriteCubemapToKtxAsASTC(std::string file, unsigned int texture,
                             int cubemap_width, int cubemap_height,
                             int num_mips = 1, int footprint_x = 4,
                             int footprint_y = 4) {
  static const std::string kExtension = ".ktx";
  ktx::KtxHeader header;
  header.gl_type = 0;  // Compressed texture must be 0.
  header.gl_format = GL_RGB;
  header.gl_internal_format = GetTextureFormatForAstc(footprint_x, footprint_y);
  header.gl_base_internal_format = GL_RGB;
  header.pixel_width = cubemap_width;
  header.pixel_height = cubemap_height;
  header.pixel_depth = 0;
  header.number_of_array_elements = 0;
  header.number_of_faces = 6;
  header.number_of_mipmap_levels = num_mips;
  header.bytes_of_key_value_data = 0;

  std::ofstream fstream;
  fstream.open((file + kExtension).c_str(),
               std::ios::out | std::ios::trunc | std::ios::binary);
  if (!fstream.is_open()) {
    return;
  }

  fstream.write(reinterpret_cast<const char*>(&header), sizeof(ktx::KtxHeader));

  char* pixels =
      new char[cubemap_width * cubemap_height * 3 * 6 * sizeof(float)];
  uint8_t* astc_data;
  size_t astc_size;

  for (int mip = 0; mip < num_mips; ++mip) {
    bool write_size = true;

    // Image size for all 6 faces of this mip.
    unsigned int mip_width = cubemap_width * std::pow(0.5, mip);
    unsigned int mip_height = cubemap_height * std::pow(0.5, mip);
    uint32_t image_size = mip_width * mip_height * 3 * sizeof(float);

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    for (int i = 0; i < 6; ++i) {
      size_t offset = image_size * i;
      glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip, GL_RGB, GL_FLOAT,
                    static_cast<void*>(pixels + offset));

      // Convert to ASTC.
      EncodeAstc(pixels + offset, mip_width, mip_height, GL_RGB, GL_FLOAT,
                 &astc_data, &astc_size, footprint_x, footprint_y);

      if (!astc_data) {
        assert(false);
      }

      if (write_size) {
        fstream.write(reinterpret_cast<const char*>(&astc_size),
                      sizeof(uint32_t));
        write_size = false;
      }
      fstream.write(reinterpret_cast<const char*>(astc_data), astc_size);
      delete[] astc_data;
    }
  }

  delete[] pixels;
  fstream.close();
}

int main(int argc, char* argv[]) {
  stbi_set_flip_vertically_on_load(true);
  GLFWwindow* window = InitWindow();
  if (!window) {
    return 1;
  }

  // Enable seamless cubemap sampling for lower mip levels in the pre-filter
  // map.
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  const int cubemap_width = 256;
  const int cubemap_height = 256;
  const int irradiance_width = 128;
  const int irradiance_height = 128;
  const int prefilter_width = 256;
  const int prefilter_height = 256;

  unsigned int cubemap_texture = ConvertEquirectangularToCubemap(
      "data/source.hdr", cubemap_width, cubemap_height);
  WriteCubemapToFile("cubemap", cubemap_texture, cubemap_width, cubemap_height);
  WriteCubemapToKtx("cubemap", cubemap_texture, cubemap_width, cubemap_height,
                    1);
  unsigned int irradiance_texture = GenerateIrradianceMap(
      cubemap_texture, irradiance_width, irradiance_height);
  WriteCubemapToFile("irradiance", irradiance_texture, irradiance_width,
                     irradiance_height);
  WriteCubemapToKtx("irradiance", irradiance_texture, irradiance_width,
                    irradiance_height, 1);
  WriteCubemapToKtxAsASTC("irradiance_astc", irradiance_texture,
                          irradiance_width, irradiance_height, 1);

  // Generate the prefilter map.
  unsigned int prefilter_texture = GeneratePreFilteredMap(
      cubemap_texture, prefilter_width, prefilter_height);
  // Write the prefilter map to textures.
  const int num_mips =
      1 + std::floor(std::log2(std::max(prefilter_width, prefilter_height)));
  for (int mip = 0; mip < num_mips; ++mip) {
    unsigned int width = prefilter_width * std::pow(0.5, mip);
    unsigned int height = prefilter_height * std::pow(0.5, mip);
    WriteCubemapToFile("prefilter_" + std::to_string(mip), prefilter_texture,
                       width, height, mip);
  }
  WriteCubemapToKtx("prefilter", prefilter_texture, prefilter_width,
                    prefilter_height, num_mips);
  WriteCubemapToKtxAsASTC("prefilter_astc", prefilter_texture, prefilter_width,
                          prefilter_height, num_mips, 4, 4);

  std::cout << "Success!" << std::endl;

  return 0;
}