
#ifdef _WIN32
#include <glad/glad.h>
#elif __linux__
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#else
#include <OpenGL/gl3.h>
#endif

#include <GLFW/glfw3.h>

#include "scene.h"
#include "state.h"
#include "camera2d.h"
#include "color.h"
#include "texture.h"
#include "transform.h"
#include "error.h"
#include "shader.h"
#include "disk.h"
#include "triangle.h"
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

static ScenePtr scene;
static CameraPtr camera;

class MovePointer;
using MovePointerPtr = std::shared_ptr<MovePointer>;
class MovePointer : public Engine 
{
  TransformPtr m_trf;
  float m_period = 0;
protected:
  MovePointer (TransformPtr trf, float period=365) 
  : m_trf(trf), m_period(period) {}
public:
  static MovePointerPtr Make (TransformPtr trf, float period=365)
  {
    return MovePointerPtr(new MovePointer(trf, period / 10));
  }
  virtual void Update (float dt)
  {
    m_trf->Rotate(dt * 1000 / m_period, 0, 0, 1);
  }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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

static void initialize (void)
{
    ShaderPtr shader = Shader::Make();
    shader->AttachVertexShader("shaders/vertex.glsl");
    shader->AttachFragmentShader("shaders/fragment.glsl");
    shader->Link();

    glEnable(GL_DEPTH_TEST);
    camera = Camera2D::Make(-20,20,-20,20);
    auto earthTex = Texture::Make("face", glm::vec3(0, 0, 1));
    auto sunTex = Texture::Make("face", glm::vec3(1, 1, 0.9));
    auto moonTex = Texture::Make("face", glm::vec3(0.3, 0.3, 0.3));
    auto mercuryTex = Texture::Make("face", glm::vec3(0.4, 0.4, 0.4));


    auto sunScale = Transform::Make();
    sunScale->Scale(3, 3, 1);
    auto sun = Node::Make(shader, sunScale, {Color::Make(1, 1, 1), sunTex}, {Disk::Make()});
    auto earth = Node::Make(shader, {Color::Make(1, 1, 1), earthTex}, {Disk::Make()});

    auto moonTransScale = Transform::Make();
    moonTransScale->Translate(2, 0, 0);
    moonTransScale->Scale(0.25, 0.25, 1);
    auto moon = Node::Make(shader, moonTransScale, {Color::Make(1, 1, 1), moonTex}, {Disk::Make()});

    auto moonOrbitTrans = Transform::Make();
    auto moonOrbit = Node::Make(shader, moonOrbitTrans, {moon});

    auto earthMoonTrans = Transform::Make();
    earthMoonTrans->Translate(15, 0, 0);
    auto earthMoon = Node::Make(shader, earthMoonTrans, {moonOrbit, earth});

    auto earthMoonOrbitTrans = Transform::Make();
    auto earthMoonOrbit = Node::Make(shader, earthMoonOrbitTrans, {earthMoon});

    auto mercuryTrans = Transform::Make();
    mercuryTrans->Translate(8, 0, 0);
    mercuryTrans->Scale(0.5, 0.5, 1);
    auto mercury = Node::Make(shader, mercuryTrans, {Color::Make(1, 1, 1), mercuryTex}, {Disk::Make()});

    auto mercuryOrbitTrans = Transform::Make();
    auto mercuryOrbit = Node::Make(shader, mercuryOrbitTrans, {mercury});

    auto root = Node::Make(shader, {sun, mercuryOrbit, earthMoonOrbit});
  
    scene = Scene::Make(root);
    scene->AddEngine(MovePointer::Make(moonOrbitTrans, 28));
    scene->AddEngine(MovePointer::Make(earthMoonOrbitTrans));
    scene->AddEngine(MovePointer::Make(mercuryOrbitTrans, 88));

}

static void display (GLFWwindow* win)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear window 
    Error::Check("before render");
    scene->Render(camera);
    Error::Check("after render");
}
int main() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);       // required for mac os
    #ifndef _WIN32
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER,GLFW_TRUE);  // option for mac os
    #endif

    glfwSetErrorCallback(error);

    GLFWwindow* win = glfwCreateWindow(WIDTH, HEIGHT, "Tarefa 3", NULL, NULL);
    if (!win) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(win, resize);  // resize callback
    glfwSetKeyCallback(win, keyboard);            // keyboard callback

    glfwMakeContextCurrent(win);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    initialize();

    float t0 = glfwGetTime();
    while(!glfwWindowShouldClose(win)) {
        float t = glfwGetTime();
        update(t-t0);
        t0 = t;
        display(win);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
    }
