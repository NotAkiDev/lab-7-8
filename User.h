#include <string>

using namespace std;

#ifndef LAB_7_8_USER_H
#define LAB_7_8_USER_H


class User {
protected:
    string name;
    string login;
    string password;
    int status;
public:
    User(const string &name, const string &login, const string &password, int status) :
            name(name), login(login), password(password), status(status) {}

    string getLogin() const { return login; }

    string getPass() const { return password; }

    string getName() const { return name; }

    int getStatus() const { return status; }
};


#endif //LAB_7_8_USER_H
