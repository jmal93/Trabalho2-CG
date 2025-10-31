
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "arcball.h"
#include "scene.h"
#include "state.h"
#include "camera3d.h"
#include "material.h"
#include "texture.h"
#include "transform.h"
#include "cube.h"
#include "quad.h"
#include "sphere.h"
#include "error.h"
#include "shader.h"
#include "light.h"
#include "light.h"
#include "polyoffset.h"
#include "orbit.h"
#include "skybox.h"
#include "texcube.h"
#include "cameratarget.h"

#include <iostream>
#include <cassert>
#include <ostream>

static float viewer_pos[3] = {2.0f, 3.5f, 8.0f};

static ScenePtr scene;
static Camera3DPtr camera;
static ArcballPtr arcball;

static CameraTargetPtr camera_target_engine;

static void initialize (void)
{
  // set background color: white 
  glClearColor(1.0f,1.0f,1.0f,1.0f);
  // enable depth test 
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);  // cull back faces

  // create objects
  camera = Camera3D::Make(viewer_pos[0],viewer_pos[1],viewer_pos[2]);
  //camera->SetOrtho(true);
  arcball = camera->CreateArcball();

  //LightPtr light = ObjLight::Make(viewer_pos[0],viewer_pos[1],viewer_pos[2]);
  LightPtr light = Light::Make(0.0f,0.0f,0.0f,1.0f,"camera");

  Error::Check("before shps");
  ShapePtr cube = Cube::Make();
  Error::Check("before quad");
  ShapePtr quad = Quad::Make();
  Error::Check("before sphere");
  ShapePtr sphere = Sphere::Make();
  Error::Check("before skybox");
  auto skybox = SkyBox::Make();
  Error::Check("after shps");

  AppearancePtr white = Material::Make(1.0f,1.0f,1.0f);
  AppearancePtr sun_texture = Texture::Make("decal", "images/lebron.jpg");
  AppearancePtr earth_texture = Texture::Make("decal", "images/earth.jpg");
  AppearancePtr mercury_texture = Texture::Make("decal", "images/mercury.jpg");
  AppearancePtr moon_texture = Texture::Make("decal", "images/moon.jpg");
  AppearancePtr sky = TexCube::Make("sky", "images/skybox.png");

  TransformPtr sun_transform = Transform::Make();
  sun_transform->Scale(0.5f, 0.5f, 0.5f);

  TransformPtr mercury_orbit_disk_transform = Transform::Make();
  mercury_orbit_disk_transform->Translate(0.0f, 0.0f, 0.0f);
  TransformPtr mercury_transform = Transform::Make();
  mercury_transform->Translate(1.0f, 0.0f, 0.0f);
  mercury_transform->Scale(0.2f, 0.2f, 0.2f);

  TransformPtr earth_orbit_disk_transform = Transform::Make();
  earth_orbit_disk_transform->Translate(0.0f, 0.0f, 0.0f);
  TransformPtr earth_transform = Transform::Make();
  earth_transform->Translate(3.0f, 0.0f, 0.0f);
  earth_transform->Scale(0.2f, 0.2f, 0.2f);
  
  TransformPtr moon_orbit_disk_transform = Transform::Make();
  moon_orbit_disk_transform->Translate(0.0f, 0.0f, 0.0f);
  TransformPtr moon_transform = Transform::Make();
  moon_transform->Translate(2.5f, 0.0f, 0.0f);
  moon_transform->Scale(0.5f, 0.5f, 0.5f);

  // create shader
  ShaderPtr shader = Shader::Make(light,"world");
  shader->AttachVertexShader("shaders/ilum_vert/vertex.glsl");
  shader->AttachFragmentShader("shaders/ilum_vert/fragment.glsl");
  shader->Link();

  // sky shader
  ShaderPtr shd_sky = Shader::Make();
  shd_sky->AttachVertexShader("shaders/skybox/vertex.glsl");
  shd_sky->AttachFragmentShader("shaders/skybox/fragment.glsl");
  shd_sky->Link();

  // Define a different shader for texture mapping
  // An alternative would be to use only this shader with a "white" texture for untextured objects
  ShaderPtr shd_tex = Shader::Make(light,"world");
  shd_tex->AttachVertexShader("shaders/ilum_vert/vertex_texture.glsl");
  shd_tex->AttachFragmentShader("shaders/ilum_vert/fragment_texture.glsl");
  shd_tex->Link();

  // make nodes
  NodePtr sun = Node::Make(shd_tex, sun_transform, {white, sun_texture}, {sphere});
  NodePtr mercury = Node::Make(shd_tex, mercury_transform, {white, mercury_texture}, {sphere}); 
  NodePtr mercury_orbit_disk = Node::Make(mercury_orbit_disk_transform, {mercury});
  NodePtr earth = Node::Make(shd_tex, earth_transform, {white, earth_texture}, {sphere}); 
  NodePtr earth_orbit_disk = Node::Make(earth_orbit_disk_transform, {earth});
  NodePtr moon = Node::Make(shd_tex, moon_transform, {white, moon_texture}, {sphere}); 
  NodePtr moon_orbit_disk = Node::Make(moon_orbit_disk_transform, {moon});
  NodePtr sky_node = Node::Make(shd_sky, {sky}, {std::static_pointer_cast<Shape>(skybox) });
  earth->AddNode(moon_orbit_disk);

  // add os nós com oreferência para o target
  NodePtr sun_target = sun;
  NodePtr mercury_target = mercury;
  NodePtr earth_target = earth;
  NodePtr moon_target = moon;

  // build scene
  NodePtr root = Node::Make(shader,
    {
                            sun,
                            mercury_orbit_disk,
                            earth_orbit_disk,
                            sky_node
    }
  );
  scene = Scene::Make(root);
  scene->AddEngine(Orbit::Make(earth_orbit_disk_transform, 2.5f));
  scene->AddEngine(Orbit::Make(mercury_orbit_disk_transform, 4.5f));
  scene->AddEngine(Orbit::Make(moon_orbit_disk_transform, 4.5f));
  scene->AddEngine(Orbit::Make(earth_transform, 5.0f));
  scene->AddEngine(Orbit::Make(moon_transform, 3.0f));
  scene->AddEngine(Orbit::Make(mercury_transform, 7.0f));

  camera_target_engine = CameraTarget::Make(camera, sun_target, mercury_target, earth_target, moon_target);
  scene->AddEngine(camera_target_engine);
}

static void update(float dt)
{
  scene->Update(dt);

  //lógica para a troca de alvo da
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
  else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    camera_target_engine->m_current_target = Sun;
  else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    camera_target_engine->m_current_target = Mercury;
  else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    camera_target_engine->m_current_target = Earth;
  else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    camera_target_engine->m_current_target = Moon;
}

static void resize (GLFWwindow* win, int width, int height)
{
  glViewport(0,0,width,height);
}

static void cursorpos (GLFWwindow* win, double x, double y)
{
  // convert screen pos (upside down) to framebuffer pos (e.g., retina displays)
  int wn_w, wn_h, fb_w, fb_h;
  glfwGetWindowSize(win, &wn_w, &wn_h);
  glfwGetFramebufferSize(win, &fb_w, &fb_h);
  x = x * fb_w / wn_w;
  y = (wn_h - y) * fb_h / wn_h;
  arcball->AccumulateMouseMotion(int(x),int(y));
}
static void cursorinit (GLFWwindow* win, double x, double y)
{
  // convert screen pos (upside down) to framebuffer pos (e.g., retina displays)
  int wn_w, wn_h, fb_w, fb_h;
  glfwGetWindowSize(win, &wn_w, &wn_h);
  glfwGetFramebufferSize(win, &fb_w, &fb_h);
  x = x * fb_w / wn_w;
  y = (wn_h - y) * fb_h / wn_h;
  arcball->InitMouseMotion(int(x),int(y));
  glfwSetCursorPosCallback(win, cursorpos);     // cursor position callback
}
static void mousebutton (GLFWwindow* win, int button, int action, int mods)
{
  if (action == GLFW_PRESS) {
    glfwSetCursorPosCallback(win, cursorinit);     // cursor position callback
  }
  else // GLFW_RELEASE 
    glfwSetCursorPosCallback(win, nullptr);      // callback disabled
}

int main ()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);       // required for mac os
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER,GLFW_TRUE);  // option for mac os
#endif

  glfwSetErrorCallback(error);

  GLFWwindow* win = glfwCreateWindow(600,400,"Window title",nullptr,nullptr);
  assert(win);
  glfwSetFramebufferSizeCallback(win, resize);  // resize callback
  glfwSetKeyCallback(win, keyboard);            // keyboard callback
  glfwSetMouseButtonCallback(win, mousebutton); // mouse button callback
  
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
    update(t - t0);
    t0 = t;
    display(win);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

