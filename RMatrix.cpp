#include "RMatrix.h"

void RMatrix::printRMatrix()
{
    std::cout << "关系矩阵为：" << std::endl;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
            std::cout << rmatrix[i][j] << "\t";
        std::cout << std::endl;
    }
}

RMatrix::RMatrix(int _n, int _type, int _density)
{
    int i, j, k;
    int odds;
    bool buf;
    n = _n;
    type = _type;
    density = _density;
    // 合法性判断
    if ((type & READ && type != READ) || (type & RANDOM && type != RANDOM) || (type & REFLEXIVE && type & IRREFLEXIVE) || (type & SYMMETRIC && type & ANTISYMMETRIC))
    {
        std::cout << "Invalid RMatrix type!" << std::endl;
        return;
    }
    std::cout << "A x A = {";
    for (i = 1; i <= n; ++i)
        for (j = 1; j <= n; ++j)
        {
            std::cout << "<" << i << "," << j << ">";
            if (i != n || j != n)
                std::cout << ", ";
        }
    std::cout << "}" << std::endl;
    rmatrix.resize(n);
    // 从文件中读
    if (type == READ)
    {
        for (i = 0; i < n; ++i)
        {
            rmatrix[i].resize(n);
            for (j = 0; j < n; ++j)
            {
                if (type == READ)
                {
                    std::cin >> buf;
                    rmatrix[i][j] = buf;
                }
            }
        }
    }
    else // 随机生成
    {
        for (i = 0; i < n; ++i)
        {
            rmatrix[i].resize(n);
            for (j = 0; j < n; ++j)
            {
                // 不等概率产生01值
                odds = rand() % 100;
                if (odds < density)
                    rmatrix[i][j] = 1;
                else
                    rmatrix[i][j] = 0;
            }
        }
    }

    // 产生自反闭包
    if (type & REFLEXIVE)
    {
        for (i = 0; i < n; ++i)
            rmatrix[i][i] = 1;
        is_reflexive = true;
    }
    // 产生对称闭包
    if (type & SYMMETRIC)
    {
        for (i = 0; i < n; ++i)
            for (j = 0; j < n; ++j)
                rmatrix[i][j] = rmatrix[i][j] & rmatrix[j][i];
        is_symmetric = true;
    }
    // 产生传递闭包
    if (type & TRANSITIVE)
    {
        // warshall算法
        for (k = 0; k < n; ++k)
            for (i = 0; i < n; ++i)
                for (j = 0; j < n; ++j)
                    if (!rmatrix[i][j])
                        rmatrix[i][j] = rmatrix[i][k] & rmatrix[k][j];
        is_transitive = true;
    }
    // 产生反自反关系矩阵
    if (type & IRREFLEXIVE)
    {
        for (i = 0; i < n; ++i)
            rmatrix[i][i] = 0;
        is_reflexive = false;
        is_irreflexive = true;
    }
    // 产生反对称关系矩阵
    if (type & ANTISYMMETRIC)
    {
        for (i = 0; i < n; ++i)
            for (j = 0; j < i; ++j)
                if (rmatrix[i][j] & rmatrix[j][i])
                    rmatrix[i][j] = 0;
        is_symmetric = false;
        is_antisymmetric = true;
    }
    // 如果是文件中读取，则还需解析，判断是否有相应的性质
    if (type == READ)
        parseRMatrix();
    // 如果同时具有自反性、对称性、传递性是等价关系
    if (is_reflexive && is_symmetric && is_transitive)
    {
        is_eq_rel = true;
        divideRMatrix();
    }
    // 如果同时具有自反性、对称性则是相容关系
    if (is_reflexive && is_symmetric)
    {
        is_comp_rel = true;
        coverageRMatrix();
    }
    // 如果同时具有自反性、反对称性、传递性是偏序关系
    if (is_reflexive && is_antisymmetric && is_transitive)
    {
        is_part_rel = true;
        ParsePartialOrder();
    }
}

RMatrix::RMatrix(int _n) : RMatrix(_n, RANDOM)
{
}

void RMatrix::parseRMatrix()
{
    is_reflexive = true;
    is_symmetric = true;
    is_transitive = true;
    is_irreflexive = true;
    is_antisymmetric = true;
    int i, j, k;
    bool temp[n][n];
    memset(temp, 0, sizeof(bool) * n * n);
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            // 如果对角线存在0，则不满足自反性；如果对角线存在1，则不满足反自反性
            if (i == j)
            {
                if (rmatrix[i][j])
                    is_irreflexive = false;
                else
                    is_reflexive = false;
            }
            // 如果非对角线的对称位置都为1则不满足反对称性
            else if (rmatrix[i][j] & rmatrix[j][i])
                is_antisymmetric = false;
            // 如果非对角线的对称位置的值不相等则不满足对称性
            else if (rmatrix[i][j] ^ rmatrix[j][i])
                is_symmetric = false;
            // 如果有传递性则同一位置，原矩阵为0，求幂后矩阵值一定为0
            for (k = 0; is_transitive && k < n; ++k)
                temp[i][j] |= rmatrix[i][k] & rmatrix[k][j];
            if (is_transitive && (temp[i][j] & !rmatrix[i][j]))
                is_transitive = false;
        }
    }
}

void RMatrix::divideRMatrix()
{
    int i, j;
    bool is_newclass;           // 当前是否属于新的等价类
    std::set<int> iffclass;     // 当前等价类
    std::set<int> intersection; // 交集
    for (i = 0; i < n; ++i)
    {
        is_newclass = true;
        iffclass.clear();
        for (j = i; j < n; ++j)
            if (rmatrix[i][j])
                iffclass.insert(j + 1);
        for (auto &iter_class : division)
        {
            intersection.clear();
            std::set_intersection(std::begin(iter_class), std::end(iter_class), std::begin(iffclass), std::end(iffclass), std::inserter(intersection, std::begin(intersection)));
            if (!intersection.empty())
            {
                iter_class.merge(iffclass);
                is_newclass = false;
                break;
            }
        }
        if (is_newclass)
            division.push_back(iffclass);
    }
}

void RMatrix::coverageRMatrix()
{
    int i, j;
    std::vector<int> curpoints, points, selectedpoints; // 分别是当且子图的顶点集合，所有顶点的集合，所有待选顶点的集合
    for (i = 1; i <= n; ++i)
    {
        points.push_back(i);
        selectedpoints.push_back(i);
    }
    for (i = 0; i < n; ++i)
    {
        curpoints.clear();
        // 值为0说明该顶点已经存在于先前某子图中
        if (selectedpoints[i] == 0)
            continue;
        curpoints.push_back(points[i]);
        selectedpoints[i] = 0;
        // 在points中找所有与curpoints中的各顶点都相连的点并从中取出放入curpoints中
        for (j = 0; j < n; ++j)
        {
            bool is_connected = true;
            for (auto &point : curpoints)
                if (!rmatrix[points[j] - 1][point - 1])
                {
                    is_connected = false;
                    break;
                }
            if (is_connected && i != j)
            {
                curpoints.push_back(points[j]);
                selectedpoints[j] = 0;
            }
        }
        // 将curpoints放入comp_coverage中
        comp_coverage.push_back(curpoints);
    }
}

void RMatrix::ParsePartialOrder()
{
    int i, j, sum;
    // 寻找极小元
    for (i = 0; i < n; ++i)
    {
        sum = 0;
        for (j = 0; j < n; ++j)
        {
            if((sum += rmatrix[j][i]) > 1)
                break;
        }
        if (sum == 1)
            min_elem.insert(i+1);
    }
    // 寻找极大元
    for (i = 0; i < n; ++i)
    {
        sum = 0;
        for (j = 0; j < n; ++j)
        {
            if((sum += rmatrix[i][j]) > 1)
                break;
        }
        if (sum == 1)
            max_elem.insert(i+1);
    }
}

void RMatrix::printAttrs()
{
    if (!is_reflexive && !is_symmetric && !is_transitive && !is_irreflexive && !is_antisymmetric)
        std::cout << "关系不含常见属性" << std::endl;
    if (is_reflexive)
        std::cout << "关系具有自反性" << std::endl;
    if (is_symmetric)
        std::cout << "关系具有对称性" << std::endl;
    if (is_transitive)
        std::cout << "关系具有传递性" << std::endl;
    if (is_irreflexive)
        std::cout << "关系具有反自反性" << std::endl;
    if (is_antisymmetric)
        std::cout << "关系具有反对称性" << std::endl;
    if (is_eq_rel)
    {
        std::cout << "关系为等价关系，划分为：" << std::endl
                  << "{";
        int i = 1, division_size = division.size();
        for (auto it_division : division)
        {
            int j = 1, it_division_size = it_division.size();
            std::cout << "{";
            for (auto it_elem : it_division)
            {
                std::cout << it_elem;
                if (j++ != it_division_size)
                    std::cout << ",";
            };
            std::cout << "}";
            if (i++ != division_size)
                std::cout << ",";
        }
        std::cout << "}" << std::endl;
    }
    if (is_comp_rel)
    {
        std::cout << "关系为相容关系，完全覆盖为：" << std::endl
                  << "{";
        int i = 1, comp_coverage_size = comp_coverage.size();
        for (auto it_coverage : comp_coverage)
        {
            int j = 1, it_coverage_size = it_coverage.size();
            std::cout << "{";
            for (auto it_elem : it_coverage)
            {
                std::cout << it_elem;
                if (j++ != it_coverage_size)
                    std::cout << ",";
            }
            std::cout << "}";
            if (i++ != comp_coverage_size)
                std::cout << ",";
        }
        std::cout << "}" << std::endl;
    }
    if (is_part_rel)
    {
        std::cout << "关系为偏序关系" << std::endl;
        std::cout << "极小元为：" << std::endl;
        int i = 1, min_elem_size = min_elem.size(), max_elem_size = max_elem.size();
        for (auto it_elem : min_elem)
        {
            std::cout << it_elem;
            if (i++ != min_elem_size)
                std::cout << ",";
        }
        std::cout << std::endl;
        std::cout << "极大元为：" << std::endl;
        i = 1;
        for (auto it_elem : max_elem)
        {
            std::cout << it_elem;
            if (i++ != max_elem_size)
                std::cout << ",";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}