#include <GLApp.h>
#include "GLHelper.h"

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif // __APPLE__

class MyGLApp : public GLApp
{
private:
    double time{};
    bool animate = false;
    bool reset = false;
    const float degreePreSecond{ 45.0f };
    float angle{ 0 };

    Mat4 modelView = Mat4();
    Mat4 projection = Mat4();
    GLuint program{};
    GLint modelViewMatrixUniform = -1;
    GLint projectionMatrixUniform = -1;

    GLuint vbos[2]{ };
    GLuint vaos[3]{ };


    inline static float sqrt3{ sqrtf(3) };

    const GLfloat vertexPositions[9] = {
         0.0f, sqrt3, 0.0f,
        -1.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 0.0f
    };

    inline static GLfloat vertexColors[] = {
        1, 0, 0,
        1, 0, 1,
        1, 1, 0,

        1, 1, 0,
        0, 1, 1,
        0, 1, 0,

        1, 0, 1,
        0, 0, 1,
        0, 1, 1
    };

public:


    virtual void init() override
    {
        const GLubyte* glVersion = glGetString(GL_VERSION); GLCheckError();
        const GLubyte* slVersion = glGetString(GL_SHADING_LANGUAGE_VERSION); GLCheckError();
        std::cout << glVersion << std::endl;
        std::cout << slVersion << std::endl;
		
		setTitle("Assignment 02 - Hello Triangles");
        time = glfwGetTime();
        setupShaders();
        setupGeometry();
    }

    virtual void draw() override
    {
        double t = glfwGetTime();
        double d = t - time;
        time = t;

        if (animate)
            angle += degreePreSecond * d;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GLCheckError();
        glClear(GL_COLOR_BUFFER_BIT); GLCheckError();

        // render upper triangle
        glUseProgram(program); GLCheckError();
        modelView = Mat4();
        modelView = modelView * Mat4::translation(0, -sqrt3 / 2, 0);
        modelView = modelView * Mat4::rotationX(angle);
        modelView = modelView * Mat4::translation(0.0f, sqrt3 / 2, 0.0f);
        glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_TRUE, modelView); GLCheckError();
        glBindVertexArray(vaos[0]); GLCheckError();
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexPositions) / sizeof(vertexPositions[0]) / 3); GLCheckError();

        // render right triangle
        modelView = Mat4();
        modelView = modelView * Mat4::rotationY(angle);
        modelView = modelView * Mat4::translation(1, -sqrt3, 0);
        glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_TRUE, modelView); GLCheckError();
        glBindVertexArray(vaos[1]); GLCheckError();
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexPositions) / sizeof(vertexPositions[0]) / 3); GLCheckError();


        // render left triangle
        modelView = Mat4();
        modelView = modelView * Mat4::translation(0, -sqrt3 / 3, 0);
        modelView = modelView * Mat4::rotationZ(angle);
        modelView = modelView * Mat4::translation(0, sqrt3 / 3, 0);
        modelView = modelView * Mat4::translation(-1, -sqrt3, 0);
        glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_TRUE, modelView); GLCheckError();
        glBindVertexArray(vaos[2]); GLCheckError();
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexPositions) / sizeof(vertexPositions[0]) / 3); GLCheckError();

        glUseProgram(0); GLCheckError();
    }

    virtual void resize(int width, int height) override
    {
        float ratio = static_cast<float>(width) / static_cast<float>(height);
        std::cout << "reshaping window, offset: (0 0) resolution: " << width << "x"
            << height << " ratio: " << ratio << std::endl;

        projection = Mat4::ortho(-ratio * sqrt3, ratio * sqrt3, -sqrt3, sqrt3, -10.0f, 10.0f);
        glUseProgram(program); GLCheckError();
        glUniformMatrix4fv(projectionMatrixUniform, 1, GL_TRUE, projection); GLCheckError();
        glUseProgram(0); GLCheckError();
        glViewport(0, 0, width, height);
    }

    void setupShaders()
    {
        std::string pathToExe = "";
#ifdef __APPLE__
        char path[1024];
        uint32_t size = sizeof(path);
        _NSGetExecutablePath(path, &size);
        pathToExe = std::string(path);
        while (pathToExe.size() > 0 && pathToExe.back() != '/') pathToExe.pop_back();
#endif
        std::string vertexSrcPath = pathToExe + "res/shaders/vertexShader.vert";
        std::string fragmentSrcPath = pathToExe + "res/shaders/fragmentShader.frag";
        std::string vertexShaderSource(vertexSrcPath);
        std::string fragmentShaderSource(fragmentSrcPath);
        GLuint vertexShader = Shader::createShaderFromFile(GL_VERTEX_SHADER, vertexShaderSource);
        GLuint fragmentShader = Shader::createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderSource);

        program = glCreateProgram(); GLCheckError();
        glAttachShader(program, vertexShader); GLCheckError();
        glAttachShader(program, fragmentShader); GLCheckError();
        glLinkProgram(program);
        Shader::checkAndThrowProgram(program);

        glUseProgram(program);
        modelViewMatrixUniform = glGetUniformLocation(program, "modelViewMatrix"); GLCheckError();
        projectionMatrixUniform = glGetUniformLocation(program, "projectionMatrix"); GLCheckError();
        glUseProgram(0); GLCheckError();
    }

    void setupGeometry()
    {
        GLint vertexPositionLocation = glGetAttribLocation(program, "vertexPosition"); GLCheckError();
        GLint vertexColorLocation = glGetAttribLocation(program, "vertexColor"); GLCheckError();

        glGenBuffers(2, vbos); GLCheckError();
        glGenVertexArrays(3, vaos); GLCheckError();

        // create position VBO
        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]); GLCheckError();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW); GLCheckError();

        // create color VBO
        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]); GLCheckError();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW); GLCheckError();

        // define VAO for upper triangle
        glBindVertexArray(vaos[0]); GLCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]); GLCheckError();
        glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); GLCheckError();
        glEnableVertexAttribArray(vertexPositionLocation); GLCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]); GLCheckError();
        glVertexAttribPointer(vertexColorLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); GLCheckError();
        glEnableVertexAttribArray(vertexColorLocation); GLCheckError();

        // define VAO for right triangle
        glBindVertexArray(vaos[1]); GLCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]); GLCheckError();
        glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); GLCheckError();
        glEnableVertexAttribArray(vertexPositionLocation); GLCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]); GLCheckError();
        glVertexAttribPointer(vertexColorLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)(9 * sizeof(float)) ); GLCheckError();
        glEnableVertexAttribArray(vertexColorLocation); GLCheckError();

        // define VAO for left triangle
        glBindVertexArray(vaos[2]); GLCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]); GLCheckError();
        glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); GLCheckError();
        glEnableVertexAttribArray(vertexPositionLocation); GLCheckError();
        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]); GLCheckError();
        glVertexAttribPointer(vertexColorLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)(18 * sizeof(float))); GLCheckError();
        glEnableVertexAttribArray(vertexColorLocation); GLCheckError();

        glBindVertexArray(0); GLCheckError();
    }

    virtual void keyboard(int key, int scancode, int action, int mods) override
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            animate = !animate;

        if (key == GLFW_KEY_R && action == GLFW_PRESS)
            angle = 0;
    }
};

int main(int argc, char** argv)
{
    MyGLApp myApp;
    myApp.run();
    return EXIT_SUCCESS;
}