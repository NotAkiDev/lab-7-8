#include <string>
using namespace std;

#ifndef LAB_7_8_BOOK_H
#define LAB_7_8_BOOK_H


class Book {
private:
    string title;
    string author;
    string borrowedBy;
    long isbn;

public:
    Book(const string &title, const string &author, const long isbn) : title(title), author(author), isbn(isbn) {}

    ~Book() {}

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


#endif //LAB_7_8_BOOK_H
