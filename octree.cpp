#include "octree.h"

Octree::Octree()
{
    parent = Node(-0.5, -0.5, 0, 1, 1, 1);
}

void Octree::addEntity(PlyEntity *entity)
{
    entities.insert(entity);
    parent.addEntity(entity);
}

void Octree::draw()
{
    parent.draw();
}

void Octree::update()
{
    parent.clear();
    foreach (PlyEntity *e, entities) {
        addEntity(e);
    }
}


Node::Node()
{

}

Node::Node(float x, float y, float z, float width, float height, float depth)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->width = width;
    this->height = height;
    this->depth = depth;
    this->collides = false;
}

void Node::addEntity(PlyEntity *entity)
{
    if(width > MIN_SIZE) {
        float newX = x, newY = y, newZ = z;

        if(entity->getX() > x + width * 0.5)
            newX = x + width * 0.5;

        if(entity->getY() > y + height * 0.5)
            newY = y + height * 0.5;

        if(entity->getZ() > z + depth * 0.5)
            newZ = z + depth * 0.5;

        int i = 0;
        Node n(newX, newY, newZ, width * 0.5, height * 0.5, depth * 0.5);
        foreach (Node other, nodes) {
            if(n.equal(other)) {
                nodes[i].addEntity(entity);
                return;
            }
            i++;
        }
        n.addEntity(entity);
        nodes.push_back(n);

    } else {
        entities.insert(entity);
    }
}

void Node::draw()
{
    if(entities.size() > 1)
        glColor3f(1, 0, 0);
    else
        glColor3f(0, 1, 0);

    glBegin(GL_LINE_STRIP);
    glVertex3f(x, y, z);
    glVertex3f(x + width, y, z);
    glVertex3f(x + width, y + height, z);
    glVertex3f(x, y + height, z);
    glVertex3f(x, y, z);
    glVertex3f(x, y, z + depth);
    glVertex3f(x, y + height, z + depth);
    glVertex3f(x, y + height, z);
    glVertex3f(x + width, y + height, z);
    glVertex3f(x + width, y + height, z + depth);
    glVertex3f(x, y + height, z + depth);
    glVertex3f(x, y, z + depth);
    glVertex3f(x + width, y, z + depth);
    glVertex3f(x + width, y, z);
    glVertex3f(x + width, y, z + depth);
    glVertex3f(x + width, y + height, z + depth);
    glEnd();

    foreach (Node child, nodes) {
        child.draw();
    }
}

void Node::clear()
{
    foreach (Node node, nodes) {
        if(node.nodes.size() == 0) {
            node.entities.clear();
        } else {
            entities.clear();
            node.clear();
            nodes.clear();
        }
    }

}

bool Node::equal(Node n)
{
    return(n.x == x && n.y == y && n.z == z &&
            n.width == width && n.height == height && n.depth == depth);
}
