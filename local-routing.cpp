// local-routing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdlib.h"
#include "malloc.h"
#include "time.h"
#include "math.h"
#include "iostream"
#include "queue"
#include "list"
#include "stack"
using namespace std;
#define MAXN1 1000
#define MAXN2 5000
#define MAXN3 20000


int N;        //����Ľ����
//int M;        //�����������

typedef struct linked_list
{
	int node_num;        //�����        
	int edge_num;        //���߱��
	struct linked_list *next;
} Linked_List;
Linked_List *adjacency_list[MAXN1];
int edge_count = 0;

typedef struct node
{
	int degree;
	double weight;
	queue<int> Q;         //����
} Node;
Node node[MAXN1];

typedef struct packet
{
	int destination;
	int node;
	bool moved;
	bool visited[MAXN2];
} Packet;
Packet packet[MAXN3];


int R;               //��λʱ���ڲ�����������
int capacity;        //ÿ����������
double alpha;        //ϵ��
double H;
int time_final;      //����ʱ��
int time1;           //ȡ��ʱ��1
int time2;           //ȡ��ʱ��2
int Np;              //�����а�������
int Np1;             //��ȡ��ʱ��1��������
int Np2;             //��ȡ��ʱ��2��������
stack<int> packet_free;      //������ſ��еİ����
list<int> packet_busy;       //�����������ʹ�õİ����


//�������������������
void network_connect(int node1, int node2)
{
	Linked_List *p;
	p = (Linked_List*)malloc(sizeof(Linked_List));
	p->node_num = node2;
	p->edge_num = edge_count;
	p->next = adjacency_list[node1];
	adjacency_list[node1] = p;
	p = (Linked_List*)malloc(sizeof(Linked_List));
	p->node_num = node1;
	p->edge_num = edge_count;
	p->next = adjacency_list[node2];
	adjacency_list[node2] = p;
	edge_count++;
}


//����lattice���� 
void create_lattice()
{
	int i, j;
	int size;
	printf("������߳���\n");
	scanf("%d", &size);
//	size = 35;
	N = size*size;
	srand((unsigned)time(NULL));
	for (i = 0; i<size - 1; i++)
		for (j = 0; j<size - 1; j++)
		{
			network_connect(i*size + j, i*size + j + 1);
			node[i*size + j].degree++;
			node[i*size + j + 1].degree++;
			network_connect(i*size + j, i*size + j + size);
			node[i*size + j].degree++;
			node[i*size + j + size].degree++;
		}
	for (i = 0; i<size - 1; i++)
	{
		network_connect(size*(size - 1) + i, size*(size - 1) + i + 1);
		node[size*(size - 1) + i].degree++;
		node[size*(size - 1) + i + 1].degree++;
		network_connect(i*size + size - 1, i*size + 2 * size - 1);
		node[i*size + size - 1].degree++;
		node[i*size + 2 * size - 1].degree++;
	}
	for (i = 0; i < size; i++)
	{
		network_connect(i, i + size*(size - 1));
		node[i].degree++;
		node[i + size*(size - 1)].degree++;
		network_connect(i*size, i*size + size - 1);
		node[i*size].degree++;
		node[i*size + size - 1].degree++;
	}
}


//����BA����
void create_BA()
{
	int i, j, k;
	int n0, nt, nc;       //ԭʼ����������ӵĽ�����������ӵĽ����ԭ����������
//	printf("������ԭʼ����������ӵĽ�����������ӵĽ����ԭ������������\n");
//	scanf("%d%d%d", &n0, &nt, &nc);
	n0 = 11;
	nt = 989;
	nc = 5;
	N = n0 + nt;

	//��ʼ��BA����
	for (i = 0; i<n0; i++)
		for (j = 0; j < i; j++)
		{
			network_connect(i, j);
			node[i].degree++;
			node[j].degree++;
		}
			
	srand((unsigned)time(NULL));
	for (i = n0; i < N; i++)
	{
		bool connected[MAXN1] = { 0 };       //��ֹ�ظ�����
		int denominator = 0;       //�Ѵ��ڽ��Ķ���֮��
		for (j = 0; j < i; j++) denominator += node[j].degree;

		j = 0;
		while (j < nc)
		{
			double threshold, probability;    //��ֵ��������ɵĸ���
			bool flag;
			threshold = 0;
			flag = 1;
			probability = (rand() % 1000)*1.0 / 1000;
			k = 0;
			while (k < i&&flag == 1)
			{
				threshold += (node[k].degree * 1.0) / (denominator*1.0);
				if (probability < threshold)
				{
					flag = 0;
					if (connected[k] == 0)
					{
						network_connect(i, k);
						node[i].degree++;
						node[k].degree++;
						j++;
						connected[k] = 1;
					}
				}
				k++;
			}
		}
	}
}


//������Ľ�����洢���ļ���
void fprint_N()
{
	FILE *fp;
	fp = fopen("N.txt", "w");
	fprintf(fp, "%d", N);
	fclose(fp);
}


//��������ÿ�����Ķȴ洢���ļ���
void fprint_degree()
{
	int i;
	FILE *fp;
	fp = fopen("degree.txt", "w");
	for (i = 0; i < N; i++) fprintf(fp, "%d\n", node[i].degree);
	fclose(fp);
}


//�������ڽӱ�洢���ļ���
void fprint_adjacency_list()
{
	int i;
	FILE *fp;
	fp = fopen("adjacency_list.txt", "w");
	for (i = 0; i<N; i++)
	{
		Linked_List *p;
		p = adjacency_list[i];
		while (p != NULL)
		{
			fprintf(fp, "%d ", p->node_num);
			p = p->next;
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}


//���ļ��ж�ȡ����Ľ����
void fscan_N()
{
	FILE *fp;
	fp = fopen("N.txt", "r");
	fscanf(fp, "%d", &N);
	fclose(fp);
}


//���ļ��ж�ȡÿ�����Ķ�
void fscan_degree()
{
	int i;
	FILE *fp;
	fp = fopen("degree.txt", "r");
	for (i = 0; i < N; i++) fscanf(fp, "%d", &node[i].degree);
	fclose(fp);
}


//���ļ��ж�ȡ�����ڽӱ�
void fscan_adjacency_list()
{
	int i, j;
	FILE *fp;
	fp = fopen("adjacency_list.txt", "r");
	for (i = 0; i < N; i++)
		for (j = 0; j < node[i].degree; j++)
		{
			int node;
			fscanf(fp, "%d", &node);
			if (node < i) network_connect(i, node);
		}
	fclose(fp);
}


//�������
void print_adjacency_list()
{
	int i;
	Linked_List *p;
	printf("\n������磺\n");
	for (i = 0; i < N; i++)
	{
		p = adjacency_list[i];
		printf("%d: ", i);
		while (p != NULL)
		{
			printf("%d(%d) ", p->node_num, p->edge_num);
			p = p->next;
		}
		printf("\n");
	}
}


//�������йص���ֵ
void print_degree()
{
	int i;
	double degree_average;
//	printf("\n����������Ķȣ�\n");
//	for (i = 0; i < N; i++) printf("%d\n", degree[i]);
	degree_average = 0;
	printf("\n�����ƽ����\n");
	for (i = 0; i < N; i++) degree_average += node[i].degree * 1.0;
	degree_average /= N;
	printf("%lf\n", degree_average);
}


//���ĳһ�����Ĳ���
void print_packet(int packet_num)
{
	int i;
	printf("�յ㣺%d\n", packet[packet_num].destination);
	printf("��ǰ��%d\n", packet[packet_num].node);
	printf("�ѷ��ʹ��ıߣ�");
	for (i = 0; i < MAXN2; i++)
	{
		if (packet[packet_num].visited[i] == 1)
		{
			printf("%d ", i);
		}
	}
	printf("\n");
	printf("\n");
}


//�õ�ĳһ��������һ�����
int test_nextnode(int packet_num)
{
	double threshold = 0, probability;  //��ֵ��������ɵĸ���
	double denominator = 0;             //���ڽ��Ȩ��֮��
	int nextnode;
	bool flag = 1;
//	bool unfound = 1;
	nextnode = packet[packet_num].node;
	probability = (rand() % 1000)*1.0 / 1000;
	Linked_List *p;
	p = adjacency_list[packet[packet_num].node];
	while (p != NULL)
	{
		if (packet[packet_num].visited[p->edge_num] == 0)
		{
//			unfound = 0;
			denominator += node[p->node_num].weight;
		}	
		p = p->next;
	}
	p = adjacency_list[packet[packet_num].node];
	while (p != NULL&&flag == 1)
	{
		if (packet[packet_num].visited[p->edge_num] == 0)
			threshold += (node[p->node_num].weight*1.0) / denominator;
		if (probability < threshold)
		{
			flag = 0;
			nextnode = p->node_num;
			packet[packet_num].visited[p->edge_num] = 1;      //���Ѿ����ʹ��ı������
		}
		p = p->next;
	}
/*	if (unfound == 1)
	{
		printf("%d����ס��\n", packet_num);
		print_packet(packet_num);
	}*/
	return nextnode;
}


//��ÿһ�����н��и���
void test_fresh(int node_num)
{
	int i;
	Linked_List *p;
	bool flag;         //�ж��Ƿ񵽴��յ�ı�־
	int packet_num;
	for (i = 0; i < capacity; i++)
	{
		if (node[node_num].Q.empty()) break;
		if (packet[node[node_num].Q.front()].moved) break;
		packet_num = node[node_num].Q.front();
		node[node_num].Q.pop();
		
		//�ж��Ƿ��Ѿ������յ�
		p = adjacency_list[node_num];
		flag = 1;
		while (p != NULL&&flag == 1)
		{
			if (p->node_num == packet[packet_num].destination)
				flag = 0;
			p = p->next;
		}

		//�����һ�����յ�
		if (!flag)
		{
			Np--;
			packet_busy.remove(packet_num);
			packet_free.push(packet_num);
//			if (packet_num == 0) printf("****************************************\n\n****************************************\n\n");
		}

		//�����һ���㲻���յ�
		else
		{
			packet[packet_num].node = test_nextnode(packet_num);
			node[packet[packet_num].node].Q.push(packet_num);
			packet[packet_num].moved = 1;
//			if (packet_num == 127) print_packet(0);
		}
	}
}


//���в���
void test()
{
	int i, j, k;
	FILE *fp1 = fopen("H.txt", "w");
//	printf("\n���������ʱ�䣺\n");
//	scanf("%d", &time_final);
	time_final = 2000;
	printf("\n�����뵥λʱ�����²����İ���������\n");
	scanf("%d", &R);
//	printf("\n������ÿ������һ�γ��ӵİ�����\n");
//	scanf("%d", &capacity);
	capacity = 10;
//	printf("\n������۲��1�͹۲��2��\n");
//	scanf("%d%d", &time1, &time2);
	time1 = 1199;
	time2 = 1999;
//	printf("\n������Ȩ��ϵ����\n");
//	scanf("%lf", &alpha);
	alpha = -1;
	Np = 0;
	for (i = MAXN3; i >= 0; i--) packet_free.push(i);
	for (i = 0; i < N; i++) node[i].weight = pow(node[i].degree * 1.0, alpha);

	//ʱ�䲻���ƽ�
	for (i = 0; i < time_final; i++)
	{
//		printf("ʱ��Ϊ��%d\n", i);
		//��ʼ��
		for (list<int>::iterator iter = packet_busy.begin(); iter != packet_busy.end(); iter++)
			packet[*iter].moved = 0;
        
		//��ÿһ�����н��и���
		for (j = 0; j < N; j++)
			if (!node[j].Q.empty()) test_fresh(j);

		//�����µĲ��԰�
		for (j = 0; j < R; j++)
		{

			int source = 0, destination = 0;
			while (source == destination)
			{
				source = rand() % N;
				destination = rand() % N;
			}
			packet[packet_free.top()].destination = destination;
			packet[packet_free.top()].node = source;
			node[source].Q.push(packet_free.top());
			for (k = 0; k < MAXN2; k++) packet[packet_free.top()].visited[k] = 0;
//			if (packet_free.top() == 127) print_packet(0);
//			printf("%d ", packet_free.top());
			packet_busy.push_back(packet_free.top());
			packet_free.pop();
		}
		Np += R;

		//ȡ��
		if (i == time1) Np1 = Np;
		if (i == time2) Np2 = Np;
	}

	//�������
	H = capacity*1.0 / (R*1.0)*(Np2*1.0 - Np1*1.0) / (time2*1.0 - time1*1.0);

	//�������
	fprintf(fp1, "%lf\n", H);
	fclose(fp1);
}


int main()
{
//	create_lattice();
//	create_BA();
//	fprint_N();
//	fprint_degree();
//	fprint_adjacency_list();
	fscan_N();
	fscan_degree();
	fscan_adjacency_list();
//	print_adjacency_list();
//	print_degree();
	test();
    return 0;
}

