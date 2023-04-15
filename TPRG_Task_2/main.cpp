#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <argparse.hpp>

using namespace std;

void st(vector <double> &u, double a, double b)
{
    for (size_t i = 0; i < u.size(); i++)
    {
        u[i] = b * u[i] + a;
    }
}

void tr(vector <double>& u, double a, double b)
{
    double u0 = u[0]; // надо в цикле сохранять
    for (size_t i = 0; i < u.size() - 1; i++)
    {
        u[i] = b * (u[i] + u[i + 1]) + a;
    }
    // Здесь U1 - последнее число, U2 - самое первое
    u[u.size() - 1] = b * (u[u.size() - 1] + u0) + a;
}

void ex(vector <double>& u, double a, double b)
{
    for (size_t i = 0; i < u.size(); i++)
    {
        u[i] = -b * log(u[i]) + a;
    }
}

void nr(vector <double>& u, double mu, double sigma)
{
    const double PI = acos(-1.0);
    for (size_t i = 0; i < u.size() - 1; i += 2)
    {
        double u1 = u[i];
        u[i] = mu + sigma * sqrt(-2 * log(1 - u[i])) * cos(2 * PI * u[i + 1]);
        u[i + 1] = mu + sigma * sqrt(-2 * log(1 - u1)) * sin(2 * PI * u[i + 1]);
    }
}

void gm(vector <double>& u, double a, double b, double c)
{
    vector <double> u_copy(u);
    if (floor(c) == c)
    {
        int len = u.size();
        double lg_uk;
        for (size_t i = 0; i < len; i++)
        {
            lg_uk = 1;
            for (size_t j = 0; j < c; ++j)
            {
                // Модуль len нужен для циклического сдвига "окна"
                lg_uk *= (1 - u_copy[(i + j) % len]);
            }
            u[i] = a - b * log(lg_uk);
        }
    }
    else
    {
        cout << "Число c должно быть целым!\n";
    }

}


void ln(vector <double>& u, double a, double b)
{
    nr(u, a, b);
    for (size_t i = 0; i < u.size(); i++)
    {
        u[i] = a + exp(b - u[i]);
    }
}


void ls(vector <double>& u, double a, double b)
{
    for (size_t i = 0; i < u.size(); i++)
    {
        u[i] = a + b * log(u[i] / (1 - u[i]));
    }
}


int binomialCoeff(int n, int k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;

    return binomialCoeff(n - 1, k - 1)
        + binomialCoeff(n - 1, k);
}

void bi(vector <double>& u, double a, double b)
{

    for (size_t i = 0; i < u.size(); i++)
    {
        double s = 0;
        int k = 0;
        while (true)
        {
            s += binomialCoeff(b, k) * pow(a, k) * pow((1 - a), b - k);
            if (s > u[i])
            {
                u[i] = k;
                break;
            }
            if (k < b - 1)
            {
                k += 1;
                continue;
            }
            u[i] = b;
        }
    }
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    argparse::ArgumentParser parser("Преобразование ПСЧ к заданному распределению", "1.1");

    parser.set_prefix_chars("-/");
    parser.set_assign_chars("=:");

    parser.add_argument("/d")
        .help("Распределения: st, tr, ex, nr, gm, ln, ls, bi");

    parser.add_argument("/f")
        .help("Имя входного файла")
        .default_value(string("rnd.dat"));

    parser.add_argument("/p1")
        .help("Первый параметр")
        .scan<'g', double>();

    parser.add_argument("/p2")
        .help("Второй параметр")
        .scan<'g', double>();

    parser.add_argument("/p3")
        .help("Третий параметр (для гамма распределения)")
        .scan<'g', double>();

    try {
        parser.parse_args(argc, argv);
    }
    catch (const runtime_error& err) {
        cout << err.what() << endl;
        cout << parser;
        return 1;
    }

    string method_code;
    string infile = "D:/CSIT/TPRG/TPRG_Task_1/rnd.dat";
    string outfile = "";
    double p1, p2, p3;

    if (parser.is_used("/d")) {
        method_code = parser.get("/d");
        outfile = "distr_" + method_code + ".dat";
    }
    else
    {
        return 1;
    }

    if (parser.is_used("/f")) {
        infile = parser.get("/f");
    }

    if (parser.is_used("/p1")) {
        p1 = parser.get<double>("/p1");
    }

    if (parser.is_used("/p2")) {
        p2 = parser.get<double>("/p2");
    }

    if (parser.is_used("/p2")) {
        p2 = parser.get<double>("/p2");
    }

    if (parser.is_used("/p3")) {
        p3 = parser.get<double>("/p3");
    }

    const int max = 1000; // ?Нужно ли считать максимальное число в файле?

    // Считывание чисел из файла и приведение их к случайной величине
    vector <double> numbers;
    ifstream inputFile(infile);
    if (inputFile)
    {
        string line;
        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string numberString;
            while (getline(ss, numberString, ','))
            {
                double number = stod(numberString);
                numbers.push_back(number / max);
            }
        }
        inputFile.close();
    }
    else
    {
        cout << "Не удалось открыть файл. \n";
        return 1;
    }

    if (method_code == "st")
    {
        st(numbers, p1, p2);
    }
    else if (method_code == "tr")
    {
        tr(numbers, p1, p2);
    }
    else if (method_code == "ex")
    {
        ex(numbers, p1, p2);
    }
    else if (method_code == "nr")
    {
        nr(numbers, p1, p2);
    }
    else if (method_code == "gm")
    {
        gm(numbers, p1, p2, p3);
    }
    else if (method_code == "ln")
    {
        ln(numbers, p1, p2);
    }
    else if (method_code == "ls")
    {
        ls(numbers, p1, p2);
    }
    else if(method_code == "bi")
    {
        bi(numbers, p1, p2);
    }

    ofstream outFile(outfile);

    for (double x : numbers)
    {
        outFile << x << ',';
    }

    cout << "Результат записан в " << outfile << "\n";
    
    outFile.close();

    return 0;
}


