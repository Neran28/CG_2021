#include "GLApp.h"
#include <cmath>

typedef std::chrono::high_resolution_clock Clock;

GLApp* GLApp::staticAppPtr = nullptr;

GLApp::GLApp(uint32_t w, uint32_t h, uint32_t s, const std::string& title,
    bool fpsCounter, bool sync)
    : window(nullptr), title(title), fpsCounter(fpsCounter), last(Clock::now())
{
    staticAppPtr = this;
    glfwSetErrorCallback(errorCallback);
    /* Initialize the library */
    if (!glfwInit())
        throw GLException{ "GLFW init failed." };

    glfwWindowHint(GLFW_SAMPLES, s);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::stringstream s;
        s << "Failed to open GLFW window.";
        glfwTerminate();
        throw GLException(s.str());
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err(glewInit());
    if (err != GLEW_OK)
    {
        std::stringstream s;
        s << "Failed to init GLEW " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        throw GLException(s.str());
    }

    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwSwapInterval(1);
    glfwSetTime(0);

    Dimensions dim{ getFramebufferSize() };
    glViewport(0, 0, dim.width, dim.height); GLCheckError();
}

GLApp::~GLApp()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GLApp::run()
{
    init();

    // overriden resize may set projection, so updating is necessary
    Dimensions dim{ getFramebufferSize() };
    resize(dim.width, dim.height);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        draw();
        endOfFrame();
    }
}

void GLApp::endOfFrame()
{
    glfwSwapBuffers(window);
    glfwPollEvents();

    if (fpsCounter)
    {
        frameCount++;
        auto now = Clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::microseconds>(now - last);
        if (diff.count() > 1e6)
        {
            auto fps = static_cast<double>(frameCount) / diff.count() * 1.e6;
            std::stringstream s;
            s << title << " (" << static_cast<int>(std::ceil(fps)) << " fps)";
            glfwSetWindowTitle(window, s.str().c_str());
            frameCount = 0;
            last = now;
        }
    }
}

void GLApp::resize(int width, int height)
{
    Dimensions dim{ getFramebufferSize() };
    glViewport(0, 0, dim.width, dim.height); GLCheckError();
}

void GLApp::setTitle(const std::string& title)
{
    this->title = title;
    glfwSetWindowTitle(window, title.c_str());
}

Dimensions GLApp::getFramebufferSize() const
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return Dimensions{ uint32_t(width), uint32_t(height) };
}

Dimensions GLApp::getWindowSize() const
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return Dimensions{ uint32_t(width), uint32_t(height) };
}