#ifndef _MY_MAZE_CONTROLLER_
#define _MY_MAZE_CONTROLLER_

#include "Maze.h"
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>



/*
	Each maze cell must hold 4 drawable vertices.
	Each vertex requires 4 coordinate values;
*/
	/*
		(First Index) we need 4 vertices to draw the actual cell.
		(Second Index) we need to know the coordinates of the each vertex itself.
	*/
class VertexAttribs{
	friend class MazeController;
private:
	float position[4];
	float colors[3];

};


/*
	Used as the middle man between the view and the raw parceable data. 
	TRanslated the parcebal data into drawable vertices, which then passes to the View.
*/
class MazeController{
private:

	//int offsetForDrawableRect;

	Maze *maze;
	vector<VertexAttribs> vertices;
	vector<GLuint> indeces;

	//vector<VertexAttribs> verticesForDrawableRect;
	//vector<GLuint> indicesForDrawableRect;


	int windowWidth, windowHeight;
	float aspectRatio;

	float rowPadding;
	float columnPadding;

	
	void initializeVertices();
	void initializeIndexArray();
	
	bool drawableReactangleVerticesArePushed;

public:
	MazeController(Maze& maze, const int windowWidth, const int windowHeight);
	~MazeController();

	int getByteCountForBuffer() const;
	VertexAttribs *getReferenceToArrayStart();
	GLuint* getPointerToIndeces();
	int getIndecesListSize() const;

	//int getByteCountForRectBuffer() const;
	//VertexAttribs *getReferenceToRectArrayStart();
	//GLuint* getPointerToIndecesForRect();
	//int getIndecesRectListSize();

	//void pushCoordsToDrawableRect(float x1, float y1, float x2, float y2);
	//void stopDrawingCoordsForRect(int x1, int y1, int x2, int y2,float wwl, float wwu, float wwr, float wwd);

	void printVerticeDebugList() const;







	
	

};







#endif