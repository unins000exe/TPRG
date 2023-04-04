#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
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

// not gm
void gm(vector <double>& u, double mu, double sigma)
{
    const double PI = acos(-1.0);
    for (size_t i = 0; i < u.size() - 1; i += 2)
    {
        double u1 = u[i];
        u[i] = mu + sigma * sqrt(-2 * log(1 - u[i])) * cos(2 * PI * u[i + 1]);
        u[i + 1] = mu + sigma * sqrt(-2 * log(1 - u1)) * sin(2 * PI * u[i + 1]);
    }
}


void ln(vector <double>& u, double a, double b)
{
    nr(u, a, b);
    for (size_t i = 0; i < u.size() - 1; i++)
    {
        u[i] = a + exp(b - u[i]);
    }
}


void ls(vector <double>& u, double a, double b)
{
    for (size_t i = 0; i < u.size() - 1; i++)
    {
        u[i] = a + b * log(u[i] / (1 - u[i]));
    }
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    argparse::ArgumentParser parser("Преобразование ПСЧ к заданному распределению", "1.0");

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
    double p1, p2;

    if (parser.is_used("/d")) {
        method_code = parser.get("/d");
        outfile = "rnc_" + method_code + ".dat";
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

    const int max = 1024; // ?Нужно ли считать максимальное число в файле?

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
        gm(numbers, p1, p2);
    }
    else if (method_code == "ln")
    {
        ln(numbers, p1, p2);
    }
    else if (method_code == "ls")
    {
        ls(numbers, p1, p2);
    }

    ofstream outFile(outfile);

    for (double x : numbers)
    {
        outFile << x << ',';
    }
    
    outFile.close();

    return 0;
}


