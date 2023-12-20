#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Book {
private:
    string title;
    string author;
    string borrowedBy = "";
    long isbn;

    friend void setBorrowBy(const string &login);

public:
    Book(const string &title, const string &author, const long isbn) :
            title(title), author(author), isbn(isbn) {}

    // Методы для доступа к данным
    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    long getISBN() const {
        return isbn;
    }

    string getBorrowedBy() const {
        return borrowedBy;
    }

    void setBorrowBy(const string &login) {
        borrowedBy = login;
    }

};

class User {
protected:
    string name;
    string login;
    string password;
public:
    User(const string &name, const string &login, const string &password) : name(name), login(login),
                                                                             password(password) {}

    string getLogin() const {
        return login;
    }

    string getPass() const {
        return password;
    }

    string getName() const {
        return name;
    }

    void borrowBook() {}
    void returnBook() {}

};

class Member : public User {
public:
    Member(const string &name, const string &login, const string &password) : User(name, login, password) {};
};
class Librarian : public User {
public:
    Librarian(const string &name, const string &login, const string &password) : User(name, login, password) {}

};


int CheckAuth(const string &login, const string &password, const vector<Librarian> &librarian,
              const vector<Member> &members) {
    for (int i = 0; i < librarian.size(); ++i) {
        if (librarian[i].getLogin() == login && librarian[i].getPass() == password) {
            return 1;
        }
    }
    for (int i = 0; i < members.size(); ++i) {
        if (members[i].getLogin() == login && members[i].getPass() == password) {
            return 0;
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

int main() {
    startConsole();

    vector<Librarian> librarians = {Librarian("Тим", "admin", "admin")};

    vector<Member> members = {Member("Беннедикт Киберскотч", "mew", "mew")};

    vector<Book> books{Book("Автостопом по Галактике", "Дуглас Адамс", 9785170987481),
                       Book("Ресторан в конце Вселенной", "Дуглас Адамс", 9781529099133),
                       Book("Доктор Кто и Криккитмены", "Дуглас Адамс", 9781785941061)};


    string login, pass;
    cin >> login;
    cin >> pass;
    int status = CheckAuth(login, pass, librarians, members);

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
                cin >> name;
                cin >> new_login;
                cin >> new_pass;
                // Проверка логина на существование
                bool isExist = false;
                for (int i = 0; i < members.size(); ++i) {
                    if (members[i].getLogin() == new_login) {
                        cerr << "Пользователь с таким логином уже существует" << endl;
                        isExist = true;
                        break;
                    }
                }
                if (!isExist) {
                    members.push_back(Member(name, new_login, new_pass));
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
            for (int i = 0; i < books.size(); ++i) {
                if (books[i].getBorrowedBy() == "")
                    cout << books[i].getTitle() << " | " << books[i].getAuthor() << " | " << books[i].getISBN() << endl;
            }

        }
        else if (input == "logout") {
            status = -2;
            cout << "Вы вышли из аккаунта" << endl;

        }
        else if (input == "login") {
            if (status == -2 || status == -1) {
                cout << "Введите логин и пароль" << endl;
                cin >> login;
                cin >> pass;
                status = CheckAuth(login, pass, librarians, members);

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
            exit(0);

        }
        else if (input == "whoami") {
            int status = CheckAuth(login, pass, librarians, members);
            cout << "Статус аккаунта:" << endl;
            if (status == 0) {
                cout << "Член библиотеки" << endl;
            } else if (status == 1) {
                cout << "Библиотекарь" << endl;
            } else if (status == -2 || status == -1) {
                cout << "Вы не вошли в аккаунт" << endl;
            }

        }
        else if (input == "user_list") {
            for (int i = 0; i < librarians.size(); ++i) {
                cout << librarians[i].getName() << " | Статус: ";
                cout << "Библиотекарь" << endl;
            }

            for (int i = 0; i < librarians.size(); ++i) {
                cout << members[i].getName() << " | Статус: ";
                cout << "Член библиотеки" << endl;
            }


        }
        else if (input == "add_book"){
            if (status == 1){
                string book_name; string author; long isbn;
                cin.ignore();
                cout << "Введите название книги:" << endl;
                getline(cin, book_name);
                cout << "Введите автора:" << endl;
                getline(cin, author);
                cout << "Введите ISBN" << endl;
                cin >> isbn;
                books.push_back(Book(book_name, author, isbn));
                cout << "Книга добавлена в список" << endl;
            }
            else {cout << "Нет доступа к команде" << endl;}
        }
        else if (input == "delete_book"){
            if (status == 1){
                string book_name;
                cin.ignore();
                cout << "Введите название книги:" << endl;
                getline(cin, book_name);
                bool isExist = false;
                for (int i = 0; i < books.size(); ++i){
                    if (books[i].getTitle() == book_name){
                        isExist = true;
                        books.erase(books.begin() + i);
                        cout << "Книга удалена" << endl;
                        break;
                    }
                }
                if (!isExist)
                    cerr << "Книга не найдена" << endl;
            }
            else {cout << "Нет доступа к команде" << endl;}
        }
        else if (input == "find_by_name"){
            string title;
            bool founded = false;
            cout << "Введите название книги" << endl;
            cin.ignore();
            getline(cin, title);
            for (int i = 0; i < books.size(); ++i){
                if (books[i].getTitle() == title){
                    cout << books[i].getAuthor() << " | " << books[i].getISBN() << endl;
                    founded = true;
                }
            }
            if (!founded)
                cerr << "Книга не найдена" << endl;
        }
        else if (input == "find_by_isbn"){
            long isbn;
            bool founded = false;
            cout << "Введите код ISBN" << endl;
            cin >> isbn;
            for (int i = 0; i < books.size(); ++i){
                if (books[i].getISBN() == isbn)
                    cout << books[i].getTitle() << " | " << books[i].getAuthor() << endl;
                    founded = true;
                break;
            }
            if (!founded)
                cerr << "Книга не найдена" << endl;

        }
        else {
            cerr << "Неизвестная команда" << endl;
        }

        cin >> input;

    }

    return 0;
}

