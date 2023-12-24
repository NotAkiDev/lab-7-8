#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Book.h"
#include "User.h"

using namespace std;


int CheckAuth(const string &login, const string &password) {
    ifstream file("/Users/aki/Desktop/lab-7-8/users.txt");
    string line;
    while (getline(file, line)) {
        if (line.find(login) == line.find(password) && line.find(login) != string::npos) {

            file.close();
            return line[line.size() - 1] - '0'; // Строковый литерал переводится в int
        }
    }

    return -1;
}

void startConsole() {
    ifstream file("/Users/aki/Desktop/lab-7-8/logo.txt");
    string line;
    while (getline(file, line)) {
        cout << "\033[32m" << line << endl;
    }

    file.close();
    cout << endl << "\033[0m" << "Введите логин и пароль" << endl;
}

vector<Book> loadBooks() {
    vector<Book> books = {};
    ifstream file("/Users/aki/Desktop/lab-7-8/books.txt");
    string line;
    while (getline(file, line)) {
        size_t pos = 0, start;
        string title, author, takenBy;
        long isbn;

        // Получение названия
        pos = line.find(';');
        title = line.substr(0, pos);

        // Author
        start = pos + 1;
        pos = line.find(';', start);
        author = line.substr(start, pos - start);

        // ISBN
        start = pos + 1;
        pos = line.find(';', start);
        isbn = stol(line.substr(start, pos - start));
        books.push_back(Book(title, author, isbn));
    }
    file.close();
    return books;
}

vector<User> loadUsers() {
    vector<User> users = {};
    ifstream file("/Users/aki/Desktop/lab-7-8/users.txt");
    string line;
    while (getline(file, line)) {
        size_t pos = 0, start;
        string name, login, password;
        int status;

        // Имя
        pos = line.find(';');
        name = line.substr(0, pos);

        // Login
        start = pos + 1;
        pos = line.find(';', start);
        login = line.substr(start, pos - start);

        // Password
        start = pos + 1;
        pos = line.find(';', start);
        password = line.substr(start, pos - start);

        // Status
        start = pos + 1;
        pos = line.find(';', start);
        status = stoi(line.substr(start, pos - start));
        users.push_back(User(name, login, password, status));
    }
    file.close();
    return users;
}

int main() {

    startConsole();
    string login, pass;
    cin >> login >> pass;
    int status = CheckAuth(login, pass);

    vector<Book> books = loadBooks();
    vector<User> users = loadUsers();

    if (status == -1) {
        cerr << "Неверный логин или пароль. Попробуйте войти ещё раз с помощью команды login " << endl;

    } else {
        cout << "Вы вошли в аккаунт" << endl;
    }

    string input;
    cin >> input;
    while (true) {

        if (input == "create_member") {
            if (status == 1) {
                cout << "Принято. Введите имя, логин и пароль" << endl;
                string name, new_login, new_pass;
                cin >> name >> new_login >> new_pass;
                bool isExist = false;
                ifstream file("/Users/aki/Desktop/lab-7-8_v2/users.txt");
                string line;
                while (getline(file, line)) {
                    size_t pos = line.find(';');
                    if (line.substr(0, pos) == login) {
                        cerr << "Пользователь с таким логином уже существует" << endl;
                        isExist = true;
                        break;
                    }
                }
                file.close();
                if (!isExist) {
                    ofstream file("/Users/aki/Desktop/lab-7-8/users.txt", ios::app);
                    file << name + ';' + new_login + ';' + new_pass + ";0" << endl;
                    file.close();
                    cout << "Пользователь успешно создан" << endl;
                };

            } else if (status == 0)
                cerr << "Недостаточно прав. Убедитесь, что вы используете верный аккаунт" << endl;
            else if (status == -2 || status == -1)
                cerr << "Вы не вошли в аккаунт. Используйте команду login" << endl;
        }
        else if (input == "borrow_book") {
            if (status >= 0) {
                cout << "Введите название книги" << endl;
                string title;
                cin.ignore();
                getline(cin, title);
                bool isExist = false;
                for (int i = 0; i < books.size(); ++i) {
                    if (books[i].getTitle() == title) {
                        isExist = true;
                        if (books[i].getBorrowedBy() == "") {
                            books[i].setBorrowBy(login);
                            cout << "Книга успешно взята в пользование" << endl;
                        } else {
                            cerr << "Книга уже взята в пользование пользователем " << books[i].getBorrowedBy() << endl;
                        }
                        break;
                    }
                }
                if (!isExist)
                    cerr << "Книга не найдена" << endl;
            } else
                cerr << "Вы не вошли в аккаунт. Используйте команду login" << endl;
        }
        else if (input == "return_book") {
            if (status >= 0) {
                cout << "Введите название книги" << endl;
                string title;
                cin.ignore();
                getline(cin, title);
                bool isExist = false;
                for (int i = 0; i < books.size(); ++i) {
                    if (books[i].getTitle() == title) {
                        isExist = true;
                        if (books[i].getBorrowedBy() == login) {
                            books[i].setBorrowBy("");
                            cout << "Книга успешно возвращена" << endl;
                        } else {
                            cerr << "Книга не взята в пользование или находится у другого человека" << endl;
                        }
                        break;
                    }
                }
                if (!isExist)
                    cerr << "Книга не найдена" << endl;

            } else
                cerr << "Вы не вошли в аккаунт. Используйте команду login" << endl;
        }
        else if (input == "my_books") {
            if (status >= 0) {
                int book_counter = 0;
                cout << "Взятые вами книги:" << endl;
                for (int i = 0; i < books.size(); ++i) {
                    if (books[i].getBorrowedBy() == login) {
                        ++book_counter;
                        cout << books[i].getTitle() << " | " << books[i].getAuthor() << " | " << books[i].getISBN()
                             << endl;
                    }
                }
                cout << "Кол-во книг: " << book_counter << endl;
            } else
                cerr << "Вы не вошли в аккаунт. Используйте команду login" << endl;
        }
        else if (input == "all_books") {
            cout << "Все книги:" << endl;
            for (int i = 0; i < books.size(); ++i) {
                cout << books[i].getTitle() << " | " << books[i].getAuthor() << " | " << books[i].getISBN();
                if (books[i].getBorrowedBy() != "")
                    cout << " | Книга взята пользователем: " << books[i].getBorrowedBy() << endl;
                else
                    cout << endl;
            }
        }
        else if (input == "available_books") {
            if (status >= 0) {
                for (int i = 0; i < books.size(); ++i) {
                    if (books[i].getBorrowedBy() == "")
                        cout << books[i].getTitle() << " | " << books[i].getAuthor() << " | " << books[i].getISBN()
                             << endl;
                }
            } else {
                cerr << "Вы не вошли в аккаунт. Используйте команду login" << endl;
            }


        }
        else if (input == "logout") {
            login = "";
            pass = "";
            status = -1;
            cout << "Вы вышли из аккаунта" << endl;

        }
        else if (input == "login") {
            if (status == -2 || status == -1) {
                cout << "Введите логин и пароль" << endl;
                cin >> login >> pass;
                status = CheckAuth(login, pass);

                if (status == -1) {
                    cerr << "Неверный логин или пароль" << endl;
                } else {
                    cout << "Вы вошли в аккаунт" << endl;
                }
            } else {
                cerr << "Вы уже вошли в аккаунт" << endl;
            }

        }
        else if (input == "exit") {
            cout << "До свидания!" << endl;
            break;

        }
        else if (input == "whoami") {
            int status = CheckAuth(login, pass);
            cout << "Статус аккаунта:" << endl;
            if (status == 0) {
                cout << "Член библиотеки" << endl;
            } else if (status == 1) {
                cout << "Библиотекарь" << endl;
            } else if (status == -1) {
                cout << "Вы не вошли в аккаунт" << endl;
            }
        }
        else if (input == "users_list") {
            // Вывод списка пользователей из файла
            for (int i = 0; i < users.size(); ++i) {
                int status = users[i].getStatus();
                if (status == 0) {
                    cout << users[i].getLogin() << " | Статус: Член библиотеки" << endl;
                } else if (status == 1) {
                    cout << users[i].getLogin() << " | Статус: Библиотекарь" << endl;
                }
            }
        }
        else if (input == "add_book") {
            if (status == 1) {
                string book_name;
                string author;
                long isbn;
                cin.ignore();
                cout << "Введите название книги:" << endl;
                getline(cin, book_name);
                cout << "Введите автора:" << endl;
                getline(cin, author);
                cout << "Введите ISBN" << endl;
                cin >> isbn;
                ofstream file("/Users/aki/Desktop/lab-7-8/books.txt", ios::app);
                file << book_name + ';' + author + ';' + to_string(isbn) << endl;
                file.close();
                books.push_back(Book(book_name, author, isbn));
                cout << "Книга добавлена в список" << endl;
            } else { cout << "Нет доступа к команде" << endl; }
        }
        else if (input == "delete_book") {
            if (status == 1) {
                string book_name;
                cin.ignore();
                cout << "Введите название книги:" << endl;
                getline(cin, book_name);
                bool isExist = false;
                for (int i = 0; i < books.size(); ++i) {
                    if (books[i].getTitle() == book_name) {
                        isExist = true;
                        books.erase(books.begin() + i);
                        cout << "Книга удалена" << endl;
                        break;
                    }
                }
                if (!isExist)
                    cerr << "Книга не найдена" << endl;
            } else { cout << "Нет доступа к команде" << endl; }
        }
        else if (input == "find_by_name") {
            if (status >= 0) {
                string title;
                bool founded = false;
                cout << "Введите название книги" << endl;
                cin.ignore();
                getline(cin, title);
                for (int i = 0; i < books.size(); ++i) {
                    if (books[i].getTitle() == title) {
                        cout << books[i].getAuthor() << " | " << books[i].getISBN() << endl;
                        founded = true;
                    }
                }
                if (!founded)
                    cerr << "Книга не найдена" << endl;
            } else {
                cerr << "Вы не вошли в аккаунт. Используйте команду login" << endl;
            }
        }
        else if (input == "find_by_isbn") {
            if (status >= 0) {
                long isbn;
                bool founded = false;
                cout << "Введите код ISBN" << endl;
                cin >> isbn;
                for (int i = 0; i < books.size(); ++i) {
                    if (books[i].getISBN() == isbn) {
                        cout << books[i].getTitle() << " | " << books[i].getAuthor() << endl;
                        founded = true;
                        break;
                    }

                }
                if (!founded)
                    cerr << "Книга не найдена" << endl;
            } else {
                cerr << "Вы не вошли в аккаунт. Используйте команду login" << endl;
            }
        }
        else {
            cerr << "Неизвестная команда" << endl;
        }

        cin >> input;
    }

    return 0;
}

