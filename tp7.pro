QMAKE_CXXFLAGS += -std=c++11 -fopenmp
LIBS += -fopenmp

INCLUDEPATH += /opt/local/include

QT += gui core network widgets


SOURCES += $$PWD/openglwindow.cpp \
    main.cpp \
    filemanager.cpp \
    gameobject.cpp \
    test-serialize.cpp \
    meshreader.cpp \
    mesh.cpp \
    offmeshreader.cpp \
    plymeshreader.cpp \
    stlmeshreader.cpp \
    scene.cpp \
    gamewindow.cpp \
    assetmanager.cpp \
    terrain.cpp \
    renderer.cpp \
    quadtree.cpp \
    qquadtree.cpp
HEADERS += $$PWD/openglwindow.h \
    filemanager.h \
    gameobject.h \
    meshreader.h \
    mesh.h \
    offmeshreader.h \
    plymeshreader.h \
    stlmeshreader.h \
    scene.h \
    gamewindow.h \
    assetmanager.h \
    terrain.h \
    renderer.h \
    quadtree.h \
    qquadtree.h

target.path = .

INSTALLS += target

RESOURCES += gestionnaire.qrc

OTHER_FILES +=
