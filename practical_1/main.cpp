#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cstdlib>
#include <direct.h>
#include <vector>

int main(){
    std::cout << "Starting the application..." << std::endl;
    char cwd[1024];
    if(_getcwd(cwd,sizeof(cwd)) != nullptr){std::cout << "Current Path: " << cwd << std::endl; }
    else{std::cerr << "Error getting current path." << std::endl;}
    //the characters i have made are quite larger than expected... whoops 
    //so i have made the display quite large for now
    //but we can make the players smaller later!!
    //for now we have the 'x' button as attack which works... kinda 
    sf::RenderWindow window(sf::VideoMode(1000, 512), "Sword Attack Game", sf::Style::Close);
    window.setFramerateLimit(60);
    sf::Texture texture;
    if(!texture.loadFromFile("player_sword_atk_64x64.png")){
        std::cerr << "Texture Size: " << texture.getSize().x << "x" << texture.getSize().y << std::endl;
        std::cerr << "Failed to load sprite sheet! Make sure the file path is correct." << std::endl;
        std::cin.get();
        return -1;
    }
    std::cout << "Loaded Texture Size: " << texture.getSize().x << "x" << texture.getSize().y << std::endl;
    std::vector<sf::IntRect> attackFrames;
    int frameWidth = 256;
    int frameHeight = 256;
    int numFrames = 6;
    for (int i = 0; i < numFrames; ++i) {attackFrames.emplace_back(i * frameWidth, 0, frameWidth, frameHeight);}
    sf::Sprite playerSprite;
    playerSprite.setTexture(texture);
    playerSprite.setTextureRect(attackFrames[0]);
    playerSprite.setPosition(400, 100);
    playerSprite.setScale(1.5f, 1.5f);
    sf::Clock animationClock;
    int currentFrame = 0;
    float frameDuration = 1.0f / 13.0f;
    sf::RectangleShape frameOutline;
    frameOutline.setSize(sf::Vector2f(frameWidth * playerSprite.getScale().x, frameHeight * playerSprite.getScale().y));
    frameOutline.setFillColor(sf::Color::Transparent);
    frameOutline.setOutlineColor(sf::Color::Red);
    frameOutline.setOutlineThickness(2);
    frameOutline.setPosition(playerSprite.getPosition());
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){if(event.type == sf::Event::Closed){window.close();}}
        bool isAttacking = sf::Keyboard::isKeyPressed(sf::Keyboard::X) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        if(isAttacking){
            if(animationClock.getElapsedTime().asSeconds() > frameDuration){
                currentFrame = (currentFrame + 1) % numFrames;
                playerSprite.setTextureRect(attackFrames[currentFrame]);
                animationClock.restart();
            }
        }else{
            currentFrame = 0;
            playerSprite.setTextureRect(attackFrames[currentFrame]);
        }
        frameOutline.setPosition(playerSprite.getPosition());
        window.clear(sf::Color::Yellow);
        window.draw(playerSprite);
        window.draw(frameOutline);
        window.display();
    }
    std::cin.get();
    return 0;
}