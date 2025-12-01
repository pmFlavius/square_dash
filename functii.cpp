#include "functii.h"

Game::Game() : window(VideoMode(1280, 720), "Square Dash", Style::Close)
{
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(144);
    
    if (!music.openFromFile("andromeda.ogg"))
    {
        perror("nu avem muzica");
    }
    else
    {
        music.setLoop(true);
        music.play();
    }
}

void Game::run()
{
    Clock clock;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
            {
                if (!level.isGameOver())
                {
                    player.jump();
                }
            }
            
            //restart pe R
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::R)
            {

                return;
            }
        }
        
        float dt = clock.restart().asSeconds();
        
        if (!level.isGameOver())
        {
            player.update(dt);
            level.update(dt, player);
        }
        
        window.clear(Color(20, 20, 30)); 
        level.draw(window);
        player.draw(window);
        window.display();
    }
}

void Player::update(float dt)
{
    vitezaY += gravitate * dt;
    Vector2f pos = corp.getPosition();
    pos.y += vitezaY * dt;
    
    if (pos.y > groundY - corp.getSize().y)
    {
        pos.y = groundY - corp.getSize().y;
        vitezaY = 0.f;
    }
    
    corp.setPosition(pos);
}

void Player::jump()
{
    if (isOnGround())
    {
        vitezaY = impulsSarit;
    }
}

void Player::draw(RenderWindow &window)
{
    window.draw(corp);
}

void Obstacle::update(float dt)
{
    shape.move(-viteza * dt, 0);
}

void Obstacle::draw(RenderWindow &window)
{
    window.draw(shape);
}

void Level::draw(RenderWindow &window)
{
    for (auto &ob : obstacole)
        ob.draw(window);
    
    if (gameOver)
    {
        Font font;
        if (font.loadFromFile("arial.ttf"))
        {
            Text text("GAME OVER! Apasa R pentru restart", font, 40);
            text.setFillColor(Color::Red);
            text.setPosition(300, 300);
            window.draw(text);
        }
    }
}

void Level::update(float dt, Player &player)
{
    if (gameOver) return;
    
    //update obstacole
    for (auto &ob : obstacole)
        ob.update(dt);
    
    //verificare coliziune mai fair
    FloatRect playerBounds = player.getBounds();
    playerBounds.left += 5;
    playerBounds.top += 5;
    playerBounds.width -= 10;
    playerBounds.height -= 10;
    
    for (auto &ob : obstacole)
    {
        if (playerBounds.intersects(ob.getBounds()))
        {
            cout << "Ai pierdut!\n";
            gameOver = true;
            return;
        }
    }
    
    //sterge obstacolele care au iesit din ecran
    obstacole.erase(
        remove_if(obstacole.begin(), obstacole.end(),
                  [](Obstacle &ob) { return ob.getRight() < 0; }),
        obstacole.end());
    
    //spawn obstacole noi 
    float rightmostX = 0.f;
    
    //cea mai din dreapta poz a unui obstacol existent
    for (const auto &ob : obstacole)
    {
        float right = ob.getRight();
        if (right > rightmostX)
            rightmostX = right;
    }
    
    //spawn la poziția inițială,daca nu s obstacole
    if (obstacole.empty())
        rightmostX = spawnX - maxDistance;
    
    //spawn doar daca e suficient spațiu
    if (spawnX - rightmostX > minDistance)
    {
        float randomDistance = minDistance + rand() % int(maxDistance - minDistance);
        float newX = rightmostX + randomDistance;
        
        float height = heightMin + rand() % int(heightMax - heightMin - 200);
        
        int textureIndex = rand() % 4;
        shared_ptr<Texture> tex = obstacleTextures[textureIndex];
        
        obstacole.emplace_back(newX, groundY - height, 50.f, height, obstacleSpeed, tex);
    }
}

Level::Level()
{
    srand(static_cast<unsigned>(time(0)));
    loadTextures(); //incarca texturile la inceput
}

void Level::loadTextures()
{
    //incarcă cele 4 texturi pentru obstacole o singură data
    for (int i = 1; i <= 4; i++)
    {
        auto tex = make_shared<Texture>();
        string texturePath = "obstacle" + to_string(i) + ".jpg";
        
        if (!tex->loadFromFile(texturePath))
        {
            cout << "Nu s-a putut incarca: " << texturePath << endl;
            tex = nullptr; //marchează ca nereusit
        }
        
        obstacleTextures.push_back(tex);
    }
}