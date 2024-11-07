#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

const string ClientsFileName = "../BankClients.txt";
struct stClient
{
    string AccountNumber;
    string PinCode;
    string UserName;
    string Phone;
    double TotalBalance;
};
stClient CurrentClient;

enum enMainMenuOptions {QuickWithdraw = 1, NormalWithdraw = 2, Deposit = 3, CheckBalance = 4, Logout = 5};

vector<stClient> ReadClientsFromFile(string FileName);
void Login();
void ShowMainMenu();
vector<string> SplitString(string line, string separator);
void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back Main Menu...";
    system("pause>0");

    ShowMainMenu();

}

string ConvertRecordToLine(stClient Client, string separator = "#//#")
{
    string line = "";
    line += Client.AccountNumber + separator;
    line += Client.PinCode + separator;
    line += Client.UserName + separator;
    line += Client.Phone + separator;
    line += to_string(Client.TotalBalance) + separator;

    return line;
}
stClient ConvertLineToRecord(string line)
{
    vector<string> vClients = SplitString(line, "#//#");
    stClient Client;

    Client.AccountNumber = vClients[0];
    Client.PinCode = vClients[1];
    Client.UserName = vClients[2];
    Client.Phone = vClients[3];
    Client.TotalBalance = stod(vClients[4]);

    return Client;
}

void SaveClientsToFile(vector<stClient>& vClients, string FileName)
{
    fstream myFile;
    myFile.open(FileName, ios::out);
    string line;
    
    if (myFile.is_open())
    {
        for (stClient Client : vClients)
        {
            line = ConvertRecordToLine(Client);
            myFile << line << endl;
        }
        myFile.close();
    }


}
bool DepositBalanceToClientByAccountNumber(string AccountNumber, short WithdrawAmount, vector<stClient>& vClients)
{
    char Answer = 'y';
    cout << "Are you sure you want to perform this transaction? (Y/N)? "; 
    cin >> Answer;

    if (tolower(Answer) == 'y')
    {
        for (stClient& Client : vClients)
        {
            if (Client.AccountNumber == AccountNumber)
            {
                Client.TotalBalance += WithdrawAmount;
                SaveClientsToFile(vClients, ClientsFileName);
                cout << "\nDone Successfully. New balance is " << CurrentClient.TotalBalance << endl;
                return true;
            }
        }
    }

    return false;
}
short GetQuickWithdrawAmount(short option)
{
    short WithdrawAmounts[9] = { 20, 50, 100, 200, 400, 600, 800, 1000 };
    return WithdrawAmounts[option - 1];
}
void PerformQuickWithdrawOption(short QuickWithdrawOption)
{
    if (QuickWithdrawOption == 9)
    {
        return;
    }

    short WithdrawAmount = GetQuickWithdrawAmount(QuickWithdrawOption);

    if (CurrentClient.TotalBalance < WithdrawAmount)
    {
        cout << "\nThe amount exceeds your balance, make another choice";
        return;
    }

    CurrentClient.TotalBalance -= WithdrawAmount;
    vector<stClient> vClients = ReadClientsFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawAmount * -1, vClients);

}
short ReadQuickWithdraw()
{
    short WithdrawOption = 0;
    do
    {
        cout << "Choose What to Withdraw from? [1] to [9]? ";
        cin >> WithdrawOption;

    } while (WithdrawOption < 1 || WithdrawOption > 9);

    return WithdrawOption;
}
void ShowQuickWithdrawScreen()
{
    cout << "===================================================== \n";
    cout << "\t\t Quick Withdraw \n";
    cout << "===================================================== " << endl;
    cout << "\t[1] 20  \t [2] 50 \n";
    cout << "\t[3] 100 \t [4] 200 \n";
    cout << "\t[5] 400 \t [6] 600 \n";
    cout << "\t[7] 800 \t [8] 1000 \n";
    cout << "\t[9] Exit \n";
    cout << "===================================================== \n";

    cout << "Your Balance Is " << CurrentClient.TotalBalance << endl;
    PerformQuickWithdrawOption(ReadQuickWithdraw());

}

void ShowCheckBalanceScreen()
{
    cout << "===================================================== \n";
    cout << "\t\t Check Balance Screen \n";
    cout << "===================================================== " << endl;

    cout << "Your Balance is " << CurrentClient.TotalBalance;
}

void PerformNormalWithdraw(int NormalWithdraw)
{
    if (CurrentClient.TotalBalance < NormalWithdraw)
    {
        cout << "\nThe amount exceeds your balance, make another choice ";
        return;
    }

    CurrentClient.TotalBalance -= NormalWithdraw;
    vector<stClient> vClients = ReadClientsFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, NormalWithdraw * -1, vClients);
}
int ReadNormalWithdraw()
{
    int WithdrawAmount = 0;
    do
    {
        cout << "\nEnter an amount multiple of 5's ? ";
        cin >> WithdrawAmount;

    } while (WithdrawAmount % 5 != 0);

    return WithdrawAmount;
}
void ShowNormalWithdrawScreen()
{
    cout << "===================================================== \n";
    cout << "\t\t Normal Withdraw \n";
    cout << "===================================================== " << endl;

    PerformNormalWithdraw(ReadNormalWithdraw());
}

double ReadDepositAmount()
{
    double DepositAmount = 0;
    do
    {
        cout << "Enter a positive Deposit amount? ";
        cin >> DepositAmount;
    } while (DepositAmount <= 0);

    return DepositAmount;
}
void ShowDepositScreen()
{
    cout << "===================================================== \n";
    cout << "\t\t Deposit Screen \n";
    cout << "===================================================== " << endl;

    double DepositAmount = ReadDepositAmount();

    CurrentClient.TotalBalance += DepositAmount;
    vector<stClient> vClients = ReadClientsFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
}

void PerformMainMenuOption(enMainMenuOptions option)
{
    switch (option)
    {
    case enMainMenuOptions::QuickWithdraw:
    {
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::NormalWithdraw:
    {
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::Deposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::CheckBalance:
    {
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToMainMenu();
        break;
    }
    case enMainMenuOptions::Logout:
    {
        system("cls");
        Login();
        break;
    }
    }
}
short ReadMainMenuOption()
{
    short option = 0;
    cout << "Choose What do you want to do? [1 to 5]? ";
    cin >> option;
    return option;
}
void ShowMainMenu()
{
    system("cls");
    cout << "===================================================== \n";
    cout << "\t\t ATM Main Menu Screen \n";
    cout << "===================================================== " << endl;
    cout << "\t [1] Quick Withdraw. \n";
    cout << "\t [2] Normal Withdraw. \n";
    cout << "\t [3] Deposit. \n";
    cout << "\t [4] Check Balance. \n";
    cout << "\t [5] Logout. \n";
    cout << "===================================================== ";
    cout << endl;

    PerformMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}

vector<string> SplitString(string line, string separator)
{
    vector<string> vString;

    short separatorPosition = 0;
    string word;
    while ((separatorPosition = line.find(separator)) != string::npos)
    {
        word = line.substr(0, separatorPosition);
        if (word != "")
        {
            vString.push_back(word);
        }
        line.erase(0, separatorPosition + separator.length());
    }
    if (line != "") vString.push_back(line);

    return vString;
}
vector<stClient> ReadClientsFromFile(string FileName)
{
    vector<stClient> vClients;
    fstream myFile;
    myFile.open(FileName, ios::in);
    if (myFile.is_open())
    {
        string line;
        while (getline(myFile, line))
        {
            stClient Client= ConvertLineToRecord(line);
            vClients.push_back(Client);
        }
        myFile.close();
    }

    return vClients;
}
bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& Client)
{
    vector<stClient> vClients = ReadClientsFromFile(ClientsFileName);
    
    for (stClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }

    return false;
}
void Login()
{
    bool InvalidClient = false;
    string AccountNumber, PinCode;
    do
    {
        system("cls");

        cout << "- - - - - - - - - - - - - - - - - - \n";
        cout << "\t Login Screen \n";
        cout << "- - - - - - - - - - - - - - - - - - \n";
        cout << endl;

        if (InvalidClient)
        {
            cout << "Invalid Account Number/PinCode! \n";
        }

        cout << "Enter Account Number? ";
        cin >> AccountNumber;
        cout << "Enter Pin? ";
        cin >> PinCode;

        InvalidClient = !FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient);

    } while (InvalidClient);

    ShowMainMenu();
}

int main()
{
    Login();

    system("pause>0");
}

