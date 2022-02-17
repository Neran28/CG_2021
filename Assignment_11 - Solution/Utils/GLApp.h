#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <chrono>

#include "GLDebug.h"
#include "Mat4.h"


struct Dimensions
{
    uint32_t width;
    uint32_t height;

    float aspect() const { return float(width) / float(height); }
};

class GLApp
{
protected:
    GLFWwindow* window;

private:
    static GLApp* staticAppPtr;
    std::string title;
    bool fpsCounter;
    std::chrono::high_resolution_clock::time_point last;
    uint64_t frameCount;

public:
    GLApp(uint32_t w = 800, uint32_t h = 600, uint32_t s = 4, const std::string& title = "My OpenGL App",
        bool fpsCounter = true, bool sync = true);
       
    ~GLApp();

    void run();

    virtual void init() {};
    virtual void draw() {};
    virtual void animate(double animationTime) {};

    virtual void resize(int width, int height);
    virtual void keyboard(int key, int scancode, int action, int mods) {}
    virtual void mouseMove(double xPosition, double yPosition) {}
    virtual void mouseButton(int button, int state, int mods, double xPosition, double yPosition) {}
    virtual void mouseWheel(double x_offset, double y_offset, double xPosition, double yPosition) {}

    void setTitle(const std::string& title);

private:

    Dimensions getFramebufferSize() const;
    Dimensions getWindowSize() const;

    void endOfFrame();

    static void errorCallback(int error, const char* description)
    {
        std::stringstream s;
        s << description << " " << error;
        throw GLException(s.str());
    }

    static void resizeCallback(GLFWwindow* window, int width, int height)
    {
        if (staticAppPtr)
            staticAppPtr->resize(width, height);
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (staticAppPtr) staticAppPtr->keyboard(key, scancode, action, mods);
    }

    static void cursorPositionCallback(GLFWwindow* window, double xPosition, double yPosition)
    {
        if (staticAppPtr) staticAppPtr->mouseMove(xPosition, yPosition);
    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int state, int mods)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (staticAppPtr)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            staticAppPtr->mouseButton(button, state, mods, xpos, ypos);
        }
    }

    static void scrollCallback(GLFWwindow* window, double x_offset, double y_offset)
    {
        if (staticAppPtr)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            staticAppPtr->mouseWheel(x_offset, y_offset, xpos, ypos);
        }
    }
};

