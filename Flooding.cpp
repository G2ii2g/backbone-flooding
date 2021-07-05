#define _CRT_SECURE_NO_WARNINGS
#include "All_functions.h"
#include<stdlib.h>
#include <cstring>
#include <ctime>
using namespace std;
void Fill_rand(double p, int n, int* cell)
{
	double max_gr=p*RAND_MAX;
	for(int i=0;i<n;i++)
	{	
		for(int j=0;j<n;j++) 
			cell[i*n+j]=rand()<max_gr;
	}
	Fill_frame(n,cell);
}
int Fill_from_file(char* Filename, int n, int* cell, int type/* type-тип чтения: 0-читать все, 1-без одного внешнего слоя, 2-без двух внешних слоев*/)
{
	int k=0;
	FILE *f1=fopen(Filename,"r");
	if(!f1)
	{
		printf("Cannot open input file:%s\n",Filename);
		print_using();
		exit(-1);
	}
	for(int i=type;i<n-type;i++)
	{	
		for(int j=type;j<n-type;j++) 
			fscanf(f1,"%d", &cell[i*n+j]),k+=cell[i*n+j];//подсчет количества 1 в кластере
	}
	fclose(f1);
	if(type==1||type==2)
		Fill_frame(n,cell);
	if(type==1||type==0)
		k-=2*(n-4);
	return k;
}
void print_matrix(FILE *f, int n, int* cell,int type)
{
	for(int i=type;i<n-type;i++)
	{	
		for(int j=type;j<n-type;j++) 
			fprintf(f,"%7d ", cell[i*n+j]);
		fprintf(f,"\n");
	}
}
void Fill_frame(int n, int* cell)
{
	for(int i=0;i<n;i++) 
		cell[i*n]=cell[i*n+1]=cell[(i+1)*n-1]=cell[(i+1)*n-2]=0;
	for(int j=2;j<n-2;j++) 
	{
		cell[j]=cell[n*(n-1)+j]=0;
		cell[n+j]=cell[n*(n-2)+j]=1;
	}
}
int Max_nnn_cell(int n, int* cell,int m)
{
	int max;
	max=cell[m];
	if(max<cell[m-n-1])
		max=cell[m-n-1];
	if(max<cell[m-n])
		max=cell[m-n];
	if(max<cell[m-n+1])
		max=cell[m-n+1];
	if(max<cell[m-1])
		max=cell[m-1];
	if(max<cell[m+1])
		max=cell[m+1];
	if(max<cell[m+n-1])
		max=cell[m+n-1];
	if(max<cell[m+n])
		max=cell[m+n];
	if(max<cell[m+n+1])
		max=cell[m+n+1];
	return max;
}
int Max_nn_cell(int n, int* cell,int m)
{
	int max;
	max=cell[m];
	if(max<cell[m-n])
		max=cell[m-n];
	if(max<cell[m-1])
		max=cell[m-1];
	if(max<cell[m+1])
		max=cell[m+1];
	if(max<cell[m+n])
		max=cell[m+n];
	
	return max;
}
int Count_Black(int n, int* cell,int m)//считать количество черных nn-соседей 
{
	int black=0;
	if(1==cell[m-n])
		black++;
	if(1==cell[m-1])
		black++;
	if(1==cell[m+1])
		black++;
	if(1==cell[m+n])
		black++;
	return black;
}
int Count_color(int n, int* cell,int m, int color)//считать количество color nn-соседей 
{
	int k=0;//количество соседей цвета color
	if(color==cell[m-n])
		k++;
	if(color==cell[m-1])
		k++;
	if(color==cell[m+1])
		k++;
	if(color==cell[m+n])
		k++;
	return k;
}
void Print_Pairs(FILE *f,Pairs *q, int n, int* cell,int type)
{
	for(int i=type;i<n-type;i++) 
	{
		for(int j=type;j<n-type;j++)
		{	int m=i*n+j;
			if(q[m].NE||q[m].NW||q[m].SE||q[m].SW)
				fprintf(f,"%6d",blue);
			else 
				fprintf(f,"%6d", cell[m]);
		}
		fprintf(f,"\n");
	}
}

void Step1(int n, int* cell)
{
	for(int i=1;i<n-1;i++) 
		for(int j=1;j<n-1;j++) 
		{
			int m=i*n+j;
			if(cell[m]==0&&cell[n+m]==1&&cell[m+1]==1&&cell[n+m+1])
				cell[m]=m;
		}
	cell[(n-2)*n+1]=(n-2)*n+1;
	cell[(n-1)*n-2]=(n-1)*n-2;
}
void Step2(FILE *f, int n, int* cell)
{
	int flag=1; 
	while(flag)
	{
		flag=0;
		for(int i=n-2;i>0;i--) //заливка снизу
			for(int j=n-2;j>0;j--) //заливка справа
			{
				int m=i*n+j;
				if(cell[m]!=1)
				{
					int max=Max_nnn_cell(n,cell,m);
					if(max>cell[m]&&max>1)
					{
						flag++;
						cell[m]=max;
					}
				}
			}
		if(cell[n*(n-2)+1]==cell[(n-1)*n-2])
			break;
	}
}
void Step3(FILE *f,Pairs *q, int* newcell,int n, int* cell)
{
	Fill_frame(n,newcell);
	NULL_Pairs(q,n);
	for(int i=2;i<n-2;i++) 
		for(int j=1;j<n-1;j++) 
		{
			int m=i*n+j;  
			if(cell[m]==1)
			{
				int b_n=Count_Black(n,cell,m);
				int max=Max_nnn_cell(n,cell,m);
				int max1=Max_nn_cell(n,cell,m);
				if(b_n<=1||
					b_n==2&&(max==cell[m-1]&&max==cell[m+1]||
					max==cell[m-n]&&max==cell[m+n]||
					cell[m-1]==max&&cell[m+n]==max&&cell[m-n+1]==max||
					cell[m+1]==max&&cell[m+n]==max&&cell[m-n-1]==max||
					cell[m+1]==max&&cell[m-n]==max&&cell[m+n-1]==max||
					cell[m-1]==max&&cell[m-n]==max&&cell[m+n+1]==max)||
					b_n==3&&(cell[m-1]==max&&cell[m+n+1]==max&&cell[m-n+1]==max||
					cell[m+n]==max&&cell[m-n-1]==max&&cell[m-n+1]==max||
					cell[m+1]==max&&cell[m-n-1]==max&&cell[m+n-1]==max||
					cell[m-n]==max&&cell[m+n-1]==max&&cell[m+n+1]==max))
					newcell[m]=max;

				else if(b_n==3&&(cell[m-1]==max&&cell[m+n+1]==max&&cell[m-n+1]!=max||
					cell[m+n]==max&&cell[m-n+1]==max&&cell[m-n-1]!=max||
					cell[m+1]==max&&cell[m-n-1]==max&&cell[m+n-1]!=max||
					cell[m-n]==max&&cell[m+n-1]==max&&cell[m+n+1]!=max||
					
					cell[m-1]==max&&cell[m-n+1]==max&&cell[m+n+1]!=max||
					cell[m+n]==max&&cell[m-n-1]==max&&cell[m-n+1]!=max||
					cell[m+1]==max&&cell[m+n-1]==max&&cell[m-n-1]!=max||  
					cell[m-n]==max&&cell[m+n+1]==max&&cell[m+n-1]!=max||

					cell[m-1]==max1&&cell[m+n+1]==max1&&cell[m-n+1]!=max1||
					cell[m+n]==max1&&cell[m-n+1]==max1&&cell[m-n-1]!=max1||
					cell[m+1]==max1&&cell[m-n-1]==max1&&cell[m+n-1]!=max1||
					cell[m-n]==max1&&cell[m+n-1]==max1&&cell[m+n+1]!=max1||
					
					cell[m-1]==max1&&cell[m-n+1]==max1&&cell[m+n+1]!=max1||
					cell[m+n]==max1&&cell[m-n-1]==max1&&cell[m-n+1]!=max1||
					cell[m+1]==max1&&cell[m+n-1]==max1&&cell[m-n-1]!=max1||  
					cell[m-n]==max1&&cell[m+n+1]==max1&&cell[m+n-1]!=max1)||
					
					b_n==4&&(cell[m-n-1]!=1&&cell[m-n-1]==cell[m+n+1]&&(cell[m+n-1]!=cell[m-n-1]||cell[m-n+1]!=cell[m-n-1])||
					cell[m-n+1]!=1&&cell[m-n+1]==cell[m+n-1]&&(cell[m-n-1]!=cell[m-n+1]||cell[m+n+1]!=cell[m-n+1])))
						newcell[m]=0;
				else newcell[m]=cell[m];
				if(b_n==3)
				{
					if(cell[m-1]==max1&&cell[m+n+1]==max1&&cell[m-n+1]!=max1||//оригинальная ориентация d
					cell[m+n]==max1&&cell[m-n-1]==max1&&cell[m-n+1]!=max1)
						q[m+1].NW=q[m-n].SE=1;
					else if(cell[m+n]==max1&&cell[m-n+1]==max1&&cell[m-n-1]!=max1||//-90 градусов
					cell[m+1]==max1&&cell[m+n-1]==max1&&cell[m-n-1]!=max1)  
						q[m-1].NE=q[m-n].SW=1;
					else if(cell[m+1]==max1&&cell[m-n-1]==max1&&cell[m+n-1]!=max1||//-180 градусов 
					cell[m-n]==max1&&cell[m+n+1]==max1&&cell[m+n-1]!=max1)
						q[m-1].SE=q[m+n].NW=1;
					else if(cell[m-n]==max1&&cell[m+n-1]==max1&&cell[m+n+1]!=max1||//-270 градусов 
					cell[m-1]==max1&&cell[m-n+1]==max1&&cell[m+n+1]!=max1)
						q[m+1].SW=q[m+n].NE=1;
				}
				else if(b_n==4)
				{ 
					if(cell[m-n-1]!=1&&cell[m-n-1]==cell[m+n+1])
					{
						if(cell[m+n-1]!=cell[m-n-1])
							q[m-1].SE=q[m+n].NW=1;
						if(cell[m-n+1]!=cell[m-n-1])
							q[m+1].NW=q[m-n].SE=1;
					}
					else if(cell[m-n+1]!=1&&cell[m-n+1]==cell[m+n-1])
					{
						if(cell[m-n-1]!=cell[m-n+1])
							q[m-1].NE=q[m-n].SW=1;
						if(cell[m+n+1]!=cell[m-n+1])
							q[m+1].SW=q[m+n].NE=1;
					}
				}
			}
			else newcell[m]=cell[m];
		}
	newcell[n+1]=cell[n+1];
	newcell[2*n-2]=cell[2*n-2];
	newcell[(n-2)*n+1]=cell[(n-2)*n+1];
	newcell[(n-1)*n-2]=cell[(n-1)*n-2];
}
void Step4(FILE *f,Pairs *q,int n, int* cell)
{
	int flag=1; //-2-зеленый
	while(flag)
	{
		flag=0;
		for(int i=2;i<n-2;i++) 
			for(int j=2;j<n-2;j++) 
			{
				int m=i*n+j;
				if(cell[m]==1)
				{
					if(i==(n-3)||i==2||cell[m-n]==green||cell[m-1]==green||cell[m+n]==green||cell[m+1]==green||
						cell[m-n-1]==green&&q[m].NW||cell[m+n-1]==green&&q[m].SW||cell[m+n+1]==green&&q[m].SE||cell[m-n+1]==green&&q[m].NE)
					{
						cell[m]=green;
						flag++;
					}
				}
			}
	}
}
void Step5(FILE *f,Pairs *q, int* newcell,int n, int* cell)
{
	for(int i=2;i<n-2;i++) 
		for(int j=2;j<n-2;j++) 
		{
			int m=i*n+j;  
			if(cell[m]==0&&(cell[m-n]==green||cell[m-1]==green||cell[m+n]==green||cell[m+1]==green))
				newcell[m]=green;
			else
				newcell[m]=cell[m];
		}
}
void Step6(FILE *f,int n, int* cell)
{
	int flag=1;
	while(flag)
	{
		flag=0;
		for(int i=n-3;i>1;i--) 
			for(int j=n-3;j>1;j--) 
			{
				int m=i*n+j;
				if(cell[m]==green&&(cell[m-n]==blue||cell[m-1]==blue||cell[m+n]==blue||cell[m+1]==blue||i==(n-3)))
				{
					cell[m]=blue;
					flag++;
				}
			}
	}
}
int Step7(FILE *f,int n, int* cell)
{
	int flag=1,k=0;
	while(flag)
	{
		flag=0;
		for(int i=2;i<n-2;i++) 
			for(int j=2;j<n-2;j++) 
			{
				int m=i*n+j;
				if(cell[m]==blue&&(cell[m-n]==red||cell[m-1]==red||cell[m+n]==red||cell[m+1]==red||i==2))
				{
					cell[m]=red;
					flag++;
				}
			}
		k+=flag;
	}
	return k;
}
void NULL_Pairs(Pairs *q, int n)
{
	for(int i=0;i<n;i++) 
		for(int j=0;j<n;j++) 
		{
			q[i*n+j].NW=0;
			q[i*n+j].SW=0;
			q[i*n+j].NE=0;
			q[i*n+j].SE=0;
		}
}
void print_using(void)
{
	printf("Using:\n");
	printf("TrobecPosl [type [infile [size [outtype [outfile]]]]]\n");
	printf("type is integer 0,1 or 2\n");
	printf("  0-with border and bus\n");
	printf("  1-with bus only\n");
	printf("  2-without border and bus\n");
	printf("  default: 0\n");
	printf("infile is input filename\n");
	printf("  default: input.txt\n");
	printf("size is positive integer>4 - the full matrix size with border and bus\n");
	printf("  if type=0 then size=number of infile lines\n");
	printf("  if type=1 then size=number of infile lines+2\n");
	printf("  if type=2 then size=number of infile lines+4\n");
	printf("  default: 23\n");
	printf("outtype is integer 0,1 or 2, mean the same as type or -1 means to write nothing\n");
	printf("  default: type\n");
	printf("outfile is output filename\n");
	printf("  default: output.txt\n");
	printf("Example:\n");
	printf("  TrobecPosl 2 input_256.txt 260 2 output_256.txt\n");
}
int main(int argc, char** argv)
{
	int n=23;
	double p=0.61;//вероятность заполнения
	int type=0, outtype=0;
	char* inFile="input.txt";
	char* outFile="output.txt";
	char* repFile="report.txt";
	int q_black,q_backbone;//количество черных ячеек и ячеек остова
	if(argc>1)
	{
		sscanf(argv[1],"%d",&type);
		if(type<0||type>2)
		{
			print_using();
			return 1;
		}
		outtype=type;
		if(argc>2)
		{
			inFile=argv[2];
			if(argc>3)
			{	
				sscanf(argv[3],"%d",&n);
				if(n<5)
				{
					print_using();
					return 1;
				}
				if(argc>4)
				{
					sscanf(argv[4],"%d",&outtype);
					if(outtype<-1||outtype>2)
					{
						print_using();
						return 1;
					}
					if(argc>5)
						outFile=argv[5];
				}
			}
		}
	}
	int *cell=new int[n*n];//ячейка
	int *cell1=new int[n*n];//ячейка
	Pairs *q=new Pairs[n*n];
	FILE *f2;
	if(outtype>=0)
		f2=fopen(outFile,"w+");
//	Fill_rand(p,n,cell);
	q_black=Fill_from_file(inFile,n,cell,type);
//	print_matrix(f2,n,cell,outtype);
//	print_matrix(f2,n,cell,1);
//	print_matrix(f2,n,cell,2);
	long t1=clock();

	Step1(n,cell);
	if(outtype>=0)
	{
		fprintf(f2,"After Step1\n");
		print_matrix(f2,n,cell,outtype); 
	}
	Step2(f2,n,cell);
	if(outtype>=0)
	{
		fprintf(f2,"After Step2\n");
		print_matrix(f2,n,cell,outtype);
	}
	if(cell[(n-2)*n+1]!=cell[(n-1)*n-2])
	{
		Step3(f2,q,cell1,n,cell);
		if(outtype>=0)
		{
			fprintf(f2,"After Step3\n");
			print_matrix(f2,n,cell1,outtype);
			fprintf(f2,"After Step3 part2\n"); 
			Print_Pairs(f2,q,n,cell1,outtype);
		}
		Step4(f2,q,n,cell1);
		if(outtype>=0)
		{
			fprintf(f2,"After Step4\n");
			print_matrix(f2,n,cell1,outtype);
		}
		Step5(f2,q,cell,n,cell1);
		if(outtype>=0)
		{
			fprintf(f2,"After Step5\n");
			print_matrix(f2,n,cell,outtype);
		}	
		Step6(f2,n,cell);
		if(outtype>=0)
		{
			fprintf(f2,"After Step6\n");
			print_matrix(f2,n,cell,outtype);
		}
		q_backbone=	Step7(f2,n,cell);
		//fprintf(f2,"With border and bus\n");
		if(outtype>=0)
		{
			fprintf(f2,"After Step7\n");
			print_matrix(f2,n,cell,outtype);
		}
	}
	else
		q_backbone=0;
	//fprintf(f2,"With border only\n");
	//print_matrix(f2,n,cell,1);
	//fprintf(f2,"Without border and bus\n");
	//print_matrix(f2,n,cell,2);
	delete[] cell;
	delete[] cell1;
	delete[] q;
	long t2=clock();
	printf("Time = %f\n",double(t2-t1)/CLOCKS_PER_SEC);
	if(outtype>=0)
		fclose(f2);
	FILE *f3;
	f3=fopen(repFile,"a+");
	int e=fseek(f3,0,SEEK_END);
	if(ftell(f3)==0)
		fprintf(f3,"Filename L Black_c Backbone_c Black_c/Backbone_c Backbone_c/Black_c Black_c/FULLSIZE Backbone_c/FULLSIZE Time\n");
	double u,u1;
	if(q_backbone>0)
		 u=q_black/(double)q_backbone;
	else
		u=0;	
	if(q_black>0)
		 u1=q_backbone/(double)q_black;
	else
		u1=0;
	fprintf(f3,"%s %d %d %6d %f %f %f %f %f\n",inFile,n-4,q_black,q_backbone,u,u1,q_black/(double)((n-4)*(n-4)),q_backbone/(double)((n-4)*(n-4)),double(t2-t1)/CLOCKS_PER_SEC);
	fclose(f3);
	
	return (q_backbone!=0);//1 есть узлы принадлежащие остову, 0 нет узлов принадлежащих остову 
}  