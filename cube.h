#ifndef CUBE_H
#define CUBE_H

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

struct Vec3 {
    int x;
    int y;
    int z;

    bool operator==(const Vec3& other) const;
};

struct StickerKey {
    Vec3 pos;
    Vec3 normal;

    bool operator==(const StickerKey& other) const;
};

struct StickerKeyHash {
    std::size_t operator()(const StickerKey& key) const;
};

class Cube {
public:
    Cube();

    void reset();
    bool isSolved() const;

    void move(const std::string& token);
    void applyAlgorithm(const std::vector<std::string>& moves);

    std::vector<std::string> scramble(int length = 20, unsigned int seed = 42);

    std::array<std::array<char, 9>, 6> asFaces() const;
    std::string prettyString() const;

private:
    // Stickers are stored by 3D position + outward normal.
    std::unordered_map<StickerKey, char, StickerKeyHash> stickers;

    void quarterTurn(char moveFace);
};

std::string inverseMove(const std::string& move);
std::vector<std::string> inverseAlgorithm(const std::vector<std::string>& moves);

#endif