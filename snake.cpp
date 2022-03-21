#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <windows.h>

using namespace std;

constexpr auto FIELD_CELL_TYPE_NONE = 0;
constexpr auto FIELD_CELL_TYPE_APPLE = -1;
constexpr auto FIELD_CELL_TYPE_WALL = -2;
constexpr auto FIELD_CELL_TYPE_GREEN_APPLE = -3;
constexpr auto FIELD_CELL_TYPE_HEART = -4;
constexpr auto FIELD_CELL_TYPE_YELLOW_APPLE = -5;
constexpr auto SNAKE_DIRECTION_UP = 0;
constexpr auto SNAKE_DIRECTION_RIGHT = 1;
constexpr auto SNAKE_DIRECTION_DOWN = 2;
constexpr auto SNAKE_DIRECTION_LEFT = 3;

const int field_size_x = 35;
const int field_size_y = 25;
const int cell_size = 32;
const int window_width = field_size_x * cell_size;
const int window_height = field_size_y * cell_size;
int type_of_control;

vector<int> snake_direction_queue;

int score = 0;
bool game_over = false;
bool game_paused = false;
int count_of_apples = 0;
const int n = 10;
bool event_green = false;
int speed; 
int level;
int speed_last;
bool invert_control = false;
bool length_increase = false;
bool score_decrease = false;
int count_of_red_apples = 0;
int x, y, z, r, g, b;
int count_of_lives = 0;

struct GameState {
    int field[field_size_y][field_size_x];
    int snake_position_x = field_size_x / 2;
    int snake_position_y = field_size_y / 2;
    int snake_length = 4;
    int snake_direction = SNAKE_DIRECTION_RIGHT;
};

GameState game_state;
vector <GameState> game_last_states;
bool rall_back = false;

int difficulty_level()
{
    int snake_speed, level;
    cout << "Вашему вниманию представлена игра SNAKE" << endl << endl;
    cout << "Для управления можно использвать:\n\t1 - курсоры\n\t2 - W, S, A, D" << endl;
    cout << endl << "Выберите тип управления: "; cin >> type_of_control; cout << endl;
    cout << "Чтобы поставить игру на паузу - нажмите пробел, для продолжения используйте клавиши управления или enter" << endl;
    cout << "Будьте внимательны! В игре есть 3 типа яблок. Зеленые яблоки появляются после съеденных 10 красных, желтые после 15." << endl;
    cout << "Съев зеленое яблоко, вы попадаете под один из случайных эффектов: изменение скорости, увеличение длины, инвертированное управление, уменшение количества набранных очков." << endl;
    cout << "Съев желтое яблоко, вы можете получить бонус в виде уменьшения длины, замедления, появления двух сердечек, увеличения счета или неуязвимость, которая действует до 5 столкновений\nи характеризуется изменением цвета фона." << endl;
    cout << "Сердечки нужны для снятия эффектов зеленого яблока. Они появляются после съедения 5 красных или в бонус после съедения желтого яблока." << endl;
    cout << endl << "Приятной игры!\n\nДля продолжения нажмите любую клавишу.";  system("pause > nul");
    cout << endl << endl << "Загрузка: " << endl;
    srand(time(NULL));
    int i, k; double time_l;
    for (i = 0; i < 10; i++)
    {
        cout << i * 10 << "%" << "_";
        time_l = 0 + rand() % 2;
        Sleep(time_l * 1000);
    }
    cout << "100%" << endl;
    cout << endl << "Загрузка завершена." << endl;
    cout << endl << "Доступны уровни сложности: " << endl;
    cout << "\t0 - standart" << endl;
    cout << "\t1 - easy" << endl;
    cout << "\t2 - medium" << endl;
    cout << "\t3 - hard" << endl;
    cout << "\t4 - crazy" << endl;
    cout << "\t5 - impossible" << endl;
    cout << endl << "Выберите уровень сложности: "; cin >> level; cout << endl;

    switch (level) {
    case 1:
        snake_speed = 100;
        break;
    case 2:
        snake_speed = 80;
        break;
    case 3:
        snake_speed = 60;
        break;
    case 4:
        snake_speed = 40;
        break;
    case 5:
        snake_speed = 20;
        break;
    default:
        snake_speed = 120;
    }

    return snake_speed;
}

void window_color()
{
    int color;
    cout << "Доступные стили поля: " << endl;
    cout << "\t1 - standart\n\t2 - green\n\t3- mint" << endl;
    cout << endl << "Выберите стиль: "; cin >> color;
    cin.clear(); cin.ignore();
    cout << endl;
    switch (color) {
    case 2:
        x = 152; y = 251; z = 152;
        r = x; g = y; b = z;
        break;
    case 3:
        x = 0; y = 255; z = 153;
        r = x; g = y; b = z;
        break;
    default:
        x = 127; y = 255; z = 212;
        r = x; g = y; b = z;
        break;
    }
}

int get_random_empty_cell()
{
    int empty_cell_count = 0;
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            if (game_state.field[j][i] == FIELD_CELL_TYPE_NONE) {
                empty_cell_count++;
            }
        }
    }

    int target_empty_cell_index = rand() % empty_cell_count;
    int empty_cell_index = 0;

    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            if (game_state.field[j][i] == FIELD_CELL_TYPE_NONE) {
                if (empty_cell_index == target_empty_cell_index) {
                    return j * field_size_x + i;
                }
                empty_cell_index++;
            }
        }
    }
    return -1;
}

void add_apple()
{
    int apple_pos = get_random_empty_cell();
    if (apple_pos != -1) {
        if (count_of_apples != n){
            game_state.field[apple_pos / field_size_x][apple_pos % field_size_x] = FIELD_CELL_TYPE_APPLE;
        }
        else {
            game_state.field[apple_pos / field_size_x][apple_pos % field_size_x] = FIELD_CELL_TYPE_GREEN_APPLE;
        }
    }
    
}

void add_heart()
{
    int heart_pos = get_random_empty_cell();
    if (heart_pos != -1) {
        game_state.field[heart_pos / field_size_x][heart_pos % field_size_x] = FIELD_CELL_TYPE_HEART;
    }
}

void add_yellow_apple()
{
    int yellow_apple_pos = get_random_empty_cell();
    if (yellow_apple_pos != -1) {
        game_state.field[yellow_apple_pos / field_size_x][yellow_apple_pos % field_size_x] = FIELD_CELL_TYPE_YELLOW_APPLE;
    }
}

void add_green_apple()
{
    int green_apple_pos = get_random_empty_cell();
    if (green_apple_pos != -1) {
        game_state.field[green_apple_pos / field_size_x][green_apple_pos % field_size_x] = FIELD_CELL_TYPE_GREEN_APPLE;
    }
}

void clear_field()
{
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            game_state.field[j][i] = FIELD_CELL_TYPE_NONE;
        }
    }
    for (int i = 0; i < game_state.snake_length; i++)
        game_state.field[game_state.snake_position_y][game_state.snake_position_x - i] = game_state.snake_length - i;
    
    for (int i = 0; i < field_size_x; i++) {
        if (i < 10 || field_size_x - i - 1 < 10) {
            game_state.field[0][i] = FIELD_CELL_TYPE_WALL;
            game_state.field[field_size_y - 1][i] = FIELD_CELL_TYPE_WALL;
        }
    }

    for (int j = 1; j < field_size_y - 1; j++) {
        if (j < 8 || field_size_y - j - 1 < 8) {
            game_state.field[j][0] = FIELD_CELL_TYPE_WALL;
            game_state.field[j][field_size_x - 1] = FIELD_CELL_TYPE_WALL;
        }
    }
    add_green_apple();
    add_apple();
}

void draw_field(sf::RenderWindow &window)
{
    sf::Texture none_texture;
    none_texture.loadFromFile("images/none.png");
    sf::Sprite none;
    none.setTexture(none_texture);

    sf::Texture snake_texture;
    snake_texture.loadFromFile("images/snake.png");
    sf::Sprite snake;
    snake.setTexture(snake_texture);

    sf::Texture snake_head_texture;
    snake_head_texture.loadFromFile("images/head.png");
    sf::Sprite snake_head;
    snake_head.setTexture(snake_head_texture);

    sf::Texture apple_texture;
    apple_texture.loadFromFile("images/apple.png");
    sf::Sprite apple;
    apple.setTexture(apple_texture);

    sf::Texture apple_green_texture;
    apple_green_texture.loadFromFile("images/apple_green.png");
    sf::Sprite apple_green;
    apple_green.setTexture(apple_green_texture);

    sf::Texture apple_yellow_texture;
    apple_yellow_texture.loadFromFile("images/yelow_apple.png");
    sf::Sprite apple_yellow;
    apple_yellow.setTexture(apple_yellow_texture);

    sf::Texture wall_texture;
    wall_texture.loadFromFile("images/wall.png");
    sf::Sprite wall;
    wall.setTexture(wall_texture);

    sf::Texture heart_texture;
    heart_texture.loadFromFile("images/life.png");
    sf::Sprite heart;
    heart.setTexture(heart_texture);

    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            switch (game_state.field[j][i]) {
            case FIELD_CELL_TYPE_NONE:
                none.setPosition(float(i * cell_size), float(j * cell_size));
                window.draw(none);
                break;
            case FIELD_CELL_TYPE_APPLE:
                apple.setPosition(float(i * cell_size), float(j * cell_size));
                window.draw(apple);
                break;
            case FIELD_CELL_TYPE_GREEN_APPLE:
                apple_green.setPosition(float(i * cell_size), float(j * cell_size));
                window.draw(apple_green);
                break;
            case FIELD_CELL_TYPE_YELLOW_APPLE:
                apple_yellow.setPosition(float(i * cell_size), float(j * cell_size));
                window.draw(apple_yellow);
                break;
            case FIELD_CELL_TYPE_WALL:
                wall.setPosition(float(i * cell_size), float(j * cell_size));
                window.draw(wall);
                break;
            case FIELD_CELL_TYPE_HEART:
                heart.setPosition(float(i * cell_size), float(j * cell_size));
                window.draw(heart);
                break;
            default:
                if (game_state.field[j][i] == game_state.snake_length) {
                    float offset_x = snake_head.getLocalBounds().width / 2;
                    float offset_y = snake_head.getLocalBounds().height / 2;
                    snake_head.setPosition(float(i * cell_size + offset_x), float(j * cell_size + offset_y));
                    snake_head.setOrigin(offset_x, offset_y);
                    switch (game_state.snake_direction) {
                    case SNAKE_DIRECTION_RIGHT:
                        snake_head.setRotation(90);
                        break;
                    case SNAKE_DIRECTION_LEFT:
                        snake_head.setRotation(-90);
                        break;
                    case SNAKE_DIRECTION_DOWN:
                        snake_head.setRotation(180);
                        break;
                    case SNAKE_DIRECTION_UP:
                        snake_head.setRotation(0);
                        break;
                    }

                    window.draw(snake_head);
                }
                else {
                    snake.setPosition(float(i * cell_size), float(j * cell_size));
                    window.draw(snake);
                }
            }
        }
    }
}

void grow_snake()
{
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            if (game_state.field[j][i] > FIELD_CELL_TYPE_NONE) {
                game_state.field[j][i]++;
            }
        }
    }
}

void random_event()
{
    srand(time(NULL));
    int random_trap;
    random_trap = rand() % 4;
    switch (random_trap) {
    case 0:
        invert_control = true;
        break;
    case 1:
        speed = 50;
        break;
    case 2:
        if (score >= 10) {
            score -= 10;
        }
        else {
            score = 0;
        }
        score_decrease = true;
        break;
    case 3:
        game_state.snake_length += 3;
        length_increase = true;
        break;
    }
}

int random_bonus()
{
    srand(time(NULL));
    int bonus = rand() % 5;
    switch (bonus) {
    case 0:
        score += 15;
        break;
    case 1:
        return 1;
        break;
    case 2:
        speed = 130;
        break;
    case 3:
        if (game_state.snake_length >= 9) {
            game_state.snake_length -= 5;
        }
        break;
    case 4:
        count_of_lives = 5;
        x = 0; y = 220; z = 255;
        break;
    }
}

void normal_game()
{
    speed = speed_last;
    if (count_of_lives == 0) {
        x = r; y = g; z = b;
    }
    invert_control = false;
    if (length_increase) {
        length_increase = false;
        game_state.snake_length -= 3;
    }
    if (score_decrease) {
        score_decrease = false;
        score += 10;
    }
}

void make_move()
{
    game_last_states.push_back(game_state);
    if (game_last_states.size() > 10) {
        game_last_states.erase(game_last_states.begin());
    }
    switch (game_state.snake_direction) {
    case SNAKE_DIRECTION_UP:
        game_state.snake_position_y--;
        if (game_state.snake_position_y < 0) {
            game_state.snake_position_y = field_size_y - 1;
        }
        break;
    case SNAKE_DIRECTION_RIGHT:
        game_state.snake_position_x++;
        if (game_state.snake_position_x > field_size_x - 1) {
            game_state.snake_position_x = 0;
        }
        break;
    case SNAKE_DIRECTION_DOWN:
        game_state.snake_position_y++;
        if (game_state.snake_position_y > field_size_y - 1) {
            game_state.snake_position_y = 0;
        }
        break;
    case SNAKE_DIRECTION_LEFT:
        game_state.snake_position_x--;
        if (game_state.snake_position_x < 0) {
            game_state.snake_position_x = field_size_x - 1;
        }
        break;
    }

    if (game_state.field[game_state.snake_position_y][game_state.snake_position_x] != FIELD_CELL_TYPE_NONE) {
        switch (game_state.field[game_state.snake_position_y][game_state.snake_position_x]) {
        case FIELD_CELL_TYPE_APPLE:
            score++;
            game_state.snake_length++;
            count_of_apples++;
            if (count_of_apples == n) {
                add_green_apple();
                count_of_apples = 0;
            }
            count_of_red_apples++;
            if (count_of_red_apples == 5) {
                add_heart();
            }
            if (score != 0 && score % 15 == 0) {
                add_yellow_apple();
            }
            grow_snake();
            add_apple();
            break;
        case FIELD_CELL_TYPE_GREEN_APPLE:
            count_of_red_apples = 0;
            count_of_apples = 0;
            random_event();
            if (count_of_lives == 0) {
                x = 50; y = 185; z = 50;
            }
            break;
        case FIELD_CELL_TYPE_YELLOW_APPLE:
            if (random_bonus() == 1) {
                for (int m = 0; m < 2; m++) {
                    add_heart();
                }
            }
            break;
        case FIELD_CELL_TYPE_HEART:
            normal_game();
            break;
        case FIELD_CELL_TYPE_WALL:
            if (count_of_lives != 0) {
                rall_back = true;
                count_of_lives--;
                switch (count_of_lives) {
                case 4:
                    x = 255; y = 20; z = 147;
                    break;
                case 3:
                    x = 255; y = 140; z = 0;
                    break;
                case 2:
                    x = 139; y = 0; z = 139;
                    break;
                case 1:
                    x = 255; y = 255; z = 0;
                    break;
                default:
                    x = r; y = g; z = b;
                }
            }
            else {
                game_over = true;
            }
            break;
        default:
            if (game_state.field[game_state.snake_position_y][game_state.snake_position_x] > 1) {
                if (count_of_lives != 0) {
                    rall_back = true;
                    count_of_lives--;
                    switch (count_of_lives) {
                    case 5:
                        x = 0; y = 220; z = 255;
                        break;
                    case 4:
                        x = 255; y = 20; z = 147;
                        break;
                    case 3:
                        x = 255; y = 140; z = 0;
                        break;
                    case 2:
                        x = 139; y = 0; z = 139;
                        break;
                    case 1:
                        x = 255; y = 255; z = 0;
                        break;
                    default:
                        x = r; y = g; z = b;
                    }
                }
                else {
                    game_over = true;
                }
            }
        }
    }

    if (!rall_back) {
        for (int j = 0; j < field_size_y; j++) {
            for (int i = 0; i < field_size_x; i++) {
                if (game_state.field[j][i] > FIELD_CELL_TYPE_NONE) {
                    game_state.field[j][i]--;
                }

            }
        }

        game_state.field[game_state.snake_position_y][game_state.snake_position_x] = game_state.snake_length;
    }
}

void start_game()
{
    game_state.snake_position_x = field_size_x / 2;
    game_state.snake_position_y = field_size_y / 2;
    game_state.snake_length = 4;
    game_state.snake_direction = SNAKE_DIRECTION_RIGHT;
    score = 0;
    window_color();
    clear_field();
}

void game_control(bool &invert_control,  sf::RenderWindow& window)
{
    sf::Event event;
    if (invert_control == false) {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                int snake_direction_last = snake_direction_queue.empty() ? game_state.snake_direction : snake_direction_queue.at(0);
                switch (event.key.code) {
                case sf::Keyboard::Up:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_UP && snake_direction_last != SNAKE_DIRECTION_DOWN && type_of_control != 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_UP);
                        }
                    }
                    break;
                case sf::Keyboard::Right:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_RIGHT && snake_direction_last !=  SNAKE_DIRECTION_LEFT && type_of_control != 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_RIGHT);
                        }
                    }
                    break;
                case sf::Keyboard::Down:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_DOWN && snake_direction_last !=  SNAKE_DIRECTION_UP && type_of_control != 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_DOWN);
                        }
                    }
                    break;
                case sf::Keyboard::Left:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_LEFT && snake_direction_last !=  SNAKE_DIRECTION_RIGHT && type_of_control != 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_LEFT);
                        }
                    }
                    break;
                case sf::Keyboard::W:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_UP && snake_direction_last != SNAKE_DIRECTION_DOWN && type_of_control == 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_UP);
                        }
                    }
                    break;
                case sf::Keyboard::D:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_RIGHT && snake_direction_last != SNAKE_DIRECTION_LEFT && type_of_control == 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_RIGHT);
                        }
                    }
                    break;
                case sf::Keyboard::S:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_DOWN && snake_direction_last != SNAKE_DIRECTION_UP && type_of_control == 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_DOWN);
                        }
                    }
                    break;
                case sf::Keyboard::A:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_LEFT && snake_direction_last != SNAKE_DIRECTION_RIGHT && type_of_control == 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_LEFT);
                        }
                    }
                    break;
                case sf::Keyboard::Escape:
                    game_over = true;
                    window.close();
                    cout << "You are stopped the game!" << endl;
                    cout << "Your score: " << score << endl << endl;
                    game_over = false;
                    break;
                case sf::Keyboard::Space:
                    game_paused = true;
                    break;
                case sf::Keyboard::Enter:
                    game_paused = false;
                    break;

                }
            }
        }
    }
    else {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                int snake_direction_last = snake_direction_queue.empty() ? game_state.snake_direction : snake_direction_queue.at(0);
                switch (event.key.code) {
                case sf::Keyboard::Down:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_UP &&  snake_direction_last !=  SNAKE_DIRECTION_DOWN && type_of_control != 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_UP);
                        }
                    }
                    break;
                case sf::Keyboard::Up:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_DOWN && snake_direction_last !=  SNAKE_DIRECTION_UP && type_of_control != 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_DOWN);
                        }
                    }
                    break;
                case sf::Keyboard::Left:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_RIGHT && snake_direction_last !=  SNAKE_DIRECTION_LEFT && type_of_control != 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_RIGHT);
                        }
                    }
                    break;
                case sf::Keyboard::Right:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_LEFT && snake_direction_last !=  SNAKE_DIRECTION_RIGHT && type_of_control != 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_LEFT);
                        }
                    }
                    break;
                case sf::Keyboard::W:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_UP && snake_direction_last != SNAKE_DIRECTION_DOWN && type_of_control == 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_UP);
                        }
                    }
                    break;
                case sf::Keyboard::D:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_RIGHT && snake_direction_last != SNAKE_DIRECTION_LEFT && type_of_control == 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_RIGHT);
                        }
                    }
                    break;
                case sf::Keyboard::S:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_DOWN && snake_direction_last != SNAKE_DIRECTION_UP && type_of_control == 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_DOWN);
                        }
                    }
                    break;
                case sf::Keyboard::A:
                    game_paused = false;
                    if (snake_direction_last != SNAKE_DIRECTION_LEFT && snake_direction_last != SNAKE_DIRECTION_RIGHT && type_of_control == 2) {
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_LEFT);
                        }
                    }
                    break;
                case sf::Keyboard::Escape:
                    game_over = true;
                    window.close();
                    cout << "You are stopped the game!" << endl;
                    cout << "Your score: " << score << endl << endl;
                    game_over = false;
                    break;
                case sf::Keyboard::Space:
                    game_paused = true;
                    break;
                case sf::Keyboard::Enter:
                    game_paused = false;
                    break;
                }
            }
        }
    }
}

int main(void)
{
    system("color B0");
    setlocale(0, "");

    srand(time(NULL));

    speed = difficulty_level();
    speed_last = speed;

    start_game();

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "snake", sf::Style::Close);

    while (window.isOpen()){
        game_control(invert_control, window);

        if (!snake_direction_queue.empty()) {
            game_state.snake_direction = snake_direction_queue.back();
            snake_direction_queue.pop_back();
        }

        if (!game_paused) {
            if (!rall_back) {
                make_move();
            }
            else {
                if (!game_last_states.empty()) {
                    game_state = game_last_states.back();
                    game_last_states.pop_back();
                }
                else {
                    rall_back = false;
                }
            }
        }

        if (game_over) {
            if (!rall_back) {
                sf::sleep(sf::seconds(1));
                window.close();
                cout << "It's GAMEOVER!" << endl;
                cout << endl << "Your score: " << score << endl << endl;
            }
        }


        window.clear(sf::Color(x, y, z));
        draw_field(window);

        window.display();

        sf::sleep(sf::milliseconds(speed));
        }
    
    system("pause");

    return 0;
}


