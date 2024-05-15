"""
* CSCI3180 Principles of Programming Languages
*
* --- Declaration ---
* For all submitted files, including the source code files and the written
* report, for this assignment:
*
* I declare that the assignment here submitted is original except for source
* materials explicitly acknowledged. I also acknowledge that I am aware of
* University policy and regulations on honesty in academic work, and of the
* disciplinary guidelines and procedures applicable to breaches of such policy
* and regulations, as contained in the website
* http://www.cuhk.edu.hk/policy/academichonesty/
*
*
* Source material acknowledgements (if any):
*
* Students whom I have discussed with (if any):
"""

import time
import random

# constants
GAME_MAP_ROWS = 16
GAME_MAP_COLS = 48

ENEMY_LIFE = 5
ENEMY_SHOOT_INTERVAL = 2
ENEMY_SIZE_ROWS = 3
ENEMY_SIZE_COLS = 7
ENEMY_SPEED = 1

PLAYER_LIFE = 2
PLAYER_SHOOT_INTERVAL = 5
PLAYER_SIZE_ROWS = 2
PLAYER_SIZE_COLS = 5
PLAYER_SPEED = 1

GIFT_DURATION = 30

# enums
class DirectionType:
    DIR_UP = 0
    DIR_DOWN = 1
    DIR_LEFT = 2
    DIR_RIGHT = 3
    DIR_ERROR = 4

class BulletType:
    BUL_FROM_ENEMY = 0
    BUL_FROM_PLAYER = 1

class GameOverType:
    PLAYER_WIN = 1
    ENEMY_WIN = 2
    
global_directions = [[-1, 0], [1, 0], [0, -1], [0, 1]]
global_game_map = [[' ' for _ in range(GAME_MAP_COLS)] for _ in range(GAME_MAP_ROWS)]
global_player = None
global_enemy = None
global_gift = None
global_elements = []

class Plane:
    def __init__(self, init_location, init_symbol, init_size, init_speed, init_direction, init_life):
        self._life = init_life
        self._location = init_location
        self._size = init_size
        self._symbol = init_symbol
        self._speed = init_speed
        self._direction = init_direction

    # TODO: add Property Decorator for attributes self._life, self._symbol, self._direction here, including setter.
    @property
    def life(self):
        return self._life
    
    @life.setter
    def life(self, new_life):
        if new_life < 0:
            print("The life value can not be negative")
        else:
            self._life = new_life
    
    @property
    def symbol(self):
        return self._symbol
    
    @symbol.setter
    def symbol(self, new_symbol):
        self._symbol = new_symbol
    
    @property
    def direction(self):
        return self._direction
    
    @direction.setter
    def direction(self, new_direction):
        if new_direction >= DirectionType.DIR_ERROR:
            print("The direction value can only be less than 4")
        else:
            self._direction = new_direction
    
    def draw(self):
        for i in range(self._size[0]):
            for j in range(self._size[1]):
                global_game_map[(self._location[0] + i) % GAME_MAP_ROWS][(self._location[1] + j) % GAME_MAP_COLS] = self._symbol

    def move(self):
        # TODO: Implement the logic of the next position the plane moves to here.
        new_location = ((self._location[0] + (global_directions[self.direction][0] * self._speed + GAME_MAP_ROWS)) % GAME_MAP_ROWS,
                        (self._location[1] + (global_directions[self.direction][1] * self._speed + GAME_MAP_COLS)) % GAME_MAP_COLS)
        self._location = new_location 

    def is_collision(self, bullet_location):
        # TODO: Implement the logic to determine whether the plane has been hit by a bullet here.
        check_x = bullet_location[0] >= self._location[0] and bullet_location[0] < (self._location[0] + self._size[0])
        check_y = bullet_location[1] >= self._location[1] and bullet_location[1] < (self._location[1] + self._size[1])
        return check_x and check_y

    def shoot(self, bullet_location, bullet_direction, bullet_type):
        new_bullet = Bullet(bullet_location, self._symbol, bullet_direction, bullet_type)
        global_elements.append(new_bullet)
    
    def hit(self):
        self.life -= 1

class Gift:
    def __init__(self, init_location):  
        self._validity = True
        self._location = init_location
        self._symbol = 'G'

    # TODO: add Property Decorator for attributes self._validity here, including setter.
    @property
    def validity(self):
        return self._validity
    
    @validity.setter
    def validity(self, new_validity):
        self._validity = new_validity
    
    def draw(self):
        if not self._validity:
            return
        global_game_map[self._location[0] % GAME_MAP_ROWS][self._location[1] % GAME_MAP_COLS] = self._symbol

    def move(self):
        # TODO: Implement the logic to determine whether the player has picked up the gift.
        if global_player.is_collision(self._location):
            global_player.give_gift()
            self.validity = False
    
    def respawn(self):
        # TODO: Implement the logic to respawn the gift.
        self._location = (random.randint(0, GAME_MAP_ROWS-1), random.randint(0, GAME_MAP_COLS-1))
        self.validity = True

class Bullet:
    def __init__(self, init_location, init_symbol, init_direction, init_type):
        assert init_direction == DirectionType.DIR_UP or init_direction == DirectionType.DIR_DOWN
        assert init_type == BulletType.BUL_FROM_ENEMY or init_type == BulletType.BUL_FROM_PLAYER
        self._validity = True
        self._bullet_type = init_type
        self._bullet_on_edge = False
        self._location = init_location
        self._symbol = init_symbol
        self._speed = 1
        self._direction = init_direction

    # TODO: add Property Decorator for attributes self._validity here, including setter.
    @property
    def validity(self):
        return self._validity
    
    @validity.setter
    def validity(self, new_validity):
        self._validity = new_validity
    
    def draw(self):
        if not self._validity:
            return
        global_game_map[self._location[0] % GAME_MAP_ROWS][self._location[1] % GAME_MAP_COLS] = self._symbol

    def move(self):
        if self._bullet_on_edge:
            self.validity = False
            return

        # TODO: Implement the logic of the next position the bullet moves to here.
        new_location = ((self._location[0] + (global_directions[self._direction][0] * self._speed + GAME_MAP_ROWS)) % GAME_MAP_ROWS,
                        (self._location[1] + (global_directions[self._direction][1] * self._speed + GAME_MAP_COLS)) % GAME_MAP_COLS)
        self._location = new_location
        
        # TODO: Implement the logic to determine whether the bullet hits the plane.
        if self._bullet_type == BulletType.BUL_FROM_ENEMY:
            if global_player.is_collision(self._location):
                global_player.hit()
                self._validity = False
        else:
            if global_enemy.is_collision(self._location):
                global_enemy.hit()
                self._validity = False
        
        self.check_on_edge()
    
    def check_on_edge(self):
        # TODO: Implement the logic to determine whether the bullet reaches the map boundary.
        if (self._location[0] <= self._speed) and (self._bullet_type == BulletType.BUL_FROM_PLAYER):
            self._bullet_on_edge = True
        
        if (self._location[0] + self._speed >= GAME_MAP_ROWS) and (self._bullet_type == BulletType.BUL_FROM_ENEMY):
            self._bullet_on_edge = True

class Player(Plane):
    def __init__(self, init_location):
        super().__init__(init_location, '^', (PLAYER_SIZE_ROWS, PLAYER_SIZE_COLS), PLAYER_SPEED, DirectionType.DIR_LEFT, PLAYER_LIFE)
        self._shoot_interval = 0
        self._gift_countdown = 0

    # TODO: add Property Decorator for attributes self._gift_countdown here, including setter.
    @property
    def gift_countdown(self):
        return self._gift_countdown
    
    @gift_countdown.setter
    def gift_countdown(self, new_gift_countdown):
        self._gift_countdown = new_gift_countdown
    
    def move(self):
        super().move()
        # TODO: Implement the logic of the player automatically firing a bullet after PLAYER_SHOOT_INTERVAL frames.
        if self._shoot_interval >= PLAYER_SHOOT_INTERVAL:
            self.shoot()
            self._shoot_interval = 0
        else:
            self._shoot_interval += 1

        # TODO: Implement the logic of the gift effect countdown.
        if self.gift_countdown > 0:
            self.gift_countdown -= 1
            if self.gift_countdown == 0:
                self._symbol = '^'

    def shoot(self):
        bullet_location = (self._location[0], self._location[1] + (self._size[1] // 2))
        super().shoot(bullet_location, DirectionType.DIR_UP, BulletType.BUL_FROM_PLAYER)
    
    def speak(self):
        dialogues = ["I'm ready for action!", "No one can defeat me!", "I'll save the world!"]
        print("Player:\t", random.choice(dialogues))
        
    def display_info(self):
        print("Congratulations! You have defeated them.")
        
    def give_gift(self):
        # TODO: Implement the logic of the effect of the player picking up a gift.
        self.gift_countdown += GIFT_DURATION
        self._symbol = 'G'

    def hit(self):
        # TODO: Implement the logic of the player gets hit (considering the effect of gift).
        if self.gift_countdown <= 0:
            self._life -= 1

class Enemy(Plane):
    def __init__(self, init_location):
        super().__init__(init_location, '$', (ENEMY_SIZE_ROWS, ENEMY_SIZE_COLS), ENEMY_SPEED, DirectionType.DIR_LEFT, ENEMY_LIFE)
        self._shoot_interval = 0

    def move(self):
        super().move()
        # TODO: Implement the logic that enemy has a 10% chance of changing direction in each frame.
        if random.random() * 100 < 10:
            if self.direction == DirectionType.DIR_LEFT:
                self.direction = DirectionType.DIR_RIGHT
            else:
                self.direction = DirectionType.DIR_LEFT
        
        # TODO: Implement the logic of the player automatically firing a bullet after ENEMY_SHOOT_INTERVAL frames.
        if self._shoot_interval >= ENEMY_SHOOT_INTERVAL:
            self.shoot()
            self._shoot_interval = 0
        else:
            self._shoot_interval += 1

    def shoot(self):
        bullet_location = (self._location[0] + self._size[0] - 1, self._location[1] + (self._size[1] // 2))        
        super().shoot(bullet_location, DirectionType.DIR_DOWN, BulletType.BUL_FROM_ENEMY)
    
    def speak(self):
        dialogues = ["You won't escape!", "Prepare to be destroyed!", "I will crush you!"]
        print("Enemy:\t", random.choice(dialogues))
        
    def display_info(self):
        print("Sad! You made sacrifices in this fierce battle.")
        
class Environment:
    def __init__(self):
        self._winner = None
        self._speaker = None
        
        global global_enemy, global_player, global_gift
        global_enemy = Enemy((0, (GAME_MAP_COLS - ENEMY_SIZE_COLS) // 2))
        global_player = Player((GAME_MAP_ROWS - PLAYER_SIZE_ROWS, (GAME_MAP_COLS - PLAYER_SIZE_COLS) // 2))
        global_gift = Gift((10, 10))
        global_elements.append(global_enemy)
        global_elements.append(global_player)
        global_elements.append(global_gift)

        # counting down
        print("\n\n\t\tThe game is about to begin!")
        for i in range(3, 0, -1):
            start = time.time()
            while (time.time() - start) <= 1:
                pass  # 1 s

            if i > 0:
                print("\n\n\t\tCountdown:", i)
    
    def move_one(self, obj):
        obj.move()
    
    def move_all(self):
        for element in global_elements:
            self.move_one(element)

    def draw_one(self, obj):
        obj.draw()
    
    def draw_all(self):
        global global_game_map
        global_game_map = [[0] * GAME_MAP_COLS for _ in range(GAME_MAP_ROWS)]
        for element in global_elements:
            self.draw_one(element)

    def check_state(self):
        to_be_delete = []
        for i in range(len(global_elements)):
            if hasattr(global_elements[i], "validity") and not global_elements[i].validity:
                to_be_delete.append(i)
        for idx in reversed(to_be_delete):
            del global_elements[idx]
        
        # TODO: Implement the logic of respawning if the gift does not exist.
        if global_gift.validity is False:
            global_gift.respawn()
            global_elements.append(global_gift)
        
        # TODO: Implement the logic to determine whether the game should end.
        if global_enemy.life <= 0:
            self._winner = global_player
        if global_player.life <= 0:
            self._winner = global_enemy

    def display_all(self):
        print()
        for _ in range(GAME_MAP_COLS + 2):
            print('-', end='')
        print()
        for i in range(GAME_MAP_ROWS):
            print('|', end='')
            for j in range(GAME_MAP_COLS):
                if global_game_map[i][j] != 0:
                    print(global_game_map[i][j], end='')
                else:
                    print(" ", end='')
            print('|')
        for _ in range(GAME_MAP_COLS + 2):
            print('-', end='')
        print()
        
        # TODO: Implement speaking logic here.
        if random.randint(0, 1) == 0:
            self._speaker = global_player
        else:
            self._speaker = global_enemy
        self._speaker.speak() 
        
        for _ in range(GAME_MAP_COLS + 2):
            print('-', end='')
        print()

        print("ENEMY\tHP: ", global_enemy.life)
        print("PLAYER\tHP: ", global_player.life, "\tGIFT COUNTDOWN: ", global_player.gift_countdown)
    
    def display_result(self):
        self._winner.display_info()
        
    def get_input(self):
        print("Please select the moving direction! (w : up, s : down, a: left, d: right)")
        player_input = input()  # get input from player
        if player_input == 'w':
            global_player.direction = DirectionType.DIR_UP
        elif player_input == 's':
            global_player.direction = DirectionType.DIR_DOWN
        elif player_input == 'a':
            global_player.direction = DirectionType.DIR_LEFT
        elif player_input == 'd':
            global_player.direction = DirectionType.DIR_RIGHT

    def run(self):
        self.check_state()
        self.draw_all()
        self.display_all()
        while not self._winner:
            self.get_input()
            self.move_all()
            self.draw_all()
            self.display_all()
            self.check_state()
        self.display_result()
        
if __name__ == "__main__":
    random.seed(555)
    env = Environment()
    env.run()
    