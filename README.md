# Теория псевдослучайных генераторов
## Задание 1. Тесты
Порядок входных параметров определяется требованиями, указанными в https://github.com/gurgutan/TPRG.

Для вывода помощи `prng.exe /h`.

### Линейный конгруэнтный метод
* `prng.exe /g:lc /i:31104,625,6571,23 /n:100000 /f:rnd_lc1.dat`
* `prng.exe /g:lc /i:134456,141,28411,997 /n:100000 /f:rnd_lc2.dat`

### Аддитивный 
* `prng.exe /g:add /i:30000,24,55,79,134,213,347,560,907,1467,2374,3841,6215,10056,16271,26327,12598,8925,21523,448,21971,22419,14390,6809,21199,28008,19207,17215,6422,23637,59,23696,23755,17451,11206,28657,9863,8520,18383,26903,15286,12189,27475,9664,7139,16803,23942,10745,4687,15432,20119,5551,25670,1221,26891,28112,23779,17506 /n:100000 /f:rnd_add.dat`

### Пятипараметрический метод
Параметры 
* `prng.exe /g:5p /i:89,7,13,24,32 /n:50000 /f:rnd_5p1.dat`
* `prng.exe /g:5p /i:35,7,13,11,16 /n:50000 /f:rnd_5p2.dat`

### РСЛОС
Вектор коэффициентов многочлена должен иметь длину не больше 32, начальное значение регистра задаётся в десятичной системе счисления.
* `prng.exe /g:lfsr /i:10011011010011010,17 /f:rnd_lfsr1.dat`

### Нелинейная комбинация РСЛОС
Порядок параметров: двоичные векторы коэффициентов для $R_1, R_2, R_3$, соответсвующие им начальные значения, битность w выходных значений.
* `prng.exe /g:nfsr /i:00000001010101,01011100000111101,010101001100000,97,1234,345231,10 /f:rnd_nfsr.dat`
* `prng.exe /g:nfsr /i:100000010110,101000111000001,11110010111001,239,127,9876,10`

### Вихрь Мерсенна
* `prng.exe /g:mt /i:1000,1234 /f:rnd_mt.dat`

### RC4
* `prng.exe /g:rc4 /i:213,968,838,64,355,214,212,36,695,139,897,518,656,956,810,510,985,105,670,8,907,951,685,989,222,931,169,286,289,556,731,902,688,701,771,533,990,630,708,884,255,683,25,214,792,348,34,758,9,781,946,580,615,955,585,5,886,563,81,38,809,444,619,222,544,53,635,621,630,251,497,257,2,467,897,790,728,676,722,838,465,781,10,828,903,235,857,841,146,719,681,678,961,652,491,38,256,909,251,21,110,811,273,25,642,286,489,478,184,812,770,846,241,141,266,500,375,827,633,761,154,663,461,206,529,212,667,342,360,165,523,749,582,803,553,345,786,990,361,702,256,380,234,238,73,965,266,300,847,755,969,681,146,843,125,306,845,752,879,458,788,833,727,817,122,239,765,877,827,327,733,658,644,880,150,474,493,689,670,368,611,263,113,417,834,103,725,754,117,824,623,338,540,337,879,521,183,370,808,120,571,871,301,210,796,744,398,106,845,745,842,876,399,27,105,601,802,831,53,266,157,352,175,303,505,484,994,425,292,729,654,584,860,420,412,49,281,417,703,400,48,404,772,389,733,152,271,585,404,333,381,696,928,609,659,180 /f:rnd_rc.dat `

### RSA
* `prng.exe /g:rsa /i:10967,571,77 /f:rnd_rsa.dat`

### Блюм-Блюма-Шуба
* `prng.exe /g:bbs /i:239 /f:rnd_bbs.dat`
