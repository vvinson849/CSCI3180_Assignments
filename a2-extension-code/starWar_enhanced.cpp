#include <iostream>
#include <ctime>
#include <vector>
#include <cassert>
#include <cstring>
#include <sstream>
#include <chrono>
#include <thread>

class Element;
class Plane;
class Gift;
class Bullet;
class Player;
class Enemy;


namespace setting
{
    constexpr int gameMapRows = 16;
    constexpr int gameMapCols = 48;

    constexpr int enemyLife = 5;
    constexpr int enemyShootInterval = 2;
    constexpr int enemySizeRows = 3;
    constexpr int enemySizeCols = 7;
    constexpr int enemySpeed = 1;

    constexpr int playerLife = 2;
    constexpr int playerShootInterval = 5;
    constexpr int playerSizeRows = 2;
    constexpr int playerSizeCols = 5;
    constexpr int playerSpeed = 1;
    
    constexpr int giftDuration = 30;
}

enum DirectionType
{
    DIR_UP = 0,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_ERROR,
};

enum BulletType
{
    BUL_FROM_ENEMY,
    BUL_FROM_PLAYER,
};

enum GameOverType
{
    PLAYER_WIN = 1,
    ENEMY_WIN,
};

const int global_directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
char global_game_map[setting::gameMapRows][setting::gameMapCols];
Player *global_player;
Enemy *global_enemy;
Gift *global_gift;
std::vector<Element *> global_elements;

/*** new codes for enhanced version ***/
class Element
{
public:
    Element() = default;

	virtual void draw() = 0;

	virtual void move() = 0;

    virtual bool get_validity()
    {
        return true;
    }
};
/**************************************/

class Plane : public Element
{
protected:
    int life; // left HP value
    // for drawing
    std::pair<unsigned int, unsigned int> location;
    std::pair<unsigned int, unsigned int> size;
    char symbol;
    // for moving
    int speed;
    int direction;

public:
    Plane(std::pair<unsigned int, unsigned int> init_location, char init_symbol, std::pair<unsigned int, unsigned int> init_size, int init_speed, int init_direction, int init_life)
    {
        life = init_life;
        location = init_location;
        size = init_size;
        symbol = init_symbol;
        speed = init_speed;
        direction = init_direction;
    }

    int get_life()
    {
        return life;
    }

    void set_life(int new_life)
    {
        if (new_life < 0)
            std::cout << "The life value can not be negative" << std::endl;
        else
            life = new_life;
    }

    char get_symbol()
    {
        return symbol;
    }

    void set_symbol(char new_symbol)
    {
        symbol = new_symbol;
    }

    int get_direction()
    {
        return direction;
    }

    void set_direction(int new_direction)
    {
        if (new_direction >= DIR_ERROR)
            std::cout << "The direction value can only be less than 4" << std::endl;
        else
            direction = new_direction;
    }

    void draw()
    {
        for (int i = 0; i < size.first; i++)
        {
            for (int j = 0; j < size.second; j++)
            {
                global_game_map[(location.first + i) % setting::gameMapRows][(location.second + j) % setting::gameMapCols] = symbol;
            }
        }
    }

    void move()
    {
        location.first = (location.first + (global_directions[direction][0] * speed + setting::gameMapRows)) % setting::gameMapRows;
        location.second = (location.second + (global_directions[direction][1] * speed + setting::gameMapCols)) % setting::gameMapCols;
    }

    bool is_collision(std::pair<int, int> bullet_location)
    {
        bool check_x = bullet_location.first >= location.first && bullet_location.first < (location.first + size.first);
        bool check_y = bullet_location.second >= location.second && bullet_location.second < (location.second + size.second);
        return check_x && check_y;
    }

    virtual void shoot(std::pair<int, int> bullet_location, int bullet_direction, int bullet_type);

    virtual void hit()
    {
        life -= 1;
    }

    virtual void speak() = 0;
    
    virtual void display_info() = 0;
};

/*** new codes for enhanced version ***/
class Gift : public Element
{
private:
    bool validity;
    // for drawing
    std::pair<unsigned int, unsigned int> location;
    char symbol;
public:
    Gift(std::pair<unsigned int, unsigned int> init_location) 
    {
        validity = true;
        location = init_location;
        symbol = 'G';
    }

    bool get_validity()
    {
        return validity;
    }

    void set_validity(bool new_validity)
    {
        validity = new_validity;
    }

    void draw()
    {
        if (!validity)
            return;
        global_game_map[location.first % setting::gameMapRows][location.second % setting::gameMapCols] = symbol;
    }

    void move();

    void respawn()
    {
        location = std::make_pair(rand() % setting::gameMapRows, rand() % setting::gameMapCols);
        set_validity(true);
    }
};
/**************************************/

class Bullet : public Element
{
private:
    bool validity;
    int bullet_type;
    bool bullet_on_edge;
    // for drawing
    std::pair<unsigned int, unsigned int> location;
    char symbol;
    // for moving
    int speed;
    int direction;

public:
    Bullet(std::pair<unsigned int, unsigned int> init_location, char init_symbol, int init_direction, int init_type)
    {
        assert(init_direction == DIR_UP || init_direction == DIR_DOWN);
        assert(init_type == BUL_FROM_ENEMY || init_type == BUL_FROM_PLAYER);
        validity = true;
        bullet_type = init_type;
        bullet_on_edge = false;
        location = init_location;
        symbol = init_symbol;
        speed = 1;
        direction = init_direction;
    }

    bool get_validity()
    {
        return validity;
    }

    void set_validity(bool new_validity)
    {
        validity = new_validity;
    }

    void draw()
    {
        if (!validity)
            return;
        global_game_map[location.first % setting::gameMapRows][location.second % setting::gameMapCols] = symbol;
    }

    void move();
    
    void check_on_edge()
    {
        if ((location.first <= speed) && bullet_type == BUL_FROM_PLAYER)
            bullet_on_edge = true;

        if ((location.first + speed >= setting::gameMapRows) && bullet_type == BUL_FROM_ENEMY)
            bullet_on_edge = true;
    }
};

class Player : public Plane
{
private:
    int shoot_interval;
    int gift_countdown;

public:
    Player(std::pair<unsigned int, unsigned int> init_location) : Plane(init_location, '^', std::make_pair(setting::playerSizeRows, setting::playerSizeCols), setting::playerSpeed, DIR_LEFT, setting::playerLife)
    {
        shoot_interval = 0;
        gift_countdown = 0;
    }

    void set_gift_countdown(int new_gift_countdown)
    {
        gift_countdown = new_gift_countdown;
    }

    int get_gift_countdown()
    {
        return gift_countdown;
    }

    void move()
    {
        Plane::move();
        if (shoot_interval++ >= setting::playerShootInterval)
        {
            shoot();
            shoot_interval = 0;
        }
        /*** new codes for enhanced version ***/
        if (gift_countdown > 0)
        {
            gift_countdown--;
            if (gift_countdown == 0)
                set_symbol('^');
        }
        /**************************************/
    }

    void shoot();

    void speak()
    {
        std::vector<std::string> dialogues = {"I'm ready for action!", "No one can defeat me!", "I'll save the world!"};
        std::cout << "Player:\t" << dialogues[rand() % dialogues.size()] << std::endl;
    }

    void display_info()
    {
        std::cout << "Congratulations! You have defeated them." << std::endl;
    }

    /*** new codes for enhanced version ***/
    void give_gift()
    {
        gift_countdown += setting::giftDuration;
        set_symbol('G');
    }
    /**************************************/

    /*** new codes for enhanced version ***/
    void hit()
    {
        if(gift_countdown <= 0)
            life -= 1;
    }
    /**************************************/
};

class Enemy : public Plane
{
private:
    int shoot_interval;

public:
    Enemy(std::pair<unsigned int, unsigned int> init_location) : Plane(init_location, '$', std::make_pair(setting::enemySizeRows, setting::enemySizeCols), setting::enemySpeed, DIR_LEFT, setting::enemyLife)
    {
        shoot_interval = 0;
    }

    void move()
    {
        Plane::move();
        if ((rand() % 100) < 10)
        {
            if (direction == DIR_LEFT)
                set_direction(DIR_RIGHT);
            else
                set_direction(DIR_LEFT);
        }
        if (shoot_interval++ >= setting::enemyShootInterval)
        {
            shoot();
            shoot_interval = 0;
        }
    }

    void shoot();

    void speak()
    {
        std::vector<std::string> dialogues = {"You won't escape!", "Prepare to be destroyed!", "I will crush you!"};
        std::cout << "Enemy:\t" << dialogues[rand() % dialogues.size()] << std::endl;
    }

    void display_info()
    {
        std::cout << "Sad! You made sacrifices in this fierce battle." << std::endl;
    }
};

/*** new codes for enhanced version ***/
void Gift::move()
{
    if (global_player->is_collision(location))
    {
        global_player->give_gift();
        set_validity(false);
    }
}
/**************************************/

void Bullet::move()
{
    if (bullet_on_edge)
    {
        set_validity(false);
        return;
    }

    location.first = (location.first + (global_directions[direction][0] * speed + setting::gameMapRows)) % setting::gameMapRows;
    location.second = (location.second + (global_directions[direction][1] * speed + setting::gameMapCols)) % setting::gameMapCols;

    if (bullet_type == BUL_FROM_ENEMY)
    {
        if (global_player->is_collision(location))
        {
            global_player->hit();
            set_validity(false);
        }
    }
    else
    {
        if (global_enemy->is_collision(location))
        {
            global_enemy->hit();
            set_validity(false);
        }
    }
    check_on_edge();
}

void Plane::shoot(std::pair<int, int> bullet_location, int bullet_direction, int bullet_type)
{
    Bullet *new_bullet = new Bullet(bullet_location, symbol, bullet_direction, bullet_type);
    global_elements.push_back(new_bullet);
}

void Player::shoot()
{
    std::pair<int, int> bullet_location = location;
    bullet_location.second += (size.second / 2);
    Plane::shoot(bullet_location, DIR_UP, BUL_FROM_PLAYER);
}

void Enemy::shoot()
{
    std::pair<int, int> bullet_location = location;
    bullet_location.first += size.first - 1;
    bullet_location.second += (size.second / 2);
    Plane::shoot(bullet_location, DIR_DOWN, BUL_FROM_ENEMY);
}

class Environment
{
public:
    Plane *winner;
    Plane *speaker;

public:
    Environment()
    {
        winner = nullptr;
        speaker = nullptr;
        global_enemy = new Enemy(std::make_pair(0, (setting::gameMapCols - setting::enemySizeCols) / 2));
        global_player = new Player(std::make_pair(setting::gameMapRows - setting::playerSizeRows, (setting::gameMapCols - setting::playerSizeCols) / 2));
        global_gift = new Gift(std::make_pair(10, 10));
        global_elements.push_back(global_enemy);
        global_elements.push_back(global_player);
        global_elements.push_back(global_gift);

        // counting down
        std::cout << "\n\n\t\tThe game is about to begin!" << std::endl;
        for (int i = 3; i >= 0; i--)
        {
            long start = clock();
            while (clock() - start <= 1000)
                ; // 1 s

            if (i > 0)
                std::cout << "\n\n\t\tCountdown:" << i << std::endl;
        }
    }

    /*** new codes for enhanced version ***/
    void move_one(Element *obj)
    {
        obj->move();
    }
    /**************************************/

    void move_all()
    {
        for (int i = 0; i < global_elements.size(); i++)
        {
            move_one(global_elements[i]);
        }
    }

    /*** new codes for enhanced version ***/
    void draw_one(Element *obj)
    {
        obj->draw();
    }
    /**************************************/

    void draw_all()
    {
        memset(global_game_map, 0, sizeof(global_game_map));
        for (int i = 0; i < global_elements.size(); i++)
        {
            draw_one(global_elements[i]);
        }
    }

    void check_state()
    {
        std::vector<int> to_be_delete;
        for (int i = 0; i < global_elements.size(); i++)
        {
            if (global_elements[i]->get_validity() == false)
            {
                to_be_delete.push_back(i);
            }
        }
        for (auto it = to_be_delete.rbegin(); it != to_be_delete.rend(); ++it)
        {
            global_elements.erase(global_elements.begin() + *it);
        }

        /*** new codes for enhanced version ***/
        if (global_gift->get_validity() == false)
        {
            global_gift->respawn();
            global_elements.push_back(global_gift);
        }
        /**************************************/

        if (global_enemy->get_life() <= 0)
            winner = global_player;
        if (global_player->get_life() <= 0)
            winner = global_enemy;
    }

    void display_all()
    {
        std::cout << std::endl;
        for (int j = 0; j < setting::gameMapCols + 2; j++)
        {
            std::cout << '-';
        }
        std::cout << std::endl;
        for (int i = 0; i < setting::gameMapRows; i++)
        {
            std::cout << '|';
            for (int j = 0; j < setting::gameMapCols; j++)
            {
                if (global_game_map[i][j] != 0)
                    std::cout << global_game_map[i][j];
                else
                    std::cout << " ";
            }
            std::cout << '|';
            std::cout << std::endl;
        }
        for (int j = 0; j < setting::gameMapCols + 2; j++)
        {
            std::cout << '-';
        }
        std::cout << std::endl;

        if ((rand() % 2) == 0) 
        {
            speaker = global_player;
        }
        else
        {
            speaker = global_enemy;
        }
        speaker->speak();

        for (int j = 0; j < setting::gameMapCols + 2; j++)
        {
            std::cout << '-';
        }
        std::cout << std::endl;
        std::cout << "ENEMY\tHP: "
                  << global_enemy->get_life() << std::endl;
        std::cout << "PLAYER\tHP: "
                  << global_player->get_life() << "\tGIFT COUNTDOWN: " << global_player->get_gift_countdown() << std::endl;
    }
    
    void display_result()
    {
        winner->display_info();
    }

    void get_input()
    {
        std::cout << "Please select the moving direction! (w : up, s : down, a: left, d: right)" << std::endl;
        char input;
        std::cin >> input; // get input from player
        if (input == 'w')
            global_player->set_direction(DIR_UP);
        else if (input == 's')
            global_player->set_direction(DIR_DOWN);
        else if (input == 'a')
            global_player->set_direction(DIR_LEFT);
        else if (input == 'd')
            global_player->set_direction(DIR_RIGHT);
    }
    
    void run()
    {
        check_state();
        draw_all();
        display_all();
        while (!winner)
        {
            get_input();
            move_all();
            draw_all();
            display_all();
            check_state();
        }
        display_result();
    }
};

int main()
{
    srand(555);
    Environment env;
    env.run();
}
