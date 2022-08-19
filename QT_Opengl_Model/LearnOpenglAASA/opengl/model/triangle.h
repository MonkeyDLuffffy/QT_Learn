#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "iglbase.h"

class Trianglea:public IGLBase
{
public:
    Trianglea(QOpenGLFunctions_3_3_Core* glfun):IGLBase(glfun)
    {
        setupMesh();
    }

    virtual void Draw(QOpenGLShaderProgram &shader) override;
    virtual void setupMesh() override;
    virtual void Draw()override;



};

#endif // TRIANGLE_H
