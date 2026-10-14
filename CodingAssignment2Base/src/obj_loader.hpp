#pragma once

#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// 面上的一个角：位置编号 + 法线编号（已转成从 0 开始；没有法线时为 -1）
struct FaceCorner {
    int v = -1;
    int n = -1;
};

// 解析一个角，支持 "5//1"、"5/2/1"、"5/2"、"5" 四种写法
static FaceCorner parse_corner(const std::string& token) {
    FaceCorner c;
    const size_t slash1 = token.find('/');
    c.v = std::stoi(token.substr(0, slash1)) - 1;          // OBJ 编号从 1 开始，减 1
    if (slash1 != std::string::npos) {
        const size_t slash2 = token.find('/', slash1 + 1);
        if (slash2 != std::string::npos && slash2 + 1 < token.size()) {
            c.n = std::stoi(token.substr(slash2 + 1)) - 1;
        }
    }
    return c;
}

// 读取 OBJ 文件，输出交错排列的顶点数组：每个顶点 6 个 float [px py pz nx ny nz]
static std::vector<float> load_obj(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Could not open " + path);
    }

    std::vector<glm::vec3> positions;  // 所有 v 行
    std::vector<glm::vec3> normals;    // 所有 vn 行
    std::vector<float> vertices;       // 最终输出

    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string tag;
        ss >> tag;                     // 每行的第一个词决定这一行的类型

        if (tag == "v") {
            glm::vec3 p;
            ss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        } else if (tag == "vn") {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        } else if (tag == "f") {
            std::vector<FaceCorner> corners;
            std::string token;
            while (ss >> token) {
                corners.push_back(parse_corner(token));
            }

            // 扇形三角化：(0,1,2), (0,2,3), ...
            for (size_t i = 1; i + 1 < corners.size(); ++i) {
                const FaceCorner tri[3] = {corners[0], corners[i], corners[i + 1]};

                // 备用法线：文件里没有 vn 时，用两条边的叉积算面法线
                const glm::vec3 a = positions[tri[0].v];
                const glm::vec3 b = positions[tri[1].v];
                const glm::vec3 c = positions[tri[2].v];
                const glm::vec3 faceNormal = glm::normalize(glm::cross(b - a, c - a));

                // 展开：每个角都写出一份完整的 [位置 + 法线]
                for (const FaceCorner& k : tri) {
                    const glm::vec3 p = positions[k.v];
                    const glm::vec3 n = (k.n >= 0) ? normals[k.n] : faceNormal;
                    vertices.insert(vertices.end(), {p.x, p.y, p.z, n.x, n.y, n.z});
                }
            }
        }
        // 其他行（#、o、s 等）全部忽略
    }
    return vertices;
}