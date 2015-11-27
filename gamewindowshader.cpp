#include "gamewindowshader.h"
#include "filemanager.h"
#include "objetply3d.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtOpenGL/QGLWidget>

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>

#include <QtCore>
#include <QtGui>

using namespace std;


GameWindowShader::GameWindowShader(int fps, Camera* camera, int day)
{
    m_fps = fps;
    m_isRotating = false;
    m_timer = new QTimer(this);
    m_timer->connect(m_timer, SIGNAL(timeout()),this, SLOT(renderNow()));
    m_timer->start(1000/fps);
    m_camera = camera;

    m_timer_day = new QTimer(this);
    m_timer_day->connect(m_timer_day, SIGNAL(timeout()),this, SLOT(update_day()));
    m_timer_day->start(20);

    m_day = day;

    if(m_day >= 270){
        titre = "Automne";
        m_type = AUTOMNE;
    }
    else if (m_day >= 180){
        titre = "Ete";
        m_type = ETE;
    }
    else if(m_day >= 90){
        titre = "Printemps";
        m_type = PRINTEMPS;
    }
    else if(m_day >= 0){
        titre = "Hiver";
        m_type = HIVER;
    }

    m_arbreEte =  ObjetPly3D("..\\tp7\\summertree.ply");
    m_arbreHiver =  ObjetPly3D("..\\tp7\\wintertree.ply");
    m_arbreAutomne =  ObjetPly3D("..\\tp7\\autumntree.ply");
    m_arbrePrintemps =  ObjetPly3D("..\\tp7\\springtree.ply");
    m_tonneau = ObjetPly3D ("..\\tp7\\barril.ply");

    m_resolution = 0.0f;

}

void GameWindowShader::initialize()
{
    const qreal retinaScale = devicePixelRatio();

    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);

    m_taille_min = 12;

    loadMap(":/heightmap-2.png");




//    if(m_type == HIVER || m_type == AUTOMNE)
//        initall();


    m_programNM = new QOpenGLShaderProgram(this);
    m_programNM->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\tp7\\Shaders\\NM_vertexShader.vsh");
    m_programNM->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\tp7\\Shaders\\NM_fragmentShader.fsh");

    if(!m_programNM->link()){
        qDebug() << "Erreur de link !" << endl;
        qDebug() << m_programNM->log() << endl;
        exit(0);
    }

    m_matrixUniform = m_programNM->uniformLocation("matrix");

    loadTexture2("..\\tp7\\Texture\\water.jpg", m_textureEau);
    loadTexture2("..\\tp7\\Texture\\Grass.jpg", m_textureHerbe);
    loadTexture2("..\\tp7\\Texture\\texture_montagne.jpg", m_textureMontagne);
    loadTexture2("..\\tp7\\Texture\\textureSandstone.png", m_textureSable);
    loadTexture2("..\\tp7\\Texture\\deep_blue.jpg", m_textureEauProfonde);
    loadTexture2("..\\tp7\\Texture\\neige.jpg", m_textureNeige);
    loadTexture2("..\\tp7\\Texture\\feuilles.jpg", m_textureFeuilleMorte);
    loadTexture2("..\\tp7\\Texture\\normalMap_heightmap2.png", m_normalMap);

    /** Creation des arbres ** /
    int t = ARBRE_ETE;

    m_objectList << t << 0.0 << 0.0 << 0.0 << 0.03;
    m_objectList << t << 0.39 << -0.1 << 0.0 << 0.02;
    m_objectList << t << 0.19 << -0.3 << 0.0 << 0.02;
    m_objectList << t << 0.15 << -0.3 << 0.0 << 0.02;
    m_objectList << t << -0.35 << -0.09 << 0.0 << 0.02;
    m_objectList << t << -0.28 << -0.08 << 0.0 << 0.02;
    m_objectList << t << -0.29 << 0.01 << 0.0 << 0.02;
    m_objectList << t << -0.1 << 0.2 << 0.0 << 0.03;
    m_objectList << t << -0.1 << 0.09 << 0.0 << 0.03;
    m_objectList << t << 0.39 << -0.2 << 0.0 << 0.02;
    m_objectList << t << 0.30 << -0.29 << 0.0 << 0.02;
    m_objectList << t << 0.1 << -0.4 << 0.0 << 0.02;
    m_objectList << t << 0.0 << -0.42 << 0.0 << 0.03;
    m_objectList << t << 0.2 << -0.42 << 0.0 << 0.03;
    m_objectList << t << -0.1 << -0.1 << 0.0 << 0.03;
    m_objectList << t << -0.05 << -0.15 << 0.0 << 0.03;
    /** **/

    //m_objectList << TONNEAU << 0.0 << 0.0 << 0.2 << 0.03;

    glEnable(GL_DEPTH_TEST);

    glScalef(0.3f,0.3f,0.3f);

    glEnable(GL_LIGHTING);  // Active l'éclairage
    glEnable(GL_LIGHT0);    // Allume la lumière n°1

    glEnable( GL_COLOR_MATERIAL );

    float ambient[] = {0.5 , 0.5 , 0.5 , 1.0};
    float diffuse[] = {1.0 , 1.0 , 1.0 , 1.0};
    float position[] = {3.0 , 14.0 , 20.0 , 1.0};

    glLightfv(GL_LIGHT0 , GL_AMBIENT , ambient);
    glLightfv(GL_LIGHT0 , GL_DIFFUSE , diffuse);
    glLightfv(GL_LIGHT0 , GL_POSITION , position);
}

void GameWindowShader::loadMap(QString localPath)
{

    if (QFile::exists(localPath)) {
        m_image = QImage(localPath);
    }

    uint id = 0;
    p = new point[m_image.width() * m_image.height()];
    QRgb pixel;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {

            pixel = m_image.pixel(i,j);

            id = i*m_image.width() +j;

            p[id].x = (float)i/(m_image.width()) - ((float)m_image.width()/2.0)/m_image.width();
            p[id].y = (float)j/(m_image.height()) - ((float)m_image.height()/2.0)/m_image.height();
            p[id].z = 0.001f * (float)(qRed(pixel));
        }
    }
    m_quad = new quadtree();

    division(m_quad,0,m_image.width(),0,m_image.height(),m_resolution,m_taille_min);
}

void GameWindowShader::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(m_isRotating)
        m_camera->set_rotY(m_camera->get_rotY() + 1.0f);

    glLoadIdentity();
    m_camera->scale();
    glRotatef(m_camera->get_rotX(),1.0f,0.0f,0.0f);
    glRotatef(m_camera->get_rotY(),0.0f,0.0f,1.0f);
    /*
    if(m_type == HIVER || m_type == AUTOMNE){
        renderMeteo();
        update();
    }
    */

    float diffuse1[] = {1.0 , 1.0 , 1.0 , 1.0};
    float diffuse2[] = {0.5 , 0.5 , 0.5 , 1.0};

    switch(m_camera->get_etat())
    {
    case 0:
        this->setTitle(this->titre + ": points + texture");
        glLightfv(GL_LIGHT0 , GL_DIFFUSE , diffuse1);
        //displayPoints();
        callDisplayPointQuadtree(m_quad);
        renderObjectsPoints();
        break;
    case 1:
        this->setTitle(this->titre + ": lines + texture");
        glLightfv(GL_LIGHT0 , GL_DIFFUSE , diffuse2);
        //displayLines();
        callDisplayLinesQuadtree(m_quad);
        renderObjectsPoints();
        break;
    case 2:
        this->setTitle(this->titre + ": triangles + texture");
        glLightfv(GL_LIGHT0 , GL_DIFFUSE , diffuse1);
        //displayTrianglesTexture();
        callDisplayTriangleQuadtree(m_quad);
        renderObjectsFace();
        break;
    case 3:
        this->setTitle(this->titre + ": triangles + couleur");
        glLightfv(GL_LIGHT0 , GL_DIFFUSE , diffuse2);
        displayTriangles();
        renderObjectsFace();
        break;
    default:
        this->setTitle(this->titre + ": point + texture");
        glLightfv(GL_LIGHT0 , GL_DIFFUSE , diffuse1);
        displayPoints();
        renderObjectsPoints();
        break;
    }

    ++m_frame;

}

void GameWindowShader::renderObjectsFace(){

    int type;

    for(int i = 0; i < m_objectList.size() / 5; ++i){

        type = m_objectList.at(i*5);

        if(type == ARBRE_AUTOMNE){
            glColor3f(165.0f/255.0f, 38.0f/255.0f, 10.0f/255.0f);
            m_arbreAutomne.renderFace(m_objectList.at(i*5+1),m_objectList.at(i*5+2),
                                      m_objectList.at(i*5+3),m_objectList.at(i*5+4));
        }
        else if(type == ARBRE_ETE){
            glColor3f(237.0f/255.0f, 127.0f/255.0f, 16.0f/255.0f);
            m_arbreEte.renderFace(m_objectList.at(i*5+1),m_objectList.at(i*5+2),
                                  m_objectList.at(i*5+3),m_objectList.at(i*5+4));
        }
        else if(type == ARBRE_HIVER){
            glColor3f(9.0f/255.0f, 82.0f/255.0f, 40.0f/255.0f);
            m_arbreHiver.renderFace(m_objectList.at(i*5+1),m_objectList.at(i*5+2),
                                    m_objectList.at(i*5+3),m_objectList.at(i*5+4));
        }
        else if(type == ARBRE_PRINTEMPS){
            glColor3f(255.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
            m_arbrePrintemps.renderFace(m_objectList.at(i*5+1),m_objectList.at(i*5+2),
                                        m_objectList.at(i*5+3),m_objectList.at(i*5+4));
        }
        else{ //if(type == TONNEAU)
            glColor3f(165.0f/255.0f, 38.0f/255.0f, 10.0f/255.0f);
            m_tonneau.renderFace(m_objectList.at(i*5+1),m_objectList.at(i*5+2),
                                 m_objectList.at(i*5+3),m_objectList.at(i*5+4));
        }
    }
}

void GameWindowShader::renderObjectsPoints(){

    int type;

    for(int i = 0; i < m_objectList.size() / 5; ++i){

        type = m_objectList.at(i*5);

        if(type == ARBRE_AUTOMNE){
            glColor3f(165.0f/255.0f, 38.0f/255.0f, 10.0f/255.0f);
            m_arbreAutomne.renderPoints(m_objectList.at(i*5+1),m_objectList.at(i*5+2),
                                        m_objectList.at(i*5+3),m_objectList.at(i*5+4));
        }
        else if(type == ARBRE_ETE){
            glColor3f(237.0f/255.0f, 127.0f/255.0f, 16.0f/255.0f);
            m_arbreEte.renderPoints(m_objectList.at(i*5+1),m_objectList.at(i*5+2),
                                    m_objectList.at(i*5+3),m_objectList.at(i*5+4));
        }
        else if(type == ARBRE_HIVER){
            glColor3f(9.0f/255.0f, 82.0f/255.0f, 40.0f/255.0f);
            m_arbreHiver.renderPoints(m_objectList.at(i*5+1),m_objectList.at(i*5+2),
                                      m_objectList.at(i*5+3),m_objectList.at(i*5+4));
        }
        else if(type == ARBRE_PRINTEMPS){
            glColor3f(255.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
            m_arbrePrintemps.renderPoints(m_objectList.at(i*5+1),m_objectList.at(i*5+2),
                                          m_objectList.at(i*5+3),m_objectList.at(i*5+4));
        }
        else{ //if(type == TONNEAU)
            glColor3f(165.0f/255.0f, 38.0f/255.0f, 10.0f/255.0f);
            m_tonneau.renderPoints(m_objectList.at(i*5+1),m_objectList.at(i*5+2),
                                   m_objectList.at(i*5+3),m_objectList.at(i*5+4));
        }
    }
}

void GameWindowShader::update_day(){
    m_day = (m_day + 1) % 365;

    if(m_day >= 270){
        titre = "Automne";
        m_type = AUTOMNE;
    }
    else if (m_day >= 180){
        titre = "Ete";
        m_type = ETE;
    }
    else if(m_day >= 90){
        titre = "Printemps";
        m_type = PRINTEMPS;
    }
    else if(m_day >= 0){
        titre = "Hiver";
        m_type = HIVER;
    }
}

bool GameWindowShader::event(QEvent *event)
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

void GameWindowShader::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {

    case 'L':
    {
        qDebug() << "Loading";
        FileManager filemanager;
        filemanager.loadFile(this,this->title());

        //reload la bonne map
        QString depth (":/heightmap-");
        depth += QString::number(carte) ;
        depth += ".png" ;

        loadMap(depth);

        qDebug() << "Fin load";
        break;
    }
    case 'G':
    {
        qDebug() << "Sauvegarde";
        FileManager filemanager(this,this->title());
        qDebug() << "Fin sauvegarde";
        break;
    }
    case 'C':
    {
        m_isRotating = !m_isRotating;
        break;
    }
    case 'P':
    {
        if(m_fps < 1000){
            m_timer->stop();
            m_fps *= 2;
            m_timer->start(1000/m_fps);
        }
        break;
    }
    case 'M':
    {
        if(m_fps >= 2){
            m_timer->stop();
            m_fps /= 2;
            m_timer->start(1000/m_fps);
        }
        break;
    }
    case 'Z':
        m_camera->set_ss(m_camera->get_ss()+0.10f);

        --m_taille_min;
        m_resolution -= 3.0f;

        if(m_taille_min >= 1){
            division(m_quad,0,m_image.width(),0,m_image.height(),m_resolution,m_taille_min);
        }
        break;
    case 'S':
        m_camera->set_ss(m_camera->get_ss()-0.10f);

        ++m_taille_min;
        m_resolution += 3.0f ;
        division(m_quad,0,m_image.width(),0,m_image.height(),m_resolution,m_taille_min);

        break;
    case 'A':
        m_camera->set_rotX(m_camera->get_rotX()+1.0f);
        break;
    case 'E':
        m_camera->set_rotX(m_camera->get_rotX()-1.0f);
        break;
    case 'Q':
        m_camera->set_rotY(m_camera->get_rotY()+1.0f);
        break;
    case 'D':
        m_camera->set_rotY(m_camera->get_rotY()-1.0f);
        break;
    case 'W':
        m_camera->set_etat(m_camera->get_etat()+1);
        if(m_camera->get_etat() > 3)
            m_camera->set_etat(0);
        break;
    case 'X':
        carte ++;
        if(carte > 3)
            carte = 1;
        QString depth (":/heightmap-");
        depth += QString::number(carte) ;
        depth += ".png" ;

        loadMap(depth);
        break;

    }
    //renderNow();
}


void GameWindowShader::initall()
{

    srand(time(NULL));
    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        Particles[i].x = (-0.5f) + ((float)(rand())/ (float)(RAND_MAX));
        Particles[i].y = (-0.5f) + ((float)(rand())/ (float)(RAND_MAX));
        Particles[i].z = ((float)(rand())/ (float)(RAND_MAX));
    }
}

void GameWindowShader::initentity(int index)
{
    Particles[index].x = (-0.5f) + ((float)(rand())/ (float)(RAND_MAX));
    Particles[index].y = (-0.5f) + ((float)(rand())/ (float)(RAND_MAX));
    Particles[index].z = ((float)(rand())/ (float)(RAND_MAX));
}

void GameWindowShader::renderMeteo()
{
    glBegin(GL_POINTS);

    if(m_type == HIVER)
        glColor3f(1.0f,1.0f,1.0f);
    else//automne
        glColor3f(44.0f/255.0f,117.0f/255.0f,1.0f);

    for(int i = 0; i < MAX_PARTICLES; i++)
    {
        glVertex3f(Particles[i].x, Particles[i].y, Particles[i].z);
    }
    glEnd();
}

void GameWindowShader::update()
{
    for(int i = 0; i < MAX_PARTICLES; i++)
    {

       if(m_type == AUTOMNE)
            Particles[i].z -= ((float)(rand())/(float)(RAND_MAX)) * 0.15f;
       else{//hiver
            Particles[i].z -= ((float)(rand())/(float)(RAND_MAX))* 0.025f;
            //Particles[i].x += ((float)(rand())/(float)(RAND_MAX));
            //Particles[i].y += ((float)(rand())/(float)(RAND_MAX));
        }

        if(Particles[i].z < 0)
        {
            initentity(i);
        }
    }
}



void GameWindowShader::displayPoints()
{
    /*glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    uint id = 0;

    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {
            displayColorSeasons(p[id].z);

            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

        }
    }
    glEnd();*/

    m_programNM->bind();

    QMatrix4x4 matrix = QMatrix4x4();
    matrix.scale(m_camera->get_ss());
    matrix.rotate(m_camera->get_rotX(), 1, 0, 0);
    matrix.rotate(m_camera->get_rotY(), 0, 0, 1);

    m_programNM->setUniformValue(m_matrixUniform, matrix);

    glUniform1i(m_programNM->uniformLocation("texture1"), 0);
    glUniform1i(m_programNM->uniformLocation("texture2"), 1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalMap);

    uint id = 0;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {
            //displayColorSeasons(p[id].z);
            id = i*m_image.width() +j;
            bindTextureRegardingZ(p[id].z);

            glBegin(GL_POINTS);
            glTexCoord2f((float)(i)/(float)(m_image.width()),(float)(j)/(float)(m_image.height()));
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            glEnd();
        }
    }

    m_programNM->release();

}

void GameWindowShader::bindTextureRegardingZ(float z){

    glActiveTexture(GL_TEXTURE0);

    if(m_type == ETE){
        if (z > 0.08f){//marron
            glBindTexture(GL_TEXTURE_2D, m_textureMontagne);
        }
        else if (z > 0.04f){//vert
            glBindTexture(GL_TEXTURE_2D, m_textureHerbe);
        }
        else if (z > 0.020f){//jaune
            glBindTexture(GL_TEXTURE_2D, m_textureSable);
        }
        else {//bleu clair
            glBindTexture(GL_TEXTURE_2D, m_textureEau);
        }
    }
    else if (m_type == AUTOMNE){

        if (z > 0.08f){//marron
            glBindTexture(GL_TEXTURE_2D, m_textureMontagne);
        }
        else if (z > 0.04f){//vert
            glBindTexture(GL_TEXTURE_2D, m_textureHerbe);
        }
        else if (z > 0.030f){//orange
            glBindTexture(GL_TEXTURE_2D, m_textureFeuilleMorte);
        }
        else if (z > 0.025f){//bleu clair
            glBindTexture(GL_TEXTURE_2D, m_textureEau);
        }
        else{//bleu foncé
            glBindTexture(GL_TEXTURE_2D, m_textureEauProfonde);
        }
    }
    else if(m_type == HIVER){

        if(z > 0.15f){//blanc
           glBindTexture(GL_TEXTURE_2D, m_textureNeige);
        }
        else if (z > 0.08f){//marron
            glBindTexture(GL_TEXTURE_2D, m_textureMontagne);
        }
        else if (z > 0.04f){//blanc
            glBindTexture(GL_TEXTURE_2D, m_textureNeige);
        }
        else if (z > 0.015f){//bleu clair
            glBindTexture(GL_TEXTURE_2D, m_textureEau);
        }
        else{//bleu foncé
            glBindTexture(GL_TEXTURE_2D, m_textureEauProfonde);
        }
    }
    else{//PRINTEMPS

        if(z > 0.2f){//blanc
           glBindTexture(GL_TEXTURE_2D, m_textureNeige);
        }
        else if (z > 0.08f){//marron
           glBindTexture(GL_TEXTURE_2D, m_textureMontagne);
        }
        else if (z > 0.03f){//vert
            glBindTexture(GL_TEXTURE_2D, m_textureHerbe);
        }
        else if (z > 0.015f){//bleu clair
            glBindTexture(GL_TEXTURE_2D, m_textureEau);
        }
        else{//bleu foncé
            glBindTexture(GL_TEXTURE_2D, m_textureEauProfonde);
        }
    }
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, m_normalMap);
}


void GameWindowShader::displayTriangles()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {

            id = i*m_image.width() +j;
            //displayColor(p[id].z);
            displayColorSeasons(p[id].z);

            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            //displayColor(p[id].z);
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            //displayColor(p[id].z);
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);



            id = i*m_image.width() +(j+1);
            //displayColor(p[id].z);
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            //displayColor(p[id].z);
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            //displayColor(p[id].z);
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }
    glEnd();
}

void GameWindowShader::displayLines()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {


            id = i*m_image.width() +j;
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j;
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +j;
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j;
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = i*m_image.width() +(j+1);
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j+1;
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +(j);
            displayColorSeasons(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }

    glEnd();
}

void GameWindowShader::displayTrianglesTexture()
{

    uint id = 0;
    m_programNM->bind();

    QMatrix4x4 matrix = QMatrix4x4();
    //matrix.perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
    //matrix.translate(0, 0, -m_camera->get_ss());
    matrix.scale(m_camera->get_ss());
    matrix.rotate(m_camera->get_rotX(), 1, 0, 0);
    matrix.rotate(m_camera->get_rotY(), 0, 0, 1);

    m_programNM->setUniformValue(m_matrixUniform, matrix);

    glUniform1i(m_programNM->uniformLocation("texture1"), 0);
    glUniform1i(m_programNM->uniformLocation("texture2"), 1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalMap);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureHerbe);


    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {

            id = i*m_image.width() +j;
            bindTextureRegardingZ(p[id].z);
            glBegin(GL_TRIANGLES);
            glTexCoord2f((float)(i)/(float)(m_image.width()),(float)(j)/(float)(m_image.height()));
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);


            id = i*m_image.width() +(j+1);
            //bindTextureRegardingZ(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            glTexCoord2f((float)(i)/(float)(m_image.width()),(float)(j+1)/(float)(m_image.height()));

            id = (i+1)*m_image.width() +j;
            //bindTextureRegardingZ(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            glTexCoord2f((float)(i+1)/(float)(m_image.width()),(float)(j)/(float)(m_image.height()));
            //glEnd();

            id = i*m_image.width() +(j+1);
            //bindTextureRegardingZ(p[id].z);

            //glBegin(GL_TRIANGLES);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            glTexCoord2f((float)(i)/(float)(m_image.width()),(float)(j+1)/(float)(m_image.height()));

            id = (i+1)*m_image.width() +j+1;
            //bindTextureRegardingZ(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            glTexCoord2f((float)(i+1)/(float)(m_image.width()),(float)(j+1)/(float)(m_image.height()));

            id = (i+1)*m_image.width() +j;
            //bindTextureRegardingZ(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            glTexCoord2f((float)(i+1)/(float)(m_image.width()),(float)(j)/(float)(m_image.height()));
            glEnd();
        }
    }


    m_programNM->release();
}


void GameWindowShader::displayColor(float alt)
{

    float R;
    float G;
    float B;

    if(alt > 0.15f){//blanc
       R = 1.0f;
       G = 1.0f;
       B = 1.0f;
    }
    else if (alt > 0.08f){//marron
        R = 88.0f/255.0f;
        G = 41.0f/255.0f;
        B = 0.0f;
    }
    else if (alt > 0.04f){//vert
        R = 20.0f/255.0f;
        G = 148.0f/255.0f;
        B = 20.0f/255.0f;
    }
    else if (alt > 0.030f){//jaune
        R = 255.0f/255.0f;
        G = 222.0f/255.0f;
        B = 117.0f/255.0f;
    }
    else if (alt > 0.015f){//bleu clair
        R = 44.0f/255.0f;
        G = 117.0f/255.0f;
        B = 1.0f;
    }
    else{//bleu foncé
        R = 15.0f/255.0f;
        G = 5.0f/255.0f;
        B = 107.0f/255.0f;
    }

    glColor3f(R, G, B);
 /*
    if (alt > 0.2)
    {
        glColor3f(01.0f, 1.0f, 1.0f);
    }
    else     if (alt > 0.1)
    {
        glColor3f(alt, 1.0f, 1.0f);
    }
    else     if (alt > 0.05f)
    {
        glColor3f(01.0f, alt, alt);
    }
    else
    {
        glColor3f(0.0f, 0.0f, 1.0f);
    }
*/
}

void GameWindowShader::displayColorSeasons(float alt)
{
    float R;
    float G;
    float B;

    if(m_type == ETE){

        if (alt > 0.08f){//marron
            R = 143.0f/255.0f;
            G = 89.0f/255.0f;
            B = 34.0f/255.0f;
        }
        else if (alt > 0.04f){//vert
            R = 20.0f/255.0f;
            G = 148.0f/255.0f;
            B = 20.0f/255.0f;
        }
        else if (alt > 0.020f){//jaune
            R = 255.0f/255.0f;
            G = 222.0f/255.0f;
            B = 117.0f/255.0f;
        }
        else {//bleu clair
            R = 44.0f/255.0f;
            G = 117.0f/255.0f;
            B = 1.0f;
        }
    }
    else if(m_type == AUTOMNE){

        if (alt > 0.08f){//marron
            R = 88.0f/255.0f;
            G = 41.0f/255.0f;
            B = 0.0f;
        }
        else if (alt > 0.04f){//vert
            R = 20.0f/255.0f;
            G = 148.0f/255.0f;
            B = 20.0f/255.0f;
        }
        else if (alt > 0.030f){//orange
            R = 223.0f/255.0f;
            G = 120.0f/255.0f;
            B = 20.0f/255.0f;
        }
        else if (alt > 0.025f){//bleu clair
            R = 44.0f/255.0f;
            G = 117.0f/255.0f;
            B = 1.0f;
        }
        else{//bleu foncé
            R = 15.0f/255.0f;
            G = 5.0f/255.0f;
            B = 107.0f/255.0f;
        }
    }
    else if(m_type == HIVER){

        if(alt > 0.15f){//blanc
           R = 1.0f;
           G = 1.0f;
           B = 1.0f;
        }
        else if (alt > 0.08f){//marron
            R = 88.0f/255.0f;
            G = 41.0f/255.0f;
            B = 0.0f;
        }
        else if (alt > 0.04f){//blanc
            R = 1.0f;
            G = 1.0f;
            B = 1.0f;
        }
        else if (alt > 0.015f){//bleu clair
            R = 44.0f/255.0f;
            G = 117.0f/255.0f;
            B = 1.0f;
        }
        else{//bleu foncé
            R = 15.0f/255.0f;
            G = 5.0f/255.0f;
            B = 107.0f/255.0f;
        }
    }
    else{//PRINTEMPS

        if(alt > 0.2f){//blanc
           R = 1.0f;
           G = 1.0f;
           B = 1.0f;
        }
        else if (alt > 0.08f){//marron
            R = 88.0f/255.0f;
            G = 41.0f/255.0f;
            B = 0.0f;
        }
        else if (alt > 0.03f){//vert
            R = 20.0f/255.0f;
            G = 148.0f/255.0f;
            B = 20.0f/255.0f;
        }
        else if (alt > 0.015f){//bleu clair
            R = 44.0f/255.0f;
            G = 117.0f/255.0f;
            B = 1.0f;
        }
        else{//bleu foncé
            R = 15.0f/255.0f;
            G = 5.0f/255.0f;
            B = 107.0f/255.0f;
        }
    }

    glColor3f(R, G, B);

}

void GameWindowShader::loadTexture2(char *filename, GLuint &textureID){
    glEnable(GL_TEXTURE_2D); // Enable texturing

    glGenTextures(1, &textureID); // Obtain an id for the texture
    glBindTexture(GL_TEXTURE_2D, textureID); // Set as the current texture

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    QImage im(filename);

    QImage tex = QGLWidget::convertToGLFormat(im);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glDisable(GL_TEXTURE_2D);

}


int GameWindowShader::day() const
{
    return m_day;
}

void GameWindowShader::setDay(int day)
{
    m_day = day;
}

int GameWindowShader::type() const
{
    return m_type;
}

void GameWindowShader::setType(int type)
{
    m_type = type;
}

Camera *GameWindowShader::camera() const
{
    return m_camera;
}

void GameWindowShader::setCamera(Camera *camera)
{
    m_camera = camera;
}

int GameWindowShader::getCarte() const
{
    return carte;
}

void GameWindowShader::setCarte(int value)
{
    carte = value;
}

bool GameWindowShader::getIsRotating() const
{
    return m_isRotating;
}

void GameWindowShader::setIsRotating(bool isRotating)
{
    m_isRotating = isRotating;
}

QList<float> GameWindowShader::getObjectList() const
{
    return m_objectList;
}

void GameWindowShader::setObjectList(const QList<float> &objectList)
{
    m_objectList = objectList;
}


void GameWindowShader::division(quadtree* quad, int borneInf_I, int borneSup_I, int borneInf_J, int borneSup_J, int seuil, int taille_min){

    int nW = m_image.width();

    if((borneSup_I - borneInf_I) <= taille_min){

        quad->fils1 = NULL;
        quad->fils2 = NULL;
        quad->fils3 = NULL;
        quad->fils4 = NULL;


        if(borneSup_I == nW)
            borneSup_I = nW -1;
        if(borneSup_J == nW)
            borneSup_J= nW -1;

        //1er points en bas a gauche
        quad->coord[0] = borneInf_I;
        quad->coord[1] = borneInf_J;
        //2eme points en bas a droite
        quad->coord[2] = borneSup_I;
        quad->coord[3] = borneInf_J;
        //3eme points en haut a gauche
        quad->coord[4] = borneInf_I;
        quad->coord[5] = borneSup_J;
        //4eme points en haut a gauche
        quad->coord[6] = borneSup_I;
        quad->coord[7] = borneSup_J;

        return;
    }


   int proba1 [256] = {0};
   int proba2 [256] = {0};
   int proba3 [256] = {0};
   int proba4 [256] = {0};

    int milieu_I = borneInf_I + ((borneSup_I - borneInf_I)/2);
    int milieu_J = borneInf_J + ((borneSup_J - borneInf_J)/2);

    for (int i = borneInf_I; i < borneSup_I; i++)
     for (int j = borneInf_J; j < borneSup_J; j++)
     {
        if( i < milieu_I && j < milieu_J)
            ++proba1[(int)(p[i*nW + j].z*1000)];
        if( i < milieu_I && j > milieu_J )
            ++proba2[(int)(p[i*nW + j].z*1000)];
        if( i > milieu_I && j < milieu_J )
            ++proba3[(int)(p[i*nW + j].z*1000)];
        if( i > milieu_I && j > milieu_J )
            ++proba4[(int)(p[i*nW + j].z*1000)];
     }

   int somme1 = 0;
   int somme2 = 0;
   int somme3 = 0;
   int somme4 = 0;

   double sommeVariance1_1 = 0;
   double sommeVariance2_1 = 0;
   double sommeVariance3_1 = 0;
   double sommeVariance4_1 = 0;

   double dividende = ((borneSup_I-borneInf_I) / 2)*((borneSup_J-borneInf_J) / 2);

   for (int i = borneInf_I; i < borneSup_I; i++)
     for (int j = borneInf_J; j < borneSup_J; j++)
     {

        if( i < milieu_I && j < milieu_J ){
            somme1 += (int)(p[i*nW + j].z*1000);
            sommeVariance1_1 += ((double)proba1[(int)(p[i*nW + j].z*1000)]/dividende) * ((int)(p[i*nW + j].z*1000)*(int)(p[i*nW + j].z*1000));
        }
        if( i < milieu_I && j > milieu_J ){
            somme2 += (int)(p[i*nW + j].z*1000);
            sommeVariance2_1 += ((double)proba2[(int)(p[i*nW + j].z*1000)]/dividende) * ((int)(p[i*nW + j].z*1000)*(int)(p[i*nW + j].z*1000));
        }
        if( i > milieu_I && j < milieu_J ){
            somme3 += (int)(p[i*nW + j].z*1000);
            sommeVariance3_1 += ((double)proba3[(int)(p[i*nW + j].z*1000)]/dividende) * ((int)(p[i*nW + j].z*1000)*(int)(p[i*nW + j].z*1000));
        }
        if( i > milieu_I && j > milieu_J ){
            somme4 += (int)(p[i*nW + j].z*1000);
            sommeVariance4_1 += ((double)proba4[(int)(p[i*nW + j].z*1000)]/dividende) * ((int)(p[i*nW + j].z*1000)*(int)(p[i*nW + j].z*1000));
        }

     }

    int moyenne1 = somme1 / dividende;
    int moyenne2 = somme2 / dividende;
    int moyenne3 = somme3 / dividende;
    int moyenne4 = somme4 / dividende;

    double variance1 = (sommeVariance1_1 - (moyenne1*moyenne1) )/dividende;
    double variance2 = (sommeVariance2_1 - (moyenne2*moyenne2) )/dividende;
    double variance3 = (sommeVariance3_1 - (moyenne3*moyenne3) )/dividende;
    double variance4 = (sommeVariance4_1 - (moyenne4*moyenne4) )/dividende;

    //qDebug() << "Variance 1" << variance1;
    //qDebug() << "Variance 2" << variance2;
    //qDebug() << "Variance 3" << variance3;
    //qDebug() << "Variance 4" << variance4 << endl;

    quadtree * fils1 = new quadtree();
    quadtree * fils2 = new quadtree();
    quadtree * fils3 = new quadtree();
    quadtree * fils4 = new quadtree();

    if(variance1 > seuil){
        //qDebug() << "Fils 1 division";
        division(fils1, borneInf_I, milieu_I,  borneInf_J, milieu_J, seuil,taille_min);
        //qDebug() << "Division fils 1 faite";
    }
    else{

        //qDebug() << "Fils 1 triangle";

        fils1->fils1 = NULL;
        fils1->fils2 = NULL;
        fils1->fils3 = NULL;
        fils1->fils4 = NULL;

        //1er points en bas a gauche
        fils1->coord[0] = borneInf_I;
        fils1->coord[1] = borneInf_J;
        //2eme points en bas a droite
        fils1->coord[2] = milieu_I;
        fils1->coord[3] = borneInf_J;
        //3eme points en haut a gauche
        fils1->coord[4] = borneInf_I;
        fils1->coord[5] = milieu_J;
        //4eme points en haut a gauche
        fils1->coord[6] = milieu_I;
        fils1->coord[7] = milieu_J;
    }

    if(variance2 > seuil){
        //qDebug() << "Fils 2 division";
        division(fils2, milieu_I, borneSup_I,  borneInf_J, milieu_J, seuil,taille_min);
        //qDebug() << "Division fils 2 faite";
    }
    else{

        //qDebug() << "Fils 2 triangle";

        fils2->fils1 = NULL;
        fils2->fils2 = NULL;
        fils2->fils3 = NULL;
        fils2->fils4 = NULL;

        boolean hasChanged = false;

        if(borneSup_I == nW){
            borneSup_I = nW -1;
            hasChanged = true;
        }

        //1er points en bas a gauche
        fils2->coord[0] = milieu_I;
        fils2->coord[1] = borneInf_J;
        //2eme points en bas a droite
        fils2->coord[2] = borneSup_I;
        fils2->coord[3] = borneInf_J;
        //3eme points en haut a gauche
        fils2->coord[4] = milieu_I;
        fils2->coord[5] = milieu_J;
        //4eme points en haut a gauche
        fils2->coord[6] = borneSup_I;
        fils2->coord[7] = milieu_J;

        if(hasChanged)
            borneSup_I++;
    }
    if(variance3 > seuil){
        //qDebug() << "Fils 3 division";
        division(fils3, borneInf_I, milieu_I,  milieu_J, borneSup_J, seuil,taille_min);
        //qDebug() << "Division fils 3 faite";
    }
    else{
        //qDebug() << "Fils 3 triangle";

        fils3->fils1 = NULL;
        fils3->fils2 = NULL;
        fils3->fils3 = NULL;
        fils3->fils4 = NULL;

        boolean hasChanged = false;
        if(borneSup_J == nW){
            borneSup_J= nW -1;
            hasChanged = true;
        }

        //1er points en bas a gauche
        fils3->coord[0] = borneInf_I;
        fils3->coord[1] = milieu_J;
        //2eme points en bas a droite
        fils3->coord[2] = milieu_I;
        fils3->coord[3] = milieu_J;
        //3eme points en haut a gauche
        fils3->coord[4] = borneInf_I;
        fils3->coord[5] = borneSup_J;
        //4eme points en haut a gauche
        fils3->coord[6] = milieu_I;
        fils3->coord[7] = borneSup_J;

        if(hasChanged)
            borneSup_J++;
    }
    if(variance4 > seuil){
        //qDebug() << "Fils 4 division";
        division(fils4, milieu_I, borneSup_I,  milieu_J, borneSup_J, seuil,taille_min);
        //qDebug() << "Division fils 4 faite";
    }
    else{
        //qDebug() << "Fils 4 triangle";
        fils4->fils1 = NULL;
        fils4->fils2 = NULL;
        fils4->fils3 = NULL;
        fils4->fils4 = NULL;

        if(borneSup_I == nW)
            borneSup_I = nW -1;
        if(borneSup_J == nW)
            borneSup_J= nW -1;

        //1er points en bas a gauche
        fils4->coord[0] = milieu_I;
        fils4->coord[1] = milieu_J;
        //2eme points en bas a droite
        fils4->coord[2] = borneSup_I;
        fils4->coord[3] = milieu_J;
        //3eme points en haut a gauche
        fils4->coord[4] = milieu_I;
        fils4->coord[5] = borneSup_J;
        //4eme points en haut a droite
        fils4->coord[6] = borneSup_I;
        fils4->coord[7] = borneSup_J;


    }

    quad->fils1 = fils1;
    quad->fils2 = fils2;
    quad->fils3 = fils3;
    quad->fils4 = fils4;


}


void GameWindowShader::callDisplayLinesQuadtree(quadtree * quad){

    m_programNM->bind();

    QMatrix4x4 matrix = QMatrix4x4();
    matrix.scale(m_camera->get_ss());
    matrix.rotate(m_camera->get_rotX(), 1, 0, 0);
    matrix.rotate(m_camera->get_rotY(), 0, 0, 1);

    m_programNM->setUniformValue(m_matrixUniform, matrix);

    glUniform1i(m_programNM->uniformLocation("texture1"), 0);
    glUniform1i(m_programNM->uniformLocation("texture2"), 1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalMap);

    displayLinesQuadtree(quad);

    m_programNM->release();

}

void GameWindowShader::displayLinesQuadtree(quadtree * quad)
{

    //si le fils 1 est null, le noeud est une fueille, on doit afficher les 4 points
    if(quad->fils1 == NULL){

        int width = m_image.width();
        int height = m_image.height();

        int wBG = quad->coord[0];
        int hBG = quad->coord[1];
        point ptsBG = p[wBG*width + hBG];

        int wBD = quad->coord[2];
        int hBD = quad->coord[3];
        point ptsBD = p[wBD*width + hBD];

        int wHG = quad->coord[4];
        int hHG = quad->coord[5];
        point ptsHG = p[wHG*width + hHG];

        int wHD = quad->coord[6];
        int hHD = quad->coord[7];
        point ptsHD = p[wHD*width + hHD];

        bindTextureRegardingZ(ptsBG.z);
        glBegin(GL_LINES);

        glTexCoord2f((float)(wBG)/(float)(width),(float)(hBG)/(float)(height));
        glVertex3f( ptsBG.x,
                    ptsBG.y,
                    ptsBG.z);

        glTexCoord2f((float)(wBD)/(float)(width),(float)(hBD)/(float)(height));
        glVertex3f( ptsBD.x,
                    ptsBD.y,
                    ptsBD.z);


        glTexCoord2f((float)(wBG)/(float)(width),(float)(hBG)/(float)(height));
        glVertex3f( ptsBG.x,
                    ptsBG.y,
                    ptsBG.z);

        glTexCoord2f((float)(wHG)/(float)(width),(float)(hHG)/(float)(height));
        glVertex3f( ptsHG.x,
                    ptsHG.y,
                    ptsHG.z);

        glTexCoord2f((float)(wBD)/(float)(width),(float)(hBD)/(float)(height));
        glVertex3f( ptsBD.x,
                    ptsBD.y,
                    ptsBD.z);

        glTexCoord2f((float)(wHG)/(float)(width),(float)(hHG)/(float)(height));
        glVertex3f( ptsHG.x,
                    ptsHG.y,
                    ptsHG.z);

        glEnd();


        bindTextureRegardingZ(ptsHD.z);
        glBegin(GL_LINES);

        glTexCoord2f((float)(wHD)/(float)(width),(float)(hHD)/(float)(height));
        glVertex3f( ptsHD.x,
                    ptsHD.y,
                    ptsHD.z);

        glTexCoord2f((float)(wBD)/(float)(width),(float)(hBD)/(float)(height));
        glVertex3f( ptsBD.x,
                    ptsBD.y,
                    ptsBD.z);


        glTexCoord2f((float)(wHD)/(float)(width),(float)(hHD)/(float)(height));
        glVertex3f( ptsHD.x,
                    ptsHD.y,
                    ptsHD.z);

        glTexCoord2f((float)(wHG)/(float)(width),(float)(hHG)/(float)(height));
        glVertex3f( ptsHG.x,
                    ptsHG.y,
                    ptsHG.z);

       glEnd();

    }
    else{

        displayLinesQuadtree(quad->fils1);
        displayLinesQuadtree(quad->fils2);
        displayLinesQuadtree(quad->fils3);
        displayLinesQuadtree(quad->fils4);

    }
}


void GameWindowShader::callDisplayPointQuadtree(quadtree * quad){

    m_programNM->bind();

    QMatrix4x4 matrix = QMatrix4x4();
    matrix.scale(m_camera->get_ss());
    matrix.rotate(m_camera->get_rotX(), 1, 0, 0);
    matrix.rotate(m_camera->get_rotY(), 0, 0, 1);

    m_programNM->setUniformValue(m_matrixUniform, matrix);

    glUniform1i(m_programNM->uniformLocation("texture1"), 0);
    glUniform1i(m_programNM->uniformLocation("texture2"), 1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalMap);

    displayPointsQuadtree(quad);

    m_programNM->release();

}

void GameWindowShader::displayPointsQuadtree(quadtree * quad)
{

    //si le fils 1 est null, le noeud est une fueille, on doit afficher les 4 points
    if(quad->fils1 == NULL){


        int w;
        int h;
        int width = m_image.width();
        int height = m_image.height();
        point pts;
        for(int i = 0; i < 8; i += 2){

            w = quad->coord[i];
            h = quad->coord[i+1];

            pts = p[w*width + h];

            bindTextureRegardingZ(pts.z);
            glBegin(GL_POINTS);

            //displayColorSeasons(pts.z);

            glTexCoord2f((float)(w)/(float)(width),(float)(h)/(float)(height));
            glVertex3f( pts.x,
                        pts.y,
                        pts.z);
            glEnd();
        }


    }
    else{

        displayPointsQuadtree(quad->fils1);
        displayPointsQuadtree(quad->fils2);
        displayPointsQuadtree(quad->fils3);
        displayPointsQuadtree(quad->fils4);

    }
}

void GameWindowShader::callDisplayTriangleQuadtree(quadtree * quad){

    m_programNM->bind();

    QMatrix4x4 matrix = QMatrix4x4();
    matrix.scale(m_camera->get_ss());
    matrix.rotate(m_camera->get_rotX(), 1, 0, 0);
    matrix.rotate(m_camera->get_rotY(), 0, 0, 1);

    m_programNM->setUniformValue(m_matrixUniform, matrix);

    glUniform1i(m_programNM->uniformLocation("texture1"), 0);
    glUniform1i(m_programNM->uniformLocation("texture2"), 1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalMap);

    displayTriangleQuadtree(quad);

    m_programNM->release();

}

void GameWindowShader::displayTriangleQuadtree(quadtree * quad)
{

    //si le fils 1 est null, le noeud est une fueille, on doit afficher les 4 points
    if(quad->fils1 == NULL){

        int w;
        int h;
        int width = m_image.width();
        int height = m_image.height();
        point pts;

        w = quad->coord[0];
        h = quad->coord[1];
        pts = p[w*width + h];

        bindTextureRegardingZ(pts.z);
        glBegin(GL_TRIANGLES);
        glVertex3f( pts.x, pts.y, pts.z);
        glTexCoord2f((float)(w)/(float)(width),(float)(h)/(float)(height));

        w = quad->coord[4];
        h = quad->coord[5];
        pts = p[w*width + h];
        glVertex3f( pts.x, pts.y, pts.z);
        glTexCoord2f((float)(w)/(float)(width),(float)(h)/(float)(height));

        w = quad->coord[2];
        h = quad->coord[3];
        pts = p[w*width + h];
        glVertex3f( pts.x, pts.y, pts.z);
        glTexCoord2f((float)(w)/(float)(width),(float)(h)/(float)(height));

        w = quad->coord[4];
        h = quad->coord[5];
        pts = p[w*width + h];

        glVertex3f( pts.x, pts.y, pts.z);
        glTexCoord2f((float)(w)/(float)(width),(float)(h)/(float)(height));

        w = quad->coord[6];
        h = quad->coord[7];
        pts = p[w*width + h];
        glVertex3f( pts.x, pts.y, pts.z);
        glTexCoord2f((float)(w)/(float)(width),(float)(h)/(float)(height));

        w = quad->coord[2];
        h = quad->coord[3];
        pts = p[w*width + h];
        glVertex3f( pts.x, pts.y, pts.z);
        glTexCoord2f((float)(w)/(float)(width),(float)(h)/(float)(height));

        glEnd();

    }
    else{

        displayTriangleQuadtree(quad->fils1);
        displayTriangleQuadtree(quad->fils2);
        displayTriangleQuadtree(quad->fils3);
        displayTriangleQuadtree(quad->fils4);

    }
}
