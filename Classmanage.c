//Function Available:
//IO of two databases:Classdata and Studentdata
//Register of Student
//Show class status
//Show student status

//TODO:
//Add class choosing system

//Ticks:
//2 4 7
//Havent done:
//1 3 5 6

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct __classdata__
{
    char __classname__[128];
    int __points__;
    int __seats__;
    int __seats_remain__;
} classdata;

typedef struct __studentdata__
{
    int id;
    int sumpoints;
    int numofchosenclasses;
    char class[16][128];
} studentdata;

void __InitClassDataSheet__(classdata *__cdp__, int __dt_size__)
{
    for (int i = 0; i < __dt_size__; i++)
    {
        __cdp__->__points__ = i + 1;
        __cdp__->__seats__ = 10 * (i + 1);
        //__cdp__->__classname__ = (char *)(malloc(128));
    }
}

void __GetClassData__(classdata dt[], int __items__, char *__database_location__)
{
    FILE *fp = fopen(__database_location__, "r+");
    for (int i = 0; i < __items__; i++)
    {
        fscanf(fp, "%s%d%d%d", &(dt[i].__classname__), &(dt[i].__points__), &(dt[i].__seats__), &(dt[i].__seats_remain__));
        //strcat(dt[i].__classname__, "\0");
        //dt[i].__classname__
    }
    fclose(fp);
    return;
}

void __GetStudentData__(studentdata dt[], int __items__, char *__database_location__)
{
    FILE *fp = fopen(__database_location__, "r+");
    for (int i = 0; i < __items__; i++)
    {
        fscanf(fp, "%d%d%d", &dt[i].id, &dt[i].sumpoints, &dt[i].numofchosenclasses);
        for (int j = 0; j < dt[i].numofchosenclasses; j++)
        {
            fscanf(fp, "%s", &dt[i].class[j]);
        }
        //strcat(dt[i].__classname__, "\0");
        //dt[i].__classname__
    }
    fclose(fp);
    return;
}

int __GetFileLineCount__(char *__database_location__)
{
    FILE *fp = fopen(__database_location__, "r+");
    char __cbuf__[128];
    //__cbuf__ = (char*)(malloc(128));
    int __lct__ = 0;
    while (fgets(__cbuf__, 256, fp) != NULL)
    {
        __lct__++;
    }
    fclose(fp);
    return __lct__;
}

void __ACTION__(int __result__)
{
    switch (__result__)
    {
    case 0:
        printf("Action Performed.\n");
        break;
    case 1:
        printf("Action Failed.\n");
        break;
    case 254:
        printf("No Seats Available.\n");
        break;
    case 255:
        printf("Limit Exceeded\n");
        break;
    default:
        printf("Invalid Action.\n");
        break;
    }
}

int __CHOOSE_CLASS__(const char *__classname__, classdata __table__[], int __tablesize__)
{
    int i = 0;
    for (; i < __tablesize__; i++)
    {
        if (strcmp(__table__[i].__classname__, __classname__) == 0)
            break;
    }
    if (strcmp(__table__[i].__classname__, __classname__) != 0)
        return 255;
    if (__table__[i].__seats_remain__ <= 0)
        return 254;
    __table__[i].__seats_remain__--;
    return 0;
}

int __Register_In_Stu_Database__(int id, studentdata __table__[], int *slct)
{
    for (int i = 0; i < *slct; i++)
    {
        if (__table__[i].id == id)
            return i;
    }
    __table__[*slct].id = id;
    __table__[*slct].sumpoints = 0;
    __table__[*slct].numofchosenclasses = 0;
    (*slct)++;
    return (*slct) - 1;
}

void writeStudent(studentdata dt[], int __items__, char *__database_location__)
{
    FILE *fp = fopen(__database_location__, "r+");
    for (int i = 0; i < __items__; i++)
    {
        fprintf(fp, "%d %d %d ", dt[i].id, dt[i].sumpoints, dt[i].numofchosenclasses);
        for (int j = 0; j < dt[i].numofchosenclasses; j++)
        {
            fprintf(fp, "%s ", dt[i].class[j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return;
}

void writeClass(classdata dt[], int __items__, char *__database_location__)
{
    FILE *fp = fopen(__database_location__, "r+");
    for (int i = 0; i < __items__; i++)
    {
        fprintf(fp, "%s %d %d %d\n", dt[i].__classname__, dt[i].__points__, dt[i].__seats__, dt[i].__seats_remain__);
    }
    fclose(fp);
    return;
}

/* Database format:
*
*   title   points  seats   seats_remain
*
*/

/* Student Database format:
*
*   StudentIDnumber sumpoints   numofchosenclasses  class1  class2  class3  class4  class5
*
*/

int main()
{
    classdata cd[100];
    studentdata s[256];

    char __database_location__[] = "database";
    char __stu_database_location__[] = "students";

    char __operation__[256];

    int clct = __GetFileLineCount__(__database_location__);
    int slct = __GetFileLineCount__(__stu_database_location__);
    int sid = 0, op;

    __InitClassDataSheet__(cd, clct);

    __GetClassData__(cd, clct, __database_location__);
    __GetStudentData__(s, slct, __stu_database_location__);

    printf("Input your student ID\n");
    scanf("%d", &sid);
    op = __Register_In_Stu_Database__(sid, s, &slct);


    scanf("%s", __operation__);

    if (strcmp(__operation__, "SHOW") == 0)
    {
        scanf("%s", __operation__);
        if (strcmp(__operation__, "CLASS") == 0)
        {
            for (int i = 0; i < clct; i++)
            {
                //printf("%d\n", strlen((s[i]).__classname__));
                printf("%s %d %d %d\n", (cd[i]).__classname__, (cd[i]).__points__, (cd[i]).__seats__, (cd[i]).__seats_remain__);
            }
        }
        else if (strcmp(__operation__, "STUDENT") == 0)
        {
            for (int i = 0; i < slct; i++)
            {
                printf("%d %d ", (s[i]).id, (s[i]).sumpoints);
                for(int j = 0; j < s[i].numofchosenclasses; j++)
                    printf("%s ", s[i].class[j]);
                printf("\n");
            }
        }
    }
    else if (strcmp(__operation__, "CHOOSE") == 0)
    {
        scanf("%s", __operation__);
        __ACTION__(__CHOOSE_CLASS__(__operation__, cd, clct));
    }
    writeStudent(s, slct, __stu_database_location__);
    writeClass(cd, clct, __database_location__);
    //printf("%s\n", __operation__);
    //printf("%d", strlen(__operation__));
    return 0;
}