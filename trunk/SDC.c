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
#define USER_ADMIN 1001
#define USER_BASIC 1002
//Menu Option Types
#define NUMERIC 2000
#define ALPHABETIC 2001

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
void ShowMainMenu(void);
int MainMenuController(char);
//Report Menu
void ShowReportsMenu(void);
//PatientsMenu
void ShowPatientsMenu(void);
//DoctorReportMenu
void ShowDocReportSelect(void);
//Update Fees Menu
void ShowUpdateFeesMenu(void);


//Login
int LoginMenu(void);

//Tools
char GetChar(void);
char OptionDriver(int,int,int);
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
    ShowMainMenu();
    do
    {
    }while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
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
char OptionDriver(int x,int y,int OptionType)
{
    char option;
    gotoxy(x,y);
    printf("Option: ");
    while(true)
    {
        option = GetChar();
        if(OptionType==NUMERIC)
        {
            if(isdigit(option))
            {
                break;
            }
        }
        else if(OptionType==ALPHABETIC)
        {
            if(isalpha(option))
            {
                break;
            }
        }
    }
    return option;
}
void ShowMainMenu(void)
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
int MainMenuController(char option)
{
    switch(option)
    {
        case '1':
            ShowPatientsMenu();
        break;
        case '2':
            ShowReportsMenu();
        break;
        case '3':
        break;
        case '4':
        break;
    }
    return 0;
}
void ShowReportsMenu(void)
{
    DefaultService();
    gotoxy(30,8);
    printf("REPORTS MENU");
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

void ShowPatientsMenu(void)
{
    DefaultService();
    gotoxy(33,8);
    printf("PATIENT MENU");
    gotoxy(19,12);
    printf("[1]Existing Patient");
    gotoxy(43,12);
    printf("[2]Add New Patient");
    gotoxy(27,14);
    printf("[esc]Return To Main Menu");

}


void ShowDocReportSelectMenu(void)
{
    DefaultService();
    gotoxy(25,8);
    printf("DOCTOR'S REPORT MENU");
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


void ShowUpdateFeesMenu(void)
{
    DefaultService();
    gotoxy(20,8);
    printf("[1]Dental Examination");
    gotoxy(29,8);
    printf("[2]Cleaning");
    gotoxy(38,8);
    printf("[3]Extraction");
    gotoxy(20,13);
    printf("[4]Filling");
    gotoxy(29,13);
    printf("[5]X Ray");
    gotoxy(38,13);
	printf("[6]Braces");
	gotoxy(38,19);
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
 * Gets a character from the keyboard
 * no parameters
 * returns ascii key character
*/
char GetChar(void)
{
    HANDLE Handle;
    DWORD AmountRead;
    INPUT_RECORD InputRecord;
    Handle = GetStdHandle(STD_INPUT_HANDLE);
    while(true)
    {
        ReadConsoleInput(Handle,&InputRecord,1,&AmountRead);
        if(InputRecord.EventType==KEY_EVENT)
        {
            if(InputRecord.Event.KeyEvent.bKeyDown)
            {
                return InputRecord.Event.KeyEvent.uChar.AsciiChar;
            }
        }
    }
    return '0';
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
