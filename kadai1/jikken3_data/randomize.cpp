//4619055 辰川力駆
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <random>
#include <iostream>
#include <string>
using namespace std;

struct queue // 待ち行列
{
    int item;
    struct queue *next;
};

struct queue *head, *tail;

void addq(int x)
{
    tail->item = x;
    tail->next = (struct queue *)malloc(sizeof(struct queue));
    tail = tail->next;
}

int rmvq(void)
{
    int x;
    struct queue *p;

    p = head;
    head = p->next;
    x = p->item;
    free((char *)p);
    return x;
}

int main(int argc, char *argv[])
{
    char fname[128];
    FILE *fp;
    printf("input filename: ");
    fgets(fname, sizeof(fname), stdin);
    fname[strlen(fname) - 1] = '\0';
    fflush(stdin);
    fp = fopen(fname, "r");

    int i, j, k;
    int vertex = 0;                            //頂点数を保存する。
    while (fscanf(fp, "%d %d", &i, &j) != EOF) // 順番に辺の両端 i,jを読み込む
    {
        if (i + 1 > vertex)
            vertex = i + 1;
        if (j + 1 > vertex)
            vertex = j + 1;
    }

    int Adj[vertex][vertex]; // 隣接行列を表す変数

    for (i = 0; i < vertex; i++) // 隣接行列の初期化
    {
        for (j = 0; j < vertex; j++)
        {
            Adj[i][j] = 0;
        }
    }

    fp = fopen(fname, "r");
    while (fscanf(fp, "%d %d", &i, &j) != EOF) // 順番に辺の両端 i,jを読み込む
    {
        Adj[i][j]++;
        Adj[j][i]++;
    }
    fclose(fp);

    //メルセンヌ・ツイスタによるランダマイズ
    int seed;
    if (argc > 1)
    {
        seed = stoi(argv[1]);
    }
    else
    {
        seed = random_device{}();
    }
    mt19937 rand_src(seed);

    int Number_of_attempts;

    double sum_cluster_total = 0; //クラスタ係数の合計
    double sum_mean_vertex = 0;   // 平均頂点間距離の合計

    for (Number_of_attempts = 0; Number_of_attempts < 15; Number_of_attempts++)
    {
        int k1, k2, k3, k4;
        for (i = 0; i < 1000000; i++)
        {
            k1 = rand_src() % vertex;
            k2 = rand_src() % vertex;
            k3 = rand_src() % vertex;
            k4 = rand_src() % vertex;

            if (Adj[k1][k2] == 1 && Adj[k3][k4] == 1 && Adj[k1][k3] == 0 && Adj[k2][k4] == 0 && k1 != k2 && k3 != k4 && k1 != k3 && k2 != k4)
            {
                Adj[k1][k2] = 0;
                Adj[k3][k4] = 0;
                Adj[k1][k3] = 1;
                Adj[k2][k4] = 1;
            }
        }

        double cluster_total = 0; //クラスタ数の合計を表す変数
        int degree[vertex];       //次数を表す変数
        int triangle;             //三角形の数を数える変数
        for (i = 0; i < vertex; i++)
        {
            degree[i] = 0;
            for (j = 0; j < vertex; j++)
            {
                degree[i] += Adj[i][j];
            }
        }

        //クラスタ係数を求める
        for (i = 0; i < vertex; i++)
        {
            triangle = 0;

            for (j = 0; j < vertex; j++)
            {
                if (Adj[i][j] == 1)
                {
                    for (k = 0; k < vertex; k++)
                    {
                        if (Adj[i][k] == 1 && Adj[j][k] == 1)
                            triangle++;
                    }
                }
            }
            if (degree[i] > 1)
                cluster_total += (double)triangle / (double)((degree[i] - 1) * degree[i]);
        }

        int distance[vertex];   //最短距離を表す変数
        double mean_vertex = 0; //平均頂点間距離を表す変数

        head = (struct queue *)malloc(sizeof(struct queue));
        tail = head;

        int START;
        for (START = 0; START < vertex; START++)
        {
            for (i = 0; i < vertex; i++)
            {
                distance[i] = -1;
            }

            addq(START);
            distance[START] = 0;
            do
            {
                i = rmvq();
                for (j = 0; j < vertex; j++)
                {
                    if (Adj[i][j] != 0 && distance[j] < 0)
                    {
                        addq(j);
                        distance[j] = distance[i] + 1;
                    }
                }
            } while (head != tail);

            for (i = 0; i < vertex; i++)
            {
                if (START < i)
                {
                    mean_vertex += 2 * (double)distance[i] / (double)((vertex - 1) * vertex);
                }
            }
        }

        printf("クラスタ係数: %.4lf        平均頂点間距離: %.4lf\n", cluster_total / (double)(vertex), mean_vertex);
        sum_cluster_total += cluster_total / (double)(vertex);
        sum_mean_vertex += mean_vertex;
    }

    printf("\n\nこれらの平均の(クラスタ係数: %.4lf        平均頂点間距離: %.4lf)\n\n", sum_cluster_total / (double)(15), sum_mean_vertex / (double)(15));
}
