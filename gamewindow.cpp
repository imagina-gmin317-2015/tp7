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

#include <QGLWidget>

#include <QtCore>
#include <QtGui>

#include <QTimer>

using namespace std;

static float xDir = 1, zDir = 1;                    //Orientation de la lumière directionnelle en X et Z
static QVector3D direction;                         //vecteur direction de la caméra
static QVector3D droite;                            //vecteur droite perpendiculaire au vecteur direction de la caméra
static QVector3D up;                                //vecteur indiquant le haut pour la caméra
static QVector3D pos = QVector3D(0,0,-20);          //position camera

/**
 * @brief GameWindow::GameWindow, constructeur de la classe GameWindow.
 * @param refresh_rate, taux de rafraîchissement de la fenêtre
 * @param c, paramètre facultatif qui permet d'avoir une caméra partagée par plusieurs fenêtres
 */
GameWindow::GameWindow(int refresh_rate) : carte(1), m_refresh_rate(refresh_rate), speed(0.5f), indexBuf(QOpenGLBuffer::IndexBuffer)
{
    srand(time(NULL));

    season = "NONE";
    saison = Saison::NONE;

    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this, SLOT(renderNow()));
    restartTimer();

    doConnect();

    ///////////////////////////////////////////////////////////
    direction_vue_h = 3.14f;
    direction_vue_v =0;
    wireframe = false;
    souris_active = false;
    openImage(":/heightmap-1.png");
    ///////////////////////////////////////////////////////////
}
GLuint GameWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

/**
 * @brief GameWindow::~GameWindow, destructeur de la classe GameWindow.
 */
GameWindow::~GameWindow(){

    delete m_timer;
    delete texture;

    delete hauteur;
    delete vertices;
    delete indices;

    arrayBuf.destroy();
    indexBuf.destroy();
}

/**
 * @brief GameWindow::initialize, initialise les fonctionnalités OpenGL et charge une heightmap.
 */
void GameWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    // Compile vertex shader
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!m_program->link())
        close();

    // Bind shader pipeline for use
    if (!m_program->bind())
        close();

    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_texAttr = m_program->attributeLocation("texAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    glEnable(GL_DEPTH_TEST);    // Active le Z-Buffer
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    //Add ambient light
    GLfloat ambientColor[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    //Add directionnal light
    GLfloat lightColor0[] = {1.f, 1.f, 1.f, 1.0f};
    GLfloat lightPos0[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    arrayBuf.create();
    indexBuf.create();

    arrayBuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    indexBuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    q = new Quadtree(vertices, delta, resolution, terrain_width);
    createTerrain();

    image = QImage(":/grass.jpg");
    image = QGLWidget::convertToGLFormat(image);
    glGenTextures(1, &m_texture_location);
    glBindTexture(GL_TEXTURE_2D, m_texture_location);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
    glGenerateMipmap(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);

    initTree(0);
}

/**
 * @brief GameWindow::render, fonction de rendu OpenGL contenant plusieurs type de rendu.
 */
void GameWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    glPushMatrix();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 500.0f);
    matrix.lookAt(pos, pos+direction, up);

    glBindTexture(GL_TEXTURE_2D, m_texture_location);

    m_program->setUniformValue(m_matrixUniform, matrix);
    m_program->setUniformValue(m_program->uniformLocation("tex"), m_texture_location);
    m_program->setUniformValue(m_program->uniformLocation("lightDir"), QVector3D(xDir,-1,zDir));

    if(wireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if(heightmap.depth() != 0 && bDisplayTerrain){
        displayTerrain();
        tree->display();
    }
    glPopMatrix();

    if(bDisplayQuadtree)
        q->DisplayQuadtree();

    if(bDisplayDecimateTerrain)
        DisplayStaticQuatree();


    m_program->release();

    ++m_frame;
}

void GameWindow::openImage(QString str){
    heightmap.load(str);

    if(heightmap.depth() != 0){

        terrain_width = heightmap.width();
        terrain_height = heightmap.height();

        hauteur = new int[terrain_width * terrain_height];

        int index = 0;
        for(int i = 0  ; i < terrain_height ; i++){
            for(int j = 0 ; j < terrain_width ; j++){
                QRgb pixel = heightmap.pixel(i,j);
                hauteur[index++] = qRed(pixel);
            }
        }
    }
}

void GameWindow::createTerrain(){

    if(heightmap.depth() == 0)
        return;

    //VERTICES /////////////////////////////////////////////////////////////////
    delete vertices;
    vertices = new VertexData[terrain_width * terrain_height];

    float gap = 0.5f;
    float posX = -(gap * terrain_width/2.f);
    float posY = -10.f;
    float posZ = -(gap * terrain_height/2.f);

    int index = 0;
    float x = 1.f / terrain_width;
    float y = 1.f / terrain_height;
    for(int i = 0 ; i < terrain_height ; i++){
        for(int j = 0 ; j < terrain_width ; j++){
            vertices[index].position = QVector3D(posX + gap * j, posY + hauteur[index] / 20.f, posZ + gap * i);
            if(hauteur[index] < 85){
                vertices[index].color = QVector3D(0.f,0.5f,0.f);
            }else if(hauteur[index] < 170){
                vertices[index].color = QVector3D(0.33f,0.15f,0.f);
            }else{
                vertices[index].color = QVector3D(1.f,1.f,1.f);
            }

            vertices[index].texCoord = QVector2D(x * j, y * i);
            index++;
        }
    }

    index = 0;

    for(int i = 0 ; i < terrain_height ; i++){
        for(int j = 0 ; j < terrain_width ; j++){
            if(j == terrain_width - 1){
                vertices[index].normal = vertices[index-1].normal;
            }
            else if(i == terrain_height - 1){
                vertices[index].normal = vertices[index - terrain_width].normal;
            }else{
                vertices[index].normal = QVector3D::normal(vertices[index].position, vertices[index+1].position, vertices[index + terrain_width].position);
            }
            //Perturbation des normales
            //vertices[index].normal = QVector3D(vertices[index].normal.x() + rand(), vertices[index].normal.y() + rand(), vertices[index].normal.z() + rand());
            index++;
        }
    }

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, terrain_width * terrain_height * sizeof(VertexData));

    q->Reset(vertices);
    q->QuadtreeStatic(new Voxel(QVector3D(0,-5,0), terrain_width/2), 0, 0);
    voxels = q->getVoxels();

    if(!bBuildDecimateTerrain){
        BuildHighResTerrain();
    }else{
        BuildStaticQuatree();
    }
}

void GameWindow::displayTerrain(){
    if(arrayBuf.size() <= 0){
        return;
    }

    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = m_program->attributeLocation("posAttr");
    m_program->enableAttributeArray(vertexLocation);
    m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate color data
    int colorLocation = m_program->attributeLocation("colAttr");
    m_program->enableAttributeArray(colorLocation);
    m_program->setAttributeBuffer(colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = m_program->attributeLocation("texAttr");
    m_program->enableAttributeArray(texcoordLocation);
    m_program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2,sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector2D);

    // Tell OpenGL programmable pipeline how to locate vertex normals data
    int normalLocation = m_program->attributeLocation("normalAttr");
    m_program->enableAttributeArray(normalLocation);
    m_program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3,sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, indexBuf.size(), GL_UNSIGNED_SHORT, 0);
}

void GameWindow::DisplayStaticQuatree()
{
    glBegin(GL_TRIANGLES);
    for(int v = 0 ; v < voxels.size() ; v++){
        int offsetX = voxels[v]->getOffsetX();
        int offsetY = voxels[v]->getOffsetY();
        int length = voxels[v]->getLength();

        if(length == 7){
            length *= 2;
            length++;
        }else{
            length *= 2;
        }

        int lengthX = length;
        int lengthY = length;

        if(offsetX + length >= terrain_width && offsetY + length >= terrain_height){
            lengthX--;
            lengthY--;
        }
        else if(offsetX + length >= terrain_width){
            lengthX--;
        }
        else if(offsetY + length >= terrain_height){
            lengthY--;
        }

        glVertex3f(vertices[offsetY * terrain_width + offsetX].position.x(), vertices[offsetY * terrain_width + offsetX].position.y(), vertices[offsetY * terrain_width + offsetX].position.z());
        glVertex3f(vertices[offsetY * terrain_width + offsetX + lengthX].position.x(), vertices[offsetY * terrain_width + offsetX + lengthX].position.y(), vertices[offsetY * terrain_width + offsetX + lengthX].position.z());
        glVertex3f(vertices[(offsetY + lengthY) * terrain_width + offsetX + lengthX].position.x(), vertices[(offsetY + lengthY) * terrain_width + offsetX + lengthX].position.y(), vertices[(offsetY + lengthY) * terrain_width + offsetX + lengthX].position.z());

        glVertex3f(vertices[offsetY * terrain_width + offsetX].position.x(), vertices[offsetY * terrain_width + offsetX].position.y(), vertices[offsetY * terrain_width + offsetX].position.z());
        glVertex3f(vertices[(offsetY + lengthY) * terrain_width + offsetX + lengthX].position.x(), vertices[(offsetY + lengthY) * terrain_width + offsetX + lengthX].position.y(), vertices[(offsetY + lengthY) * terrain_width + offsetX + lengthX].position.z());
        glVertex3f(vertices[(offsetY + lengthY) * terrain_width + offsetX].position.x(), vertices[(offsetY + lengthY) * terrain_width + offsetX].position.y(), vertices[(offsetY + lengthY) * terrain_width + offsetX].position.z());
    }
    glEnd();
}

void GameWindow::BuildStaticQuatree()
{
    delete indices;
    indices = new GLushort[voxels.size()*6];
    int index = 0;

    for(int v = 0 ; v < voxels.size() ; v++){
        int offsetX = voxels[v]->getOffsetX();
        int offsetY = voxels[v]->getOffsetY();
        int length = voxels[v]->getLength();

        if(length == 7){
            length *= 2;
            length++;
        }else{
            length *= 2;
        }

        int lengthX = length;
        int lengthY = length;

        if(offsetX + length >= terrain_width && offsetY + length >= terrain_height){
            lengthX--;
            lengthY--;
        }
        else if(offsetX + length >= terrain_width){
            lengthX--;
        }
        else if(offsetY + length >= terrain_height){
            lengthY--;
        }

        indices[index++] = (GLushort)(offsetY * terrain_width + offsetX);
        indices[index++] = (GLushort)(offsetY * terrain_width + offsetX + lengthX);
        indices[index++] = (GLushort)((offsetY + lengthY) * terrain_width + offsetX + lengthX);

        indices[index++] = (GLushort)(offsetY * terrain_width + offsetX);
        indices[index++] = (GLushort)((offsetY + lengthY) * terrain_width + offsetX + lengthX);
        indices[index++] = (GLushort)((offsetY + lengthY) * terrain_width + offsetX);
    }

    indexBuf.bind();
    indexBuf.allocate(indices, voxels.size()*6 * sizeof(GLushort));
}

void GameWindow::BuildHighResTerrain()
{
    //INDICES /////////////////////////////////////////////////////////////////
    delete indices;
    indices = new GLushort[(terrain_width-1)*(terrain_height-1)*6];

    int index = 0;

    for(int i = 0 ; i < terrain_height-1 ; i++){
        for(int j = 0 ; j < terrain_width-1 ; j++){

            indices[index++] = GLushort(i * terrain_width + j);
            indices[index++] = GLushort((i+1) * terrain_width + j);
            indices[index++] = GLushort((i+1) * terrain_width + j + 1);

            indices[index++] = GLushort(i * terrain_width + j);
            indices[index++] = GLushort((i+1) * terrain_width + j + 1);
            indices[index++] = GLushort(i * terrain_width + j + 1);
        }
    }

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, (terrain_width-1)*(terrain_height-1)*6 * sizeof(GLushort));
}

/**
 * @brief GameWindow::event, fonction permettant la gestion d'évenements.
 * @param event, QEvent
 * @return
 */
bool GameWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:

        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

/**
 * @brief GameWindow::keyPressEvent, permet de gérer les évenements de type clavier.
 * @param event, évenement de type clavier
 */
void GameWindow::keyPressEvent(QKeyEvent *event)
{
    float speed = 1.f;

    switch(event->key())
    {
    case Qt::Key_Z:
        pos += direction * speed;
        break;
    case Qt::Key_S:
        pos -= direction * speed;
        break;
    case Qt::Key_Q:
        pos -= droite * speed;
        break;
    case Qt::Key_D:
        pos += droite * speed;
        break;
    case Qt::Key_Up:
        pos.setY(pos.y() + speed);
        break;
    case Qt::Key_Down:
        pos.setY(pos.y() - speed);
        break;
    case Qt::Key_1:
        bDisplayTerrain = !bDisplayTerrain;
        break;
    case Qt::Key_2:
        bDisplayQuadtree = !bDisplayQuadtree;
        break;
    case Qt::Key_3:
        bDisplayDecimateTerrain = !bDisplayDecimateTerrain;
        break;
    case Qt::Key_4:
        xDir--;
        break;
    case Qt::Key_6:
        xDir++;
        break;
    case Qt::Key_5:
        zDir--;
        break;
    case Qt::Key_8:
        zDir++;
        break;
    case Qt::Key_P:
        m_refresh_rate *= 2;
        if(m_refresh_rate > 120)
            m_refresh_rate = 120;

        restartTimer();
        break;
    case Qt::Key_Minus:
        resolution --;
        if(resolution < 7){
            resolution = 7;
        }

        q->setResolution(resolution);
        q->Reset(vertices);
        q->QuadtreeStatic(new Voxel(QVector3D(0,-5,0), terrain_width/2), 0, 0);
        voxels.clear();
        voxels = q->getVoxels();
        break;
    case Qt::Key_Plus:
        resolution ++;
        q->setResolution(resolution);
        q->Reset(vertices);
        q->QuadtreeStatic(new Voxel(QVector3D(0,-5,0), terrain_width/2), 0, 0);
        voxels.clear();
        voxels = q->getVoxels();
        break;
    case Qt::Key_Left:
        delta -= 0.01f;
        if(delta < 0.01f){
            delta = 0.01f;
        }
        q->setDelta(delta);
        q->Reset(vertices);
        q->QuadtreeStatic(new Voxel(QVector3D(0,-5,0), terrain_width/2), 0, 0);
        voxels.clear();
        voxels = q->getVoxels();
        qDebug() << delta;
        break;
    case Qt::Key_Right:
        delta += 0.01f;
        q->setDelta(delta);
        q->Reset(vertices);
        q->QuadtreeStatic(new Voxel(QVector3D(0,-5,0), terrain_width/2), 0, 0);
        voxels.clear();
        voxels = q->getVoxels();
        qDebug() << delta;
        break;
    case Qt::Key_M:
        m_refresh_rate /= 2;
        if(m_refresh_rate < 1)
            m_refresh_rate = 1;

        restartTimer();
        break;
    case Qt::Key_V:
        bBuildDecimateTerrain = !bBuildDecimateTerrain;
        if(!bBuildDecimateTerrain){
            BuildHighResTerrain();
        }else{
            BuildStaticQuatree();
        }
        break;
    case Qt::Key_W:
        wireframe = !wireframe;
        break;
    case Qt::Key_X:
        carte ++;
        if(carte > 3){
            carte = 1;
        }
        QString depth (":/heightmap-");
        depth += QString::number(carte) ;
        depth += ".png" ;

        openImage(depth);
        createTerrain();
        break;
    }

    if(event->key() == Qt::Key_Z || event->key() == Qt::Key_Q || event->key() == Qt::Key_S || event->key() == Qt::Key_D){
        int x = terrain_width / 2;
        int y = terrain_height / 2;
        int index = y * terrain_width + x;

        QVector3D center = vertices[index].position;
        float distance = sqrt(pow(pos.x() - center.x(),2) + pow(pos.y() - center.y(),2) + pow(pos.z() - center.z(),2));

        q->setDelta((distance * 0.1) / 10);
        q->Reset(vertices);
        q->QuadtreeStatic(new Voxel(QVector3D(0,-5,0), terrain_width/2), 0, 0);
        voxels = q->getVoxels();

        if(bBuildDecimateTerrain){
            BuildStaticQuatree();
        }

        if(distance > 50){
            tree->setLOD(1, "summertree");
        }else if(distance > 100){
            tree->setLOD(0, "summertree");
        }else{
            tree->setLOD(2, "summertree");
        }
    }
}

/**
 * @brief GameWindow::mousePressEvent, permet d'écouter l'action sur un clic souris.
 * Défini si l'utilisateur a enfoncé le clic gauche.
 * Désactive le curseur.
 * @param event, événement de souris
 */
void GameWindow::mousePressEvent( QMouseEvent * event )
{
    if(event->type() == QEvent::MouseButtonPress){
        if(event->buttons() == Qt::LeftButton){
            setCursor(Qt::BlankCursor);
            souris_active = true;
        }
    }
}

/**
 * @brief GameWindow::mouseReleaseEvent, permet d'écouter l'action sur un clic souris.
 * Défini si l'utilisateur a relâché le clic gauche.
 * Réactive le curseur.
 * @param event, événement de souris
 */
void GameWindow::mouseReleaseEvent( QMouseEvent * event )
{
    if(event->type() == QEvent::MouseButtonRelease){
        setCursor(Qt::BitmapCursor);
        souris_active = false;
    }

}

/**
 * @brief GameWindow::mouseMoveEvent, permet d'écouter les déplacements de la souris
 * @param event, événement de souris
 */
void GameWindow::mouseMoveEvent(QMouseEvent* event){

    float mouseSpeed = 0.005f;

    if(souris_active && event->type() == QEvent::MouseMove){
        float xm, ym;

        xm = (float)event->x()/width()  - 0.5f;
        ym = (float)event->y()/height() - 0.5f;

        if( xm < -0.25f )
        {
            xm = 0.25f;
        }
        else if( xm > 0.25f )
        {
            xm = -0.25f;
        }

        if( ym < -0.25f )
        {
            ym = -0.25f;
        }
        else if( ym > 0.25f )
        {
            ym = 0.25f;
        }

        direction_vue_h += mouseSpeed * float(width()/2 - event->x() );
        direction_vue_v += mouseSpeed * float( height()/2 - event->y() );

        direction = QVector3D(
                    cos(direction_vue_v) * sin(direction_vue_h),
                    sin(direction_vue_v),
                    cos(direction_vue_v) * cos(direction_vue_h)
                    );

        droite = QVector3D(
                    sin(direction_vue_h - 3.14f/2.0f),
                    0,
                    cos(direction_vue_h - 3.14f/2.0f)
                    );

        up = QVector3D::crossProduct(droite, direction);

        QCursor::setPos(width()/2 + QWindow::x(), height()/2 + QWindow::y());
    }
}

/**
 * @brief GameWindow::restartTimer, permet de relancer le timer lorsque celui-ci change.
 */
void GameWindow::restartTimer(){
    m_timer->stop();
    m_timer->start(1000.f / m_refresh_rate);

    updateTitle();
}

/**
 * @brief GameWindow::updateTitle, Met à jour le titre de la fenêtre en fonction de la saison et du framerate de celle-ci.
 */
void GameWindow::updateTitle(){
    QString fps = QString::number(m_refresh_rate);
    QString title = fps + "FPS - " + season;
    setTitle(title);
}

/**
 * @brief GameWindow::seasonColor, Affiche le terrain d'une couleur différente en fonction de la saison.
 */
color GameWindow::seasonColor(){
    if(saison == Saison::PRINTEMPS){
        glColor3f(0.f, 0.7f, 0.2f);
        return color{0.f, 0.7f, 0.2f};
    }
    else if(saison == Saison::ETE){
        glColor3f(1.f, 1.f, 0.f);
        return color{1.f, 1.f, 0.f};
    }
    else if(saison == Saison::AUTOMNE){
        glColor3f(0.86f, 0.4f, 0.f);
        return color{0.86f, 0.4f, 0.f};
    }
    else if(saison == Saison::HIVER){
        glColor3f(0.8f, 0.8f, 0.8f);
        return color{0.8f, 0.8f, 0.8f};
    }

    return color{0.f,0.f,0.f};
}

/**
 * @brief GameWindow::doConnect, Connecte le client au serveur.
 */
void GameWindow::doConnect(){
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    qDebug() << QString::number(m_refresh_rate) << "FPS window : connecting...";

    // this is not blocking call
    socket->connectToHost("127.0.0.1", 9999);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
}

void GameWindow::initTree(int LOD)
{
    QVector3D pos;

    int rand_x, rand_y, id;

    rand_x = rand() % terrain_width;
    rand_y = rand() % terrain_height;
    id = rand_y * terrain_width + rand_x;

    pos.setX(vertices[id].position.x()); pos.setY(vertices[id].position.y()); pos.setZ(vertices[id].position.z());

    delete tree;
    tree = new GameObject(pos, QVector3D(0.f,0.f,90.f), QVector3D(1.f,1.f,1.f), "summertree", LOD);
}

/**
 * @brief GameWindow::updateEnumSaison, met à jour l'enum Saison en fonction de la saison courante.
 */
void GameWindow::updateEnumSaison()
{
    if(season == "PRINTEMPS"){
        saison = Saison::PRINTEMPS;
    }else if(season == "ETE"){
        saison = Saison::ETE;
    }else if(season == "AUTOMNE"){
        saison = Saison::AUTOMNE;
    }else if(season == "HIVER"){
        saison = Saison::HIVER;
    }

    //Ne fonctionne pas
    updateTerrainColor();
}

void GameWindow::updateTerrainColor(){

    int index = 0;

    for(int i = 0 ; i < terrain_height ; i++){
        for(int j = 0 ; j < terrain_width ; j++){
            if(saison == Saison::PRINTEMPS){
                if(hauteur[index] < 85){
                    vertices[index].color = QVector3D(0.f,0.5f,0.f);
                }else if(hauteur[index] < 170){
                    vertices[index].color = QVector3D(0.33f,0.15f,0.f);
                }else{
                    vertices[index].color = QVector3D(1.f,1.f,1.f);
                }
            }
            else if(saison == Saison::ETE){
                if(hauteur[index] < 85){
                    vertices[index].color = QVector3D(0.f,0.8f,0.f);
                }else{
                    vertices[index].color = QVector3D(0.33f,0.15f,0.f);
                }
            }else if(saison == Saison::AUTOMNE){
                if(hauteur[index] < 170){
                    vertices[index].color = QVector3D(0.33f,0.15f,0.f);
                }else{
                    vertices[index].color = QVector3D(1.f,1.f,1.f);
                }
            }else if(saison == Saison::HIVER){
                vertices[index].color = QVector3D(1.f,1.f,1.f);
            }
            index++;
        }
    }
    size_t bufSize = terrain_width * terrain_height * sizeof(VertexData);
    arrayBuf.write(0, vertices, bufSize);
}

/** SLOTS **/

/**
 * @brief GameWindow::connected, s'execute lorsque le client se connecte à un serveur.
 */
void GameWindow::connected()
{
    qDebug() << QString::number(m_refresh_rate) << "FPS window : connected...";
}

/**
 * @brief GameWindow::disconnected, s'éxecute lorsque le client est déconnecté.
 */
void GameWindow::disconnected()
{
    qDebug() << QString::number(m_refresh_rate) << "FPS window : disconnected...";
}

/**
 * @brief GameWindow::readyRead, permet de recevoir les données envoyées par le serveur.
 */
void GameWindow::readyRead()
{
    // read the data from the socket
    QString str = QString(socket->readAll());
    season = str;
    updateEnumSaison();
    updateTitle();
}
