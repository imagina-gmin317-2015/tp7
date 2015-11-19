#include "gamewindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <functional>
#include <algorithm>

#include <QtCore>
#include <QtGui>
using namespace std;

static int firstSeason = 0;
GameWindow::GameWindow()
{
    camera = new Camera();
}

GameWindow::GameWindow(Camera *camera, float framerate)
{
    this->camera = camera;
    this->framerate = framerate;

    cthread = new ClientThread();
    connect(cthread, SIGNAL(seasonChangeSignal()), this, SLOT(onSeasonChange()));

    this->cursor = new QCursor(Qt::BlankCursor);
    this->cursor2 = new QCursor(Qt::ArrowCursor);
}

void GameWindow::initialize()
{
    timer.setInterval(framerate * 1000);
    this->camera->initialize(devicePixelRatio(), width(), height(), 0, 100.0);
    timer.start();
    elapsedTimer.start();
    this->connect(&timer, SIGNAL(timeout()), this, SLOT(renderNow()));

    imagePath = ":/heightmap-1.png";
    this->m_image = QImage(imagePath);

        this->vertices = initVertices(this->m_image.width(), this->m_image.height(), 2);
//        this->vertices = initVertices(this->m_image.width(), this->m_image.height());

    //    entity = PlyEntity::load(":/gull.ply");
    //    entity->setPosition(0, 0, qGray(this->m_image.pixel((this->m_image.width() * (0 + 0.5f)), (this->m_image.height() * (0 + 0.5f)))) * 0.0008f);
    //    entity->setScale(0.1f);

    forest = new Forest(&m_image, 10);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,  GL_FASTEST);	// Active la correction de perspective (pour ombrage, texture, ...)

    snow = new SnowParticles(m_image.width() * 2, m_image.height() * 2, &this->m_image);
    rain = new RainParticles(&this->m_image);
    drought = new Drought();
    spring = new Spring(&this->m_image);
    this->season = firstSeason++;
    this->windowId = this->season;
    this->onSeasonChange();
    this->displayNormals = false;

    snowTexture = new QOpenGLTexture(QImage(":/snow.jpg"));
    snowTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    snowTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    rockTexture = new QOpenGLTexture(QImage(":/rock.jpg"));
    rockTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    rockTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    grassTexture = new QOpenGLTexture(QImage(":/grass.jpg"));
    grassTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    grassTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    m_vertexbuffer.create();
    m_vertexbuffer.bind();

    m_vertexbuffer.allocate(verticesArray.constData(), verticesArray.size() * sizeof(QVector3D));
    m_vertexbuffer.release();

    m_normalbuffer.create();
    m_normalbuffer.bind();
    m_normalbuffer.allocate(normalsArray.constData(), normalsArray.size() * sizeof(QVector3D));
    m_normalbuffer.release();

    m_colorbuffer.create();
    m_colorbuffer.bind();
    m_colorbuffer.allocate(colorsArray.constData(), colorsArray.size() * sizeof(QVector3D));
    m_colorbuffer.release();

    shader = new QOpenGLShaderProgram();
    shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.glsl");
    shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.glsl");
    qDebug() << "linked = " << shader->link();
}

void GameWindow::onSeasonChange()
{
    snow->reset();
    rain->reset();
    if(++season >= 4) season = 0;

    if(season == 0) {
        snow->setActive(true);
        rain->setActive(false);
        spring->setActive(false);
        drought->setActive(false);
    } else if (season == 3) {
        rain->setActive(true);
        snow->setActive(false);
        spring->setActive(false);
        drought->setActive(false);
    } else if (season == 2){
        drought->setActive(true);
        rain->setActive(false);
        snow->setActive(false);
        spring->setActive(false);
    } else {
        spring->setActive(true);
        drought->setActive(false);
        rain->setActive(false);
        snow->setActive(false);
    }
}

void GameWindow::onSaveRequest()
{
    //    ResourceManager::setSave(this->serialize(), 0);
    ResourceManager::setSave(this->serialize(), windowId);
}

void GameWindow::onLoadRequest()
{
    this->load();
}

void GameWindow::render()
{

    if (!m_device)
        m_device = new QOpenGLPaintDevice();

    m_device->setSize(size());


    elapsed = elapsedTimer.elapsed();
    if(elapsed > timer.interval() * 0.5) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        this->render((float) elapsed * 0.0005f);
        QPainter p;
        p.begin(m_device);
        p.setPen(Qt::yellow);
        p.setFont(QFont("Arial", 20));
        p.drawText(10, 30, QString::number((int) (1.0 / (elapsed * 0.001f))));
        p.end();
        elapsed -= timer.interval();
        elapsedTimer.restart();
    }
}

void GameWindow::render(float delta)
{
    if(cursorCaptured) {
        this->cursor->setPos(this->position().x() + width() * 0.5f, this->position().y() + height() * 0.5f);
    }
    this->camera->update(delta);


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Create light components
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { -0.5f, 0.5f, 4.0f, 0.0f };

    shader->bind();
    shader->setUniformValueArray("light_direction", position, 1, 4);
    shader->release();

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    drawTriangles();
    drawNormals();

    snow->update(delta);
    snow->draw(delta);
    rain->update(delta);
    rain->draw(delta);
    drought->update(delta);
    drought->draw();
    spring->update(delta);
    spring->draw(delta);
    ++m_frame;

    //    entity->draw(delta);
    forest->draw(delta);
}

bool GameWindow::event(QEvent *event)
{
    QMouseEvent *mouseEvent;
    float deltaX = this->width() * 0.5f;
    float deltaY = this->height() * 0.5f;
    switch (event->type())
    {
    case QEvent::MouseMove:
        if(cursorCaptured) {
            mouseEvent = static_cast<QMouseEvent*>(event);
            camera->rotate(-(deltaY - mouseEvent->y()) * 0.1f,
                           0,
                           -(deltaX - mouseEvent->x()) * 0.1f
                           );
        }
        return true;
    case QEvent::MouseButtonPress:
        this->setCursor(*cursor);
        cursorCaptured = true;
        return true;
    case QEvent::UpdateRequest:

        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        cursorCaptured = false;
        this->setCursor(*cursor2);
        break;
    case Qt::Key_Tab:
        qApp->exit();
        break;
    case Qt::Key_Space:
        displayNormals = !displayNormals;
        if(displayNormals) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        break;
    case 'Z':
        camera->scale(0.10f, 0.10f, 0);
        break;
    case 'S':
        camera->scale(-0.10f, -0.10f, 0);
        break;
    case Qt::Key_F5:
        emit requestLoad();
        //        fill = !fill;
        break;
    case Qt::Key_F6:
        emit requestSave();
        break;
    case 'W':
        etat ++;
        if(etat > 5)
            etat = 0;
        break;
    case 'C':
        this->camera->setAnimated(!this->camera->isAnimated());
        break;
    }
    renderNow();
}

void GameWindow::drawTriangles()
{
    shader->bind();
    shader->setUniformValue("a_summerTime", drought->getYellow() * 5);
    shader->setUniformValue("a_snowHeightModifier", drought->getSnowHeightModifier());
    glActiveTexture(GL_TEXTURE0);
    grassTexture->bind();

    glActiveTexture(GL_TEXTURE1);
    snowTexture->bind();

    glActiveTexture(GL_TEXTURE2);
    rockTexture->bind();

    glUniform1i(glGetUniformLocation(shader->programId(), "grass"), 0);
    glUniform1i(glGetUniformLocation(shader->programId(), "snow"), 1);
    glUniform1i(glGetUniformLocation(shader->programId(), "rock"), 2);

    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    m_vertexbuffer.bind();
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    m_vertexbuffer.release();
    m_normalbuffer.bind();
    glNormalPointer(GL_FLOAT, 0, NULL);
    m_normalbuffer.release();
    m_colorbuffer.bind();
    glColorPointer(3, GL_FLOAT, 0, NULL);
    m_colorbuffer.release();
    glDrawArrays(GL_TRIANGLES, 0, verticesArray.size());
//    glDrawElements(GL_TRIANGLES, indexesArray.size(), GL_UNSIGNED_INT, indexesArray.constData());
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    rockTexture->release();
    snowTexture->release();
    grassTexture->release();
    shader->release();
}

void GameWindow::drawNormals()
{
//    if(displayNormals) {
//        glColor3f(1, 0, 0);
//        glBegin(GL_LINES);
//        float x = 0.01;
//        for (int i = 0; i < normalsArray.size(); i += 1) {
//            QVector3D n = normalsArray[i];
//            QVector3D v = verticesArray[i];
//            glVertex3f(v.x(), v.y(), v.z());
//            glVertex3f(v.x() + n.x() * x, v.y() + n.y() * x, v.z() + n.z() * x);
//        }
//        glEnd();
//    }
}

QString GameWindow::serialize()
{
    QString s;
    s +=
            imagePath + ";" + ResourceManager::INT(&season)->toString() + "\n" +
            ResourceManager::serialize(camera->attributes()) + "\n" +
            ResourceManager::serialize(drought->attributes()) + "\n" +
            ResourceManager::serialize(rain->attributes()) + "\n" +
            ResourceManager::serialize(snow->attributes()) + "\n" +
            ResourceManager::serialize(spring->attributes()) + "\n";
    return s;
}

void GameWindow::load()
{
    QString s = ResourceManager::getSave(windowId);
    QStringList list = s.split("\n");

    QStringList l = list.at(0).split(";");
    this->imagePath = l.at(0);
    qDebug() << "loaded image" << this->imagePath;
    ResourceManager::assign(camera->attributes(), ResourceManager::parse(list.at(1)));
    ResourceManager::assign(drought->attributes(), ResourceManager::parse(list.at(2)));
    ResourceManager::assign(rain->attributes(), ResourceManager::parse(list.at(3)));
    ResourceManager::assign(snow->attributes(), ResourceManager::parse(list.at(4)));
    ResourceManager::assign(spring->attributes(), ResourceManager::parse(list.at(5)));
}

GLfloat *GameWindow::initVertices(GLint countX, GLint countY)
{
    GLfloat stepX = 1.0 / (countX);
    GLfloat stepY = 1.0 / (countY);

    float posX;
    float posY;

    auto color = [](QVector3D v) {
        if(v.z() < 0.08) {
            return QVector3D(v.z(), 0.4, 0);
        } else if(v.z() > 0.08 && v.z() < 0.15) {
            return QVector3D(0.54, 0.27 + v.z(), 0.07);
        } else {
            return QVector3D(0.9, 0.8, 0.9);
        }
    };

#pragma omp for schedule(dynamic)
    for (int i = 0 ; i < countX - 1; ++i) {
        for (int j = 0; j < countY - 1; ++j) {
            posX = i * stepX - 0.5; posY = (j + 1) * stepY - 0.5;
            QVector3D v2(posX,  posY, getRandomZ(posX, posY));
            verticesArray.push_back(v2);
            colorsArray.push_back(color(v2));

            posX = i * stepX - 0.5; posY = j * stepY - 0.5;
            QVector3D v1(posX,  posY, getRandomZ(posX, posY));
            verticesArray.push_back(v1);
            colorsArray.push_back(color(v1));

            posX = (i + 1) * stepX - 0.5; posY = j * stepY - 0.5;
            QVector3D v3(posX,  posY, getRandomZ(posX, posY));
            verticesArray.push_back(v3);
            colorsArray.push_back(color(v3));

            QVector3D n = QVector3D::normal(v3 - v1, v2 - v1);
            normalsArray.push_back(n);
            normalsArray.push_back(n);
            normalsArray.push_back(n);

            posX = i * stepX - 0.5; posY = (j + 1) * stepY - 0.5;
            QVector3D v4(posX, posY, getRandomZ(posX, posY));
            verticesArray.push_back(v4);
            colorsArray.push_back(color(v4));

            posX = (i + 1) * stepX - 0.5; posY = j * stepY - 0.5;
            QVector3D v6(posX, posY, getRandomZ(posX, posY));
            verticesArray.push_back(v6);
            colorsArray.push_back(color(v6));

            posX = (i + 1) * stepX - 0.5; posY = (j + 1) * stepY - 0.5;
            QVector3D v5(posX, posY, getRandomZ(posX, posY));
            verticesArray.push_back(v5);
            colorsArray.push_back(color(v5));

            QVector3D n1 = QVector3D::normal(v6 - v4, v5 - v4);
            normalsArray.push_back(n1);
            normalsArray.push_back(n1);
            normalsArray.push_back(n1);
        }
    }
    return 0;
}

GLfloat *GameWindow::initVertices(GLint countX, GLint countY, float threshold)
{
    auto color = [](QVector3D v) {
        if(v.z() < 0.08) {
            return QVector3D(v.z(), 0.4, 0);
        } else if(v.z() > 0.08 && v.z() < 0.15) {
            return QVector3D(0.54, 0.27 + v.z(), 0.07);
        } else {
            return QVector3D(0.9, 0.8, 0.9);
        }
    };

    step(0, 0, countX, countY, threshold);
    del_point2d_t	points_[verticesSet.size()];
    QList<QVector3D> l = verticesSet.toList();
    for (int i = 0; i < verticesSet.size(); ++i) {
        del_point2d_t p;
        p.x = l[i].x(); p.y = l[i].y();
        points_[i] = p;
    }
    delaunay2d_t* res = delaunay2d_from(points_, verticesSet.size());
    tri_delaunay2d_t*	tdel	= tri_delaunay2d_from(res);

    for (int i = 0; i < tdel->num_triangles; ++i) {
        for (int j = 0; j < 3; ++j) {
            indexesArray.push_back(tdel->tris[i * 3 + j]);
        }
    }
    delaunay2d_release(res);

    for (int i = 0; i < indexesArray.size(); i += 3) {
        QVector3D v1 = l.at(indexesArray[i]);
        QVector3D v2 = l.at(indexesArray[i + 1]);
        QVector3D v3 = l.at(indexesArray[i + 2]);
        QVector3D n = QVector3D::normal(v2 - v1, v3 - v1);

        verticesArray.push_back(v1);
        colorsArray.push_back(color(v1));
        normalsArray.push_back(n);

        verticesArray.push_back(v2);
        colorsArray.push_back(color(v2));
        normalsArray.push_back(n);

        verticesArray.push_back(v3);
        colorsArray.push_back(color(v3));
        normalsArray.push_back(n);

    }

    qDebug() << "vertices size = " << l.size();
    qDebug() << "normals size = " << normalsArray.size();
    return 0;
}


void GameWindow::step(int startx, int starty, int width, int height, float threshold)
{
    float posX, posY;

    if(startx + width >= m_image.width()) width--;
    if(starty + height >= m_image.height()) height--;

    auto areaAvg = [=]() {
        double sum = 0;
        for (int i = startx; i < width + startx; ++i) {
            for (int j = starty; j < height + starty; ++j) {
                sum += qGray(m_image.pixel(i , j));
            }
        }
        return sum /= (width * height);
    };

    auto areaStdDev = [=] (double avg) {
        double dev = 0;

        for (int i = startx; i < width + startx; ++i) {
            for (int j = starty; j < height + starty; ++j) {
                dev += pow(qGray(m_image.pixel(i , j)) - avg, 2);
            }
        }

        return sqrt(dev / (width * height));
    };

    double v = areaStdDev(areaAvg());

    if(v < threshold || width == 1) {
        posX = startx / (float) m_image.width() - 0.5;
        posY = starty / (float) m_image.height() - 0.5;
        QVector3D v1(posX,  posY, getRandomZ(posX, posY));
        verticesSet.insert(v1);

        posX = startx / (float) m_image.width() - 0.5;
        posY = (starty + height) / (float) m_image.height() - 0.5;
        QVector3D v2(posX,  posY, getRandomZ(posX, posY));
        verticesSet.insert(v2);

        posX = (startx + width) / (float) m_image.width() - 0.5;
        posY = starty / (float) m_image.height() - 0.5;
        QVector3D v3(posX,  posY, getRandomZ(posX, posY));
        verticesSet.insert(v3);

    } else {
        step(startx, starty, width * 0.5, height * 0.5, threshold);
        step(startx + width * 0.5, starty, width * 0.5, height * 0.5, threshold);
        step(startx, starty + height * 0.5, width * 0.5, height * 0.5, threshold);
        step(startx + width * 0.5, starty + height * 0.5, width * 0.5, height * 0.5, threshold);
    }
}

QString GameWindow::serializeVertices() const
{
    int countX = m_image.width();
    int countY = m_image.height();
    int count = countX * countY * 3 * 2 + countX * 3 + 3;

    QString s = "";

    for (int i = 0; i < count; ++i) {
        s += QString::number(vertices[i]) + ";";
    }

    s += "\n";

    for (int i = 0; i < normals.size(); ++i) {
        s += QString::number(normals[i]->x) + ";";
        s += QString::number(normals[i]->y) + ";";
        s += QString::number(normals[i]->z) + ";";
    }
    return s;
}

float GameWindow::getRandomZ(float i, float j)
{
//    return 0;
    return qGray(this->m_image.pixel((this->m_image.width() * (i + 0.5f)), (this->m_image.height() * (j + 0.5f)))) * 0.0008f;
}

#define ROTL10(x) (((x) << 10) | (((x) >> 22) & 0x000000ff))
#define ROTL20(x) (((x) << 20) | (((x) >> 12) & 0x0000ffff))

uint qHash(double data)
{
    union U {
        quint64 n;
        double f;
    };
    U u;
    u.f = data;
    return u.f;
}

inline uint qHash(const QVector3D &v, uint seed)
{
    return qHash(v.x()) ^ ROTL10(qHash(v.y())) ^ ROTL20(qHash(v.z()));
}
