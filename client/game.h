#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "../player.h"

// This class manages the window and input for the game

const std::string version = "Project: Brains v0.0.0.3 Dev";

class Game
{
    public:
        Game();
        void Start();
    private:
        // Functions
        void GetInput(), ProcessEvents(), ProcessInput(), Update(), Display();

        // Constants
        static const int windowWidth = 1024;
        static const int windowHeight = 768;

        // Variables
        sf::VideoMode vidMode;
        sf::RenderWindow window;
        bool playing;
        float elapsedTime;

        Player player; // TODO: use an entity list or something similar
        sf::Image playerImg; // TODO: make a file loading/resource manager class or something
};

#endif // GAME_H