#include <iostream>
#include <fstream>
#include <argparse.hpp>
#include <vector>

using namespace std;

void lc(int x0, int a, int c, int m, int n, string file_name)
{
    ofstream outFile(file_name);

    int xn = x0;
    cout << "Прогресс генерации ПСЧ: \n";
    int step = n / 10;
    for (size_t i = 0; i < n; i++)
    {
        if (i % step == 0) {
            cout << '\r' << flush;
            cout << "  * Выполнено " << (i * 100) / n << "%";
        }

        outFile << xn << endl;
        xn = (a * x0 + c) % m;
        x0 = xn;
    }

    cout << '\r' << flush;
    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name <<  "\n";

    outFile.close();
}


void add(int k, int j, int m, int n, string file_name)
{
    ofstream outFile(file_name);

    cout << "Прогресс генерации ПСЧ: \n";
    int step = n / 10;

    vector <int> lag = { k, j }; // Первые j чисел определяются по формуле Фибоначчи по модулю m

    int xn;
    for (size_t i = 0; i < j - 2; i++)
    {
        xn = (lag[i] + lag[i + 1]) % m;
        lag.push_back(xn);
        outFile << xn << endl;
    }

    for (size_t i = 55; i < n; i++)
    {
        if (i % step == 0) {
            cout << '\r' << flush;
            cout << "  * Выполнено " << (i * 100) / n << "%";
        }

        xn = (lag[j - k] + lag[0]) % m;
        lag.erase(lag.begin());
        lag.push_back(xn);

        outFile << xn << endl;
    }

    cout << '\r' << flush;
    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name << "\n";

    outFile.close();
}

//void lfsr(int k, int j, int m, int n, string file_name)
//{
//    ofstream outFile(file_name);
//
//    cout << "Прогресс генерации ПСЧ: \n";
//    int step = n / 10;
//
//    int xn;
//    for (size_t i = 55; i < n; i++)
//    {
//        if (i % step == 0) {
//            cout << '\r' << flush;
//            cout << "  * Выполнено " << (i * 100) / n << "%";
//        }
//
//
//        outFile << xn << endl;
//    }
//
//    cout << '\r' << flush;
//    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name << "\n";
//
//    outFile.close();
//}


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    // lc(24);

    argparse::ArgumentParser parser("Генератор псевдослучайных чисел", "1.1");

    parser.set_prefix_chars("-/");
    parser.set_assign_chars("=:");

    parser.add_argument("/g")
        .help("Методы генерации ПСЧ: lc");

    parser.add_argument("/i", "")
        .help("Инициализационный вектор генератора");

    parser.add_argument("/n", "")
        .help("Количество генерируемых чисел")
        .default_value(int(10000))
        .scan<'i', int>();

    parser.add_argument("/f")
        .help("Имя файла для вывода")
        .default_value(string("rnd.dat"));

    try {
        parser.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        cout << err.what() << endl;
        cout << parser;
        return 1;
    }

    vector <int> i_vec;
    string method_code;
    int n = 10000;
    string file_name = "rnd.dat";

    if (parser.is_used("/g")) {
        method_code = parser.get("/g");
        std::cout << "/g: " << method_code << "\n";
    }

    if (parser.is_used("/i")) {
        string i_str = parser.get<string>("/i");

        std::stringstream ss(i_str);
        std::string item;

        while (getline(ss, item, ',')) {
            i_vec.push_back(stoi(item));
        }

        cout << "/i: ";
        for (auto x : i_vec)
        {
            cout << x << " ";
        }
        cout << "\n";
    }

    if (parser.is_used("/n")) {
        n = parser.get<int>("/n");
        std::cout << "/n: " << n << "\n";
    }

    if (parser.is_used("/f")) {
        file_name = parser.get("/f");
        std::cout << "/f: " << file_name << "\n";
    }


    // /g:lc /i:39,625,6571,31104 /n:1000000
    if (method_code == "lc") {
        lc(i_vec[0], i_vec[1], i_vec[2], i_vec[3], n, file_name);
    }
    // /g:add /i:24,55,30000 /n : 100000
    else if (method_code == "add")
    {
        add(i_vec[0], i_vec[1], i_vec[2], n, file_name);
    }



    //string method_code = parser.get<string>("код метода");

    //int n = parser.get<int>("количество генерируемых чисел");

    //string file_name = parser.get<string>("имя файла вывода");

    //cout << method_code << endl;
    //cout << n << endl;
    //cout << file_name << endl;


    //cout << method_code << endl << n << endl << file_name << endl;


    return 0;

}


