    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);


//============================================
    void dump () {

        for (int i = 0; i < size; ++i) {

            std::cout << "i: " << i << '\n';
            for (int j = 0; j < size; j++) {
                std::cout << '\t' << "j: " << j << '\n';

                for (int k = 0; k < size; k++) 
                    std::cout << data[i][j][k] << ' ';

                std::cout << '\n';
            }

            std::cout << '\n';

        }

    }

//===========================================
// TaskA:

void dump () {

    std::cout << "dataZero: " << dataZero << '\n';

    for (int i = 0; i < tree.size(); ++i)
        std::cout << i << ':' << tree[i] << '\n';

}

