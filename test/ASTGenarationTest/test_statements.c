int main(void)
{
    int n;
    float x, ans;

    x = 9000;
    if (x < 1000)
        ans = 0;
    else if (x < 2000)
        ans = (x - 1000) * 0.05;
    else if (x < 3000)
        ans = (x - 2000) * 0.10 + 50;
    else if (x < 4000)
        ans = (x - 3000) * 0.15 + 150;
    else if (x < 5000)
        ans = (x - 4000) * 0.20 + 300;
    else
        ans = (x - 5000) * 0.25 + 500;

    int i, j, k;
    int **d;

    for (k = 0; k < n; k++)
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                if (d[i][j] > d[i][k] + d[k][j])
                    d[i][j] = d[i][k] + d[k][j];

    return 0;
}

int binarySearch(int *a, int n, int key)
{
    int low, high, mid, midVal;

    low = 0;
    high = n - 1;
    while (low <= high)
    {
        mid = (low + high) / 2;
        midVal = a[mid];
        if (midVal < key)
            low = mid + 1;
        else if (midVal > key)
            high = mid - 1;
        else
            return mid;
    }
    return -1;
}

void block(void)
{
    int i, j, n, k, row;
    {
        while (row > 0)
        {
            for (n = 0; n < row; n++)
            {
                for (i = n; i < (row - 1); i++)
                    if (i == 90)
                        break; //do nothing
                    else
                        continue;
                for (j = 0; j <= n; j++)
                {
                    if (j == 0)
                        k = 1;
                    else
                        k = k * (n - j + 1) / j;
                }
            }
        }
    }
}