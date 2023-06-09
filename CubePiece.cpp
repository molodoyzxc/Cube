
#include "CubePiece.h"

Cube::Cube() : sides{'y', 'w', 'b', 'g', 'o', 'r'} {}

char &Cube::operator[](const int i) {
    if (i >= 0 && i < SIDES_COUNT)
        return sides[i];
    throw out_of_range("Indexation of cube out of range");
}

int Cube::sideToInt(const string s) const {
    const unordered_map<string, int> sideMap{
            {"top",   0},
            {"down",  1},
            {"front", 2},
            {"back",  3},
            {"left",  4},
            {"right", 5}
    };

    if (sideMap.count(s))
        return sideMap.at(s);

    throw out_of_range("Indexation of cube out of range");
}

char &Cube::operator[](const string s) {
    return sides[sideToInt(s)];
}

string Cube::inversion(string s) const {
    const unordered_map<string, string> inverseMap{
            {"up",       "down"},
            {"down",     "up"},
            {"left",     "right"},
            {"right",    "left"},
            {"up_dop",   "down_dop"},
            {"down_dop", "up_dop"}
    };

    if (inverseMap.count(s))
        return inverseMap.at(s);

    throw runtime_error("No such rotation");
}

void Cube::rotation(string s, bool inv) {
    if (inv)
        s = inversion(s);

    const unordered_map<string, vector<int>> rotationOrderMap{
            {"up",       {0, 2, 1, 3}},
            {"down",     {0, 3, 1, 2}},
            {"left",     {2, 5, 3, 4}},
            {"right",    {2, 4, 3, 5}},
            {"up_dop",   {0, 5, 1, 4}},
            {"down_dop", {0, 4, 1, 5}}
    };

    if (rotationOrderMap.count(s)) {
        const vector<int> &rotation_order = rotationOrderMap.at(s);

        char c = sides[rotation_order[0]];
        for (int i = 1; i < ROTATIONS_COUNT; i++) {
            sides[rotation_order[i - 1]] = sides[rotation_order[i]];
        }
        sides[rotation_order[ROTATIONS_COUNT - 1]] = c;
    } else {
        throw runtime_error("No such rotation");
    }
}

vector<char> Cube::getColors() {
    vector<char> colors;
    for (char side: sides) {
        if (side != 'd')
            colors.push_back(side);
    }
    return colors;
}

Info::Info() {
    cubes.resize(SIDE_LENGHT, vector<vector<Cube>>(SIDE_LENGHT, vector<Cube>(SIDE_LENGHT)));

    for (int i = 0; i < SIDE_LENGHT; i++) {
        for (int j = 0; j < SIDE_LENGHT; j++) {
            for (int k = 0; k < SIDE_LENGHT; k++) {
                const bool checks[6] = {j < 2, j > 0, k < 2, k > 0, i > 0, i < 2};

                for (int t = 0; t < SIDES_COUNT; t++) {
                    if (checks[t])
                        cubes[i][j][k][t] = 'd';
                }
            }
        }
    }
}

Cube &Info::operator()(int i, int j, int k) {
    if (i >= -1 && i <= 1 && j >= -1 && j <= 1 && k >= -1 && k <= 1)
        return cubes[i + 1][j + 1][k + 1];

    throw out_of_range("Indexation out of range");
}

Cube &Info::operator()(int constant, int j, int k, char axis) {
    if (constant >= -1 && constant <= 1 && j >= -1 && j <= 1 && k >= -1 && k <= 1) {
        switch (axis) {
            case 'x':
                return cubes[constant + 1][j + 1][k + 1];
            case 'y':
                return cubes[j + 1][constant + 1][k + 1];
            case 'z':
                return cubes[j + 1][k + 1][constant + 1];
        }
    }

    throw out_of_range("Indexation out of range");
}