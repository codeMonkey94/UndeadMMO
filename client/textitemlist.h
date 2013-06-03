// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef TEXTITEMLIST_H
#define TEXTITEMLIST_H

#include <SFML/Graphics.hpp>
#include <deque>

#include "scrollbar.h"
#include "textitem.h"

/**
    A scrollable "window" that contains text items.
*/
class TextItemList: public sf::Drawable
{
    public:
        TextItemList();
        ~TextItemList();

        //Used when the itemList needs to be interacted with
        std::string handleEvents(sf::RenderWindow& window);

        void setupList(sf::RenderWindow&, sf::FloatRect, std::string&, unsigned int);
        sf::FloatRect getListDimensions();

        void setClickable(bool);
        bool getClickable();

        void addTextItem(const std::string&);

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

        void scrollDown(unsigned int);
        void scrollUp(unsigned int);

        void setTextFont(std::string&);

        void handleScrolling(sf::Event&, sf::RenderWindow&);
        void handleMouseClicked(sf::Event&, sf::RenderWindow&);
        void handleResize(sf::Event&, sf::RenderWindow&);

    private:

        sf::Vector2f getNewItemPos();

        sf::View itemListView;

        //Dimensions of the actual list
        sf::FloatRect viewableArea;
        sf::RectangleShape viewableAreaBox;

        ScrollBar scrollBar;
        std::deque<TextItem> textItemList;

        bool isClickable;
        bool isReady;
        int selection;

        sf::Font textItemFont;
        unsigned int textFontSize;

};

#endif // TEXTITEMLIST_H