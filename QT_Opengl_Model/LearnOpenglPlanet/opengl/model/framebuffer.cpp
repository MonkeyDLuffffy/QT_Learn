#include "framebuffer.h"
#include "myopenglwidget.h"


//顶点数据
GLfloat quad1Vertices[] = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};


frameBuffer::frameBuffer(QOpenGLFunctions_3_3_Core * glfuns,MyOpenglWidget* glwidget)
{
    m_glfuns=glfuns;
    m_glwidget=glwidget;
    initScreen();
    initFbo();
}

frameBuffer::~frameBuffer()
{
    freeScreen();
    freeFbo();
}

void frameBuffer::initScreen()
{



     MyOpenglWidget::genShader(screenShaderProgram,":/shader/screen.vert",":/shader/screen.frag");


     m_glfuns->glGenVertexArrays(1,&VAO);
     m_glfuns->glGenBuffers(1, &VBO);

     m_glfuns->glBindVertexArray(VAO);
     m_glfuns->glBindBuffer(GL_ARRAY_BUFFER, VBO);

     m_glfuns->glBufferData(GL_ARRAY_BUFFER,sizeof(quad1Vertices),&quad1Vertices[0], GL_STATIC_DRAW);




     m_glfuns->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
     m_glfuns->glEnableVertexAttribArray(0);

     m_glfuns->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(sizeof(float)*2));
     m_glfuns->glEnableVertexAttribArray(1);


     m_glfuns->glBindVertexArray(0);



       screenShaderProgram.bind();
      screenShaderProgram.setUniformValue("screenTexture", 0);
      screenShaderProgram.release();

        screenTexture=new QOpenGLTexture(QImage(":/iamge/container2.png").mirrored());
}

void frameBuffer::initFbo()
{
  initSingleFbo();
  initMultiFbo();
}

void frameBuffer::initSingleFbo()
{
    m_glfuns->glGenFramebuffers(1, &framebuffer);
    m_glfuns->glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
   // create a color attachment texture

    m_glfuns->glGenTextures(1, &textureColorbuffer);
    m_glfuns->glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    m_glfuns->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_glwidget->width(), m_glwidget->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    m_glfuns->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_glfuns->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_glfuns-> glBindTexture(GL_TEXTURE_2D, 0);

    m_glfuns->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
   // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

    m_glfuns->glGenRenderbuffers(1, &rbo);
    m_glfuns->glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    m_glfuns->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_glwidget->width(), m_glwidget->height()); // use a single renderbuffer object for both a depth AND stencil buffer.
    m_glfuns->glBindRenderbuffer(GL_RENDERBUFFER, 0);

    m_glfuns->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
   // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
   if (m_glfuns->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
       qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" ;
    m_glfuns->glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void frameBuffer::initMultiFbo()
{
    //创建一个自定义的multiSample帧缓冲
    m_glfuns-> glGenFramebuffers(1, &fboMultiSample);
    m_glfuns-> glBindFramebuffer(GL_FRAMEBUFFER, fboMultiSample);
    m_glfuns-> glGenTextures(1, &multiSampleTex);
     m_glfuns->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multiSampleTex);
     m_glfuns->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB,m_glwidget-> width(),m_glwidget-> height(), GL_TRUE);
    m_glfuns-> glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    m_glfuns-> glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multiSampleTex, 0);
    m_glfuns-> glGenRenderbuffers(1, &rboMultiSample);
    m_glfuns-> glBindRenderbuffer(GL_RENDERBUFFER, rboMultiSample);
    m_glfuns-> glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8,m_glwidget-> width(), m_glwidget->height());
    m_glfuns-> glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboMultiSample);
    //善后工作
    if (m_glfuns->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    m_glfuns-> glBindFramebuffer(GL_FRAMEBUFFER,m_glwidget-> defaultFramebufferObject() );
}

void frameBuffer::paintScreen()
{

    paintMultiScreen();
}

void frameBuffer::paintSingleScreen()
{
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        m_glfuns->  glBindFramebuffer(GL_FRAMEBUFFER, m_glwidget->defaultFramebufferObject());
        m_glfuns->  glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
         // clear all relevant buffers
        m_glfuns->  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        m_glfuns->  glClear(GL_COLOR_BUFFER_BIT);

         screenShaderProgram.bind();
        // screenVao.bind();
         m_glfuns->glBindVertexArray(VAO);
            //screenTexture->bind(0);
         m_glfuns-> glActiveTexture(GL_TEXTURE0);
         m_glfuns-> glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane


         m_glfuns-> glDrawArrays(GL_TRIANGLES, 0, 6);


    screenShaderProgram.bind();
    screenVao.bind();


    screenShaderProgram.release();
}

void frameBuffer::paintMultiScreen()
{

     m_glfuns->  glBindFramebuffer(GL_READ_FRAMEBUFFER, fboMultiSample);
     m_glfuns-> glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
     m_glfuns->  glBlitFramebuffer(0, 0,m_glwidget-> width(), m_glwidget->height(), 0, 0,m_glwidget-> width(),m_glwidget-> height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        m_glfuns->  glBindFramebuffer(GL_FRAMEBUFFER, m_glwidget->defaultFramebufferObject());
        m_glfuns->  glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
         // clear all relevant buffers
        m_glfuns->  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        m_glfuns->  glClear(GL_COLOR_BUFFER_BIT);

         screenShaderProgram.bind();
         m_glfuns->glBindVertexArray(VAO);
            //screenTexture->bind(0);
         m_glfuns-> glActiveTexture(GL_TEXTURE0);
         m_glfuns-> glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane


         m_glfuns-> glDrawArrays(GL_TRIANGLES, 0, 6);


    screenShaderProgram.bind();
    screenVao.bind();


    screenShaderProgram.release();

      //激活纹理
}

void frameBuffer::paintFbo()
{
    //paintSingleFbo();
    paintMultiFbo();
}

void frameBuffer::paintSingleFbo()
{
    m_glfuns->glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    m_glfuns->  glDepthFunc(GL_LESS);
    m_glfuns->   glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

      //渲染自定义帧缓冲
        // fboBuffer->bind();
      m_glfuns->  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      // m_glfuns->  glEnable(GL_DEPTH_TEST);
      m_glfuns->   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      //激活纹理
}

void frameBuffer::paintMultiFbo()
{
      m_glfuns->glBindFramebuffer(GL_FRAMEBUFFER, fboMultiSample);
      m_glfuns->  glDepthFunc(GL_LESS);
      m_glfuns->   glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

      m_glfuns->  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

      m_glfuns->   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void frameBuffer::freeScreen()
{
screenVao.destroy();
screenVbo.destroy();
}

void frameBuffer::freeFbo()
{
    if(fboBuffer){
         delete fboBuffer;
     }

}


void frameBuffer::resetFbo()
{



}
