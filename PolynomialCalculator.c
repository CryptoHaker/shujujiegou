#include<stdio.h>
#include<stdlib.h>

#define MAXSIZE 1000
#define OK 1
#define ERROR 0

typedef struct Polynomial{
    int c;  //多项式系数
    int e;  //多项式指数
    struct Polynomial* next;
}Polynomial;

typedef Polynomial* PolyPoint;

int LENGTH = 0;  //运算后多项式的项的个数

int StringToInt(char* str, int len);  //将输入的字符串转换为整数

//快速排序链表
//按指数进行升序排列，1为升序，0为降序
void QuickSort(PolyPoint head, PolyPoint tail, int cmp);  
PolyPoint partition(PolyPoint head, PolyPoint tail, int cmp);
void swap(PolyPoint p, PolyPoint q);
//

/*****main begin*****/
int main()
{
    Polynomial Poly;
    PolyPoint P;
    P = &Poly;

    char* inputStr = (char*)malloc(sizeof(char) * MAXSIZE);
    gets(inputStr);

    //对输入的多项式字符串进行处理，将相关系数存入线性表中
    /*****handle begin*****/
    int begin, end;
    for(begin = 0, end = 0; ; )
    {
        //对x前的系数进行处理
        while(inputStr[end] != 'x' && inputStr[end] != 'X' && inputStr[end] != '\0') 
            end++;
        P->c = StringToInt(inputStr + begin, end - begin);

        //对x后的指数进行处理
        begin = end;
        //常数项处理
        if(inputStr[end] == '\0')
        {
            P -> e = 0;
            break;
        }

        end++, begin++;
        if(inputStr[begin] == '^') begin++, end = begin + 1;
        while(inputStr[end] != '+' && inputStr[end] != '-' && inputStr[end] != '\0') end++;
        P->e = StringToInt(inputStr + begin, end - begin);
        
        begin = end;
        if(inputStr[end] == '\0')
            break;
        P->next = (PolyPoint)malloc(sizeof(Polynomial)); 
        P = P->next;
    }
    P->next = NULL;
    /*****handle end*****/

    PolyPoint head = &Poly;
    QuickSort(head, NULL, 0);

    while(head)
    {
        printf("%d %d\n", head->c, head->e);
        head = head->next;
    }
    system("pause");
}
/*****main end*****/


int StringToInt(char* str, int len)
{
    int i, num = 0;
    int isSymbol = 0;  //判断是否为负数

    switch(len)
    {
    //str = x 或 str = 0 对应的情况
    case 0:
        if(str[0] == 'x' || str[0] == 'X' || str[0] == '+' || str[0] == '-' || str[0] == '\0') 
            return 1;
        else return 0;
        break;

    //str = -x 或 str = 2x 的情况
    case 1:
        if(str[0] == '-') return -1;
        else if(str[0] == '+') return 1;
        else return (str[0] - '0');
        break;
    
    //剩余情况
    default:
        if(str[0] == '-' || str[0] == '+') isSymbol = 1;
        for(i = isSymbol ? 1 : 0; i < len; i++)
        {
            num *= 10;
            num += str[i] - '0';
        }
        return str[0] == '-' ? 0 - num : num;
        break;
    }
}

void QuickSort(PolyPoint head, PolyPoint tail, int cmp)
{
    if(head != tail && head->next != tail)
    {
        PolyPoint mid = partition(head, tail, cmp);
        QuickSort(head, mid, cmp);
        QuickSort(mid->next, tail, cmp);
    }
}

PolyPoint partition(PolyPoint head, PolyPoint tail, int cmp)
{
    PolyPoint p = head;
    PolyPoint q = p->next;
    int pivot = p->e;
    while(q != tail)
    {
        if(cmp ? q->e < pivot : q->e > pivot)
        {
            p = p->next;
            swap(p, q);
        }
        q = q->next;
    }
    swap(p, head);
    return p;
}

void swap(PolyPoint p, PolyPoint q)
{
    int pivotC, pivotE;
    pivotC = p->c, pivotE = p->e;
    p->c = q->c, p->e = q->e;
    q->c = pivotC, q->e = pivotE;
}