# 关系矩阵类RMatrix文档

## 前言

在学习完集合论后，老师针对集合论的部分知识（主要是关系）布置了一个程序。在仔细阅读课本和查阅大量资料的积累下，我花了一个周末写完了程序。我主要写了一个关系矩阵类，将题目要求的功能抽象成一个关系矩阵类的方法，这样使这个类具有一定的通用性和扩展性。对于具体关系矩阵的生成，我设定了两种模式，分别是从文件中读取和按要求随机生成，具体的实现细节可以看正文或源代码，这样方便了测试，也提高了实用性。经过测试，本程序能通过题目所有要求和我自己额外的要求，但测试不可能兼顾所有情况，而且部分算法由于个人能力有限，只针对题目要求的数据集，对于超出要求之外的一些复杂情况没有在数学角度充分考虑并加以证明，只是经过几次测试，可能存在缺陷，如有错误请不吝指教。

## 题目要求

### 输入

一个正整数n

R是A的二元关系  (A = {1,2,...,n})

### 输出

1.A×A

2.R的性质

3.R是否为等价关系、相容关系、偏序关系

4.如果R是等价关系，输出划分；是相容关系，输出完全覆盖；是偏序关系，输出极大元、极小元

## 基础知识

#### 自反性

$(\forall x)(x \in X \to x R x) \\ r(R) = R$

#### 对称性

$(\forall x)(\forall y)(x \to X \wedge y \to X \wedge x R y \wedge y R x) \\ R = R^0 \\ s(R) = R$

#### 传递性

$(\forall x)(\forall y)(\forall z)(x \in X \wedge y \in X \wedge z \in X \ x R y \wedge y R z \to x R z) \\ t(R) = R \\ R \circ R \subseteq R$

#### 反自反性

$(\forall x)(x \in X \to <x,x> \notin R)$

#### 反对称性

$(\forall x)(\forall y)(x \in X \wedge y \in X \wedge x R y \wedge y R x \to x = y) \\ R \cap R^0 \subseteq I_x$

#### 自反闭包

$r(R) = R \cup I_x$

#### 对称闭包

$s(R) = R \cup R^0$

#### 传递闭包

$t(R) = \overset{|A|}{\underset{i = 1}{\cup}}R^i $

#### 等价关系

自反+对称+传递

#### 等价类

设R为集合A上的等价关系,对任何a∈A,集合$[a]_R$={xlx∈A,aRx}称为元素a形成的R等价类。

#### 划分

集合A上的等价关系R,决定了A的一个划分

#### 相容关系

自反+对称

#### 最大相容类

在相容关系图中，最大完全多边形的顶点集合

#### 完全覆盖

在集合A上给定相容关系，其最大相容类的集合称作集合A的完全覆盖

#### 偏序关系

自反+反对称+传递

#### 极大元、极小元

设$<A,\preccurlyeq>$;是一个偏序集合，且B是A的子集，对于B中的一个元素，如果B中没有任何元素x,满足$b \ne x$且,$b \preccurlyeq x$则称b为B的极大元。同理，对于$b \in B$,如果B中没有任何元素x,满足$b \ne x$且,$b \preccurlyeq x$则称为B的极小元。

## 类说明

```c++
#define READ 0           // 从文件中读
#define RANDOM 1         // 随机
#define REFLEXIVE 2      // 自反性
#define SYMMETRIC 4      // 对称性
#define TRANSTIVE 8      // 传递性
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
```

## 算法

声明：由于本人能力有限，部分复杂的算法仅用文字说明，无法用流程图展示

### 生成关系矩阵

关系矩阵采用顺序存储，具体实现采用了C++STL中的std::vector类，其原理就是可变数组，并在此基础上增加了对应的方法，方便操作，并且相比传统数组安全性得到提高。

生成关系矩阵有两种模式：一是从文件读取，二是根据要求随机产生符合的关系矩阵。前者生成关系矩阵只涉及文件读取和矩阵元素的赋值，而后者较为复杂。后者用宏定义了6种模式：RANDOM模式根据设定的矩阵密度完全随机生成关系矩阵，REFLEXIVE模式生成具有自反性的关系矩阵，SYMMETRIC模式生成具有对称性的关系矩阵，TRANSITIVE模式生成具有传递性的关系矩阵，IRREFLEXIVE模式生成具有反自反性的关系矩阵，ANTISYMMETRIC模式生成具有反对称性的关系矩阵。后5种模式分别对应了关系的5种常见属性，值分别为2的幂，可以用'|'将各模式组合，生成同时具有多种属性的关系矩阵。而生成方式是先按RANDOM模式随机生成一个关系矩阵，再以此为基础生成对应属性的闭包或关系矩阵。

##### warshall算法

(1)置新矩阵A:=M;
(2)置i:=1;
(3)对所有j如果A[j,i]=1,则对k=1,2,…,n，A[j,k]:=A[j,k] + A[i,k];
(4)i加1;
(5))如果i<=n则转到步骤(3),否则停止。

warshall算法的时间复杂度为$O(n^3)$

#### 流程图

![生成关系矩阵](https://gitee.com/InfiniteNightmare/album/raw/master/img/%E7%94%9F%E6%88%90%E5%85%B3%E7%B3%BB%E7%9F%A9%E9%98%B5.png)

### 判断关系矩阵属性

如果生成时已经指定关系矩阵的5种属性，则跳过关系矩阵的解析过程；否则遍历并解析关系矩阵，判断关系矩阵是否具有相应的属性。

遍历关系矩阵时，如果对角线存在0，则不满足自反性；如果对角线存在1，则不满足反自反性；如果非对角线的对称位置的值不相等则不满足对称性；如果非对角线的对称位置都为1则不满足反对称性；如果有传递性则同一位置，原矩阵为0，求幂后矩阵值一定为0。判断前几种属性的算法显而易见，接下来对判断传递性的算法进行证明。

-------------

#### 判断传递性的算法

##### 原理

设R为A上的关系，则

$R在A上传递 \Leftrightarrow R \circ R \subseteq R$

##### 证明

必要性：$\forall <x,y> \in R \circ R \\ \Rightarrow \exist t(<x,t> \in R \wedge <t,y> \in R) \\ \Rightarrow <x,y> \in R（因为R在A上时传递的）$

所以$R \circ R \subseteq R$

充分性：$\forall <x, y>, <y, z> \in R， 则 \\ <x, y> \in R \wedge <y, z> \in R \\ \Rightarrow <x, z> \in R \circ R \\ \Rightarrow  <x, z> \in R（因为R \circ R \subseteq R)$

所以R在A上是传递的

##### 算法说明

设M为R的关系矩阵形式，则

$R在A上传递 \Leftrightarrow R \circ R \subseteq R \Leftrightarrow M^2 \subseteq M$

若在同一位置，M的值为0，M*M必为0，否则关系矩阵不具有传递性。

算法时间复杂度为$O(n^3)$

----------

单一属性判断完后判断关系的复合属性：如果关系矩阵同时具有自反性、对称性和传递性，则关系矩阵为等价关系；如果关系矩阵同时具有自反性和对称性，则关系矩阵为相容关系；如果关系矩阵同时具有自反性、反对称性和传递性，则关系矩阵为偏序关系。

#### 流程图

![判断关系矩阵属性](https://gitee.com/InfiniteNightmare/album/raw/master/img/%E5%88%A4%E6%96%AD%E5%85%B3%E7%B3%BB%E7%9F%A9%E9%98%B5%E5%B1%9E%E6%80%A7%20(1).png)

### 求关系矩阵划分

先从头到尾遍历A，将当前元素和其后所有与其有关系的元素放在一个集合中，该集合代表当前的等价类，若该类与划分中已有的等价类无交集则代表该等价类是一个新的等价类，并将其插入划分中；若该类与某一等价类有交集则将该类合并到等价类中。

算法时间复杂度为$O(n^3)$

### 求关系矩阵完全覆盖

若将关系矩阵看作图的邻接矩阵，则关系矩阵的完全覆盖的寻找可以看作寻找图的所有极大连通子图。算法为用三个数组分别存储当前子图的顶点、所有顶点、所有待选顶点。先遍历所有顶点，若当前顶点不在先前的子图中，即待选顶点集中有该顶点，则将该顶点从待选顶点中取出，以该结点构造一个极大连通子图，使当前子图的顶点集为该顶点，并在所有顶点中找到与当前子图都相连的顶点后从待选顶点集中删除该顶点，放到当前子图的顶点集中，最后将当前子图插入完全覆盖集中。

算法时间复杂度为$O(n^3)$

### 寻找关系矩阵极小/大元

偏序关系的极小元是没有其他元素被该元素盖住，极大元是没有其他元素盖住该元素。反映到关系矩阵上就是一列上只有对角线元素为1和一行上只有对角线元素为1。算法就是遍历关系矩阵，如果一列或一行元素的累加和为1，则对应元素为极小/大元。算法的时间复杂度都为$O(n^2)$

#### 流程图

<div> 
    <span>
        <span style="font-weight:bold">寻找极小元</span>       
        <img src="https://gitee.com/InfiniteNightmare/album/raw/master/img/%E6%9E%81%E5%B0%8F%E5%85%83.png" alt="极小元" style="zoom:80%"/>         
    </span>
	<span>
        <span style="font-weight:bold">寻找极大元</span>             
        <img src="https://gitee.com/InfiniteNightmare/album/raw/master/img/%E6%9E%81%E5%A4%A7%E5%85%83%20(1).png" alt="极大元" style="zoom:80%"/>
    </span>
</div>

## 实例演示

声明：AXA为全域关系，限于篇幅，输出截图可能截不完全

### 演示一：从文件中读取关系矩阵

#### 演示代码

```c++
    int n;
    std::cin >> n;
    RMatrix rmatrix = RMatrix(n, READ);
    rmatrix.printRMatrix();
    rmatrix.printAttrs();
```

#### 输入

![image-20201129213821299](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129213821299.png)

#### 输出

![image-20201129214042410](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129214042410.png)

### 演示二：随机产生关系矩阵

#### 演示代码

```c++
    int n;
    std::cin >> n;
    RMatrix rmatrix = RMatrix(n, RANDOM);
    rmatrix.printRMatrix();
    rmatrix.printAttrs();
```

#### 输入

![image-20201129215020235](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129215020235.png)

#### 输出

![image-20201129215058234](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129215058234.png)

### 演示三：自反性+传递性

#### 演示代码

```c++
    int n;
    std::cin >> n;
    RMatrix rmatrix = RMatrix(n, REFLEXIVE | TRANSITIVE);
    rmatrix.printRMatrix();
    rmatrix.printAttrs();
```



#### 输入

![image-20201129215720083](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129215720083.png)

#### 输出

![image-20201129215736820](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129215736820.png)



### 演示四：反自反性+反对称性

#### 演示代码

```c++
    int n;
    std::cin >> n;
    RMatrix rmatrix = RMatrix(n, IRREFLEXIVE | ANTISYMMETRIC);
    rmatrix.printRMatrix();
    rmatrix.printAttrs();
```

#### 输入

![image-20201129220713416](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129220713416.png)

#### 输出

![image-20201129220727311](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129220727311.png)

### 演示五：等价关系

#### 演示代码

```c++
    int n;
    std::cin >> n;
    RMatrix rmatrix = RMatrix(n, REFLEXIVE | SYMMETRIC | TRANSITIVE);
    rmatrix.printRMatrix();
    rmatrix.printAttrs();
```

#### 输入

![image-20201129215914934](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129215914934.png)

#### 输出

![image-20201129215930098](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129215930098.png)

### 演示六：相容关系

#### 演示代码

```c++
    int n;
    std::cin >> n;
    RMatrix rmatrix = RMatrix(n, REFLEXIVE | SYMMETRIC);
    rmatrix.printRMatrix();
    rmatrix.printAttrs()
```

#### 输入

![image-20201129220130920](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129220130920.png)

#### 输出

![image-20201129220147500](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129220147500.png)

### 演示七：偏序关系

#### 演示代码

```c++
    int n;
    std::cin >> n;
    RMatrix rmatrix = RMatrix(n, REFLEXIVE | ANTISYMMETRIC | TRANSITIVE, 15);
    rmatrix.printRMatrix();
    rmatrix.printAttrs();
```

#### 输入

![image-20201129220409100](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129220409100.png)

#### 输出

![image-20201129220440228](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129220440228.png)

### 演示八：大关系矩阵

#### 演示代码

```c++
    int n;
    std::cin >> n;
    RMatrix rmatrix = RMatrix(n, REFLEXIVE | SYMMETRIC | TRANSITIVE, 5);
    rmatrix.printRMatrix();
    rmatrix.printAttrs();
```

#### 输入

![image-20201129221413449](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129221413449.png)

#### 输出（部分）

![image-20201129221353557](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201129221353557.png)

## 总结

本次作业虽然实际代码行数不多，但涉及的知识有广度也有深度，包括了集合论、图论、面向对象程序设计、算法设计等方面，而且大部分时间花在了搜寻、阅读和理解资料上。一些算法中用到了还未学的图论的算法思想，但我对其的理解还不深，比如求完全覆盖就是求所有极大连通子图，常用的算法是Bron-Kerbosch算法，而我没有彻底弄明白并能实现算法，所以我退而求其次按照我的理解改造了Bron-Kerbosch算法，虽然最后能完成求所有极大连通子图，但算法的效率肯定不如Bron-Kerbosch算法。而且在随机生成关系矩阵的算法我也没有证明其可行性，按前文所述，我是在随机生成关系矩阵的前提下通过构造相应属性的闭包来产生随机的带有指定属性的关系矩阵的；我仿照已有的生成自反闭包、传递闭包的算法写了生成反自反闭包和反传递闭包，但不清楚这样的闭包经过复合后是否保留原属性，如自反闭包的传递闭包的反对称闭包是否保留了自反性、传递性和反对称性。许多问题要么是因为学习的知识不足，要么是因为时间不足，希望以后等我有能力、有时机再深入研究这些问题。总体说来本次作业对我的提升很大，更重要的是让我看到了计算机科学的魅力。
