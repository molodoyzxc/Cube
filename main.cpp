#include <bits/stdc++.h>
#include "Cube.h"
#include <GL/glut.h>
#include <GL/freeglut.h>

CubeRubik cube;

string user_input;

vector<Rotate_anim> steps;
int iter;
int n;

bool random = false;
bool solve = false;
float increment = 5;

int refresh_mills = 15;

float angle = 0;

bool animation_in_progress = false;

const float length_to_edge = 2.1;
const float length_to_corner = 2.96985;
const float pi_div_180 = 0.01745329;

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
}

vector<GLfloat> charToFloat(char i) {
    vector<GLfloat> colors;
    switch (i) {
        case 'y':
            colors = {1.0f, 1.0f, 0.0f};
            break;
        case 'w':
            colors = {1.0f, 1.0f, 1.0f};
            break;
        case 'b':
            colors = {0.0f, 0.0f, 1.0f};
            break;
        case 'g':
            colors = {0.1f, 1.0f, 0.0f};
            break;
        case 'o':
            colors = {1.0f, 0.5f, 0.0f};
            break;
        case 'r':
            colors = {1.0f, 0.0f, 0.0f};
            break;
        default:
            colors = {0.1f, 0.1f, 0.1f};
            break;
    }
    return colors;
}

float mysin(float angle, float length) {
    return sin(angle * pi_div_180) * length;
}

float mycos(float angle, float length) {
    return cos(angle * pi_div_180) * length;
}

void start_animation() {
    if (steps.size() == 0)
        return;
    animation_in_progress = true;
    iter = 0;
    n = steps.size();
}

void translation_coords(vector<GLfloat> &translation, char axis, float j, float k) {
    int n = -1;
    switch (axis) {
        case 'x':
            n = 0;
            break;
        case 'y':
            n = 1;
            break;
        case 'z':
            n = 2;
            break;
    }
    translation = {j, k};
    translation.insert(translation.begin() + n, 0.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    Rotate_anim r = Rotate_anim();
    if (!animation_in_progress) {
        if (iter < n) {
            animation_in_progress = true;
            r = steps[iter];
        } else {
            n = 0;
            iter = 0;
            if (steps.size() != 0)
                steps.clear();
            if (solve) {
                cout << "Rubik's cube is done!\n";
                solve = false;
            }
            if (random) {
                cout << "A random state is set.\n";
                random = false;
                increment /= 10;
            }
        }
    } else {
        r = steps[iter];
        if (angle >= 90 || angle <= -90) {
            if (solve) {
                if (r.value_axis == 2)
                    cout << "Rotation ";
                cout << r.command << endl;
            }
            animation_in_progress = false;
            iter++;
            angle = 0;
            cube.rotation_helper(r.axis, r.value_axis, r.inv);
        }
    }
    for (int i = -1; i < SIDE_LENGHT - 1; i++) {
        for (int j = -1; j < SIDE_LENGHT - 1; j++) {
            for (int k = -1; k < SIDE_LENGHT - 1; k++) {
                glLoadIdentity();
                gluLookAt(10, 10, 9.5, 0, 0, 0, 0, 90, 1);
                glTranslatef(i * length_to_edge, j * length_to_edge, k * length_to_edge);

                if (r.axis != '0') {
                    int a, b, c;
                    switch (r.axis) {
                        case 'x':
                            a = i;
                            b = j;
                            c = k;
                            break;
                        case 'y':
                            a = j;
                            b = i;
                            c = k;
                            break;
                        case 'z':
                            a = k;
                            b = i;
                            c = j;
                            break;
                    }

                    if (a == r.value_axis || r.value_axis == 2) {
                        switch (r.axis) {
                            case 'x':
                                glRotatef(angle, 1, 0, 0);
                                break;
                            case 'y':
                                glRotatef(-angle, 0, 1, 0);
                                break;
                            case 'z':
                                glRotatef(angle, 0, 0, 1);
                                break;
                        }

                        if (!(b == c && c == 0)) {
                            vector<GLfloat> translation;

                            if (b == 0)
                                translation_coords(translation, r.axis, c * mysin(-angle, length_to_edge),
                                                   c * (length_to_edge - mycos(angle, length_to_edge)));
                            else if (c == 0)
                                translation_coords(translation, r.axis,
                                                   b * (length_to_edge - mycos(angle, length_to_edge)),
                                                   b * mysin(angle, length_to_edge));
                            else if (b == c)
                                translation_coords(translation, r.axis,
                                                   b * (length_to_edge - mycos(45 - angle, length_to_corner)),
                                                   b * (length_to_edge - mysin(45 - angle, length_to_corner)));
                            else
                                translation_coords(translation, r.axis,
                                                   b * (mycos(135 - angle, length_to_corner) + length_to_edge),
                                                   b * (mysin(135 - angle, length_to_corner) - length_to_edge));

                            glTranslatef(translation[0], translation[1], translation[2]);
                        }
                    }
                }

                vector<GLfloat> v;
                glBegin(GL_QUADS);

                v = charToFloat(cube(i, j, k)["top"]);
                glColor3f(v[0], v[1], v[2]);
                glVertex3f(1.0f, 1.0f, -1.0f);
                glVertex3f(-1.0f, 1.0f, -1.0f);
                glVertex3f(-1.0f, 1.0f, 1.0f);
                glVertex3f(1.0f, 1.0f, 1.0f);

                v = charToFloat(cube(i, j, k)["down"]);
                glColor3f(v[0], v[1], v[2]);
                glVertex3f(1.0f, -1.0f, 1.0f);
                glVertex3f(-1.0f, -1.0f, 1.0f);
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(1.0f, -1.0f, -1.0f);

                v = charToFloat(cube(i, j, k)["front"]);
                glColor3f(v[0], v[1], v[2]);
                glVertex3f(1.0f, 1.0f, 1.0f);
                glVertex3f(-1.0f, 1.0f, 1.0f);
                glVertex3f(-1.0f, -1.0f, 1.0f);
                glVertex3f(1.0f, -1.0f, 1.0f);

                v = charToFloat(cube(i, j, k)["back"]);
                glColor3f(v[0], v[1], v[2]);
                glVertex3f(1.0f, -1.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(-1.0f, 1.0f, -1.0f);
                glVertex3f(1.0f, 1.0f, -1.0f);

                v = charToFloat(cube(i, j, k)["left"]);
                glColor3f(v[0], v[1], v[2]);
                glVertex3f(-1.0f, 1.0f, 1.0f);
                glVertex3f(-1.0f, 1.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, 1.0f);

                v = charToFloat(cube(i, j, k)["right"]);
                glColor3f(v[0], v[1], v[2]);
                glVertex3f(1.0f, 1.0f, -1.0f);
                glVertex3f(1.0f, 1.0f, 1.0f);
                glVertex3f(1.0f, -1.0f, 1.0f);
                glVertex3f(1.0f, -1.0f, -1.0f);

                glEnd();
            }
        }
    }
    glutSwapBuffers();
}

void timer(int) {
    if (animation_in_progress) {
        if (steps[iter].inv)
            angle -= increment;
        else
            angle += increment;
    }
    glutPostRedisplay();
    glutTimerFunc(refresh_mills, timer, 0);
}

void reshape(GLsizei width, GLsizei height) {
    if (height == 0)
        height = 1;
    GLfloat aspect = (GLfloat) width / (GLfloat) height;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void arrows(int key, int, int) {
    if (!solve) {
        switch (key) {
            case GLUT_KEY_UP:
                cube.rotation("up", false);
                break;
            case GLUT_KEY_DOWN:
                cube.rotation("down", false);
                break;
            case GLUT_KEY_LEFT:
                cube.rotation("left", false);
                break;
            case GLUT_KEY_RIGHT:
                cube.rotation("right", false);
                break;
        }
        start_animation();
    }
}

static void functions() {
    int length = user_input.length();
    if (user_input[0] == '/') {
        auto position = user_input.find(' ');
        if (user_input == "/check") {
            bool check = cube.check_correct();
            if (check) {
                CubeRubik additional_cube = cube;
                check = additional_cube.solve();
                steps.clear();
            }
            string s = (check) ? "correct" : "incorrect";
            cout << "Rubik's cube is " << s << ".\n";
        } else if (user_input == "/print") {
            cube.print();
        } else if (user_input == "/random") {
            increment *= 10;
            random = true;
            cube.generate_valid_cube();
            start_animation();
        } else if (user_input == "/solve") {
            if (!cube.check_correct()) {
                cout << "Rubik's cube is incorrect.\n";
                return;
            }

            CubeRubik additional_cube = cube;
            if (additional_cube.solve()) {
                solve = true;
                if (steps.size() != 0)
                    cout << "Turn sequence is:\n";
                start_animation();
            } else {
                cout << "Rubik's cube is incorrect.\n";
                solve = false;
            }

        } else if (position != string::npos) {
            string command = user_input.substr(0, position);
            string filename = user_input.substr(position + 1);
            string path = "C:/Users/semen/Документы/куб";
            if (command == "/save") {
                cube.save(path + filename);
            } else if (command == "/load") {
                cube.load(path + filename);
            } else
                cout << "Incorrect input\n";
        } else
            cout << "Incorrect input\n";
        return;
    }

    if (length == 1 || length == 2) {
        cube.motion(user_input, false);
        start_animation();
    } else
        cout << "Incorrect input\n";
}

static void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 8: //backspace
            if (user_input.length() > 0) {
                user_input.pop_back();
                cout << "\b \b";
            }
            break;
        case 13: //enter
            if (!solve) {
                cout << endl;
                functions();
                user_input.clear();
            }
            break;
        case 27: //escape
            exit(0);
        case '+':
            if (increment < 100)
                increment *= 2;
            break;
        case '-':
            if (increment > 0.1)
                increment /= 2;
            break;
        default:
            if (!solve) {
                user_input.push_back((char) key);
                cout << user_input[user_input.length() - 1];
            }
    }
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    cube = CubeRubik();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(640, 120);
    glutCreateWindow("Rubik's cube");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    initGL();
    glutTimerFunc(0, timer, 0);
    glutSpecialFunc(arrows);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}