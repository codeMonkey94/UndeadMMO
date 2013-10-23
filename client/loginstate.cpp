// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "loginstate.h"
#include <iostream>
#include <sstream>
#include "paths.h"

LoginState::LoginState(GameObjects& gameObjects): State(gameObjects)
{
    sf::Vector2f windowSize;
    windowSize.x = objects.windowSize.x;
    windowSize.y = objects.windowSize.y;

    loginMenu.setUpMenu(Paths::menuBgImage,                                  //Background file
                        sf::Color( 25, 25, 25, 200),
                       16,                                                  //Font size
                       sf::Vector2f(windowSize.x / 4, windowSize.y / 1.2),    //Button position
                       objects                                              //Rendering window
                       );


    textItemList.setupList(objects.window, sf::FloatRect(0, 0, 1, .5), objects.fontBold, 16, true, true);

   /* for (int x = 1; x <= 150; x++)
    {
        std::stringstream tmp;
        tmp << "Test server " << x << "\t\t\tDescription: This is a game server where we play zombies!!!!!\t\t\tPlayers: 30/32";
        textItemList.addItemWithHiddenText(tmp.str(), "ayebear.com", sf::Color(190, 190, 190, 255));
    }
    */

    // TODO: Use a master server list
    textItemList.addItemWithHiddenText("Eric's Server", "ayebear.com", sf::Color(190, 190, 190, 255));
    textItemList.addItemWithHiddenText("My Local Server", "192.168.1.4", sf::Color(190, 190, 190, 255));
    textItemList.addItemWithHiddenText("HCC", "10.10.198.22", sf::Color(190, 190, 190, 255));

    textItemList.scrollToBottom();


    //Just using these because the width in pixels changes based on window size
    sf::Text usernameWidth("Username:__", objects.fontBold, fontSize);
    float userWidth = usernameWidth.getGlobalBounds().width;

    sf::Text passwordWidth("Password:__", objects.fontBold, fontSize);
    float passWidth = passwordWidth.getGlobalBounds().width;

    sf::Text dirConnectWidth("Direct Connect:__", objects.fontBold, fontSize);
    float directConnWidth = dirConnectWidth.getGlobalBounds().width;


    //Set up menuOption structs
    loginMenu.addMenuButton("Login");
    loginMenu.addMenuButton("Create Account");
    loginMenu.addMenuButton("Return to Main Menu");

    loginMenu.createLabel("Username: ", sf::Vector2f(windowSize.x / 4 - userWidth, windowSize.y / 1.5));
    loginMenu.createLabel("Password: ", sf::Vector2f(windowSize.x / 4 - passWidth, windowSize.y / 1.4));
    loginMenu.createLabel("Direct Connect: ", sf::Vector2f(windowSize.x / 1.5 - directConnWidth, windowSize.y / 1.4));

    usernameBox.setUp(fontSize, objects.fontBold, windowSize.x / 4, windowSize.y / 1.5, windowSize.x / 8, false);
    passwordBox.setUp(fontSize, objects.fontBold, windowSize.x / 4, windowSize.y / 1.4, windowSize.x / 8, true);
    directConnectBox.setUp(fontSize, objects.fontBold, windowSize.x / 1.5, windowSize.y / 1.4, windowSize.x / 8, false);

    string username = objects.config["username"].asString();
    string password = objects.config["password"].asString();
    string ip = objects.config["server"].asString();
    usernameBox.setString(username);
    passwordBox.setString(password);
    directConnectBox.setString(ip);
}

LoginState::~LoginState()
{
    loginMenu.clearButtons();
}

void LoginState::handleEvents()
{
    sf::Event event;
    while(objects.window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                action.exitGame();
                break;

            case sf::Event::MouseMoved:
                loginMenu.handleMouseMovement(event);
                break;

            case sf::Event::MouseWheelMoved:
                textItemList.handleScrolling(event, objects.window);
                break;

            case sf::Event::MouseButtonPressed:

                usernameBox.handleMouseClicked(event, objects.window);
                passwordBox.handleMouseClicked(event, objects.window);
                directConnectBox.handleMouseClicked(event, objects.window);
                loginMenu.handleMousePressed(event);

                if(textItemList.listContainsMouse(objects.window))
                    directConnectBox.setString(textItemList.handleMouseClicked(event, objects.window));
                break;

            case sf::Event::MouseButtonReleased:
                processChoice(loginMenu.handleMouseReleased(event));
                break;

            //Allow user to make selections with the keyboard. Enter makes a selection
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        action.popState();
                        break;

                    case sf::Keyboard::Return:
                        processChoice(loginMenu.handleKeyPressed(event));
                        break;

                    case sf::Keyboard::Key::Tab:
                        if (usernameBox.isActive())
                        {
                            usernameBox.setInput(false);
                            passwordBox.setInput(true);
                        }
                        else if( passwordBox.isActive())
                        {
                            passwordBox.setInput(false);
                            usernameBox.setInput(true);
                        }
                        break;

                    default:
                        break;
                }
                usernameBox.processInput(event.key.code);
                passwordBox.processInput(event.key.code);
                directConnectBox.processInput(event.key.code);
                loginMenu.handleKeyPressed(event);
                break;

            case sf::Event::TextEntered:
                if(usernameBox.isActive())
                    usernameBox.processTextEntered(event.text.unicode);
                else if(passwordBox.isActive())
                    passwordBox.processTextEntered(event.text.unicode);
                else if(directConnectBox.isActive())
                    directConnectBox.processTextEntered(event.text.unicode);
                break;

            case sf::Event::Resized:
                loginMenu.handleResize(event);
                break;

            default:
                break;
        }
    }
    usernameBox.updateCursor();
    passwordBox.updateCursor();
    directConnectBox.updateCursor();
}

void LoginState::processChoice(int choice)
{
    if (choice == 1)
    {
        string server = directConnectBox.getString();
        sf::IpAddress serverAddr(server);
        string username = usernameBox.getString();
        string password = passwordBox.getString();
        cout << "Logging into " << server << " with username = " << username << ", password = " << password << endl;
        int status = objects.netManager.logIn(serverAddr, username, password);
        if (status == Packet::LogInCode::Successful)
            action.pushState(StateType::Game);
        else
        {
            StateArgs args;
            args.push_back(Packet::LogInMessages[status - 1]);
            action.pushState(StateType::Message, args);
        }
    }
    else if (choice == 2)
    {
        string server = directConnectBox.getString();
        sf::IpAddress serverAddr(server);
        string username = usernameBox.getString();
        string password = passwordBox.getString();
        cout << "Creating account on " << server << " with username = " << username << ", password = " << password << endl;
        int status = objects.netManager.createAccount(serverAddr, username, password);
        StateArgs args;
        args.push_back(Packet::CreateAccountMessages[status - 1]);
        action.pushState(StateType::Message, args);
    }
    else if (choice == 3)
        action.popState();
}

void LoginState::update()
{
    loginMenu.updateMenu();
}

void LoginState::draw()
{
    objects.window.clear();
    objects.window.draw(loginMenu);
    objects.window.draw(textItemList);

    objects.window.draw(usernameBox);
    objects.window.draw(passwordBox);
    objects.window.draw(directConnectBox);

    objects.window.display();
}
