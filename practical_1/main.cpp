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
    // Stamina Bar Setup
    sf::RectangleShape staminaBar;
    staminaBar.setSize(sf::Vector2f(200, 20)); // Width of 200 pixels, height of 20 pixels
    staminaBar.setFillColor(sf::Color::Green);
    staminaBar.setPosition(10, 10); // Top left corner, with 10 pixel padding from edges
    float maxStamina = 100.0f;
    float currentStamina = maxStamina;
    float staminaDepletionRate = 10.0f;
    float staminaRecoveryRate = 5.0f;
    static bool attackInProgress = false;
    static bool attackTriggered = false;
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){if(event.type == sf::Event::Closed){window.close();}}
        if(!attackInProgress && (sf::Keyboard::isKeyPressed(sf::Keyboard::X) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))){
            if(currentStamina > staminaDepletionRate){
                attackInProgress = true;
                // attackTriggered is used to prevent re-triggering while attacking
            attackTriggered = true;
                currentFrame = 0;
                animationClock.restart();
                currentStamina -= staminaDepletionRate;
                if (currentStamina < 0) currentStamina = 0;
            }
        }

        if(attackInProgress){
            if(animationClock.getElapsedTime().asSeconds() > frameDuration){
                currentFrame++;
                if(currentFrame >= numFrames){
                    currentFrame = 0;
                    attackInProgress = false;
                    attackTriggered = false;
                }
                playerSprite.setTextureRect(attackFrames[currentFrame]);
                animationClock.restart();
            }
        } else {
            // Recover stamina when not attacking
            if (currentStamina < maxStamina) {
                currentStamina += staminaRecoveryRate * animationClock.getElapsedTime().asSeconds();
                if (currentStamina > maxStamina) currentStamina = maxStamina;
            }
        }

        // Update the stamina bar size based on current stamina
        float staminaPercentage = currentStamina / maxStamina;
        staminaBar.setSize(sf::Vector2f(200 * staminaPercentage, 20));
        frameOutline.setPosition(playerSprite.getPosition());
        window.clear(sf::Color::Yellow);
        window.draw(playerSprite);
        window.draw(frameOutline);
        window.draw(staminaBar);
        window.display();
    }
    std::cin.get();
    return 0;
}