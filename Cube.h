#pragma once

#include "CubePiece.h"
#include <bits/stdc++.h>

using namespace std;

class Rotate_anim {
public:
    char axis;
    int value_axis;
    bool inv;
    string command;

    Rotate_anim() { axis = '0'; };

    Rotate_anim(char a, int v, bool i, string c) {
        axis = a;
        value_axis = v;
        inv = i;
        command = c;
    }
};

extern vector<Rotate_anim> steps;

class CubeRubik {
private:
    Info c;

    string colors_to_string(vector<char> &);

    bool color_opposite(char, char);

    void double_function(string, string);

    void cross_yellow_up();

    void cross_rotate_top(int, int);

    void cross_rotate_top();

    void cross_whites_up(int &);

    void cross();

    void pif_paf();

    vector<char> color_corner(int, int, int, char);

    bool colors_existence(vector<char> &, char, char);

    void first_layer_rotate_top();

    void first_later_rotate_top(char, char);

    void first_layer();

    void pif_paf_left();

    bool colors_existence(vector<char> &, char);

    void second_layer_rotate_top();

    void second_layer_rotate_top(char, char);

    void second_layer_edge_right();

    void second_layer_edge_left();

    void second_layer();

    void pif_paf_f();

    bool top_cross();

    void correct_edges();

    void correct_top_cross();

    void correct_cubes_create();

    int top_layer_correct_cubes_count();

    void top_layer_rotate();

    bool top_layer_correct_cubes();

    bool top_layer();

public:
    CubeRubik() { c = Info(); }

    Cube &operator()(int, int, int);

    void load(string);

    void save(string);

    void print();

    void rotation_helper(char, int, bool);

    void rotation(string, bool);

    void rotation(string);

    void motion(string, bool);

    void motion(string);

    bool check_correct();

    void generate_valid_cube();

    bool solve();
};
