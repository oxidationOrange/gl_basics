#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <math.h>

const char *vertex_shader_char = {
  "#version 330\n"
  "layout (location = 0) in vec2 in_position;"
  "uniform vec2 translation;"
  "uniform mat2 rotation_matrix;"
  "void main() {"
  "  vec2 position = rotation_matrix * in_position;"
  "  gl_Position = vec4(position.x + translation.x, position.y + translation.y, 0.0, 1.0);"
  "}\0"
};

const char *fragment_shader_char = {
  "#version 330\n"
  "out vec4 fragment;"
  "void main() {"
  "  fragment = vec4(0.4, 0.8, 0.8, 1.0);"
  "}\0"
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

float positions[6] = {
  0.0, 0.5,
  -0.5, -0.5,
  0.5, -0.5
};

int main() {
  GLFWwindow *window;
  GLuint program, vao, vbo, vertex_shader, fragment_shader;

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(640, 640, "also try DOOM!", NULL, NULL);

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {return -1;}

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_char, NULL);
  glCompileShader(vertex_shader);
  
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_char, NULL);
  glCompileShader(fragment_shader);

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  float time = 0.0f;

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0, 0.5, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vao);

    float rotation_matrix[4] = {
      cosf(time), -sinf(time),
      sinf(time), cosf(time),
    };

    GLuint translation_location = glGetUniformLocation(program, "translation");
    glUniform2f(translation_location, cosf(time) / 1.5, sinf(time) / 1.5);

    GLuint rotation_location = glGetUniformLocation(program, "rotation_matrix");
    glUniformMatrix2fv(rotation_location, 1, GL_FALSE, &rotation_matrix[0]);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    time += 1.0 / 60.0;
  }

  return 0;
}
