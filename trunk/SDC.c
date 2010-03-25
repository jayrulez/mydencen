/*Smiles Dental Centre
  BSCIT
  Group Members:      Name                 Id#
  						Dyonne Duberry        0802189
                  Xavier Lowe           0802488

	(c)2010
*/
#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

//Admin Login
#define AdminName "administrator"
#define AdminPassword "12345"

//User Type
#define USER_ADMIN 0x1001
#define USER_BASIC 0x1002

typedef struct PATIENT
{
    int Id;
    char Fname[20];
    char Lname[20];
    char Address[20];
    int Phone;
    char LastTreatment[8];
    char Allergies[50];
    char NextAppDate[10];
    float CardBalance;
}Patient;

typedef struct PAYMENT
{
    float cash;
    float card;
}Payment;

typedef struct VISIT
{
    int PatientID;
    int DoctorID;
    int Procedure;
    Payment VisitPayment;
}Visit;

typedef struct DOCTOR
{
    int Id;
    char Fname[20];
    char Lname[20];
    int Phone;
    char Specialty[20];
}Doctor;


//Welcome
void WelcomeScreen(void);
//Login
int LoginMenu(void);
//Tools
void gotoxy(int, int);
void ScreenFrame(void);
//Services
void DefaultService(void);
//Functions
void GenIncomeReport (void);
void DocIncomeReport (void);
void DocReport (void);
void PatientNotiReport (void);


int main()
{
    LoginMenu();
    getch();
    return 0;
}
void WelcomeScreen(void)
{
}
/*
 * Login Menu
 * void
 * return boolean value
*/
int LoginMenu(void)
{
    char UserName[50];
    char Password[15];

    do{
        DefaultService();
        gotoxy(20,8);
        printf("UserName:");
        gotoxy(20,10);
        printf("Password:");
        gotoxy(30,8);
        scanf("%s",UserName);
        gotoxy(30,10);
        scanf("%s",Password);

        if(strcmp(UserName,AdminName)==0)
        {
            gotoxy(25,3);
            printf("Login Successfull!");
            return USER_ADMIN;
        }
        else
        {
            FILE *UserFile;
            char name[50];
            char pass[15];
            UserFile = fopen("./DataFiles/UserFile.txt","+r");
            if(UserFile)
            {
                while(UserFile!=NULL)
                {
                    fscanf(UserFile,"%s\t%s",name,pass);
                    if(strcmp(name,AdminName)==0 && strcmp(pass,Password)==0)
                    {
                        return USER_BASIC;
                    }
                }
                fclose(UserFile);
            }
        }
        gotoxy(20,3);
        printf("Failed: Incorrect username or password");
        getch();
    }while(true);
    return 0;
}


void GenIncomeReport (void)
{
}

void DocIncomeReport (void)
{
}

void DocReport (void)
{
}

void PatientNotiReport (void)
{
}



/*
 * Positions cursor in the cosole
 * "x" is the x cordinates. "y" y cordinate
 * returns void
*/
void gotoxy(int x, int y)
{
    //create system Api handle instance
    HANDLE handle;
    //create an instance of COORD structure
    COORD CursorCoord;
    CursorCoord.X = x;
    CursorCoord.Y = y;
    //get an output handle to console
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    //set the cursor position based on the above x and y values
    SetConsoleCursorPosition(handle,CursorCoord);
}
void ScreenFrame(void)
{
    for(int x=0;x<80;x++)
    {
        gotoxy(x,0);
        printf("%c",(unsigned char) 205);
        gotoxy(x,24);
        printf("%c",(unsigned char) 205);
    }
    for(int y=0;y<24;y++)
    {
        gotoxy(0,y);
        printf("%c",(unsigned char) 186);
        gotoxy(79,y);
        printf("%c",(unsigned char) 186);
    }
    gotoxy(0,0);
    printf("%c",(unsigned char)201);
    gotoxy(79,0);
    printf("%c",(unsigned char)187);
    gotoxy(0,24);
    printf("%c",(unsigned char)200);
    gotoxy(79,24);
    printf("%c",(unsigned char)188);
    gotoxy(0,0);
}
void DefaultService(void)
{
    system("cls");
    ScreenFrame();
}
