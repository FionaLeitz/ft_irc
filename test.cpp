#include <iostream>
#include <fstream>
#include <string>
#include <errno.h>
#include <cstring>

int main() {
    std::ifstream file("./.IRCd-config"); // Ouvrir le fichier en lecture
    if (!file) {
        std::cout << "Erreur lors de l'ouverture du fichier : " << strerror(errno) << std::endl;
        return 1;
    }

    std::string admin;
    std::string password;

    std::string line;
    while (std::getline(file, line)) {
        std::size_t found = line.find("operator_name =");
        if (found != std::string::npos && line.size() > sizeof("operator_name ="))
            admin = line.substr(found + sizeof("operator_name ="));

        found = line.find("operator_password");
       if (found != std::string::npos && line.size() > sizeof("operator_password ="))
            password = line.substr(found + sizeof("operator_password ="));
    }

    std::cout << "Admin : " << admin << std::endl;
    std::cout << "Password : " << password << std::endl;

    file.close();
    return 0;
}