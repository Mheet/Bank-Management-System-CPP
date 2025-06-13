# 💳 Bank Management System (C++)

A full-featured, console-based Bank Management System built in C++ using Object-Oriented Programming and fundamental Data Structures.

This project simulates real-world banking operations like account creation, deposits/withdrawals, loan services, and query management — all with persistent file storage and a structured, menu-driven interface.

---

## 🧰 Features

🔐 **Account Management**  
- Create, search, and delete bank accounts  
- Deposit and withdraw funds  
- File I/O for saving/loading data across sessions

💸 **Loan Services**  
- Apply for loans (with interest, tenure, EMI calculations)  
- Loans managed in a queue (FIFO)  
- Admin can approve and process loans

📄 **Card & Passbook Requests**  
- Request Debit Card, Credit Card, Passbook, or Checkbook  
- Duplicate prevention through flags and authentication

📝 **Customer Query Management**  
- Users can raise queries with priority levels (1 = highest, 5 = lowest)  
- Queries stored in a priority-sorted linked list  
- Admins can view, sort, and search queries efficiently

🧾 **Authentication System**  
- Secure login for customers and admins  
- Input validation and account verification before actions

📂 **Persistent Data Handling**  
- All data is saved and retrieved using file operations  
- No data is lost between sessions

---

## 🔧 Tech Stack & Concepts Used

- **Language:** C++  
- **Concepts:** OOP (Classes, Encapsulation), File I/O  
- **Data Structures:** Linked Lists, Queues  
- **Modules:** Account, Loan, Query, Card/Passbook System  
- **Interface:** Menu-driven Console UI

---


## 🚀 Getting Started

### 1. Clone the Repository
```bash
git clone https://github.com/Mheet/Bank-Management-System-CPP.git
cd Bank-Management-System-CPP
 ``` 
### 2. Compile the Project (Using g++)
```bash
g++ main.cpp -o BankSystem
 ``` 
### 3. Run the Executable
```bash
./BankSystem      # For Linux/Mac
BankSystem.exe    # For Windows (just double click or run from CMD)
 ``` 
---

## 🪪 License

This project is licensed under the **MIT License**.  
Feel free to use, modify, and share!

---

## 🙋‍♂️ Author

**Mheet Singh**  
Connect with me on [LinkedIn](https://www.linkedin.com/in/mheet-singh/)  
View more projects at: [github.com/Mheet](https://github.com/Mheet)
