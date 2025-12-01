#ifndef FUNCTII_H_
#define FUNCTII_H_

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;
using namespace std;

class Player
{
    RectangleShape corp;
    Texture texture;
    float vitezaY = 0.f;
    float gravitate = 1500.f;
    float impulsSarit = -1000.f;
    float groundY = 600.f;
public:
    Player()
    {
        corp.setSize({100.f, 100.f});
        
        if (!texture.loadFromFile("player.jpg"))
        {
            corp.setFillColor(Color::Cyan);
        }
        else
        {
            corp.setTexture(&texture);
        }
        
        corp.setPosition(200.f, groundY - corp.getSize().y);
    }
    
    float getGravitate() { return gravitate; }
    float getImpuls() { return impulsSarit; }
    float getViteza() const { return this->vitezaY; }
    
    bool isOnGround() const
    {
        return corp.getPosition().y >= groundY - corp.getSize().y - 0.5f;
    }
    
    void setViteza(float viteza) { this->vitezaY = viteza; }
    void update(float dt);
    void jump();
    void draw(RenderWindow &window);
    
    FloatRect getBounds() const
    {
        return corp.getGlobalBounds();
    }
};

class Obstacle
{
    RectangleShape shape;
    shared_ptr<Texture> texture;
    float viteza;
public:
    Obstacle(float x, float y, float width, float height, float speed, shared_ptr<Texture> tex) : viteza(speed), texture(tex)
    {
        shape.setPosition(x, y);
        shape.setSize({width, height});
        
        if (texture && texture->getSize().x > 0)
        {
            shape.setTexture(texture.get());
        }
        else
        {
            shape.setFillColor(Color::White);
        }
    }
    
    void update(float dt);
    void draw(RenderWindow &window);
    
    FloatRect getBounds() const
    {
        return shape.getGlobalBounds();
    }
    
    float getRight() const
    {
        return shape.getPosition().x + shape.getSize().x;
    }
};

class Level
{
    vector<Obstacle> obstacole;
    vector<shared_ptr<Texture>> obstacleTextures; // Texturi partajate
    float obstacleSpeed = 300.f;      
    float groundY = 600.f;           
    float spawnX = 1280.f;            
    float minDistance = 250.f;        
    float maxDistance = 500.f;        
    float heightMin = 50.f;           
    float heightMax = 150.f;
    bool gameOver = false;
    
    void loadTextures(); 
    
public:
    Level();
    void update(float dt, Player &player);
    void draw(RenderWindow &window);
    bool isGameOver() const { return gameOver; }
};

class Game
{
private:
    RenderWindow window;
    Player player;
    Level level;
    Music music;
public:
    Game();
    void run();
};

#endif