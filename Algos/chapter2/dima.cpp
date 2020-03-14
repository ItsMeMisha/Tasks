#include <iostream>
#include <vector>
typedef int key_t;
class Heap
{
public:
    key_t* data;
    int size, max_size = 0;
    Heap(int max_size)
    {
        this->max_size = max_size;
        data = new key_t[max_size];
        size = 0;
    }
    ~Heap()
    {
        delete[] data;
    }
private:
    int Left(int index)
    {
        return 2 * index + 1;
    }
    int Right(int index)
    {
        return 2 * index + 2;
    }
    int Parent(int index)
    {
        return (index - 1) / 2;
    }
    void Swap(int index_1, int index_2)
    {
        key_t tmp = data[index_1];
        data[index_1] = data[index_2];
        data[index_2] = tmp;
    }
    void SiftUp(int index)
    {
        while (index != 0 && data[index] < data[Parent(index)])
        {
            Swap(Parent(index), index);
            index = Parent(index);
        }
    }
    void SiftDown(int index)
    {
        int left = Left(index), right = Right(index);
        if (left >= size)
        {
            return;
        }
        else if (right == size)
        {
            if (data[index] > data[left])
            {
                Swap(index, left);
            }
            return;
        }
        if (data[left] > data[right])
        {
            left = right;
        }
        if (data[index] > data[left])
        {
            Swap(index, left);
            SiftDown(left);
        }
    }
public:
    void PrintData()
    {
        printf("\n===================================================================\n");
        for (int i = 0; i < size; ++i)
        {
            printf("%d ", data[i]);
        }
        printf("\n===================================================================\n");
    }
    int Insert(key_t value)
    {
        if (size == max_size)
            return -1;
        data[size] = value;
        if (size > 0)
            SiftUp(size);
        ++size;
        return 0;
    }
    key_t GetMin()
    {
        if (size == 0)
            return 1000000001;
        return data[0];
    }
    int RemoveMin()
    {
        if (size <= 0)
            return -1;
        data[0] = data[size - 1];
        --size;
        SiftDown(0);
        return 0;
    }
};
enum codes
{
    insert,
    getMin,
    removeMin
};
const char* command_by_code[3] = { "insert", "getMin", "removeMin" };
void PrintAnswer(std::vector < std::pair < codes, key_t» commands, int ans)
{
    printf("%d\n", ans);
    for (int i = 0; i < ans; ++i)
    {
        if (commands[i].first != removeMin)
            printf("%s %d\n", command_by_code[commands[i].first], commands[i].second);
        else
            printf("%s\n", command_by_code[commands[i].first]);
    }
}
void GenerateStressTest()
{
    FILE* test = fopen("heap.test", "w");
    for (long i = 0; i < 1e2; ++i)
    {
        int coin = rand() % 3;
        if (coin == 0)
            fprintf(test, "heap->Insert(%d);\n", rand() % 2048);
        if (coin == 1)
            fprintf(test, "heap->RemoveMin();\n");
        if (coin == 2)
            fprintf(test, "heap->GetMin();\n");
    }
    fclose(test);
}
int main()
{
    Heap* heap = new Heap(1000000);
    key_t tmp;
    char buf[10] = "";
    int commandsQty = 0;
    scanf("%d", &commandsQty);
    int ans = commandsQty;
    std::vector < std::pair < codes, key_t» commands;
    for (int i = 0; i < commandsQty; ++i)
    {
        scanf("%s", buf);
        if (buf[0] == 'i')
        {
            scanf("%d", &tmp);
            heap->Insert(tmp);
            commands.push_back(std::pair<codes, key_t>(insert, tmp));
        }
        if (buf[0] == 'g')
        {
            scanf("%d", &tmp);
            while ((heap->size > 0) && (heap->GetMin() < tmp))
            {
                commands.push_back(std::pair<codes, key_t>(removeMin, 0));
                heap->RemoveMin();
                ++ans;
            }
            if ((heap->size == 0) || (heap->GetMin() > tmp))
            {
                commands.push_back(std::pair<codes, key_t>(insert, tmp));
                heap->Insert(tmp);
                ++ans;
            }
            commands.push_back(std::pair<codes, key_t>(getMin, tmp));
        }
        if (buf[0] == 'r')
        {
            if (heap->size == 0)
            {
                commands.push_back(std::pair<codes, key_t>(insert, 0));
                commands.push_back(std::pair<codes, key_t>(removeMin, 0));
                ++ans;
            }
            else
            {
                commands.push_back(std::pair<codes, key_t>(removeMin, 0));
                heap->RemoveMin();
            }
        }
    }
    PrintAnswer(commands, ans);
    delete heap;
    return 0;
}

