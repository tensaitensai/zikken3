//4619055 辰川力駆
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(void)
{
    char fname[128];
    FILE *fp;
    printf("input filename: ");
    fgets(fname, sizeof(fname), stdin);
    fname[strlen(fname) - 1] = '\0';
    fflush(stdin);
    fp = fopen(fname, "r");

    int i, j;
    int vertex = 0;                            //頂点数を保存する。
    while (fscanf(fp, "%d %d", &i, &j) != EOF) // 順番に辺の両端 i,jを読み込む
    {
        if (i + 1 > vertex)
            vertex = i + 1;
        if (j + 1 > vertex)
            vertex = j + 1;
    }

    int Adj[vertex][vertex];     // 隣接行列を表す変数
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

    int distance[vertex];   //最短距離を表す変数
    double mean_vertex = 0; //平均頂点間距離を表す変数

    head = (struct queue *)malloc(sizeof(struct queue));
    tail = head;

    int START = 0;
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

    printf("平均頂点間距離: %.4lf\n", mean_vertex);
}
