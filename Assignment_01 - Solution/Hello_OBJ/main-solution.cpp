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
    for (const OBJFile::IndexType& triangle : m.indices) {
      for (const size_t& index : triangle) {
        data_bunny.push_back(m.vertices[index][0]);
        data_bunny.push_back(m.vertices[index][1]);
        data_bunny.push_back(m.vertices[index][2]);
         
        data_bunny.push_back(m.vertices[index][0]+0.5f);
        data_bunny.push_back(m.vertices[index][1]+0.5f);
        data_bunny.push_back(m.vertices[index][2]+0.5f);
        data_bunny.push_back(1.0f);

        data_bunny.push_back(m.normals[index][0]);
        data_bunny.push_back(m.normals[index][1]);
        data_bunny.push_back(m.normals[index][2]);
      }
    }

    const OBJFile c{ "cube.obj", true };
    for (const OBJFile::IndexType& triangle : c.indices)
    {
        for (const size_t& index : triangle)
        {
            data_cube.push_back(c.vertices[index][0]);
            data_cube.push_back(c.vertices[index][1]);
            data_cube.push_back(c.vertices[index][2]);

            data_cube.push_back(c.vertices[index][0] + 0.5f);
            data_cube.push_back(c.vertices[index][1] + 0.5f);
            data_cube.push_back(c.vertices[index][2] + 0.5f);
            data_cube.push_back(1.0f);
        }
    }
  }
  
  virtual void keyboard(int key, int scancode, int action, int mods) override {
      if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
          drawCube = !drawCube;
  }
  
  virtual void animate(double animationTime) {
    angle = animationTime*30;
  }
  
  virtual void draw() {
    GL(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));
    setDrawProjection(Mat4::perspective(45, glEnv.getFramebufferSize().aspect(), 0.0001, 100));
    setDrawTransform(Mat4::rotationY(float(angle)) * Mat4::lookAt({0,0,2},{0,0,0},{0,1,0}));
    if(!drawCube)
        drawTriangles(data_bunny, TrisDrawType::TD_LIST, false, true);
    else
        drawTriangles(data_cube, TrisDrawType::TD_LIST, false, false);
  }

} myApp;

int main(int argc, char ** argv) {
  myApp.run();
  return EXIT_SUCCESS;
}  
