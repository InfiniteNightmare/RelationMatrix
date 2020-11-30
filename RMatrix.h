#include <algorithm>
#include <cstring>
#include <iostream>
#include <set>
#include <vector>

#define READ 0           // ���ļ��ж�
#define RANDOM 1         // ���
#define REFLEXIVE 2      // �Է���
#define SYMMETRIC 4      // �Գ���
#define TRANSITIVE 8      // ������
#define IRREFLEXIVE 16   // ���Է���
#define ANTISYMMETRIC 32 // ���Գ���

class RMatrix
{
public:
    void printRMatrix();                           // �����ϵ����
    void printAttrs();                             // �����ϵ����ĸ�����
    RMatrix(int _n, int _type, int _density = 50); // ���캯������������ָ�����ɹ�ϵ����
    RMatrix(int _n);                               // Ĭ�Ϲ��캯�����޲����������һ����ϵ����
private:
    int n;                                                // �������
    int type;                                             // ��ϵ����
    int density;                                          // ��ʼ��ϵ������1���ܶ�,ȡֵΪ[0, 100]
    bool is_reflexive = false;                            // �Ƿ�����Է���
    bool is_symmetric = false;                            // �Ƿ���жԳ���
    bool is_transitive = false;                            // �Ƿ���д�����
    bool is_irreflexive = false;                          // �Ƿ���з��Է���
    bool is_antisymmetric = false;                        // �Ƿ���з��Գ���
    bool is_eq_rel = false;                               // �Ƿ��ǵȼ۹�ϵ
    bool is_comp_rel = false;                             // �Ƿ������ݹ�ϵ
    bool is_part_rel = false;                             // �Ƿ���ƫ���ϵ
    std::vector<std::vector<bool>> rmatrix;               // ��ϵ����
    std::vector<std::set<int>> division;                  // ��ϵ����ĵȼ���/����
    std::vector<std::vector<int>> comp_coverage;          // ��ϵ��������������/��ȫ����
    std::set<int> min_elem;                               // ��СԪ
    std::set<int> max_elem;                               // ����Ԫ
    void parseRMatrix();                                  // ������ϵ����
    void divideRMatrix();                                 // ���ϵ���󻮷�
    void coverageRMatrix();                               // ���ϵ������ȫ����
    void ParsePartialOrder();                             // ����ƫ���ϵ
};