#pragma once

#include <bits/stdc++.h>

#define SIDE_LENGHT 3
#define ROTATIONS_COUNT 4
#define SIDES_COUNT 6
using namespace std;

class Cube {
private:
    vector<char> sides;

    string inversion(string) const;

public:
    Cube();

    char &operator[](const int);

    int sideToInt(const string) const;

    char &operator[](const string);

    void rotation(string, bool);

    vector<char> getColors();
};

class Info {
private:
    vector<vector<vector<Cube>>> cubes;
public:
    Info();

    Cube &operator()(int, int, int);

    Cube &operator()(int, int, int, char);
};