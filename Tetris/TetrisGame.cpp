#include "TetrisGame.h"

const int TetrisGame::BLOCK_WIDTH = 32;
const int TetrisGame::BLOCK_HEIGHT = 32;
const int TetrisGame::NUM_BLOCK_COLORS = 7;
const double TetrisGame::MAX_SECONDS_PER_TICK = 0.75;
const double TetrisGame::MIN_SECONDS_PER_TICK = 0.20;

TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, const Point& gameboardOffset, const Point& nextShapeOffset) : 
    window(window), 
    blockSprite(blockSprite), 
    gameboardOffset(gameboardOffset), 
    nextShapeOffset(nextShapeOffset) {

    //currentShape.setShape(Tetromino::getRandomShape());
    /*currentShape.setGridLoc(board.getSpawnLoc());*/
    //board.setContent(Gameboard::MAX_X / 2, Gameboard::MAX_Y / 2, 1);
    reset();

    if (!scoreFont.loadFromFile("fonts/RedOctober.ttf"))
    {
        //assert(false && "Missing font: RedOctober.ttf");
    };
    scoreText.setFont(scoreFont);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(425, 325);

}

void TetrisGame::draw() {
    //TESTING
 //   Point topLeft = gameboardOffset;  // Start at the top-left corner of the gameboard
 //   int xOffset = 0;
 //   int yOffset = 0;
 //   TetColor color = TetColor::BLUE_DARK;  // Use the RED color for the block
 //   /*for (int i = 0; i < 10; i++) {
 //       for (int j = 0; j < 19; j++) {*/
 //           drawBlock(topLeft, yOffset, xOffset, color);
 ///*       }
 //   }*/
   /* board.setContent(5, 5, 2);
    drawGameboard();*/
    drawGameboard();
    drawTetromino(currentShape, gameboardOffset);

    drawTetromino(nextShape, nextShapeOffset);
    window.draw(scoreText);


}

void TetrisGame::onKeyPressed(const sf::Event& event) {
   

    switch (event.key.code) {
    case sf::Keyboard::Up:
        attemptRotate(currentShape);
        break;
    case sf::Keyboard::Down:
        attemptMove(currentShape, 0, 1);
        break;
    case sf::Keyboard::Left:
        attemptMove(currentShape, -1, 0);
        break;
    case sf::Keyboard::Right:
        attemptMove(currentShape, 1, 0);
        break;
    case sf::Keyboard::Space:
        drop(currentShape);
        break;
    }
}

// called every game loop to handle ticks & tetromino placement (locking)
// - param 1: float secondsSinceLastLoop
// return: nothing
void TetrisGame::processGameLoop(const float secondsSinceLastLoop) {

    secondsSinceLastTick += secondsSinceLastLoop;

    while (secondsSinceLastTick > secondsPerTick) {
        tick();  
        secondsSinceLastTick -= secondsPerTick;
    }
    if (shapePlacedSinceLastGameLoop) {
        shapePlacedSinceLastGameLoop = false;

        if (spawnNextShape()) {
            pickNextShape();
            int cleared = board.removeCompletedRows();
            score += cleared * 100;

            updateScoreDisplay();
            determineSecondsPerTick();
        }
        else{
            reset();
        }   

    }

}


// A tick() forces the currentShape to move (if there were no tick,
// the currentShape would float in position forever). This should
// call attemptMove() on the currentShape.  If not successful, lock() 
// the currentShape (it can move no further).
// - params: none
// - return: nothing
void TetrisGame::tick() {
    std::cout << "tick" << std::endl;

    if (!attemptMove(currentShape, 0, 1)) {
        lock(currentShape);
    };
}



// reset everything for a new game (use existing functions) 
//  - set the score to 0 and call updateScoreDisplay()
//  - call determineSecondsPerTick() to determine the tick rate.
//  - clear the gameboard,
//  - pick & spawn next shape
//  - pick next shape again (for the "on-deck" shape)
// - params: none
// - return: nothing
void TetrisGame::reset() {
    score = 0;
    updateScoreDisplay();
    determineSecondsPerTick();
   board.empty();
    pickNextShape();
    spawnNextShape();
    pickNextShape();
    board.printToConsole();

}



// assign nextShape.setShape a new random shape  
// - params: none
// - return: nothing
void TetrisGame::pickNextShape() {
    TetShape newShape = Tetromino::getRandomShape();
    nextShape.setShape(newShape);
    //nextShape.setGridLoc(board.getSpawnLoc());
}

// copy the nextShape into the currentShape (through assignment)
//   position the currentShape to its spawn location.
// - params: none
// - return: bool, true/false based on isPositionLegal()
bool TetrisGame::spawnNextShape() {

    currentShape = nextShape;
    currentShape.setGridLoc(board.getSpawnLoc());

    if (!isPositionLegal(currentShape)) {
        return false;
    }
    return true;
}

// Test if a rotation is legal on the tetromino and if so, rotate it. 
//  To accomplish this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) rotate it (temp.rotateClockwise())
//	 3) test if temp rotation was legal (isPositionLegal()), 
//      if so - rotate the original tetromino.
// - param 1: GridTetromino shape
// - return: bool, true/false to indicate successful movement
bool TetrisGame::attemptRotate(GridTetromino& shape) {
    // temporary shape
    GridTetromino temp = shape;

    if (temp.getShape() == TetShape::O) {
        return false;
    }
    // Rotate temporary shape
    temp.rotateClockwise();

    if (isPositionLegal(temp)) {
        shape = temp;
        return true;  
    }

    return false;  
}


// test if a move is legal on the tetromino, if so, move it.
//  To do this:
//	 1) create a (local) temporary copy of the tetromino
//	 2) move it (temp.move())
//	 3) test if temp move was legal (isPositionLegal(),
//      if so - move the original.	
// - param 1: GridTetromino shape
// - param 2: int x;
// - param 3: int y;
// - return: true/false to indicate successful movement
bool TetrisGame::attemptMove(GridTetromino& shape, int x, int y) {
    
    GridTetromino temp = shape;

    temp.move(x, y);

    if (isPositionLegal(temp)) {
         
        shape.setGridLoc(temp.getGridLoc());  
        return true;  
    }

    return false;  
}

// drops the tetromino vertically as far as it can 
    //   legally go.  Use attemptMove(). This can be done in 1 line.
    // - param 1: GridTetromino shape
    // - return: nothing;
void TetrisGame::drop(GridTetromino& shape) {
    while (attemptMove(shape, 0, 1)) {}
    lock(shape);
}


// copy the contents (color) of the tetromino's mapped block locs to the grid.
    //	 1) get the tetromino's mapped locs via tetromino.getBlockLocsMappedToGrid()
    //   2) use the board's setContent() method to set the content at the mapped locations.
    //   3) record the fact that we placed a shape by setting shapePlacedSinceLastGameLoop
    //      to true
    // - param 1: GridTetromino shape
    // - return: nothing
void TetrisGame::lock(GridTetromino& shape) {

    auto location = shape.getBlockLocsMappedToGrid();

    for (Point& s : location) 
    {
        board.setContent(s.getX(), s.getY(), shape.getColor());
    }
    shapePlacedSinceLastGameLoop = true;
    board.printToConsole();
}




void TetrisGame::drawBlock(const Point& topLeft, int xOffset, int yOffset, TetColor& color) {
    
    //Positon of the block 
    int positionOnX = topLeft.getX() + (xOffset * BLOCK_WIDTH); // 0 + distance from x * 32
    int positionOnY = topLeft.getY() + (yOffset * BLOCK_HEIGHT);

    //Set texture based on color
    int colorOffset = static_cast<int>(color) * BLOCK_WIDTH;

    blockSprite.setTextureRect(sf::IntRect(colorOffset, 0, BLOCK_WIDTH, BLOCK_HEIGHT));

    //Give position to the block on gameboard
    blockSprite.setPosition(positionOnX, positionOnY);

    // Draw block
    window.draw(blockSprite);

}
void TetrisGame::drawGameboard() {
    
    Point topLeft = gameboardOffset;  //Start Point

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 19; j++) {
            
            //only call drawBlock() if board.getContent(x, y) is not empty
            if (board.getContent(i, j) != Gameboard::EMPTY_BLOCK) {
                    
                TetColor color = static_cast<TetColor>(board.getContent(i, j));
                
                drawBlock(topLeft, i, j, color);
            }
        }
    }
}

// Draw a tetromino on the window
//	 Iterate through each mapped loc & drawBlock() for each.
//   The topLeft determines a 'base point' from which to calculate block offsets
//      If the Tetromino is on the gameboard: use gameboardOffset
// param 1: GridTetromino tetromino
// param 2: Point topLeft
// return: nothing
void TetrisGame::drawTetromino(GridTetromino& tetromino,const Point& topLeft) {

    std::vector<Point> mappedBlocs = tetromino.getBlockLocsMappedToGrid();

    for (const Point& block : mappedBlocs) {

        int x = block.getX();
        int y = block.getY();
        TetColor color = tetromino.getColor();

        drawBlock(topLeft, x, y, color);
    }

}

// update the score display
// form a string "score: ##" to display the current score
// user scoreText.setString() to display it.
// params: none:
// return: nothing
void TetrisGame::updateScoreDisplay() {

        std::string scorePrint = "SCORE: " + std::to_string(score);

        scoreText.setString(scorePrint);
}


// Determine if a Tetromino can legally be placed at its current position
// on the gameboard.
//   Tip: Make use of Gameboard's areLocsEmpty() and pass it the shape's mapped locs.
// - param 1: GridTetromino shape
// - return: bool, true if shape is within borders (isWithinBorders()) and 
//           the shape's mapped board locs are empty (false otherwise).
bool TetrisGame::isPositionLegal(GridTetromino& shape) const {

    std::vector<Point> shapeLocation = shape.getBlockLocsMappedToGrid();

    if (!isWithinBorders(shape)) {
        return false;  
    }

    if (!board.areAllLocsEmpty(shapeLocation)) {
        return false;  // for block locations already occupied
    }

    return true;
}

// Determine if the shape is within the left, right, & bottom gameboard borders
//   * Ignore the upper border because we want shapes to be able to drop
//     in from the top of the gameboard.
//   All of a shape's blocks must be inside these 3 borders to return true
// - param 1: GridTetromino shape
// - return: bool, true if the shape is within the left, right, and lower border
//	         of the grid, but *NOT* the top border (false otherwise)
bool TetrisGame::isWithinBorders(GridTetromino& shape) const {

    std::vector<Point> shapeLocation = shape.getBlockLocsMappedToGrid();

    for (const auto& location : shapeLocation) {
        
        if (location.getX() < 0 || location.getX() >= Gameboard::MAX_X || location.getY() >= Gameboard::MAX_Y) {
            return false; 
        }
    }

    return true;
}

void TetrisGame::determineSecondsPerTick() {

}
