// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "messagestate.h"
#include <iostream>
#include <sstream>
#include "paths.h"

MessageState::MessageState(GameObjects& gameObjects): CommonState(gameObjects)
{
    sf::Vector2f windowSize;
    windowSize.x = objects.windowSize.x;
    windowSize.y = objects.windowSize.y;

    messageMenu.setUpMenu(Paths::menuBgImage,                               //Background file
                        sf::Color (25, 25, 25, 200),
                       32,                                                  //Font size
                       sf::Vector2f(windowSize.x / 2.3, windowSize.y / 1.5),    //Button position
                       &objects.window,                                     //Rendering window
                       &objects.fontBold
                       );

    messageMenu.addMenuButton("( OK )");

    messageFont.loadFromFile(Paths::boldFont);

    messageText.setFont(messageFont);
    messageText.setCharacterSize(40);
    messageText.setColor(sf::Color::White);
    messageText.setPosition(50, 200);
}

MessageState::~MessageState()
{
    messageMenu.clearButtons();
}

void MessageState::processArgs(const StateArgs& args)
{
    if (args.empty())
        messageText.setString("No details were returned.");
    else
        messageText.setString(args.front());
}

void MessageState::handleEvents()
{
    sf::Event event;
    while(objects.window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                stateEvent.exitGame();
                break;

            case sf::Event::MouseMoved:
                messageMenu.handleMouseMovement(event);
                break;

            case sf::Event::MouseButtonReleased:
                processChoice(messageMenu.handleMouseReleased(event));
                break;

            //Allow user to make selections with the keyboard. Enter makes a selection
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    stateEvent.popState();
                else
                    processChoice(messageMenu.handleKeyPressed(event));
                break;

            case sf::Event::Resized:
                messageMenu.handleResize(event);
                break;

            default:
                break;
        }
    }
}

void MessageState::processChoice(int choice)
{
    if (choice == 1)
        stateEvent.popState();
}

void MessageState::update()
{
    messageMenu.updateMenu();
}

void MessageState::draw()
{
    objects.window.clear();

    objects.window.draw(messageMenu);
    objects.window.draw(messageText);

    objects.window.display();
}
