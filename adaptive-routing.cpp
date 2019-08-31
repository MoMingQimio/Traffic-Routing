// adaptive-routing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"
#include "iostream"
#include "queue"
#include "list"
#include "stack"
using namespace std;
#define MAXN1 1500
#define MAXN2 2000000


int N = 1500;
double velocity = 0.3;
double radius = 1.0;
int L = 10;      //cell�ı߳�
int capacity = 1;
int R;      //��λʱ���ڲ�����������
double h = 0.5;
int time_final;      //����ʱ��
int time1;      //ȡ��ʱ��1
int time2;      //ȡ��ʱ��2
int Np;      //�����а�������
int Np1;      //��ȡ��ʱ��1��������
int Np2;      //��ȡ��ʱ��2��������
double H;
stack<int> packet_free;      //������ſ��еİ����
list<int> packet_busy;      //�����������ʹ�õİ����

struct
{
	double x;
	double y;
	int load;      //���г���
	queue<int> Q;
} node[MAXN1];

struct
{
	int destination;
	int node;
	bool moved;
} packet[MAXN2];



//��������
void create_network()
{
	int i;
	srand((unsigned)time(NULL));
	for (i = 0; i < N; i++)
	{
		node[i].x = (rand() % 9999 + 1)*1.0 / 10000 * L;
		node[i].y = (rand() % 9999 + 1)*1.0 / 10000 * L;
	}
}


//������������ľ���
double calculate_distance(int i, int j)
{
	double distance;
	distance = sqrt((node[i].x - node[j].x)*(node[i].x - node[j].x) + (node[i].y - node[j].y)*(node[i].y - node[j].y));
	return distance;
}


//�ж�������������Ƿ���ͨ�Ű뾶��
bool within_scope(int i, int j)
{
	double distance;
	distance = sqrt((node[i].x - node[j].x)*(node[i].x - node[j].x) + (node[i].y - node[j].y)*(node[i].y - node[j].y));
	if (distance < radius) return 1;
	else return 0;
}


//�ж�ĳ������Ƿ���cell��
bool within_cell(double x, double y)
{
	if (x <= 0) return 0;
	if (x >= L) return 0;
	if (y <= 0) return 0;
	if (y >= L) return 0;
	return 1;
}


//������Ч����
double calculate_effective_distance(int i, int j)
{
	double effective_distance;
	effective_distance = h*calculate_distance(i, j) / radius + (1 - h)*node[i].load / capacity;
	return effective_distance;
}


//��ÿһ�����н��и���
void queue_fresh(int node_num)
{
	int i, j;
	int packet_num;
	bool found;      //���û��һ������ڷ�Χ�ڣ���Ϊ0
	double effective_distance_min;
	int next_node_num;
	for (i = 0; i < capacity; i++)
	{
		if (node[node_num].Q.empty()) break;
		if (packet[node[node_num].Q.front()].moved) break;
		packet_num = node[node_num].Q.front();

		//����յ���ͨ�ŷ�Χ��
		if (within_scope(node_num, packet[packet_num].destination) == 1)
		{
			Np--;
			node[node_num].Q.pop();
			packet_busy.remove(packet_num);
			packet_free.push(packet_num);
		}

		//����յ㲻��ͨ�ŷ�Χ��
		else
		{
			found = 0;
			effective_distance_min = MAXN1;
			for (j = 0; j < N; j++)
			{
				if (j != node_num&&j != packet[packet_num].destination)
				{
					if (within_scope(i, j) == 1)
					{
						found = 1;
						if (calculate_effective_distance(j, packet[packet_num].destination) < effective_distance_min)
						{
							effective_distance_min = calculate_effective_distance(j, packet[packet_num].destination);
							next_node_num = j;
						}
					}
				}
			}

			//����ҵ�����ͨ�ŷ�Χ�ڵĽ��
			if (found == 1)
			{
				node[node_num].Q.pop();
				node[next_node_num].Q.push(packet_num);
				packet[packet_num].node = next_node_num;
				packet[packet_num].moved = 1;
			}
		}
	}
}


//��������и���
void network_fresh()
{
	int i;
	bool flag;
	double theta;
	for (i = 0; i < N; i++)
	{
		flag = 0;
		while (flag == 0)
		{
			theta = rand() % 6284 * 1.0 / 1000;
			if (theta == 6.283)
				theta += rand() % 186 * 1.0 / 1000000;
			else
				theta += rand() % 1000 * 1.0 / 1000000;

			if (within_cell(node[i].x + velocity*cos(theta), node[i].y + velocity*sin(theta)) == 1)
			{
				node[i].x += velocity*cos(theta);
				node[i].y += velocity*sin(theta);
				flag = 1;
			}
		}
	}
}


//���в���
void test()
{
	int i, j;
	int Np_before = 0;
	int Np_delta = 0;
	FILE *fp1 = fopen("H.txt", "w");
	FILE *fp2 = fopen("time.txt", "w");
	FILE *fp3 = fopen("Np.txt", "w");
//	printf("\n���������ʱ�䣺\n");
//	scanf("%d", &time_final);
	time_final = 2000;
	printf("\n�����뵥λʱ�����²����İ���������\n");
	scanf("%d", &R);
//	printf("\n������ÿ������һ�γ��ӵİ�����\n");
//	scanf("%d", &capacity);
	capacity = 1;
//	printf("\n������۲��1�͹۲��2��\n");
//	scanf("%d%d", &time1, &time2);
	time1 = 1699;
	time2 = 1999;
	Np = 0;
	for (i = MAXN2; i >= 0; i--) packet_free.push(i);

	//ʱ�䲻���ƽ�
	for (i = 0; i < time_final; i++)
	{
		Np_delta = Np - Np_before;
		Np_before = Np;
		printf("ʱ��Ϊ��%d", i);
		fprintf(fp2, "%d\n", i);
		printf("    %d\n", Np_delta);
		fprintf(fp3, "%d\n", Np);
		//��ʼ��
		for (list<int>::iterator iter = packet_busy.begin(); iter != packet_busy.end(); iter++)
			packet[*iter].moved = 0;
		for (j = 0; j < N; j++) node[i].load = node[i].Q.size();

		//��ÿһ�����н��и���
		for (j = 0; j < N; j++)
			if (!node[j].Q.empty())
			{
				queue_fresh(j);
			}

		//�����µĲ��԰�
		for (j = 0; j < R; j++)
		{

			int source = 0, destination = 0;
			while (source == destination)
			{
				source = rand() % N;
				destination = rand() % N;
			}
//			printf("%d ", packet_free.top());
			packet[packet_free.top()].destination = destination;
			packet[packet_free.top()].node = source;
			node[source].Q.push(packet_free.top());
			packet_busy.push_back(packet_free.top());
			packet_free.pop();
		}
		Np += R;

		//��������
		network_fresh();

		//ȡ��
		if (i == time1) Np1 = Np;
		if (i == time2) Np2 = Np;
	}
	//�������
	H = capacity*1.0 / (R*1.0)*(Np2*1.0 - Np1*1.0) / (time2*1.0 - time1*1.0);

	//�������
	fprintf(fp1, "%lf\n", H);

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}

int main()
{
	create_network();
	test();
	return 0;
}



