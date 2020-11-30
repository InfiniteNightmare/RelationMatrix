#include <algorithm>
#include <cstring>
#include <iostream>
#include <set>
#include <vector>

#define READ 0           // 从文件中读
#define RANDOM 1         // 随机
#define REFLEXIVE 2      // 自反性
#define SYMMETRIC 4      // 对称性
#define TRANSITIVE 8      // 传递性
#define IRREFLEXIVE 16   // 反自反性
#define ANTISYMMETRIC 32 // 反对称性

class RMatrix
{
public:
    void printRMatrix();                           // 输出关系矩阵
    void printAttrs();                             // 输出关系矩阵的各属性
    RMatrix(int _n, int _type, int _density = 50); // 构造函数，按照输入指令生成关系矩阵
    RMatrix(int _n);                               // 默认构造函数，无参则随机生成一个关系矩阵
private:
    int n;                                                // 矩阵阶数
    int type;                                             // 关系种类
    int density;                                          // 初始关系矩阵中1的密度,取值为[0, 100]
    bool is_reflexive = false;                            // 是否具有自反性
    bool is_symmetric = false;                            // 是否具有对称性
    bool is_transitive = false;                            // 是否具有传递性
    bool is_irreflexive = false;                          // 是否具有反自反性
    bool is_antisymmetric = false;                        // 是否具有反对称性
    bool is_eq_rel = false;                               // 是否是等价关系
    bool is_comp_rel = false;                             // 是否是相容关系
    bool is_part_rel = false;                             // 是否是偏序关系
    std::vector<std::vector<bool>> rmatrix;               // 关系矩阵
    std::vector<std::set<int>> division;                  // 关系矩阵的等价类/划分
    std::vector<std::vector<int>> comp_coverage;          // 关系矩阵的最大相容类/完全覆盖
    std::set<int> min_elem;                               // 极小元
    std::set<int> max_elem;                               // 极大元
    void parseRMatrix();                                  // 解析关系矩阵
    void divideRMatrix();                                 // 求关系矩阵划分
    void coverageRMatrix();                               // 求关系矩阵完全覆盖
    void ParsePartialOrder();                             // 解析偏序关系
};