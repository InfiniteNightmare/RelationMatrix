#include "RMatrix.h"

void RMatrix::printRMatrix()
{
    std::cout << "��ϵ����Ϊ��" << std::endl;
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
    // �Ϸ����ж�
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
    // ���ļ��ж�
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
    else // �������
    {
        for (i = 0; i < n; ++i)
        {
            rmatrix[i].resize(n);
            for (j = 0; j < n; ++j)
            {
                // ���ȸ��ʲ���01ֵ
                odds = rand() % 100;
                if (odds < density)
                    rmatrix[i][j] = 1;
                else
                    rmatrix[i][j] = 0;
            }
        }
    }

    // �����Է��հ�
    if (type & REFLEXIVE)
    {
        for (i = 0; i < n; ++i)
            rmatrix[i][i] = 1;
        is_reflexive = true;
    }
    // �����ԳƱհ�
    if (type & SYMMETRIC)
    {
        for (i = 0; i < n; ++i)
            for (j = 0; j < n; ++j)
                rmatrix[i][j] = rmatrix[i][j] & rmatrix[j][i];
        is_symmetric = true;
    }
    // �������ݱհ�
    if (type & TRANSITIVE)
    {
        // warshall�㷨
        for (k = 0; k < n; ++k)
            for (i = 0; i < n; ++i)
                for (j = 0; j < n; ++j)
                    if (!rmatrix[i][j])
                        rmatrix[i][j] = rmatrix[i][k] & rmatrix[k][j];
        is_transitive = true;
    }
    // �������Է���ϵ����
    if (type & IRREFLEXIVE)
    {
        for (i = 0; i < n; ++i)
            rmatrix[i][i] = 0;
        is_reflexive = false;
        is_irreflexive = true;
    }
    // �������Գƹ�ϵ����
    if (type & ANTISYMMETRIC)
    {
        for (i = 0; i < n; ++i)
            for (j = 0; j < i; ++j)
                if (rmatrix[i][j] & rmatrix[j][i])
                    rmatrix[i][j] = 0;
        is_symmetric = false;
        is_antisymmetric = true;
    }
    // ������ļ��ж�ȡ������������ж��Ƿ�����Ӧ������
    if (type == READ)
        parseRMatrix();
    // ���ͬʱ�����Է��ԡ��Գ��ԡ��������ǵȼ۹�ϵ
    if (is_reflexive && is_symmetric && is_transitive)
    {
        is_eq_rel = true;
        divideRMatrix();
    }
    // ���ͬʱ�����Է��ԡ��Գ����������ݹ�ϵ
    if (is_reflexive && is_symmetric)
    {
        is_comp_rel = true;
        coverageRMatrix();
    }
    // ���ͬʱ�����Է��ԡ����Գ��ԡ���������ƫ���ϵ
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
            // ����Խ��ߴ���0���������Է��ԣ�����Խ��ߴ���1�������㷴�Է���
            if (i == j)
            {
                if (rmatrix[i][j])
                    is_irreflexive = false;
                else
                    is_reflexive = false;
            }
            // ����ǶԽ��ߵĶԳ�λ�ö�Ϊ1�����㷴�Գ���
            else if (rmatrix[i][j] & rmatrix[j][i])
                is_antisymmetric = false;
            // ����ǶԽ��ߵĶԳ�λ�õ�ֵ�����������Գ���
            else if (rmatrix[i][j] ^ rmatrix[j][i])
                is_symmetric = false;
            // ����д�������ͬһλ�ã�ԭ����Ϊ0�����ݺ����ֵһ��Ϊ0
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
    bool is_newclass;           // ��ǰ�Ƿ������µĵȼ���
    std::set<int> iffclass;     // ��ǰ�ȼ���
    std::set<int> intersection; // ����
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
    std::vector<int> curpoints, points, selectedpoints; // �ֱ��ǵ�����ͼ�Ķ��㼯�ϣ����ж���ļ��ϣ����д�ѡ����ļ���
    for (i = 1; i <= n; ++i)
    {
        points.push_back(i);
        selectedpoints.push_back(i);
    }
    for (i = 0; i < n; ++i)
    {
        curpoints.clear();
        // ֵΪ0˵���ö����Ѿ���������ǰĳ��ͼ��
        if (selectedpoints[i] == 0)
            continue;
        curpoints.push_back(points[i]);
        selectedpoints[i] = 0;
        // ��points����������curpoints�еĸ����㶼�����ĵ㲢����ȡ������curpoints��
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
        // ��curpoints����comp_coverage��
        comp_coverage.push_back(curpoints);
    }
}

void RMatrix::ParsePartialOrder()
{
    int i, j, sum;
    // Ѱ�Ҽ�СԪ
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
    // Ѱ�Ҽ���Ԫ
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
        std::cout << "��ϵ������������" << std::endl;
    if (is_reflexive)
        std::cout << "��ϵ�����Է���" << std::endl;
    if (is_symmetric)
        std::cout << "��ϵ���жԳ���" << std::endl;
    if (is_transitive)
        std::cout << "��ϵ���д�����" << std::endl;
    if (is_irreflexive)
        std::cout << "��ϵ���з��Է���" << std::endl;
    if (is_antisymmetric)
        std::cout << "��ϵ���з��Գ���" << std::endl;
    if (is_eq_rel)
    {
        std::cout << "��ϵΪ�ȼ۹�ϵ������Ϊ��" << std::endl
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
        std::cout << "��ϵΪ���ݹ�ϵ����ȫ����Ϊ��" << std::endl
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
        std::cout << "��ϵΪƫ���ϵ" << std::endl;
        std::cout << "��СԪΪ��" << std::endl;
        int i = 1, min_elem_size = min_elem.size(), max_elem_size = max_elem.size();
        for (auto it_elem : min_elem)
        {
            std::cout << it_elem;
            if (i++ != min_elem_size)
                std::cout << ",";
        }
        std::cout << std::endl;
        std::cout << "����ԪΪ��" << std::endl;
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