#include <SFML/Graphics.hpp>
#include <optional>
#include "tictactoe_gamelogic.cpp"

void showEndScreen(const std::string& message);

int main() {
    // Tworzenie okna
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Tic-Tac-Toe");
    
    sf::RectangleShape lineHorizontal(sf::Vector2f(600.0f, 5.0f));
    sf::RectangleShape lineVertical(sf::Vector2f(5.0f, 600.0f)); 
    float offsetX = 200.f;
    float offsetY = 200.f;

    char board[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} }; //moja tabela
    char player = 'X';
    char computer = 'O';
    //game state flags
    bool gameOver = false; //flaga konca gry
    bool playerTurn = true; //flaga sprawdzajaca, czy ruch gracza, zeby AI czekalo
    if(player == 'O'){ playerTurn = false; }
    std::string endMessage;
    int moveCount = 0;

    sf::Font font;
    if (!font.openFromFile("assets/fonts/Arial Unicode-kopia.ttf")) {
        std::cerr << "Error: cant load the font" << std::endl;
    }
    sf::Text text(font); //potrzeba fontu do stworzenia objektu klasy Text
    text.setCharacterSize(100);
    text.setFillColor(sf::Color::White);

    while(window.isOpen()){ 
        while(const std::optional<sf::Event> event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
            if(const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){ //getIf zwraca referencje do eventu, jesli event == podany event
                if(mouseButtonPressed->button == sf::Mouse::Button::Left){
                    sf::Vector2i clickPosition = mouseButtonPressed->position;

                    //obliczenie indeksu tablicy w miejscu klikniecia, plansza 3x3, 600:3=200
                    //convert pixel position to board coordinates (3x3 grid)
                    int col = clickPosition.x/200;
                    int row = clickPosition.y/200;

                    if(playerTurn && !gameOver){
                        getPlayerMove(board, player, row, col, &playerTurn);
                        moveCount++;
                        if(checkWin(board, player)){
                            endMessage = "Congrats, you win";
                            gameOver = true;  
                        }else if(checkTie(board)) {
                            endMessage = "It's a tie!";
                            gameOver = true;
                        }
                    }   
                }
            }

            if(!playerTurn && !gameOver){
                getComputerMove(board, computer, player);
                moveCount++;
                if(checkWin(board, computer)){
                    endMessage = "You lose, loser ;pp.";
                    gameOver = true;
                }else if(checkTie(board)) {
                    endMessage = "It's a tie!";
                    gameOver = true;
                }
                playerTurn = true;
            }
        }

        window.clear(sf::Color::Black);
        lineHorizontal.setFillColor(sf::Color::White);
        lineVertical.setFillColor(sf::Color::White);

        //drawing the game grid, plansza
        lineHorizontal.setPosition({0.f, offsetY});
        window.draw(lineHorizontal);
        lineHorizontal.setPosition({0.f, offsetY*2});
        window.draw(lineHorizontal);
        lineVertical.setPosition({offsetX, 0.f});
        window.draw(lineVertical);
        lineVertical.setPosition({offsetX*2, 0.f});
        window.draw(lineVertical);

        //rysowanie x i o w sorkdu planszy
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(board[i][j] == 'X' || board[i][j] == 'O'){
                    float x = j*offsetX+offsetX/2 - text.getGlobalBounds().size.x/2 ;
                    float y = i*offsetY+offsetY/2 - text.getGlobalBounds().size.y ;
                    text.setString(board[i][j]);
                    text.setPosition({x,y});
                    window.draw(text);
                }
            }
        }
        window.display();

        if(gameOver){
            showEndScreen(endMessage);
            break;
        }

    }
    return 0;
}

void showEndScreen(const std::string& message){
    sf::RenderWindow endWindow(sf::VideoMode({500, 100}), "Game outcome");
    sf::Font font;
    if (!font.openFromFile("/Users/macbook/projekty/c++/tictactoe/assets/fonts/Arial Unicode-kopia.ttf")) { // lub "assets/fonts/arial.ttf"
        std::cerr << "Error: cant load the font" << std::endl;
    }
    sf::Text text(font); //potrzeba fontu do stworzenia obiektu klasy Text
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);
    text.setString(message);

    while (endWindow.isOpen()) {
        while (const std::optional<sf::Event> event = endWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                endWindow.close();
            }
        }
        endWindow.clear(sf::Color::Black);
        endWindow.draw(text);
        endWindow.display();
    }
}