#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>

using namespace std;


struct Account
{
    string accountNumber;
    string name;
    string password;
    double balance;
    bool hasDebitCard;
    bool hasCreditCard;
    bool hasPassbook;
    bool hasCheckbook;
    string accountType;
    vector<string> transactionHistory;
    Account* next;


    Account()
    {
        balance = 0.0;
        hasDebitCard = false;
        hasCreditCard = false;
        hasPassbook = false;
        hasCheckbook = false;
        next = nullptr;
    }


    void addTransaction(const string& transaction)
    {
        transactionHistory.push_back(transaction);
    }
};


struct Query
{
    string accountNumber;
    string message;
    int priority;
    Query* next;

    Query(const string& accNum, const string& msg, int pri)
    {
        accountNumber = accNum;
        message = msg;
        priority = pri;
        next = nullptr;
    }
};


struct Loan
{
    string accountNumber;
    double amount;
    double interestRate;
    int tenure;
    double monthlyInstallment;
    bool isApproved;
    Loan* next;

    Loan(const string& accNum, double amt, double rate, int tn)
    {
        accountNumber = accNum;
        amount = amt;
        interestRate = rate;
        tenure = tn;
        isApproved = false;
        next = nullptr;

        monthlyInstallment = (amount + (amount * interestRate / 100)) / tenure;
    }
};


struct LoanQueue
{
    Loan* front;
    Loan* rear;

    LoanQueue()
    {
        front = nullptr;
        rear = nullptr;
    }


    void enqueue(Loan* newLoan)
    {
        if (rear == nullptr)
        {
            front = rear = newLoan;
            return;
        }
        rear->next = newLoan;
        rear = newLoan;
    }


    Loan* dequeue()
    {
        if (front == nullptr)
            return nullptr;

        Loan* temp = front;
        front = front->next;

        if (front == nullptr)
            rear = nullptr;

        return temp;
    }


    bool isEmpty()
    {
        return front == nullptr;
    }
};


class BankManagementSystem
{
private:
    Account* head;
    string dataFile;
    Query* queryHead;
    LoanQueue loanQueue;

public:
    BankManagementSystem()
    {
        head = nullptr;
        dataFile = "account.txt";
        queryHead = nullptr;
    }


    void addAccount()
    {
        Account* newAccount = new Account;

        cout << "Enter Account Number: ";
        cin >> newAccount->accountNumber;

        if (isAccountNumberUnique(newAccount->accountNumber))
        {
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, newAccount->name);

            cout << "Set Password: ";
            cin >> newAccount->password;
            cout << "Enter Initial Balance: ";

            while (!(cin >> newAccount->balance) || newAccount->balance < 0)
            {
                cout << "Invalid input. Please enter a positive initial balance: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            cout << "Select Account Type (Savings/Checking): ";
            cin >> newAccount->accountType;

            newAccount->next = nullptr;

            if (head == nullptr)
            {
                head = newAccount;
            }
            else
            {
                Account* current = head;
                while (current->next != nullptr)
                {
                    current = current->next;
                }
                current ->next = newAccount;
            }
            cout << "\nAccount Added Successfully!" << endl;
            saveAccountsToFile();
        }
        else
        {
            cout << "\nAccount Number is not unique. Please enter a unique account number." << endl;
            delete newAccount;
        }
    }


    bool isAccountNumberUnique(const string& accountNumber)
    {
        Account* current = head;

        while (current != nullptr)
        {
            if (current->accountNumber == accountNumber)
            {
                return false;
            }
            current = current->next;
        }

        return true;
    }


    Account* findAccount(const string& accountNumber, const string& password)
    {
        Account* current = head;

        while (current != nullptr)
        {
            if (current->accountNumber == accountNumber && current->password == password)
            {
                return current;
            }
            current = current->next;
        }

        return nullptr;
    }


    void searchAccount()
    {
        string accountNumber;
        string password;

        cout << "Enter Account Number: ";
        cin >> accountNumber;

        cout << "Enter Password: ";
        cin >> password;

        Account* account = findAccount(accountNumber, password);

        if (account != nullptr)
        {
            cout << "\nAccount Number: " << account->accountNumber << endl;
            cout << "Name: " << account->name << endl;
            cout << "Balance: " << fixed << setprecision(2) << account->balance << endl;
            cout << "Debit Card: " << (account->hasDebitCard ? "Yes" : "No") << endl;
            cout << "Credit Card: " << (account->hasCreditCard ? "Yes" : "No") << endl;
            cout << "Passbook: " << (account->hasPassbook ? "Yes" : "No") << endl;
            cout << "Check book: " << (account->hasCheckbook ? "Yes" : "No") << endl;
            cout << "Account Type: " << account->accountType << endl;
        }
        else
        {
            cout << "\nAccount Not Found or Invalid Password!" << endl;
        }
    }


    void depositFunds()
    {
        string accountNumber;
        string password;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;
        cout << "Enter Password: ";
        cin >> password;

        Account* account = findAccount(accountNumber, password);

        if (account != nullptr)
        {
            double amount;
            cout << "Enter amount to deposit: ";
            while (!(cin >> amount) || amount <= 0)
            {
                cout << "Invalid input. Please enter a positive amount: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            account->balance += amount;
            account->addTransaction("Deposit: " + to_string(amount));
            cout << "\nDeposit successful!" << endl;

            saveAccountsToFile();
        }
        else
        {
            cout << "\nAccount Not Found or Invalid Password!" << endl;
        }
    }


    void withdrawFunds()
    {
        string accountNumber;
        string password;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;

        cout << "Enter Password: ";
        cin >> password;

        Account* account = findAccount(accountNumber, password);

        if (account != nullptr)
        {
            double amount;

            cout << "Enter amount to withdraw: ";
            while (!(cin >> amount) || amount <= 0)
            {
                cout << "Invalid input. Please enter a positive amount: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            if (amount <= account->balance)
            {
                account->balance -= amount;
                account->addTransaction("Withdrawal: " + to_string(amount));
                cout << "\nWithdrawal successful!" << endl;
                saveAccountsToFile();
            }
            else
            {
                cout << "\nInsufficient funds!" << endl;
            }
        }
        else
        {
            cout << "\nAccount Not Found or Invalid Password!" << endl;
        }
    }


    void displayAccount()
    {
        cout << "\n--- All Accounts ---" << endl;

        Account* current = head;

        while (current != nullptr)
        {
            cout << "\nAccount Number: " << current->accountNumber << endl;
            cout << "Name: " << current->name << endl;
            cout << "Balance: " << fixed << setprecision(2) << current->balance << endl;
            cout << "Debit Card: " << (current->hasDebitCard ? "Yes" : "No") << endl;
            cout << "Credit Card: " << (current->hasCreditCard ? "Yes" : "No") << endl;
            cout << "Passbook: " << (current->hasPassbook ? "Yes" : "No") << endl;
            cout << "Check book: " << (current->hasCheckbook ? "Yes" : "No") << endl;
            cout << "Account Type: " << current->accountType << endl;

            current = current->next;
        }

        cout << endl;
    }


    void applyForDebitCard(Account* account)
    {
        if (!account->hasDebitCard)
        {
            account->hasDebitCard = true;
            cout << "\nDebit Card applied successfully for account: " << account->accountNumber << endl;
            saveAccountsToFile();
        }
        else
        {
            cout << "\nDebit Card already issued for this account!" << endl;
        }
    }

    void applyForCreditCard(Account* account)
    {
        if (!account->hasCreditCard)
        {
            account->hasCreditCard = true;
            cout << "\nCredit Card applied successfully for account: " << account->accountNumber << endl;
            saveAccountsToFile();
        }
        else
        {
            cout << "\nCredit Card already issued for this account!" << endl;
        }
    }

    void applyForPassbook(Account* account)
    {
        if (!account->hasPassbook)
        {
            account->hasPassbook = true;
            cout << "\nPassbook applied successfully for account: " << account->accountNumber << endl;
            saveAccountsToFile();
        }
        else
        {
            cout << "\nPassbook already issued for this account!" << endl;
        }
    }

    void applyForCheckbook(Account* account)
    {
        if (!account->hasCheckbook)
        {
            account->hasCheckbook = true;
            cout << "\nCheckbook applied successfully for account: " << account->accountNumber << endl;
            saveAccountsToFile();
        }
        else
        {
            cout << "\nCheckbook already issued for this account!" << endl;
        }
    }


    void applyForLoan()
    {
        string accountNumber;
        string password;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;
        cout << "Enter Password: ";
        cin >> password;

        Account* account = findAccount(accountNumber, password);

        if (account != nullptr)
        {
            double loanAmount, interestRate;
            int tenure;

            if (account->balance >= 200000)
            {
                cout << "Enter Loan Amount: ";
                cin >> loanAmount;

                cout << "Enter Interest Rate (in %): ";
                cin >> interestRate;

                cout << "Enter Tenure (in months): ";
                cin >> tenure;

                Loan* newLoan = new Loan(accountNumber, loanAmount, interestRate, tenure);
                loanQueue.enqueue(newLoan);

                cout << "\nLoan applied successfully! Your loan request is being processed." << endl;
            }
            else
            {
                cout << "\nLoan application denied. Minimum balance of 200,000 required!" << endl;
            }
        }
        else
        {
            cout << "\nAccount Not Found or Invalid Password!" << endl;
        }
    }


    void viewLoanDetails()
    {
        string accountNumber;
        string password;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;

        cout << "Enter Password: ";
        cin >> password;

        Account* account = findAccount(accountNumber, password);

        if (account != nullptr)
        {
            Loan* current = loanQueue.front;

            while (current != nullptr)
            {
                if (current->accountNumber == accountNumber)
                {
                    cout << "\nLoan Amount: " << current->amount << endl;
                    cout << "Interest Rate: " << current->interestRate << "%" << endl;
                    cout << "Tenure: " << current->tenure << " months" << endl;
                    cout << "Monthly Installment: " << current->monthlyInstallment << endl;
                    break;
                }
                current = current->next;
            }
        }
        else
        {
            cout << "\nAccount Not Found or Invalid Password!" << endl;
        }
    }


    void makeLoanPayment()
    {
        string accountNumber;
        string password;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;

        cout << "Enter Password: ";
        cin >> password;

        Account* account = findAccount(accountNumber, password);

        if (account != nullptr)
        {
            Loan* current = loanQueue.front;

            while (current != nullptr)
            {
                if (current->accountNumber == accountNumber)
                {
                    double payment;
                    cout << "Enter payment amount: ";
                    cin >> payment;

                    if (payment > 0)
                    {
                        current->amount -= payment;
                        cout << "\nPayment successful!" << endl;
                    }
                    else
                    {
                        cout << "\nInvalid payment amount!" << endl;
                    }
                    break;
                }
                current = current->next;
            }
        }
        else
        {
            cout << "\nAccount Not Found or Invalid Password!" << endl;
        }
    }


    void handleQueries()
    {
        string accountNumber, message;
        int priority;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;
        cout << "Enter Query Message: ";
        cin.ignore();

        getline(cin, message);

        cout << "Enter Priority (1-5, where 1 is highest): ";
        cin >> priority;

        if (priority > 5 || priority < 1)
        {
            cout << "\nInvalid Priority, Please Try Again Later!" << endl;
        }
        else
        {
            Query* newQuery = new Query(accountNumber, message, priority);

            if (queryHead == nullptr || queryHead->priority > priority)
            {
                newQuery->next = queryHead;
                queryHead = newQuery;
            }
            else
            {
                Query* current = queryHead;

                while (current->next != nullptr && current->next->priority <= priority)
                {
                    current = current->next;
                }

                newQuery->next = current->next;
                current->next = newQuery;
            }

            cout << "\nQuery submitted successfully!" << endl;
        }
    }



    void displayQueries()
    {
        cout << "\n--- Customer Queries (Priority Order) ---" << endl;

        Query* current = queryHead;

        if (!current)
        {
            cout << "No queries available." << endl;
            return;
        }

        while (current != nullptr)
        {
            cout << "Account Number: " << current->accountNumber
                 << ", Message: " << current->message
                 << ", Priority: " << current->priority << endl;
            current = current->next;
        }

        cout << endl;
    }



    void searchQuery()
    {
        string accountNumber;

        cout << "\nEnter Account Number: ";
        cin >> accountNumber;

        Query* queryArray[100];
        int count = 0;

        Query* current = queryHead;
        while (current != nullptr)
        {
            if (current->accountNumber == accountNumber)
            {
                queryArray[count++] = current;
            }
            current = current->next;
        }

        if (count == 0)
        {
            cout << "\nNo queries found for the given account number!" << endl;
            return;
        }

        for (int i = 0; i < count - 1; ++i)
        {
            for (int j = 0; j < count - i - 1; ++j)
            {
                if (queryArray[j]->priority > queryArray[j + 1]->priority)
                {
                    Query* temp = queryArray[j];
                    queryArray[j] = queryArray[j + 1];
                    queryArray[j + 1] = temp;
                }
            }
        }

        cout << "\n--- Queries for Account: " << accountNumber << " ---" << endl;
        for (int i = 0; i < count; ++i)
        {
            cout << "Message: " << queryArray[i]->message
                 << ", Priority: " << queryArray[i]->priority << endl;
        }
        cout << endl;
    }


    void saveAccountsToFile()
    {
        ofstream outFile(dataFile);
        Account* current = head;

        while (current != nullptr)
        {
            outFile << current->accountNumber << endl;
            outFile << current->name << endl;
            outFile << current->password << endl;
            outFile << current->balance << endl;
            outFile << current->hasDebitCard << endl;
            outFile << current->hasCreditCard << endl;
            outFile << current->hasPassbook << endl;
            outFile << current->hasCheckbook << endl;
            outFile << current->accountType << endl;

            for (const string& transaction : current->transactionHistory)
            {
                outFile << transaction << endl;
            }

            current = current->next;
        }

        outFile.close();
    }


    void loadAccountsFromFile()
    {
        ifstream inFile(dataFile);
        if (inFile)
        {
            while (!inFile.eof())
            {
                Account* newAccount = new Account;

                inFile >> newAccount->accountNumber;
                inFile.ignore();
                getline(inFile, newAccount->name);
                inFile >> newAccount->password;
                inFile >> newAccount->balance;
                inFile >> newAccount->hasDebitCard;
                inFile >> newAccount->hasCreditCard;
                inFile >> newAccount->hasPassbook;
                inFile >> newAccount->hasCheckbook;
                inFile >> newAccount->accountType;

                string transaction;
                while (getline(inFile, transaction) && !transaction.empty())
                {
                    newAccount->addTransaction(transaction);
                }

                newAccount->next = head;
                head = newAccount;
            }
            inFile.close();
        }
    }


    void processLoanQueue()
    {
        if (!loanQueue.isEmpty())
        {
            Loan* loan = loanQueue.dequeue();
            cout << "\nProcessing loan for account: " << loan->accountNumber << endl;
            delete loan;
        }
        else
        {
            cout << "\nLoan queue is empty." << endl;
        }
    }


    void loanMenu()
    {
        int choice;
        do
        {
            cout << "\n--- Loan Services ---" << endl;
            cout << "1. Apply for Loan" << endl;
            cout << "2. View Loan Details" << endl;
            cout << "3. Make Loan Payment" << endl;
            cout << "4. Process Loan Queue" << endl;
            cout << "0. Back to Main Menu" << endl;
            cout << "Choose an option: ";
            cin >> choice;

            switch (choice)
            {
                case 1: applyForLoan(); break;
                case 2: viewLoanDetails(); break;
                case 3: makeLoanPayment(); break;
                case 4: processLoanQueue(); break;
                case 0: return;
                default: cout << "Invalid choice! Please try again." << endl;
            }
        }
        while (choice != 0);
    }


    void accountMenu()
    {
        int choice;
        do
        {
            cout << "\n--- Account Services ---" << endl;
            cout << "1. Add Account" << endl;
            cout << "2. Search Account" << endl;
            cout << "3. Deposit Funds" << endl;
            cout << "4. Withdraw Funds" << endl;
            cout << "5. Display All Accounts" << endl;
            cout << "0. Back to Main Menu" << endl;
            cout << "Choose an option: ";
            cin >> choice;

            switch (choice)
            {
                case 1: addAccount(); break;
                case 2: searchAccount(); break;
                case 3: depositFunds(); break;
                case 4: withdrawFunds(); break;
                case 5: displayAccount(); break;
                case 0: return;
                default: cout << "Invalid choice! Please try again." << endl;
            }
        }
        while (choice != 0);
    }


    void cardAndPassbookMenu()
    {
        int choice;
        string accountNumber, password;

        do
        {
            cout << "\n--- Card and Passbook Services ---" << endl;
            cout << "1. Apply for Debit Card" << endl;
            cout << "2. Apply for Credit Card" << endl;
            cout << "3. Apply for Passbook" << endl;
            cout << "4. Apply for Checkbook" << endl;
            cout << "0. Back to Main Menu" << endl;
            cout << "Choose an option: ";
            cin >> choice;

            if (choice >= 1 && choice <= 4)
            {
                cout << "\nEnter Account Number: ";
                cin >> accountNumber;

                cout << "Enter Password: ";
                cin >> password;

                Account* account = findAccount(accountNumber, password);

                if (account == nullptr)
                {
                    cout << "\nAccount Not Found or Invalid Password!" << endl;
                    continue;
                }

                switch (choice)
                {
                    case 1: applyForDebitCard(account); break;
                    case 2: applyForCreditCard(account); break;
                    case 3: applyForPassbook(account); break;
                    case 4: applyForCheckbook(account); break;
                }
            }
            else if (choice != 0)
            {
                cout << "Invalid choice! Please try again." << endl;
            }
        }
        while (choice != 0);
    }


    void queryMenu()
    {
        int choice;
        do
        {
            cout << "\n--- Query Management ---" << endl;
            cout << "1. Handle Queries" << endl;
            cout << "2. Display Queries" << endl;
            cout << "3. Search Query" << endl;
            cout << "0. Back to Main Menu" << endl;
            cout << "Choose an option: ";
            cin >> choice;

            switch (choice)
            {
                case 1: handleQueries(); break;
                case 2: displayQueries(); break;
                case 3: searchQuery(); break;
                case 0: return;
                default: cout << "Invalid choice! Please try again." << endl;
            }
        }
        while (choice != 0);
    }


    void menu()
    {
        loadAccountsFromFile();
        int choice;

        do
        {
            cout << "\n--- Bank Management System ---" << endl;
            cout << "1. Account Services" << endl;
            cout << "2. Loan Services" << endl;
            cout << "3. Card and Passbook Services" << endl;
            cout << "4. Query Management" << endl;
            cout << "0. Exit" << endl;
            cout << "Choose a category: ";
            cin >> choice;

            switch (choice)
            {
                case 1: accountMenu(); break;
                case 2: loanMenu(); break;
                case 3: cardAndPassbookMenu(); break;
                case 4: queryMenu(); break;
                case 0: cout << "Exiting system. Goodbye!" << endl; break;
                default: cout << "Invalid choice! Please try again." << endl;
            }
        }
        while (choice != 0);
    }
};


int main()
{
    BankManagementSystem bms;
    bms.menu();
    return 0;
}
