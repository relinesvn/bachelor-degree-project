// rodion1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>

const int N = 60;
float svertka(const float x[], const float w[], int n);
//*************************************************************
int main()
{
	FILE* p1;
	float sdNAGR[N], sdGAZ[N], sdW[N];//  
	//float TW=10,TNAGR=20,TGAZ=10,dt=1,kW=10,kNAGR=20,kGAZ=5,wW[N],wNAGR[N],wGAZ[N],sdI[N],sdH[N],sl;
	//float TW=1,TNAGR=2,TGAZ=1,dt=1,kW=10,kNAGR=20,kGAZ=5,wW[N],wNAGR[N],wGAZ[N],sdI[N],sdH[N],sl;
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
	//   wI[N]
	for (i = 0; i < N; i++) {
		wW[i] = (kW / TW) * exp(-(float)i * dt / TW);
	}
	//wH[N]
	for (i = 0; i < N; i++) {
		wNAGR[i] = (kNAGR / TNAGR) * exp(-(float)i * dt / TNAGR);
	}
	for (i = 0; i < N; i++) {
		wGAZ[i] = (kGAZ / TGAZ) * exp(-(float)i * dt / TGAZ);
	}
	//w2[i]=(k/(root*Ty))*exp(-i*dt*root/Ty)*sin(root*i*dt/Ty);
	for (j = 0; j < NN - 1; j++)//  
	{
		for (i = 0; i < N - 1; i++)//      N 
		{
			sdGAZ[i] = sdGAZ[i + 1];
			sdNAGR[i] = sdNAGR[i + 1];
			sdW[i] = sdW[i + 1];
		}
		sl = 0;
		for (i = 0; i < 12; i++) {
			sl += (float)rand() / 32767.0f;
		}
		sdGAZ[N - 1] = sigma_GAZ * (sl - 6.0f);// . 
		sl = 0;
		for (i = 0; i < 12; i++) {
			sl += (float)rand() / 32767.0f;
		}
		sdNAGR[N - 1] = sigma_NAGR * (sl - 6.0f);// . 
		sl = 0;
		for (i = 0; i < 12; i++) {
			sl += (float)rand() / 32767.0f;
		}
		sdW[N - 1] = sigma_W * (sl - 6.0f);//  .  

		//       3-  
		dtpar = svertka(sdGAZ, wGAZ, N) + svertka(sdNAGR, wNAGR, N) + svertka(sdW, wW, N);
		stpar += dtpar;
		stpar2 += (dtpar * dtpar);
	}
	// . .,   ...  . . .
	mtpar = stpar / (float)NN;
	Dtpar = (stpar2 - stpar * stpar / (float)NN) / (float)(NN - 1);
	if (Dtpar >= 0)
	{
		sig_tpar = sqrt(Dtpar);
		printf("mpar=%f sig_tpar=%f\n", mtpar, sig_tpar);

		// Перевіряємо, чи файл успішно відкрито, перш ніж писати в нього
		if (p1 != nullptr)
		{
			fprintf(p1, "NN=%li mpar=%f sig_tpar=%f\n", NN, mtpar, sig_tpar);
		}
	}
	else {
		puts("Dtpar<0\n");
	}
	puts("Finish!");
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