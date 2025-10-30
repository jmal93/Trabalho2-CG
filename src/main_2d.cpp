
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "state.h"
#include "camera2d.h"
#include "color.h"
#include "transform.h"
#include "error.h"
#include "shader.h"
#include "texture.h"
#include "quad.h"
#include "triangle.h"
#include "disk.h"
#include "orbit.h"

#include <initializer_list>
#include <iostream>
#include <ostream>

static ScenePtr scene;
static CameraPtr camera;

static void initialize (void)
{
  // set background color: white 
  glClearColor(0.0f,0.0f,0.0f,1.0f);
  // enable depth test 
  glEnable(GL_DEPTH_TEST);

  // create objects
  camera = Camera2D::Make(0,10,0,10);

  auto trf_moon_orbit_disk = Transform::Make();
  trf_moon_orbit_disk->Translate(0.0f, 0.0f, 0.0f);
  auto trf_moon = Transform::Make();
  trf_moon->Translate(2.5f, 0.0f, 0.0f);
  trf_moon->Scale(0.4f, 0.4f, 1.0f);
  auto moon_orbit_disk = Node::Make(
    trf_moon_orbit_disk, {Node::Make(trf_moon, {Color::Make(190, 190, 190), Texture::Make("tex", "images/moon.jpg")}, {Disk::Make()})}
  );

  auto trf_earth = Transform::Make();
  trf_earth->Translate(4.8f, 0.0f, 1.0f);
  trf_earth->Scale(0.3f, 0.3f, 1.0f);
  auto earth = Node::Make(trf_earth, {Color:: Make(1, 1, 1), Texture::Make("tex", "images/earth.jpg")}, {Disk::Make()});
  earth->AddNode(moon_orbit_disk);
  auto trf_earth_orbit_disk = Transform::Make();
  trf_earth_orbit_disk->Translate(5.0f, 5.0f, 0.0f);
  auto earth_orbit_disk = Node::Make(trf_earth_orbit_disk, {earth});

  auto trf_mercury = Transform::Make();
  trf_mercury->Translate(2.5f, 0.0f, 1.0f);
  trf_mercury->Scale(0.25f, 0.25f, 1.0f);
  auto mercury = Node::Make(trf_mercury, {Color::Make(0.7f, 0.7f, 0.7f), Texture::Make("tex", "images/mercury.jpg")}, {Disk::Make()});
  auto trf_mercury_orbit_disk = Transform::Make();
  trf_mercury_orbit_disk->Translate(5.0f, 5.0f, 0.0f);
  auto mercury_orbit_disk = Node::Make(trf_mercury_orbit_disk, {mercury});

  auto trf_sun = Transform::Make();
  trf_sun->Translate(5.0f,5.0f,0.5f);
  trf_sun->Scale(1.7f,1.7f,1.0f);
  auto sun = Node::Make(trf_sun, {Color:: Make(1, 1, 1), Texture::Make("tex", "images/lebron.jpg")},{Disk::Make()});

  auto shader = Shader::Make();
  shader->AttachVertexShader("shaders/2d/vertex.glsl");
  shader->AttachFragmentShader("shaders/2d/fragment.glsl");
  shader->Link();

  auto root =  Node::Make(shader, {sun, earth_orbit_disk, mercury_orbit_disk});
  scene = Scene::Make(root);
  scene->AddEngine(Orbit::Make(trf_mercury_orbit_disk, 4.15f));
  scene->AddEngine(Orbit::Make(trf_earth_orbit_disk, 1.0f));
  scene->AddEngine(Orbit::Make(trf_moon_orbit_disk, 13.37f));
  scene->AddEngine(Orbit::Make(trf_earth, 0.5f));
  scene->AddEngine(Orbit::Make(trf_mercury, 13.37f));
}

static void display (GLFWwindow* win)
{ 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear window 
  Error::Check("before render");
  scene->Render(camera);
  Error::Check("after render");
}

static void error (int code, const char* msg)
{
  printf("GLFW error %d: %s\n", code, msg);
  glfwTerminate();
  exit(0);
}

static void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void resize (GLFWwindow* win, int width, int height)
{
  glViewport(0,0,width,height);
}

static void update (float dt)
{
  scene->Update(dt);
}

int main ()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwSetErrorCallback(error);

  GLFWwindow* win = glfwCreateWindow(600, 400, "Window title", nullptr, nullptr);
  assert(win);
  glfwSetFramebufferSizeCallback(win, resize);  // resize callback
  glfwSetKeyCallback(win, keyboard);            // keyboard callback

  glfwMakeContextCurrent(win);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  printf("OpenGL version: %s\n", glGetString(GL_VERSION));

  initialize();

  float t0 = float(glfwGetTime());
  while(!glfwWindowShouldClose(win)) {
    float t = float(glfwGetTime());
    update(t-t0);
    t0 = t;
    display(win);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

