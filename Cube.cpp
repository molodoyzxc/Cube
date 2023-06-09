#include "Cube.h"

Cube &CubeRubik::operator()(int i, int j, int k) {
    return c(i, j, k);
}

void CubeRubik::load(string filename) {
    ifstream file(filename);
    if (!file) {
        cout << "File was not opened\n";
        return;
    }

    for (int i = 0; i < SIDES_COUNT; i++) {
        for (int j = 0; j < SIDE_LENGHT; j++) {
            for (int k = 0; k < SIDE_LENGHT; k++) {
                for (int t = 0; t < SIDE_LENGHT; t++) {
                    if (c(j - 1, k - 1, t - 1)[i] != 'd') {
                        char ch;
                        file >> ch;
                        c(j - 1, k - 1, t - 1)[i] = ch;
                    }
                }
            }
        }
    }

    file.close();
    cout << "Cube successfully loaded from file.\n";
}

void CubeRubik::save(string filename) {
    ofstream file(filename);
    if (!file) {
        cout << "File was not opened\n";
        return;
    }

    for (int i = 0; i < SIDES_COUNT; i++) {
        for (int j = 0; j < SIDE_LENGHT; j++) {
            for (int k = 0; k < SIDE_LENGHT; k++) {
                for (int t = 0; t < SIDE_LENGHT; t++) {
                    if (c(j - 1, k - 1, t - 1)[i] != 'd') {
                        file << c(j - 1, k - 1, t - 1)[i] << " ";
                    }
                }
            }
        }
        file << endl;
    }

    file.close();
    cout << "Cube successfully loaded to file.\n";
}

void CubeRubik::print() {
    for (int i = 0; i < SIDES_COUNT; i++) {
        for (int j = 0; j < SIDE_LENGHT; j++) {
            for (int k = 0; k < SIDE_LENGHT; k++) {
                for (int t = 0; t < SIDE_LENGHT; t++) {
                    if (c(j - 1, k - 1, t - 1)[i] != 'd') {
                        cout << c(j - 1, k - 1, t - 1)[i] << " ";
                    }
                }
            }
        }
        cout << endl;
    }
}

string CubeRubik::colors_to_string(vector<char> &colors) {
    for (unsigned int i = 0; i < colors.size(); i++) {
        for (unsigned int j = i; j < colors.size(); j++) {
            if (colors[j] < colors[i])
                swap(colors[i], colors[j]);
        }
    }

    string s = "";
    for (unsigned int i = 0; i < colors.size(); i++) {
        s += colors[i];
    }
    return s;
}

bool CubeRubik::color_opposite(char a, char b) {
    vector<char> all_colors = {'y', 'b', 'r', 'o', 'g', 'w'};
    for (int i = 0; i < SIDES_COUNT; i++) {
        if (a == all_colors[i]) {
            if (all_colors[SIDES_COUNT - 1 - i] == b)
                return true;
            return false;
        }
    }
    return false;
}

bool CubeRubik::check_correct() {
    Info standart_cube = Info();
    map<string, int> components;
    for (int i = 0; i < SIDE_LENGHT; i++) {
        for (int j = 0; j < SIDE_LENGHT; j++) {
            for (int k = 0; k < SIDE_LENGHT; k++) {
                vector<char> colors = standart_cube(i - 1, j - 1, k - 1).getColors();
                if (colors.size() == 0)
                    continue;
                string s = colors_to_string(colors);
                components[s] = 1;
            }
        }
    }

    for (int i = 0; i < SIDE_LENGHT; i++) {
        for (int j = 0; j < SIDE_LENGHT; j++) {
            for (int k = 0; k < SIDE_LENGHT; k++) {
                vector<char> colors = c(i - 1, j - 1, k - 1).getColors();
                if (colors.size() == 0)
                    continue;
                string s = colors_to_string(colors);
                if (components.find(s) != components.end()) {
                    if (components[s] == 1)
                        components[s] = 0;
                    else
                        return false;
                } else
                    return false;
            }
        }
    }

    if (!color_opposite(c(0, 1, 0)["top"], c(0, -1, 0)["down"]))
        return false;
    if (!color_opposite(c(0, 0, 1)["front"], c(0, 0, -1)["back"]))
        return false;
    if (!color_opposite(c(1, 0, 0)["right"], c(-1, 0, 0)["left"]))
        return false;

    return true;
}

void CubeRubik::rotation_helper(char axis, int value_axis, bool inv) {

    vector<vector<int>> r =
            {
                    {-1, -1},
                    {-1, 0},
                    {-1, 1},
                    {0,  1},
                    {1,  1},
                    {1,  0},
                    {1,  -1},
                    {0,  -1}
            };
    if (inv)
        reverse(r.begin(), r.end());
    int rotation_end = value_axis;
    if (value_axis == 2) {
        value_axis = -1;
        rotation_end = 1;
    }
    for (int i = value_axis; i <= rotation_end; i++) {
        for (int t = -1; t < SIDE_LENGHT - 1; t++) {
            for (int j = -1; j < SIDE_LENGHT - 1; j++) {
                switch (axis) {
                    case 'x':
                        c(i, t, j).rotation("down", inv);
                        break;
                    case 'y':
                        c(t, i, j).rotation("left", inv);
                        break;
                    case 'z':
                        c(t, j, i).rotation("up_dop", inv);
                        break;
                }
            }
        }

        Cube c1 = c(i, r[0][0], r[0][1], axis);
        Cube c2 = c(i, r[1][0], r[1][1], axis);
        for (unsigned int k = 0; k < r.size() - 3; k += 2) {
            c(i, r[k][0], r[k][1], axis) = c(i, r[k + 2][0], r[k + 2][1], axis);
            c(i, r[k + 1][0], r[k + 1][1], axis) = c(i, r[k + 3][0], r[k + 3][1], axis);

        }
        c(i, r[r.size() - 2][0], r[r.size() - 2][1], axis) = c1;
        c(i, r[r.size() - 1][0], r[r.size() - 1][1], axis) = c2;
    }
}

void CubeRubik::motion(string s, bool check_rotate) {
    bool reverse_rotation = false;
    if (s.length() == 2) {
        if (s[1] == '\'') {
            reverse_rotation = true;
        } else {
            cout << "Incorrect input\n";
            return;
        }
    }
    char axis;
    int value_axis;
    switch (s[0]) {
        case 'r':
        case 'R':
            axis = 'x';
            value_axis = 1;
            break;
        case 'l':
        case 'L':
            axis = 'x';
            value_axis = -1;
            break;
        case 'u':
        case 'U':
            axis = 'y';
            value_axis = 1;
            break;
        case 'd':
        case 'D':
            axis = 'y';
            value_axis = -1;
            break;
        case 'f':
        case 'F':
            axis = 'z';
            value_axis = 1;
            break;
        case 'b':
        case 'B':
            axis = 'z';
            value_axis = -1;
            break;
        default:
            cout << "Incorrect input\n";
            return;
    }
    bool inv = reverse_rotation ^ (value_axis == -1) ^ (axis != 'y');
    if (check_rotate)
        rotation_helper(axis, value_axis, inv);
    steps.push_back(Rotate_anim(axis, value_axis, inv, s));
}

void CubeRubik::motion(string s) {
    motion(s, true);
}

void CubeRubik::rotation(string s, bool check_rotate) {
    bool inv = false;
    char axis;
    if (s == "up") {
        axis = 'x';
        inv = true;
    } else if (s == "down") {
        axis = 'x';
    } else if (s == "left") {
        axis = 'y';
        inv = true;
    } else if (s == "right") {
        axis = 'y';
    } else
        throw runtime_error("No such rotation");
    if (check_rotate)
        rotation_helper(axis, 2, inv);
    steps.emplace_back(axis, 2, inv, s);
}

void CubeRubik::rotation(string s) {
    rotation(s, true);
}

void CubeRubik::generate_valid_cube() {
    c = Info();
    srand(time(nullptr));
    int n = rand() % 50 + 25;
    for (int i = 0; i < n; i++) {
        int k = rand() % 10;
        switch (k) {
            case 0:
                motion("R", false);
                break;
            case 1:
                motion("L", false);
                break;
            case 2:
                motion("U", false);
                break;
            case 3:
                motion("D", false);
                break;
            case 4:
                motion("F", false);
                break;
            case 5:
                motion("B", false);
                break;
            case 6:
                rotation("up", false);
                break;
            case 7:
                rotation("down", false);
                break;
            case 8:
                rotation("left", false);
                break;
            case 9:
                rotation("right", false);
                break;
        }
    }

}

void CubeRubik::double_function(string type, string command) {
    bool rotation_motion;
    if (type == "rotation")
        rotation_motion = false;
    else if (type == "motion")
        rotation_motion = true;
    else
        throw runtime_error("Double function call was wrong");
    for (int i = 0; i < 2; i++) {
        if (rotation_motion)
            motion(command);
        else
            rotation(command);
    }
}

void CubeRubik::cross_yellow_up() {
    if (c(0, 1, 0)["top"] == 'y')
        return;
    for (int i = 0; i < 3; i++) {
        if (c(0, 1, 0)["top"] == 'y')
            return;
        rotation("up");
    }
}

void CubeRubik::cross_rotate_top(int x, int z) {
    for (int i = 0; i < 4; i++) {
        if (c(x, 1, z)["top"] == 'w')
            motion("U");
        else
            return;
    }
}

void CubeRubik::cross_whites_up(int &counter) {
    int k = 1;
    if (c(-1, 0, k)["front"] == 'w') {
        cross_rotate_top(-1, 0);
        motion("L'");
        counter++;
    }
    if (c(1, 0, k)["front"] == 'w') {
        cross_rotate_top(1, 0);
        motion("R");
        counter++;
    }
    if (c(0, 1, k)["front"] == 'w') {
        if (c(-1, 0, k)["left"] == 'w')
            counter++;
        motion("F");
        cross_rotate_top(1, 0);
        motion("R");
        counter++;
    }
    if (c(0, -1, k)["front"] == 'w') {
        if (c(-1, 0, k)["left"] == 'w')
            counter++;
        motion("F");
        cross_rotate_top(-1, 0);
        motion("L'");

        if (c(1, 0, k)["right"] == 'w') {
            cross_rotate_top(0, 1);
            motion("F'");
        }

        counter++;
    }
    if (c(0, -1, k)["down"] == 'w') {
        cross_rotate_top(0, 1);
        double_function("motion", "F");
        counter++;
    }
}

void CubeRubik::cross_rotate_top() {
    for (int i = 0; i < 3; i++) {
        if (c(0, 1, 1)["front"] == c(0, 0, 1)["front"]
            && c(0, 1, 1)["top"] == 'w')
            return;
        motion("U");
    }
}

void CubeRubik::cross() {
    while (true) {
        cross_yellow_up();
        if (c(0, 1, 0)["top"] == 'y')
            break;
        rotation("right");
    }

    int counter = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 || j == 0)
                if (!(i == 0 && j == 0))
                    if (c(i, 1, j)["top"] == 'w')
                        counter++;
        }
    }

    while (counter != 4) {
        cross_whites_up(counter);
        rotation("right");
    }

    for (int i = 0; i < 4; i++) {
        cross_rotate_top();
        double_function("motion", "F");
        rotation("right");
    }
}

void CubeRubik::pif_paf() {
    motion("R");
    motion("U");
    motion("R'");
    motion("U'");
}

vector<char> CubeRubik::color_corner(int x, int y, int z, char color) {
    vector<char> corner = c(x, y, z).getColors();
    for (unsigned int i = 0; i < corner.size(); i++) {
        if (corner[i] == color) {
            corner.erase(corner.begin() + i);
            return corner;
        }
    }
    vector<char> emp;
    return emp;
}

bool CubeRubik::colors_existence(vector<char> &corner, char a, char b) {
    if (corner[0] == a && corner[1] == b)
        return true;
    if (corner[0] == b && corner[1] == a)
        return true;
    return false;
}

void CubeRubik::first_layer_rotate_top() {
    for (int i = 0; i < 3; i++) {
        vector<char> top_corner = color_corner(1, 1, 1, 'w');
        if (top_corner.size() == 0)
            return;
        motion("U");
    }
}

void CubeRubik::first_later_rotate_top(char a, char b) {
    for (int i = 0; i < 3; i++) {
        vector<char> top_corner = color_corner(1, 1, 1, 'w');
        if (top_corner.size() != 0) {
            bool check = colors_existence(top_corner, a, b);
            if (check)
                return;
        }
        motion("U");
    }
}

void CubeRubik::first_layer() {
    for (int i = 0; i < 4; i++) {
        vector<char> corner = color_corner(1, -1, 1, 'w');
        if (corner.size() != 0) {
            char color_front = c(0, 0, 1)["front"];
            char color_right = c(1, 0, 0)["right"];
            bool check = colors_existence(corner, color_front, color_right);
            if (!check) {
                first_layer_rotate_top();
                pif_paf();
            }
        }
        rotation("right");
    }
    for (int i = 0; i < 4; i++) {
        char color_front = c(0, 0, 1)["front"];
        char color_right = c(1, 0, 0)["right"];
        vector<char> corner = color_corner(1, -1, 1, 'w');

        if (corner.size() == 0) {
            first_later_rotate_top(color_front, color_right);
        }

        while (true) {
            Cube corner_cube = c(1, -1, 1);
            if (corner_cube["down"] == 'w'
                && corner_cube["front"] == color_front
                && corner_cube["right"] == color_right)
                break;
            pif_paf();
        }
        rotation("right");
    }
}

void CubeRubik::pif_paf_left() {
    motion("L'");
    motion("U'");
    motion("L");
    motion("U");
}

bool CubeRubik::colors_existence(vector<char> &edge, char a) {
    for (unsigned int i = 0; i < edge.size(); i++)
        if (edge[i] == a)
            return true;
    return false;
}

void CubeRubik::second_layer_rotate_top() {
    for (int i = 0; i < 3; i++) {
        vector<char> edge = c(-1, 1, 0).getColors();
        bool check = colors_existence(edge, 'y');
        if (check)
            return;
        motion("U");
    }
}

void CubeRubik::second_layer_rotate_top(char a, char b) {
    for (int i = 0; i < 3; i++) {
        vector<char> edge = c(0, 1, 1).getColors();
        bool check = colors_existence(edge, a, b);
        if (check)
            return;
        motion("U");
    }
}

void CubeRubik::second_layer_edge_right() {
    pif_paf();
    rotation("right");
    pif_paf_left();
}

void CubeRubik::second_layer_edge_left() {
    pif_paf_left();
    rotation("left");
    pif_paf();
}

void CubeRubik::second_layer() {
    for (int i = 0; i < 4; i++) {
        vector<char> edge = c(1, 0, 1).getColors();
        bool check = colors_existence(edge, 'y');
        if (!check) {
            char color_front = c(0, 0, 1)["front"];
            char color_right = c(1, 0, 0)["right"];
            Cube edge_cube = c(1, 0, 1);
            if (edge_cube["front"] != color_front
                || edge_cube["right"] != color_right) {
                second_layer_rotate_top();
                second_layer_edge_right();
                continue;
            }
        }
        rotation("right");
    }

    for (int i = 0; i < 4; i++) {
        char color_front = c(0, 0, 1)["front"];
        char color_right = c(1, 0, 0)["right"];
        Cube edge_cube = c(1, 0, 1);
        if (edge_cube["front"] != color_front
            || edge_cube["right"] != color_right) {
            second_layer_rotate_top(color_front, color_right);
            if (c(0, 1, 1)["front"] == color_front) {
                motion("U");
                second_layer_edge_right();
            } else {
                rotation("right");
                double_function("motion", "U'");
                second_layer_edge_left();
                rotation("right");
            }
        } else
            rotation("right");
    }
}

void CubeRubik::pif_paf_f() {
    motion("F");
    pif_paf();
    motion("F'");
}

bool CubeRubik::top_cross() {
    int counter = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 || j == 0)
                if (c(i, 1, j)["top"] == 'y')
                    counter++;
        }
    }

    if (counter == 5)
        return true;

    if (counter == 2 || counter == 4)
        return false;

    if (counter == 1)
        pif_paf_f();

    for (int i = 0; i < 3; i++) {
        if (c(-1, 1, 0)["top"] == 'y') {
            if (c(1, 1, 0)["top"] != 'y') {
                if (c(0, 1, 1)["top"] == 'y')
                    motion("U");
                pif_paf_f();
            }
            pif_paf_f();
            break;
        }
        motion("U");
    }

    return true;
}

void CubeRubik::correct_edges() {
    motion("R");
    motion("U");
    motion("R'");
    motion("U");
    motion("R");
    double_function("motion", "U");
    motion("R'");
}

void CubeRubik::correct_top_cross() {
    for (int i = 0; i < 4; i++) {
        int counter = 0;
        for (int j = 0; j < 4; j++) {
            if (c(0, 1, 1)["front"] == c(0, 0, 1)["front"])
                counter++;
            rotation("right");
        }
        steps.erase(steps.end() - 4, steps.end());
        if (counter == 4)
            return;
        if (counter == 2)
            break;
        motion("U");
    }

    for (int i = 0; i < 3; i++) {
        if (c(0, 1, -1)["back"] == c(0, 0, -1)["back"]) {
            if (c(0, 1, 1)["front"] == c(0, 0, 1)["front"]) {
                correct_edges();
                rotation("left");
            }

            if (c(-1, 1, 0)["left"] == c(-1, 0, 0)["left"])
                motion("U");

            correct_edges();
            break;
        }
        rotation("right");
    }

    for (int i = 0; i < 3; i++) {
        if (c(0, 1, 1)["front"] == c(0, 0, 1)["front"])
            break;
        motion("U");
    }
}

void CubeRubik::correct_cubes_create() {
    motion("R");
    motion("U'");
    motion("L'");
    motion("U");
    motion("R'");
    motion("U'");
    motion("L");
    motion("U");
}

int CubeRubik::top_layer_correct_cubes_count() {
    int counter = 0;
    for (int i = 0; i < 4; i++) {
        vector<char> colors = color_corner(1, 1, 1, 'y');
        char color_front = c(0, 0, 1)["front"];
        char color_right = c(1, 0, 0)["right"];
        if (colors_existence(colors, color_front, color_right))
            counter++;
        rotation("right");
    }
    steps.erase(steps.end() - 4, steps.end());
    return counter;
}

void CubeRubik::top_layer_rotate() {
    for (int i = 0; i < 4; i++) {
        vector<char> color = color_corner(-1, 1, 1, 'y');
        char color_front = c(0, 0, 1)["front"];
        char color_left = c(-1, 0, 0)["left"];
        if (colors_existence(color, color_front, color_left))
            return;
        rotation("right");
    }
}

bool CubeRubik::top_layer_correct_cubes() {
    int correct = top_layer_correct_cubes_count();
    if (correct == 4)
        return true;
    if (correct == 2 || correct == 3)
        return false;
    if (correct == 0) {
        correct_cubes_create();
    }

    while (true) {
        top_layer_rotate();
        correct_cubes_create();
        if (top_layer_correct_cubes_count() == 4)
            break;
    }

    return true;
}

bool CubeRubik::top_layer() {
    if (!top_layer_correct_cubes())
        return false;

    double_function("rotation", "down");
    for (int i = 0; i < 4; i++) {
        char color_front = c(0, -1, 1)["front"];
        char color_right = c(1, -1, 0)["right"];
        while (true) {
            Cube corner = c(1, -1, 1);
            if (corner["front"] == color_front
                && corner["right"] == color_right)
                break;
            pif_paf();
        }
        motion("D");
    }
    double_function("rotation", "down");

    for (int i = 0; i < SIDE_LENGHT; i++) {
        for (int j = 0; j < SIDE_LENGHT; j++) {
            if (c(i - 1, -1, j - 1)["down"] != c(0, -1, 0)["down"])
                return false;
        }
    }
    return true;
}

bool CubeRubik::solve() {
    cross();
    first_layer();
    second_layer();
    if (!top_cross())
        return false;
    correct_top_cross();
    if (!top_layer())
        return false;
    return true;
}