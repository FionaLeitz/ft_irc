#include <iostream>
#include <sstream>

// Définition des fonctions associées à chaque action
void handleUser(const std::string& arguments) {
    std::cout << "Traitement de l'action USER avec les arguments : " << arguments << std::endl;
}

void handleJoin(const std::string& arguments) {
    std::cout << "Traitement de l'action JOIN avec les arguments : " << arguments << std::endl;
}

void handleMode(const std::string& arguments) {
    std::cout << "Traitement de l'action MODE avec les arguments : " << arguments << std::endl;
}

int main(int argc, char **argv) {
    // Tableau de pointeurs sur fonctions
	(void)argc;
    void (*functionPointers[])(const std::string &) = { handleUser, handleJoin, handleMode };

    // Exemple de chaîne d'entrée
    std::string input(argv[1]);

    // Parsing de la chaîne d'entrée
    std::istringstream iss(input);
    std::string actionStr;
    std::string arguments;
    iss >> actionStr >> arguments;

    // Conversion de l'action en valeur d'énumération
    enum Action { USER, JOIN, MODE };
    Action action;
    if (actionStr == "USER")
        action = USER;
    else if (actionStr == "JOIN")
        action = JOIN;
    else if (actionStr == "MODE")
        action = MODE;
    else {
        std::cout << "Action non reconnue." << std::endl;
        return 0;
    }

    // Recherche de la fonction correspondante et appel si trouvée
    if (action >= 0 && action < sizeof(functionPointers) / sizeof(functionPointers[0])) {
        functionPointers[action](arguments);
    } else {
        std::cout << "Action non reconnue." << std::endl;
    }

    return 0;
}