#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <limits>
using namespace std;

// ---------- Book Structure ----------
struct Book
{
    int id;
    string title;
    string author;
    bool issued;
};

const string FILENAME = "books.txt";

// ---------- Function Declarations ----------
void addBook();
void viewAllBooks();
void searchBook();
void issueBook();
void returnBook();
void deleteBook();
vector<Book> loadBooks();
void saveBooks(const vector<Book> &books);
int getValidInt(const string &prompt);
void showMenu();

// ---------- Main ----------
int main()
{
    int choice;

    do
    {
        showMenu();
        choice = getValidInt("Enter your choice: ");

        switch (choice)
        {
            case 1: addBook(); break;
            case 2: viewAllBooks(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: deleteBook(); break;
            case 7: cout << "Exiting... Goodbye!" << endl; break;
            default: cout << "Invalid choice! Try again.\n"; break;
        }
        cout << endl;

    } while (choice != 7);

    return 0;
}

// ---------- Menu ----------
void showMenu()
{
    cout << "=========================================" << endl;
    cout << "       LIBRARY MANAGEMENT SYSTEM          " << endl;
    cout << "=========================================" << endl;
    cout << "1. Add Book" << endl;
    cout << "2. View All Books" << endl;
    cout << "3. Search Book by ID" << endl;
    cout << "4. Issue Book" << endl;
    cout << "5. Return Book" << endl;
    cout << "6. Delete Book" << endl;
    cout << "7. Exit" << endl;
    cout << "-----------------------------------------" << endl;
}

// ---------- File Handling Helpers ----------
vector<Book> loadBooks()
{
    vector<Book> books;
    ifstream file(FILENAME);
    string line;

    while (getline(file, line))
    {
        if (line.empty()) continue;
        stringstream ss(line);
        Book b;
        string idStr, issuedStr;

        getline(ss, idStr, '|');
        getline(ss, b.title, '|');
        getline(ss, b.author, '|');
        getline(ss, issuedStr, '|');

        b.id = stoi(idStr);
        b.issued = (issuedStr == "1");

        books.push_back(b);
    }
    file.close();
    return books;
}

void saveBooks(const vector<Book> &books)
{
    ofstream file(FILENAME, ios::trunc);
    for (const auto &b : books)
    {
        file << b.id << "|" << b.title << "|" << b.author << "|" << (b.issued ? 1 : 0) << "\n";
    }
    file.close();
}

// ---------- Core Features ----------
void addBook()
{
    Book b;
    vector<Book> books = loadBooks();

    b.id = getValidInt("Enter Book ID: ");

    for (const auto &existing : books)
    {
        if (existing.id == b.id)
        {
            cout << "A book with this ID already exists!" << endl;
            return;
        }
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Title: ";
    getline(cin, b.title);

    cout << "Enter Author: ";
    getline(cin, b.author);

    b.issued = false;

    ofstream file(FILENAME, ios::app);
    file << b.id << "|" << b.title << "|" << b.author << "|0\n";
    file.close();

    cout << "Book added successfully!" << endl;
}

void viewAllBooks()
{
    vector<Book> books = loadBooks();

    if (books.empty())
    {
        cout << "No books found in the library." << endl;
        return;
    }

    cout << left << setw(6) << "ID" << setw(25) << "Title"
         << setw(20) << "Author" << setw(10) << "Status" << endl;
    cout << "-------------------------------------------------------------" << endl;

    for (const auto &b : books)
    {
        cout << left << setw(6) << b.id << setw(25) << b.title
             << setw(20) << b.author << setw(10) << (b.issued ? "Issued" : "Available") << endl;
    }
}

void searchBook()
{
    int id = getValidInt("Enter Book ID to search: ");
    vector<Book> books = loadBooks();

    for (const auto &b : books)
    {
        if (b.id == id)
        {
            cout << "\nBook Found:" << endl;
            cout << "ID     : " << b.id << endl;
            cout << "Title  : " << b.title << endl;
            cout << "Author : " << b.author << endl;
            cout << "Status : " << (b.issued ? "Issued" : "Available") << endl;
            return;
        }
    }
    cout << "Book with ID " << id << " not found." << endl;
}

void issueBook()
{
    int id = getValidInt("Enter Book ID to issue: ");
    vector<Book> books = loadBooks();
    bool found = false;

    for (auto &b : books)
    {
        if (b.id == id)
        {
            found = true;
            if (b.issued)
            {
                cout << "This book is already issued." << endl;
            }
            else
            {
                b.issued = true;
                saveBooks(books);
                cout << "Book issued successfully!" << endl;
            }
            break;
        }
    }
    if (!found) cout << "Book with ID " << id << " not found." << endl;
}

void returnBook()
{
    int id = getValidInt("Enter Book ID to return: ");
    vector<Book> books = loadBooks();
    bool found = false;

    for (auto &b : books)
    {
        if (b.id == id)
        {
            found = true;
            if (!b.issued)
            {
                cout << "This book was not issued." << endl;
            }
            else
            {
                b.issued = false;
                saveBooks(books);
                cout << "Book returned successfully!" << endl;
            }
            break;
        }
    }
    if (!found) cout << "Book with ID " << id << " not found." << endl;
}

void deleteBook()
{
    int id = getValidInt("Enter Book ID to delete: ");
    vector<Book> books = loadBooks();
    bool found = false;

    vector<Book> updated;
    for (const auto &b : books)
    {
        if (b.id == id)
            found = true;
        else
            updated.push_back(b);
    }

    if (found)
    {
        saveBooks(updated);
        cout << "Book deleted successfully!" << endl;
    }
    else
    {
        cout << "Book with ID " << id << " not found." << endl;
    }
}

// ---------- Input Validation ----------
int getValidInt(const string &prompt)
{
    int val;
    while (true)
    {
        cout << prompt;
        cin >> val;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
        }
        else
        {
            return val;
        }
    }
}
