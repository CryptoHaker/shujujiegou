#include<stdio.h>
#include<stdlib.h>

#define MAXSIZE 1000
#define OK 1
#define ERROR 0
#define true 1
#define false 0

typedef struct Polynomial{
    float c;  //多项式系数
    float e;  //多项式指数
    struct Polynomial* next;
}Polynomial;

typedef Polynomial* PolyPoint;
typedef int bool;

int LENGTH = 0;  //运算后多项式的项的个数

//操作界面
//程序设计的功能有输入、多项式相加、多项式相减、多项式相乘
void GUI_start();
void GUI_main();
//


//处理数据
//输入合法性检测、输入的数据转化成链表存储方式、数据中特殊例处理
void DataSwitch(PolyPoint head, char* str);
bool IsWrong(char* str);  //对输入的多项式进行检测，是否合法
void HandleInput(PolyPoint P, char* intputStr);  //处理输入的字符串，将相关系数存入线性表中
float StringToInt(char* str, int len);  //将输入的字符串转换为整数
void CombineSame(PolyPoint P);  //将具有相同指数的项进行合并
PolyPoint Delete(PolyPoint P);  //将系数为0的向删除
//

//功能区
//多项式加法、减法、乘法
PolyPoint AddPoly(PolyPoint A, PolyPoint B);  //计算两个多项式加法并返回结果


//快速排序链表
//按指数进行升序排列，1为升序，0为降序
void QuickSort(PolyPoint head, PolyPoint tail, int cmp);  
PolyPoint partition(PolyPoint head, PolyPoint tail, int cmp);
void swap(PolyPoint p, PolyPoint q);
//



/*****main begin*****/
int main()
{
    Polynomial Poly, Poly2;
    PolyPoint P, Q;
    P = &Poly, Q = &Poly2;

    char* str1 = (char*)malloc(sizeof(char) * MAXSIZE);
    char* str2 = (char*)malloc(sizeof(char) * MAXSIZE);
    GUI_start();
    scanf("%s", str1);
    scanf("%s", str2);
    if(IsWrong(str1) || IsWrong(str2)) 
    {
        printf("The input is illegal!\n");
        system("pause");
        return 0;
    } 
    GUI_main();
    DataSwitch(P, str1);
    DataSwitch(Q, str2);
    PolyPoint result;
    result = AddPoly(P, Q);
    result = Delete(result);
    while(result)
    {
        printf("%f %f\n", result->c, result->e);
        result = result->next;
    }

    system("pause");
}
/*****main end*****/

void GUI_start()
{
    printf("Welcome to use the polynomial calculator!\n");
    printf("First you should input two strings to continue...\n");
    printf("Input:\n");
}

void GUI_main()
{
    printf("\nuseage: [command]\n");
    printf("        add     Add the two polynomials and show the result.\n");
    printf("        sub     Sub the two polynomials and show the result.\n");
    printf("        mul     Mul the two polynomials and show the result.\n");
}

bool IsWrong(char* str)
{
    int i;
    for(i = 0; str[i] != '\0'; i++)
    {
        if(!(str[i] == 'x' || str[i] == 'X' || str[i] == '^' || str[i] == '+'
            || str[i] == '-' || (str[i] >= '0' && str[i] <= '9') || str[i] == '.'))
            return true;
        if(str[i+1] != '\0' && (str[i] == 'x' || str[i] == 'X') && ((str[i+1] >= '0' && str[i+1] <= '9') || str[i+1] == '.'))
            return true;
        if(str[i] == '.' && (str[i+1] == '\0' || str[i+1] == 'x' || str[i+1] == 'X'))
            return true;
    }
    if(str[i-1] == '-' || str[i-1] == '+' || str[i-1] == '^' || str[i-1] == '.')
        return true;
    return false;
}

void DataSwitch(PolyPoint head, char* str)
{
    HandleInput(head, str);
    QuickSort(head, NULL, 0);
    CombineSame(head);
    head = Delete(head);
}

void HandleInput(PolyPoint P, char* inputStr)
{
    int begin, end;
    for(begin = 0, end = 0; ; )
    {
        //对x前的系数进行处理
        while(inputStr[end] != 'x' && inputStr[end] != 'X' && inputStr[end] != '\0' &&
        inputStr[end] != '+' && inputStr[end] != '-') 
            end++;
        if(end == 0 &&(inputStr[end+1] == 'x' || inputStr[end+1] == 'X')) end++;
        P->c = StringToInt(inputStr + begin, end - begin);

        //常数项处理
        if(inputStr[end] == '+' || inputStr[end] == '-')
        {
            P->e = 0;
            P->next = (PolyPoint)malloc(sizeof(Polynomial)); 
            P = P->next;
            begin = end, end++;
            continue;
        }

        //对x后的指数进行处理
        begin = end;
        //尾部处理
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
        end++;
        P->next = (PolyPoint)malloc(sizeof(Polynomial)); 
        P = P->next;
    }
    P->next = NULL;
}

float StringToInt(char* str, int len)
{
    int i;
    float num = 1, a = 10, b = 1;
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
            if(b < 1) b = b * 0.1;
            if(str[i] == '.')
            {
                a = 1;
                b = 0.1;
                i++;
            }
            num *= a;
            num += (str[i] - '0') * b;
        }
        return str[0] == '-' ? 0 - num : num;
        break;
    }
}

void CombineSame(PolyPoint P)
{
    PolyPoint Q, S;
    Q = P;
    while(Q && Q->next)
    {
        S = Q->next;
        if(Q->e == S->e)
        {
            Q->c += S->c;
            Q->next = S->next;
            free(S);
        }
        else
            Q = Q->next;
    }
}

PolyPoint Delete(PolyPoint P)
{
    PolyPoint S, Q;
    Q = P;
    //处理头部节点
    while(Q && Q->c == 0)
    {
        P = P->next;
        Q = P;
    }

    S = Q;
    if(Q)
        Q = Q->next;
    while(Q)
    {
        if(Q->c == 0)
        {
            S->next = Q->next;
            free(Q);
            Q = S->next;
        }
        else
        {
            S = S->next;
            Q = S->next;
        }
    }
    
    //链表值为0时返回的情况
    if(P == NULL)
    {
        P = (PolyPoint)malloc(sizeof(Polynomial));
        P->c = 0;
        P->e = 0;
        P->next = NULL;
    }
    return P;
}

PolyPoint AddPoly(PolyPoint A, PolyPoint B)
{
    PolyPoint C = (PolyPoint)malloc(sizeof(Polynomial)), Head;
    Head = C;
    C->next = NULL;
    PolyPoint P, Q;
    P = A, Q = B;
    while(P && Q)
    {
        if(P->e == Q->e)
        {
            C->c = P->c + Q->c;
            C->e = P->e;
            P = P->next, Q = Q->next;
        }
        else if(P->e > Q->e)
        {
            C->c = P->c;
            C->e = P->e;
            P = P->next;
        }
        else
        {
            C->c = Q->c;
            C->e = Q->e;
            Q = Q->next;
        }
        if(!P && !Q) 
        {
            C->next = NULL;
            return Head;
        }
        C->next = (PolyPoint)malloc(sizeof(Polynomial));
        C = C->next;
    }
    PolyPoint S;
    if(!P && Q) S = Q;
    else  S = P;
    while(S)
    {
        C->c = S->c;
        C->e = S->e;
        if(!S->next) C->next = NULL;
        else
        {   
            C->next = (PolyPoint)malloc(sizeof(Polynomial));
            C = C->next;
        }
        S = S->next;
    }
    return Head;
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