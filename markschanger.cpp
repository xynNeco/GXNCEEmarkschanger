#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

using namespace std;

// 定义科目满分常量
const int SCORE_MAX_CHINESE_MATH_ENGLISH = 150; // 语数英满分
const int SCORE_MAX_PHYS_CHEM_BIO = 100;        // 物化生满分

// 赋分等级（广西五等级标准）
struct GradeInfo {
    string grade;       // 等级（A/B/C/D/E）
    double ratioStart;  // 等级起始比例（%）
    double ratioEnd;    // 等级结束比例（%）
    int scoreStart;     // 赋分区间下限
    int scoreEnd;       // 赋分区间上限
};

// 初始化广西高考赋分等级规则（5个等级）
GradeInfo GuangXiGradeRules[5] = {
    {"A", 0.0, 15.0, 86, 100},   // 前15%，赋分86-100
    {"B", 15.0, 50.0, 71, 85},   // 15%-50%，赋分71-85
    {"C", 50.0, 85.0, 56, 70},   // 50%-85%，赋分56-70
    {"D", 85.0, 98.0, 41, 55},   // 85%-98%，赋分41-55
    {"E", 98.0, 100.0, 30, 40}   // 后2%，赋分30-40
};

// 函数：输入合法性校验（整数分数）
int inputValidScore(string subjectName, int maxScore) {
    int score;
    while (true) {
        cout << "  请输入" << subjectName << "分数（0-" << maxScore << "）：";
        cin >> score;
        if (cin.fail() || score < 0 || score > maxScore) {
            cin.clear(); // 清除输入错误标记
            cin.ignore(1024, '\n'); // 忽略缓冲区剩余错误数据
            cout << "       输入错误！请输入0到" << maxScore << "之间的有效整数！" << endl;
        }
        else {
            cin.ignore(1024, '\n'); // 清空缓冲区
            break;
        }
    }
    return score;
}

// 函数：输入合法性校验（正整数，用于排名、总人数）
int inputValidPositiveInt(string prompt) {
    int num;
    while (true) {
        cout << prompt;
        cin >> num;
        if (cin.fail() || num <= 0) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "       输入错误！请输入正整数！" << endl;
        }
        else {
            cin.ignore(1024, '\n');
            break;
        }
    }
    return num;
}

// 函数：根据排名和总人数判定赋分等级
GradeInfo getGradeInfo(int rank, int totalNum) {
    // 计算排名比例（%），注意：排名第1为前1/totalNum，这里按"排名≤n"计算比例
    double ratio = (double)rank / totalNum * 100.0;
    // 匹配广西赋分等级规则
    for (int i = 0; i < 5; i++) {
        if (ratio > GuangXiGradeRules[i].ratioStart && ratio <= GuangXiGradeRules[i].ratioEnd) {
            return GuangXiGradeRules[i];
        }
    }
    // 默认返回E等级（极端情况兜底）
    return GuangXiGradeRules[4];
}

// 函数：广西高考赋分计算公式实现
int calculateAssignedScore(int rawScore, GradeInfo grade, int rawGradeStart, int rawGradeEnd) {
    // 赋分公式：X = X1 + (Y-Y1)*(X2-X1)/(Y2-Y1)
    // Y：原始分，Y1：等级原始分下限，Y2：等级原始分上限
    // X：赋分结果，X1：等级赋分下限，X2：等级赋分上限
    double Y = rawScore;
    double Y1 = rawGradeStart;
    double Y2 = rawGradeEnd;
    double X1 = grade.scoreStart;
    double X2 = grade.scoreEnd;

    // 特殊情况：原始分等于区间上下限，直接返回对应赋分
    if (rawScore == Y1) return (int)X1;
    if (rawScore == Y2) return (int)X2;

    // 计算赋分结果并四舍五入
    double assignedScore = X1 + (Y - Y1) * (X2 - X1) / (Y2 - Y1);
    return (int)round(assignedScore);
}

// 函数：展示程序欢迎界面
void showWelcomeUI() {
    cout << "=============================================================" << endl;
    cout << "                      广西高考赋分后总分计算器" << endl;
    cout << "=============================================================" << endl;
    cout << "  说明：1. 语/数/英/物理为原始分计入总分" << endl;
    cout << "        2. 化学/生物按广西五等级赋分规则计算赋分" << endl;
    cout << "        3. 赋分核心：排名决定等级，等级内等比例转换" << endl;
    cout << "=============================================================" << endl;
}

int main() {
    // 1. 展示欢迎界面
    showWelcomeUI();

    //输入各科原始分数（语数英物化生）
    cout << "\n------------------------ 输入各科原始分数 ------------------------" << endl;
    int chinese = inputValidScore("语文", SCORE_MAX_CHINESE_MATH_ENGLISH);
    int math = inputValidScore("数学", SCORE_MAX_CHINESE_MATH_ENGLISH);
    int english = inputValidScore("英语", SCORE_MAX_CHINESE_MATH_ENGLISH);
    int physics = inputValidScore("物理", SCORE_MAX_PHYS_CHEM_BIO);
    int chemistryRaw = inputValidScore("化学", SCORE_MAX_PHYS_CHEM_BIO);
    int biologyRaw = inputValidScore("生物", SCORE_MAX_PHYS_CHEM_BIO);

    //输入化学科目的赋分相关数据
    cout << "\n------------------------ 输入化学赋分辅助数据 ------------------------" << endl;
    int chemRank = inputValidPositiveInt("  请输入你的化学排名（正整数）：");
    int chemTotalNum = inputValidPositiveInt("  请输入化学参考总人数（正整数）：");
    while (chemRank > chemTotalNum) {
        cout << "     排名不能超过参考总人数！请重新输入！" << endl;
        chemRank = inputValidPositiveInt("  请输入你的化学排名（正整数）：");
    }
    int chemRawStart = inputValidScore("化学该等级原始分下限", SCORE_MAX_PHYS_CHEM_BIO);
    int chemRawEnd = inputValidScore("化学该等级原始分上限", SCORE_MAX_PHYS_CHEM_BIO);
    while (chemRawStart >= chemRawEnd) {
        cout << "     原始分下限不能大于等于上限！请重新输入！" << endl;
        chemRawStart = inputValidScore("化学该等级原始分下限", SCORE_MAX_PHYS_CHEM_BIO);
        chemRawEnd = inputValidScore("化学该等级原始分上限", SCORE_MAX_PHYS_CHEM_BIO);
    }

    //输入生物科目的赋分相关数据
    cout << "\n------------------------ 输入生物赋分辅助数据 ------------------------" << endl;
    int bioRank = inputValidPositiveInt("  请输入你的生物排名（正整数）：");
    int bioTotalNum = inputValidPositiveInt("  请输入生物参考总人数（正整数）：");
    while (bioRank > bioTotalNum) {
        cout << "     排名不能超过参考总人数！请重新输入！" << endl;
        bioRank = inputValidPositiveInt("  请输入你的生物排名（正整数）：");
    }
    int bioRawStart = inputValidScore("生物该等级原始分下限", SCORE_MAX_PHYS_CHEM_BIO);
    int bioRawEnd = inputValidScore("生物该等级原始分上限", SCORE_MAX_PHYS_CHEM_BIO);
    while (bioRawStart >= bioRawEnd) {
        cout << "     原始分下限不能大于等于上限！请重新输入！" << endl;
        bioRawStart = inputValidScore("生物该等级原始分下限", SCORE_MAX_PHYS_CHEM_BIO);
        bioRawEnd = inputValidScore("生物该等级原始分上限", SCORE_MAX_PHYS_CHEM_BIO);
    }

    //计算赋分（化+生）
    GradeInfo chemGrade = getGradeInfo(chemRank, chemTotalNum);
    GradeInfo bioGrade = getGradeInfo(bioRank, bioTotalNum);
    int chemistryAssigned = calculateAssignedScore(chemistryRaw, chemGrade, chemRawStart, chemRawEnd);
    int biologyAssigned = calculateAssignedScore(biologyRaw, bioGrade, bioRawStart, bioRawEnd);

    //计算总分
    int totalScore = chinese + math + english + physics + chemistryAssigned + biologyAssigned;

    //格式化输出结果
    cout << "\n=============================================================" << endl;
    cout << "                          计算结果汇总" << endl;
    cout << "=============================================================" << endl;
    cout << left << setw(12) << "科目" << setw(12) << "原始分" << setw(12) << "赋分/原始分"
        << setw(12) << "赋分等级" << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << left << setw(12) << "语文" << setw(12) << chinese << setw(12) << "原始分计入"
        << setw(12) << "无" << endl;
    cout << left << setw(12) << "数学" << setw(12) << math << setw(12) << "原始分计入"
        << setw(12) << "无" << endl;
    cout << left << setw(12) << "英语" << setw(12) << english << setw(12) << "原始分计入"
        << setw(12) << "无" << endl;
    cout << left << setw(12) << "物理" << setw(12) << physics << setw(12) << "原始分计入"
        << setw(12) << "无" << endl;
    cout << left << setw(12) << "化学" << setw(12) << chemistryRaw << setw(12) << chemistryAssigned
        << setw(12) << chemGrade.grade << endl;
    cout << left << setw(12) << "生物" << setw(12) << biologyRaw << setw(12) << biologyAssigned
        << setw(12) << bioGrade.grade << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << "   最终总分（含赋分）：" << totalScore << " 分" << endl;
    cout << "=============================================================" << endl;

    return 0;
}