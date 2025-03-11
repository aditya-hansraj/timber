#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
using namespace sf;
using namespace std;
// g++ -o out/timber main.cpp -lsfml-graphics -lsfml-window -lsfml-system

enum Side {
    LEFT, RIGHT, NONE
};

const int NUM_BRANCHES = 6;

Side branchPositon[NUM_BRANCHES];

void updateBranches(int seed);

int main() {
    VideoMode vm(1600, 900);
    View view(FloatRect(0, 0, 1600, 900));
    RenderWindow window(vm, "Timber");
    window.setView(view);

    // Textures
    Texture textureBG, textureTree, textureCloud, textureBee, textureBranch, texturePlayer;
    textureBG.loadFromFile("./assets/graphics/background.jpeg");
    textureTree.loadFromFile("./assets/graphics/tree.jpeg");
    textureCloud.loadFromFile("./assets/graphics/cloud.png");
    textureBee.loadFromFile("./assets/graphics/bee.png");
    textureBranch.loadFromFile("./assets/graphics/branch.png");
    texturePlayer.loadFromFile("./assets/graphics/player.png");

    // Sprites
    Sprite spriteBG, spriteTree, spriteCloud1, spriteCloud2, spriteCloud3, spriteBee, spritePlayer;
    Sprite spriteBranches[NUM_BRANCHES];

    spriteBG.setTexture(textureBG);
    spriteTree.setTexture(textureTree);
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    spriteBee.setTexture(textureBee);
    spritePlayer.setTexture(texturePlayer);

    spriteBG.setPosition(0, 0);
    spriteTree.setPosition(650, 0);
    spriteCloud1.setPosition(0, -50);
    spriteCloud2.setPosition(0, 50);
    spriteCloud3.setPosition(0, 0);
    spriteBee.setPosition(1500, 555);
    spritePlayer.setPosition(500, 708);

    Side brancPosition[NUM_BRANCHES];

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
    	float height = i * 150;
        spriteBranches[i].setTexture(textureBranch);
        spriteBranches[i].setPosition(2000, 2000);
        spriteBranches[i].setOrigin(220, 20);
    }

    // Fonts
    Font brownieStencil;
    brownieStencil.loadFromFile("./assets/fonts/brownie-stencil-font/BrownieStencil-8O8MJ.ttf");

    // Texts
    Text msgText, scoreText, timeText;
    msgText.setFont(brownieStencil);
    msgText.setString("Press <Space> to Start !");
    msgText.setCharacterSize(75);
    msgText.setPosition(800, 450);
    msgText.setFillColor(Color::Yellow);
    FloatRect textRect = msgText.getLocalBounds();
    msgText.setOrigin(textRect.top + textRect.width / 2.0, textRect.top + textRect.height / 2.0);

    scoreText.setFont(brownieStencil);
    scoreText.setString("Score: 0");
    scoreText.setCharacterSize(44);
    scoreText.setPosition(20, 20);
    scoreText.setFillColor(Color::Green);

    // Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setPosition(100, 800);
    timeBar.setFillColor(Color::Red);

    // Game state
    bool beeActive = false;
    float beeSpeed = 0.0f;

    bool cloud1Active = false, cloud2Active = false, cloud3Active = false;
    float cloud1Speed = 0.0f, cloud2Speed = 0.0f, cloud3Speed = 0.0f;

    bool paused = true;
    int score = 0;
    float timeRemaining = 6.0f; // game duration in seconds
    float timeBarStartWidthPerSec = timeBarStartWidth / timeRemaining;

    Clock clock;

    while (window.isOpen())
    {
        Event event1;
        while (window.pollEvent(event1))
        {
            if (event1.type == event1.Closed)
            {
                window.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
            cout << "Exited !" << endl;
            return 0;
        }

        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            paused = false;
        }

        Time dt = clock.restart();

        if (!paused)
        {
            // Handle bee movement
            if (!beeActive)
            {
                srand((int)time(0));
                beeSpeed = rand() % 200 + 200;
                srand((int)time(0) * 10);
                float height = rand() % 400 + 400;
                spriteBee.setPosition(1700, height);
                beeActive = true;
            }
            else
            {
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
                if (spriteBee.getPosition().x < -100)
                {
                    beeActive = false;
                    score++; // Increase score when bee is gone
                }
            }

            // Handle cloud movement
            if (!cloud1Active)
            {
                srand((int)time(0));
                cloud1Speed = rand() % 100 + 100;
                srand((int)time(0) * 10);
                float height = rand() % 25 + 25;
                spriteCloud1.setPosition(-400, height);
                cloud1Active = true;
            }
            else
            {
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
                if (spriteCloud1.getPosition().x > 1800)
                {
                    cloud1Active = false;
                }
            }

            if (!cloud2Active)
            {
                srand((int)time(0));
                cloud2Speed = rand() % 100 + 100;
                srand((int)time(0) * 10);
                float height = rand() % 50 + 50;
                spriteCloud2.setPosition(-888, height);
                cloud2Active = true;
            }
            else
            {
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
                if (spriteCloud2.getPosition().x > 1800)
                {
                    cloud2Active = false;
                }
            }

            if (!cloud3Active)
            {
                srand((int)time(0));
                cloud3Speed = rand() % 100 + 100;
                srand((int)time(0) * 10);
                float height = rand() % 75 + 75;
                spriteCloud3.setPosition(-1200, height);
                cloud3Active = true;
            }
            else
            {
                spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
                if (spriteCloud3.getPosition().x > 1800)
                {
                    cloud3Active = false;
                }
            }

            // Handle timer
            timeRemaining -= dt.asSeconds();
            timeBar.setSize(Vector2f(timeBarStartWidthPerSec * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f)
            {
                paused = true;
                msgText.setString("OUT OF TIME!!");
                msgText.setFillColor(Color::Red);
                FloatRect textRect = msgText.getLocalBounds();
                msgText.setOrigin(textRect.top + textRect.width / 2.0, textRect.top + textRect.height / 2.0);
            }

            // Update score text
            stringstream ss;
            ss << "Score: " << score;
            scoreText.setString(ss.str());
        }

        window.clear();
        window.draw(spriteBG);
        window.draw(spriteTree);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        window.draw(spriteBee);
        window.draw(spritePlayer); // Player sprite
        window.draw(scoreText);
        window.draw(timeBar);

        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(spriteBranches[i]);
        }

        if (paused)
        {
            window.draw(msgText);
        }

        window.display();
    }

    return 0;
}

void updateBranches(int seed) {
    for(int j = NUM_BRANCHES-1; j > 0; j--) {
        branchPositon[j] = branchPositon[j-1];
    }

    srand((int)time(0) + seed);
    int r = rand() % 5;
    switch(r) {
        case 0:
            branchPositon[0] = Side::LEFT;
            break;
        case 1:
            branchPositon[0] = Side::RIGHT;
        case 2:
            branchPositon[0] = Side::LEFT;
            break;
        case 3:
            branchPositon[0] = Side::NONE;
            break;
        case 4:
            branchPositon[0] = Side::RIGHT;
            break;
    }
}
