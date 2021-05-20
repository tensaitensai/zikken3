//4619055 辰川力駆
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char fname[128];
    FILE *fp;
    printf("input filename: ");
    fgets(fname, sizeof(fname), stdin);
    fname[strlen(fname) - 1] = '\0';
    fflush(stdin);
    fp = fopen(fname, "r");

    int i, j, k;
    int vertex = 0;                            //頂点数-1を保存する。
    while (fscanf(fp, "%d %d", &i, &j) != EOF) // 順番に辺の両端 a,bを読み込む
    {
        if (i > vertex)
            vertex = i;
        if (j > vertex)
            vertex = j;
    }

    int Adj[vertex + 1][vertex + 1]; // 隣接行列を表す変数
    for (i = 0; i < vertex; i++)     // 隣接行列の初期化
    {
        for (j = 0; j < vertex; j++)
        {
            Adj[i][j] = 0;
        }
    }

    fp = fopen(fname, "r");
    while (fscanf(fp, "%d %d", &i, &j) != EOF) // 順番に辺の両端 a,bを読み込む
    {
        Adj[i][j]++;
        Adj[j][i]++;
    }
    fclose(fp);

    double cluster_total = 0; //クラスタ数の合計を表す変数
    int degree[vertex + 1];   //次数を表す変数
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

    printf("クラスタ係数: %.4lf", cluster_total / (double)(vertex + 1));
}
