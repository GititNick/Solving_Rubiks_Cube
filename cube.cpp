#include "cube.h"

#include <algorithm>
#include <random>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

bool Vec3::operator==(const Vec3& other) const {
    return x == other.x && y == other.y && z == other.z;
}

bool StickerKey::operator==(const StickerKey& other) const {
    return pos == other.pos && normal == other.normal;
}

std::size_t StickerKeyHash::operator()(const StickerKey& key) const {
    auto h = [](int v) -> std::size_t {
        return std::hash<int>{}(v);
    };

    std::size_t seed = 0;
    seed ^= h(key.pos.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= h(key.pos.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= h(key.pos.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= h(key.normal.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= h(key.normal.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= h(key.normal.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
}

namespace {

const std::array<char, 6> FACE_ORDER = {'U', 'R', 'F', 'D', 'L', 'B'};

struct MoveSpec {
    char axis;
    int layerValue;
    int direction;
};

const std::unordered_map<char, MoveSpec> MOVE_SPECS = {
    {'U', {'y',  1,  1}},
    {'D', {'y', -1, -1}},
    {'R', {'x',  1,  1}},
    {'L', {'x', -1, -1}},
    {'F', {'z',  1, -1}},
    {'B', {'z', -1,  1}}
};

struct FaceletRef {
    int face;
    int idx;
};

struct Maps {
    std::array<StickerKey, 54> faceletToKey;
    std::unordered_map<StickerKey, FaceletRef, StickerKeyHash> keyToFacelet;
};

Vec3 rotateCoord(const Vec3& c, char axis, int direction) {
    if (axis == 'x') {
        if (direction == 1) return {c.x, -c.z, c.y};
        return {c.x, c.z, -c.y};
    }

    if (axis == 'y') {
        if (direction == 1) return {c.z, c.y, -c.x};
        return {-c.z, c.y, c.x};
    }

    if (axis == 'z') {
        if (direction == 1) return {-c.y, c.x, c.z};
        return {c.y, -c.x, c.z};
    }

    throw std::invalid_argument("Invalid rotation axis");
}

bool inLayer(const Vec3& pos, char axis, int layerValue) {
    if (axis == 'x') return pos.x == layerValue;
    if (axis == 'y') return pos.y == layerValue;
    if (axis == 'z') return pos.z == layerValue;
    return false;
}

const Maps& getMaps() {
    static const Maps maps = []() {
        Maps m;
        int flatIndex = 0;

        for (int f = 0; f < static_cast<int>(FACE_ORDER.size()); ++f) {
            char face = FACE_ORDER[f];

            for (int idx = 0; idx < 9; ++idx) {
                int row = idx / 3;
                int col = idx % 3;

                Vec3 pos{};
                Vec3 normal{};

                if (face == 'U') {
                    pos = {-1 + col, 1, -1 + row};
                    normal = {0, 1, 0};
                } else if (face == 'R') {
                    pos = {1, 1 - row, 1 - col};
                    normal = {1, 0, 0};
                } else if (face == 'F') {
                    pos = {-1 + col, 1 - row, 1};
                    normal = {0, 0, 1};
                } else if (face == 'D') {
                    pos = {-1 + col, -1, 1 - row};
                    normal = {0, -1, 0};
                } else if (face == 'L') {
                    pos = {-1, 1 - row, -1 + col};
                    normal = {-1, 0, 0};
                } else if (face == 'B') {
                    pos = {1 - col, 1 - row, -1};
                    normal = {0, 0, -1};
                } else {
                    throw std::runtime_error("Unknown face");
                }

                StickerKey key{pos, normal};
                m.faceletToKey[flatIndex++] = key;
                m.keyToFacelet[key] = {f, idx};
            }
        }

        return m;
    }();

    return maps;
}

std::string rowString(const std::array<char, 9>& face, int r) {
    int start = r * 3;
    std::string s;
    s += face[start];
    s += ' ';
    s += face[start + 1];
    s += ' ';
    s += face[start + 2];
    return s;
}

} // namespace

Cube::Cube() {
    reset();
}

void Cube::reset() {
    stickers.clear();

    const Maps& maps = getMaps();
    int flatIndex = 0;

    for (char face : FACE_ORDER) {
        for (int idx = 0; idx < 9; ++idx) {
            const StickerKey& key = maps.faceletToKey[flatIndex++];
            stickers[key] = face;
        }
    }
}

bool Cube::isSolved() const {
    auto faces = asFaces();

    for (int f = 0; f < 6; ++f) {
        char first = faces[f][0];
        for (int i = 1; i < 9; ++i) {
            if (faces[f][i] != first) {
                return false;
            }
        }
    }

    return true;
}

std::array<std::array<char, 9>, 6> Cube::asFaces() const {
    std::array<std::array<char, 9>, 6> faces;
    for (auto& face : faces) {
        face.fill('?');
    }

    const Maps& maps = getMaps();

    for (const auto& entry : stickers) {
        auto it = maps.keyToFacelet.find(entry.first);
        if (it == maps.keyToFacelet.end()) {
            throw std::runtime_error("Sticker mapping error");
        }

        faces[it->second.face][it->second.idx] = entry.second;
    }

    return faces;
}

void Cube::quarterTurn(char moveFace) {
    auto it = MOVE_SPECS.find(moveFace);
    if (it == MOVE_SPECS.end()) {
        throw std::invalid_argument("Unsupported move face");
    }

    char axis = it->second.axis;
    int layerValue = it->second.layerValue;
    int direction = it->second.direction;

    std::unordered_map<StickerKey, char, StickerKeyHash> newStickers;
    newStickers.reserve(stickers.size());

    for (const auto& entry : stickers) {
        const StickerKey& key = entry.first;
        char color = entry.second;

        if (inLayer(key.pos, axis, layerValue)) {
            StickerKey rotated;
            rotated.pos = rotateCoord(key.pos, axis, direction);
            rotated.normal = rotateCoord(key.normal, axis, direction);
            newStickers[rotated] = color;
        } else {
            newStickers[key] = color;
        }
    }

    stickers = std::move(newStickers);
}

void Cube::move(const std::string& token) {
    if (token.empty()) {
        return;
    }

    char face = token[0];
    if (MOVE_SPECS.find(face) == MOVE_SPECS.end()) {
        throw std::invalid_argument("Invalid move: " + token);
    }

    int turns = 1;

    if (token.size() == 2) {
        if (token[1] == '\'') {
            turns = 3;
        } else if (token[1] == '2') {
            turns = 2;
        } else {
            throw std::invalid_argument("Invalid move suffix: " + token);
        }
    } else if (token.size() > 2) {
        throw std::invalid_argument("Invalid move token: " + token);
    }

    for (int i = 0; i < turns; ++i) {
        quarterTurn(face);
    }
}

void Cube::applyAlgorithm(const std::vector<std::string>& moves) {
    for (const auto& m : moves) {
        move(m);
    }
}

std::vector<std::string> Cube::scramble(int length, unsigned int seed) {
    std::mt19937 rng(seed);

    const std::array<char, 6> basicFaces = {'U', 'D', 'L', 'R', 'F', 'B'};
    const std::array<std::string, 3> suffixes = {"", "'", "2"};

    std::uniform_int_distribution<int> faceDist(0, 5);
    std::uniform_int_distribution<int> suffixDist(0, 2);

    std::vector<std::string> moves;
    moves.reserve(length);

    char prevFace = '\0';

    while (static_cast<int>(moves.size()) < length) {
        char face = basicFaces[faceDist(rng)];
        if (face == prevFace) {
            continue;
        }

        std::string move(1, face);
        move += suffixes[suffixDist(rng)];
        moves.push_back(move);
        prevFace = face;
    }

    applyAlgorithm(moves);
    return moves;
}

std::string Cube::prettyString() const {
    auto faces = asFaces();
    std::ostringstream out;

    for (int r = 0; r < 3; ++r) {
        out << "      " << rowString(faces[0], r) << '\n';
    }

    for (int r = 0; r < 3; ++r) {
        out << rowString(faces[4], r) << "   "
            << rowString(faces[2], r) << "   "
            << rowString(faces[1], r) << "   "
            << rowString(faces[5], r) << '\n';
    }

    for (int r = 0; r < 3; ++r) {
        out << "      " << rowString(faces[3], r) << '\n';
    }

    return out.str();
}

std::string inverseMove(const std::string& move) {
    if (move.empty()) {
        return move;
    }

    if (move.size() == 2 && move[1] == '\'') {
        return std::string(1, move[0]);
    }

    if (move.size() == 2 && move[1] == '2') {
        return move;
    }

    return move + "'";
}

std::vector<std::string> inverseAlgorithm(const std::vector<std::string>& moves) {
    std::vector<std::string> result;
    result.reserve(moves.size());

    for (auto it = moves.rbegin(); it != moves.rend(); ++it) {
        result.push_back(inverseMove(*it));
    }

    return result;
}