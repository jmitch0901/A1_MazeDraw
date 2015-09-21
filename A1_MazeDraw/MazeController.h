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
	vector<GLuint> indices;

	//vector<VertexAttribs> verticesForDrawableRect;
	//vector<GLuint> indicesForDrawableRect;

	float aspectRatio;

	float ROW_VERTEX_SPACING;
	float COLUMN_VERTEX_SPACING;

	
	void initializeVertices();
	void initializeIndexArray();
	
	bool drawableReactangleVerticesArePushed;

public:
	MazeController(Maze& maze,float aspectRatio);
	~MazeController();

	int getByteCountForBuffer() const;
	VertexAttribs *getReferenceToArrayStart();
	GLuint* getPointerToIndeces();
	int getIndecesListSize() const;

	//int getByteCountForRectBuffer() const;
	//VertexAttribs *getReferenceToRectArrayStart();
	//GLuint* getPointerToIndecesForRect();
	//int getIndecesRectListSize();

	void notifyAspectRationChanged(float aspectRatio);

	void pushCoordsToDrawableRect(float xPixel, float yPixel);
	void stopDrawingCoordsForRect();

	void printVerticeDebugList() const;







	
	

};







#endif