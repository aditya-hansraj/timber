#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <ctime>
#include <cstdlib>

// g++ -o out/timber main.cpp -lsfml-graphics -lsfml-window -lsfml-system

using namespace std;
using namespace sf;

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
enum class side{LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
    VideoMode vm(1600, 900);	
    View view(FloatRect(0, 0, 1600, 900));
    RenderWindow window(vm, "TIMBER GAME!!!", Style::Default);
    window.setView(view);
    
    //---------------------------------------------------------------
    //background
    Texture textureBackground;//texture cannot move
    textureBackground.loadFromFile("./assets/graphics/background.jpeg");
    Sprite spriteBackground;//sprite can move
    spriteBackground.setTexture(textureBackground);//textureBackground contains the image
    spriteBackground.setPosition(0, 0);//the position where the img is set
    
    //tree
    Texture textureTree;//texture cannot move
    textureTree.loadFromFile("./assets/graphics/tree.jpeg");
    Sprite spriteTree;//sprite can move
    spriteTree.setTexture(textureTree);//textureBackground contains the image
    spriteTree.setPosition(650, 0);//the position where the img is set
    
    //cloud
    Texture textureCloud;//texture cannot move
    textureCloud.loadFromFile("./assets/graphics/cloud.png");
    Sprite spriteCloud;//sprite can move
    spriteCloud.setTexture(textureCloud);//textureBackground contains the image
    spriteCloud.setPosition(0, -50);//the position where the img is set
    
    //cloud1
    Sprite spriteCloud1;//sprite can move
    spriteCloud1.setTexture(textureCloud);//textureBackground contains the image
    spriteCloud1.setPosition(0, 50);//the position where the img is set
    
    //cloud2
    Sprite spriteCloud2;//sprite can move
    spriteCloud2.setTexture(textureCloud);//textureBackground contains the image
    spriteCloud2.setPosition(0, 0);//the position where the img is set
    
    //bee
    Texture textureBee;
    textureBee.loadFromFile("./assets/graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(1500, 555);
    
    //branch
    Texture textureBranch;
    textureBranch.loadFromFile("./assets/graphics/branch.png");
    Sprite spriteBranches[NUM_BRANCHES];
    for (int i = 0; i < NUM_BRANCHES; i++) {
        spriteBranches[i].setTexture(textureBranch);
        spriteBranches[i].setPosition(2000, 2000);
        spriteBranches[i].setOrigin(220, 20);
    }
    
    //player added
    Texture texturePlayer;
    texturePlayer.loadFromFile("./assets/graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(500, 720);
    side PlayerSide = side::LEFT;//player left
    
    //RIP 
    Texture textureRip;
    textureRip.loadFromFile("./assets/graphics/rip.png");
    Sprite spriteRip;
    spriteRip.setTexture(textureRip);
    spriteRip.setPosition(600, 860);
    
    //Axe
    Texture textureAxe;
    textureAxe.loadFromFile("./assets/graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(710, 820);
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;
    
    //Log
    Texture textureLog;
    textureLog.loadFromFile("./assets/graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(650, 720);
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;
    
    //---------------------------------------------------------------------
    bool beeActive = false;
    float beeSpeed = 0.0f;
    bool cloudActive = false;
    float cloudSpeed = 0.0f;
    bool cloud1Active = false;
    float cloud1Speed = 0.0f;
    bool cloud2Active = false;
    float cloud2Speed = 0.0f;
    Clock clock;
    bool paused = true;
    bool acceptInput = false;
    int score = 0;
    Font font;
    Font Score;
    font.loadFromFile("./assets/fonts/brownie-stencil-font/BrownieStencil-8O8MJ.ttf");
    Score.loadFromFile("./assets/fonts/brownie-stencil-font/BrownieStencil-8O8MJ.ttf");
    Text messageText;
    Text scoreText;
    messageText.setFont(font);
    scoreText.setFont(Score);
    messageText.setString("Press Enter to Start");
    scoreText.setString("Score = 0");
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(55);
    // Fix: Center message text to 1600x900 resolution
    messageText.setPosition(1600/2.0f, 900/2.0f);
    scoreText.setPosition(0/2.0f, 0/2.0f);
    messageText.setFillColor(Color::Red);
    scoreText.setFillColor(Color::Yellow);
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    // Fix: Adjust time bar position for 1600x900 resolution
    timeBar.setPosition(1000, 800);
    timeBar.setFillColor(Color::Green);
    float timeRemaining = 6.0f;
    float timeBarStartWidthPerSec = timeBarStartWidth/timeRemaining;

    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == event.Closed)
            {
                window.close();
            }
            if(event.type == Event::KeyReleased && !paused)
            {
                acceptInput = true;
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        } //event while close
        
        if(Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }
        
        if(Keyboard::isKeyPressed(Keyboard::Return)) {
            paused = false;
            score = 0;
            timeRemaining = 6.0f;
            
            for(int i = 0; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }
            
            spritePlayer.setPosition(500, 720);
            spriteRip.setPosition(2000, 2000);
            acceptInput = true;
        } //if closed for return
        
        if(acceptInput)
        {
            if(Keyboard::isKeyPressed(Keyboard::Right))
            {
                PlayerSide = side::RIGHT;
                score++;
                timeRemaining += 0.15 + (2/score);
                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(980, 720);
                updateBranches(score);
                spriteLog.setPosition(650, 720);
                logActive = true;
                logSpeedX = -5000;
                acceptInput = false;
            } //if right key pressed
            
            if(Keyboard::isKeyPressed(Keyboard::Left))
            {
                PlayerSide = side::LEFT;
                score++;
                timeRemaining += 0.15 + (2/score);
                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(500, 720);
                updateBranches(score);
                spriteLog.setPosition(650, 720);
                logActive = true;
                logSpeedX = 5000;
                acceptInput = false;
            } //if left key pressed
        }
        
        if(!paused) {
            Time dt = clock.restart();
            timeRemaining = (timeRemaining - dt.asSeconds());
            timeBar.setSize(Vector2f(timeBarStartWidthPerSec * timeRemaining, timeBarHeight));
            
            if(timeRemaining <= 0.0f) {
                paused = true;
                messageText.setString("Out Of Time!!!");
                // Fix: Center message text to 1600x900 resolution
                messageText.setPosition(1600/2.0f, 900/2.0f);
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
            }  
            
            if(!beeActive) {
                srand((int)time(0));
                beeSpeed = rand() % 200 + 200;
                srand((int)time(0) * 10);
                float height = rand() % 500 + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else {
                spriteBee.setPosition(spriteBee.getPosition().x - beeSpeed * dt.asSeconds(), spriteBee.getPosition().y);
                if(spriteBee.getPosition().x < -100) {
                    beeActive = false;
                }
            }
            
            if(!cloudActive) {
                srand((int)time(0) * 10);
                cloudSpeed = rand() % 200;
                srand((int)time(0) * 10);
                float height = rand() % 150;
                spriteCloud.setPosition(-2, height);
                cloudActive = true;
            }
            else {
                spriteCloud.setPosition(spriteCloud.getPosition().x + cloudSpeed * dt.asSeconds(), spriteCloud.getPosition().y);
                // Fix: Adjust cloud boundary check for 1600x900 resolution
                if(spriteCloud.getPosition().x > 1600) {
                    cloudActive = false;
                }
            }
            
            if(!cloud1Active) {
                srand((int)time(0) * 20);
                cloud1Speed = rand() % 200 + 100;
                srand((int)time(0) * 20);
                float height = rand() % 300 - 150;
                spriteCloud1.setPosition(-2, height);
                cloud1Active = true;
            }
            else {
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + cloud1Speed * dt.asSeconds(), spriteCloud1.getPosition().y);
                // Fix: Adjust cloud boundary check for 1600x900 resolution
                if(spriteCloud1.getPosition().x > 1600) {
                    cloud1Active = false;
                }
            }
            
            if(!cloud2Active) {
                srand((int)time(0) * 30);
                cloud2Speed = rand() % 200 + 50;
                srand((int)time(0) * 30);
                float height = rand() % 450 - 150;
                spriteCloud2.setPosition(-2, height);
                cloud2Active = true;
            }
            else {
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + cloud2Speed * dt.asSeconds(), spriteCloud2.getPosition().y);
                // Fix: Adjust cloud boundary check for 1600x900 resolution
                if(spriteCloud2.getPosition().x > 1600) {
                    cloud2Active = false;
                }
            }
            
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str()); 
            
            for(int i = 0; i < NUM_BRANCHES; i++) {
                float height = i * 150;
                if(branchPositions[i] == side::LEFT)
                {
                    spriteBranches[i].setPosition(430, height);
                    spriteBranches[i].setRotation(180);
                }
                else if(branchPositions[i] == side::RIGHT)
                {
                    spriteBranches[i].setPosition(1170, height);
                    spriteBranches[i].setRotation(0);
                }
                else
                {
                    spriteBranches[i].setPosition(2550, height);
                }
            }
            
            if(logActive)
            {
                spriteLog.setPosition(spriteLog.getPosition().x + logSpeedX * dt.asSeconds(), 
                                    spriteLog.getPosition().y + logSpeedY * dt.asSeconds());
                // Fix: Adjust log boundary check for 1600x900 resolution
                if(spriteLog.getPosition().x < 100 || spriteLog.getPosition().x > 1600)
                {
                    logActive = false;
                    spriteLog.setPosition(650, 720);
                }
            }
            
            //Player Death
            if(branchPositions[5] == PlayerSide) {
                paused = true;
                acceptInput = false;
                spritePlayer.setPosition(2000, 320);
                spriteRip.setPosition(525, 760);
                messageText.setString("SQUISHED!"); 
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                                     textRect.top + textRect.height / 2.0f);
                // Fix: Center message text to 1600x900 resolution
                messageText.setPosition(1600 / 2.0f, 900 / 2.0f);
            }
        }  //if not paused end 
        
        window.clear();
        window.draw(spriteBackground);
        window.draw(spriteTree);
        if(paused)
        {
            window.draw(messageText);
        }
        window.draw(spriteCloud);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteBee);
        window.draw(scoreText);
        window.draw(timeBar);
        for(int i = 0; i < NUM_BRANCHES; i++) {
            window.draw(spriteBranches[i]);
        }
        window.draw(spritePlayer);
        window.draw(spriteRip);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.display();
    }

    return 0;
}

void updateBranches(int seed) {
    for(int j = (NUM_BRANCHES - 1); j > 0; j--)
    {
        branchPositions[j] = branchPositions[j-1];
    }
    
    srand((int)time(0) + seed);
    int r = rand() % 5;
    
    switch(r) {
        case 0:
            branchPositions[0] = side::LEFT;  // Fixed: was == instead of =
            break;
        case 1:
            branchPositions[0] = side::RIGHT;
            break;   
        default:
            branchPositions[0] = side::NONE;
            break;
    }
}

//g++ timber.cpp -o timber -lsfml-system -lsfml-window -lsfml-graphics
//./test earlier in your code
