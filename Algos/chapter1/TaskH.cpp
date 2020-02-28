#include <iostream>
#include <stdio.h>

typedef long long llu; 
llu ReadEnemies (const int& num, int* enemies);

llu BinSearch (llu left, llu right, const int* enemies, const int& num, const int& p, const int& q);

int main() {

    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

    int num = 0;
    int p = 0;
    int q = 0;

    std::cin >> num >> p >> q;

    int* enemies = new int[num];

    llu max = ReadEnemies (num, enemies) / p + 1; 

    llu hits = BinSearch (0, max, enemies, num, p, q);

    std::cout << hits;

    delete[] enemies;
    return 0;

}

llu ReadEnemies (const int& num, int* enemies) {

    llu sum = 0;

    for (int i = 0; i < num; ++i) {

        std::cin >> enemies[i];
        sum += enemies[i];

    }

    return sum;

}

llu BinSearch (llu left, llu right, const int* enemies, const int& num, const int& p, const int& q) {

    if (left == right)
        return right;

    llu mid = (left + right) / 2;

    llu add = 0;

    if (p != q)
        for (int i = 0; i < num; ++i) {

            llu mod = (enemies[i] - q*mid) % (p - q);
            add += ((enemies[i] - q*mid)/(p - q) > 0) ? (enemies[i] - q*mid)/(p -q) : 0;
            add += (mod > 0) ? 1 : 0;
        }

    else    
        for (int i = 0; i < num; ++i)
            if (enemies[i] - (q*mid) > 0)
                return BinSearch (mid + 1, right, enemies, num, p, q);

    if (add > mid)
        return BinSearch (mid + 1, right, enemies, num, p, q);

    else return BinSearch (left, mid, enemies, num, p, q); 

}

