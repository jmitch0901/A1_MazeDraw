#include "MazeController.h"
#include <iostream>



MazeController::MazeController(Maze& maze,float aspectRatio){
	this->maze = &maze;
	this->aspectRatio = aspectRatio;

	recVerticesPushed=false;
	//offsetForDrawableRect = 0;
	
	initializeVertices();
	initializeIndexArray();

}

void MazeController::initializeVertices(){
	cout<<"Aspect Ratio: "<<aspectRatio<<endl;

	const int COLUMN_COUNT = maze->getColumnCount();
	const int ROW_COUNT = maze->getRowCount();

	const float Z_POS = 0;
	const float W_POS = 1.0f;

	if(COLUMN_COUNT >= ROW_COUNT){

		COLUMN_VERTEX_SPACING=1.0f/(float)COLUMN_COUNT;
		ROW_VERTEX_SPACING=1.0f/(float)COLUMN_COUNT;
	} else {

		COLUMN_VERTEX_SPACING=1.0f/(float)ROW_COUNT;
		ROW_VERTEX_SPACING=1.0f/(float)ROW_COUNT;
	}


	for(int i = 0; i <= ROW_COUNT; i++){
		for(int j = 0; j <= COLUMN_COUNT; j++){

			VertexAttribs newVertex;

			newVertex.position[0]=(j)*COLUMN_VERTEX_SPACING;
			newVertex.position[1]=(i)*ROW_VERTEX_SPACING;
			newVertex.position[2]=Z_POS;
			newVertex.position[3]=W_POS;


			//cout<<newVertex.position[0]<<", "<<newVertex.position[1]<<endl;

			//Deal with color logic later!
			for(int j=0; j < 3; j++){
				newVertex.colors[j]=0;
			}

			vertices.push_back(newVertex);
		}

	}


	
}


/*
	Creates the index array so that the lines can be drawn correctly!
	First draws all Horizontal Lines, then vertical.
*/
void MazeController::initializeIndexArray(){

	const int COLUMN_COUNT = maze->getColumnCount();
	const int ROW_COUNT = maze->getRowCount();

	for(int i = 0; i < ROW_COUNT; i++){
		for(int j = 0; j < COLUMN_COUNT; j++){

			int INT_CODE = maze->getCellLogicAsInteger(j,i);



			int thisVertexIndex = i*(COLUMN_COUNT+1)+j;
			int rightVertexIndex = thisVertexIndex+1;
			int bottomVertexIndex = thisVertexIndex+COLUMN_COUNT+1;

			

			
			//left bit
			if((INT_CODE & 8) == 8){
				indices.push_back(thisVertexIndex);
				indices.push_back(bottomVertexIndex);
			}
			

			//top bit
			if((INT_CODE & 4) == 4){
				indices.push_back(thisVertexIndex);
				indices.push_back(rightVertexIndex);
			}

			

			//right bit
			if(j == (COLUMN_COUNT-1)){
				//cout<<"Adding right indeces to be drawn"<<endl;
				if((INT_CODE & 2) == 2){
					indices.push_back(rightVertexIndex);
					indices.push_back(bottomVertexIndex+1);
				}
			}
			

			//bottom bit
			if(i == (ROW_COUNT-1)){
				//cout<<"Adding bottom indeces to be drawn"<<endl;
				if((INT_CODE & 1) == 1){
					indices.push_back(bottomVertexIndex);
					indices.push_back(bottomVertexIndex+1);
				}
			}	
		}
	}
}


int MazeController::getByteCountForBuffer() const{
	return sizeof(VertexAttribs)* vertices.size();
}


VertexAttribs* MazeController::getReferenceToArrayStart(){
	return &vertices[0];
}

GLuint* MazeController::getPointerToIndeces(){
	return &indices[0];
}

int MazeController::getIndecesListSize() const{
	return indices.size();
}



void MazeController::printVerticeDebugList() const{
	
}

MazeController::~MazeController(){
	cout<<"Deconstructing Maze Controller"<<endl;
}


void MazeController::notifyAspectRationChanged(float aspectRatio){
	this->aspectRatio = aspectRatio;
}

void MazeController::onRectangleDragged(float xPixel, float yPixel, float WINDOW_WIDTH, float WINDOW_HEIGHT){
	/*//1
	vertices.push_back(upLeft);

	//2
	vertices.push_back(upRight);

	//3
	vertices.push_back(bottomRight);

	//4
	vertices.push_back(bottomLeft);*/


	int i = vertices.size();

	vertices[i-3].position[0] = xPixel/WINDOW_WIDTH;

	vertices[i-2].position[0] = xPixel/WINDOW_WIDTH;
	vertices[i-2].position[1] = yPixel/WINDOW_HEIGHT;

	vertices[i-1].position[1] = yPixel/WINDOW_HEIGHT;
	
}


void MazeController::pushCoordsToDrawableRect(float xPixel, float yPixel, float WINDOW_WIDTH, float WINDOW_HEIGHT){


	

	float startX, startY, endX, endY = 0.0f;
	startX = (xPixel-1)/WINDOW_WIDTH;
	endX = (xPixel)/WINDOW_WIDTH;
	startY=(yPixel-1)/WINDOW_HEIGHT;
	endY = (yPixel)/WINDOW_HEIGHT;

	//Debug by hardcode
	/*startX = 0.15f;
	startY = 0.15f;
	endX = 0.75f;
	endY = 0.75f;*/


	VertexAttribs upLeft;
	upLeft.position[0] = startX;
	upLeft.position[1] = startY;
	upLeft.position[2] = 0;
	upLeft.position[3] = 1.0f;

	for(int i = 0; i < 3; i++){
		if(i==0){
			upLeft.colors[i] = 1;
		} else {
			upLeft.colors[i]=0;
		}
	}
	
	VertexAttribs upRight;
	upRight.position[0] = endX;
	upRight.position[1] = startY;
	upRight.position[2] = 0;
	upRight.position[3] = 1.0f;

	for(int i = 0; i < 3; i++){
		if(i==0){
			upRight.colors[i] = 1;
		} else {
			upRight.colors[i] = 0;
		}
	}

	VertexAttribs bottomRight;
	bottomRight.position[0] = endX;
	bottomRight.position[1] = endY;
	bottomRight.position[2] = 0;
	bottomRight.position[3] = 1.0f;

	for(int i = 0; i < 3; i++){
		if(i==0){
			bottomRight.colors[i] = 1;
		} else {
			bottomRight.colors[i] = 0;
		}
	}

	VertexAttribs bottomLeft;
	bottomLeft.position[0] = startX;
	bottomLeft.position[1] = endY;
	bottomLeft.position[2] = 0;
	bottomLeft.position[3] = 1.0f;

	for(int i = 0; i < 3; i++){
		if(i==0){
			bottomLeft.colors[i] = 1;
		} else {
			bottomLeft.colors[i] = 0;
		}
	}
	
	//for(int i = 0; i < 4; i++){
	/*if( < vertices.size()){
		for(int i = 0 ; i < 4; i++){
			vertices.pop_back();
		}	
	}
	*/

	//1
	vertices.push_back(upLeft);

	//1
	vertices.push_back(upRight);

	//3
	vertices.push_back(bottomRight);

	//4
	vertices.push_back(bottomLeft);

	for(int i = 1; i <= 4; i++){
		indices.push_back(vertices.size() - i);

		if(i!=4){
			indices.push_back(vertices.size()-(i+1));

		} else {
			indices.push_back(vertices.size()-1);

		}
	}
	
	recVerticesPushed = true;
}

void MazeController::stopDrawingCoordsForRect(float x1, float y1, float x2, float y2, float WINDOW_WIDTH, float WINDOW_HEIGHT){
	
	//Remove indeces and vertices we don't care about
	for(int i = 0 ; i < 8; i++){
		indices.pop_back();
	}

	for(int i = 0; i < 4; i++){
		vertices.pop_back();
	}

	recVerticesPushed=false;

	//Now mess with maze logic
	float xScaleStart = x1/WINDOW_WIDTH;
	float yScaleStart = y1/WINDOW_HEIGHT;
	float xScaleEnd = x2/WINDOW_WIDTH;
	float yScaleEnd = y2/WINDOW_HEIGHT;

	const int COLUMN_COUNT = maze->getColumnCount();
	const int ROW_COUNT = maze->getRowCount();



	for(int i = 1; i < vertices.size(); i++){
		//We start at 1 because were comparing this vertex to it's prev vertex.
		

		if(vertices[i].position[0] >= xScaleStart && vertices[i-1].position[0] <= xScaleStart){
			

		}
		



	}

	/*int colStart, rowStart = -1;

	int colEnd, rowEnd = -1;

	for(int i = 0; i < COLUMN_COUNT; i++){
		if(vertices[i].position[0] < xScaleStart && vertices[i+1].position[0] > xScaleStart){
			colStart = i;
		}

		if(vertices[i].position[0] < xScaleEnd && vertices[i+1].position[0] > xScaleEnd){
			colEnd = i;
			break;
		}
	}*/

	


	



	//Just translate the logic to maze object, and reinitialize the controller
	/*for(int i = 1; i < ROW_COUNT; i++){
		for(int j = 1; j < COLUMN_COUNT; j++){

			/*if(i==0 || i==ROW_COUNT || j==0 || j == COLUMN_COUNT)
				continue;*/



			/*int index = i*(COLUMN_COUNT+1)+j;

			int INT_CODE = 0;

			//left
			if(
				xScaleStart < vertices[index+1].position[0]
				&& xScaleStart > vertices[index].position[0]
				)
				INT_CODE+=8;

			//up
			if(
				
				)
				INT_CODE+=4;

			maze->setCellLogicAsInteger(j,i,INT_CODE);*/


			/*if(
				vertices[index].position[0]>xScaleStart
				&&vertices[index].position[0]<=xScaleEnd
				&&vertices[index].position[1]>yScaleStart
				&&vertices[index].position[1]<=yScaleEnd
				)
			{*/

				


				//Then change the wall logic inside the maze.cpp 
				/*if(
					vertices[index].position[0]-COLUMN_VERTEX_SPACING>=xScaleStart
					&&vertices[index].position[0]+COLUMN_VERTEX_SPACING<=xScaleEnd
					&&vertices[index].position[1]-ROW_VERTEX_SPACING>=yScaleStart
					&&vertices[index].position[1]+ROW_VERTEX_SPACING<=yScaleEnd
					)

				{
					//Then the integer value for the maze at that cell must be 0!
					
					maze->setCellLogicAsInteger(j,i,0);
				} else {

					//Otherwise, we add walls to the cell if needed!

					int INT_CODE = maze->getCellLogicAsInteger(j,i);
					//left
					if((INT_CODE&8)!=8)
						maze->setCellLogicAsInteger(j,i,INT_CODE+8);
					
					INT_CODE = maze->getCellLogicAsInteger(j,i);


					//up
					if((INT_CODE&4)!=4)
						maze->setCellLogicAsInteger(j,i,INT_CODE+4);
					
					
					INT_CODE = maze->getCellLogicAsInteger(j,i);

					//right
					if((INT_CODE&2)!=2)
						maze->setCellLogicAsInteger(j,i,INT_CODE+2);

					INT_CODE = maze->getCellLogicAsInteger(j,i);

					//down
					if((INT_CODE&1)!=1)
						maze->setCellLogicAsInteger(j,i,INT_CODE+1);
					
				}
			//}
		}	
	}*/

}


//GOES INSDIE INITIALIZE VERTICES
//Add a placeHolder for The vertices used for a drawable Rect???
	/*VertexAttribs drawableRectVertices;
	drawableRectVertices.position[0] = 0.0f;//COLUMN_COUNT*columnPadding;
	drawableRectVertices.position[1] = 0.0f;//aspectRatio/ROW_COUNT;
	drawableRectVertices.position[2] = Z_POS;
	drawableRectVertices.position[3] = W_POS;

	for(int i=0; i < 3; i++){
		if(i==0){
			drawableRectVertices.colors[i]=1;
		} else {
			drawableRectVertices.colors[i]=0;
		}
	}

	for(int i = 0; i < 4; i++){
		verticesForDrawableRect.push_back(drawableRectVertices);
	}*/

	//End Placeholder

/*void MazeController::stopDrawingCoordsForRect(int x1, int y1, int x2, int y2,float wwl, float wwu, float wwr, float wwd){
	//cout<<"removing indeces"<<endl;

	

	for(int i = 0; i < 8; i++){
		indicesForDrawableRect.pop_back();
	}

	drawableReactangleVerticesArePushed = false;

	//Start removing the bad indeces
	//The difference depending on which values are larger
	bool x1WasHigh, y1WasHigh;

	float xHigh = (x1WasHigh =(x1 > x2)) ? (float)x1 : (float)x2;
	float yHigh = (y1WasHigh =(y1 > y2)) ? (float)y1 : (float)y2;

	float xLow = x1WasHigh ? (float)x2 : (float)x1;
	float yLow = y1WasHigh ? (float)y2 : (float)y1;

	xLow/=windowWidth;
	xHigh/=windowWidth;
	yHigh/=windowHeight;
	yLow/=windowHeight;



	//cout<<"xLow: "<<xLow<<", xHigh: "<<xHigh<<", yLow: "<<yLow<<", yHigh: "<<yHigh<<endl;

	vector<int> indecesToRemove;
	vector<int> indecesToAdd;

	for(int i = 0; i < vertices.size(); i++){

		if(
			vertices[i].position[0] >= xLow
			&& vertices[i].position[0] <= xHigh
			&& vertices[i].position[1] >= yLow
			&& vertices[i].position[1] <= yHigh
			//Dont forget to check edges
			&& xLow > wwl
			&& xHigh< wwr
			&& yLow> wwu
			&& yHigh< wwd
			)
		{

			//Then remove those indeces entries
			
			indecesToRemove.push_back(i);
			indecesToRemove.push_back(i+1);
		}
	}

	//remove such indeces
	for(int i = 0; i < indecesToRemove.size(); i++){
		int indexEntryToRemove = indecesToRemove[i];
		for(int j = indeces.size()-1; j >= 0; j--){
			if(indeces[j] == indexEntryToRemove){
				cout<<"This needs to be deleted!: "<<indeces[j]<<endl;
				indeces[j]=-1; //Signifier as a entry which needs to be erased
			}
		}
	}

	for(vector<GLuint>::iterator it = indeces.begin(); it!=indeces.end(); it++){
		if(*it<0){//CHECK THIS
			cout<<"Erasing: "<<*it<<endl;
			indeces.erase(it);
		}
	}

	//Add vertex indices for redrawing inner walls

	//N/A





}*/


/*int MazeController::getByteCountForRectBuffer() const{
	return sizeof(VertexAttribs)*verticesForDrawableRect.size();
}
VertexAttribs* MazeController::getReferenceToRectArrayStart(){
	return &verticesForDrawableRect[0];
}
GLuint* MazeController::getPointerToIndecesForRect(){
	return &indicesForDrawableRect[0];
}
int MazeController::getIndecesRectListSize(){
	return indicesForDrawableRect.size();
}*/

