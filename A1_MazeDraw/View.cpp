#include "View.h"
#include <GL/glew.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

//glm headers to access various matrix producing functions, like ortho below in resize
#include <glm/gtc/matrix_transform.hpp>
//the glm header required to convert glm objects into normal float/int pointers expected by OpenGL
//see value_ptr function below for an example
#include <glm/gtc/type_ptr.hpp>




View::View(){
	PADDING=0.1f;
	mazeController=NULL;
	isDrawingRect=false;
}

View::~View(){
	cout<<"Deconstructing the View"<<endl;
	if(mazeController!=NULL){
		delete mazeController;
	}
}

void View::resize(int w, int h){
	WINDOW_WIDTH=w;
	WINDOW_HEIGHT=h;
	aspectRatio = w/(float)h;

	if(mazeController!=NULL)
		mazeController->notifyAspectRationChanged(aspectRatio);

	//The World-Window
	if(aspectRatio>=1.0f){
		worldWindowTop = 0.0f;
		worldWindowBottom = 1.0f;
		worldWindowLeft = 0.0f + (1.0f - aspectRatio)/2;
		worldWindowRight = 1.0f - worldWindowLeft;

		//worldWindowLeft-=PADDING*aspectRatio;
		//worldWindowRight+=PADDING* aspectRatio;


	} else {
		worldWindowTop = 0.0f + (1.0f - (1.0f/aspectRatio))/2;
		worldWindowBottom = 1.0f - worldWindowTop;
		worldWindowLeft = 0.0f;
		worldWindowRight = 1.0f;

		//worldWindowTop-=PADDING/aspectRatio;
		//worldWindowBottom+=PADDING/aspectRatio;

	}

	
	proj = glm::ortho(worldWindowLeft,worldWindowRight,worldWindowBottom,worldWindowTop);
}

void View::initializeViewFromMaze(Maze &maze){

	if(!maze.successfullyConstructed()){
		cout<<"Can't initialize view because the maze wasn't constructed correctly!"<<endl;
		return;
	}

	ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER,"default.vert"},
        {GL_FRAGMENT_SHADER,"default.frag"},
        {GL_NONE,""}
    };

	programID = linkShadersToGPU(shaders);

	mazeController = new MazeController(maze,aspectRatio);
	
	glUseProgram(programID);

	projectionLocation = glGetUniformLocation(programID,"projection");
    modelViewLocation = glGetUniformLocation(programID,"modelview");

    vPositionLocation = glGetAttribLocation(programID,"vPosition");
    vColorLocation = glGetAttribLocation(programID,"vColor");

	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	glGenBuffers(2,&vbo[0]);

	glBindVertexArray(vao);
	glGenBuffers(2,vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);

	//After binding the VAo and VBo, push the Maze Drawable vertices from MazeController to Vbo.
	glBufferData(GL_ARRAY_BUFFER,mazeController->getByteCountForBuffer(),mazeController->getReferenceToArrayStart(),GL_STATIC_DRAW);

	//Do the same for the indeces array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint)*mazeController->getIndecesListSize(), mazeController->getPointerToIndeces(),GL_STATIC_DRAW);


	//Tell the shaders where we can link the position and memory
	glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
	glVertexAttribPointer(vPositionLocation,4,GL_FLOAT,GL_FALSE,sizeof(VertexAttribs),BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPositionLocation);
	glVertexAttribPointer(vColorLocation,3,GL_FLOAT,GL_FALSE,sizeof(VertexAttribs),BUFFER_OFFSET(4*sizeof(GLfloat)));
    glEnableVertexAttribArray(vColorLocation);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	glUseProgram(0);
	
	cout<<"Error after init: "<<glGetError()<<endl;

}

void View::draw(){

	glUseProgram(programID);

	//How We scale the maze itself
	modelView = glm::scale(glm::mat4(1.0),glm::vec3(1,1,1));
	
    glBindVertexArray(vao);

	
	glUniformMatrix4fv(projectionLocation,1,GL_FALSE,glm::value_ptr(proj));

		glUniformMatrix4fv(modelViewLocation,1,GL_FALSE,glm::value_ptr(modelView));
	
	//Giver a good offset later, when we contain all data inside one VBo
	
	//glDrawArrays(GL_POINTS, 0, 88);

	if(isDrawingRect){

		//Draws the maze only
		glDrawElements(GL_LINES,mazeController->getIndecesListSize()-8,GL_UNSIGNED_INT,BUFFER_OFFSET(0));

		
		//Draws your drawable rect only
		glm::mat4 rectangleProj = glm::ortho(0.0f,1.0f,1.0f,0.0f);

		//First, we want our drawing grid to be the whole screen again.
		glUniformMatrix4fv(projectionLocation,1,GL_FALSE,glm::value_ptr(rectangleProj));


		//Create transformation to scale rectangle
		/*glm::mat4 rectangleTransorm = 	
			 
			 glm::scale(glm::mat4(1.0f),glm::vec3(x2/x1,y2/y1,1.0f))
			 ;

		glUniformMatrix4fv(modelViewLocation,1,GL_FALSE,glm::value_ptr(rectangleTransorm));*/



		glDrawElements(GL_LINES,8,GL_UNSIGNED_INT,BUFFER_OFFSET((sizeof(GLuint)*mazeController->getIndecesListSize()) - (sizeof(GLuint)*8)));

	} else {
		
		

		glDrawElements(GL_LINES,mazeController->getIndecesListSize(),GL_UNSIGNED_INT,BUFFER_OFFSET(0));
	}

	/*if(isDrawingRect){
		//glBindVertexArray(vao[1]);

		modelViewDrawableRect = 
			glm::translate(glm::mat4(1.0),glm::vec3(x1/x2*2,y2/y1*2,1.0f)) 
			* glm::scale(glm::mat4(1.0),glm::vec3((x1/x2)*2,(y2/y1*2),1.0f));//....?

		glUniformMatrix4fv(projectionLocation,1,GL_FALSE,glm::value_ptr(glm::ortho(0.0f,1.0f,1.0f,0.0f)));
		glUniformMatrix4fv(modelViewLocation,1,GL_FALSE,glm::value_ptr(modelViewDrawableRect));
		
		glDrawElements(GL_LINES,mazeController->getIndecesRectListSize(),GL_UNSIGNED_INT,BUFFER_OFFSET(0));

		
	}*/

	glBindVertexArray(0);

	glFlush();

	glUseProgram(0);

}

void View::initTransparentRect(int xPixel, int yPixel){

	mazeController->pushCoordsToDrawableRect((float)xPixel, (float)yPixel,(float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);

	isDrawingRect=true;
	reload();
}

void View::stopDrawingRect(){


	isDrawingRect=false;
	mazeController->stopDrawingCoordsForRect();
	reload();

}

void View::reload(){
	glUseProgram(programID);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,mazeController->getByteCountForBuffer(),mazeController->getReferenceToArrayStart(),GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint)*mazeController->getIndecesListSize(), mazeController->getPointerToIndeces(),GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glBindVertexArray(0);

	glUseProgram(0);
}

void View::scaleTransparentRect(int x1, int y1, int x2, int y2){
	this->x1=x1;
	this->x2=x2;
	this->y1=y1;
	this->y2=y2;

	mazeController->onRectangleDragged(x2,y2,WINDOW_WIDTH,WINDOW_HEIGHT);
	reload();


}



void View::getOpenGLVersion(int *major, int *minor){
	const char *verstr = (const char *)glGetString(GL_VERSION);
    if ((verstr == NULL) || (sscanf_s(verstr,"%d.%d",major,minor)!=2))
    {
        *major = *minor = 0;
    }
}

void View::getGLSLVersion(int *major, int *minor){
	int gl_major,gl_minor;

    getOpenGLVersion(&gl_major,&gl_minor);
    *major = *minor = 0;

    if (gl_major==1)
    {
        /* GL v1.x can only provide GLSL v1.00 as an extension */
        const char *extstr = (const char *)glGetString(GL_EXTENSIONS);
        if ((extstr!=NULL) && (strstr(extstr,"GL_ARB_shading_language_100")!=NULL))
        {
            *major = 1;
            *minor = 0;
        }
    }
    else if (gl_major>=2)
    {
        /* GL v2.0 and greater must parse the version string */
        const char *verstr = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
        if ((verstr==NULL) || (sscanf_s(verstr,"%d.%d",major,minor) !=2))
        {
            *major = 0;
            *minor = 0;
        }
    }
}



GLuint View::linkShadersToGPU(ShaderInfo* shaders){
	ifstream file;
	GLuint shaderProgram;
    GLint linked;

	shaderProgram=glCreateProgram();

	while(shaders->type != GL_NONE){

		file.open(shaders->filename.c_str());
		GLint compiled;

		if(!file.is_open())
			return false;

		string source,line;

		getline(file,line);
		while(!file.eof()){
			source = source+"\n"+line;
			getline(file,line);
		}
		file.close();

		const char *codev = source.c_str();

		shaders->shader = glCreateShader(shaders->type);
		glShaderSource(shaders->shader,1,&codev,NULL);
		glCompileShader(shaders->shader);
		glGetShaderiv(shaders->shader,GL_COMPILE_STATUS,&compiled);

		if(!compiled){
			printShaderInfoToLog(shaders->shader);
            for (ShaderInfo *processed = shaders;processed->type!=GL_NONE;processed++)
            {
                glDeleteShader(processed->shader);
                processed->shader = 0;
            }
            return 0;

		}

		glAttachShader( shaderProgram, shaders->shader );
        shaders++;
	}

	glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&linked);

    if (!linked)
    {
        printShaderInfoToLog(shaders->shader);
        for (ShaderInfo *processed = shaders;processed->type!=GL_NONE;processed++)
        {
            glDeleteShader(processed->shader);
            processed->shader = 0;
        }
        return 0;
    }

	cout<<"GL ERROR FOR SHADERS: "<<glGetError()<<endl;

	return shaderProgram;

}
void View::printShaderInfoToLog(GLuint shader){
	int infologLen = 0;
    int charsWritten = 0;
    GLubyte *infoLog;

    glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infologLen);
    //	printOpenGLError();
    if (infologLen>0)
    {
        infoLog = (GLubyte *)malloc(infologLen);
        if (infoLog != NULL)
        {
            glGetShaderInfoLog(shader,infologLen,&charsWritten,(char *)infoLog);
            printf("InfoLog: %s\n\n",infoLog);
            free(infoLog);
        }

    }
//	printOpenGLError();
}

/*

float aspectRatio = WINDOW_WIDTH/(float)WINDOW_HEIGHT;

	/*cout<<"Initializing the drawable Rect"<<endl;
	const float PADDING = 0.1f;
	//Convert each value into a ratio between 0->1 (our drawable surface boundaries)!
	//These are my percentages of WHERE the mouse is one the screen-window
	float xDiff = (WINDOW_WIDTH - smallEdge)/2;
	float yDiff = (WINDOW_HEIGHT - smallEdge)/2;

	float xInitial = -1*((xDiff + (float)smallEdge)/2 - x1 + (xDiff))/(smallEdge/2);

	float yInitial = ((yDiff + (float)smallEdge)/2 - y1 + (yDiff))/(smallEdge/2);
	cout << "Xinitial: " << xInitial << endl;
	cout << "YInitial: " << yInitial << endl;
	
	float xScaleStart = (xPixel)/(float)(WINDOW_WIDTH);
	float yScaleStart = (yPixel)/(float)(WINDOW_HEIGHT);
	//float yScaleStart = ((WINDOW_HEIGHT-smallEdge)/2 +smallEdge/2-yPixel)/(smallEdge/2);
	float yScaleEnd = (yPixel)/(float)WINDOW_HEIGHT;
	float xScaleEnd = (xPixel+1)/(float)WINDOW_WIDTH;
	//float yScaleEnd = (yPixel+1)/(float)WINDOW_HEIGHT;
	//float yScaleEnd = ((WINDOW_HEIGHT-smallEdge)/2 +smallEdge/2-(yPixel+1))/(smallEdge/2);

	cout<<"(xScaleStart, yScaleStart, xScaleEnd, yScaleEnd) -> ("<<xScaleStart<<", "<<yScaleStart<<", "<<xScaleEnd<<", "<<yScaleEnd<<")"<<endl;

	
	if(w>=h){
		worldWindowTop = 0.0f - PADDING;
		worldWindowBottom = 1.0f + PADDING;
		worldWindowLeft = 0.0f + (1.0f - aspectRatio)/2;
		worldWindowRight = 1.0f - worldWindowLeft;

		worldWindowLeft-=PADDING*aspectRatio;
		worldWindowRight+=PADDING* aspectRatio;

	} else {
		worldWindowTop = 0.0f + (1.0f - (1.0f/aspectRatio))/2;
		worldWindowBottom = 1.0f - worldWindowTop;
		worldWindowLeft = 0.0f - PADDING;
		worldWindowRight = 1.0f + PADDING;

		worldWindowTop-=PADDING/aspectRatio;
		worldWindowBottom+=PADDING/aspectRatio;
	}
	
	//I need to convert this ratio to the world window coordinates...

	float left,right,up,down;
	if(WINDOW_WIDTH>=WINDOW_HEIGHT){
		//yScaleStart = (yPixel-(0.9f*yPixel)) / (float)(WINDOW_HEIGHT - (0.9f * WINDOW_HEIGHT));
		//yScaleStart = 
		//yScaleEnd = (0.1f * (yPixel+1)) / (0.1f * WINDOW_HEIGHT);
	} else {

	}

	//mazeController->pushCoordsToDrawableRect(xScaleStart, xScaleEnd, yScaleStart, yScaleEnd);
	mazeController->pushCoordsToDrawableRect(xScaleStart, yScaleStart, xScaleEnd, yScaleEnd);


*/