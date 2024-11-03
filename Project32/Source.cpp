#include <iostream>
#include <string>
#include <sqlite_modern_cpp.h>

using namespace std;

class Game {
public:
    Game() {
        sqlite::database db("game_users.db");                                                                                                                                                                                                                                                                         
        db << "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE, password TEXT, login TEXT UNIQUE);";
};
	
	~Game() {
		delete db;
	}

	void registerUser(const string& username, const string& password, const string& login);
	bool loginUser(const string& login, const string& password);
	void gameMenu();
	void accountSettings(int userId);

private:
	sqlite::database* db = nullptr;
};

void Game::registerUser(const string& username, const string& password, const string& login) {
	try {
		(*db) << "INSERT INTO users (username, password, login) VALUES (?, ?, ?)" << username << password << login;
		cout << "Registration is succesfully done! You did log in the system by automaticly" << endl;
	}
	catch (const sqlite::sqlite_exception& e) {
		cout << "Registration error: " << e.what() << endl;
	}
}

bool Game::loginUser(const string& login, const string& password) {
	string storedPassword;
	try {
		*db << "SELECT password FROM users WHERE login = ?" << login >> storedPassword;
	}
	catch (const sqlite::sqlite_exception& e) {
		cout << "Autorization error: " << e.what() << endl;
		return false;
	}

	if (storedPassword == password) {
		cout << "Succesfull logging in the system" << endl;
		return true;
	}
	else {
		cout << "Wrong login or password" << endl;
		return false;
	}
}

void Game::accountSettings(int userId) {
	string newUsername, newPassword;

	cout << "Enter new username: ";
	cin >> newUsername;
	cout << "Enter new password: ";
	cin >> newPassword;

    try {
        *db << "UPDATE users SET username = ?, password = ? WHERE id = ?"
            << newUsername
            << newPassword
            << userId;
        cout << "Account's data is succesfully updated" << endl;
    }
    catch (const sqlite::sqlite_exception& e) {
        cout << "Data's uppdating error: " << e.what() << endl;
    }
}

void Game::gameMenu() {
    while (true) {
        cout << "\nMain menu: " << endl;
        cout << "1. Start the game" << endl;
        cout << "2. Show statistic" << endl;
        cout << "3. Change account settings" << endl;
        cout << "4. Exit" << endl;

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "The game is started!" << endl;
            break;
        case 2:
            cout << "Statistic:it's unrealized yet." << endl;
            break;
        case 3:
            int userId; 
            accountSettings(userId);
            break;
        case 4:
            cout << "You are out of the system." << endl;
            return;
        default:
            cout << "Unrevolved input." << endl;
        }
    }
}

int main() {
    Game game;
    string username, password, login;

    while (true) {
        cout << "\nChoose action: 1. Registration 2. Autorization 3. Exit" << endl;
        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            cout << "Enter login: ";
            cin >> login;
            game.registerUser(username, password, login);
            game.loginUser(login, password); 
            game.gameMenu();

        }
        else if (choice == 2) {
            cout << "Enter login: ";
            cin >> login;
            cout << "Enter password: ";
            cin >> password;
            if (game.loginUser(login, password)) {
                game.gameMenu();
            }

        }
        else if (choice == 3) {
            break;
        }
        else {
            cout << "Unresolved logging." << endl;
        }
    }

    return 0;
}
