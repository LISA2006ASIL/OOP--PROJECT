#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<memory>

using namespace std;

// ==========================================================
// INPUT VALIDATION
// ==========================================================

int getValidInteger() {

    int x;

    while (!(cin >> x)) {

        cin.clear();
        cin.ignore(1000, '\n');

        cout << "Invalid input. Enter integer: ";
    }

    return x;
}

// ==========================================================
// USER BASE CLASS
// ==========================================================

class User {

protected:

    string username;
    string password;

public:

    User(string un, string pw) {

        username = un;
        password = pw;
    }

    virtual ~User() {}

    virtual void ShowRoleMenu() = 0;

    string getUsername() {

        return username;
    }

    string getPassword() {

        return password;
    }
};

// ==========================================================
// BOOK CLASS
// ==========================================================

class Book {

private:

    int id;
    string title;
    string author;
    bool available;

public:

    Book() {

        id = 0;
        title = "";
        author = "";
        available = true;
    }

    Book(int ID, string Title, string Author) {

        id = ID;
        title = Title;
        author = Author;
        available = true;
    }

    int getId() {

        return id;
    }

    string getTitle() {

        return title;
    }

    string getAuthor() {

        return author;
    }

    bool isAvailable() {

        return available;
    }

    void setAvailability(bool status) {

        available = status;
    }

    void displayBook() {

        cout << "[ID: "
             << id
             << "] "
             << title
             << " by "
             << author;

        if (available)
            cout << " (Available)\n";
        else
            cout << " (Issued)\n";
    }
};

// ==========================================================
// LIBRARIAN CLASS
// ==========================================================

class Librarian : public User {

public:

    Librarian(string un, string pw)
        : User(un, pw) {}

    void ShowRoleMenu() override {

        cout << "\n========== LIBRARIAN MENU ==========\n";

        cout << "1.Add Book\n";
        cout << "2.Issue Book\n";
        cout << "3.Return Book\n";
        cout << "4.Search Book\n";
        cout << "5.View Books\n";
        cout << "6.Mark Overdue Book\n";
        cout << "7.View Transactions\n";
        cout << "8.Logout\n";
    }
};

// ==========================================================
// STUDENT CLASS
// ==========================================================

class Student : public User {

private:

    string rollnumber;

public:

    Student(
        string un,
        string pw,
        string roll
    )
        : User(un, pw) {

        rollnumber = roll;
    }

    string getRollNumber() {

        return rollnumber;
    }

    void ShowRoleMenu() override {

        cout << "\n========== STUDENT MENU ==========\n";

        cout << "1.Search Book\n";
        cout << "2.View Books\n";
        cout << "3.Logout\n";
    }
};

// ==========================================================
// LIBRARY SYSTEM
// ==========================================================

class LibrarySystem {

private:

    vector<Book> inventory;

    vector<shared_ptr<Student>>
        registeredStudents;

    shared_ptr<User> LoggedInUser;

public:

    LibrarySystem() {

        LoggedInUser = nullptr;

        loadBooksFromFile();
        loadStudentsFromFile();
    }

    ~LibrarySystem() {

        saveBooksToFile();
        saveStudentsToFile();
    }

    // ======================================================
    // SAVE BOOKS
    // ======================================================

    void saveBooksToFile() {

        ofstream outFile(
            "books_database.txt"
        );

        if (!outFile)
            return;

        for (size_t i = 0;
             i < inventory.size();
             i++) {

            outFile
                << inventory[i].getId()
                << ","
                << inventory[i].getTitle()
                << ","
                << inventory[i].getAuthor()
                << ","
                << inventory[i].isAvailable()
                << "\n";
        }

        outFile.close();
    }

    // ======================================================
    // LOAD BOOKS
    // ======================================================

    void loadBooksFromFile() {

        ifstream inFile(
            "books_database.txt"
        );

        if (!inFile)
            return;

        string line;

        while (getline(inFile, line)) {

            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);
            size_t pos3 = line.find(',', pos2 + 1);

            if (pos1 != string::npos &&
                pos2 != string::npos &&
                pos3 != string::npos) {

                int id =
                    stoi(line.substr(0, pos1));

                string title =
                    line.substr(
                        pos1 + 1,
                        pos2 - pos1 - 1
                    );

                string author =
                    line.substr(
                        pos2 + 1,
                        pos3 - pos2 - 1
                    );

                bool available =
                    stoi(line.substr(pos3 + 1));

                Book b(id, title, author);

                b.setAvailability(available);

                inventory.push_back(b);
            }
        }

        inFile.close();
    }

    // ======================================================
    // SAVE STUDENTS
    // ======================================================

    void saveStudentsToFile() {

        ofstream outFile(
            "students_database.txt"
        );

        if (!outFile)
            return;

        for (size_t i = 0;
             i < registeredStudents.size();
             i++) {

            outFile
                << registeredStudents[i]
                       ->getUsername()
                << ","

                << registeredStudents[i]
                       ->getPassword()
                << ","

                << registeredStudents[i]
                       ->getRollNumber()
                << "\n";
        }

        outFile.close();
    }

    // ======================================================
    // LOAD STUDENTS
    // ======================================================

    void loadStudentsFromFile() {

        ifstream inFile(
            "students_database.txt"
        );

        if (!inFile)
            return;

        string line;

        while (getline(inFile, line)) {

            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);

            if (pos1 != string::npos &&
                pos2 != string::npos) {

                string username =
                    line.substr(0, pos1);

                string password =
                    line.substr(
                        pos1 + 1,
                        pos2 - pos1 - 1
                    );

                string roll =
                    line.substr(pos2 + 1);

                registeredStudents.push_back(
                    make_shared<Student>(
                        username,
                        password,
                        roll
                    )
                );
            }
        }

        inFile.close();
    }

    // ======================================================
    // STUDENT SIGN UP
    // ======================================================

    void studentSignup() {

        string username;
        string password;
        string roll;

        cin.ignore();

        cout << "\n========== SIGN UP ==========\n";

        cout << "Enter Username: ";
        getline(cin, username);

        for (size_t i = 0;
             i < registeredStudents.size();
             i++) {

            if (registeredStudents[i]
                    ->getUsername() == username) {

                cout << "Username already exists.\n";
                return;
            }
        }

        cout << "Enter Password: ";
        getline(cin, password);

        cout << "Enter Roll Number: ";
        getline(cin, roll);

        registeredStudents.push_back(

            make_shared<Student>(
                username,
                password,
                roll
            )
        );

        cout << "Account created successfully.\n";
    }

    // ======================================================
    // LOGIN
    // ======================================================

    bool handleLogin(
        string user,
        string pass,
        string type
    ) {

        if (type == "admin") {

            if (user == "admin" &&
                pass == "admin123") {

                LoggedInUser =
                    make_shared<Librarian>(
                        "admin",
                        "admin123"
                    );

                return true;
            }
        }

        else if (type == "student") {

            for (size_t i = 0;
                 i < registeredStudents.size();
                 i++) {

                if (registeredStudents[i]
                        ->getUsername() == user &&

                    registeredStudents[i]
                        ->getPassword() == pass) {

                    LoggedInUser =
                        registeredStudents[i];

                    return true;
                }
            }
        }

        cout << "Invalid credentials.\n";
        return false;
    }

    // ======================================================
    // LOGOUT
    // ======================================================

    void handleLogout() {

        LoggedInUser = nullptr;

        cout << "Logged out successfully.\n";
    }

    shared_ptr<User> getUser() {

        return LoggedInUser;
    }

    // ======================================================
    // ADD BOOK
    // ======================================================

    void addBook() {

        int id;
        string title;
        string author;

        cout << "Enter Book ID: ";

        id = getValidInteger();

        for (size_t i = 0;
             i < inventory.size();
             i++) {

            if (inventory[i].getId() == id) {

                cout << "Book ID already exists.\n";
                return;
            }
        }

        cin.ignore();

        cout << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Author: ";
        getline(cin, author);

        inventory.push_back(
            Book(id, title, author)
        );

        cout << "Book added successfully.\n";
    }

    // ======================================================
    // ISSUE BOOK
    // ======================================================

    void issueBook() {

        string roll;
        string targetTitle;

        bool studentFound = false;

        cout << "\n========== ISSUE BOOK ==========\n";

        cin.ignore();

        cout << "Enter Student Roll Number: ";
        getline(cin, roll);

        for (size_t i = 0;
             i < registeredStudents.size();
             i++) {

            if (registeredStudents[i]
                    ->getRollNumber() == roll) {

                studentFound = true;
                break;
            }
        }

        if (!studentFound) {

            cout << "Student not found.\n";
            return;
        }

        cout << "Enter Book Title: ";
        getline(cin, targetTitle);

        for (size_t i = 0;
             i < inventory.size();
             i++) {

            if (inventory[i].getTitle()
                == targetTitle) {

                if (!inventory[i]
                        .isAvailable()) {

                    cout << "Book already issued.\n";
                    return;
                }

                inventory[i]
                    .setAvailability(false);

                ofstream outFile(
                    "transactions.txt",
                    ios::app
                );

                if (outFile) {

                    outFile
                        << "Student ID: "
                        << roll
                        << " | Book: "
                        << inventory[i]
                               .getTitle()

                        << " | Author: "
                        << inventory[i]
                               .getAuthor()

                        << " | Status: Issued"
                        << "\n";

                    outFile.close();
                }

                cout << "Book issued successfully.\n";

                return;
            }
        }

        cout << "Book not found.\n";
    }

    // ======================================================
    // RETURN BOOK
    // ======================================================

    void returnBook() {

        string targetTitle;

        cin.ignore();

        cout << "Enter Book Title: ";

        getline(cin, targetTitle);

        for (size_t i = 0;
             i < inventory.size();
             i++) {

            if (inventory[i].getTitle()
                == targetTitle) {

                inventory[i]
                    .setAvailability(true);

                ofstream outFile(
                    "transactions.txt",
                    ios::app
                );

                if (outFile) {

                    outFile
                        << "Book: "
                        << inventory[i].getTitle()
                        << " | Author: "
                        << inventory[i].getAuthor()
                        << " | Status: Returned"
                        << "\n";

                    outFile.close();
                }

                cout << "Book returned.\n";

                return;
            }
        }

        cout << "Book not found.\n";
    }

    // ======================================================
    // MARK OVERDUE BOOK
    // ======================================================

    void markOverdueBook() {

        string targetTitle;

        cin.ignore();

        cout << "Enter Overdue Book Title: ";

        getline(cin, targetTitle);

        for (size_t i = 0;
             i < inventory.size();
             i++) {

            if (inventory[i].getTitle()
                == targetTitle) {

                ofstream outFile(
                    "transactions.txt",
                    ios::app
                );

                if (outFile) {

                    outFile
                        << "Book: "
                        << inventory[i].getTitle()
                        << " | Author: "
                        << inventory[i].getAuthor()
                        << " | Status: Overdue"
                        << "\n";

                    outFile.close();
                }

                cout << "Book marked overdue.\n";

                return;
            }
        }

        cout << "Book not found.\n";
    }

    // ======================================================
    // SEARCH BOOK
    // ======================================================

    void searchBook() {

        string targetTitle;

        cin.ignore();

        cout << "Enter Book Title: ";

        getline(cin, targetTitle);

        for (size_t i = 0;
             i < inventory.size();
             i++) {

            if (inventory[i].getTitle()
                == targetTitle) {

                inventory[i].displayBook();

                return;
            }
        }

        cout << "Book not found.\n";
    }

    // ======================================================
    // VIEW BOOKS
    // ======================================================

    void viewBooks() {

        cout << "\n========== BOOK CATALOG ==========\n";

        if (inventory.empty()) {

            cout << "No books available.\n";
            return;
        }

        for (size_t i = 0;
             i < inventory.size();
             i++) {

            inventory[i].displayBook();
        }
    }

    // ======================================================
    // VIEW TRANSACTIONS
    // ======================================================

    void viewTransactions() {

        ifstream inFile(
            "transactions.txt"
        );

        if (!inFile) {

            cout << "No transaction history found.\n";
            return;
        }

        string line;

        cout << "\n========== TRANSACTIONS ==========\n";

        while (getline(inFile, line)) {

            cout << line << "\n";
        }

        inFile.close();
    }
};

// ==========================================================
// MAIN FUNCTION
// ==========================================================

int main() {

    LibrarySystem systemEngine;

    int choice;

    cout << "====================================\n";
    cout << "     LIBRARY MANAGEMENT SYSTEM\n";
    cout << "====================================\n";

    while (true) {

        if (systemEngine.getUser() == nullptr) {

            cout << "\n1.Admin Login\n";
            cout << "2.Student Login\n";
            cout << "3.Student Sign Up\n";
            cout << "4.Exit\n";

            cout << "Selection: ";

            choice = getValidInteger();

            if (choice == 1) {

                string user;
                string pass;

                cout << "Username: ";
                cin >> user;

                cout << "Password: ";
                cin >> pass;

                systemEngine.handleLogin(
                    user,
                    pass,
                    "admin"
                );
            }

            else if (choice == 2) {

                string user;
                string pass;

                cout << "Username: ";
                cin >> user;

                cout << "Password: ";
                cin >> pass;

                systemEngine.handleLogin(
                    user,
                    pass,
                    "student"
                );
            }

            else if (choice == 3) {

                systemEngine.studentSignup();
            }

            else {

                break;
            }
        }

        else {

            systemEngine
                .getUser()
                ->ShowRoleMenu();

            cout << "Enter Choice: ";

            choice = getValidInteger();

            if (systemEngine
                    .getUser()
                    ->getUsername() == "admin") {

                switch (choice) {

                case 1:
                    systemEngine.addBook();
                    break;

                case 2:
                    systemEngine.issueBook();
                    break;

                case 3:
                    systemEngine.returnBook();
                    break;

                case 4:
                    systemEngine.searchBook();
                    break;

                case 5:
                    systemEngine.viewBooks();
                    break;

                case 6:
                    systemEngine.markOverdueBook();
                    break;

                case 7:
                    systemEngine.viewTransactions();
                    break;

                case 8:
                    systemEngine.handleLogout();
                    break;

                default:
                    cout << "Invalid option.\n";
                }
            }

            else {

                switch (choice) {

                case 1:
                    systemEngine.searchBook();
                    break;

                case 2:
                    systemEngine.viewBooks();
                    break;

                case 3:
                    systemEngine.handleLogout();
                    break;

                default:
                    cout << "Invalid option.\n";
                }
            }
        }
    }

    cout << "Program terminated.\n";

    return 0;
}
