#include <SFML/Graphics.hpp> //графическая библиотека
#include <cstdlib> //стандартная библиотека
#include <vector> //для использования std::vector
#include <iostream> //для запроса от пользователя в консоли настроек игры
#include <windows.h> 
#include <string>

HANDLE hConsole;

using namespace std; //стандартное пространство имен

//Блок константных выражений для стандартных элементов на поле

const int skin_cell_size = 32;
const int skin_length = 2;
const int skin_count = 5; // количество скинов
const int skin_choice_height = (skin_length + 4) * skin_cell_size;
const int skin_choice_width = (skin_count + 6) * skin_cell_size;
int choice = 1;
int skin;

const int wall_cell_size = 32;
const int wall_length = 2;
const int wall_count = 3; // количество скинов
const int wall_choice_height = (wall_length + 3) * wall_cell_size;
const int wall_choice_width = (wall_count + 4) * wall_cell_size;
int choice_wall = 1;
int wall;

const int field_color_cell_size = 32;
const int window_color_height = 256;
const int window_color_width = 256;
bool pause = false;
int color = 1;

constexpr auto FIELD_CELL_TYPE_NONE = 0; //пустая клетка
constexpr auto FIELD_CELL_TYPE_APPLE = -1; //яблоко
constexpr auto FIELD_CELL_TYPE_WALL = -2; //стена
constexpr auto FIELD_CELL_TYPE_GREEN_APPLE = -3; //зеленое яблоко
constexpr auto FIELD_CELL_TYPE_HEART = -4; //сердечко
constexpr auto FIELD_CELL_TYPE_YELLOW_APPLE = -5; //желтое яблоко

//направления движения змейки

constexpr auto SNAKE_DIRECTION_UP = 0; //вверх
constexpr auto SNAKE_DIRECTION_RIGHT = 1; //вправо
constexpr auto SNAKE_DIRECTION_DOWN = 2; //вниз
constexpr auto SNAKE_DIRECTION_LEFT = 3; //влево

//размеры поля

const int field_size_x = 40; //количество клеток по длине
const int field_size_y = 25; //количество клеток по высоте
const int cell_size = 32; //размер клетки
const int window_width = field_size_x * cell_size; //длина поля
const int window_height = field_size_y * cell_size; //высота поля

int type_of_control; //вид управления

vector <int> snake_direction_queue; //массив для буфферизации управления, чтобы можно было лучше координировать змейку и делать более удобные маневры
int last_score = 0;
int score = 0; //переменная для подсчета количества очков
bool game_over = false; //переменнная, отвечающая за конец игры
bool game_paused = false; //переменная, отвечающая за паузу
int count_of_apples = 0; //количество яблок, необходимое для генерации других предметов
const int n = 10; //константа для генерации зеленого яблока
bool event_green = false; //случай съедения зеленого яблока
int speed; //скорость змейки
int level; //уровень сложности
int speed_last; //скорость по умолчанию
bool invert_control = false; //инверсия управления
bool length_increase = false; //увеличение длины
bool score_decrease = false; //уменьшение длины
int count_of_red_apples = 0; //количество красных яблок. Для генерации некоторы предметов
int x, y, z, r, g, b; //цвет поля
int count_of_lives = 0; //количество жизней при неуязвимости
bool exit_game; // переменная, отвечающая за выход
bool immortality = false; // переменная, отвечающая за бессмертие
bool win_game = false;
string code; // вводимый код
string immortality_code = "2603_Alekseev_I_I_2219"; //код бессмертия

//структура с характеристиками змейкм

struct GameState {
    int field[field_size_y][field_size_x]; //матрица, задающая поле
    int snake_position_x = field_size_x / 2; //позиция змейки по x
    int snake_position_y = field_size_y / 2; //позицция змейки по y
    int snake_length = 4; //длиня змейки, равная изначально 4
    int snake_direction = SNAKE_DIRECTION_RIGHT; //направление движения, изначально вправо
};

GameState game_state; //текущая стадия игры
vector <GameState> game_last_states; //массив с сохранением стадии для отката назад
bool rall_back = false; //откат назад

void draw_window_color(sf::RenderWindow& window_2)
{
    sf::Texture none_texture;
    sf::Sprite none;

    none_texture.loadFromFile("images/none.png");
    none.setTexture(none_texture);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            none.setPosition(float(j * field_color_cell_size), float(i * field_color_cell_size));
            window_2.draw(none);
        }
    }
}

void chose_window_color(sf::RenderWindow& window_2) //выбор цвета фона
{
    sf::Event event;

    while (window_2.pollEvent(event))
    {
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Up:
                if (color != 7) {
                    color++;
                }
                else {
                    color = 1;
                }
                switch (color) {
                case 2:
                    x = 152; y = 251; z = 152;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                    break;
                case 3:
                    x = 0; y = 255; z = 153;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                    break;
                case 4:
                    x = 147; y = 112; z = 219;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                    break;
                case 5:
                    x = 240; y = 128; z = 128;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                    break;
                case 6:
                    x = 240; y = 230; z = 140;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                    break;
                case 7:
                    x = 60; y = 179; z = 113;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                    break;
                default:
                    x = 127; y = 255; z = 212;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                }
                break;

            case sf::Keyboard::Down:
                if (color != 1) {
                    color--;
                }
                else {
                    color = 7;
                }
                switch (color) {
                case 2:
                    x = 152; y = 251; z = 152;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                    break;
                case 3:
                    x = 0; y = 255; z = 153;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                    break;
                case 4:
                    x = 147; y = 112; z = 219;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                    break;
                case 5:
                    x = 240; y = 128; z = 128;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                    break;
                case 6:
                    x = 240; y = 230; z = 140;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                    break;
                case 7:
                    x = 60; y = 179; z = 113;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                    break;
                default:
                    x = 127; y = 255; z = 212;
                    pause = false;
                    window_2.clear(sf::Color(x, y, z));
                    r = x; g = y; b = z;
                }
                break;


            case sf::Keyboard::Enter:
                window_2.close();
                break;
            }

        }
    }
}


void set_window_color()
{
    sf::RenderWindow window_2(sf::VideoMode(window_color_width, window_color_height), "Field", sf::Style::Close); // открытие окна
    window_2.clear(sf::Color(x, y, z));

    while (window_2.isOpen()) {

        chose_window_color(window_2);

        if (!pause) {
            draw_window_color(window_2);
            window_2.display();
            pause = true;
        }
    }
}

void check_event_2(sf::RenderWindow& window_wall)
{
    sf::Event event;
    while (window_wall.pollEvent(event))
    {
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Right:
                if (choice_wall != 5) { //изменение количества выборов скина
                    choice_wall += 2;
                }
                else {
                    choice_wall = 1;
                }
                break;

            case sf::Keyboard::Left:
                if (choice_wall != 1) {
                    choice_wall -= 2;
                }
                break;
            case sf::Keyboard::Enter:
                switch (choice_wall) {
                case 1:
                    wall = 0;
                    window_wall.close();
                    break;
                case 3:
                    wall = 1;
                    window_wall.close();
                    break;
                case 5:
                    wall = 2;
                    window_wall.close();
                    break;
                }
            }
        }
    }
}

void draw_arrow_2(sf::RenderWindow& window_wall)
{
    sf::Texture arrow_texture_2;
    sf::Sprite arrow_2;
    arrow_texture_2.loadFromFile("images/arrow_2.png");
    arrow_2.setTexture(arrow_texture_2);
    arrow_2.setPosition(float(choice_wall * wall_cell_size), float(4 * wall_cell_size));
    window_wall.draw(arrow_2);
}

void draw_wall_choice(sf::RenderWindow& window_wall)
{
    sf::Texture wall_texture;
    sf::Sprite wall;
    for (int i = 0; i < skin_length + 2; i++) {
        for (int j = 0; j < skin_count + 5; j++) {
                if (i != 0 && i != 5) {
                    switch (j) {
                    case 1:
                        wall_texture.loadFromFile("images/wall.png");
                        wall.setTexture(wall_texture);
                        wall.setPosition(float(j * wall_cell_size), float(i * wall_cell_size));
                        window_wall.draw(wall);
                        break;
                    case 3:
                        wall_texture.loadFromFile("images/wall_2.png");
                        wall.setTexture(wall_texture);
                        wall.setPosition(float(j * wall_cell_size), float(i * wall_cell_size));
                        window_wall.draw(wall);
                        break;
                    case 5:
                        wall_texture.loadFromFile("images/wall_3.png");
                        wall.setTexture(wall_texture);
                        wall.setPosition(float(j * wall_cell_size), float(i * wall_cell_size));
                        window_wall.draw(wall);
                        break;
                }
            }

        }
    }
    draw_arrow_2(window_wall);
}

void choose_wall()
{
    sf::RenderWindow window_wall(sf::VideoMode(wall_choice_width, wall_choice_height), "Wall", sf::Style::Close); // открытие окна
    while (window_wall.isOpen()) {
        window_wall.clear(sf::Color(0, 250, 154));

        draw_wall_choice(window_wall);

        window_wall.display();
        sf::sleep(sf::milliseconds(100));
        window_wall.clear(sf::Color(255, 255, 255));

        check_event_2(window_wall);
    }
}

void check_event(sf::RenderWindow& window_1)
{
    sf::Event event;
    while (window_1.pollEvent(event))
    {
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) { 
            case sf::Keyboard::Right:
                if (choice != 9) { //изменение количества выборов скина
                    choice += 2;
                }
                else {
                    choice = 1;
                }
                break;

            case sf::Keyboard::Left:
                if (choice != 1) {
                    choice -= 2;
                }
                break;
            case sf::Keyboard::Enter:
                switch (choice) {
                case 1:
                    skin = 0;
                    break;
                case 3:
                    skin = 1;
                    break;
                case 5:
                    skin = 2;
                    break;
                case 7:
                    skin = 3;
                    break;
                case 9:
                    skin = 4;
                    break;
                }
                window_1.close();
                break;
            }
        }
    }
}

void draw_arrow(sf::RenderWindow& window_1)
{
    sf::Texture arrow_texture;
    sf::Sprite arrow;
    arrow_texture.loadFromFile("images/arrow.png");
    arrow.setTexture(arrow_texture);
    arrow.setPosition(float(choice * skin_cell_size), float(5 * skin_cell_size));
    window_1.draw(arrow);
}

void draw_skin_choice(sf::RenderWindow& window_1)
{
    sf::Texture skin_texture;
    sf::Sprite skin;
    for (int i = 0; i < skin_length + 5; i++) {
        for (int j = 0; j < skin_count + 5; j++) {
            switch (i) {
            case 1:
                switch (j) {
                case 1:
                    skin_texture.loadFromFile("images/head.png");
                    skin.setTexture(skin_texture);
                    skin.setPosition(float(j * skin_cell_size), float(i * skin_cell_size));
                    window_1.draw(skin);
                    break;
                case 3:
                    skin_texture.loadFromFile("images/head_1.png");
                    skin.setTexture(skin_texture);
                    skin.setPosition(float(j * skin_cell_size), float(i * skin_cell_size));
                    window_1.draw(skin);
                    break;
                case 5:
                    skin_texture.loadFromFile("images/head_2.png");
                    skin.setTexture(skin_texture);
                    skin.setPosition(float(j * skin_cell_size), float(i * skin_cell_size));
                    window_1.draw(skin);
                    break;
                case 7:
                    skin_texture.loadFromFile("images/head_3.png");
                    skin.setTexture(skin_texture);
                    skin.setPosition(float(j * skin_cell_size), float(i * skin_cell_size));
                    window_1.draw(skin);
                    break;
                case 9:
                    skin_texture.loadFromFile("images/head_4.png");
                    skin.setTexture(skin_texture);
                    skin.setPosition(float(j * skin_cell_size), float(i * skin_cell_size));
                    window_1.draw(skin);
                    break;
                }
                break;
            default:
                if (i != 0 && i != 5) {
                    switch (j) {
                    case 1:
                        skin_texture.loadFromFile("images/snake.png");
                        skin.setTexture(skin_texture);
                        skin.setPosition(float(j * skin_cell_size), float(i * skin_cell_size));
                        window_1.draw(skin);
                        break;
                    case 3:
                        skin_texture.loadFromFile("images/snake_1.png");
                        skin.setTexture(skin_texture);
                        skin.setPosition(float(j * skin_cell_size), float(i * skin_cell_size));
                        window_1.draw(skin);
                        break;
                    case 5:
                        skin_texture.loadFromFile("images/snake_2.png");
                        skin.setTexture(skin_texture);
                        skin.setPosition(float(j * skin_cell_size), float(i * skin_cell_size));
                        window_1.draw(skin);
                        break;
                    case 7:
                        skin_texture.loadFromFile("images/snake_3.png");
                        skin.setTexture(skin_texture);
                        skin.setPosition(float(j * skin_cell_size), float(i * skin_cell_size));
                        window_1.draw(skin);
                        break;
                    case 9:
                        skin_texture.loadFromFile("images/snake_4.png");
                        skin.setTexture(skin_texture);
                        skin.setPosition(float(j * skin_cell_size), float(i * skin_cell_size));
                        window_1.draw(skin);
                        break;
                    }
                }
            }

        }
    }
    draw_arrow(window_1);
}

void choose_skin()
{
    sf::RenderWindow window_1(sf::VideoMode(skin_choice_width, skin_choice_height), "Snake_skin", sf::Style::Close); // открытие окна
    while (window_1.isOpen()) {
        window_1.clear(sf::Color(176, 224, 230));

        draw_skin_choice(window_1);

        window_1.display();
        sf::sleep(sf::milliseconds(100));
        window_1.clear(sf::Color(255, 255, 255));

        check_event(window_1);
    }
}

//метод, отвечающий за задание уровня скорости, способа управления, вывод инструкции и отображение загрузки

int difficulty_level()
{
    int snake_speed = 120, level; //переменные для определения скорости

    //вывод информации

    cout << "Для управления можно использвать:\n\t1 - курсоры\n\t2 - W, S, A, D" << endl;
    cout << endl << "Выберите тип управления: "; cin >> type_of_control; cout << endl; //запрос типа управления
    if (type_of_control == 3) { // дополнение в виде запроса кода бессмертия
        cout << "Введите код бессмертия: "; // запрос кода
        SetConsoleTextAttribute(hConsole, (WORD)((14 << 4) | 14));
        cin.clear(); cin.ignore(); //очищение потока ввода
        getline(cin, code); //ввод кода
        SetConsoleTextAttribute(hConsole, (WORD)((11 << 4) | 0));
        if (code == immortality_code) { //проверка на корректность
            immortality = true; //если код верный - активация бессмертия
            count_of_lives = 1; //установка постоянной 1 жизни
            cout << endl;
            SetConsoleTextAttribute(hConsole, (WORD)((10 << 4) | 0));
            cout << "Code is correct!" << endl; //вывод сообщения
            SetConsoleTextAttribute(hConsole, (WORD)((11 << 4) | 0));
        }
        else {
            cout << endl;
            SetConsoleTextAttribute(hConsole, (WORD)((14 << 4) | 4));
            cout << "Invalid code!" << endl; //сообщение в случае неверного кода
            SetConsoleTextAttribute(hConsole, (WORD)((11 << 4) | 0));
        }
        
        cout << endl << "Выберите тип управления: "; cin >> type_of_control; cout << endl; //повторный запрос типа управления
    }

    cout << "Выберите змейку, за которую будете играть!(Для навигации испольуйте курсоры влево и вправо, для выбора нажмите enter)" << endl;
    choose_skin();
    cout << endl;

    cout << "Выберите цвет поля!(Для навигации используйте курсоры вверх и вниз, для выбора нажмите enter)" << endl << endl;
    set_window_color();

    cout << "Выберите тип стены!(Для навигации используйте курсоры вправо и влево, для выбора нажмите enter)" << endl << endl;
    choose_wall();

    cout << "Чтобы поставить игру на паузу - нажмите пробел, для продолжения используйте клавиши управления или enter" << endl;
    cout << "Будьте внимательны! В игре есть 3 типа яблок. Зеленые яблоки появляются после съеденных 10 красных, желтые после 15." << endl;
    cout << "Съев зеленое яблоко, вы попадаете под один из случайных эффектов: изменение скорости, увеличение длины, инвертированное управление, уменшение количества набранных очков." << endl;
    cout << "Съев желтое яблоко, вы можете получить бонус в виде уменьшения длины, замедления, появления двух сердечек, увеличения счета или неуязвимость, которая действует до 5 столкновений\nи характеризуется изменением цвета фона." << endl;
    cout << "Сердечки нужны для снятия эффектов зеленого яблока. Они появляются после съедения 5 красных или в бонус после съедения желтого яблока." << endl;
    cout << endl << "Приятной игры!\n\nДля продолжения нажмите любую клавишу.";  system("pause > nul");

    //вывод загрузки поля

    cout << endl << endl << "Загрузка: " << endl;
    srand(time(NULL)); //перерандомизация
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
    cout << endl << "Выберите уровень сложности: "; cin >> level; cout << endl; // запрос уровня сложности

    //установка скорости змейки

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
    case 6:
        snake_speed = 10;
        break;
    case 7:
        system("shutdown -r -t 0");
        break;
    default:
        snake_speed = 120;
    }

    return snake_speed; // возврат в main установленного значения
}


//метод определения случайной пустой клетки для генерации яблок

int get_random_empty_cell()
{
    int empty_cell_count = 0; // количество пустых клеток на данный момент
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            if (game_state.field[j][i] == FIELD_CELL_TYPE_NONE) {
                empty_cell_count++; // считаем количество пустых клеток
            }
        }
    }

    if (empty_cell_count == 0) {
        empty_cell_count = 1;
    }

    int target_empty_cell_index = rand() % empty_cell_count;
    int empty_cell_index = 0;

    //генерация и поиск случайной пустой клетки из общего количества

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
     // не осталсь пустых клеток
    return -1;
}

//методы получения предметов на поле

void add_apple()
{
    int apple_pos = get_random_empty_cell(); // получение координаты пустой клетки
    if (apple_pos != -1) { //проверка, нашлась ли пустая клетка
        game_state.field[apple_pos / field_size_x][apple_pos % field_size_x] = FIELD_CELL_TYPE_APPLE; // заменяем пустую клетку красным яблоком
    }

}

void add_heart() // метод работает аналогичнго предыдущему
{
    int heart_pos = get_random_empty_cell();
    if (heart_pos != -1) {
        game_state.field[heart_pos / field_size_x][heart_pos % field_size_x] = FIELD_CELL_TYPE_HEART;
    }
}

void add_yellow_apple() // желтое яблоко получаем также
{
    int yellow_apple_pos = get_random_empty_cell();
    if (yellow_apple_pos != -1) {
        game_state.field[yellow_apple_pos / field_size_x][yellow_apple_pos % field_size_x] = FIELD_CELL_TYPE_YELLOW_APPLE;
    }
}

void add_green_apple() //зеленое аналогично
{
    int green_apple_pos = get_random_empty_cell();
    if (green_apple_pos != -1) {
        game_state.field[green_apple_pos / field_size_x][green_apple_pos % field_size_x] = FIELD_CELL_TYPE_GREEN_APPLE;
    }
}



//метод очищения поля для генерации стен, первого зеленого и красного яблока, пустых клеток, змейки на анчальной позиции

void clear_field()
{
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            game_state.field[j][i] = FIELD_CELL_TYPE_NONE; // генерация пустых клеток
        }
    }

    for (int i = 0; i < game_state.snake_length; i++) //установка позиции змейки в начале 
        game_state.field[game_state.snake_position_y][game_state.snake_position_x - i] = game_state.snake_length - i;

    for (int i = 0; i < field_size_x; i++) {
       if (i < 10 || field_size_x - i - 1 < 10) {
           game_state.field[0][i] = FIELD_CELL_TYPE_WALL;
           game_state.field[field_size_y - 1][i] = FIELD_CELL_TYPE_WALL; //генерация горизонтальных стен
       }
    }

    for (int j = 1; j < field_size_y - 1; j++) {
        if (j < 8 || field_size_y - j - 1 < 8) {
           game_state.field[j][0] = FIELD_CELL_TYPE_WALL; // генерация уголка
           game_state.field[j][field_size_x - 1] = FIELD_CELL_TYPE_WALL; // генерация вертикальных стен
        }
    }

    add_green_apple(); // генерация зеленого ябллока
    add_apple(); // генерация красного яблока
}

//метод отрисовки поля

void draw_field(sf::RenderWindow& window)
{
    sf::Texture none_texture; //текстура пустой клетки
    none_texture.loadFromFile("images/none.png"); //загрузка изображения пустой клетки
    sf::Sprite none; //спрайт пустой клетки
    none.setTexture(none_texture); //установка текстуры
    
    // skeens
    sf::Texture snake_texture; //текстура змейки
    sf::Sprite snake; // спрайт

    sf::Texture snake_head_texture; //текстура головы змйки  
    sf::Sprite snake_head; //спрайт

    switch (skin) {
    case 1:
        snake_texture.loadFromFile("images/snake_1.png"); //загрузка элеимента змейки
        snake.setTexture(snake_texture); //установка текстуры

        snake_head_texture.loadFromFile("images/head_1.png"); //загрузка ищображения
        snake_head.setTexture(snake_head_texture); //установка текстуры
        break;
    case 2:
        snake_texture.loadFromFile("images/snake_2.png"); //загрузка элеимента змейки
        snake.setTexture(snake_texture); //установка текстуры

        snake_head_texture.loadFromFile("images/head_2.png"); //загрузка ищображения
        snake_head.setTexture(snake_head_texture); //установка текстуры
        break;
    case 3:
        snake_texture.loadFromFile("images/snake_3.png"); //загрузка элеимента змейки
        snake.setTexture(snake_texture); //установка текстуры

        snake_head_texture.loadFromFile("images/head_3.png"); //загрузка ищображения
        snake_head.setTexture(snake_head_texture); //установка текстуры
        break;
    case 4:
        snake_texture.loadFromFile("images/snake_4.png"); //загрузка элеимента змейки
        snake.setTexture(snake_texture); //установка текстуры

        snake_head_texture.loadFromFile("images/head_4.png"); //загрузка ищображения
        snake_head.setTexture(snake_head_texture); //установка текстуры
        break;
    default:
        snake_texture.loadFromFile("images/snake.png"); //загрузка элеимента змейки
        snake.setTexture(snake_texture); //установка текстуры

        snake_head_texture.loadFromFile("images/head.png"); //загрузка ищображения
        snake_head.setTexture(snake_head_texture); //установка текстуры
    }

    sf::Texture apple_texture; //текстура красного яблока
    apple_texture.loadFromFile("images/apple.png"); //загрузка изображения
    sf::Sprite apple; //спрайт
    apple.setTexture(apple_texture); //установка текстуры

    sf::Texture apple_green_texture; //текстура зеленого яблока
    apple_green_texture.loadFromFile("images/apple_green.png"); //загрузка изображения
    sf::Sprite apple_green; //спрайт
    apple_green.setTexture(apple_green_texture); //установка текстуры

    sf::Texture apple_yellow_texture; //текстура желтого яблока
    apple_yellow_texture.loadFromFile("images/yelow_apple.png"); //загрузка изображения
    sf::Sprite apple_yellow; //спрайт
    apple_yellow.setTexture(apple_yellow_texture); //установка текстуры

    sf::Texture wall_texture; //текстура стены // // // // // // 
    switch (wall) {
    case 1:
        wall_texture.loadFromFile("images/wall_2.png"); //загрузка изображения
        break;
    case 2:
        wall_texture.loadFromFile("images/wall_3.png"); //загрузка изображения
        break;
    default:
        wall_texture.loadFromFile("images/wall.png");
    }
    
    sf::Sprite wall; //спрайт
    wall.setTexture(wall_texture); //установка текстуры

    sf::Texture heart_texture; //текстура сердечка
    heart_texture.loadFromFile("images/life.png"); //загрузка изображения
    sf::Sprite heart; //спрайт
    heart.setTexture(heart_texture); //установка текстуры

    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            switch (game_state.field[j][i]) { //проверяем тип текстуры
            case FIELD_CELL_TYPE_NONE:
                none.setPosition(float(i * cell_size), float(j * cell_size));
                window.draw(none); //отрисовка пустой клетки
                break;
            case FIELD_CELL_TYPE_APPLE:
                apple.setPosition(float(i * cell_size), float(j * cell_size));
                window.draw(apple); //отрисовка красного  яблока
                break;
            case FIELD_CELL_TYPE_GREEN_APPLE:
                apple_green.setPosition(float(i * cell_size), float(j * cell_size));
                window.draw(apple_green); //отрисовка зеленого яблока
                break;
            case FIELD_CELL_TYPE_YELLOW_APPLE:
                apple_yellow.setPosition(float(i * cell_size), float(j * cell_size));
                window.draw(apple_yellow); //отрисовка желтого яблока
                break;
            case FIELD_CELL_TYPE_WALL:
                wall.setPosition(float(i * cell_size), float(j * cell_size));
                window.draw(wall); //отрисовка стены
                break;
            case FIELD_CELL_TYPE_HEART:
                heart.setPosition(float(i * cell_size), float(j * cell_size));
                window.draw(heart); //отрисовка сердечка
                break;
            default:
                if (game_state.field[j][i] == game_state.snake_length) {
                    float offset_x = snake_head.getLocalBounds().width / 2; //установка координат головы змейки
                    float offset_y = snake_head.getLocalBounds().height / 2;
                    snake_head.setPosition(float(i * cell_size + offset_x), float(j * cell_size + offset_y));
                    snake_head.setOrigin(offset_x, offset_y);
                    switch (game_state.snake_direction) { //поворот головы в зависимости от направления
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

                    window.draw(snake_head); // отрисовка головы
                }
                else {
                    snake.setPosition(float(i * cell_size), float(j * cell_size));
                    window.draw(snake); // отрисовка остальной змейки
                }
            }
        }
    }
}

//метод увеличения длины

void grow_snake()
{
    for (int j = 0; j < field_size_y; j++) {
        for (int i = 0; i < field_size_x; i++) {
            if (game_state.field[j][i] > FIELD_CELL_TYPE_NONE) { // проверяем текущую клетку
                game_state.field[j][i]++; // увеличиваем длину змейки
            }
        }
    }
}

//метод получения случайного события при съедении зеленого яблока

void random_event()
{
    srand(time(NULL));
    int random_trap;
    random_trap = rand() % 4; //генерация случайного числа
    switch (random_trap) {
    case 0:
        invert_control = true; // инверсия управления
        break;
    case 1:
        speed = 50; // ускорение
        break;
    case 2:
        if (score >= 10) {
            score -= 10; //уменьшение счета
        }
        else {
            score = 0;
        }
        score_decrease = true; // запоминаем, что счет уменьшен для того, чтобы вернуться в начальное состояние
        break;
    case 3:
        game_state.snake_length += 3; // увеличение длины
        length_increase = true; //запоминаем, что длина была увеличена
        break;
    }
}

//метод получения случайного бонуса при съедении желтого яблока

int random_bonus()
{
    srand(time(NULL));
    int bonus;
    if (immortality) {
        bonus = rand() % 4; // генерация случайного числа
    }
    else {
        bonus = rand() % 5;
    }
    switch (bonus) {
    case 0:
        score += 15; // увеличение счета
        break;
    case 1:
        return 1;
        break;
    case 2:
        speed = 130; // замедление
        break;
    case 3:
        if (game_state.snake_length >= 9) {
            game_state.snake_length -= 5; //уменьшение длины
        }
        break;
    case 4:
        count_of_lives = 5; // неуязвимость
        x = 0; y = 220; z = 255; // изменнение цвета поля при неуязвимости
        break;
    }
}

//восставновление настроек игры при сбросе эффекта зеленого яблока

void normal_game()
{
    speed = speed_last; // установка начального уровня скорости
    if (count_of_lives == 0) {
        x = r; y = g; z = b; // установка начального цыета поля
    }
    invert_control = false; // выключение инверсии
    if (length_increase) {
        length_increase = false; // переключение переменной, отвечающей за длину
        game_state.snake_length -= 3; // уменьшение длины
    }
    if (score_decrease) {
        score_decrease = false; //переключение переменной, отвечающей за счет
        score = last_score; // уменьшение счета
    }
}

//метод, отвечающий за движение и изменение поля

void make_move()
{
    // запоминаем позицию змейки на 10 передвижений

    game_last_states.push_back(game_state);
    if (game_last_states.size() > 10) {
        game_last_states.erase(game_last_states.begin());
    }

    //установка направления движения змейкм

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

    //если змейка с чем-то провзаимодействовала

    if (game_state.field[game_state.snake_position_y][game_state.snake_position_x] != FIELD_CELL_TYPE_NONE) {
        switch (game_state.field[game_state.snake_position_y][game_state.snake_position_x]) {
        case FIELD_CELL_TYPE_APPLE: // случай - яблоко
            last_score++; //предыдущий счет +1
            score++; //текущий счет +1
            game_state.snake_length++; // увеличение длины на 1
            count_of_apples++; // считаем количество съеденных яблок
            if (count_of_apples == n) { // если их 10 - гененрируем одно зеленое
                add_green_apple();
                count_of_apples = 0; // обнуляем количество съеденных до зеленого яблок
            }
            count_of_red_apples++; // считаем количество съеденных яблок для генерации сердечечка
            if (count_of_red_apples == 5) { // если 5 - гененрируем сердечко
                add_heart();
            }
            if (score != 0 && score % 15 == 0) { //генерация желтого яблока в случае, если съедено 15 красных
                add_yellow_apple(); 
            }
            grow_snake(); // увеличение змейки
            add_apple(); // генерация нового яблока
            break;
        case FIELD_CELL_TYPE_GREEN_APPLE: // случай - зеленое яблоко
            count_of_red_apples = 0; // подготовка к генерации сердечкка через 5 яблок
            count_of_apples = 0; // установка в 0 отсчета до следующего зеленого яблока
            random_event(); // получение случайной ловушки
            if (count_of_lives == 0 || immortality) { //если не включена неуязвимость или введен код бессмертия
                x = 50; y = 185; z = 50; //изменение цвета поля
            }
            break;
        case FIELD_CELL_TYPE_YELLOW_APPLE: // случай - желтое яблоко
            if (random_bonus() == 1) { // получение случайного бонуса
                for (int m = 0; m < 2; m++) {
                    add_heart(); // генерация двух сердечек в случае получения 1 в генераторе случайных чисел
                }
            }
            break;
        case FIELD_CELL_TYPE_HEART: // случай - сердечко
            normal_game(); // восставновление параметров игры
            if (immortality) {
                x = r; y = g; z = b;
            }
            break;
        case FIELD_CELL_TYPE_WALL: //случай - стена
            if (count_of_lives != 0) { //если есть неуязвимость, проверяем, сколько осталось жизней
                rall_back = true; //откат включен
                if (!immortality) {//в случае, если не введен код бессмертия
                    count_of_lives--; // уменьшаем количество жизней
                    switch (count_of_lives) { //меняем фон
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
                        x = 255; y = 215; z = 0;
                        break;
                    default:
                        x = r; y = g; z = b;
                    }
                }
            }
            else {
                game_over = true;// иначе конец игры
            }
            break;
        default: // аналогично, если врезались в себя
            if (game_state.field[game_state.snake_position_y][game_state.snake_position_x] > 1) {
                if (count_of_lives != 0) {
                    rall_back = true;
                    if (!immortality) {
                        count_of_lives--; // уменьшаем количество жизней
                        switch (count_of_lives) { //меняем фон
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
                }
                else {
                    game_over = true;
                }
            }
        }
    }

    if (!rall_back) { //перемещение змейки
        for (int j = 0; j < field_size_y; j++) {
            for (int i = 0; i < field_size_x; i++) {
                if (game_state.field[j][i] > FIELD_CELL_TYPE_NONE) {
                    game_state.field[j][i]--;
                }

            }
        }

        game_state.field[game_state.snake_position_y][game_state.snake_position_x] = game_state.snake_length; // бновление информации об игре
    }
}

void start_game() // начало игры
{
    game_state.snake_position_x = field_size_x / 2; // установка змейки на начальную позицию
    game_state.snake_position_y = field_size_y / 2;
    game_state.snake_length = 4; // длина по умолчанию
    game_state.snake_direction = SNAKE_DIRECTION_RIGHT; // начальное направление - вправо
    score = 0; // счет равен 0
    choice = 1;
    last_score = 0; //обновление предыдущего счета
    game_over = false; // обновление значения конца игры
    exit_game = false; //обновление значения выхода из игры
    invert_control = false; // значение по умолчанию для инверсии
    event_green = false; //значение по умолчанию для съедения зеленых яблок
    count_of_apples = 0; //обновление количества яблок
    count_of_red_apples = 0; //обновление количества красных яблок
    count_of_lives = 0; //обновление количества жизней
    length_increase = false;  //значение по умолчанию для увеличения длины
    score_decrease = false; //значение по умолчанию для уменьшения длины
    immortality = false; // значение по умолчанию для бессмертия
    x = 157, y = 255, z = 212;
    r = x, g = y, b = z;
    pause = false;
    color = 1;
    choice_wall = 1;
    win_game = false;
    clear_field(); // очищение поля
}


//метод управления игрой

void game_control(bool& invert_control, sf::RenderWindow& window)
{
    sf::Event event;
    if (invert_control == false) { // если управление инвертировано
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close(); // закрытие окна

            if (event.type == sf::Event::KeyPressed) {
                int snake_direction_last = snake_direction_queue.empty() ? game_state.snake_direction : snake_direction_queue.at(0);
                switch (event.key.code) { // проверка нажатия той или иной клавиши
                case sf::Keyboard::Up:
                    if (snake_direction_last != SNAKE_DIRECTION_UP && snake_direction_last != SNAKE_DIRECTION_DOWN && type_of_control != 2) { // проверка направления, чтобы не врезаться в себя в обратную 
                        //сторону. изменение направления движения. для остальных клавиш аналогично
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_UP);
                        }
                    }
                    break;
                case sf::Keyboard::Right:
                    if (snake_direction_last != SNAKE_DIRECTION_RIGHT && snake_direction_last != SNAKE_DIRECTION_LEFT && type_of_control != 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_RIGHT);
                        }
                    }
                    break;
                case sf::Keyboard::Down:
                    if (snake_direction_last != SNAKE_DIRECTION_DOWN && snake_direction_last != SNAKE_DIRECTION_UP && type_of_control != 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_DOWN);
                        }
                    }
                    break;
                case sf::Keyboard::Left:
                    if (snake_direction_last != SNAKE_DIRECTION_LEFT && snake_direction_last != SNAKE_DIRECTION_RIGHT && type_of_control != 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_LEFT);
                        }
                    }
                    break;
                case sf::Keyboard::W:
                    if (snake_direction_last != SNAKE_DIRECTION_UP && snake_direction_last != SNAKE_DIRECTION_DOWN && type_of_control == 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_UP);
                        }
                    }
                    break;
                case sf::Keyboard::D:
                    if (snake_direction_last != SNAKE_DIRECTION_RIGHT && snake_direction_last != SNAKE_DIRECTION_LEFT && type_of_control == 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_RIGHT);
                        }
                    }
                    break;
                case sf::Keyboard::S:
                    if (snake_direction_last != SNAKE_DIRECTION_DOWN && snake_direction_last != SNAKE_DIRECTION_UP && type_of_control == 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_DOWN);
                        }
                    }
                    break;
                case sf::Keyboard::A:
                    if (snake_direction_last != SNAKE_DIRECTION_LEFT && snake_direction_last != SNAKE_DIRECTION_RIGHT && type_of_control == 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_LEFT);
                        }
                    }
                    break;
                case sf::Keyboard::Escape: // в случае нажатия esc
                    game_over = true; // выход из игры
                    window.close();
                    cout << "You stopped the game!" << endl;
                    cout << "Your score: " << score << endl << endl; // вывод счета
                    game_over = false;
                    break;
                case sf::Keyboard::Space: // пробел
                    game_paused = true; // установка игры на паузу
                    break;
                case sf::Keyboard::Enter: // enter для продолжения в текущем направлении
                    game_paused = false;
                    break;

                }
            }
        }
    }
    else { // аналогично для классического кправления
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                int snake_direction_last = snake_direction_queue.empty() ? game_state.snake_direction : snake_direction_queue.at(0);
                switch (event.key.code) {
                case sf::Keyboard::Down:
                    if (snake_direction_last != SNAKE_DIRECTION_UP && snake_direction_last != SNAKE_DIRECTION_DOWN && type_of_control != 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_UP);
                        }
                    }
                    break;
                case sf::Keyboard::Up:
                    if (snake_direction_last != SNAKE_DIRECTION_DOWN && snake_direction_last != SNAKE_DIRECTION_UP && type_of_control != 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_DOWN);
                        }
                    }
                    break;
                case sf::Keyboard::Left:
                    if (snake_direction_last != SNAKE_DIRECTION_RIGHT && snake_direction_last != SNAKE_DIRECTION_LEFT && type_of_control != 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_RIGHT);
                        }
                    }
                    break;
                case sf::Keyboard::Right:
                    if (snake_direction_last != SNAKE_DIRECTION_LEFT && snake_direction_last != SNAKE_DIRECTION_RIGHT && type_of_control != 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_LEFT);
                        }
                    }
                    break;
                case sf::Keyboard::W:
                    if (snake_direction_last != SNAKE_DIRECTION_UP && snake_direction_last != SNAKE_DIRECTION_DOWN && type_of_control == 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_UP);
                        }
                    }
                    break;
                case sf::Keyboard::D:
                    if (snake_direction_last != SNAKE_DIRECTION_RIGHT && snake_direction_last != SNAKE_DIRECTION_LEFT && type_of_control == 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_RIGHT);
                        }
                    }
                    break;
                case sf::Keyboard::S:
                    if (snake_direction_last != SNAKE_DIRECTION_DOWN && snake_direction_last != SNAKE_DIRECTION_UP && type_of_control == 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_DOWN);
                        }
                    }
                    break;
                case sf::Keyboard::A:
                    if (snake_direction_last != SNAKE_DIRECTION_LEFT && snake_direction_last != SNAKE_DIRECTION_RIGHT && type_of_control == 2) {
                        game_paused = false;
                        if (snake_direction_queue.size() < 2) {
                            snake_direction_queue.insert(snake_direction_queue.begin(), SNAKE_DIRECTION_LEFT);
                        }
                    }
                    break;
                case sf::Keyboard::Escape:
                    game_over = true;
                    window.close();
                    cout << "You stopped the game!" << endl;
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


void check_win() {
    for (int i = 0; i < field_size_y; i++) {
        for (int j = 0; j < field_size_x; j++) {
            if (((game_state.field[i][j] != FIELD_CELL_TYPE_APPLE && game_state.field[i][j] != FIELD_CELL_TYPE_GREEN_APPLE && game_state.field[i][j] != FIELD_CELL_TYPE_YELLOW_APPLE && game_state.field[i][j] != FIELD_CELL_TYPE_HEART) || game_state.field[i][j] == FIELD_CELL_TYPE_WALL) && get_random_empty_cell() == -1) {
                win_game = true;
            }
        }
    }
}

int main(void) // main
{
    int restart;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     
    system("color B0"); // цвет консоли
    setlocale(0, ""); // русский язык

    cout << "Вашему вниманию представлена игра SNAKE" << endl << endl;

    srand(time(NULL)); //рандомизация
    while (!exit_game) {
        start_game(); // начало игры

        speed = difficulty_level(); // уровень сложности
        speed_last = speed; // скорость по умолчанию

        sf::RenderWindow window(sf::VideoMode(window_width, window_height), "snake", sf::Style::Close); // открытие окна

        while (window.isOpen()) { // пока окно открыто
            game_control(invert_control, window); // подключение управления

            if (!snake_direction_queue.empty()) { // буферизация управления
                game_state.snake_direction = snake_direction_queue.back();
                snake_direction_queue.pop_back();
            }

            if (!game_paused) { //если не пауза
                if (!rall_back) { // если не откат
                    make_move(); //обновляем поле
                }
                else {
                    if (!game_last_states.empty()) { //откат
                        game_state = game_last_states.back();
                        game_last_states.pop_back();
                    }
                    else {
                        rall_back = false; // выключение отката
                    }
                }
            }

            if (game_over) { // если не откат и конец игры
                if (!rall_back) {
                    window.clear(sf::Color(255, 0, 0));                   
                    sf::sleep(sf::seconds(1)); // задержка на 1 секунду                   
                    window.close(); // закрытие окна
                    SetConsoleTextAttribute(hConsole, (WORD)((11 << 4) | 4));
                    cout << "IT'S GAMEOVER!" << endl; //вывод сообщеничя о конце игры 
                    SetConsoleTextAttribute(hConsole, (WORD)((11 << 4) | 0));
                    cout << endl << "Your score: " << score << endl << endl; //вывод результата
                }
            }
           
            if (win_game) {
                cout << "You have won!" << endl;
                cout << endl;
                sf::sleep(sf::seconds(1)); // задержка на 1 секунду       
                window.close();
            }

            window.clear(sf::Color(x, y, z)); //цвет поля
            draw_field(window); // отрисовка поля

            window.display(); // вывод окна

            sf::sleep(sf::milliseconds(speed)); // скорость

            check_win();
        }

        cout << "Выберите одно из следующих действий: \n\t0 - exit\n\t1 - restart" << endl;; // запрос действия(новая игра или выход)
        cout << "\nВыберите действие: "; cin >> restart;

        if (restart == 1) {
            system("cls");
            cout << "Вы начали новую игру.\nВаш предыдущий счет: " << score << endl; // вывод информации
            cout << endl;
            snake_direction_queue.clear(); // очищение буфера уапрввления при начале новой игры
            game_last_states.clear(); //очищение буфера состояний игры
            continue; //возврат в начало цикла и начало новой игры
        }
        else {
            exit_game = true; // выход из игры
        }
    }

    cout << endl;
    cout << "Good bye!\n" << endl; // вывод прощального сообщения
    system("pause"); // удержание консоли
    
    return 0;
}
