#include "MazeController.h"
#include <iostream>



MazeController::MazeController(Maze& maze, const int WINDOW_WIDTH, const int WINDOW_HEIGHT){
	this->maze=&maze;
	this->windowHeight=WINDOW_HEIGHT;
	this->windowWidth=WINDOW_WIDTH;

	drawableReactangleVerticesArePushed = false;
	//offsetForDrawableRect = 0;
	
	initializeVertices();
	initializeIndexArray();

}

void MazeController::initializeVertices(){

	aspectRatio=(float)windowWidth/windowHeight;
	cout<<"Aspect Ratio: "<<aspectRatio<<endl;

	const int COLUMN_COUNT = maze->getColumnCount();
	const int ROW_COUNT = maze->getRowCount();

	const float Z_POS = 0;
	const float W_POS = 1.0f;

	if(COLUMN_COUNT >= ROW_COUNT){

		columnPadding=1.0f/(float)COLUMN_COUNT;
		rowPadding=1.0f/(float)COLUMN_COUNT;
	} else {

		columnPadding=1.0f/(float)ROW_COUNT;
		rowPadding=1.0f/(float)ROW_COUNT;
	}


	for(int i = 0; i <= ROW_COUNT; i++){
		for(int j = 0; j <= COLUMN_COUNT; j++){

			VertexAttribs newVertex;

			newVertex.position[0]=(j)*columnPadding;
			newVertex.position[1]=(i)*rowPadding;
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
				indeces.push_back(thisVertexIndex);
				indeces.push_back(bottomVertexIndex);
			}
			

			//top bit
			if((INT_CODE & 4) == 4){
				indeces.push_back(thisVertexIndex);
				indeces.push_back(rightVertexIndex);
			}

			

			//right bit
			if(j == (COLUMN_COUNT-1)){
				//cout<<"Adding right indeces to be drawn"<<endl;
				if((INT_CODE & 2) == 2){
					indeces.push_back(rightVertexIndex);
					indeces.push_back(bottomVertexIndex+1);
				}
			}
			

			//bottom bit
			if(i == (ROW_COUNT-1)){
				//cout<<"Adding bottom indeces to be drawn"<<endl;
				if((INT_CODE & 1) == 1){
					indeces.push_back(bottomVertexIndex);
					indeces.push_back(bottomVertexIndex+1);
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
	return &indeces[0];
}

int MazeController::getIndecesListSize() const{
	return indeces.size();
}



void MazeController::printVerticeDebugList() const{
	
}

MazeController::~MazeController(){
	cout<<"Deconstructing Maze Controller"<<endl;
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



/*void MazeController::pushCoordsToDrawableRect(float startX, float startY, float endX, float endY){

	//drawableRectVertices.clear();
	VertexAttribs upLeft;
	upLeft.position[0] = startX;
	upLeft.position[1] = startY;
	upLeft.position[2] = 0;
	upLeft.position[3] = 1.0f;

	for(int i = 0; i < 3; i++){
		if(i==2){
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
		if(i==2){
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
		if(i==2){
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
		if(i==2){
			bottomLeft.colors[i] = 1;
		} else {
			bottomLeft.colors[i] = 0;
		}
	}
	
	for(int i = 0; i < 4; i++){
		verticesForDrawableRect[verticesForDrawableRect.size() -4].position[i] = upLeft.position[i];
		verticesForDrawableRect[verticesForDrawableRect.size() -3].position[i] = upRight.position[i];
		verticesForDrawableRect[verticesForDrawableRect.size() -2].position[i] = bottomRight.position[i];
		verticesForDrawableRect[verticesForDrawableRect.size() -1].position[i] = bottomLeft.position[i];
	}


	//cout<<"Vertices List Size ->"<<verticesForDrawableRect.size()<<endl;
	

	//cout<<"adding indices"<<endl;
	//indeces.push_back(vertices.size() - 2);
	//indeces.push_back(vertices.size() - 1);
	for(int i = 0; i < 4 ; i++){
		indicesForDrawableRect.push_back(i);

		//cout<<"Pushing index->"<<i<<endl;


		if(i!=3){
			indicesForDrawableRect.push_back(i+1);
			//cout<<"Pushing index->"<<(i+1)<<endl;
		} else {
			indicesForDrawableRect.push_back(0);
			//cout<<"Pushing index->"<<0<<endl;
		}

	}
	drawableReactangleVerticesArePushed=true;
}*/

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

