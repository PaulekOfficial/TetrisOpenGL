#include "Drawer.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <ostream>
#include "shaders.h"

Drawer::Drawer(const glm::mat4& projection, GLfloat tileSize, GLfloat x, GLfloat y, int nRows, int nCols, const std::vector<Texture>& texturess, const std::vector<Texture>& imaginaryTexturess, const std::vector<FragmentGlyph>& font, bool enchanced) :
elementSize(tileSize), x_(x), y_(y), rows(nRows), cols(nCols), textures(texturess), imaginaryTextures(imaginaryTexturess), backgroundShader(primitiveVertexShader, primitiveFragmentShader),
font(font), shader_(vertexShader, glyphShader), elementShader(velementShader, fragmentShader), enchanced(enchanced)
{
	backgroundShader.use();
    backgroundShader.setMat4("projection", projection);

    GLfloat width = cols * elementSize;
    GLfloat height = rows * elementSize;

    verticesBackground = { x_, y_,
                           x_, y_ + height,
                           x_ + width, y_,
                           x_ + width, y_ + height };

    GLfloat yGrid = y_;
    for (int row = 0; row < rows + 1; ++row) {
        verticesBackground.push_back(x_);
        verticesBackground.push_back(yGrid);
        verticesBackground.push_back(x_ + width);
        verticesBackground.push_back(yGrid);
        yGrid += elementSize;
    }

    GLfloat xGrid = x_;
    for (int col = 0; col < cols + 1; ++col) {
        verticesBackground.push_back(xGrid);
        verticesBackground.push_back(y_);
        verticesBackground.push_back(xGrid);
        verticesBackground.push_back(y_ + height);
        xGrid += elementSize;
    }

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vaoBackground);

    glBindVertexArray(vaoBackground);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesBackground.size() * sizeof(GLfloat), verticesBackground.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    GLfloat vertices[] = { 0, 0, 0, 1,
                      0, 1, 0, 0,
                      1, 0, 1, 1,
                      1, 1, 1, 0 };
    
    GLuint vbo2;
    glGenBuffers(1, &vbo2);
    
    glGenVertexArrays(1, &elementVao);
    glBindVertexArray(elementVao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    elementShader.use();
    elementShader.setMat4("projection", projection);

    // setup text
    shader_.use();
    shader_.setMat4("projection", projection);

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Drawer::renderBackground() const
{
    backgroundShader.use();
    glBindVertexArray(vaoBackground);

    backgroundShader.setVec3("inColor", backgroundColor);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    backgroundShader.setVec3("inColor", gridColor);
    glDrawArrays(GL_LINES, 4, 2 * (rows + cols + 2));
}

const glm::vec3 kColorBlack(0, 0, 0);

void Drawer::renderElementsOnBoard(const GameBoard& gameBoard, GLfloat alphaMultiplier) const
{
    auto elements = gameBoard.getElements();

    for (int row = 0; row < elements.size(); row ++)
    {
	    for (int col = 0; col < elements.at(row).size(); col++)
	    {
            auto color = elements.at(row).at(col);
            if (color == Element::empty)
            {
	            continue;
            }

            // Render block
            Texture texture = textures.at(color);
            texture.bind();
            elementShader.use();
            if (enchanced)
            {
                elementShader.setVec2("shift", glm::vec2(x_ + col * elementSize, y_ + (rows - 1 - row) * elementSize));
            }
            else
            {
                elementShader.setVec2("shift", glm::vec2(x_ + col * elementSize, y_ + row * elementSize));
            }
            elementShader.setVec2("scale", glm::vec2(elementSize, elementSize));
            elementShader.setFloat("mixCoeff", 0.0f);
            elementShader.setVec3("mixColor", kColorBlack);
            elementShader.setFloat("alphaMultiplier", alphaMultiplier);
            glBindVertexArray(elementVao);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glBindVertexArray(0);
	    }
    }
}

void Drawer::renderElement(const Element& element, int row, int col, double lockPercent, double alphaMultiplier) const
{
    GLfloat mixCoeff = 0.5f * sin(M_PI_2 * lockPercent);
    if (enchanced)
    {
        renderShape(element, x_ + col * elementSize, y_ + (rows - row - element.getShape().size()) * elementSize, mixCoeff, kColorBlack, alphaMultiplier, false);
        return;
    }
    renderShape(element, x_ + col * elementSize, y_ + row * elementSize, mixCoeff, kColorBlack, alphaMultiplier, false);
}

void Drawer::renderNextElement(const Element& element, double lockPercent, double alphaMultiplier) const
{
    auto size = (((7 * 32 + 6) - element.getShape().at(0).size() * elementSize) / 2);

    GLfloat mixCoeff = 0.5f * sin(M_PI_2 * lockPercent);
    renderShape(element, size, 100, mixCoeff, kColorBlack, alphaMultiplier, false);
}

void Drawer::renderShape(const Element& element, GLfloat x, GLfloat y, GLfloat mixCoeff, const glm::vec3& mixColor, GLfloat alphaMultiplier, bool imaginary) const
{
    if (element.getColor() == Element::Color::empty)
        return;

    auto shape = element.getShape();

    if (enchanced)
    {
        // Swap
        std::map<int, std::vector<int>> cmap;
        for (int i = 0; i < shape.size(); i++)
        {
            std::vector<int> v;
            for (int ii = shape.at(i).size() - 1; ii >= 0; ii--)
            {
                v.push_back(shape.at(i).at(ii));
            }

            cmap[shape.size() - 1 - i] = v;
        }

        // Mirror
		for (int i = 0; i < cmap.size(); i++)
		{
		    std::vector<int> v;
		    for (int ii = cmap.at(i).size() - 1; ii >= 0; ii--)
		    {
		        v.push_back(cmap.at(i).at(ii));
		    }

            cmap.at(i) = v;
		}

        shape = cmap;
    }
    

    for (int row = 0; row < shape.size(); row++)
    {
        for (int column = 0; column < shape.at(row).size(); column++)
        {
            if (shape.at(row).at(column) == 0)
            {
	            continue;
            }

            renderFragment(element.getColor(), x + column * elementSize, y + row * elementSize, mixCoeff, mixColor, alphaMultiplier, imaginary);
        }
    }
}

void Drawer::renderFragment(const Element::Color& element, GLfloat x, GLfloat y, GLfloat mixCoeff, const glm::vec3& mixColor, GLfloat alphaMultiplier, bool imaginary) const
{
    if (element == Element::Color::empty)
        return;

    if (imaginary)
    {
        imaginaryTextures.at(element).bind();
    }
    else
    {
        textures.at(element).bind();
    }
    elementShader.use();
    elementShader.setVec2("shift", glm::vec2(x, y));
    elementShader.setVec2("scale", glm::vec2(elementSize, elementSize));
    elementShader.setFloat("mixCoeff", mixCoeff);
    elementShader.setVec3("mixColor", mixColor);
    elementShader.setFloat("alphaMultiplier", alphaMultiplier);
    glBindVertexArray(elementVao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
}

void Drawer::renderImaginaryElement(const Element& element, int row, int col) const
{
    if (enchanced)
    {
        renderShape(element, x_ + col * elementSize, y_ + (rows - row - element.getShape().size()) * elementSize, 0, kColorBlack, 0.8, true);
        return;
    }
    renderShape(element, x_ + col * elementSize, y_ + row * elementSize, 0, kColorBlack, 0.8, true);
}


void Drawer::playLinesClearAnimation(const GameBoard gameBoard, std::vector<int> clearRows, double percentFinished) const
{
    double t = 0.3;
    
    glm::vec3 mixColor;
    GLfloat mixCoeff;
    
    if (percentFinished < t) {
        double s = sin(M_PI * percentFinished / t);
        mixColor = glm::vec3(1, 1, 1);
        mixCoeff = 0.8f * s;
    } else {
        mixColor = backgroundColor;
        mixCoeff = (percentFinished - t) / (1 - t);
    }

    for (int row : clearRows) {
        for (int col = 0; col < cols; col++) {
            GLfloat x = x_ + col * elementSize;
            GLfloat y = y_ + row * elementSize;

            if (enchanced)
            {
                y = y_ + (rows - row) * elementSize;
            }
            renderFragment(gameBoard.getElements().at(row).at(col), x, y, mixCoeff, mixColor, 1, false);
        }
    }
}

void Drawer::drawText(const std::string& text, GLfloat x, GLfloat y, glm::vec3 color) const
{
    shader_.use();
    shader_.setVec3("textColor", color);
    glBindVertexArray(vao_);

    x = std::round(x);
    y = std::round(y);

    for (char c : text) {
        FragmentGlyph glyph = font.at(c);

        GLfloat xBbox = x + glyph.bearing.x;
        GLfloat yBbox = y + (font.at('A').bearing.y - glyph.bearing.y);

        GLfloat width = glyph.texture.width;
        GLfloat height = glyph.texture.height;

        GLfloat vertices[] = { xBbox, yBbox, 0, 0,
                              xBbox, yBbox + height, 0 ,1,
                              xBbox + width, yBbox, 1, 0,
                              xBbox + width, yBbox + height, 1, 1 };

        glyph.texture.bind();
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        x += glyph.advance;
    }

    glBindVertexArray(0);
}


void Drawer::drawTextCentered(const std::string& text, GLfloat x, GLfloat y, GLfloat width, const glm::vec3& color) const
{
    GLfloat textWidth = computeTextWidth(text);
    GLfloat shift = 0.5f * (width - textWidth);
    drawText(text, std::round(x + shift), std::round(y), color);
}


GLint Drawer::computeTextWidth(const std::string& text) const
{
    GLint width = 0;
    for (auto c = text.begin(); c != text.end() - 1; ++c) {
        width += font.at(*c).advance;
    }
    width += font.at(text.back()).texture.width;
    return width;
}


GLint Drawer::computeTextHeight(const std::string& text) const
{
    GLint height = 0;
    for (char c : text) {
        FragmentGlyph glyph = font.at(c);
        GLint textureHeight = static_cast<GLint>(glyph.texture.height);
        height = std::max(height, font.at('H').bearing.y - glyph.bearing.y + textureHeight);
    }
    return height;
}
