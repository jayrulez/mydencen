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
#include <dos.h>

//Admin Login
#define AdminName "admin"
#define AdminPassword "12345"

//User Type
#define USER_ADMIN 1001
#define USER_BASIC 1002

//Menu Option Types
#define NUMERIC 2000
#define ALPHABETIC 2001

//Payment Type
#define CASH_PAYEMNT 0
#define CARD_PAYMENT 1

typedef INPUT_RECORD KEY_RECORD;
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
typedef struct PROCEDURE
{
    int Code;
    char Name[20];
    float Cost;
}Procedure;
typedef struct VISIT
{
    int PatientID;
    int DoctorID;
    int ProcedureCode;
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

//Create Files
int CreateFiles(void);
//Welcome
void WelcomeScreen(void);
// Menus
void ShowMainMenu(void);
int MainMenuController(char);
//Report Menu
void ShowReportsMenu(void);
int ReportsMenuController(char);
//PatientsMenu
void ShowPatientsMenu(void);
int PatientsMenuController(char);
int ShowExistingPatientMenuController(char);
int AddNewPatient (Patient *);
int AddPatientToFile(Patient);
void ShowAddPatientsMenu(void);
int AddPatientsMenuController(char,Patient *);
int PatientSearch(void);
void ShowViewPatientRecord(void);
int ViewPatientRecordMenuController(char);
int FindAndShowPatient(int);
void ShowNewPatientVisitMenu(void);
int NewPatientVisitMenuController(char,Visit*);
int NewPatientVisit(Visit *);
int ProcessVisitTransaction(Visit *,Patient *,int);
int ShowPaymentOptionsMenu(Visit *);
int AddPatientVisitToFile(Visit);
//DoctorReportMenu
void ShowDocReportSelect(void);
//Update Fees Menu
void ShowUpdateFeesMenu(void);
int UpdateFeesMenuController(char);


//Login
int LoginMenu(void);

//Tools
KEY_RECORD GetChar(void);
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


int main(void)
{
    CreateFiles();
    LoginMenu();
    ShowMainMenu();
    do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
    return 0;
}
int CreateFiles(void)
{
    FILE *PatientStream;
    FILE *VisitStream;
    FILE *ProcedureStream;
    DefaultService();
    mkdir("./DataFiles");
    PatientStream = fopen("./DataFiles/Patients.txt","r");
    VisitStream = fopen("./DataFiles/PatientVisit.txt","r");
    ProcedureStream = fopen("./DataFiles/Procedure.txt","r");
    if(!PatientStream && !VisitStream && !ProcedureStream)
    {
        PatientStream = fopen("./DataFiles/Patients.txt","a");
        VisitStream = fopen("./DataFiles/PatientVisit.txt","a");
        ProcedureStream = fopen("./DataFiles/Procedure.txt","a");
        gotoxy(25,6);
        printf("Error: Files do not exist!");
        Sleep(700);
        gotoxy(30,8);
        printf("Creating Files...");
        if(PatientStream && VisitStream && ProcedureStream)
        {
            Sleep(500);
            gotoxy(40,10);
            printf("1. Patients.txt");
            Sleep(500);
            gotoxy(40,11);
            printf("2. PatientVisit.txt");
            Sleep(500);
            gotoxy(40,12);
            printf("3. Procedure.txt");
            Sleep(700);
            gotoxy(30,15);
            printf("All Files Created!");
            gotoxy(27,21);
            printf("Press any key to Continue");
            fclose(VisitStream);
            fclose(PatientStream);
            fclose(ProcedureStream);
            fflush(stdin);
            GetChar();
            return 1;
        }
        else
        {
            Sleep(700);
            gotoxy(30,13);
            printf("Error: File Creation Failed!");
            gotoxy(30,21);
            printf("Press any key to Exit");
            GetChar();
            exit(0);
            return 0;
        }
    }
    else
    {
        PatientStream = fopen("./DataFiles/Patients.txt","a");
        VisitStream = fopen("./DataFiles/PatientVisit.txt","a");
        ProcedureStream = fopen("./DataFiles/Procedure.txt","a");
        if(PatientStream && VisitStream && ProcedureStream)
        {
            fclose(VisitStream);
            fclose(PatientStream);
            fclose(ProcedureStream);
            return 1;
        }
        else
        {
            gotoxy(20,9);
            printf("Warning: Files are in READ-ONLY mode.");
            gotoxy(20,11);
            printf("Changes made to records will not be saved.");
            gotoxy(27,16);
            printf("Press any key to Continue");
            GetChar();
        }
    }
    return 1;
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
    DefaultService();
    do{
        gotoxy(25,12);
        printf("UserName:");
        gotoxy(25,14);
        printf("Password:");
        gotoxy(35,12);
        scanf("%s",UserName);
        gotoxy(35,14);
        scanf("%s",Password);

        if(strcmp(UserName,AdminName)==0 && strcmp(AdminPassword,Password)==0)
        {
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
        DefaultService();
        gotoxy(20,3);
        printf("Failed: Incorrect username or password");
    }while(1);
    return 0;
}
char OptionDriver(int x,int y,int OptionType)
{
    KEY_RECORD KeyRecord;
    char option;
    gotoxy(x,y);
    printf("Option: ");
    while(true)
    {

        KeyRecord = GetChar();
        option = KeyRecord.Event.KeyEvent.uChar.AsciiChar;
        if(OptionType==NUMERIC)
        {
            if(isdigit(option))
                break;
            else
                return KeyRecord.Event.KeyEvent.wVirtualKeyCode;
        }
        else if(OptionType==ALPHABETIC)
        {
            if(isalpha(option))
                break;
            else
                return (char)KeyRecord.Event.KeyEvent.wVirtualKeyCode;
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
            do{}while(PatientsMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case '2':
            ShowReportsMenu();
            do{}while(ReportsMenuController(OptionDriver(30,21,NUMERIC))==0);
        break;
        case '3':
            ShowUpdateFeesMenu();
            do{}while(UpdateFeesMenuController(OptionDriver(30,21,NUMERIC))==0);
        break;
        case '4':
        break;
    }
    return 0;
}
void ShowReportsMenu(void)
{
    DefaultService();
    gotoxy(30,5);
    printf("REPORTS MENU");
    gotoxy(20,8);
    printf("[1]Patient Notification Report");
    gotoxy(20,10);
    printf("[2]Doctors Report");
    gotoxy(20,12);
    printf("[3]General Income Report");
    gotoxy(20,14);
    printf("[4]Doctors Income Report");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
}
int ReportsMenuController(char option)
{
    switch(option)
    {
        case '1':
        PatientNotiReport();
            do{}while(PatientsMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case '2':
        DocReport();
            do{}while(PatientsMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case '3':
        GenIncomeReport();
            do{}while(PatientsMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case '4':
        DocIncomeReport();
            do{}while(PatientsMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case (char)VK_ESCAPE:
            ShowMainMenu();
            do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
    }
    return 0;
}
void ShowPatientsMenu(void)
{
    DefaultService();
    gotoxy(33,4);
    printf("PATIENT MENU");
    gotoxy(25,7);
    printf("[1]New Patient Visit");
    gotoxy(25,9);
    printf("[2]Add New Patient");
    gotoxy(25,11);
    printf("[3]View Patient Record");
    gotoxy(25,13);
    printf("[4]Update Patient Record");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
}
int PatientsMenuController(char option)
{
    switch(option)
    {
        case '1':
            Visit NewV;
            int Result;
            Visit *NewVisit = &NewV;
            if(NewPatientVisit(NewVisit))
            {
                Result = ShowPaymentOptionsMenu(NewVisit);
                if(Result == 1)
                {
                /*
                ShowNewPatientVisitMenu();
                do{}while(NewPatientVisitMenuController(OptionDriver(30,21,NUMERIC),NewVisit)==0);
                */
                }
                else if(Result == 0)
                {
                    gotoxy(20,10);
                    printf("Error: Procedure Code not found in files.");
                }
                if(Result == -1)
                {
                    gotoxy(30,10);
                    printf("Error: Patient does not exist.");
                }
                if(Result == -2)
                {
                    gotoxy(30,10);
                    printf("Error: Doctor does not exist.");
                }
            }
        break;
        case '2':
            Patient NewP;
            Patient *NewPatient = &NewP;
            if(AddNewPatient(NewPatient))
            {
                ShowAddPatientsMenu();
                do{}while(AddPatientsMenuController(OptionDriver(30,21,NUMERIC),NewPatient)==0);
            }
            else
            {
            }
        break;
        case '3':
            FindAndShowPatient(PatientSearch());
            ShowViewPatientRecord();
            do{}while(ViewPatientRecordMenuController(OptionDriver(30,21,NUMERIC))==0);
        break;
        case '4':
        break;
        case (char)VK_ESCAPE:
            ShowMainMenu();
            do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
    }
    return 0;
}
int AddNewPatient (Patient *NewPatient)
{
    FILE *PatientStream;
    Patient TempPatient;
    DefaultService();
    gotoxy(26,4);
    printf("ADD NEW PATIENT");
    gotoxy(20,8);
    printf("Fist Name   :");
    gotoxy(20,10);
    printf("Last Name   :");
    gotoxy(20,12);
    printf("Address     :");
    gotoxy(20,14);
    printf("Phone Number:");
    gotoxy(20,16);
    printf("Allergies   :");
    gotoxy(20+14,8);
    fflush(stdin);
    scanf("%s",NewPatient->Fname);
    gotoxy(20+14,10);
    scanf("%s",NewPatient->Lname);
    gotoxy(20+14,12);
    scanf("%s",NewPatient->Address);
    gotoxy(20+14,14);
    scanf("%d",&(NewPatient->Phone));
    gotoxy(20+14,16);
    scanf("%s",NewPatient->Allergies);
    TempPatient.Id = 1;
    NewPatient->Id = 1;
    strcpy(NewPatient->NextAppDate,"N/A");
    strcpy(NewPatient->LastTreatment,"N/A");
    NewPatient->CardBalance = 0.00;
    PatientStream = fopen("./DataFiles/Patients.txt","r");
    if(!PatientStream)
    {
        fclose(PatientStream);
        return 0;
    }
    else
    {
        while(!feof(PatientStream))
        {
            fscanf(PatientStream,"%d %s %s %s %d %s %s %s %f",&TempPatient.Id,TempPatient.Fname,TempPatient.Lname,
            TempPatient.Address,&TempPatient.Phone,TempPatient.Allergies,TempPatient.LastTreatment,
            TempPatient.NextAppDate,&TempPatient.CardBalance);
            NewPatient->Id = TempPatient.Id + 1;
        }
    }
    fclose(PatientStream);
    return 1;
}
int AddPatientToFile(Patient NewPatient)
{
    FILE * PatientStream;
    PatientStream = fopen("./DataFiles/Patients.txt","a");
    if(!PatientStream)
    {
        fclose(PatientStream);
        return 0;
    }
    else
    {
        if(fprintf(PatientStream,"%d %s %s %s %d %s %s %s %.2f\n",NewPatient.Id,NewPatient.Fname,NewPatient.Lname,
        NewPatient.Address,NewPatient.Phone,NewPatient.Allergies,NewPatient.LastTreatment,
        NewPatient.NextAppDate,NewPatient.CardBalance)==9)
        {
        }
        fclose(PatientStream);
    }
    return 1;
}

void ShowAddPatientsMenu(void)
{
    gotoxy(30,19);
    printf("[1]Save");
    gotoxy(42,19);
    printf("[2]Cancel");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
}
int AddPatientsMenuController(char option,Patient *NewPatient)
{
    switch(option)
    {
        case '1':
            AddPatientToFile(*NewPatient);
            ShowPatientsMenu();
            do{}while(PatientsMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case '2':
            ShowPatientsMenu();
            do{}while(PatientsMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case (char)VK_ESCAPE:
            ShowMainMenu();
            do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
    }
    return 0;
}
int NewPatientVisit(Visit *NewVisit)
{
    FILE *VisitStream;
    Patient TempPatient;
    DefaultService();
    gotoxy(26,4);
    printf("NEW PATIENT VISIT");
    gotoxy(20,8);
    printf("Patient ID       :");
    gotoxy(20,10);
    printf("Doctor ID        :");
    gotoxy(20,12);
    printf("Procedure Code   :");

    fflush(stdin);
    gotoxy(20+14,8);
    scanf("%d",&NewVisit->DoctorID);
    gotoxy(20+14,10);
    scanf("%d",&NewVisit->PatientID);
    gotoxy(20+14,12);
    scanf("%d",&NewVisit->ProcedureCode);
    return 1;
}
int ShowPaymentOptionsMenu(Visit *NewVisit)
{
    FILE *ProcedureStream;
    Procedure TempProcedure;
    ProcedureStream = fopen("./DataFiles/Procedure.txt","r");
    if(!ProcedureStream)
    {
        fclose(ProcedureStream);
        return 0;
    }
    else
    {
        while(!feof(ProcedureStream))
        {
            fscanf(ProcedureStream,"%d %s %f",&TempProcedure.Code,TempProcedure.Name,&TempProcedure.Cost);
            if(TempProcedure.Code == NewVisit->ProcedureCode)
            {
                gotoxy(27,8);
                printf("Procedure Code:");
                gotoxy(27,10);
                printf("Procedure Name:");
                gotoxy(37,13);
                printf("Cost: $%.2f",TempProcedure.Cost);
                fclose(ProcedureStream);
                return 1;
            }
        }
    }
    fclose(ProcedureStream);
    return 0;
}
int ProcessVisitTransaction(Visit *NewVisit,Patient *ExistingPatient,int PaymentType)
{
    FILE *ProcedureStream;
    FILE *PatientStream;
    Procedure TempProcedure;
    float VisitCost;
    float CardCoverage;
    float CashCoverage;
    ProcedureStream = fopen("./DataFiles/Procedure.txt","r");
    PatientStream = fopen("./DataFiles/Patients.txt","r");
    if(!ProcedureStream || !PatientStream)
    {
        fclose(ProcedureStream);
        return 0;
    }
    else
    {
        while(!feof(PatientStream))
        {
            fscanf(PatientStream,"%d %s %s %s %d %s %s %s %f",&ExistingPatient->Id,ExistingPatient->Fname,ExistingPatient->Lname,
            ExistingPatient->Address,&ExistingPatient->Phone,ExistingPatient->Allergies,ExistingPatient->LastTreatment,
            ExistingPatient->NextAppDate,&ExistingPatient->CardBalance);
            if(ExistingPatient->Id == NewVisit->PatientID)
            {
                break;
            }
        }
        while(!feof(ProcedureStream))
        {
            fscanf(ProcedureStream,"%d %s %f",&TempProcedure.Code,TempProcedure.Name,&TempProcedure.Cost);
            if(TempProcedure.Code == NewVisit->ProcedureCode)
            {
                VisitCost = TempProcedure.Cost;
                CardCoverage = ExistingPatient->CardBalance * 0.85;
                if(PaymentType == CARD_PAYMENT && (ExistingPatient->CardBalance>=CardCoverage))
                {
                    CashCoverage = VisitCost - CardCoverage;
                    NewVisit->VisitPayment.card = CardCoverage;
                    NewVisit->VisitPayment.cash = CashCoverage;
                }
                else
                {
                    CardCoverage = 0.0;
                    CashCoverage = VisitCost;
                    NewVisit->VisitPayment.card = CardCoverage;
                    NewVisit->VisitPayment.cash = CashCoverage;
                }
                break;
            }
        }
    }
    return 1;
}
int AddPatientVisitToFile(Visit NewVisit)
{
    FILE * VisitStream;
    VisitStream = fopen("./DataFiles/PatientVisit.txt","r");
    if(!VisitStream)
    {
        fclose(VisitStream);
        return 0;
    }
    else
    {
        while(!feof(VisitStream))
        {
            fprintf(VisitStream,"%d %d %d %f %f",&NewVisit.DoctorID,&NewVisit.PatientID,
            &NewVisit.ProcedureCode,&NewVisit.VisitPayment.card,&NewVisit.VisitPayment.cash);
        }
    }
    fclose(VisitStream);
    return 1;
}
void ShowNewPatientVisitMenu(void)
{
    gotoxy(30,19);
    printf("[1]Save");
    gotoxy(42,19);
    printf("[2]Cancel");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
}
int NewPatientVisitMenuController(char option, Visit *NewVisit)
{
}
int PatientSearch(void)
{
    int id;
    DefaultService();
    gotoxy(30,4);
    printf("PATIENT SEARCH");
    gotoxy(20,11);
    printf("Patient Id:");
    gotoxy(20+12,11);
    scanf("%d",&id);
    return id;
}
int FindAndShowPatient(int Id)
{
    FILE * PatientStream;
    Patient TempPatient;
    DefaultService();
    PatientStream = fopen("./DataFiles/Patients.txt","r");
    if(!PatientStream)
    {
        fclose(PatientStream);
        return 0;
    }
    else
    {
        while(!feof(PatientStream))
        {
            fscanf(PatientStream,"%d %s %s %s %d %s %s %s %f",&TempPatient.Id,TempPatient.Fname,TempPatient.Lname,
            TempPatient.Address,&TempPatient.Phone,TempPatient.Allergies,TempPatient.LastTreatment,
            TempPatient.NextAppDate,&TempPatient.CardBalance);
            if(TempPatient.Id==Id)
            {
                gotoxy(17,8);
                printf("Patient Id    : %d",TempPatient.Id);
                gotoxy(17,9);
                printf("First Name    : %s",TempPatient.Fname);
                gotoxy(17,10);
                printf("Last Name     : %s",TempPatient.Lname);
                gotoxy(17,11);
                printf("Address       : %s",TempPatient.Address);
                gotoxy(17,12);
                printf("Phone Number  : %d",TempPatient.Phone);
                gotoxy(17,13);
                printf("Allergies     : %s",TempPatient.Allergies);
                gotoxy(17,14);
                printf("Last Treatment: %s",TempPatient.LastTreatment);
                gotoxy(17,15);
                printf("Next AppDate  : %s",TempPatient.NextAppDate);
                gotoxy(17,16);
                printf("Card Balance  : %.2f",TempPatient.CardBalance);
                return 1;
            }
        }
        gotoxy(20,9);
        printf("Record not found!");

    }
    fclose(PatientStream);
    return -1;
}
void ShowViewPatientRecord(void)
{
    gotoxy(30,4);
    printf("VIEW PATIENT MENU");
    gotoxy(20,19);
    printf("[1]Patient Menu");
    gotoxy(40,19);
    printf("[2]<--Search Patient");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
}
int ViewPatientRecordMenuController(char option)
{
    switch(option)
    {
        case '1':
            ShowPatientsMenu();
            do{}while(PatientsMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case '2':
            FindAndShowPatient(PatientSearch());
            ShowViewPatientRecord();
            do{}while(ViewPatientRecordMenuController(OptionDriver(30,21,NUMERIC))==0);
        break;
        case (char)VK_ESCAPE:
            ShowMainMenu();
            do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
    }
    return 0;
}
void ShowDocReportSelectMenu(void)
{
    DefaultService();
    gotoxy(25,5);
    printf("DOCTOR'S REPORT MENU");
    gotoxy(20,8);
    printf("Select Doctor");
    gotoxy(29,8);
    printf("[1]Dental Surgeon");
    gotoxy(38,8);
    printf("[2] Orthodontist");
	gotoxy(20,13);
    printf("[3]Dentist");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");

}
void ShowUpdateFeesMenu(void)
{
    DefaultService();
    gotoxy(27,5);
    printf("UPDATE FEES MENU");
    gotoxy(20,8);
    printf("[1]Dental Examination");
    gotoxy(20,10);
    printf("[2]Cleaning");
    gotoxy(20,12);
    printf("[3]Extraction");
    gotoxy(20,14);
    printf("[4]Filling");
    gotoxy(20,16);
    printf("[5]X Ray");
    gotoxy(20,18);
	printf("[6]Braces");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
}

int UpdateFeesMenuController(char option)
{
    switch(option)
    {
        case '1':
        break;
        case '2':
        break;
        case '3':
        break;
        case (char)VK_ESCAPE:
            ShowMainMenu();
            do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
    }
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
 * Gets a character from the keyboard
 * no parameters
 * returns ascii key character
*/
KEY_RECORD GetChar(void)
{
    HANDLE Handle;
    DWORD AmountRead;
    KEY_RECORD KeyRecord;
    Handle = GetStdHandle(STD_INPUT_HANDLE);
    while(true)
    {
        ReadConsoleInput(Handle,&KeyRecord,1,&AmountRead);
        if(KeyRecord.EventType==KEY_EVENT)
        {
            if(KeyRecord.Event.KeyEvent.bKeyDown)
            {
                return KeyRecord;//InputRecord.Event.KeyEvent.uChar.AsciiChar;
            }
        }
    }
    return KeyRecord;
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
