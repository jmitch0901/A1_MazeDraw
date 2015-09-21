#ifndef _VIEW_H_
#define _VIEW_H_

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <GL/GL.h>
#include <string>
#include "Maze.h"
#include "MazeController.h"
using namespace std;

#include <glm/glm.hpp>


class View{
	#define BUFFER_OFFSET(offset) ((void *)(offset))

	typedef struct {
        GLenum       type; //is it a vertex shader, a fragment shader, a geometry shader, a tesselation shader or none of the above?
        string  filename; //the file that stores this shader
        GLuint       shader; //the ID for this shader after it has been compiled
    } ShaderInfo;

	

public:
	View();
	~View();
	float worldWindowLeft,worldWindowTop,worldWindowRight,worldWindowBottom;
	void resize(int w, int h);
	void initializeViewFromMaze(Maze &maze);
	void draw();

	void getOpenGLVersion(int *major, int *minor);
	void getGLSLVersion(int *major, int *minor);

	void initTransparentRect(int x1, int y1);
	void scaleTransparentRect(int x1, int y1, int x2, int y2);

	void stopDrawingRect();
	

protected:
	
	GLuint linkShadersToGPU(ShaderInfo* shaders);
	void printShaderInfoToLog(GLuint shader);

	static const int VAO_MAZE_INDEX = 0;
	static const int VAO_DRAWABLE_RECT_INDEX = 1;

	

private:
	float PADDING;
	

	bool isDrawingRect;

	float x1,y1,x2,y2;

	int WINDOW_WIDTH, WINDOW_HEIGHT;

	MazeController* mazeController;

	float aspectRatio;

	GLuint programID;

	GLuint vao;
	GLuint vbo[2];

	glm::mat4 proj,modelView,modelViewDrawableRect;

	GLuint NUM_VERTICES;

	GLint projectionLocation, modelViewLocation, vPositionLocation, vColorLocation;

};





#endif