#include "myopenglwidget.h"

#include "QDebug"
#include <QVector3D>

const QVector<QVector3D> cubePositions= {
QVector3D( 0.0f, 0.0f, 0.0f),
QVector3D( 2.0f, 5.0f, -15.0f),
QVector3D(-1.5f, -2.2f, -2.5f),
QVector3D(-3.8f, -2.0f, -12.3f),
QVector3D( 2.4f, -0.4f, -3.5f),
QVector3D(-1.7f, 3.0f, -7.5f),
QVector3D( 1.3f, -2.0f, -2.5f),
QVector3D( 1.5f, 2.0f, -2.5f),
QVector3D( 1.5f, 0.2f, -1.5f),
QVector3D(-1.3f, 1.0f, -1.5f)
};

float vertices_N[] = {
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
   };

MyOpenglWidget::MyOpenglWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    //激活键盘鼠标事件
    setFocusPolicy(Qt::StrongFocus);

    setMouseTracking(true);

    connect(&timer,SIGNAL(timeout()),this,SLOT(on_timeout()));
    timer.start(10);
}

MyOpenglWidget::~MyOpenglWidget()
{

    makeCurrent();
    for(int i=0;i<ActorNum;++i)
    {
      glDeleteVertexArrays(1,&ActorVector[i]->glData.VAO);
      glDeleteBuffers(1,&ActorVector[i]->glData.VBO);
     // glDeleteShader(ActorVector[i]->m_shader);

      delete ActorVector[i];
    }
    ActorVector.empty();

    if(m_texture1)
    {
       delete m_texture1;
        m_texture1=nullptr;
    }


    doneCurrent();
}

void MyOpenglWidget::drawShape(EShape shape)
{
    m_shape=shape;

    //如果需要从paintGL（）以外的位置触发重新绘制（典型示例是使用计时器设置场景动画），则应调用widget的update（）函数来安排更新。

    update();
}

void MyOpenglWidget::setWireFrame(bool wireFrame)
{
    //如果需要从除了initializeGL()，paintGL（）、resizeGL（）其他位置调用标准OpenGL API函数，则必须首先调用makeCurrent();
    makeCurrent();
    if(wireFrame)
    {
        //线框模式
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    else
    {
        //填充模式
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }

    update();

}
QVector3D lightPos(1.2f, 1.0f, 2.0f);
QVector3D objectPos(-1.7f, 3.0f, -7.5f);
QVector3D objectScale(5.0f, 5.0f, 5.0f);


void MyOpenglWidget::initializeGL()
{
    initializeOpenGLFunctions();
    //开始计时
    StartTime.start();

    //开启透明度
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    //开启深度
    glEnable(GL_DEPTH_TEST);

    qDebug()<<"11111111111";
    textureWall=new QOpenGLTexture(QImage(":/iamge/wall.jpg").mirrored());
    textureSmile=new QOpenGLTexture(QImage(":/iamge/awesomeface.png").mirrored());

    m_shape=Rect;
          Actor*  actorLight=new Actor(this,":/shaders/object.vs",":/shaders/object.fs",":/iamge/wall.jpg");
          actorLight->InitModel(QVector3D(10,10,10),lightPos,0,QVector3D(0,0,1));
          ActorVector.push_back(actorLight);


          Actor* actorObject=new Actor(this,":/shaders/object.vs",":/shaders/object.fs",":/iamge/wall.jpg");
          actorObject->InitModel(objectScale,objectPos,0,QVector3D(0,0,1));
          ActorVector.push_back(actorObject);

          ActorNum=ActorVector.size();
          for(int i=0;i<ActorNum;++i)
          {
              glGenVertexArrays(1, &ActorVector[i]->glData.VAO);
              glGenBuffers(1, &ActorVector[i]->glData.VBO);

              glBindVertexArray(ActorVector[i]->glData.VAO);
              glBindBuffer(GL_ARRAY_BUFFER, ActorVector[i]->glData.VBO);

              //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
              glBufferData(GL_ARRAY_BUFFER,sizeof(vertices_N) ,vertices_N, GL_STATIC_DRAW);
              glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
              glEnableVertexAttribArray(0);

              glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
              //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)3);
              glEnableVertexAttribArray(1);

              glBindVertexArray(0);
              glBindBuffer(GL_ARRAY_BUFFER, 0);

              ActorVector[i]->m_shader.bind();
              if(i!=0)
              {
                  ActorVector[i]->m_shader.setUniformValue("ratio",ratio);
                  ActorVector[i]->m_shader.setUniformValue("objectColor",1.0f, 0.5f, 0.31f);
                  ActorVector[i]->m_shader.setUniformValue("lightColor",1.0f, 1.0f, 1.0f);
                  ActorVector[i]->m_shader.setUniformValue("lithtPos",ActorVector[0]->Position);
              }

          }


}

void MyOpenglWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
    //glViewport(0,0,w,h);
}

void MyOpenglWidget::paintGL()
{

    CurrentTime=StartTime.elapsed()/1000.0;
    deltaTime=CurrentTime-lastTime;
    lastTime=CurrentTime;

    QMatrix4x4 view;
    QMatrix4x4 projection;


    for(int i=0;i<8;++i)
    {
        int temp=1 <<i;
        if(keyboard &temp)
        {
            if(i<6)
            m_camera.ProcessKeyboard((Camera_Movement)i,deltaTime);
            else if(i==6)
            {
                ratio+=0.5*deltaTime;
                ratio=ratio>1?1:ratio;
            }
            else if(i==7)
            {
                ratio-=0.5*deltaTime;
                ratio=ratio<0?0:ratio;
            }
            qDebug()<<ratio;
        }

    }

    //model.rotate(CurrentTime*45,1,1,0);
    //view.translate(0,0,-3);

    view=m_camera.GetViewMatrix();
    projection.perspective(m_camera.Zoom,(float)width()/height(),0.1,1000);

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //int ActorNum=ActorVector.size();
    //float borderColor[]={0.2f,0.3f,0.3f,1.0f};

    switch (m_shape) {
    case Rect:
        for(int i=0;i<ActorNum;++i)
        {
            ActorVector[i]->m_shader.setUniformValue("view",view);
            ActorVector[i]->m_shader.setUniformValue("projection",projection);
            if(i!=0)
            {
                ActorVector[i]->m_shader.setUniformValue("viewPos",m_camera.Position);
            }

            glBindVertexArray(ActorVector[i]->glData.VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        break;
    default:
        break;
    }
}
#include<QKeyEvent>
void MyOpenglWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:

    keyboard|= 1<<6;
    break;
    case Qt::Key_Down:
    keyboard|= 1<<7;

    break;

    case Qt::Key_W:
        keyboard|= 1<<2;
        break;
    case Qt::Key_S:
        keyboard|= 1<<3;
        break;
    case Qt::Key_D:
        keyboard|= 1<<4;
        break;
    case Qt::Key_A:
        keyboard|= 1<<5;
        break;
    case Qt::Key_R:
        m_camera.ResetCamera();
        break;

    }

    ActorVector[0]->m_shader.bind();
    ActorVector[0]->m_shader.setUniformValue("ratio",ratio);

    update();
}


void MyOpenglWidget::keyReleaseEvent(QKeyEvent *event)
{
    int Temp;
    switch(event->key())
    {
    case Qt::Key_Up:
        Temp=1<<6;
        Temp = ~Temp;
        keyboard &=Temp;
    break;
    case Qt::Key_Down:
        Temp=1<<7;
        Temp = ~Temp;
        keyboard &=Temp;
    break;


    case Qt::Key_W:
        Temp=1<<2;
        Temp = ~Temp;
        keyboard &=Temp;
        break;
    case Qt::Key_S:
        Temp=1<<3;
        Temp = ~Temp;
        keyboard &=Temp;
        break;
    case Qt::Key_D:
        Temp=1<<4;
        Temp = ~Temp;
        keyboard &=Temp;
        break;
    case Qt::Key_A:
        Temp=1<<5;
        Temp = ~Temp;
        keyboard &=Temp;
        break;
    }

    update();
}

void MyOpenglWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
          keyboard|= 1;
    }
    else if(event->button()==Qt::RightButton)
    {
         keyboard|= 1<<1;
    }
    isMousePressed=true;
}

void MyOpenglWidget::mouseReleaseEvent(QMouseEvent *event)
{
    int Temp;
    if(event->button()==Qt::LeftButton)
    {
        Temp=~1;
        keyboard &=Temp;
    }
    else if(event->button()==Qt::RightButton)
    {
        Temp=~(1<<1);
        keyboard &=Temp;
    }
    isMousePressed=false;
    isCanUseMouseMove=false;
}


void MyOpenglWidget::mouseMoveEvent(QMouseEvent *event)
{

    if(!isMousePressed)
        return;
    //static QPoint lastPos(width()/2,height()/2);
    auto currentPos=event->pos();
    deltaPos=currentPos-lastPos;
    lastPos=currentPos;
    if(isCanUseMouseMove)
    {
        m_camera.ProcessMouseMovement(deltaPos.x(),-deltaPos.y());
        update();
    }

    isCanUseMouseMove=true;
}

void MyOpenglWidget::wheelEvent(QWheelEvent *event)
{
    m_camera.ProcessMouseScroll(event->angleDelta().y()/120);
    qDebug()<<event->angleDelta().y();
    update();
}

void MyOpenglWidget::on_timeout()
{
    update();
}