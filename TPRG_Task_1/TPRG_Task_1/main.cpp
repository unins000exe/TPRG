#include <iostream>
#include <fstream>
#include <argparse.hpp>
#include <vector>
#include <bitset>

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

        outFile << xn << ',';
        xn = (a * x0 + c) % m;
        x0 = xn;
    }

    cout << '\r' << flush;
    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name <<  "\n";

    outFile.close();
}


void add(int k, int j, int m, vector<int> lag, int n, string file_name)
{
    ofstream outFile(file_name);

    cout << "Прогресс генерации ПСЧ: \n";
    int step = n / 10;

    int xn;
    for (size_t i = 0; i < j; i++)
    {
        outFile << lag[i] << ',';
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

        outFile << xn << ',';
    }

    cout << '\r' << flush;
    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name << "\n";

    outFile.close();
}
//
void lfsr(unsigned int seed, unsigned int coef, int n, string file_name)
{
    ofstream outFile(file_name);
    
    const size_t reg_size = 32;
    const bitset<reg_size> coefs(coef);
    bitset<reg_size> reg(seed); // Первоначальный регистр задаётся пользователем

    cout << "Прогресс генерации ПСЧ: \n";
    int step = n / 10;

    for (size_t i = 0; i < n; i++)
    {
        if (i % step == 0) {
            cout << '\r' << flush;
            cout << "  * Выполнено " << (i * 100) / n << "%";
        }

        bool new_bit = false;
        for (size_t j = 0; j < reg_size; j++)
        {
            if (coefs[j])
            {
                new_bit ^= reg[j];
            }
        }

        reg >>= 1;
        reg.set(reg_size - 1, new_bit);

        unsigned int xn = static_cast<unsigned int>(reg.to_ulong());
        outFile << xn << ',';
    }

    cout << '\r' << flush;
    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name << "\n";

    outFile.close();
}


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    // lc(24);

    argparse::ArgumentParser parser("Генератор псевдослучайных чисел", "1.2");

    parser.set_prefix_chars("-/");
    parser.set_assign_chars("=:");

    parser.add_argument("/g")
        .help("Методы генерации ПСЧ: lc, add \n");

    parser.add_argument("/i", "")
        .help(R"(Инициализационный вектор генератора (параметры записываются через запятую).
                 * lc: x0, a, c, m    
                 * add: k, j, m, j чисел)");

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
    // /g:add /i:24,55,30000,79,134,213,347,560,907,1467,2374,3841,6215,10056,16271,26327,12598,8925,21523,448,21971,22419,14390,6809,21199,28008,19207,17215,6422,23637,59,23696,23755,17451,11206,28657,9863,8520,18383,26903,15286,12189,27475,9664,7139,16803,23942,10745,4687,15432,20119,5551,25670,1221,26891,28112,23779,17506 /n:100000
    else if (method_code == "add")
    {
        int k = i_vec[0];
        int j = i_vec[1];
        int m = i_vec[2];
        i_vec.erase(i_vec.begin(), i_vec.begin() + 3);
        add(k, j, m, i_vec, n, file_name);
    }
    else if (method_code == "lfsr")
    {
        lfsr(i_vec[0], i_vec[1], n, file_name);
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


