#pragma once
/*

*/

#include <array>
#include <cstdint>
#include <iostream>

/*-----

----*/

namespace ss

{
    constexpr int N_NID_2 = 3;   // N of possible values
    constexpr int N_NID_1 = 336; // N of possible values
    constexpr int SS_LEN = 127;

    using PssTable = std::array<std::array<int8_t, SS_LEN>, N_NID_2>;
    using SssTable = std::array<std::array<std::array<int8_t, SS_LEN>, N_NID_1>, N_NID_2>;

    constexpr PssTable init_pss()
    {
        std::array<uint8_t, SS_LEN> x{0, 1, 1, 0, 1, 1, 1};
        for (int i = 0; i < SS_LEN - 7; ++i)
            x[i + 7] = x[i + 4] ^ x[i];

        PssTable d{};
        for (int k = 0; k < N_NID_2; ++k)
        {
            for (int n = 0; n < SS_LEN; ++n)
                d[k][n] = 1 - 2 * x[(n + 43 * k) % SS_LEN];
        }
        return d;
    }

    constexpr SssTable init_sss()
    {
        std::array<uint8_t, SS_LEN> x0{1, 0, 0, 0, 0, 0, 0};
        std::array<uint8_t, SS_LEN> x1{1, 0, 0, 0, 0, 0, 0};
        for (int i = 0; i < SS_LEN - 7; ++i)
        {
            x0[i + 7] = x0[i + 4] ^ x0[i];
            x1[i + 7] = x1[i + 1] ^ x1[i];
        }

        SssTable d{};
        for (int k = 0; k < N_NID_2; ++k)
        { // NID_2
            for (int j = 0; j < N_NID_1; ++j)
            { // NID_1
                const int m0 = 15 * (j / 112) + 5 * k;
                const int m1 = j % 112;
                for (int n = 0; n < SS_LEN; ++n)
                {
                    d[k][j][n] = (1 - 2 * x0[(n + m0) % SS_LEN]) *
                                 (1 - 2 * x1[(n + m1) % SS_LEN]);
                }
            }
        }
        return d;
    }

    inline constexpr PssTable PSS = init_pss();
    inline constexpr SssTable SSS = init_sss();

    static_assert(ss::PSS[0].size() == 127);
    static_assert(ss::SSS[0][0].size() == 127);
}