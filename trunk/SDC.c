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
#define PASSWORD_LENGTH 30

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
    char Phone[15];
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
    char Name[40];
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
    char Phone[15];
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
void ShowInReportMenu(void);
int InReportMenuController(char);
void ShowDocReportSelectMenu(void);
int DocReportSelectMenuController(char);
void DocReport(char);
void GenIncomeReport(void);
void PatientNotiReport(void);
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
int ProcessVisitTransaction(Visit *,Patient *);
void ShowFailPaymentOptionsMenu(void);
int FailPaymentOptionsMenuController(char);
void ShowPaymentOptionsMenu(void);
int PaymentOptionsMenuController(char,Visit*,Patient*);
int AddPatientVisitToFile(Visit,Patient);
void ShowAfterAddPatientVisitMenu(void);
int AfterAddPatientVisitMenuController(char);
int IfPatientExist(int);
int IfDoctorExist(int);
int IfProcedureExist(int);
int UpdatePatient(int);
void ShowUpdatePatientMenu(void);
int UpdatePatientMenuController(char,Patient);
int UpdateRecordInPatientFile(Patient);
void ShowSearchPatientMenu(void);
//DoctorReportMenu
void ShowDocReportSelect(void);
//Update Fees Menu
void ShowUpdateFeesMenu(void);
int UpdateFeesMenuController(char);
void ShowVerifyUpdateProcedureFee(void);
int VerifyUpdateProcedureFeeMenuController(char,int,float);
int VerifyUpdateProcedureFee(int,float);
void ShowAfterUpdateFeeMenu(void);
int AfterUpdateFeeMenuController(char);
int FindAndShowProcedure(int ,int, int);
char* maskedInput(void);


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
int GenIncomeMenuController(char);
int DocIncomeMenuController(char);
int DocReportMenuController(char);
int PatientNotiMenuController(char);


int main(void)
{
    CreateFiles();
    LoginMenu();
    ShowMainMenu();
    do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
    return 0;
}

char* maskedInput(void)
{
    KEY_RECORD input;
    char buffer[PASSWORD_LENGTH];
    static char ret[PASSWORD_LENGTH];

    int position = 0;
    do
    {
        input = GetChar();
        if((isalnum(input.Event.KeyEvent.uChar.AsciiChar) || ispunct(input.Event.KeyEvent.uChar.AsciiChar) || input.Event.KeyEvent.uChar.AsciiChar == VK_SPACE) && position < PASSWORD_LENGTH)
        {
            printf("*");
            buffer[position] = input.Event.KeyEvent.uChar.AsciiChar;
            position++;
        }
        if(input.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
        {
            if(position > 0)
            {
                position--;
                buffer[position] = '\0';
                printf("\b \b");
            }
        }
        if(input.Event.KeyEvent.uChar.AsciiChar == VK_RETURN)
        {
            buffer[position] = '\0';
            strcpy(ret, buffer);

            return ret;
        }
    }while(1);
}

int CreateFiles(void)//Creates Data Files Directory & txt files with default values
{
    FILE *PatientStream;
    FILE *VisitStream;
    FILE *ProcedureStream;
    FILE * DoctorStream;
    DefaultService();
    mkdir("./DataFiles");
    PatientStream = fopen("./DataFiles/Patients.txt","r");
    VisitStream = fopen("./DataFiles/PatientVisit.txt","r");
    ProcedureStream = fopen("./DataFiles/Procedure.txt","r");
    DoctorStream = fopen("./DataFiles/Doctors.txt","r");
    if(!PatientStream && !VisitStream && !ProcedureStream && !DoctorStream)
    {
        PatientStream = fopen("./DataFiles/Patients.txt","a");
        VisitStream = fopen("./DataFiles/PatientVisit.txt","a");
        ProcedureStream = fopen("./DataFiles/Procedure.txt","a");
        DoctorStream = fopen("./DataFiles/Doctors.txt","a");
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
            char PatientHeader[9][40] = {{"Id"},{"First_Name"},{"Last_Name"},{"Address"},{"Phone_Number"},{"Allergies"},
            {"Last_Treatment"},{"Next_Appointment_Date"},{"Card_Balance"}};
            fprintf(PatientStream,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",PatientHeader[0],PatientHeader[1],PatientHeader[2],
            PatientHeader[3],PatientHeader[4],PatientHeader[5],PatientHeader[6],PatientHeader[7],PatientHeader[8]);
            Sleep(500);
            gotoxy(40,11);
            printf("2. PatientVisit.txt");
            char VisitHeader[5][30] = {{"Patient_Id"},{"Doctor_Id"},{"Procedure_Code"},{"Card_Payment($)"},{"Cash_Payment($)"}};
            fprintf(VisitStream,"%s\t%s\t%s\t%s\t%s\n",VisitHeader[0],VisitHeader[1],VisitHeader[2],VisitHeader[3],VisitHeader[4]);
            Sleep(500);
            gotoxy(40,12);
            printf("3. Procedure.txt");
            char ProcedureHeader[3][20] = {{"Code"},{"Name"},{"Cost($)"}};
            fprintf(ProcedureStream,"%s\t%s\t%s\n",ProcedureHeader[0],ProcedureHeader[1],ProcedureHeader[2]);

            Procedure DefaultProcedure;
            DefaultProcedure.Code = 1001;strcpy(DefaultProcedure.Name,"Dental-Examination");DefaultProcedure.Cost = 5500.00;
            fprintf(ProcedureStream,"%d\t%s\t%.2f\n",DefaultProcedure.Code,DefaultProcedure.Name,DefaultProcedure.Cost);

            DefaultProcedure.Code = 1002;strcpy(DefaultProcedure.Name,"Cleaning");DefaultProcedure.Cost = 7500.00;
            fprintf(ProcedureStream,"%d\t%s\t%.2f\n",DefaultProcedure.Code,DefaultProcedure.Name,DefaultProcedure.Cost);

            DefaultProcedure.Code = 1003;strcpy(DefaultProcedure.Name,"Extraction");DefaultProcedure.Cost = 11500.00;
            fprintf(ProcedureStream,"%d\t%s\t%.2f\n",DefaultProcedure.Code,DefaultProcedure.Name,DefaultProcedure.Cost);

            DefaultProcedure.Code = 1004;strcpy(DefaultProcedure.Name,"Fillings");DefaultProcedure.Cost = 12000.00;
            fprintf(ProcedureStream,"%d\t%s\t%.2f\n",DefaultProcedure.Code,DefaultProcedure.Name,DefaultProcedure.Cost);

            DefaultProcedure.Code = 1005;strcpy(DefaultProcedure.Name,"Xray");DefaultProcedure.Cost = 2800.00;
            fprintf(ProcedureStream,"%d\t%s\t%.2f\n",DefaultProcedure.Code,DefaultProcedure.Name,DefaultProcedure.Cost);

            DefaultProcedure.Code = 1006;strcpy(DefaultProcedure.Name,"Braces");DefaultProcedure.Cost = 7000.00;
            fprintf(ProcedureStream,"%d\t%s\t%.2f\n",DefaultProcedure.Code,DefaultProcedure.Name,DefaultProcedure.Cost);
            Sleep(500);
            gotoxy(40,13);
            printf("4. Doctors.txt");
            char DoctorHeader[5][40] = {{"Id"},{"First_Name"},{"Last_Name"},{"Phone_Number"},{"Specialty"}};
            fprintf(DoctorStream,"%s\t%s\t%s\t%s\t%s\n",DoctorHeader[0],DoctorHeader[1],DoctorHeader[2],DoctorHeader[3],DoctorHeader[4]);

            Doctor DefaultDoctor;
            DefaultDoctor.Id=2001;strcpy(DefaultDoctor.Fname,"John");strcpy(DefaultDoctor.Lname,"Mavrick");
            strcpy(DefaultDoctor.Phone,"876-988-0833");strcpy(DefaultDoctor.Specialty,"na");
            fprintf(DoctorStream,"%d\t%s\t%s\t%d\t%s\n",DefaultDoctor.Id,DefaultDoctor.Fname,DefaultDoctor.Lname,
            DefaultDoctor.Phone,DefaultDoctor.Specialty);

            DefaultDoctor.Id=2002;strcpy(DefaultDoctor.Fname,"Alfred");strcpy(DefaultDoctor.Lname,"Mingo");
            strcpy(DefaultDoctor.Phone,"876-332-2333");strcpy(DefaultDoctor.Specialty,"na");
            fprintf(DoctorStream,"%d\t%s\t%s\t%d\t%s\n",DefaultDoctor.Id,DefaultDoctor.Fname,DefaultDoctor.Lname,
            DefaultDoctor.Phone,DefaultDoctor.Specialty);

            DefaultDoctor.Id=2003;strcpy(DefaultDoctor.Fname,"kobe");strcpy(DefaultDoctor.Lname,"loser");
            strcpy(DefaultDoctor.Phone,"876-703-6253");strcpy(DefaultDoctor.Specialty,"na");
            fprintf(DoctorStream,"%d\t%s\t%s\t%d\t%s\n",DefaultDoctor.Id,DefaultDoctor.Fname,DefaultDoctor.Lname,
            DefaultDoctor.Phone,DefaultDoctor.Specialty);

            DefaultDoctor.Id=2004;strcpy(DefaultDoctor.Fname,"Tony");strcpy(DefaultDoctor.Lname,"Montana");
            strcpy(DefaultDoctor.Phone,"876-988-2937");strcpy(DefaultDoctor.Specialty,"na");
            fprintf(DoctorStream,"%d\t%s\t%s\t%d\t%s\n",DefaultDoctor.Id,DefaultDoctor.Fname,DefaultDoctor.Lname,
            DefaultDoctor.Phone,DefaultDoctor.Specialty);

            DefaultDoctor.Id=2005;strcpy(DefaultDoctor.Fname,"Emanuel");strcpy(DefaultDoctor.Lname,"Robbery");
            strcpy(DefaultDoctor.Phone,"876-589-0012");strcpy(DefaultDoctor.Specialty,"na");
            fprintf(DoctorStream,"%d\t%s\t%s\t%d\t%s\n",DefaultDoctor.Id,DefaultDoctor.Fname,DefaultDoctor.Lname,
            DefaultDoctor.Phone,DefaultDoctor.Specialty);

            Sleep(700);
            gotoxy(30,15);
            printf("All Files Created!");
            gotoxy(27,21);
            printf("Press any key to Continue");
            fclose(VisitStream);
            fclose(PatientStream);
            fclose(ProcedureStream);
            fclose(DoctorStream);
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
        DoctorStream = fopen("./DataFiles/Doctors.txt","a");
        if(PatientStream && VisitStream && ProcedureStream && DoctorStream)
        {
            fclose(VisitStream);
            fclose(PatientStream);
            fclose(ProcedureStream);
            fclose(DoctorStream);
            return 1;
        }
        else
        {
            fclose(VisitStream);
            fclose(PatientStream);
            fclose(ProcedureStream);
            fclose(DoctorStream);
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
int LoginMenu(void) //login menu compares username and password
{
    char UserName[50];
    char Password[PASSWORD_LENGTH];
    int attempt = 0;
    int incorrect =0;
    do{
        DefaultService();
        if(incorrect == 1)
        {
            gotoxy(20,5);
            printf("Failed: Incorrect username or password\b");
        }
        attempt+=1;
        gotoxy(30,3);
        printf("Attempt %d of 3",attempt);
        gotoxy(25,12);
        printf("UserName:");
        gotoxy(25,14);
        printf("Password:");
        gotoxy(35,12);
        scanf("%s",UserName);
        gotoxy(35,14);
        strcpy(Password, maskedInput());

        if(strcmp(UserName,AdminName)==0 && strcmp(AdminPassword,Password)==0)
        {
            return USER_ADMIN;
        }
        else
        {
            incorrect = 1;
        }
    }while(attempt!=3);
    gotoxy(33,17);
    printf("System Locked!");
    gotoxy(30,20);
    printf("Press any key to Exit");
    GetChar();
    exit(0);
    return 0;
}
char OptionDriver(int x,int y,int OptionType)
{
    KEY_RECORD KeyRecord;
    char option;
    gotoxy(x,y);
    printf("Option: ");
    while(1)
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
void ShowMainMenu(void)//Displays Main Menu on screen
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
int MainMenuController(char option)//Routes the entered value to a function
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
            exit(0);
        break;
    }
    return 0;
}
void ShowReportsMenu(void)//Displays Reports Menu
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
int ReportsMenuController(char option)//Routes the entered value to a function
{
    switch(option)
    {
        case '1':
            PatientNotiReport();
            ShowInReportMenu();
            do{}while(InReportMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case '2':
            ShowDocReportSelectMenu();
            do{}while(DocReportSelectMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case '3':
            GenIncomeReport();
            ShowInReportMenu();
            do{}while(InReportMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case '4':
            DocIncomeReport();
            ShowInReportMenu();
            do{}while(InReportMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case (char)VK_ESCAPE:
            ShowMainMenu();
            do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
    }
    return 0;
}
void ShowPatientsMenu(void) //Displays Patient Menu
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
int PatientsMenuController(char option)//Routes the entered value to a function
{
    switch(option)
    {
        case '1':
            Visit NewV;
            Patient Exp;
            Patient *ExistingPatient = &Exp;
            int Result;
            Visit *NewVisit = &NewV;
            Result = NewPatientVisit(NewVisit);
            if(Result == 1)
            {
                ProcessVisitTransaction(NewVisit,ExistingPatient);
            }
            else if(Result == -3)
            {
                ShowFailPaymentOptionsMenu();
                gotoxy(26,7);
                printf("Error: Patient does not exist.");
                do{}while(FailPaymentOptionsMenuController(OptionDriver(30,20,NUMERIC))==0);
            }
            else if(Result == -2)
            {
                ShowFailPaymentOptionsMenu();
                gotoxy(26,7);
                printf("Error: Doctor does not exist.");
                do{}while(FailPaymentOptionsMenuController(OptionDriver(30,20,NUMERIC))==0);
            }
            else if(Result == -1)
            {
                ShowFailPaymentOptionsMenu();
                gotoxy(20,7);
                printf("Error: Procedure Code not found in files.");
                do{}while(FailPaymentOptionsMenuController(OptionDriver(30,20,NUMERIC))==0);
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
            UpdatePatient(PatientSearch());
            ShowSearchPatientMenu();
            do{}while(ViewPatientRecordMenuController(OptionDriver(30,21,NUMERIC))==0);
        break;
        case (char)VK_ESCAPE:
            ShowMainMenu();
            do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
    }
    return 0;
}
int IfPatientExist(int code)
{
    FILE * PatientStream;
    Patient TempPatient;
    PatientStream = fopen("./DataFiles/Patients.txt","r");
    if(!PatientStream)
    {
        return -1;
    }
    else
    {
        char PatientHeader[9][40];
        fscanf(PatientStream,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",PatientHeader[0],PatientHeader[1],PatientHeader[2],
        PatientHeader[3],PatientHeader[4],PatientHeader[5],PatientHeader[6],PatientHeader[7],PatientHeader[8]);
        while(!feof(PatientStream))
        {
            fscanf(PatientStream,"%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%f",&TempPatient.Id,TempPatient.Fname,TempPatient.Lname,
            TempPatient.Address,TempPatient.Phone,TempPatient.Allergies,TempPatient.LastTreatment,
            TempPatient.NextAppDate,&TempPatient.CardBalance);
            if(TempPatient.Id==code)
            {
                fclose(PatientStream);
                return 1;
            }
        }
    }
    fclose(PatientStream);
    return 0;
}
int IfDoctorExist(int code)
{
    FILE * DoctorStream;
    Doctor TempDoctor;
    DoctorStream = fopen("./DataFiles/Doctors.txt","r");
    if(!DoctorStream)
    {
        return -1;
    }
    else
    {
        char DoctorHeader[5][40];
        fscanf(DoctorStream,"%s\t%s\t%s\t%s\t%s",DoctorHeader[0],DoctorHeader[1],DoctorHeader[2],DoctorHeader[3],DoctorHeader[4]);
        while(!feof(DoctorStream))
        {
            fscanf(DoctorStream,"%d\t%s\t%s\t%s\t%s",&TempDoctor.Id,TempDoctor.Fname,TempDoctor.Lname,
            TempDoctor.Phone,TempDoctor.Specialty);
            if(TempDoctor.Id==code)
            {
                fclose(DoctorStream);
                return 1;
            }
        }
    }
    fclose(DoctorStream);
    return 0;
}
int IfProcedureExist(int code)
{
    FILE * ProcedureStream;
    Procedure TempProcedure;
    ProcedureStream = fopen("./DataFiles/Procedure.txt","r");
    if(!ProcedureStream)
    {
        return -1;
    }
    else
    {
        char ProcedureHeader[3][20];
        fscanf(ProcedureStream,"%s\t%s\t%s",ProcedureHeader[0],ProcedureHeader[1],ProcedureHeader[2]);
        while(!feof(ProcedureStream))
        {
            fscanf(ProcedureStream,"%d\t%s\t%f",&TempProcedure.Code,TempProcedure.Name,&TempProcedure.Cost);
            if(TempProcedure.Code == code)
            {
                fclose(ProcedureStream);
                return 1;
            }
        }
    }
    fclose(ProcedureStream);
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
    scanf("%s",NewPatient->Phone);
    gotoxy(20+14,16);
    scanf("%s",NewPatient->Allergies);
    TempPatient.Id = 0;
    NewPatient->Id = 1;
    strcpy(NewPatient->NextAppDate,"N/A");
    strcpy(NewPatient->LastTreatment,"N/A");
    NewPatient->CardBalance = 4000.00;
    PatientStream = fopen("./DataFiles/Patients.txt","r");
    if(!PatientStream)
    {
        return 0;
    }
    else
    {
        char PatientHeader[9][40];
        fscanf(PatientStream,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",PatientHeader[0],PatientHeader[1],PatientHeader[2],
        PatientHeader[3],PatientHeader[4],PatientHeader[5],PatientHeader[6],PatientHeader[7],PatientHeader[8]);
        while(!feof(PatientStream))
        {
            fscanf(PatientStream,"%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%f",&TempPatient.Id,TempPatient.Fname,TempPatient.Lname,
            TempPatient.Address,TempPatient.Phone,TempPatient.Allergies,TempPatient.LastTreatment,
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
        return 0;
    }
    else
    {
        if(fprintf(PatientStream,"%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%.2f\n",NewPatient.Id,NewPatient.Fname,NewPatient.Lname,
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
    printf("Patient ID    :");
    gotoxy(20,10);
    printf("Doctor ID     :");
    gotoxy(20,12);
    printf("Procedure Code:");
    gotoxy(20+16,8);
    scanf("%d",&NewVisit->PatientID);
    gotoxy(20+16,10);
    scanf("%d",&NewVisit->DoctorID);
    gotoxy(20+16,12);
    scanf("%d",&NewVisit->ProcedureCode);
    if(IfPatientExist(NewVisit->PatientID)==0)
    {
        return -3;
    }
    else if(IfDoctorExist(NewVisit->DoctorID)==0)
    {
        return -2;
    }
    else if(IfProcedureExist(NewVisit->ProcedureCode)==0)
    {
        return -1;
    }
    return 1;
}
void ShowFailPaymentOptionsMenu(void)
{
    gotoxy(23,15);
    printf("[1]<--New Visit");
    gotoxy(44,15);
    printf("[2]Patient Menu");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
}
int FailPaymentOptionsMenuController(char option)
{
    switch(option)
    {
        case '1':
            Visit NewV;
            Visit *NewVisit = &NewV;
            NewPatientVisit(NewVisit);
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
void ShowPaymentOptionsMenu(void)
{
    DefaultService();
    gotoxy(30,18);
    printf("[1]Save");
    gotoxy(40,18);
    printf("[2]Cancel");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
}
int PaymentOptionsMenuController(char option, Visit *NewVisit, Patient *ExistingPatient)
{
    switch(option)
    {
        case '1':
            AddPatientVisitToFile(*NewVisit,*ExistingPatient);
            ShowAfterAddPatientVisitMenu();
            do{}while(AfterAddPatientVisitMenuController(OptionDriver(30,20,NUMERIC))==0);
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
int ProcessVisitTransaction(Visit *NewVisit,Patient *ExistingPatient)
{
    FILE *ProcedureStream;
    FILE *PatientStream;
    Procedure TempProcedure;
    float VisitCost=0;
    float CardCoverage=0;
    float CashCoverage=0;
    ProcedureStream = fopen("./DataFiles/Procedure.txt","r");
    PatientStream = fopen("./DataFiles/Patients.txt","r");
    if(!ProcedureStream || !PatientStream)
    {
        return 0;
    }
    else
    {
        char PatientHeader[9][40];
        fscanf(PatientStream,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",PatientHeader[0],PatientHeader[1],PatientHeader[2],
        PatientHeader[3],PatientHeader[4],PatientHeader[5],PatientHeader[6],PatientHeader[7],PatientHeader[8]);
        while(!feof(PatientStream))
        {
            fscanf(PatientStream,"%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%f",&ExistingPatient->Id,ExistingPatient->Fname,ExistingPatient->Lname,
            ExistingPatient->Address,ExistingPatient->Phone,ExistingPatient->Allergies,ExistingPatient->LastTreatment,
            ExistingPatient->NextAppDate,&ExistingPatient->CardBalance);
            if(ExistingPatient->Id == NewVisit->PatientID)
            {
                fclose(PatientStream);
                break;
            }
        }
        char ProcedureHeader[3][20];
        fscanf(ProcedureStream,"%s\t%s\t%s",ProcedureHeader[0],ProcedureHeader[1],ProcedureHeader[2]);
        while(!feof(ProcedureStream))
        {
            fscanf(ProcedureStream,"%d\t%s\t%f",&TempProcedure.Code,TempProcedure.Name,&TempProcedure.Cost);
            if(TempProcedure.Code == NewVisit->ProcedureCode)
            {
                ShowPaymentOptionsMenu();
                gotoxy(27,4);
                printf("Procedure Code: %d",TempProcedure.Code);
                gotoxy(27,6);
                printf("Procedure Name: %s",TempProcedure.Name);
                gotoxy(37,9);
                printf("Total Cost: $%.2f",TempProcedure.Cost);
                VisitCost = TempProcedure.Cost;
                CardCoverage = (ExistingPatient->CardBalance) * 0.85;
                if(VisitCost>=CardCoverage)
                {
                    CashCoverage = VisitCost - CardCoverage;
                    ExistingPatient->CardBalance = ExistingPatient->CardBalance - CardCoverage;
                }
                else
                {
                    CardCoverage = VisitCost;
                    ExistingPatient->CardBalance = ExistingPatient->CardBalance - CardCoverage;
                }
                NewVisit->VisitPayment.card = CardCoverage;
                NewVisit->VisitPayment.cash = CashCoverage;

                gotoxy(27,12);
                printf("Card Coverage: $%.2f",CardCoverage);
                gotoxy(27,14);
                printf("Cash Needed: $%.2f",CashCoverage);
                fclose(ProcedureStream);
                do{}while(PaymentOptionsMenuController(OptionDriver(30,21,NUMERIC),NewVisit,ExistingPatient));
                break;
            }
        }
        fclose(ProcedureStream);
    }
    return 1;
}
int AddPatientVisitToFile(Visit NewVisit,Patient ExistingPatient)
{
    FILE * VisitStream;
    FILE * PatientStream;
    VisitStream = fopen("./DataFiles/PatientVisit.txt","a");
    PatientStream = fopen("./DataFiles/Patients.txt","a");
    DefaultService();
    if(!VisitStream || !PatientStream)
    {
        if(!VisitStream)
        {
            gotoxy(15,7);
            printf("Error: Could not save data to \"PatientVisit\" file");
        }
        if(!PatientStream)
        {
            gotoxy(15,9);
            printf("Error: Could not save data to \"Patients\" file");
        }
        return 0;
    }
    else
    {

        fprintf(VisitStream,"%d\t%d\t%d\t%.2f\t%.2f\n",NewVisit.PatientID,NewVisit.DoctorID,
        NewVisit.ProcedureCode,NewVisit.VisitPayment.card,NewVisit.VisitPayment.cash);
        UpdateRecordInPatientFile(ExistingPatient);
        gotoxy(27,7);
        printf("Transaction Successfull!");
    }
    fclose(VisitStream);
    return 1;
}
int UpdateRecordInPatientFile(Patient UpdatedPatient)
{
    FILE * PatientStream;
    Patient TempPatient;
    int Position=0;
    PatientStream = fopen("./DataFiles/Patients.txt","r+");
    if(!PatientStream)
    {
        return 0;
    }
    else
    {
        char PatientHeader[9][40];
        fscanf(PatientStream,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",PatientHeader[0],PatientHeader[1],PatientHeader[2],
        PatientHeader[3],PatientHeader[4],PatientHeader[5],PatientHeader[6],PatientHeader[7],PatientHeader[8]);
        while(!feof(PatientStream))
        {
            Position = ftell(PatientStream);
            fscanf(PatientStream,"%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%f",&TempPatient.Id,TempPatient.Fname,TempPatient.Lname,
            TempPatient.Address,TempPatient.Phone,TempPatient.Allergies,TempPatient.LastTreatment,
            TempPatient.NextAppDate,&TempPatient.CardBalance);
            if(TempPatient.Id == UpdatedPatient.Id)
            {
                fseek(PatientStream,Position+1,SEEK_SET);
                fprintf(PatientStream,"%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%.2f\n",UpdatedPatient.Id,UpdatedPatient.Fname,UpdatedPatient.Lname,
                UpdatedPatient.Address,UpdatedPatient.Phone,UpdatedPatient.Allergies,UpdatedPatient.LastTreatment,
                UpdatedPatient.NextAppDate,UpdatedPatient.CardBalance);
                break;
            }
        }
    }
    fclose(PatientStream);
    return 1;
}
void ShowAfterAddPatientVisitMenu(void)
{
    gotoxy(23,15);
    printf("[1]<--New Visit");
    gotoxy(44,15);
    printf("[2]Patient Menu");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
}
int AfterAddPatientVisitMenuController(char option)
{
    switch(option)
    {
        case '1':
            Visit NewV;
            Visit *NewVisit = &NewV;
            NewPatientVisit(NewVisit);
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
int UpdatePatient(int Id)
{
    FILE * PatientStream;
    do
    {
        Patient TempPatient;
        DefaultService();
        int found = 0;
        PatientStream = fopen("./DataFiles/Patients.txt","r");
        if(!PatientStream)
        {
            fclose(PatientStream);
            return 0;
        }
        else
        {
            char PatientHeader[9][40];
            fscanf(PatientStream,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",PatientHeader[0],PatientHeader[1],PatientHeader[2],
            PatientHeader[3],PatientHeader[4],PatientHeader[5],PatientHeader[6],PatientHeader[7],PatientHeader[8]);
            while(!feof(PatientStream))
            {
                fscanf(PatientStream,"%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%f",&TempPatient.Id,TempPatient.Fname,TempPatient.Lname,
                TempPatient.Address,TempPatient.Phone,TempPatient.Allergies,TempPatient.LastTreatment,
                TempPatient.NextAppDate,&TempPatient.CardBalance);
                if(TempPatient.Id==Id)
                {
                    gotoxy(17,5);
                    printf("Patient Id    : %d",TempPatient.Id);
                    gotoxy(17,6);
                    printf("First Name    : %s",TempPatient.Fname);
                    gotoxy(17,7);
                    printf("Last Name     : %s",TempPatient.Lname);
                    gotoxy(17,8);
                    printf("Address       : %s",TempPatient.Address);
                    gotoxy(17,9);
                    printf("Phone Number  : %s",TempPatient.Phone);
                    gotoxy(17,10);
                    printf("Allergies     : %s",TempPatient.Allergies);
                    gotoxy(17,11);
                    printf("Last Treatment: %s",TempPatient.LastTreatment);
                    gotoxy(17,12);
                    printf("Next AppDate  : %s",TempPatient.NextAppDate);
                    gotoxy(17,13);
                    printf("Card Balance  : %.2f",TempPatient.CardBalance);
                    found = 1;
                    fclose(PatientStream);
                    ShowUpdatePatientMenu();
                    do{}while(UpdatePatientMenuController(OptionDriver(40,16,NUMERIC),TempPatient)==0);
                    break;
                }
            }
            if(found==0)
            {
                fclose(PatientStream);
                gotoxy(25,9);
                printf("Patient Record not found!");
                return -1;
            }
        }
    }while(1);
    return 1;
}
void ShowUpdatePatientMenu(void)
{
    gotoxy(30,3);
    printf("UPDATE PATIENT MENU");
    gotoxy(10,15);
    printf("[1]First Name");
    gotoxy(10,16);
    printf("[2]Last Name");
    gotoxy(10,17);
    printf("[3]Address");
    gotoxy(10,18);
    printf("[4]Phone Number");
    gotoxy(10,19);
    printf("[5]Allergies");
    gotoxy(10,20);
    printf("[6]Last Treatment");
    gotoxy(10,21);
    printf("[7]Next AppDate");
    gotoxy(10,22);
    printf("[8]Card Balance");
    gotoxy(30,22);
    printf("[<-]Search Patient");
    gotoxy(53,22);
    printf("[9]Patient Menu");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
}
int UpdatePatientMenuController(char option,Patient TempPatient)
{
    DefaultService();
    gotoxy(30,5);
    printf("Update Patient Field");
    fflush(stdin);
    switch(option)
    {
        case '1':
            gotoxy(20,11);
            printf("First Name:");
            gotoxy(20+12,11);
            scanf("%s",TempPatient.Fname);
            UpdateRecordInPatientFile(TempPatient);
        break;
        case '2':
            gotoxy(20,11);
            printf("Last Name:");
            gotoxy(20+11,11);
            scanf("%s",TempPatient.Lname);
            UpdateRecordInPatientFile(TempPatient);
        break;
        case '3':
            gotoxy(20,11);
            printf("Address:");
            gotoxy(20+9,11);
            scanf("%s",TempPatient.Address);
            UpdateRecordInPatientFile(TempPatient);
        break;
        case '4':
            gotoxy(20,11);
            printf("Phone Number:");
            gotoxy(20+13,11);
            scanf("%s",TempPatient.Phone);
            UpdateRecordInPatientFile(TempPatient);
        break;
        case '5':
            gotoxy(20,11);
            printf("Allergies:");
            gotoxy(20+11,11);
            scanf("%s",TempPatient.Allergies);
            UpdateRecordInPatientFile(TempPatient);
        break;
        case '6':
            gotoxy(20,11);
            printf("Last Treatment:");
            gotoxy(20+16,11);
            scanf("%s",TempPatient.LastTreatment);
            UpdateRecordInPatientFile(TempPatient);
        break;
        case '7':
            gotoxy(20,11);
            printf("Next AppDate:");
            gotoxy(20+14,11);
            scanf("%s",TempPatient.NextAppDate);
            UpdateRecordInPatientFile(TempPatient);
        break;
        case '8':
            gotoxy(20,11);
            printf("Card Balance:");
            gotoxy(20+14,11);
            scanf("%f",&TempPatient.CardBalance);
            UpdateRecordInPatientFile(TempPatient);
        break;
        case '9':
            ShowPatientsMenu();
            do{}while(PatientsMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
        case (char)VK_LEFT:
            UpdatePatient(PatientSearch());
            ShowSearchPatientMenu();
            do{}while(ViewPatientRecordMenuController(OptionDriver(30,21,NUMERIC))==0);
        break;
        case (char)VK_ESCAPE:
            ShowMainMenu();
            do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
        default:
            return 0;
    }
    return 1;
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
        char PatientHeader[9][40];
        fscanf(PatientStream,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",PatientHeader[0],PatientHeader[1],PatientHeader[2],
        PatientHeader[3],PatientHeader[4],PatientHeader[5],PatientHeader[6],PatientHeader[7],PatientHeader[8]);
        while(!feof(PatientStream))
        {
            fscanf(PatientStream,"%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%f",&TempPatient.Id,TempPatient.Fname,TempPatient.Lname,
            TempPatient.Address,TempPatient.Phone,TempPatient.Allergies,TempPatient.LastTreatment,
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
                printf("Phone Number  : %s",TempPatient.Phone);
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
    gotoxy(29,4);
    printf("SEARCH PATIENT MENU");
    gotoxy(20,19);
    printf("[1]Patient Menu");
    gotoxy(40,19);
    printf("[2]<--Search Patient");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
}
void ShowSearchPatientMenu(void)
{
    gotoxy(29,4);
    printf("SEARCH PATIENT MENU");
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
    gotoxy(28,5);
    printf("DOCTOR'S REPORT MENU");
    gotoxy(31,8);
    printf("Select Doctor");
    gotoxy(15,12);
    printf("[1]Dental Surgeon");
    gotoxy(36,12);
    printf("[2]Orthodontist");
	gotoxy(55,12);
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
    float fee;
    DefaultService();
    gotoxy(30,2);
    printf("Update Procedure Fees");
    fflush(stdin);
    switch(option)
    {
        case '1':
            FindAndShowProcedure(1001,25,6);
            gotoxy(20,14);
            printf("Dental Examination:");
            gotoxy(20+20,14);
            scanf("%f",&fee);
            ShowVerifyUpdateProcedureFee();
            do{}while(VerifyUpdateProcedureFeeMenuController(OptionDriver(30,21,NUMERIC),1001,fee)==0);
        break;
        case '2':
            FindAndShowProcedure(1002,25,6);
            gotoxy(20,14);
            printf("Cleaning:");
            gotoxy(20+10,14);
            scanf("%f",&fee);
            ShowVerifyUpdateProcedureFee();
            do{}while(VerifyUpdateProcedureFeeMenuController(OptionDriver(30,21,NUMERIC),1002,fee)==0);
        break;
        case '3':
            FindAndShowProcedure(1003,25,6);
            gotoxy(20,14);
            printf("Extraction:");
            gotoxy(20+12,14);
            scanf("%f",&fee);
            ShowVerifyUpdateProcedureFee();
            do{}while(VerifyUpdateProcedureFeeMenuController(OptionDriver(30,21,NUMERIC),1003,fee)==0);
        break;
        case '4':
            FindAndShowProcedure(1004,25,6);
            gotoxy(20,14);
            printf("Filling:");
            gotoxy(20+9,14);
            scanf("%f",&fee);
            ShowVerifyUpdateProcedureFee();
            do{}while(VerifyUpdateProcedureFeeMenuController(OptionDriver(30,21,NUMERIC),1004,fee)==0);
        break;
        case '5':
            FindAndShowProcedure(1005,25,6);
            gotoxy(20,14);
            printf("X-Ray:");
            gotoxy(20+7,14);
            scanf("%f",&fee);
            ShowVerifyUpdateProcedureFee();
            do{}while(VerifyUpdateProcedureFeeMenuController(OptionDriver(30,21,NUMERIC),1005,fee)==0);
        break;
        case '6':
            FindAndShowProcedure(1006,25,6);
            gotoxy(20,14);
            printf("Braces:");
            gotoxy(20+8,14);
            scanf("%f",&fee);
            ShowVerifyUpdateProcedureFee();
            do{}while(VerifyUpdateProcedureFeeMenuController(OptionDriver(30,21,NUMERIC),1006,fee)==0);
        break;
        case (char)VK_ESCAPE:
            ShowMainMenu();
            do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
        default:
            return 0;
    }
    return 0;
}
int UpdateProcedureFee(int code, float fee)
{
    FILE * ProcedureStream;
    Procedure TempProcedure;
    int Position=0;
    ProcedureStream = fopen("./DataFiles/Procedure.txt","r+");
    if(!ProcedureStream)
    {
        return 0;
    }
    else
    {
        char ProcedureHeader[3][20];
        fscanf(ProcedureStream,"%s\t%s\t%s",ProcedureHeader[0],ProcedureHeader[1],ProcedureHeader[2]);
        while(!feof(ProcedureStream))
        {
            Position = ftell(ProcedureStream);
            fscanf(ProcedureStream,"%d\t%s\t%f",&TempProcedure.Code,TempProcedure.Name,&TempProcedure.Cost);
            if(TempProcedure.Code == code)
            {
                fseek(ProcedureStream,Position+1,SEEK_SET);
                fprintf(ProcedureStream,"%d\t%s\t%.2f\n",TempProcedure.Code,TempProcedure.Name,fee);
                fclose(ProcedureStream);
                return 1;
            }
        }
    }
    fclose(ProcedureStream);
    return 1;
}
void ShowVerifyUpdateProcedureFee(void)
{
    gotoxy(30,19);
    printf("[1]Save");
    gotoxy(42,19);
    printf("[2]Cancel");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
}
int VerifyUpdateProcedureFeeMenuController(char option,int code,float fee)
{
    switch(option)
    {
        case '1':
            DefaultService();
            if(UpdateProcedureFee(code,fee)==1)
            {
                gotoxy(30,5);
                printf("Fee Updated Successfully!");
                FindAndShowProcedure(code,25,9);
                ShowAfterUpdateFeeMenu();
                do{}while(AfterUpdateFeeMenuController(OptionDriver(30,21,NUMERIC))==0);
            }
            else
            {
                gotoxy(27,8);
                printf("Error: Could not access file");
                ShowAfterUpdateFeeMenu();
                do{}while(AfterUpdateFeeMenuController(OptionDriver(30,21,NUMERIC))==0);
            }
        break;
        case '2':
            ShowUpdateFeesMenu();
            do{}while(UpdateFeesMenuController(OptionDriver(30,21,NUMERIC))==0);
        break;
        case (char)VK_ESCAPE:
            ShowMainMenu();
            do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
    }
    return 0;
}
void ShowAfterUpdateFeeMenu(void)
{
    gotoxy(17,18);
    printf("[1]Update Fees");
    gotoxy(37,18);
    printf("[Esc]Return To Main Menu");
}
int AfterUpdateFeeMenuController(char option)
{
    switch(option)
    {
        case '1':
            ShowUpdateFeesMenu();
            do{}while(UpdateFeesMenuController(OptionDriver(30,21,NUMERIC))==0);
        break;
        case (char)VK_ESCAPE:
            ShowMainMenu();
            do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
    }
    return 0;
}
int FindAndShowProcedure(int code,int x, int y)
{
    FILE * ProcedureStream;
    Procedure TempProcedure;
    ProcedureStream = fopen("./DataFiles/Procedure.txt","r");
    if(!ProcedureStream)
    {
        return 0;
    }
    else
    {
        char ProcedureHeader[3][20];
        fscanf(ProcedureStream,"%s\t%s\t%s",ProcedureHeader[0],ProcedureHeader[1],ProcedureHeader[2]);
        while(!feof(ProcedureStream))
        {
            fscanf(ProcedureStream,"%d\t%s\t%f",&TempProcedure.Code,TempProcedure.Name,&TempProcedure.Cost);
            if(TempProcedure.Code == code)
            {
                gotoxy(x,y);
                printf("Code   : %d",TempProcedure.Code);
                gotoxy(x,y+2);
                printf("Name   : %s",TempProcedure.Name);
                gotoxy(x,y+4);
                printf("Fee    : $%.2f",TempProcedure.Cost);
                fclose(ProcedureStream);
                return 1;
            }
        }
    }
    fclose(ProcedureStream);
    return 1;
}
void GenIncomeReport (void)
{
    DefaultService();
    gotoxy(27,5);
    printf("GENERAL INCOME REPORT");


}
void ShowInReportMenu(void)
{
    gotoxy(30,23);
    printf("[<-]Reports Menu");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
}
int InReportMenuController(char option)
{
    switch(option)
    {
        break;
        case (char)VK_LEFT:
            ShowReportsMenu();
            do{}while(ReportsMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
        case (char)VK_ESCAPE:
            ShowMainMenu();
            do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
    }
    return 0;
}

void DocIncomeReport (void)
{
    DefaultService();
    gotoxy(27,5);
    printf("DOCTORS INCOME REPORT");
    FILE *docFPtr = fopen("./DataFiles/Doctors.txt","r");
    Doctor doc;
    if(docFPtr)
    {
        char DoctorHeader[5][40];
        fscanf(docFPtr,"%s\t%s\t%s\t%s\t%s",DoctorHeader[0],DoctorHeader[1],DoctorHeader[2],DoctorHeader[3],DoctorHeader[4]);

        while(!feof(docFPtr))
        {
            int totalNumOfPatients = 0;
            char patientFName[20];
            char patientLName[20];
            char patientTreatment[8];
            float feeCharged = 0;
            float amountPerPatient = 0;

            fscanf(docFPtr,"%i %s %s %s %s",&doc.Id, doc.Fname, doc.Lname, doc.Phone, doc.Specialty);
            FILE * visitFPtr = fopen("./DataFiles/PatientVisit.txt","r");
            if(visitFPtr)
            {
                char VisitHeader[5][30] = {{"Patient_Id"},{"Doctor_Id"},{"Procedure_Code"},{"Card_Payment($)"},{"Cash_Payment($)"}};
                Visit NewVisit;
                fscanf(visitFPtr,"%s\t%s\t%s\t%s\t%s",VisitHeader[0],VisitHeader[1],VisitHeader[2],VisitHeader[3],VisitHeader[4]);
                while(!feof(visitFPtr))
                {
                    fscanf(visitFPtr,"%d\t%d\t%d\t%f\t%f",&NewVisit.PatientID,&NewVisit.DoctorID,
                    &NewVisit.ProcedureCode,&NewVisit.VisitPayment.card,&NewVisit.VisitPayment.cash);
                    if(doc.Id == NewVisit.DoctorID)
                    {
                        FILE *patFPtr = fopen("./DataFiles/Patients.txt","r");
                        if(patFPtr)
                        {
                            char PatientHeader[9][40];
                            fscanf(patFPtr,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",PatientHeader[0],PatientHeader[1],PatientHeader[2],
                            PatientHeader[3],PatientHeader[4],PatientHeader[5],PatientHeader[6],PatientHeader[7],PatientHeader[8]);
                            Patient TempPatient;
                            while(!feof(patFPtr))
                            {
                                fscanf(patFPtr,"%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%f",&TempPatient.Id,TempPatient.Fname,TempPatient.Lname,
                                TempPatient.Address,TempPatient.Phone,TempPatient.Allergies,TempPatient.LastTreatment,
                                TempPatient.NextAppDate,&TempPatient.CardBalance);

                                if(NewVisit.PatientID == TempPatient.Id)
                                {
                                    strcpy(patientFName, TempPatient.Fname);
                                    strcpy(patientLName, TempPatient.Lname);
                                    totalNumOfPatients++;
                                }
                            }
                            //close(patFPtr);
                        }
                    }
                    amountPerPatient = NewVisit.VisitPayment.card + NewVisit.VisitPayment.cash;
                }
                //close(visitFPtr);
            }else{
                // cannot read cisit data
            }
            //print records
        }
        //close(docFPtr);
    }else{
        // cannot read doctor data
    }
}

void DocReport(char option)
{
    DefaultService();
    if(option=='1')
    {
        gotoxy(30,5);
        printf("Dental Surgeon Report");
    }
    else if(option=='2')
    {
        gotoxy(28,5);
        printf("Orthodontist Report");
    }
    else
    {
        gotoxy(30,5);
        printf("Dentist Report");
    }
}

int DocReportSelectMenuController(char option)
{
    switch(option)
    {
        case '1':
            DocReport(option);
            ShowInReportMenu();
            do{}while(InReportMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case '2':
            DocReport(option);
            ShowInReportMenu();
            do{}while(InReportMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case '3':
            DocReport(option);
            ShowInReportMenu();
            do{}while(InReportMenuController(OptionDriver(30,17,NUMERIC))==0);
        break;
        case (char)VK_LEFT:
            ShowReportsMenu();
            do{}while(ReportsMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
        case (char)VK_ESCAPE:
            ShowMainMenu();
            do{}while(MainMenuController(OptionDriver(30,18,NUMERIC))==0);
        break;
    }
    return 0;
}


void PatientNotiReport(void)
{
    DefaultService();
    gotoxy(27,5);
    printf("PATIENT NOTIFICATION REPORT");

    gotoxy(30,23);
    printf("[<-]Reports Menu");
    gotoxy(1,23);
    printf("[Esc]Return To Main Menu");
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
void ScreenFrame(void)//Draws screen borders
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
void DefaultService(void)//Sets up the screen for each new menu
{
    system("cls");
    ScreenFrame();
}
