/**
 * @file BachelorProject.cpp
 * @brief Головний файл бакалаврського проєкту для математичного моделювання.
 * * @details Цей файл містить реалізацію математичної моделі, яка генерує
 * шумові сигнали (навантаження, газ, вплив) та виконує їх згортку з
 * відповідними ваговими функціями.
 * * **Архітектурні рішення:**
 * Програма працює як монолітний CLI-додаток. Результати серіалізуються
 * безпосередньо у файлову систему (`tpar.rez`).
 * * **Бізнес-логіка:**
 * Розрахунок середнього значення (mpar) та середньоквадратичного
 * відхилення (sig_tpar) на основі згенерованих випадкових вибірок.
 * * @author Іващев Родіон
 * @date 2026
 */

#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>

 /** * @def N
  * @brief Розмірність масивів для зберігання вибірок сигналів та вагових функцій.
  */
const int N = 60;

/**
 * @brief Обчислює дискретну згортку двох сигналів.
 * * @details **Алгоритм (Важкий для розуміння):**
 * Функція виконує операцію дискретної згортки (convolution) вхідного сигналу `x`
 * з ваговою функцією `w`. Елементи масиву `x` беруться у зворотному порядку
 * `x[n - 1 - i]`, що імітує "перевертання" сигналу на осі часу, як це вимагається
 * в класичній цифровій обробці сигналів (DSP).
 * * @param x Масив вхідного сигналу (шуму).
 * @param w Масив вагової функції (імпульсної характеристики).
 * @param n Кількість елементів для обробки (зазвичай N).
 * @return float Результат згортки (одне число) для поточної ітерації.
 */
float svertka(const float x[], const float w[], int n);

//*************************************************************

/**
 * @brief Головна точка входу в програму.
 * * @details **Взаємодія компонентів:**
 * 1. Зчитування кількості ітерацій `NN` від користувача.
 * 2. Ініціалізація експоненційних вагових функцій (`wW`, `wNAGR`, `wGAZ`).
 * 3. Головний цикл Монте-Карло: генерація гауссівського шуму методом
 * центральної граничної теореми (додавання 12 рівномірно розподілених
 * випадкових чисел) та їхня подальша згортка.
 * 4. Запис результатів у файл `tpar.rez`.
 * * @warning Якщо дисперсія (`Dtpar`) виявиться від'ємною (що теоретично неможливо
 * через формулу, але можливо через похибки float), програма видасть помилку.
 * * @return int Код завершення програми (0 - успіх).
 */
int main()
{
	FILE* p1;
	float sdNAGR[N], sdGAZ[N], sdW[N];
	float TW = 1, TNAGR = 2, TGAZ = 1, dt = 1, kW = 10, kNAGR = 20, kGAZ = 5, wW[N], wNAGR[N], wGAZ[N], sl;
	long NN, i, j;
	float sigma_W = 0.05f, sigma_NAGR = 0.05f, sigma_GAZ = 5.0f, stpar = 0.0f, stpar2 = 0.0f;
	float   dtpar, mtpar, Dtpar, sig_tpar;

	fopen_s(&p1, "tpar.rez", "a");
	puts("Input NN=");
	scanf_s("%li", &NN);

	for (i = 0; i < N; i++)
	{
		sdGAZ[i] = 0;
		sdNAGR[i] = 0;
		sdW[i] = 0;
	}

	for (i = 0; i < N; i++) {
		wW[i] = (kW / TW) * exp(-(float)i * dt / TW);
	}

	for (i = 0; i < N; i++) {
		wNAGR[i] = (kNAGR / TNAGR) * exp(-(float)i * dt / TNAGR);
	}
	for (i = 0; i < N; i++) {
		wGAZ[i] = (kGAZ / TGAZ) * exp(-(float)i * dt / TGAZ);
	}

	for (j = 0; j < NN - 1; j++)
	{
		for (i = 0; i < N - 1; i++)
		{
			sdGAZ[i] = sdGAZ[i + 1];
			sdNAGR[i] = sdNAGR[i + 1];
			sdW[i] = sdW[i + 1];
		}

		sl = 0;
		for (i = 0; i < 12; i++) {
			sl += (float)rand() / 32767.0f;
		}
		sdGAZ[N - 1] = sigma_GAZ * (sl - 6.0f);

		sl = 0;
		for (i = 0; i < 12; i++) {
			sl += (float)rand() / 32767.0f;
		}
		sdNAGR[N - 1] = sigma_NAGR * (sl - 6.0f);

		sl = 0;
		for (i = 0; i < 12; i++) {
			sl += (float)rand() / 32767.0f;
		}
		sdW[N - 1] = sigma_W * (sl - 6.0f);

		dtpar = svertka(sdGAZ, wGAZ, N) + svertka(sdNAGR, wNAGR, N) + svertka(sdW, wW, N);
		stpar += dtpar;
		stpar2 += (dtpar * dtpar);
	}

	mtpar = stpar / (float)NN;
	Dtpar = (stpar2 - stpar * stpar / (float)NN) / (float)(NN - 1);

	if (Dtpar >= 0)
	{
		sig_tpar = sqrt(Dtpar);
		printf("mpar=%f sig_tpar=%f\n", mtpar, sig_tpar);

		if (p1 != nullptr)
		{
			fprintf(p1, "NN=%li mpar=%f sig_tpar=%f\n", NN, mtpar, sig_tpar);
		}
	}
	else {
		puts("Dtpar<0\n");
	}
	puts("Finish!");
	return 0;
}

//**********************************************
float svertka(const float x[], const float w[], int n)
{
	float s = 0;
	int i;
	for (i = 0; i < n; i++) {
		s += w[i] * x[n - 1 - i];
	}
	return s;
}