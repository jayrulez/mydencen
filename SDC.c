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
#define AdminName "admin"
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

// Menus
void MainMenu(void);
void Reports(void);
void Patients(void);
void DocReportSelect(void);

//Login
int LoginMenu(void);

//Tools
int OptionDriver(int,int);
void gotoxy(int, int);
void ScreenFrame(void);

//Services
void DefaultService(void);

//Reports
void GenIncomeReport (void);
void DocIncomeReport (void);
void DocReport (void);
void PatientNotiReport (void);


int main()
{
    LoginMenu();
    MainMenu();
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
int OptionDriver(int x,int y)
{

    gotoxy(x,y);
    printf("Option: ");

}
void MainMenu(void)
{
    DefaultService();

    gotoxy(35,8);
    printf("MAIN MENU");

    gotoxy(19,13);
    printf("[1]");
    gotoxy(16,14);
    printf("PATIENTS");

    gotoxy(33,13);
    printf("[2]");
    gotoxy(31,14);
    printf("REPORTS");

    gotoxy(48,13);
    printf("[3]");
    gotoxy(44,14);
    printf("UPDATE FEES");

    gotoxy(61,13);
    printf("[4]");
    gotoxy(61,14);
    printf("EXIT");
}

void Reports(void)
{
    DefaultService();
    gotoxy(20,8);
    printf("[1]Patient Notification Report");
    gotoxy(29,8);
    printf("[2]Doctors Report");
    gotoxy(38,8);
    printf("[3]General Income Report");
    gotoxy(20,13);
    printf("[4]Doctors Income Report");
    gotoxy(29,13);
    printf("[5]Individual Doctor's Report");
    gotoxy(38,13);
    printf("[esc]Return To Main Menu");
}

void Patients(void)
{
    DefaultService();
    gotoxy(20,8);
    printf("[1]Existing Patient");
    gotoxy(29,8);
    printf("[2]Add New Patient");
    gotoxy(38,8);
    printf("[esc]Return To Main Menu");

}

void DocReportSelect(void)
{
    DefaultService();
    gotoxy(20,8);
    printf("Select Doctor");
    gotoxy(29,8);
    printf("[1]Dental Surgeon");
    gotoxy(38,8);
    printf("[2] Orthodontist");
	gotoxy(20,13);
    printf("[3]Dentist");
    gotoxy(29,13);
    printf("[esc]Return To Main Menu");

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
