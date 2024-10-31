#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include <direct.h> //current directory
#include <vector>

int main(){
    std::cout << "Starting the application..." << std::endl; //starting message
    char cwd[1024]; //store player in directory
    if(_getcwd(cwd,sizeof(cwd)) != nullptr){std::cout << "Current Path: " << cwd << std::endl;} //print current working directory
    else{std::cerr << "Error getting current path." << std::endl;} //current path fails
    sf::RenderWindow window(sf::VideoMode(1000, 512), "Sword Attack Game", sf::Style::Close); //window size and title
    window.setFramerateLimit(60); //60 frames per second
    
    sf::Texture texture; //sprite object
    if(!texture.loadFromFile("player_sword_atk_64x64.png")){ //load sprite from file
        std::cerr << "Texture Size: " << texture.getSize().x << "x" << texture.getSize().y << std::endl; //size if load fails
        std::cerr << "Failed to load sprite sheet! Make sure the file path is correct." << std::endl; //load fails
        std::cin.get(); //wait for user input before closing
        return -1; //return error code
    }
    std::cout << "Loaded Texture Size: " << texture.getSize().x << "x" << texture.getSize().y << std::endl; //print loaded texture size

    std::vector<sf::IntRect> attackFrames; //vector to store animation frames
    int frameWidth = 256; //width of each frame in the sprite sheet
    int frameHeight = 256; //height of each frame in the sprite sheet
    int numFrames = 6; //total number of frames in the attack animation
    for(int i=0;i<numFrames;++i){attackFrames.emplace_back(i * frameWidth, 0, frameWidth, frameHeight);} //populate vector with frames
    
    sf::Sprite playerSprite; //create a sprite for the player
    playerSprite.setTexture(texture); //set the texture for the sprite
    playerSprite.setTextureRect(attackFrames[0]); //set the initial frame of the animation
    playerSprite.setPosition(400, 100); //set the initial position of the player
    playerSprite.setScale(1.5f, 1.5f); //scale the sprite to make it larger

    sf::Clock animationClock; //clock to control animation timing
    int currentFrame = 0; //track the current frame of the animation
    float frameDuration = 1.0f / 13.0f; //duration for each frame (for 13 frames per second)

    sf::RectangleShape frameOutline; //create a rectangle shape to outline the sprite
    frameOutline.setSize(sf::Vector2f(frameWidth * playerSprite.getScale().x, frameHeight * playerSprite.getScale().y)); //set size of the outline based on player sprite size
    frameOutline.setFillColor(sf::Color::Transparent); //set fill color to transparent
    frameOutline.setOutlineColor(sf::Color::Red); //set outline color to red
    frameOutline.setOutlineThickness(2); //set the thickness of the outline
    frameOutline.setPosition(playerSprite.getPosition()); //set position of the outline to match player sprite
    
    //stamina Bar Setup
    sf::RectangleShape staminaBar; //create a rectangle shape for the stamina bar
    staminaBar.setSize(sf::Vector2f(200, 20)); //width of 200 pixels, height of 20 pixels
    staminaBar.setFillColor(sf::Color::Green); //set fill color of the stamina bar to green
    staminaBar.setPosition(10, 10); //top left corner, with 10 pixel padding from edges
    float maxStamina = 100.0f; //maximum stamina value
    float currentStamina = maxStamina; //current stamina initialized to maximum
    float staminaDepletionRate = 10.0f; //amount of stamina used per attack
    float staminaRecoveryRate = 5.0f; //rate at which stamina recovers per second

    static bool attackInProgress = false; //flag to indicate if an attack is in progress
    static bool attackTriggered = false; //flag to indicate if an attack has been triggered

    //load attack sound
    sf::SoundBuffer attackBuffer; //create a sound buffer to hold the sound data
    if(!attackBuffer.loadFromFile("437113__cpawsmusic__sword-4.wav")){ //load sound file into buffer
        std::cerr << "Failed to load attack sound! Make sure the file path is correct." << std::endl; //error message if loading fails
        std::cin.get(); //wait for user input before closing
        return -1; //return error code
    }
    sf::Sound attackSound; //create a sound object to play the sound
    attackSound.setBuffer(attackBuffer); //set the buffer for the sound

    //fps counter setup
    sf::Font font; //create font for text
    if(!font.loadFromFile("arial.ttf")){ //load font from file
        std::cerr << "Failed to load font! Make sure the file path is correct." << std::endl; //error message if loading fails
        std::cin.get(); //wait for user input before closing
        return -1; //return error code
    }
    sf::Text fpsText; //create text object for fps counter
    fpsText.setFont(font); //set font for the text
    fpsText.setCharacterSize(14); //set character size for text
    fpsText.setFillColor(sf::Color::White); //set text color to white
    fpsText.setPosition(850, 10); //set position of fps counter in top right corner

    sf::Clock fpsClock; //clock to measure time for fps calculation
    int frameCount = 0; //initialize frame count
    float fps = 0.0f; //initialize fps value

    while(window.isOpen()){ //main game loop runs while the window is open
        sf::Event event; //event object to hold window events
        while(window.pollEvent(event)){if(event.type == sf::Event::Closed){window.close();}} //close if clicked
        
        if(!attackInProgress&&(sf::Keyboard::isKeyPressed(sf::Keyboard::X)||sf::Keyboard::isKeyPressed(sf::Keyboard::Space))){ //check if attack key is pressed and no attack is in progress
            if(currentStamina > staminaDepletionRate){ //check if there is enough stamina to attack
                attackInProgress = true; //set attack in progress to true
                attackTriggered = true; //set attack triggered to true to prevent re-triggering
                currentFrame = 0; //reset current frame to start animation from the beginning
                animationClock.restart(); //restart the animation clock
                currentStamina -= staminaDepletionRate; //decrease stamina by the depletion rate
                if(currentStamina < 0) currentStamina = 0; //ensure stamina does not go below 0
                attackSound.play(); //play attack sound
            }
        }

        if(attackInProgress){ //if attack is in progress
            if(animationClock.getElapsedTime().asSeconds() > frameDuration){ //check if it's time to move to the next frame
                currentFrame++; //increment the current frame
                if(currentFrame >= numFrames){ //if current frame exceeds number of frames
                    currentFrame = 0; //reset frame to 0
                    attackInProgress = false; //end the attack
                    attackTriggered = false; //reset attack trigger
                }
                playerSprite.setTextureRect(attackFrames[currentFrame]); //update the sprite to the current frame
                animationClock.restart(); //restart the animation clock for the next frame
            }
        }else{ //if not attacking
            if(currentStamina < maxStamina){ //recover stamina when not attacking
                currentStamina += staminaRecoveryRate * animationClock.getElapsedTime().asSeconds(); //increase stamina by recovery rate
                if(currentStamina > maxStamina) currentStamina = maxStamina; //ensure stamina does not exceed maximum
            }
        }

        //update the stamina bar size based on current stamina
        float staminaPercentage = currentStamina / maxStamina; //calculate stamina as percentage of max stamina
        staminaBar.setSize(sf::Vector2f(200 * staminaPercentage, 20)); //update the width of the stamina bar based on percentage
        if(staminaPercentage > 0.5f){ //set stamina bar color based on current stamina level
            staminaBar.setFillColor(sf::Color::Green); //green if more than 50%
        }else if(staminaPercentage > 0.25f){
            staminaBar.setFillColor(sf::Color::Yellow); //yellow if between 25% and 50%
        }else{//fyi.. we can see the yellow as the background is yellow
            staminaBar.setFillColor(sf::Color::Red); //red if below 25%
        }

        //fps counter update
        frameCount++; //increment frame count
        if(fpsClock.getElapsedTime().asSeconds() >= 1.0f){ //if one second has elapsed
            fps = frameCount / fpsClock.getElapsedTime().asSeconds(); //calculate fps
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps))); //update fps text
            frameCount = 0; //reset frame count
            fpsClock.restart(); //restart fps clock
        }

        frameOutline.setPosition(playerSprite.getPosition()); //update frame outline position to match player sprite
        window.clear(sf::Color::Yellow); //clear the window with yellow background
        window.draw(playerSprite); //draw the player sprite
        window.draw(frameOutline); //draw the outline around the player sprite
        window.draw(staminaBar); //draw the stamina bar
        window.draw(fpsText); //draw the fps counter
        window.display(); //display everything drawn to the window
    }
    std::cin.get(); //wait for user input before closing
    return 0;} //return success code