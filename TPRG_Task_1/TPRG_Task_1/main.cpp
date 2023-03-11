#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <argparse.hpp>
#include <boost/dynamic_bitset.hpp>

using namespace std;
using namespace boost;

void lc(int m, int a, int c, int x0, int n, string file_name)
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


void add(int m, int k, int j, vector<int> lag, int n, string file_name)
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

void lfsr(string coef, int seed, int n, string file_name)
{
    ofstream outFile(file_name);
    const unsigned int bit_size = coef.length();

    dynamic_bitset<> reg(bit_size, seed); // Первоначальное значение регистра задаётся пользователем
    dynamic_bitset<> coefs(coef);

    cout << "Прогресс генерации ПСЧ: \n";
    const int step = n / 10;

    for (size_t i = 0; i < n; i++)
    {
        if (i % step == 0) {
            cout << '\r' << flush;
            cout << "  * Выполнено " << (i * 100) / n << "%";
        }

        bool new_bit = false;
        for (size_t j = 0; j < bit_size; j++)
        {
            if (coefs[j])
            {
                new_bit ^= reg[j];
            }
        }
        
        reg >>= 1;
        reg.set(bit_size - 1, new_bit);

        unsigned int xn = static_cast<unsigned int>(reg.to_ulong());
        outFile << xn << ',';
    }

    cout << '\r' << flush;
    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name << "\n";

    outFile.close();
}


void p5(unsigned int p, unsigned int q1, unsigned int q2, unsigned int q3,  unsigned int w, int n, string file_name)
{
    ofstream outFile(file_name);

    dynamic_bitset<> reg(w, p); 

    cout << "Прогресс генерации ПСЧ: \n";
    int step = n / 10;
   
    for (size_t i = 0; i < n; i++)
    {
        if (i % step == 0) {
            cout << '\r' << flush;
            cout << "  * Выполнено " << (i * 100) / n << "%";
        }

        bool new_bit = false;

        new_bit = reg[q1] ^ reg[q2] ^ reg[q3] ^ reg[0];

        reg >>= 1;
        reg.set(w - 1, new_bit);

        unsigned int xn = static_cast<unsigned int>(reg.to_ulong());
        outFile << xn << ',';
    }

    cout << '\r' << flush;
    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name << "\n";

    outFile.close();
}


void nfsr(string scoefs1, string scoefs2, string scoefs3, int seed1, int seed2, int seed3, int n, string file_name)
{
    ofstream outFile(file_name);
    const unsigned int bit_size1 = scoefs1.length();
    const unsigned int bit_size2 = scoefs2.length();
    const unsigned int bit_size3 = scoefs3.length();

    dynamic_bitset<> reg1(bit_size1, seed1);
    dynamic_bitset<> coefs1(scoefs1);

    dynamic_bitset<> reg2(bit_size2, seed2);
    dynamic_bitset<> coefs2(scoefs2);

    dynamic_bitset<> reg3(bit_size3, seed3);
    dynamic_bitset<> coefs3(scoefs3);

    cout << "Прогресс генерации ПСЧ: \n";
    const int step = n / 10;

    for (size_t i = 0; i < n; i++)
    {
        if (i % step == 0) {
            cout << '\r' << flush;
            cout << "  * Выполнено " << (i * 100) / n << "%";
        }

        // R1
        bool new_bit = false;
        for (size_t j = 0; j < bit_size1; j++)
        {
            if (coefs1[j])
            {
                new_bit ^= reg1[j];
            }
        }
        reg1 >>= 1;
        reg1.set(bit_size1 - 1, new_bit);

        // R2
        new_bit = false;
        for (size_t j = 0; j < bit_size2; j++)
        {
            if (coefs2[j])
            {
                new_bit ^= reg2[j];
            }
        }
        reg2 >>= 1;
        reg2.set(bit_size2 - 1, new_bit);

        // R3
        new_bit = false;
        for (size_t j = 0; j < bit_size3; j++)
        {
            if (coefs3[j])
            {
                new_bit ^= reg3[j];
            }
        }
        reg3 >>= 1;
        reg3.set(bit_size3 - 1, new_bit);

        unsigned int xn = static_cast<unsigned int>(((reg1 & reg2) ^ (reg2 & reg3) ^ reg3).to_ulong());
        outFile << xn << ',';
    }

    cout << '\r' << flush;
    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name << "\n";

    outFile.close();
}


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    argparse::ArgumentParser parser("Генератор псевдослучайных чисел", "1.4");

    parser.set_prefix_chars("-/");
    parser.set_assign_chars("=:");

    parser.add_argument("/g")
        .help("Методы генерации ПСЧ: lc, add, 5p, lfsr, nfsr");

    parser.add_argument("/i")
        .help(R"(Инициализационный вектор генератора (параметры записываются через запятую).
                 * lc: m, a, c, x0    
                 * add: m, k, j, j начальных значений
                 * 5p: p, q1, q2, q3, w (q1, q2, q3 < w < 32)
                 * lfsr: двоичный вектор коэффициентов (до 32 бит), начальное значение регистра
                 * nfsr: три двоичных вектора коэффициентов (до 32 бит), начальные значения трёх регистров)");

    parser.add_argument("/n")
        .help("Количество генерируемых чисел")
        .default_value(int(10000))
        .scan<'i', int>();

    parser.add_argument("/f")
        .help("Имя файла для вывода")
        .default_value(string("rnd.dat"));

    try {
        parser.parse_args(argc, argv);
    }
    catch (const runtime_error& err) {
        cout << err.what() << endl;
        cout << parser;
        return 1;
    }

    vector <int> i_vec;
    vector <string> is_vec; // TODO: Возможно стоит придумать что-то получше для lfsr
    string method_code;
    int n = 10000;
    string file_name = "rnd.dat";

    if (parser.is_used("/g")) {
        method_code = parser.get("/g");
        //cout << "/g: " << method_code << "\n";
    }

    if (parser.is_used("/i")) {
        string i_str = parser.get<string>("/i");

        stringstream ss(i_str);
        string item;

        if (method_code == "lfsr" or method_code == "nfsr")
        {
            while (getline(ss, item, ','))
            {
                is_vec.push_back(item);
            }
        }
        else
        {
            while (getline(ss, item, ','))
            {
                i_vec.push_back(stoi(item));
            }
        }
        //cout << "/i: ";
        //for (auto x : i_vec)
        //{
        //    cout << x << " ";
        //}
        //cout << "\n";
    }

    if (parser.is_used("/n")) {
        n = parser.get<int>("/n");
        //cout << "/n: " << n << "\n";
    }

    if (parser.is_used("/f")) {
        file_name = parser.get("/f");
        //cout << "/f: " << file_name << "\n";
    }


    if (method_code == "lc") {
        lc(i_vec[0], i_vec[1], i_vec[2], i_vec[3], n, file_name);
    }
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
        lfsr(is_vec[0], stoi(is_vec[1]), n, file_name);
    }
    else if (method_code == "5p")
    {
        p5(i_vec[0], i_vec[1], i_vec[2], i_vec[3], i_vec[4], n, file_name);
    }
    else if (method_code == "nfsr")
    {
        nfsr(is_vec[0], is_vec[1], is_vec[2], stoi(is_vec[3]), stoi(is_vec[4]), stoi(is_vec[5]), n, file_name);
    }
    
    return 0;

}


