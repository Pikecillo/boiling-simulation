#include <GL/glut.h>
#include <iostream>

#include "HeatField.h"

const size_t height = 512, width = 512;
HeatField heat_field(height, width);

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    const float height = static_cast<float>(heat_field.height());
    const float width = static_cast<float>(heat_field.width());

    for (size_t i = 0; i < heat_field.height(); i++)
        for (size_t j = 0; j < heat_field.width(); j++)
        {
            const float tone = heat_field.at(i, j) / 10.0;

            if (i == 0)
                glColor3f(1.0, 0.0, 0.0);
            else if (i == heat_field.height() - 1)
                glColor3f(1.0, 1.0, 1.0);
            else
                glColor3f(tone, tone, 1.0);

            const float x0 = j / width, y0 = i / height;
            const float x1 = (j + 1) / width, y1 = (i + 1) / height;

            glBegin(GL_POLYGON);
            glVertex3f(x0, y0, 0.0);
            glVertex3f(x1, y0, 0.0);
            glVertex3f(x1, y1, 0.0);
            glVertex3f(x0, y1, 0.0);
            glEnd();
        }

    glutSwapBuffers();
}

void idle()
{
    heat_field.update();

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(350, 350);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Simple Boiling");

    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}
