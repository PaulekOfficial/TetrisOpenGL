#include <string>
#include <vector>
#include <string>
#include "Drawer.h"
#include <GLFW/glfw3.h>
#include <irrKlang/irrKlang.h>

#include "TetrisGame.h"

struct GameSettings
{
	float elementSize = 32;
	int boardNumRows = 20;
	int boardNumCols = 10;
	float boardWidth = boardNumCols * elementSize;
	float boardHeight = boardNumRows * elementSize;
	float margin = 35;
	float hudWidth = 160;
	float windowWidth = 3 * margin + boardWidth + hudWidth;
	float windowHeight = 2 * margin + boardHeight;
	float hudX = margin;
	float hudY = margin;
	float boardX = 2 * margin + hudWidth;
	float boardY = margin;
	float hudPieceBoxHeight = 2.5f * elementSize;
	int fontSize = 18;
};

GameSettings gameSettings;
GameSettings customGameSettings;
GameSettings currentGameSettings;

enum ConsoleStatus { mainMenu = 0, Classic, Enchanced, Custom, SoundSettings, Scoreboard, Exit, SaveScoreboard };

struct PlayerScore
{
	std::string playerName;
	int score;
};

struct sort_ranking {
    bool operator()(const PlayerScore& left, const PlayerScore& right) {
        return left.score > right.score;
    }
};

double volume = 0.08;

ConsoleStatus currentStatus;

std::string windowName = "Tetris OPENGL by PaulekLab";

TetrisGame game;

GLFWwindow* window;
GLFWwindow* setupOpenGL();

irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

std::vector<PlayerScore> scores;


void saveRanking();

void loadRanking();

void drawFrame(Drawer& drawer, TetrisGame game);

bool consoleHandler();

void initGame(bool enchanced, bool custom);

std::vector<FragmentGlyph> loadFont(const std::string& path, unsigned int glyphHeight);

Texture loadTexture(std::string path);
