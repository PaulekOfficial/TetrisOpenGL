#include "main.h"

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <algorithm>
#include <fstream>
#include <thread>
#include <GL/gl.h>
#include "stb_image.h"
#include "locale.h"

using namespace std;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_SPACE:
            game.dropElement();
            break;
        case GLFW_KEY_Z:
            game.rotate(Element::Rotation::Left);
            break;
        case GLFW_KEY_X:
            game.rotate(Element::Rotation::Right);
            break;
        case GLFW_KEY_A:
            game.moveElement(true);
            break;
        case GLFW_KEY_D:
            game.moveElement(false);
            break;
        case GLFW_KEY_S:
            game.setSpeedDrop(true);
            break;
		}
    }

    if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_S:
            game.setSpeedDrop(false);
            break;
        }
    }
}


void windowFocusCallback(GLFWwindow* window, int focused)
{

}


int main()
{
    /* Prepare console */
    setlocale(LC_ALL, "pl-PL.UTF-8");

    /* initialize random seed: */
    srand(time(NULL));

    /* load ranking from file */
    loadRanking();

    currentStatus = mainMenu;

    while (consoleHandler())
    {
    }

	return -1;
}

bool consoleHandler()
{
    string value = "";
    int rows = 0;
    int cols = 0;
    int elementSize = 0;
	switch (currentStatus)
	{
	case mainMenu:
        system("CLS");
        cout << "Witaj w grze tetris wykorzystującej silnik grafiki OpenGL" << endl;
        cout << "Aby, uruchmić grę wybierz tryb w który chcesz zagrać" << endl;

        cout << "___________________________________________________" << endl << endl;

        cout << "[1] Tetris classic" << endl;
        cout << "[2] Tetris enhanced" << endl;
        cout << "[3] Tetris gra własna (konfigurowalna)" << endl;
        cout << "[4] Ustawienia dzwięku" << endl;
        cout << "[5] Wyświetl tabelę wyników" << endl;
        cout << "[6] Wylacz program" << endl;

        cout << "___________________________________________________" << endl << endl;

        cout << "Muzyka wykożystana w grze pochodi od orginalnej wersji Tetris99" << endl << endl;

        cout << "Wpisz numer zawarty w nawiasach  kwadratowych [] i potwierdź klikając enter." << endl << endl;
        cout << "Sterowanie: " << endl;
        cout << "A - D -> Ruch prawo, lewo" << endl;
        cout << "Z - X -> Obracanie elemętu" << endl;
        cout << "S -> Przyśpieszony opad" << endl;
        cout << "Spacja -> Natychmiastowy opad" << endl;

        for (int i = 0; i < 8; i++)
        {
            cout << endl;
        }

        cout << "Wybieram opcję numer: ";
        cin >> value;
        currentStatus = static_cast<ConsoleStatus>(stoi(value));
        break;
	case Classic:
        system("CLS");
        cout << "Uruchamiam grę w wersji classic, proszę czekać" << endl;
        currentGameSettings = gameSettings;
        initGame(false, false);
        break;
    case Enchanced:
        system("CLS");
        cout << "Uruchamiam grę w wersji enchanced, proszę czekać" << endl;
        currentGameSettings = gameSettings;
        initGame(true, false);
        break;
	case SaveScoreboard:
        system("CLS");
        cout << "Wow! Świetna gra, oto twoje statystyki z gry:" << endl << endl;
        cout << "___________________________________________________" << endl << endl;
        cout << "Zdobyłeś " << to_string(game.getScore()) << " punktów w grze," << endl;
        cout << "Byłeś na " << to_string(game.getLevel()) << " poziomie," << endl;
        cout << "Wyczyściłeś aż " << to_string(game.getCleanedLines()) << " linii klocków!," << endl << endl;
        cout << "___________________________________________________" << endl << endl;
        cout << "Możejsz je zapisać do tablicy chwały aka. scoreboard" << endl;
        cout << "W miejsce zapisu nazwy gracza wpisz swoje imię lub psełdonim oraz naciśnij przycisk enter, aby zapisać twoje wyniki" << endl;
        cout << "Jeżeli nie checesz zapisywać wyniku - poprostu wpisz x i potwierdz przyciskiem enter" << endl;

        for (int i = 0; i < 16; i++)
        {
            cout << endl;
        }

        cout << "Moje imie lub psedłonim to: ";
        cin >> value;
        if (value != "x")
        {
            auto score = PlayerScore{ value, game.getScore() };
            scores.push_back(score);
            std::sort(scores.begin(), scores.end(), sort_ranking());

            saveRanking();
        }
        currentStatus = mainMenu;
        break;
	case Scoreboard:
        system("CLS");
        cout << "Wyniki graczy:" << endl << endl;
        cout << "___________________________________________________" << endl << endl;

        for (int i = 0; i < 10; i++)
        {
            auto score = PlayerScore{ "Brak", 0 };
            if (scores.size() > i)
            {
                score = scores.at(i);
            }
            cout << "[" << i + 1 << "] " << "[" << score.score << "] " << score.playerName << endl;
        }
        
        cout << "___________________________________________________" << endl << endl;
        cout << "Kliknij enter aby wyjść z tabeli wyników." << endl;

        for (int i = 0; i < 12; i++)
        {
            cout << endl;
        }

        system("pause");
        currentStatus = mainMenu;
        break;
    case SoundSettings:
        system("CLS");
        cout << "Ustawienia dzięku:" << endl << endl;
        cout << "___________________________________________________" << endl << endl;
        cout << "Akutalna głośność: " << to_string(volume * 10) << endl;
        cout << "___________________________________________________" << endl << endl;
        cout << "Wpisz wartośc między 0 a 10 aby ustawić głośność dzięku" << endl;

        for (int i = 0; i < 21; i++)
        {
            cout << endl;
        }

        cout << "Wartość dzięku: ";
        cin >> value;
        volume = stod(value) * 0.1;
        currentStatus = mainMenu;
        break;
	case Custom:
        system("CLS");

        while (rows < 19 || rows > 40)
        {
            cout << "Podaj liczbę wierszy (Wartośc musi wyć większa od 19 i mniejsza od 40): " << endl;
            cin >> rows;
        }
        customGameSettings.boardNumRows = rows;

        while (cols < 9 || cols > 40)
        {
            cout << "Podaj liczbę wierszy (Wartośc musi wyć większa od 9 i mniejsza od 40): " << endl;
            cin >> cols;
        }
        customGameSettings.boardNumCols = cols;

        while (elementSize < 31 || elementSize > 51)
        {
            cout << "Podaj wielkosc klockow (Wartośc musi wyć większa od 31 i mniejsza od 51): " << endl;
            cin >> elementSize;
        }
        customGameSettings.elementSize = elementSize;

        customGameSettings.boardWidth = customGameSettings.boardNumCols * customGameSettings.elementSize;
        customGameSettings.boardHeight = customGameSettings.boardNumRows * customGameSettings.elementSize;
        customGameSettings.windowWidth = 3 * customGameSettings.margin + customGameSettings.boardWidth + customGameSettings.hudWidth;
		customGameSettings.windowHeight = 2 * customGameSettings.margin + customGameSettings.boardHeight;

        cout << "Uruchamiam grę w wersji custom, proszę czekać" << endl;
        currentGameSettings = customGameSettings;
        initGame(false, true);
        break;
	case Exit:
        return false;
	}

    return true;
}

void initGame(bool enchanced, bool custom)
{

    /* Load standard assets */
    string systemPath = "";//"C:\\Users\\pawel\\source\\repos\\tetris-game\\";

    // Create opengl window
    window = setupOpenGL();

    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowFocusCallback(window, windowFocusCallback);

    // Load resources after enable blend IMPORTANTE!
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

    // Load resources
    cout << "Ładowanie zasobów oraz tekstur... " << endl;
    vector<Texture> elements;
    std::vector<std::string> colors = { "cyan", "blue", "orange", "yellow", "green", "purple", "red" };
    
    cout << "Ładowanie tekstór bloczków" << endl;
    for (int color = Element::Color::cyan; color <= Element::Color::red; ++color) {
        std::string path = systemPath + "resources\\tile_" + colors[color] + ".png";
        cout << path << endl;
    
        elements.push_back(loadTexture(path));
    }
    
    cout << "Ładowanie kontur tekstór bloczków" << endl;
    vector<Texture> imaginaryElement;
    for (int color = Element::Color::cyan; color <= Element::Color::red; ++color) {
        std::string path = systemPath + "resources\\contour_" + colors[color] + ".png";
        cout << path << endl;
    
        imaginaryElement.push_back(loadTexture(path));
    }
    
    cout << "Ładowanie paczki liter" << endl;
    auto font = loadFont(systemPath + "resources\\Lato-Bold.ttf", currentGameSettings.fontSize);

    game = TetrisGame(currentGameSettings.boardNumRows, currentGameSettings.boardNumCols, enchanced);
    
    glm::mat4 projection = glm::ortho(0.0f, (GLfloat)currentGameSettings.windowWidth, (GLfloat)currentGameSettings.windowHeight, 0.0f, -1.0f, 1.0f);
    Drawer drawer(projection, currentGameSettings.elementSize, currentGameSettings.boardX, currentGameSettings.boardY, currentGameSettings.boardNumRows, currentGameSettings.boardNumCols, elements, imaginaryElement, font, enchanced);
    
    SoundEngine->setSoundVolume(volume);
    SoundEngine->play2D("resources\\theme-music.mp3", true, false, false, irrklang::ESM_STREAMING);
    
    // From this point we can go to the mainloop of game
    while (!glfwWindowShouldClose(window))
    {
        /* Update game state */
        game.tick();
    
        /* Poll for and process events */
        glfwPollEvents();
     
        /* Render actual frame */
        drawFrame(drawer, game);
     
        /* Swap front and back buffers */
    	glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    SoundEngine->stopAllSounds();

    currentStatus = SaveScoreboard;
}


GLFWwindow* setupOpenGL()
{
    cout << "Konfigurowanie okna OpenGL..." << endl;
    // Initialize the library
    if (!glfwInit())
    {
        cout << "Glfw init error!" << endl;
        return nullptr;
    }

    // Setup window properties
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create new window
    cout << "Tworzenie okienka, wysokość: " << currentGameSettings.windowHeight << " długość: " << currentGameSettings.windowWidth << endl;
    auto gl_window = glfwCreateWindow(currentGameSettings.windowWidth, currentGameSettings.windowHeight, windowName.c_str(), NULL, NULL);

    if (gl_window == NULL)
    {
        cout << "Błąd tworzenia okna, zamykam operację" << endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(gl_window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        cout << "Glew init error - ten blad oznacza, ze twoja karta graficzna nie wspiera OpenGL w najnowszej wersji!" << err << endl;
        system("pause");
        throw std::runtime_error("Glew init error - ten blad oznacza, ze twoja karta graficzna nie wspiera OpenGL w najnowszej wersji!");
    }
    
    return gl_window;
}

void drawFrame(Drawer& drawer, TetrisGame game)
{
    glClearColor(0.5, 1, 0.4, 0.6);
    glClear(GL_COLOR_BUFFER_BIT);

    drawer.drawTextCentered("NASTEPNY", currentGameSettings.hudX, currentGameSettings.hudY, currentGameSettings.hudWidth, glm::vec3(0, 0, 0));
    
    GLfloat letterHeight = drawer.computeTextHeight("A");
    GLfloat letterWidth = drawer.computeTextWidth("A");
    
    
    GLfloat y = 0.4f * currentGameSettings.windowHeight;
    drawer.drawTextCentered("POZIOM", currentGameSettings.hudX, y, currentGameSettings.hudWidth, glm::vec3(0, 0, 0));
    
    y += 1.4 * letterHeight;
    drawer.drawTextCentered(to_string(game.getLevel()), currentGameSettings.hudX, y, currentGameSettings.hudWidth, glm::vec3(0, 0, 0));
    
    
    y += 4 * letterHeight;
    drawer.drawTextCentered("WIERSZY", currentGameSettings.hudX, y, currentGameSettings.hudWidth, glm::vec3(0, 0, 0));
    
    y += 1.5 * letterHeight;
    drawer.drawTextCentered(to_string(game.getCleanedLines()), currentGameSettings.hudX, y, currentGameSettings.hudWidth, glm::vec3(0, 0, 0));
    
    
    y += 4 * letterHeight;
    drawer.drawTextCentered("TWOJ WYNIK", currentGameSettings.hudX, y, currentGameSettings.hudWidth, glm::vec3(0, 0, 0));
    
    y += 1.5 * letterHeight;
    drawer.drawTextCentered(to_string(game.getScore()), currentGameSettings.hudX, y, currentGameSettings.hudWidth, glm::vec3(0, 0, 0));
    
    
    y += 15 * letterHeight;
    drawer.drawTextCentered("Pawel Fortuna", currentGameSettings.hudX, y, currentGameSettings.hudWidth, glm::vec3(0, 0, 0));

    // Render background
    drawer.renderBackground();

    //Render whole board
    drawer.renderElementsOnBoard(game.getGameBoard(), 3);

    if (!game.getRowsToClear().empty())
    {
        drawer.playLinesClearAnimation(game.getGameBoard(), game.getRowsToClear(), game.getClearAnimationTime() / (2.0 / game.getLevel()));
    }

    // Render next element
    drawer.renderNextElement(game.getNextElement(), 0.0f, 2);

    if (game.isOver())
    {
        drawer.drawTextCentered("Game Over.", currentGameSettings.boardX, currentGameSettings.boardHeight / 2 - 30, currentGameSettings.boardWidth, glm::vec3(1, 0, 0));
        drawer.drawTextCentered("Zamknij okno OpenGL i przejdz do konsoli", currentGameSettings.boardX, currentGameSettings.boardHeight / 2 + 30, currentGameSettings.boardWidth, glm::vec3(1, 0, 0));

	    return;
    }

    // Render current element
    if (game.seetling())
    {
        drawer.renderElement(game.getCurrenElement(), game.getGameBoard().getY(), game.getGameBoard().getX(), 0.0f, 0.3f);
	    return;
    }
    drawer.renderElement(game.getCurrenElement(), game.getGameBoard().getY(), game.getGameBoard().getX(), 0.0f, 0.6f);

    // Render imaginary block
    drawer.renderImaginaryElement(game.getCurrenElement(), game.getGameBoard().getLowestPosition(game.getCurrenElement()), game.getGameBoard().getX());
}

Texture loadTexture(string path)
{
    // flip the image vertically, so the first pixel in the output array is the bottom left
    stbi_set_flip_vertically_on_load(1);

    int width, height, numChannels;
    GLubyte* image = stbi_load(path.c_str(), &width, &height, &numChannels, 4);
    Texture texture(width, height, image, GL_RGBA);

    // free the loaded image -- this is just free()
    stbi_image_free(image);
    return texture;
}

// https://learnopengl.com/In-Practice/Text-Rendering
std::vector<FragmentGlyph> loadFont(const std::string& path, unsigned int glyphHeight) {
    FT_Library ft;
    FT_Init_FreeType(&ft);

    FT_Face face;
    FT_Error error = FT_New_Face(ft, path.c_str(), 0, &face);

    error = FT_Set_Pixel_Sizes(face, 0, glyphHeight);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    std::vector<FragmentGlyph> glyphs;
    for (GLubyte c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "Failed to load glyph " << c << "." << std::endl;
            continue;
        }

        Texture texture( face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer, GL_RED);
        FragmentGlyph glyph = { texture,
                       glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                       face->glyph->advance.x >> 6 };

        glyphs.push_back(glyph);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return glyphs;
}

void loadRanking()
{
    ifstream rankingFile;
    rankingFile.open("ranking.txt");

    string line;
    while (getline(rankingFile, line))
    {
        cout << line << endl;
        int pos = line.find("<<>>");
        if (pos == string::npos)
        {
	        continue;
        }

        string name = line.substr(0, pos);
        cout << "name " << name << endl;
        string value = line.substr(pos + 4, line.size());
        cout << "value " << value << endl;

        cout << "Adding new score for player " << name << " score: " << value << endl;
        scores.push_back(PlayerScore{ name, stoi(value) });
    }

    std::sort(scores.begin(), scores.end(), sort_ranking());

    rankingFile.close();
}

void saveRanking()
{
    ofstream rankingFile;
    rankingFile.open("ranking.txt");
    if (!rankingFile.is_open())
    {
	    return;
    }

    for (PlayerScore score : scores)
    {
        rankingFile << score.playerName << "<<>>" << to_string(score.score) << endl;
    }

    rankingFile.close();
}
