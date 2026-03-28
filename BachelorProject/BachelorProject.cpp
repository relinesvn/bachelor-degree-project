/**
 * @file BachelorProject.cpp
 * @brief Оптимізована версія математичного модуля з APM моніторингом.
 */

#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <chrono>   // Для APM моніторингу
#include <random>   // Для оптимізованої генерації чисел
#include <cstring>  // Для std::memmove

const int N = 60;

float svertka(const float x[], const float w[], int n);

int main()
{
    FILE* p1;
    float sdNAGR[N], sdGAZ[N], sdW[N];
    float TW = 1, TNAGR = 2, TGAZ = 1, dt = 1, kW = 10, kNAGR = 20, kGAZ = 5, wW[N], wNAGR[N], wGAZ[N];
    long NN, i, j;
    float sigma_W = 0.05f, sigma_NAGR = 0.05f, sigma_GAZ = 5.0f, stpar = 0.0f, stpar2 = 0.0f;
    float dtpar, mtpar, Dtpar, sig_tpar;

    // Ініціалізація оптимізованого генератора випадкових чисел (Mersenne Twister)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> normal_dist(0.0f, 1.0f);

    fopen_s(&p1, "tpar.rez", "a");
    puts("Input NN (test sizes: 10000, 100000, 1000000)=");
    scanf_s("%li", &NN);

    for (i = 0; i < N; i++) {
        sdGAZ[i] = 0; sdNAGR[i] = 0; sdW[i] = 0;
    }

    for (i = 0; i < N; i++) {
        wW[i] = (kW / TW) * exp(-(float)i * dt / TW);
        wNAGR[i] = (kNAGR / TNAGR) * exp(-(float)i * dt / TNAGR);
        wGAZ[i] = (kGAZ / TGAZ) * exp(-(float)i * dt / TGAZ);
    }

    // --- ПОЧАТОК APM МОНІТОРИНГУ ПРОДУКТИВНОСТІ ---
    auto start_time = std::chrono::high_resolution_clock::now();

    for (j = 0; j < NN - 1; j++)
    {
        // Оптимізація пам'яті: використання memmove замість циклу for (швидше копіювання блоків)
        std::memmove(sdGAZ, sdGAZ + 1, (N - 1) * sizeof(float));
        std::memmove(sdNAGR, sdNAGR + 1, (N - 1) * sizeof(float));
        std::memmove(sdW, sdW + 1, (N - 1) * sizeof(float));

        // Оптимізація алгоритму: швидка генерація за допомогою std::normal_distribution
        sdGAZ[N - 1] = sigma_GAZ * normal_dist(gen);
        sdNAGR[N - 1] = sigma_NAGR * normal_dist(gen);
        sdW[N - 1] = sigma_W * normal_dist(gen);

        dtpar = svertka(sdGAZ, wGAZ, N) + svertka(sdNAGR, wNAGR, N) + svertka(sdW, wW, N);
        stpar += dtpar;
        stpar2 += (dtpar * dtpar);
    }

    // --- КІНЕЦЬ APM МОНІТОРИНГУ ---
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> execution_time = end_time - start_time;

    mtpar = stpar / (float)NN;
    Dtpar = (stpar2 - stpar * stpar / (float)NN) / (float)(NN - 1);

    if (Dtpar >= 0)
    {
        sig_tpar = sqrt(Dtpar);
        printf("\n--- PERFORMANCE REPORT (APM) ---\n");
        printf("Iterations (NN): %li\n", NN);
        printf("Execution time: %.2f ms\n", execution_time.count());
        printf("Speed: %.0f iter/sec\n", (NN / execution_time.count()) * 1000);
        printf("--------------------------------\n");
        printf("Result: mpar=%f sig_tpar=%f\n", mtpar, sig_tpar);

        if (p1 != nullptr) {
            fprintf(p1, "[APM: %.2f ms] NN=%li mpar=%f sig_tpar=%f\n", execution_time.count(), NN, mtpar, sig_tpar);
        }
    }
    else {
        puts("Dtpar<0\n");
    }
    puts("Finish!");
    return 0;
}

float svertka(const float x[], const float w[], int n)
{
    float s = 0;
    // Оптимізація: розгортання циклу (Loop Unrolling) компілятором буде ефективнішим
    for (int i = 0; i < n; i++) {
        s += w[i] * x[n - 1 - i];
    }
    return s;
}