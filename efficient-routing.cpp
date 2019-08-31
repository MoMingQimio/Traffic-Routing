#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"
#include "time.h"
#include "math.h"
#define MAXN1 1225
#define MAXN2 100000


int N;                              // Number of nodes
int degree[MAXN1] = { 0 };          // Degree


struct node
{
	int num;
	struct node *next;
};


struct node *network_head[MAXN1];


struct node *route[MAXN1][MAXN1];
int route_before[MAXN1];


int test_begin[MAXN2];                  //��¼һ����Ĳ������
int test_end[MAXN2];                    //��¼һ����Ĳ����յ� 
int test_step[MAXN2] = { 0 };           //��¼һ����Ĳ����ߵ��ڼ���
int test_now[MAXN2];                    //��¼һ����Ĳ������ڵ�λ��
bool test_moved[MAXN2] = { 0 };         //��¼�����ʱ�����һ�����Ե��Ƿ��Ѿ���Ų��
struct node *queue_head[MAXN1];         //ÿһ�����Ķ���


int R_now;                             //��¼��ǰ�ж��ٸ������ڲ���
int R_before;                          //��¼ǰһ��ʱ���ж��ٸ������ڲ���
int R_delta;                           //��¼���ʱ���������ϸ�ʱ���Ĳ�ֵ
int R1;                                //��¼ʱ��1�ж��ٸ������ڲ���
int R2;                                //��¼ʱ��2�ж��ٸ������ڲ���
double H;                              //Hֵ 


FILE *fp1 = fopen("R_delta.txt", "w");                     //д���ļ��� 
FILE *fp2 = fopen("process.txt", "w");
FILE *fp3 = fopen("R.txt", "w");
FILE *fp4 = fopen("H.txt", "w");

//�������������������
void network_connect(int node1, int node2)
{
	struct node *p;
	p = (struct node*)malloc(sizeof(struct node));
	p->num = node2;
	p->next = network_head[node1];
	network_head[node1] = p;

	p = (struct node*)malloc(sizeof(struct node));
	p->num = node1;
	p->next = network_head[node2];
	network_head[node2] = p;
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
			degree[i*size + j] ++;
			degree[i*size + j + 1]++;
			network_connect(i*size + j, i*size + j + size);
			degree[i*size + j] ++;
			degree[i*size + j + size]++;
		}
	for (i = 0; i<size - 1; i++)
	{
		network_connect(size*(size - 1) + i, size*(size - 1) + i + 1);
		degree[size*(size - 1) + i]++;
		degree[size*(size - 1) + i + 1]++;
		network_connect(i*size + size - 1, i*size + 2 * size - 1);
		degree[i*size + size - 1]++;
		degree[i*size + 2 * size - 1]++;
	}
	for (i = 0; i < size; i++)
	{
		network_connect(i, i + size*(size - 1));
		degree[i]++;
		degree[i + size*(size - 1)]++;
		network_connect(i*size, i*size + size - 1);
		degree[i*size]++;
		degree[i*size + size - 1]++;
	}
}


//����BA����
void create_BA()
{
	int i, j, k;
	int n0, nt, nc;       //ԭʼ����������ӵĽ�����������ӵĽ����ԭ����������
//	printf("������ԭʼ����������ӵĽ�����������ӵĽ����ԭ������������\n");
//	scanf("%d%d%d", &n0, &nt, &nc);
	n0 = 5;
	nt = 1220;
	nc = 2;
	N = n0 + nt;

	//��ʼ��BA����
	for(i=0;i<n0;i++)
		for (j = 0; j < i; j++)
		{
			network_connect(i, j);
			degree[i]++;
			degree[j]++;
		}

	srand((unsigned)time(NULL));
	for (i = n0; i < N; i++)
	{
		bool connected[MAXN1] = { 0 };       //��ֹ�ظ�����
		int denominator = 0;       //�Ѵ��ڽ��Ķ���֮��
		for (j = 0; j < i; j++) denominator += degree[j];

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
				threshold += (degree[k] * 1.0) / (denominator*1.0);
				if (probability < threshold)
				{
					flag = 0;
					if (connected[k] == 0)
					{
						network_connect(i, k);
						degree[i]++;
						degree[k]++;
						j++;
						connected[k] = 1;
					}
				}
				k++;
			}
		}
	}
}


//�������
void print_network()
{
	int i;
	struct node *p;
	printf("\n������磺\n");
	for (i = 0; i < N; i++)
	{
		p = network_head[i];
		printf("%d: ", i);
		while (p != NULL)
		{
			printf("%d ", p->num);
			p = p->next;
		}
		printf("\n");
	}
}


//�������йص���ֵ
void print_degree()
{
	int i;
	printf("\n����������Ķȣ�\n");
	for (i = 0; i < N; i++) printf("%d\n", degree[i]);
}


//�����洢��sequence������
struct node *sequence_in(struct node *sequence,int node)
{
	struct node *p;
	p = (struct node*)malloc(sizeof(struct node));
	p->num = node;
	p->next = sequence;
	sequence = p;
	return sequence;
}


//��sequence����ɾ��
void sequence_destroy(struct node *sequence)
{
	struct node *p, *q;
	p = sequence;
	while (p != NULL)
	{
		q = p->next;
		free(p);
		p = q;
	}
}


//�������
void print_sequence(struct node *sequence)
{
	struct node *p;
	p = sequence;
	while (p != NULL)
	{
		printf("%d ", p->num);
		p = p->next;
	}
	printf("\n");
}


//����ÿ�ҵ�һ����󣬻��ݣ���������·���洢
void route_retrieve(int begin, int end)
{
	int i;
	int level[MAXN1] = { 0 };
	int count = 0;

	//����level
	i = end;
	while (i != begin)
	{
		level[i] = count;
		count++;
		i = route_before[i];
	}
	i = end;
	while (i != begin)
	{
		level[i] = count - level[i];
		i = route_before[i];
	}

	//��sequence��·���洢
	int sequence[MAXN1];
	sequence[0] = begin;
	i = end;
	while (i != begin)
	{
		sequence[level[i]] = i;
		i = route_before[i];
	}

	//��route����·���洢
	struct node *p, *q;
	route[begin][end] = NULL;
	for (i = 1; i < level[end]; i++)
	{
		q = (struct node*)malloc(sizeof(struct node));
		q->num = sequence[i];
		q->next = NULL;
		if (i == 1)
		{
			route[begin][end] = q;
			p = route[begin][end];
		}
		else
		{
			p->next = q;
			p = p->next;
		}
	}
}


//������ȱ����������·��
void route_BFS()
{
	int begin;         //ÿһ��·�������
	for (begin = 0; begin < N; begin++)
	{
		struct node *p, *q;    
		struct node *sequence1, *sequence2;  //�����洢��һ��Ľ��
		struct node *sequence_before[MAXN1];  //�����洢�ɵ���ĳһ���������н��
		int before_sum[MAXN1] = { 0 };     //�����洢�ɵ���ĳһ���������н������
		bool flag = 1;
		bool visited[MAXN1] = { 0 };     //�����
		int route_sum[MAXN1] = { 0 };    //����㵽ĳ������·����
		int level[MAXN1] = { 0 };      //���ڲ���
		int level_count = 1;       //��¼����


		//��ʼ��
		visited[begin] = 1;
		p = network_head[begin];
		sequence1 = NULL;
		sequence2 = NULL;
		for (int i = 0; i < N; i++) sequence_before[i] = NULL;
		while (p != NULL)
		{
			sequence1 = sequence_in(sequence1, p->num);
			visited[p->num] = 1;
			route_sum[p->num] = 1;
			level[p->num] = level_count;
			sequence_before[p->num] = sequence_in(sequence_before[p->num], begin);
			before_sum[p->num]++;
			route_before[p->num] = begin;
			route_retrieve(begin, p->num);
			p = p->next;
		}

		//����
		while (flag == 1)
		{
			int i;
			int step;   //�����·����sequence_beforeָ����ߵĲ���
			flag = 0;
			level_count++;     //��������
			q = sequence1;
			while (q != NULL)
			{
				p = network_head[q->num];
				while (p != NULL)
				{
					if (visited[p->num] == 0)     //����ǻ�û�з��ʹ��Ľ��
					{
						flag = 1;
						sequence2 = sequence_in(sequence2, p->num);
						visited[p->num] = 1;
						route_sum[p->num] += route_sum[q->num];
						level[p->num] = level_count;
						sequence_before[p->num] = sequence_in(sequence_before[p->num], q->num);
						before_sum[p->num]++;
					}
					else
						if (level[p->num] == level_count)    //�����ͬһ���Ѿ����ʹ��Ľ��
						{
							route_sum[p->num] += route_sum[q->num];
							sequence_before[p->num] = sequence_in(sequence_before[p->num], q->num);
							before_sum[p->num]++;
						}
					p = p->next;
				}
				q = q->next;
			}

			//�������·���������л���
			p = sequence2;
			while (p != NULL)
			{
				q = sequence_before[p->num];
				if (before_sum[p->num] > 1)
				{
					step = rand() % before_sum[p->num];
					for (i = 0; i < step; i++) q = q->next;
				}
				route_before[p->num] = q->num;
				route_retrieve(begin, p->num);
				p = p->next;
			}

			//��sequence1��sequence2���д���
			sequence_destroy(sequence1);
			sequence1 = sequence2;
			sequence2 = NULL;
		}
	}
}


//����dijkstra�㷨�ҵ���Ч·��
void route_dijkstra()
{
	int i;
	int begin;      //���
	int now;               //��ǰ���ڵĽ��
	int count;             //�Ѿ��������Ľ����
	bool visited[MAXN1];
	struct node*p;

	for (begin = 0; begin < N; begin++)
	{
		int distance[MAXN1];

		//��ʼ��
		for (i = 0; i < N; i++) visited[i] = 0;
		visited[begin] = 1;
		for (i = 0; i < N; i++) distance[i] = MAXN2;
		distance[begin] = degree[begin];
		count = 1;
		now = begin;

		//����
		while (count <= N)
		{
			struct node *sequence_next;   //�����洢����Ҫ�����һ����
			int next_sum = 0;    //��¼����Ҫ�����һ�������Ŀ 
			int min_k = MAXN2;    //��С�Ķ���
			int step;   //�������һ���㣬ָ����ߵĲ���

			//��ʼ��
			sequence_next = NULL;
			p = network_head[now];

			//���¾���
			while (p != NULL)
			{
				if (visited[p->num] == 0 && degree[p->num] + distance[now] < distance[p->num])
				{
					distance[p->num] = degree[p->num] + distance[now];
					route_before[p->num] = now;
				}
				p = p->next;
			}
			
			//Ѱ��һ�¸���
			for (i = 0; i < N; i++)
			{
				if (visited[i] == 0&&distance[i] < MAXN2)
				{
					if (distance[i] < min_k)
					{
						sequence_destroy(sequence_next);
						sequence_next = NULL;
						next_sum = 0;
						min_k = distance[i];
						sequence_next = sequence_in(sequence_next, i);
						next_sum++;
					}
					else
					{
						if (distance[i] == min_k)
						{
							sequence_next = sequence_in(sequence_next, i);
							next_sum++;
						}
					}
				}
			}
			if (sequence_next != NULL)
			{
				p = sequence_next;
				if (next_sum > 1)
				{
					step = rand() % next_sum;
					for (i = 0; i < step; i++) p = p->next;
				}
				now = p->num;
				visited[now] = 1;
				route_retrieve(begin, now);
			}
			count++;
		}
	}
}


//�������·��
void print_route()
{
	int i, j;
	printf("\n�������·����\n");
	for (i = 0; i<N; i++)
		for (j = 0; j<N; j++)
			if (i != j)
			{
				struct node *p;
				p = route[i][j];
				printf("%d->", i);
				while (p != NULL)
				{
					printf("%d->", p->num);
					p = p->next;
				}
				printf("%d\n", j);
			}
}


//ͳ�ƾ���ÿһ�����·��������������㣬�������յ�
void print_distribution()
{
	int i, j;
	int distribution[MAXN1] = { 0 };
	for (i = 0; i<N; i++)
		for (j = 0; j<N; j++)
			if (i != j)
			{
				distribution[i]++;
				struct node *p;
				p = route[i][j];
				while (p != NULL)
				{
					distribution[p->num]++;
					p = p->next;
				}
			}
	for (i = 0; i < N; i++)
	{
		printf("%10d", distribution[i]);
		if ((i + 1) % 12 == 0) printf("\n");
	}
}


//���
void queue_in(int ii, int num)    //ii��ʾ�����н����ţ�num��ʾҪ��ӵĲ��Ե����
{
	struct node *p, *q;
	p = queue_head[ii];
	if (p == NULL)
	{
		p = (struct node*)malloc(sizeof(struct node));
		p->num = num;
		p->next = NULL;
		queue_head[ii] = p;
	}
	else
	{
		while (p->next != NULL)
			p = p->next;
		q = (struct node*)malloc(sizeof(struct node));
		q->num = num;
		q->next = NULL;
		p->next = q;
	}
}


//����,���ҷ��س��Ӳ��Ե�����
int queue_out(int ii)             //ii��ʾ�����еĽ�����
{
	int num;
	struct node *p, *q;
	num = queue_head[ii]->num;
	if (queue_head[ii]->next == NULL) queue_head[ii] = NULL;
	else
	{
		p = queue_head[ii]->next;
		q = queue_head[ii];
		free(q);
		queue_head[ii] = p;
	}
	test_moved[num] = 1;
	return num;
}


//�ҵ�һ����Ĳ��Ե���һ����
int test_nextnode(int num)
{
	struct node *p;
	p = route[test_begin[num]][test_end[num]];
	int i;
	for (i = 0; i < test_step[num]; i++) p = p->next;
	if (p != NULL) return p->num;
	else return MAXN1;
}


//��ÿһ�����н��и���
void test_fresh(int ii)                     //ii��ʾ�����еĽ�����
{
	int num, nextnode;
	if (test_moved[queue_head[ii]->num] == 0)      //���µ�������������׸����Ե㻹û���ƶ���
	{
		num = queue_out(ii);
		nextnode = test_nextnode(num);
		if (nextnode < MAXN1)
		{
			queue_in(nextnode, num);
			test_now[num] = nextnode;
			test_step[num]++;
		}
		else R_now--;
	}
}


//�������ڽӱ����
void print_queue()
{
	int i;
	struct node *p;
	fprintf(fp2, "\n��������ڽӱ�\n");
	for (i = 0; i<N; i++)
	{
		p = queue_head[i];
		if (queue_head[i] == NULL)
		{
			fprintf(fp2, "%d:\n", i);
		}
		else
		{
			fprintf(fp2, "%d: ", i);
			do
			{
				fprintf(fp2, "%d ", p->num);
				p = p->next;
			} while (p != NULL);
			fprintf(fp2, "\n");
		}
	}
}


//���в���
void test()
{
	int i, j;                 //i��ʾʱ��㣬j��ʾÿһ��ʱ����²����İ������
	int time;                 //time��ʾ����ʱ�� 
	int R;                    //��λʱ���ڲ����İ�����Ŀ 
	int test_count = 0;       //route_count����Ϊ·�����
//	printf("\n���������ʱ�䣺\n");
//	scanf("%d", &time);
	time = 5000;
	printf("\n�����뵥λʱ�����²����İ���������\n");
	scanf("%d", &R);
	R_now = 0;
	R_before = 0;


	//ʱ�䲻�ϸ���
	for (i = 0; i<time; i++)
	{
		R_before = R_now;
		//		                                    		fprintf(fp2,"ʱ��Ϊ%d\n", i);

		//ÿһ��ʱ��㿪ʼ��test_moved���и���
		for (j = 0; j < test_count; j++) test_moved[j] = 0;


		//��ÿһ�����н��и���
		for (j = 0; j < N; j++)
			if (queue_head[j] != NULL) test_fresh(j);

		//�����µĵ�
		//		                                    		fprintf(fp2,"�����µĲ��Ե㣺\n");
		for (j = 0; j<R; j++)
		{
			int begin = 0, end = 0;              //�����յ�
			while (begin == end)
			{
				begin = rand() % N;
				end = rand() % N;
			}
			//						                            fprintf(fp2,"%d %d\n", begin, end);
			test_begin[test_count] = begin;
			test_end[test_count] = end;
			test_now[test_count] = begin;
			queue_in(begin, test_count);
			test_count++;
		}
		R_now = R_now + R;
		R_delta = R_now - R_before;
//		fprintf(fp1, "R_delta(%d)=%d R_now(%d)=%d\n", i, R_delta, i, R_now);
		//		                                        print_queue();
		//		                                        fprintf(fp2,"\n");
	
		//���³������Բ��ԣ�ȡ��400��500��Rֵ
		if (i == 4499) R1 = R_now;
		if (i == 4999) R2 = R_now;
	}

	H = 1.0 / (R*1.0)*((R2 - R1)*1.0) / 500;
	fprintf(fp3, "%d\n", R);
	fprintf(fp4, "%lf\n", H);
//	fclose(fp1);
	//	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
}


int main()
{
	create_lattice();
//	create_BA();
//	print_network();
//	print_degree();
	route_BFS();
//	route_dijkstra();
	print_route();
	print_distribution();
//	test();
	return 0;
}

