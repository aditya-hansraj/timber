#include<SFML/Graphics.hpp>
#include<sstream>
using namespace std;
using namespace sf;

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main() {
    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "TIMBER GAME!!!", Style::Fullscreen);

    // Load textures
    Texture textureBackground;
    textureBackground.loadFromFile("./assets/graphics/background.jpeg");
    Sprite spriteBackground(textureBackground);

    Texture textureTree;
    textureTree.loadFromFile("./assets/graphics/tree.jpeg");
    Sprite spriteTree(textureTree);
    spriteTree.setPosition(800, 0);

    // Clouds
    Texture textureCloud;
    textureCloud.loadFromFile("./assets/graphics/cloud.png");
    Sprite spriteCloud(textureCloud);
    spriteCloud.setPosition(0, 0);

    Texture textureCloud1;
    textureCloud1.loadFromFile("./assets/graphics/cloud.png");
    Sprite spriteCloud1(textureCloud1);
    spriteCloud1.setPosition(0, 250);

    Texture textureCloud2;
    textureCloud2.loadFromFile("./assets/graphics/cloud.png");
    Sprite spriteCloud2(textureCloud2);
    spriteCloud2.setPosition(0, 500);

    // Bee
    Texture textureBee;
    textureBee.loadFromFile("./assets/graphics/bee.png");
    Sprite spriteBee(textureBee);
    spriteBee.setPosition(100, 700);

    // Branches
    Texture textureBranch;
    textureBranch.loadFromFile("./assets/graphics/branch.png");
    Sprite spriteBranches[NUM_BRANCHES];
    for (int i = 0; i < NUM_BRANCHES; i++) {
        spriteBranches[i].setTexture(textureBranch);
        spriteBranches[i].setPosition(2000, 2000);
        spriteBranches[i].setOrigin(220, 20);
    }

    // Player
    Texture texturePlayer;
    texturePlayer.loadFromFile("./assets/graphics/player.png");
    Sprite spritePlayer(texturePlayer);
    spritePlayer.setPosition(580, 720);
    side PlayerSide = side::LEFT;

    // RIP
    Texture textureRip;
    textureRip.loadFromFile("./assets/graphics/rip.png");
    Sprite spriteRip(textureRip);
    spriteRip.setPosition(2000, 2000);

    // Axe
    Texture textureAxe;
    textureAxe.loadFromFile("./assets/graphics/axe.png");
    Sprite spriteAxe(textureAxe);
    spriteAxe.setPosition(710, 820);
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Log
    Texture textureLog;
    textureLog.loadFromFile("./assets/graphics/log.png");
    Sprite spriteLog(textureLog);
    spriteLog.setPosition(810, 720);
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // Game state variables
    bool paused = true;
    bool acceptInput = false;
    int score = 0;
    Font font;
    Font Score;
    font.loadFromFile("font/KOMIKAP_.ttf");
    Score.loadFromFile("font/KOMIKAP_.ttf");

    Text messageText;
    Text scoreText;
    messageText.setFont(font);
    scoreText.setFont(Score);
    messageText.setString("Press Enter to Start");
    scoreText.setString("Score = 0");
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(55);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(0 / 2.0f, 0 / 2.0f);
    messageText.setFillColor(Color::Red);
    scoreText.setFillColor(Color::Yellow);

    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setPosition(1920 / 2.0f - timeBarStartWidth / 2, 980);
    timeBar.setFillColor(Color::Green);
    float timeRemaining = 6.0f;
    float timeBarStartWidthPerSec = timeBarStartWidth / timeRemaining;

    while (window.isOpen()) {
        Event event1;
        while (window.pollEvent(event1)) {
            if (event1.type == event1.Closed) window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Return)) {
            paused = false;
            score = 0;
            timeRemaining = 6.0f;
            for (int i = 0; i < NUM_BRANCHES; i++) {
                branchPositions[i] = side::NONE;
            }
            spritePlayer.setPosition(580, 720);
            spriteRip.setPosition(2000, 2000);
            acceptInput = true;
        }

        if (acceptInput) {
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                PlayerSide = side::RIGHT;
                score++;
                timeRemaining += 0.15 + (2 / score);
                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(1200, 720);
                updateBranches(score);
                spriteLog.setPosition(810, 720);
                logActive = true;
                logSpeedX = -5000;
                acceptInput = false;
            }

            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                PlayerSide = side::LEFT;
                score++;
                timeRemaining += 0.15 + (2 / score);
                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(580, 720);
                updateBranches(score);
                spriteLog.setPosition(810, 720);
                logActive = true;
                logSpeedX = 5000;
                acceptInput = false;
            }
        }

        if (!paused) {
            Time dt = clock.restart();
            timeRemaining = (timeRemaining - dt.asSeconds());
            timeBar.setSize(Vector2f(timeBarStartWidthPerSec * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f) {
                paused = true;
                messageText.setString("Out Of Time!!!");
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            }

            // Bee movement logic
            if (!beeActive) {
                srand((int)time(0));
                beeSpeed = rand() % 200 + 200;
                srand((int)time(0) * 10);
                float height = rand() % 500 + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else {
                spriteBee.setPosition(spriteBee.getPosition().x - beeSpeed * dt.asSeconds(), spriteBee.getPosition().y);
                if (spriteBee.getPosition().x < -100) {
                    beeActive = false;
                }
            }

            // Cloud movement logic
            if (!cloudActive) {
                srand((int)time(0) * 10);
                cloudSpeed = rand() % 200;
                srand((int)time(0) * 10);
                float height = rand() % 150;
                spriteCloud.setPosition(-2, height);
                cloudActive = true;
            }
            else {
                spriteCloud.setPosition(spriteCloud.getPosition().x + cloudSpeed * dt.asSeconds(), spriteCloud.getPosition().y);
                if (spriteCloud.getPosition().x > 1920) {
                    cloudActive = false;
                }
            }

            if (!cloud1Active) {
                srand((int)time(0) * 20);
                cloud1Speed = rand() % 200 + 100;
                srand((int)time(0) * 20);
                float height = rand() % 300 - 150;
                spriteCloud1.setPosition(-2, height);
                cloud1Active = true;
            }
            else {
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + cloud1Speed * dt.asSeconds(), spriteCloud1.getPosition().y);
                if (spriteCloud1.getPosition().x > 1920) {
                    cloud1Active = false;
                }
            }

            if (!cloud2Active) {
                srand((int)time(0) * 30);
                cloud2Speed = rand() % 200 + 50;
                srand((int)time(0) * 30);
                float height = rand() % 450 - 150;
                spriteCloud2.setPosition(-2, height);
                cloud2Active = true;
            }
            else {
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + cloud2Speed * dt.asSeconds(), spriteCloud2.getPosition().y);
                if (spriteCloud2.getPosition().x > 1920) {
                    cloud2Active = false;
                }
            }

            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            // Update branches
            for (int i = 0; i < NUM_BRANCHES; i++) {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT) {
                    spriteBranches[i].setPosition(610, height);
                    spriteBranches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT) {
                    spriteBranches[i].setPosition(1330, height);
                    spriteBranches[i].setRotation(0);
                }
                else {
                    spriteBranches[i].setPosition(2550, height);
                }
            }
        }

        // Clear and render everything
        window.clear();
        window.draw(spriteBackground);
        window.draw(spriteTree);
        if (paused) {
            window.draw(messageText);
        }
        window.draw(spriteCloud);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteBee);
        window.draw(scoreText);
        window.draw(timeBar);
        for (int i = 0; i < NUM_BRANCHES; i++) {
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
    // Shift the branch positions
    for (int j = NUM_BRANCHES - 1; j >= 1; j--) {
        branchPositions[j] = branchPositions[j - 1];
    }

    // Randomly generate the first branch
    srand((int)time(0) + seed);
    int r = rand() % 3;
    switch (r) {
        case 0:
            branchPositions[0] = side::LEFT;
            break;
        case 1:
            branchPositions[0] = side::RIGHT;
            break;
        default:
            branchPositions[0] = side::NONE;
            break;
    }
}

