#include<stdio.h>
typedef struct
{
	char NW;
	char SW;
	char NE;
	char SE;
} Pairs;
enum{blue=-1, green=-2,red=-3};

void Fill_rand(double p, int n, int* cell);
int Fill_from_file(char* Filename, int n, int* cell, int type);
void print_matrix(FILE *f, int n, int* cell,int type);
void Fill_frame(int n, int* cell);
int Max_nnn_cell(int n, int* cell);
int Max_nn_cell(int n, int* cell,int m);
int Count_Black(int n, int* cell,int m);
void NULL_Pairs(Pairs *q, int n);
void Print_Pairs(FILE *f,Pairs *q, int n, int* cell,int type);
void Step1(int n, int* cell);
void Step2(FILE *f,int n, int* cell);
void Step3(FILE *f,Pairs *q, int* newcell,int n, int* cell);
void Step4(FILE *f,Pairs *q, int* newcell,int n, int* cell);
void Step5(FILE *f,Pairs *q, int* newcell,int n, int* cell);
void Step6(FILE *f,int n, int* cell);
int Step7(FILE *f,int n, int* cell);
void print_using(void);