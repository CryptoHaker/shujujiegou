#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXSIZE 1000
#define OK 1
#define ERROR 0
#define true 1
#define false 0

typedef double ElemType;
typedef int bool;
typedef struct Polynomial{
    ElemType c;  //多项式系数
    ElemType e;  //多项式指数
    struct Polynomial* next;
}Polynomial;
typedef Polynomial* PolyPoint;

int ISEMPTY = 1; //数据是否为空

//操作界面
void GUI();

//处理数据
void DataSwitch(PolyPoint head, char* str);
bool IsWrong(char* str);  //对输入的多项式进行检测，是否合法
void HandleInput(PolyPoint P, char* intputStr);  //处理输入的字符串，将相关系数存入线性表中
void StringToInt(PolyPoint P, char* str, int len);  //将输入的字符串转换为整数
void CombineSame(PolyPoint P);  //将具有相同指数的项进行合并
PolyPoint Delete(PolyPoint P);  //将系数为0的向删除
void OutputResult(PolyPoint result);  //输出结果

//功能区
int InputPoly(PolyPoint A, PolyPoint B);   //输入数据
PolyPoint AddPoly(PolyPoint A, PolyPoint B, int cmp);  //计算两个多项式加法或减法并返回结果，cmp为1是加法，为0减法
PolyPoint MulPoly(PolyPoint A, PolyPoint B);  //计算两个多项式的乘法并返回结果

//快速排序链表
void QuickSort(PolyPoint head, PolyPoint tail, int cmp);  
PolyPoint partition(PolyPoint head, PolyPoint tail, int cmp);
void swap(PolyPoint p, PolyPoint q);

/*****main begin*****/
int main()
{
    char order[6] = {'\0'};
    GUI();
    while(scanf("%s", order) != EOF)
    {
        Polynomial Poly, Poly2;
        PolyPoint P, Q;
        P = &Poly, Q = &Poly2;
        if(!strcmp(order, "input"))
        {    
            if(InputPoly(P, Q))
                ISEMPTY = 0, printf("Finish.\n");
            else ISEMPTY = 1;
        }
        else if(!strcmp(order, "add"))
        {
            if(ISEMPTY) printf("ERROR: No data.\n");
            else
            {
                PolyPoint result;
                result = AddPoly(P, Q, 1);
                result = Delete(result);
                printf("The result is:\n");
                OutputResult(result);
                printf("\n");
            }
        }
        else if(!strcmp(order, "sub"))
        {
            if(ISEMPTY) printf("ERROR: No data.\n");
            else
            {
                PolyPoint result;
                result = AddPoly(P, Q, 0);
                result = Delete(result);
                printf("The result is:\n");
                OutputResult(result);
                printf("\n");
            }
        }
        else if(!strcmp(order, "mul"))
        {
            if(ISEMPTY) printf("ERROR: No data.\n");
            else
            {
                PolyPoint result;
                result = MulPoly(P, Q);
                printf("The result is:\n");
                OutputResult(result);
                printf("\n");
            }
        }
        else
        printf("ERROR: Invalid instruction.\n");
        printf("\n");
    }
    system("pause");
}
/*****main end*****/

void GUI()
{
    printf("***************************************************************\n");
    printf("*                    Polynomial Calculator                    *\n");
    printf("***************************************************************\n");
    printf("\n\n");
    printf("useage:  <command>\n");
    printf("          input     Let you to inout tow polynomials.\n");
    printf("          add	    Add the two polynomias.\n");
    printf("          sub	    Sub the two polynomias.\n");
    printf("          mul	    Mul the two polynomias.\n");
    printf("\n\n");
    printf("***************************************************************\n");
    printf("*                                                             *\n");
    printf("***************************************************************\n\n\n");
}

int InputPoly(PolyPoint A, PolyPoint B)
{
    char* str1 = (char*)malloc(sizeof(char) * MAXSIZE);
    char* str2 = (char*)malloc(sizeof(char) * MAXSIZE);
    scanf("%s", str1), scanf("%s", str2);
    if(IsWrong(str1) || IsWrong(str2)) 
    {
        printf("ERROR: Illegal input.\n");
        return ERROR;
    } 
    DataSwitch(A, str1), DataSwitch(B, str2);
    free(str1), free(str2);
    return OK;
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
    for(begin = 0, end = 1; ; )
    {
        while(inputStr[end] != '\0')
        {   
            if(inputStr[end] == '+' || inputStr[end] == '-') 
                if(inputStr[end-1] != '^') break;
            end++;
        }
        StringToInt(P, inputStr + begin, end - begin);
        begin = end;
        if(inputStr[begin] == '\0') break;
        P->next = (PolyPoint)malloc(sizeof(Polynomial));
        P = P->next;
        end++;
    }
    P->next = NULL;
}

void StringToInt(PolyPoint P,  char* str, int len)
{
    int cur = 0;
    while(str[cur] != 'x' && str[cur] != 'X' && cur < len) cur++;
    if(cur == len)  //此时为常数项
    {
        P->c = atof(str);
        P->e = 0;
    }
    else
    {
        if(cur == 0) P->c = 1;
        else if(cur == 1) 
        {
            if(str[cur-1] == '+') P->c = 1;
            else if(str[cur-1] == '-') P->c = -1;
            else P->c = atof(str);
        } 
        else P->c = atof(str);
        if(cur+1 == len)
            P->e = 1;
        else
        {
            cur = cur + 2;
            if(cur + 1 == len && str[cur] == '-') P->e = -1;
            else P->e = atof(str + cur);
        }
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

void OutputResult(PolyPoint result) 
{
    PolyPoint P = result;
    while(P)
    {
        if(P->c != 1)
        {
            if(P->c == -1)
            {
                if(P->e == 0) printf("-1");
                else printf("-");
            }
            else printf("%g", P->c);
        }
        else
            if(P->e == 0) printf("1");
        if(P->e != 0)
        {
            printf("x");
            if(P->e != 1)
            {
                printf("^");
                printf("%g", P->e);
            }
        }
        if(P->next && P->next->c > 0) printf("+");
        P = P->next;
    }
}

PolyPoint AddPoly(PolyPoint A, PolyPoint B, int cmp)
{
    PolyPoint C = (PolyPoint)malloc(sizeof(Polynomial)), Head = C;
    C->next = NULL;
    PolyPoint P, Q, S;
    P = A, Q = B;
    while(P && Q)  
    {
        if(P->e == Q->e)  //指数相等便直接相加/相减系数
        {
            C->c = cmp ? P->c + Q->c : P->c - Q->c;
            C->e = P->e;
            P = P->next, Q = Q->next;
        }
        else if(P->e > Q->e)  //哪一个指数大排在前面
        {
            C->c = P->c;
            C->e = P->e;
            P = P->next;
        }
        else
        {
            C->c = cmp ? Q->c : 0 - Q->c;
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
    if(!P && Q) S = Q;
    else  S = P, cmp = 1;
    while(S)  //处理较长的多项式
    {
        C->c = cmp ? S->c : 0 - S->c;
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

PolyPoint MulPoly(PolyPoint A, PolyPoint B)
{
    PolyPoint P, Q, R, newHead;
    Q = B, R = (PolyPoint)malloc(sizeof(Polynomial));
    newHead = R;
    while(Q)
    {
        P = A;
        while(P)
        {
            R->c = Q->c * P->c, R->e = Q->e + P->e;
            if(P->next == NULL && Q->next == NULL) R->next = NULL;
            else R->next = (PolyPoint)malloc(sizeof(Polynomial));
            R = R->next;
            P = P->next;
        }
        Q = Q->next;
    }
    QuickSort(newHead, NULL, 0);
    CombineSame(newHead);
    return Delete(newHead);
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
    ElemType pivot = p->e;
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
    ElemType pivotC, pivotE;
    pivotC = p->c, pivotE = p->e;
    p->c = q->c, p->e = q->e;
    q->c = pivotC, q->e = pivotE;
}