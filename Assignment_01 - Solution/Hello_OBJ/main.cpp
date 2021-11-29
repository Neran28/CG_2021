#include <GLApp.h>
#include <OBJFile.h>
#include <Mat4.h>

class MyGLApp : public GLApp {
public:
  double angle = 0;
  std::vector<float> data_bunny;
  std::vector<float> data_cube;
  bool drawCube = false;
  
  virtual void init() {
    glEnv.setTitle("Shared vertices to explicit representation demo");
    GL(glDisable(GL_CULL_FACE));
    GL(glEnable(GL_DEPTH_TEST));
    GL(glClearColor(0,0,0,0));

    const OBJFile m{"bunny.obj", true};

    // TODO Ex1:
    // Replace this example block of code by your code to
    // convert the shared vertex represenation in the object m
    // by an explicit representation, indices are stored in
    // the stl-vector m.indices the vertex positions are stored
    // in m.vertices and the normals are stored in m.normals.
    // As color you can choose whatever you like
    
    data_bunny.push_back(0.0f); data_bunny.push_back(0.5f); data_bunny.push_back(0.0f);  // position
    data_bunny.push_back(1.0f); data_bunny.push_back(0.0f); data_bunny.push_back(0.0f); data_bunny.push_back(1.0f); // color
    data_bunny.push_back(0.0f); data_bunny.push_back(0.0f); data_bunny.push_back(1.0f); // normal

    data_bunny.push_back(-0.5f); data_bunny.push_back(-0.5f); data_bunny.push_back(0.0f);
    data_bunny.push_back(0.0f); data_bunny.push_back(0.0f); data_bunny.push_back(1.0f); data_bunny.push_back(1.0f);
    data_bunny.push_back(0.0f); data_bunny.push_back(0.0f); data_bunny.push_back(1.0f);

    data_bunny.push_back(0.5f); data_bunny.push_back(-0.5f); data_bunny.push_back(0.0f);
    data_bunny.push_back(0.0f); data_bunny.push_back(1.0f); data_bunny.push_back(0.0f); data_bunny.push_back(1.0f);
    data_bunny.push_back(0.0f); data_bunny.push_back(0.0f); data_bunny.push_back(1.0f);

    // example block end
  }

  virtual void keyboard(int key, int scancode, int action, int mods) override
  {
      if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
          drawCube = !drawCube;
  }
  
  virtual void animate(double animationTime) {
    angle = animationTime*30;
  }
  
  virtual void draw() {
    GL(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));
    setDrawProjection(Mat4::perspective(45, glEnv.getFramebufferSize().aspect(), 0.0001f, 100.0f));
    setDrawTransform(Mat4::rotationY(float(angle)) * Mat4::lookAt({0,0,2},{0,0,0},{0,1,0}));
    if (!drawCube)
        drawTriangles(data_bunny, TrisDrawType::TD_LIST, false, true);
    else
        drawTriangles(data_cube, TrisDrawType::TD_LIST, false, false);
  }

} myApp;

int main(int argc, char ** argv) {
  myApp.run();
  return EXIT_SUCCESS;
}  
