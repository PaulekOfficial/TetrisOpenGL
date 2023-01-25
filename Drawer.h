#pragma once
#include <vector>
#include "structs.h"
#include <glm/gtc/matrix_transform.hpp>

#include "GameBoard.h"

#include <string>

class Drawer
{
public:
    Drawer(const glm::mat4& projection, GLfloat tileSize, GLfloat x, GLfloat y, int nRows, int nCols, const std::vector<Texture>& texturess, const std::vector<Texture>& imaginaryTexturess, const std::vector<FragmentGlyph>& font, bool enchanced);

    void renderBackground() const;
    void renderElementsOnBoard(const GameBoard& gameBoard, GLfloat alphaMultiplier = 1) const;

    void renderElement(const Element& element, int row, int col, double lockPercent, double alphaMultiplier = 1) const;

    void renderNextElement(const Element& element, double lockPercent, double alphaMultiplier = 1) const;

    void renderShape(const Element& element, GLfloat x, GLfloat y, GLfloat mixCoeff, const glm::vec3& mixColor, GLfloat alphaMultiplier, bool imaginary) const;
    void renderFragment(const Element::Color& element, GLfloat x, GLfloat y, GLfloat mixCoeff, const glm::vec3& mixColor, GLfloat alphaMultiplier, bool imaginary) const;

    void renderImaginaryElement(const Element& element, int row, int col) const;

    void playLinesClearAnimation(const GameBoard gameBoard, std::vector<int> clearRows, double percentFinished) const;
    void playExplosionAnimation(const GameBoard& gameBoard, double percentFinished) const;

    void drawText(const std::string& text, GLfloat x, GLfloat y, glm::vec3 color) const;
    void drawTextCentered(const std::string& text, GLfloat x, GLfloat y, GLfloat width, const glm::vec3& color) const;

    GLint computeTextWidth(const std::string& text) const;
    GLint computeTextHeight(const std::string& text) const;

private:
    GLfloat elementSize;
    GLfloat x_, y_;
    int rows, cols;

    const std::vector<Texture> textures;
    const std::vector<Texture> imaginaryTextures;

    Shader backgroundShader;
    std::vector<GLfloat> verticesBackground;
    GLuint vaoBackground;

    Shader elementShader;
    GLuint elementVao;

    std::vector<FragmentGlyph> font;
    Shader shader_;
    GLuint vbo_, vao_;

    bool enchanced;
};
