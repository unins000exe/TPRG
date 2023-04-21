#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <argparse.hpp>
#include <boost/dynamic_bitset.hpp>

using namespace std;
using namespace boost;


class MersenneTwister32 {
public:
    explicit MersenneTwister32(uint32_t seed) {
        mt[0] = seed;
        for (int i = 1; i < p; ++i)
        {
            mt[i] = ((mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
        }
        ind = p;
    }

    uint32_t generate() {
        if (ind >= p) 
        {
            twist();
        }

        uint32_t y = mt[ind++];
        y ^= (y >> u);
        y ^= (y << s) & b;
        y ^= (y << t) & c;
        y ^= (y >> l);
        return y;
    }

private:
    static constexpr int p = 624;
    const int u = 11;
    const int s = 7;
    const int t = 15;
    const int l = 18;
    const int q = 397;
    const uint32_t a = 0x9908b0dfUL;
    const uint32_t b = 0x9d2c5680UL;
    const uint32_t c = 0xefc60000UL;
    const uint32_t upper_mask = 0x80000000UL;
    const uint32_t lower_mask = 0x7fffffffUL;
    uint32_t mt[p];
    int ind;

    void twist() 
    {
        for (int i = 0; i < p; ++i) 
        {
            uint32_t x = (mt[i] & upper_mask) + (mt[(i + 1) % p] & lower_mask);
            uint32_t xA = x >> 1;
            if (x & 1) 
            {
                xA ^= a;
            }
            mt[i] = mt[(i + q) % p] ^ xA;
        }
        ind = 0;
    }
};

bool check_lc_conditions(int m, int a, int c)
{
    if (gcd(c, m) != 1)
    {
        return true;
    }

    int a1 = a - 1;

    if (m % 4 == 0)
    {
        if (a1 % 4 != 0)
        {
            return true;
        }
    }
    
    vector <int> prime_factors;
    int d = 2;
    while (m > 1) {
        while (m % d == 0) {
            prime_factors.push_back(d);
            m /= d;
        }
        d++;
        if (d * d > m) {
            if (m > 1) {
                prime_factors.push_back(m);
            }
            break;
        }
    }


    for (int p : prime_factors)
    {
        if (a1 % p == 0)
        {
            return false;
        }
    }
    return true;


}

void lc(int m, int a, int c, int x0, int n, string file_name)
{
    ofstream outFile(file_name);

    if (check_lc_conditions(m, a, c))
    {
        cout << "\nУсловия теоремы не выполнены.\n\n";
    }


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

    dynamic_bitset<> reg(bit_size, seed);
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


void p5(int p, int q1, int q2, int q3, int w, unsigned long long seed, int n, string file_name)
{
    ofstream outFile(file_name);

    dynamic_bitset<> reg(p, seed); 
    dynamic_bitset<> res_reg(w, 0);

    cout << "Прогресс генерации ПСЧ: \n";
    int step = n / 10;
   
    for (size_t i = 0; i < n; i++)
    {
        //if (i % step == 0) {
        //    cout << '\r' << flush;
        //    cout << "  * Выполнено " << (i * 100) / n << "%";
        //}
        bool new_bit = false;
        for (size_t b = 0; b < w; b++)
        {
            new_bit = reg[q1] ^ reg[q2] ^ reg[q3] ^ reg[0];
            reg >>= 1;
            reg.set(p - 1, new_bit);
            res_reg.set(w - 1 - b, new_bit);
        }

        unsigned int xn = static_cast<unsigned int>(res_reg.to_ulong());
        outFile << xn << ',';
    }

    cout << '\r' << flush;
    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name << "\n";

    outFile.close();
}


void nfsr(string scoefs1, string scoefs2, string scoefs3, int seed1, int seed2, int seed3, int w, int n, string file_name)
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

    dynamic_bitset<> res_reg(w, 0);

    cout << "Прогресс генерации ПСЧ: \n";
    const int step = n / 10;

    for (size_t i = 0; i < n; i++)
    {
        if (i % step == 0) {
            cout << '\r' << flush;
            cout << "  * Выполнено " << (i * 100) / n << "%";
        }

        for (size_t b = 0; b < w; b++)
        {

            // R1
            bool bit1 = false;
            for (size_t j = 0; j < bit_size1; j++)
            {
                if (coefs1[j])
                {
                    bit1 ^= reg1[j];
                }
            }
            reg1 >>= 1;
            reg1.set(bit_size1 - 1, bit1);

            // R2
            bool bit2 = false;
            for (size_t j = 0; j < bit_size2; j++)
            {
                if (coefs2[j])
                {
                    bit2 ^= reg2[j];
                }
            }
            reg2 >>= 1;
            reg2.set(bit_size2 - 1, bit2);

            // R3
            bool bit3 = false;
            for (size_t j = 0; j < bit_size3; j++)
            {
                if (coefs3[j])
                {
                    bit3 ^= reg3[j];
                }
            }
            reg3 >>= 1;
            reg3.set(bit_size3 - 1, bit3);

            res_reg.set(w - 1 - b, (bit1 & bit2) ^ (bit2 & bit3) ^ bit3);
        }
        
        unsigned int xn = static_cast<unsigned int>(res_reg.to_ulong());
        outFile << xn << ',';
    }

    cout << '\r' << flush;
    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name << "\n";

    outFile.close();
}


void mt(int m, int seed, int n, string file_name)
{
    ofstream outFile(file_name);

    cout << "Прогресс генерации ПСЧ: \n";
    int step = n / 10;

    MersenneTwister32 mt32(seed);
    for (size_t i = 0; i < n; i++)
    {
        if (i % step == 0) {
            cout << '\r' << flush;
            cout << "  * Выполнено " << (i * 100) / n << "%";
        }

        outFile << mt32.generate() % m << ',';
    }

    cout << '\r' << flush;
    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name << "\n";

    outFile.close();
}


void rc4(vector <int> k, int n, string file_name)
{
    ofstream outFile(file_name);

    cout << "Прогресс генерации ПСЧ: \n";
    int step = n / 10;

    int l = k.size();
    vector <int> s;
    for (size_t i = 0; i < l; i++)
    {
        s.push_back(i);
    }
    int j = 0;
    for (size_t i = 0; i < l; i++)
    {
        j = (j + s[i] + k[i]) % l;
        swap(s[i], s[j]);
    }

    int i = 0;
    int t;
    j = 0;
    for (size_t k = 0; k < n; k++)
    {
        if (k % step == 0) {
            cout << '\r' << flush;
            cout << "  * Выполнено " << (k * 100) / n << "%";
        }
        
        i = (i + 1) % l;
        j = (j + s[i]) % l;
        swap(s[i], s[j]);
        t = (s[i] + s[j]) % l;

        outFile << s[t] << ',';
    }

    cout << '\r' << flush;
    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name << "\n";

    outFile.close();
}


void rsa(int pq, int e, int x0, int w, int n, string file_name)
{
    ofstream outFile(file_name);

    dynamic_bitset<> z(w, 0);

    unsigned int xi = x0;

    cout << "Прогресс генерации ПСЧ: \n";
    const int step = n / 10;

    for (size_t i = 0; i < n; i++)
    {
        if (i % step == 0) {
            cout << '\r' << flush;
            cout << "  * Выполнено " << (i * 100) / n << "%";
        }

        bool new_bit = false;
        for (size_t j = 0; j < w; j++)
        {
            unsigned int x = xi;
            for (size_t d = 1; d < e; d++)
            {
                xi = xi * x % pq;
            }
            z.set(w - 1 - j, xi & 1);
        }

        unsigned int xn = static_cast<unsigned int>(z.to_ulong());
        outFile << xn << ',';
    }

    cout << '\r' << flush;
    cout << "  * Выполнено 100% \n" << "Результат генерации ПСЧ записан в " << file_name << "\n";

    outFile.close();
}


void bbs(int x0, int w, int n, string file_name)
{
    ofstream outFile(file_name);
    dynamic_bitset<> z(w, 0);

    const unsigned int pq = 16637;
    unsigned int xi = x0 * x0 % pq;

    cout << "Прогресс генерации ПСЧ: \n";
    const int step = n / 10;

    for (size_t i = 0; i < n; i++)
    {
        if (i % step == 0) {
            cout << '\r' << flush;
            cout << "  * Выполнено " << (i * 100) / n << "%";
        }

        bool new_bit = false;
        for (size_t j = 0; j < w; j++)
        {
            xi = xi * xi % pq;
            z.set(w - 1 - j, xi & 1);
            cout << (xi & 1) << " ";
        }

        unsigned int xn = static_cast<unsigned int>(z.to_ulong());
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
        .help("Методы генерации ПСЧ: lc, add, 5p, lfsr, nfsr, mt, rc4, rsa");

    parser.add_argument("/i")
        .help(R"(Инициализационный вектор генератора (параметры записываются через запятую).
                 * lc: m, a, c, x0    
                 * add: m, k, j, j начальных значений
                 * 5p: p, q1, q2, q3, w, начальное значение
                 * lfsr: двоичный вектор коэффициентов (до 32 бит), начальное значение регистра
                 * nfsr: три двоичных вектора коэффициентов (до 32 бит), начальные значения трёх регистров, битность w
                 * mt: модуль, начальное значение
                 * rc4: 256 начальных значений
                 * rsa: модуль n, число e, начальное значение x, битность w
                 * bbs: начальное значение x (взаимно простое с n = 16637), битность w)");

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

        if (method_code == "lfsr" or method_code == "nfsr" or method_code == "5p")
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
        p5(stoi(is_vec[0]), stoi(is_vec[1]), stoi(is_vec[2]), stoi(is_vec[3]), stoi(is_vec[4]), stoull(is_vec[5]), n, file_name);
    }
    else if (method_code == "nfsr")
    {
        nfsr(is_vec[0], is_vec[1], is_vec[2], stoi(is_vec[3]), stoi(is_vec[4]), stoi(is_vec[5]), stoi(is_vec[6]), n, file_name);
    }
    else if (method_code == "mt")
    {
        mt(i_vec[0], i_vec[1], n, file_name);
    }
    else if (method_code == "rc4")
    {
        rc4(i_vec, n, file_name);
    }
    else if (method_code == "rsa")
    {
        rsa(i_vec[0], i_vec[1], i_vec[2], i_vec[3], n, file_name);
    }
    else if (method_code == "bbs")
    {
        bbs(i_vec[0], i_vec[1], n, file_name);
    }


    return 0;

}


