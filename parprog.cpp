/********************************************************************/
/*   Force Directed Placement and Lee Maze Routing Alogirthm        */
/*																    */
/*  Authors :   madhav427                                           */
/*  Date:     April/1/2018                                          */
/********************************************************************/

//Creating a Force Directed Partitioning  
//vector implementation 
//Components are Values, Colomns, row, index

#include <iostream>
#include <vector>
#include <fstream>
#include "math.h"
#include "time.h"
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
/*************************************************************************/
/*Snippet:The script below returns the terminal positions of the given   */
/*		  cell x and y position 		                 */
/*************************************************************************/
unsigned int t1_x(unsigned int x) {
	return x + 1;
}
unsigned int t1_y(unsigned int y) {
	return y - 1;
}
unsigned int t2_x(unsigned int x) {
	return x +  4;
}
unsigned int t2_y(unsigned int y) {
	return y - 1;
}
unsigned int t3_x(unsigned int x) {
	return x + 1;
}
unsigned int t3_y(unsigned int y) {
	return  y + 6;
}
unsigned int t4_x(unsigned int x) {
	 return x + 4;
}
unsigned int t4_y(unsigned int y) {
	return y + 6;
}
unsigned int terminal_y(unsigned int x, char t) {
	switch (t) {
	case '1':
		return t1_y(x);
		break;
	case '2':
		return t2_y(x);
		break;
	case '3':
		return t3_y(x);
		break;
	case '4':
		return t4_y(x);
		break;
	}
}
unsigned int terminal_x(unsigned int y, char t) {
	switch (t) {
	case '1':
		return t1_x(y);
		break;
	case '2':
		return t2_x(y);
		break;
	case '3':
		return t3_x(y);
		break;
	case '4':
		return t4_x(y);
		break;
	}
}
/********************************************************************/
/*   Snippet: Returns a flag to move in a pariticular direction     */
/*	          in the routing layer                              */
/********************************************************************/
// a and b is denoting change made in the direction
// x and y denotes the present cell
bool checkback(bool flag, unsigned int a, unsigned int b, std::vector<std::vector<char> > &route_space_m1, int x, int y, unsigned int count, unsigned int sx, unsigned int sy) {
	if (a >= 0 && a < count && b >= 0 && b < count) {
		if ((a == sx) && (b == sy)) {
			flag = 1 - flag;
			return true;
		}
		else if (route_space_m1[y][x] == '2' && flag == 0) {
			if (route_space_m1[b][a] == '1') {
				flag = 1 - flag;
				return true;
			}
			else return false;
		}
		else if (route_space_m1[y][x] == '2' && flag == 1) {
			if (route_space_m1[b][a] == '2') {
				flag = 1 - flag;
				return true;
			}
			else return false;
		}
		else if (route_space_m1[y][x] == '1' && flag == 0) {
			if (route_space_m1[b][a] == '2') {
				flag = 1 - flag;
				return true;
			}
			else return false;
		}
		else if (route_space_m1[y][x] == '1' && flag == 1) {
			if (route_space_m1[b][a] == '1') {
				flag = 1 - flag;
				return true;
			}
			else return false;
		}
		else {
			return false;
		}
	}
}
/********************************************************************/
/*   Snippet: cordinates x and y moved one step in the given        */
/*	          direction in the routing layer                        */
/********************************************************************/
unsigned int up_x(unsigned int x) {
	return x;
}
unsigned int up_y(unsigned int y) {
	return y - 1;
}
unsigned int down_x(unsigned int x) {
	return x;
}
unsigned int down_y(unsigned int y) {
	return y + 1;
}
unsigned int left_x(unsigned int x) {
	return x - 1;
}
unsigned int left_y(unsigned int y) {
	return y;
}
unsigned int right_x(unsigned int x) {
	return x + 1;
}
unsigned int right_y(unsigned int y) {
	return y;
}
/********************************************************************/
/*Snippet: Presents a swap function between two positions           */
/********************************************************************/
void swap(unsigned int *xp, unsigned int *yp)
{
	unsigned int temp = *xp;
	*xp = *yp;
	*yp = temp;
}
int manhattan(int ao, int bo, int a1, int b1) {
	int distance = abs(ao - a1) + abs(bo - b1);
	return distance;
}
/********************************************************************/
/*Snippet:   A function to implement bubble sort in ascending order */
/********************************************************************/
std::vector<unsigned int > bubbleSort(std::vector<unsigned int> arr)
{
	int i, j;
	unsigned int n = arr.size();
	std::vector<unsigned int> arr1;

	for (unsigned int i = 0; i < n; i++) {
		arr1.push_back(i);
	}

	for (i = 0; i < n - 1; i++) {
		// Last i elements are already in place   
		for (j = 0; j < n - i - 1; j++)
			if (arr[j] > arr[j + 1]) {
				swap(&arr1[j], &arr1[j + 1]);
				swap(&arr[j], &arr[j + 1]);
			}
	}
	return arr1;
}
/**********************************************************************/
/*Snippet:  The File to be sent from the outside as arguments         */
/**********************************************************************/
int main(int argc, char* argv[])
{

	auto start =std::chrono::high_resolution_clock::now();
	unsigned int nets, nodes;
	std::ifstream infile;
	std::ofstream myfile;
	//infile.open();
	infile.open(argv[1]);
   	myfile.open(argv[2]);
  	myfile<<"magic"<<endl;
   	myfile<<"tech scmos"<<endl;
   	myfile<<"timestamp"<<endl;
	infile >> nodes;
	int unrouted = 0;
	infile >> nets;
	/**********************************************************************/
	/*Snippet: Weights of nodes are calculated for ordering               */
	/**********************************************************************/
	std::vector<unsigned int> values;		//vector implementation of the values
	std::vector<unsigned int> cols;			//vector implementation of coloums
	std::vector<unsigned int> rows(nodes + 1, 0);	//vector implementation of row index rows */size is nodes+1*/
	/**********************************************************************/
	/*Snippet: Matrix to save the individual net connections              */
	/**********************************************************************/
	std::vector <std::vector<unsigned int> > matrix;
	matrix.assign(nodes, std::vector < unsigned int >(nodes));
	/**********************************************************************/
	/*Snippet: To save the Cell and Terminal info of the given bench marks*/
	/**********************************************************************/
	std::vector<unsigned int>net_cell1;
	std::vector<unsigned int>net_cell2;
	std::vector<int>cell1_terminal;//to save the terminal info of cell 1
	std::vector<int>cell2_terminal;//to save the terminal info of cell 2
									//a=1;b=2;c=3;d=4;
	for (unsigned int i = 0; i < nodes; i++) {
		for (unsigned int j = 0; j <= i; j++) {
			matrix[i].resize(j + 1);
		}
	}
	unsigned int count = nets;
	while (count) {
		unsigned int x1, x2, x3, x4, x5;
		infile >> x1;//Net number
		infile >> x2;//Cell number1
		infile >> x3;//Terminal of Cell number 1
		infile >> x4;//Cell number 2
		infile >> x5;//Terminal Cell number 2

		x2--;
		x4--;
		net_cell1.push_back(x2);

		net_cell2.push_back(x4);
		if(x2 < x4) {
			matrix[x4][x2] = matrix[x4][x2] + 1;
		}
		if (x2 >= x4) {
			matrix[x2][x4] = matrix[x2][x4] + 1;
		}
		if (x3 == 1) {
			cell1_terminal.push_back('1');
		}
		else if (x3 == 2) {
			cell1_terminal.push_back('2');
		}
		else if (x3 == 3) {
			cell1_terminal.push_back('3');
		}
		else if (x3 == 4) {
			cell1_terminal.push_back('4');
		}
		if (x5 == 1) {
			cell2_terminal.push_back('1');
		}
		else if (x5 == 2) {
			cell2_terminal.push_back('2');
		}
		else if (x5 == 3) {
			cell2_terminal.push_back('3');
		}
		else if (x5 == 4) {
			cell2_terminal.push_back('4');
		}
		count--;
	}
	///Initializinz DSS matrix vectors
	for (unsigned int i = 0; i < nodes; i++) {
		unsigned int count = 0;
		for (unsigned int j = 0; j <= i; j++) {
			if (matrix[i][j] != 0) {
				values.push_back(matrix[i][j]);
				cols.push_back(j);
				count++;
			}
			rows[i + 1] = rows[i] + count;
		}
	}
	std::vector <unsigned int> list(nodes, 0);//for values
	std::vector <unsigned int> list_cells(nodes, 0);//for values

	unsigned int z = 0;
	//Creating the list for connectivity.
	for (unsigned int i = 0; i <nodes; i++) {
		//list_cells.push_back(i);
		unsigned int j = rows[i + 1] - rows[i];
		for (unsigned int x = 0; x < j; x++) {
			if (i != cols[z]) {
				list[i] = list[i] + values[z];//i denotes row node
				list[cols[z]] = list[cols[z]] + values[z];//saving the coloumn node
			}
			z = z + 1;
		}
	}
	list_cells = bubbleSort(list);
	/**********************************************************************/
	/*Snippet: To improve the placement complexity the nets which are     */
	/*         connected to the same nodes are removed before placement   */
	/*         The zeroes are later added after placement is done         */
	/**********************************************************************/
	std::vector<unsigned int>zeros;
	z = 0;
	for (unsigned int i = 0; list[list_cells[i]] == 0; i++) {
		zeros.push_back(list_cells[i]);
		z++;
	}
	//zero nodes are being erased
	list_cells.erase(list_cells.begin(), list_cells.begin() + z);
	/**********************************************************************/
	/*Snippet: Initialize parameters for force directed placement         */
	/**********************************************************************/
	unsigned int abort_count = 0, abort_limit = 10;
	unsigned int iteration_count = 1, iteration_limit = 20 * list_cells.size();
	unsigned int seed;
	//Creating matrix for placement of Cells;
	unsigned int s = sqrt(nodes);
	s++;
	std::vector<bool>cells_status(nodes, 0);//tell if the cell position locked or not 
	std::vector<std::vector<unsigned int> >position(s, std::vector<unsigned int>(s));//see if position is vacant or not 
	srand(time(NULL));
	std::vector<unsigned int>cells_x(nodes);
	std::vector<unsigned int>cells_y(nodes);
	std::random_shuffle(list_cells.begin(), list_cells.end());
	//assigning initial vector
	z = 0;
	for (int i = 0; i < s; i++) {
		for (int j = 0; j < s; j++) {
			position[i][j] = list_cells[z] + 1;
			z++;
			if (z == list_cells.size()) {
				break;
			}
		}
		if (z == list_cells.size()) {
			break;
		}
	}
	//Save cell x and y position from the given placement matrix
	for (unsigned int i = 0; i < s; i++) {
		for (unsigned int j = 0; j < s; j++) {

			if (position[i][j] != 0) {
				//std::cout << position[i][j] - 1 << " ";
				cells_x[position[i][j] - 1] = j;
				cells_y[position[i][j] - 1] = j;
			}
		}
	}
	//Calculates the initial placement wirelength apart from nets connected to same cell
	unsigned int len = 0;
	for (unsigned int i = 0; i < nodes; i++) {
		for (unsigned int j = i; j < nodes; j++) {
			if (i > j) {
				if (matrix[i][j] != 0) {
					len = len + manhattan(cells_x[i], cells_y[i], cells_x[j], cells_y[j]);
				}
			}
			if (i < j) {
				if (matrix[j][i] != 0) {
					len = len + manhattan(cells_x[i], cells_y[i], cells_x[j], cells_y[j]);

				}
			}

		}
	}
	bool end_ripple = 0;
	z = list_cells.size() - 1;
	/**********************************************************************/
	/*Snippet: Starting force Directed placement algorithm                */
	/**********************************************************************/
	while (iteration_count < iteration_limit) {

		seed = list_cells[z];//index of the highest weighted cell
		if (cells_status[seed] == 0) {
			//seed++;
			//std::cout << "\n" << "seed = " << seed << "\n";
			for (unsigned int i = 0; i < s; i++) {
				bool k = 0;
				for (unsigned int j = 0; j < s; j++) {
					if (position[i][j] == seed + 1) {//making that position vacant
						position[i][j] = 0;
						//std::cout << "position of i "<<i<<" j "<<j<<" "<<position[i][j];
						k = 1;
						break;
					}
				}
				if (k == 1) {
					break;
				}
			}
			end_ripple = 0;

			
			while (!end_ripple) {
				//compute the Force directed Xo,Yo for the given seed.
				int xo = 0, yo = 0;
				unsigned int sum_xd = 0, sum_yd = 0, sum_xn = 0, sum_yn = 0;

				for (unsigned int i = 0; i < s; i++) {
					for (unsigned int j = 0; j < s; j++) {
						int x = position[i][j] - 1;

						if ((x > seed) && (x >= 0)) {
							sum_xd = sum_xd + matrix[x][seed];
							sum_yd = sum_yd + matrix[x][seed];
							sum_xn = sum_xn + i * matrix[x][seed];
							sum_yn = sum_yn + j * matrix[x][seed];
						}
						else if (x < seed && (x >= 0)) {
							sum_xd = sum_xd + matrix[seed][x];
							sum_yd = sum_yd + matrix[seed][x];
							sum_xn = sum_xn + i * matrix[seed][x];
							sum_yn = sum_yn + j * matrix[seed][x];
						}
					}
				}

				xo = sum_xn / sum_xd;
				yo = sum_yn / sum_yd;

				//We have computed the xo, yo case see if it is vacant or not 
				//To say if it is vacant or not we need to have a matrix sxs  
				if (position[xo][yo] == 0)//case for vacancy
				{
					//std::cout << "It is Vacant";
					position[xo][yo] = seed + 1;
					end_ripple = 1;
					abort_count = 0;
					cells_status[position[xo][yo] - 1] = 1;	//Cell is locked
				}
				else if (seed == position[xo][yo] - 1) {//case for the same location
					cells_status[position[xo][yo] - 1] = 1;
					end_ripple = 1;
					abort_count = 0;
				}

				else if ((cells_status[position[xo][yo] - 1] == 1) && (position[xo][yo] > 0)) {//in case for locked
					//std::cout << "Locked case" << "\n";
					int distance = 0, temp = 0, x1 = xo, y1 = yo;
					for (int i = 0; i < s; i++) {
						for (int j = 0; j < s; j++) {
							if (position[i][j] == 0) {//cellstatus&&position gives occupied and not locked cases 
								distance = abs(i - xo) + abs(j - yo);

								if (temp == 0) {
									temp = distance;
									x1 = i;
									y1 = j;
								}
								if (distance < temp) {
									x1 = i;
									y1 = j;
									temp = distance;
								}
							}
						}
					}

					xo = x1;
					yo = y1;
					position[xo][yo] = seed + 1;
					//Locking the location
					end_ripple = 1;
					abort_count = abort_count + 1;
					if (abort_count > abort_limit) {
						std::fill(cells_status.begin(), cells_status.end(), 0);
						iteration_count = iteration_count + 1;
					}
				}
				else if ((position[xo][yo] != 0) && (cells_status[position[xo][yo] - 1] == 0)) {//occupied case
					//std::cout << "\n" << "Occupied case starts";
					unsigned int temp;
					temp = position[xo][yo] - 1;
					position[xo][yo] = seed + 1;
					cells_status[position[xo][yo] - 1] = 1;
					seed = temp;
					//std::cout << "\n" << "Current Seed =" << seed << "\n";
					end_ripple = 0;
					abort_count = 0;
					//std::cout << "\n" << "Occupied case stops";
				}
			}

		}
		z--;
		if (z == 4294967295) {
			break;
		}
	}
	/**********************************************************************/
	/*Snippet: nodes in the zerores list are added back to the placement  */
	/**********************************************************************/
	z = zeros.size() - 1;
	if (z!= 4294967295) {
	for (unsigned int i = 0; i < s; i++) {

		for (unsigned int j = 0; j < s; j++) {
			if (position[i][j] == 0) {
				position[i][j] = zeros[z] + 1;
				z--;
			}
			if (z == 4294967295) {
				break;
			}
		}
		if (z == 4294967295) {
			break;
		}
	}
}
//cout << "aascascasca";
	for (unsigned int i = 0; i < s; i++) {
		for (unsigned int j = 0; j < s; j++) {

			if (position[i][j] != 0) {
				//std::cout << position[i][j] - 1 << " ";
				cells_x[position[i][j] - 1] = j;
				cells_y[position[i][j] - 1] = j;
			}
		}
	}

	//Final  placement
	len = 0;
	for (unsigned int i = 0; i < nodes; i++) {
		for (unsigned int j = i; j < nodes; j++) {
			if (i > j) {
				if (matrix[i][j] != 0) {
					len = len + manhattan(cells_x[i], cells_y[i], cells_x[j], cells_y[j]);
				}
			}
			if (i < j) {
				if (matrix[j][i] != 0) {
					len = len + manhattan(cells_x[i], cells_y[i], cells_x[j], cells_y[j]);

				}
			}

		}
	}
	//std::cout << "\n" << " Final Length =" << len << "\n";
	//cout << "end of placement";
	
	cells_x.erase(cells_x.begin(),cells_x.end());
	cells_y.erase(cells_y.begin(),cells_y.end());
	//save the corner position from the examplary placement matrix of each cell

	std::vector <unsigned int> cell_x(nodes);
	std::vector <unsigned int> cell_y(nodes);

	//To save the Orientation of the nodes we are creating a new orientation flags 

	std::vector <bool> cell_o(nodes, 0);//Initially all are Clockwise only

	//from the placement save the x and y positions 

	unsigned int channel_size = 0,bb=0; //should create channel size based on s size;

if(nets>=400){
channel_size=nets/10;
bb=10;
}
else if(nets<400){
channel_size=(nets/10)+15;
bb=6;
}
	for (unsigned int i = 0; i < s; i++) {
		for (unsigned int j = 0; j < s; j++) {
			if (position[i][j] != 0) {
				cell_x[position[i][j] - 1] = channel_size * (j + 1) + 6 * j;
				cell_y[position[i][j] - 1] = channel_size * (i + 1) + 6 * i;
			}
		}
	}

	count = (s*(6 + channel_size) + channel_size);

	std::vector<std::vector<char> > route_space_m0(count);//Initializing the route space for layer 1 
	std::vector<std::vector<char> > route_space_m4(count);//Initializing the route space for layer 2 
        std::vector< std::vector<int> >v5(2);//to save the vias info 
        std::vector< std::vector<int> >a3(2); //to save coordinates of the route 
	std::vector< std::vector<int> >a4(2); //to save coordinates of the route
	for (unsigned int i = 0; i < count; i++) {
		route_space_m0[i].resize(count);
		route_space_m4[i].resize(count);
	}


	//nodes = 1;
	count = 0;
	 
	while (count < nodes) {
		for (unsigned int j = cell_y[count] - bb; j < cell_y[count] + 6 + bb; j++) {
			for (unsigned int i = cell_x[count] - bb; i < cell_x[count] + 6 + bb; i++) {
				route_space_m0[j][i] = 'b';
				route_space_m4[j][i] = 'b';

				}	
			}
	count++;
	}
   for(int i=0;i<nodes;i++)
   {//printing cells in magic file 
      myfile<<"<< pdiffusion >>"<<endl;
      myfile<<"rect "<<cell_x[i]<<" "<<cell_y[i]<<" "<<cell_x[i]+6<<" "<<cell_y[i]+6<<endl;
      myfile<<"<< labels >>"<<endl;
      myfile<<"rlabel pdiffusion "<<cell_x[i]<<" "<<cell_y[i]<<" "<<cell_x[i]+6<<" "<<cell_y[i]+6<<" 0 cell_no = "<<i+1<<endl;
         }
	unsigned int nets1 = 0, alpha = 1;
	
       
	count = (s*(6 + channel_size) + channel_size);
	while (nets1 < nets ) {
		//std::cout << "Net =" << nets1 << "\n";
		char dir_flag;
		std::vector< std::vector<int> >v3(2); //to save coordinates of the route 
		std::vector< std::vector<int> >v4(2); //to save coordinates of the route
		std::vector< std::vector<int> >v1(2);
		std::vector< std::vector<int> >v2(2);
		std::vector<std::vector<char> > route_space_m1(count);//Initializing the route space for m1
		std::vector<std::vector<char> > route_space_m2(count);//Initializing the route space for m2


		for (unsigned int i = 0; i < count; i++) {
			route_space_m1[i].resize(count);
			route_space_m2[i].resize(count);
			for (unsigned int j = 0; j < count; j++) {
				route_space_m1[i][j] = route_space_m0[i][j];
				route_space_m2[i][j] = route_space_m4[i][j];
			}
		}

		//assiging values to both the layers 
		//start from the vector v1 and save to vector v2 

		bool flag = 1;
		
		int sx = terminal_x(cell_x[net_cell1[nets1]], cell1_terminal[nets1]);
		int sy = terminal_y(cell_y[net_cell1[nets1]], cell1_terminal[nets1]);

		v1[0].push_back(sx);
		v1[1].push_back(sy);
		int tx = terminal_x(cell_x[net_cell2[nets1]], cell2_terminal[nets1]);
		int ty = terminal_y(cell_y[net_cell2[nets1]], cell2_terminal[nets1]);


count = (s*(6 + channel_size) + channel_size);


if (cell1_terminal[nets1] == 49){
	for(int i = sx; i >= sx - 1 - bb; i--){
		for(int j = sy; j >= sy - bb + 1; j --){
		route_space_m1[j][i] = '\0';
		route_space_m2[j][i] = '\0';}	}}

else if (cell1_terminal[nets1] ==50){
	for(int i = sx; i <= sx + 1 + bb; i++){
		for(int j = sy; j >= sy - bb + 1; j --){
		route_space_m1[j][i] = '\0';
		route_space_m2[j][i] = '\0';}	}}
else if (cell1_terminal[nets1] == 51){
	for(int i = sx; i >= sx - 1 - bb; i--){
		for(int j = sy; j <= sy + bb - 1; j ++){
		route_space_m1[j][i] = '\0';
		route_space_m2[j][i] = '\0';}	}}
else if (cell1_terminal[nets1] == 52){
	for(int i = sx; i <= sx + 1 + bb; i++){
		for(int j = sy; j <= sy + bb - 1; j ++){
		route_space_m1[j][i] = '\0';
		route_space_m2[j][i] = '\0';}	}}
else break;

if (cell2_terminal[nets1] == 49){
	for(int i = tx; i >= tx - 1 - bb; i--){
		for(int j = ty; j >= ty - bb + 1; j --){
		route_space_m1[j][i] = '\0';
		route_space_m2[j][i] = '\0';}	}}
else if (cell2_terminal[nets1] ==50){
	for(int i = tx; i <= tx + 1 + bb; i++){
		for(int j = ty; j >= ty - bb + 1; j --){
		route_space_m1[j][i] = '\0';
		route_space_m2[j][i] = '\0';}	}}
else if (cell2_terminal[nets1] == 51){
	for(int i = tx; i >= tx - 1 - bb; i--){
		for(int j = ty; j <= ty + bb - 1; j ++){
		route_space_m1[j][i] = '\0';
		route_space_m2[j][i] = '\0';}	}}
else if (cell2_terminal[nets1] == 52){
	for(int i = tx; i <= tx + 1 + bb; i++){
		for(int j = ty; j <= ty + bb - 1; j ++){
		route_space_m1[j][i] = '\0';
		route_space_m2[j][i] = '\0';}	}}
else break;

for (unsigned int i = 1; i < a3[0].size(); i++) {
				int m = 0;
				unsigned int vx = a3[0][i];
				unsigned int vy = a3[1][i];
if((vx>=0)&&(vx<count)&&(vy>=0)&&(vy<count)){
				route_space_m1[vy][vx] = 'b';
m++;
}
else break;
if((up_x(vx)>=0)&&(up_x(vx)<count)&&(up_y(vy)>=0)&&(up_y(vy)<count)){
				route_space_m1[up_y(vy)][up_x(vx)] = 'b';
m++;
}
else break;
if((down_x(vx)>=0)&&(down_x(vx)<count)&&(down_y(vy)>=0)&&(down_y(vy)<count)){
				route_space_m1[down_y(vy)][down_x(vx)] = 'b';
m++;
}
else break;
if((left_x(vx)>=0)&&(left_x(vx)<count)&&(left_y(vy)>=0)&&(left_y(vy)<count)){
				route_space_m1[left_y(vy)][left_x(vx)] = 'b';
m++;
}
else break;
if((right_x(vx)>=0)&&(right_x(vx)<count)&&(right_y(vy)>=0)&&(right_y(vy)<count)){
				route_space_m1[right_y(vy)][right_x(vx)] = 'b';
m++;
}
else break;
//v4 is a4
if (m == 0){break;}
			}
			for (unsigned int i = 1; i < a4[0].size(); i++) {
				int m = 0;
				unsigned int vx = a4[0][i];
				unsigned int vy = a4[1][i];

				if((vx>=0)&&(vx<count)&&(vy>=0)&&(vy<count)){
				route_space_m2[vy][vx] = 'b';
				m++;
}
if((up_x(vx)>=0)&&(up_x(vx)<count)&&(up_y(vy)>=0)&&(up_y(vy)<count)){
				route_space_m2[up_y(vy)][up_x(vx)] = 'b';m++;
}
else break;
if((down_x(vx)>=0)&&(down_x(vx)<count)&&(down_y(vy)>=0)&&(down_y(vy)<count)){
				route_space_m2[down_y(vy)][down_x(vx)] = 'b';m++;
}
else break;
if((left_x(vx)>=0)&&(left_x(vx)<count)&&(left_y(vy)>=0)&&(left_y(vy)<count)){
				route_space_m2[left_y(vy)][left_x(vx)] = 'b';m++;
}
else break;
if((right_x(vx)>=0)&&(right_x(vx)<count)&&(right_y(vy)>=0)&&(right_y(vy)<count)){
				route_space_m2[right_y(vy)][right_x(vx)] = 'b';m++;
}
else break;
if (m == 0){break;}
			}
for (unsigned int i = 0; i < v5[0].size(); i++) {
				int m = 0;
				unsigned int vx = v5[0][i];
				unsigned int vy = v5[1][i];
if((vx>=0)&&(vx<count)&&(vy>=0)&&(vy<count)){
				route_space_m1[vy][vx] = 'b';
                                route_space_m2[vy][vx] = 'b';m++;
}
else break;
if((up_x(vx)>=0)&&(up_x(vx)<count)&&(up_y(vy)>=0)&&(up_y(vy)<count)){
				route_space_m1[up_y(vy)][up_x(vx)] = 'b';
                                route_space_m2[up_y(vy)][up_x(vx)] = 'b';m++;
}
else break;
if((down_x(vx)>=0)&&(down_x(vx)<count)&&(down_y(vy)>=0)&&(down_y(vy)<count)){
				route_space_m1[down_y(vy)][down_x(vx)] = 'b';
				route_space_m2[down_y(vy)][down_x(vx)] = 'b';m++;
}
else break;
if((left_x(vx)>=0)&&(left_x(vx)<count)&&(left_y(vy)>=0)&&(left_y(vy)<count)){
				route_space_m1[left_y(vy)][left_x(vx)] = 'b';
                                route_space_m2[left_y(vy)][left_x(vx)] = 'b';m++;
}
else break;
if((right_x(vx)>=0)&&(right_x(vx)<count)&&(right_y(vy)>=0)&&(right_y(vy)<count)){
				route_space_m1[right_y(vy)][right_x(vx)] = 'b';
				route_space_m2[right_y(vy)][right_x(vx)] = 'b';m++;
}
else break;
if (m == 0){break;}
}	
//sx and sy start point 
//tx and ty terminal point
		route_space_m1[sy][sx] = '0';
		route_space_m2[sy][sx] = '0';		
		route_space_m1[ty][tx] == '\0';
		route_space_m2[ty][tx] == '\0';
		bool b = 0;
		//started searching
		/**********************************************************************/
		/*Snippet: The Snippt expands iteratively for each step and flag is   */
		/*         negated at each iteration                                  */
		/**********************************************************************/
		while ((route_space_m1[ty][tx] == '\0') || (route_space_m2[ty][tx] == '\0')) {
			unsigned int a = 1;
			while (v1[0].size() >= 1)
			{
				//v1[0][i]  //move //return v2 //append v2 to v1
				//v1[1][i]  //move	//return v2 //append v2 to v1 
				int x = v1[0][v1[0].size() - 1];
				int y = v1[1][v1[1].size() - 1];
				a = 1;
				//route_space_m1[y][x] = '1';
				if ((x - 1 >= 0) && (x - 1 < count))//left
				{
					a++;
					if ((route_space_m1[y][x - 1] != 'b') && (route_space_m1[y][x - 1] == '\0')) {

						/*v2[0].push_back(x - 1);
						v2[1].push_back(y);*/
						if (route_space_m1[y][x] == '0' && route_space_m1[y][x - 1] != 'b') {
							route_space_m1[y][x - 1] = '1';
							v2[0].push_back(x - 1);
							v2[1].push_back(y);
							if (route_space_m2[y][x - 1] != 'b') {
								route_space_m2[y][x - 1] = '1';
							}
						}
						if (route_space_m1[y][x] == '1') {
							if (flag == 0) {
								route_space_m1[y][x - 1] = '1';
								v2[0].push_back(x - 1);
								v2[1].push_back(y);
								if (route_space_m2[y][x - 1] != 'b') {
									route_space_m2[y][x - 1] = '1';
								}
							}
							else {
								route_space_m1[y][x - 1] = '2';
								v2[0].push_back(x - 1);
								v2[1].push_back(y);
								if (route_space_m2[y][x - 1] != 'b') {
									route_space_m2[y][x - 1] = '2';
								}
							}
						}
						else if (route_space_m1[y][x] == '2') {
							if (flag == 0) {
								route_space_m1[y][x - 1] = '2';
								v2[0].push_back(x - 1);
								v2[1].push_back(y);
								if (route_space_m2[y][x - 1] != 'b') {
									route_space_m2[y][x - 1] = '2';
								}
							}
							else {
								route_space_m1[y][x - 1] = '1';
								v2[0].push_back(x - 1);
								v2[1].push_back(y);
								if (route_space_m2[y][x - 1] != 'b') {
									route_space_m2[y][x - 1] = '1';
								}
							}
						}
					}
					if ((route_space_m2[y][x - 1] != 'b') && (route_space_m2[y][x - 1] == '\0')) {

						if (route_space_m2[y][x] == '0' && route_space_m2[y][x - 1] != 'b') {

							route_space_m2[y][x - 1] = '1';
							v2[0].push_back(x - 1);
							v2[1].push_back(y);
							if (route_space_m1[y][x - 1] != 'b') {
								route_space_m1[y][x - 1] = '1';
							}

						}
						if (route_space_m2[y][x] == '1') {
							if (flag == 0) {
								route_space_m2[y][x - 1] = '1';
								v2[0].push_back(x - 1);
								v2[1].push_back(y);
								if (route_space_m1[y][x - 1] != 'b') {
									route_space_m1[y][x - 1] = '1';
								}
							}
							else {
								route_space_m2[y][x - 1] = '2';
								v2[0].push_back(x - 1);
								v2[1].push_back(y);
								if (route_space_m1[y][x - 1] != 'b') {
									route_space_m1[y][x - 1] = '2';
								}
							}
						}
						else if (route_space_m2[y][x] == '2') {
							if (flag == 0) {
								route_space_m2[y][x - 1] = '2';
								v2[0].push_back(x - 1);
								v2[1].push_back(y);
								if (route_space_m1[y][x - 1] != 'b') {
									route_space_m1[y][x - 1] = '2';
								}
							}
							else {
								route_space_m2[y][x - 1] = '1';
								v2[0].push_back(x - 1);
								v2[1].push_back(y);
								if (route_space_m1[y][x - 1] != 'b') {
									route_space_m1[y][x - 1] = '1';
								}
							}
						}
					}
				}
				if ((x + 1 >= 0) && (x + 1 < count))//right
				{
					a++;
					if ((route_space_m1[y][x + 1] != 'b') && (route_space_m1[y][x + 1] == '\0')) {
						if (route_space_m1[y][x] == '0'&& route_space_m1[y][x + 1] != 'b') {

							route_space_m1[y][x + 1] = '1';
							v2[1].push_back(y);
							v2[0].push_back(x + 1);
							if (route_space_m2[y][x + 1] != 'b') {
								route_space_m2[y][x + 1] = '1';
							}

						}
						if (route_space_m1[y][x] == '1') {
							if (flag == 0) {
								route_space_m1[y][x + 1] = '1';
								v2[1].push_back(y);
								v2[0].push_back(x + 1);
								if (route_space_m2[y][x + 1] != 'b') {
									route_space_m2[y][x + 1] = '1';
								}
							}
							else {

								route_space_m1[y][x + 1] = '2';
								v2[1].push_back(y);
								v2[0].push_back(x + 1);
								if (route_space_m2[y][x + 1] != 'b') {
									route_space_m2[y][x + 1] = '2';
								}
							}
						}
						else if (route_space_m1[y][x] == '2') {
							if (flag == 0) {
								route_space_m1[y][x + 1] = '2';
								v2[1].push_back(y);
								v2[0].push_back(x + 1);
								if (route_space_m2[y][x + 1] != 'b') {
									route_space_m2[y][x + 1] = '2';
								}

							}
							else {
								route_space_m1[y][x + 1] = '1';
								v2[1].push_back(y);
								v2[0].push_back(x + 1);
								if (route_space_m2[y][x + 1] != 'b') {
									route_space_m2[y][x + 1] = '1';
								}
							}
						}
					}
					if ((route_space_m2[y][x + 1] != 'b') && (route_space_m2[y][x + 1] == '\0')) {
						if (route_space_m2[y][x] == '0'&& route_space_m2[y][x + 1] != 'b') {

							route_space_m2[y][x + 1] = '1';
							v2[1].push_back(y);
							v2[0].push_back(x + 1);
							if (route_space_m1[y][x + 1] != 'b') {
								route_space_m1[y][x + 1] = '1';
							}

						}
						v2[1].push_back(y);*/
						if (route_space_m2[y][x] == '1') {
							if (flag == 0) {
								route_space_m2[y][x + 1] = '1';
								v2[1].push_back(y);
								v2[0].push_back(x + 1);
								if (route_space_m1[y][x + 1] != 'b') {
									route_space_m1[y][x + 1] = '1';
								}
							}
							else {

								route_space_m2[y][x + 1] = '2';
								v2[1].push_back(y);
								v2[0].push_back(x + 1);
								if (route_space_m1[y][x + 1] != 'b') {
									route_space_m1[y][x + 1] = '2';
								}
							}
						}
						else if (route_space_m2[y][x] == '2') {
							if (flag == 0) {
								route_space_m2[y][x + 1] = '2';
								v2[1].push_back(y);
								v2[0].push_back(x + 1);
								if (route_space_m1[y][x + 1] != 'b') {
									route_space_m1[y][x + 1] = '2';
								}

							}
							else {
								route_space_m2[y][x + 1] = '1';
								v2[1].push_back(y);
								v2[0].push_back(x + 1);
								if (route_space_m1[y][x + 1] != 'b') {
									route_space_m1[y][x + 1] = '1';
								}
							}
						}
					}

				}
				if ((y - 1 >= 0) && (y - 1 < count))//up
				{
					a++;
					if ((route_space_m1[y - 1][x] != 'b') && (route_space_m1[y - 1][x] == '\0')) {
						if (route_space_m1[y][x] == '0'&& route_space_m1[y - 1][x] != 'b') {

							route_space_m1[y - 1][x] = '1';
							v2[0].push_back(x);
							v2[1].push_back(y - 1);
							if (route_space_m2[y - 1][x] != 'b') {
								route_space_m2[y - 1][x] = '1';
							}
						}

						/*v2[0].push_back(x);
						v2[1].push_back(y - 1);*/
						if (route_space_m1[y][x] == '1') {
							if (flag == 0) {
								route_space_m1[y - 1][x] = '1';
								v2[0].push_back(x);
								v2[1].push_back(y - 1);
								if (route_space_m2[y - 1][x] != 'b') {
									route_space_m2[y - 1][x] = '1';
								}
							}
							else {
								route_space_m1[y - 1][x] = '2';
								v2[0].push_back(x);
								v2[1].push_back(y - 1);
								if (route_space_m2[y - 1][x] != 'b') {
									route_space_m2[y - 1][x] = '2';
								}
							}
						}
						else if (route_space_m1[y][x] == '2') {
							if (flag == 0) {
								route_space_m1[y - 1][x] = '2';
								v2[0].push_back(x);
								v2[1].push_back(y - 1);
								if (route_space_m2[y - 1][x] != 'b') {
									route_space_m2[y - 1][x] = '2';
								}
							}
							else {
								route_space_m1[y - 1][x] = '1';
								v2[0].push_back(x);
								v2[1].push_back(y - 1);
								if (route_space_m2[y - 1][x] != 'b') {
									route_space_m2[y - 1][x] = '1';
								}
							}
						}
					}

					if ((route_space_m2[y - 1][x] != 'b') && (route_space_m2[y - 1][x] == '\0')) {//searchin metal2
						if (route_space_m2[y - 1][x] == '0'&& route_space_m2[y - 1][x] != 'b') {//changed here 

							route_space_m2[y - 1][x] = '1';
							v2[0].push_back(x);
							v2[1].push_back(y - 1);
							if (route_space_m1[y - 1][x] != 'b') {
								route_space_m1[y - 1][x] = '1';
							}
						}

						/*v2[0].push_back(x);
						v2[1].push_back(y - 1);*/
						if (route_space_m2[y][x] == '1') {
							if (flag == 0) {
								route_space_m2[y - 1][x] = '1';
								v2[0].push_back(x);
								v2[1].push_back(y - 1);
								if (route_space_m1[y - 1][x] != 'b') {
									route_space_m1[y - 1][x] = '1';
								}
							}
							else {
								route_space_m2[y - 1][x] = '2';
								v2[0].push_back(x);
								v2[1].push_back(y - 1);
								if (route_space_m1[y - 1][x] != 'b') {
									route_space_m1[y - 1][x] = '2';
								}
							}
						}
						else if (route_space_m2[y][x] == '2') {
							if (flag == 0) {
								route_space_m2[y - 1][x] = '2';
								v2[0].push_back(x);
								v2[1].push_back(y - 1);
								if (route_space_m1[y - 1][x] != 'b') {
									route_space_m1[y - 1][x] = '2';
								}
							}
							else {
								route_space_m2[y - 1][x] = '1';
								v2[0].push_back(x);
								v2[1].push_back(y - 1);
								if (route_space_m1[y - 1][x] != 'b') {
									route_space_m1[y - 1][x] = '1';
								}
							}
						}
					}

				}
				if ((y + 1 >= 0) && (y + 1 < count))//right
				{

					if ((route_space_m1[y + 1][x] != 'b') && (route_space_m1[y + 1][x] == '\0')) {
						if (route_space_m1[y][x] == '0'&& route_space_m1[y + 1][x] != 'b') {

							route_space_m1[y + 1][x] = '1';
							v2[0].push_back(x);
							v2[1].push_back(y + 1);
							if (route_space_m2[y + 1][x] != 'b') {
								route_space_m2[y + 1][x] = '1';
							}
						}

						/*v2[0].push_back(x);
						v2[1].push_back(y + 1);*/
						if (route_space_m1[y][x] == '1') {
							if (flag == 0) {
								route_space_m1[y + 1][x] = '1';
								v2[0].push_back(x);
								v2[1].push_back(y + 1);
								if (route_space_m2[y + 1][x] != 'b') {
									route_space_m2[y + 1][x] = '1';
								}
							}
							else {
								route_space_m1[y + 1][x] = '2';
								v2[0].push_back(x);
								v2[1].push_back(y + 1);
								if (route_space_m2[y + 1][x] != 'b') {
									route_space_m2[y + 1][x] = '2';
								}
							}
						}
						else if (route_space_m1[y][x] == '2') {
							if (flag == 0) {
								route_space_m1[y + 1][x] = '2';
								v2[0].push_back(x);
								v2[1].push_back(y + 1);
								if (route_space_m2[y + 1][x] != 'b') {
									route_space_m2[y + 1][x] = '2';
								}
							}
							else {
								route_space_m1[y + 1][x] = '1';
								v2[0].push_back(x);
								v2[1].push_back(y + 1);
								if (route_space_m2[y + 1][x] != 'b') {
									route_space_m2[y + 1][x] = '1';
								}
							}
						}
					}

					if ((route_space_m2[y + 1][x] != 'b') && (route_space_m2[y + 1][x] == '\0')) {
						if (route_space_m2[y][x] == '0'&& route_space_m2[y + 1][x] != 'b') {

							route_space_m2[y + 1][x] = '1';
							v2[0].push_back(x);
							v2[1].push_back(y + 1);
							if (route_space_m1[y + 1][x] != 'b') {
								route_space_m1[y + 1][x] = '1';
							}
						}

						/*v2[0].push_back(x);
						v2[1].push_back(y + 1);*/
						if (route_space_m2[y][x] == '1') {
							if (flag == 0) {
								route_space_m2[y + 1][x] = '1';
								v2[0].push_back(x);
								v2[1].push_back(y + 1);
								if (route_space_m1[y + 1][x] != 'b') {
									route_space_m1[y + 1][x] = '1';
								}
							}
							else {
								route_space_m2[y + 1][x] = '2';
								v2[0].push_back(x);
								v2[1].push_back(y + 1);
								if (route_space_m1[y + 1][x] != 'b') {
									route_space_m1[y + 1][x] = '2';
								}
							}
						}
						else if (route_space_m2[y][x] == '2') {
							if (flag == 0) {
								route_space_m2[y + 1][x] = '2';
								v2[0].push_back(x);
								v2[1].push_back(y + 1);
								if (route_space_m1[y + 1][x] != 'b') {
									route_space_m1[y + 1][x] = '2';
								}
							}
							else {
								route_space_m2[y + 1][x] = '1';
								v2[0].push_back(x);
								v2[1].push_back(y + 1);
								if (route_space_m1[y + 1][x] != 'b') {
									route_space_m1[y + 1][x] = '1';
								}
							}
						}
					}

				}
				v1[0].pop_back();
				v1[1].pop_back();
				if (a == 1) {
				//If any more moves cannot be made in either of the layers
					b = 1;
				
					v1.erase(v1.begin(), v1.end());
					v2.erase(v2.begin(), v2.end());
					break;
				}

				//v2 is pushed 
			}//end of while 

			if (a == 1) {
				//std::cout << "cannot move anymore";

				//unrouted +1;
				b = 1;

				v1.erase(v1.begin(), v1.end());
				v2.erase(v2.begin(), v2.end());
				break;
			}
			else {
				v1.swap(v2);
				flag = 1 - flag;
			}

		}//end of the maze routing expansion
		
		 /**********************************************************************/
		 /*Snippet: The program starts backtracing if a terminal is hit        */
		 /**********************************************************************/
		if (b == 0) {
			v3[0].push_back(tx);
			v3[1].push_back(ty);
			v4[0].push_back(tx);
			v4[1].push_back(ty);

			int cx = v3[0][v3[0].size() - 1];
			int cy = v3[1][v3[1].size() - 1];
			
			bool layer = 0,z=0;
			unsigned int beta=0;
			while (beta < count*count) {//beta governs the iteration of backtracing 
				count++;
				int cx = v3[0][v3[0].size() - 1];
				int cy = v3[1][v3[1].size() - 1];

				int dx = v4[0][v4[0].size() - 1];
				int dy = v4[1][v4[1].size() - 1];
				if(z==0){
					z=1;
					myfile<<"<< poly >>"<<endl;
      					myfile<<"rect "<<sx<<" "<<sy<<" "<<sx+1<<" "<<sy+1<<endl;
					myfile<<"<< labels >>"<<endl;
					myfile<<"rlabel poly "<<sx<<" "<<sy<<" "<<sx+1<<" "<<sy+1<<" 0  T"<<cell1_terminal[nets1]-48<<endl;
					myfile<<"<< poly >>"<<endl;
      					myfile<<"rect "<<tx<<" "<<ty<<" "<<tx+1<<" "<<ty+1<<endl;
					myfile<<"<< labels >>"<<endl;
					myfile<<"rlabel poly "<<tx<<" "<<ty<<" "<<tx+1<<" "<<ty+1<<" 0  T"<<cell2_terminal[nets1]-48<<endl;}

					if (((cx == sx) && (cy == sy)) || ((dx == sx) && (dy == sy))) {
						flag = 1 - flag;
						break;
						}
				/**********************************************************************/
				/*Snippet: starts with layer and shifts until the source is hit       */
				/**********************************************************************/
				if (layer == 0) {
					int x = v3[0][v3[0].size() - 1];
					int y = v3[1][v3[1].size() - 1];

					if ((x == sx) && (y == sy)) {
						flag = 1 - flag;
						break;
					}
					if (x == tx && y == ty) {
						if (y - 1 >= 0 && y - 1 < count && x >= 0 && x < count && route_space_m1[y - 1][x] != 'b' && route_space_m1[y - 1][x] != '\0' && checkback(flag, x, y - 1, route_space_m1, x, y, count, sx, sy)) {//checking for north
							flag = 1 - flag;
							dir_flag = 'n';
							v3[0].push_back(x);
							v3[1].push_back(y - 1);
						}
						else if (y >= 0 && y < count && x - 1 >= 0 && x - 1 < count && route_space_m1[y][x - 1] != 'b' && route_space_m1[y][x - 1] != '\0' && checkback(flag, x - 1, y, route_space_m1, x, y, count, sx, sy)) {//checking for west
							flag = 1 - flag;
							dir_flag = 'w';
							v3[0].push_back(x - 1);
							v3[1].push_back(y);
						}
						else if (y >= 0 && y < count && x + 1 >= 0 && x + 1 < count && route_space_m1[y][x + 1] != 'b' && route_space_m1[y][x + 1] != '\0' && checkback(flag, x + 1, y, route_space_m1, x, y, count, sx, sy)) {//check for east
							flag = 1 - flag;
							dir_flag = 'e';
							v3[0].push_back(x + 1);
							v3[1].push_back(y);
						}
						else if (y + 1 >= 0 && y + 1 < count && x >= 0 && x < count && route_space_m1[y + 1][x] != 'b' && route_space_m1[y + 1][x] != '\0' && checkback(flag, x, y + 1, route_space_m1, x, y, count, sx, sy)) {// check for south
							flag = 1 - flag;
							dir_flag = 's';
							v3[0].push_back(x);
							v3[1].push_back(y + 1);
						}
						else {
							layer = 1;
							v4[0].push_back(x);
							v4[1].push_back(y);

							v5[0].push_back(x);
							v5[1].push_back(y);
					
						}
					}

					//// start backtrace in layer m1 
					else {
						if (dir_flag == 'w') {
							if (y >= 0 && y < count && x - 1 >= 0 && x - 1 < count && route_space_m1[y][x - 1] != 'b' && route_space_m1[y][x - 1] != '\0' && checkback(flag, x - 1, y, route_space_m1, x, y, count, sx, sy)) {//checking for west
								flag = 1 - flag;
								dir_flag = 'w';
								v3[0].push_back(x - 1);
								v3[1].push_back(y);
							}
							else if (y - 1 >= 0 && y - 1 < count && x >= 0 && x < count && route_space_m1[y - 1][x] != 'b' && route_space_m1[y - 1][x] != '\0' && checkback(flag, x, y - 1, route_space_m1, x, y, count, sx, sy)) {//checking for north
								flag = 1 - flag;
								dir_flag = 'n';
								v3[0].push_back(x);
								v3[1].push_back(y - 1);
							}
							else if (y + 1 >= 0 && y + 1 < count && x >= 0 && x < count && route_space_m1[y + 1][x] != 'b' && route_space_m1[y + 1][x] != '\0' && checkback(flag, x, y + 1, route_space_m1, x, y, count, sx, sy)) {// check for south
								flag = 1 - flag;
								dir_flag = 's';
								v3[0].push_back(x);
								v3[1].push_back(y + 1);
							}
							else {
								layer = 1;
								v4[0].push_back(x);
								v4[1].push_back(y);
                                                                v5[0].push_back(x);
							        v5[1].push_back(y);
					
							}
						}

						else  if (dir_flag == 'e') {

							if (y >= 0 && y < count && x + 1 > 0 && x + 1 < count && route_space_m1[y][x + 1] != 'b' && route_space_m1[y][x + 1] != '\0' && checkback(flag, x + 1, y, route_space_m1, x, y, count, sx, sy)) {//check for east
								flag = 1 - flag;
								dir_flag = 'e';
								v3[0].push_back(x + 1);
								v3[1].push_back(y);
							}
							else if (y - 1 >= 0 && y - 1 < count && x >= 0 && x < count && route_space_m1[y - 1][x] != 'b' && route_space_m1[y - 1][x] != '\0' && checkback(flag, x, y - 1, route_space_m1, x, y, count, sx, sy)) {//checking for north
								flag = 1 - flag;
								dir_flag = 'n';
								v3[0].push_back(x);
								v3[1].push_back(y - 1);
							}
							else if (y + 1 >= 0 && y + 1 < count && x >= 0 && x < count && route_space_m1[y + 1][x] != 'b' && route_space_m1[y + 1][x] != '\0' && checkback(flag, x, y + 1, route_space_m1, x, y, count, sx, sy)) {// check for south
								flag = 1 - flag;
								dir_flag = 's';
								v3[0].push_back(x);
								v3[1].push_back(y + 1);
							}
							else {
								layer = 1;
								v4[0].push_back(x);
								v4[1].push_back(y);
                                    				v5[0].push_back(x);
							        v5[1].push_back(y);
					
							}
						}
						else  if (dir_flag == 's') {
							if (y + 1 > 0 && y + 1 < count && x >= 0 && x < count && route_space_m1[y + 1][x] != 'b' && route_space_m1[y + 1][x] != '\0' && checkback(flag, x, y + 1, route_space_m1, x, y, count, sx, sy)) {// check for south
								flag = 1 - flag;
								dir_flag = 's';
								v3[0].push_back(x);
								v3[1].push_back(y + 1);
							}
							else if (y >= 0 && y < count && x + 1 >= 0 && x + 1 < count && route_space_m1[y][x + 1] != 'b' && route_space_m1[y][x + 1] != '\0' && checkback(flag, x + 1, y, route_space_m1, x, y, count, sx, sy)) {//check for east
								flag = 1 - flag;
								dir_flag = 'e';
								v3[0].push_back(x + 1);
								v3[1].push_back(y);
							}
							else if (y >= 0 && y < count && x - 1 >= 0 && x - 1 < count && route_space_m1[y][x - 1] != 'b' && route_space_m1[y][x - 1] != '\0' && checkback(flag, x - 1, y, route_space_m1, x, y, count, sx, sy)) {//checking for west
								flag = 1 - flag;
								dir_flag = 'w';
								v3[0].push_back(x - 1);
								v3[1].push_back(y);
							}
							else {
								layer = 1;
								v4[0].push_back(x);
								v4[1].push_back(y);
                                				v5[0].push_back(x);
							    	v5[1].push_back(y);
							}
						}
						else  if (dir_flag == 'n') {
							if (y - 1 >= 0 && y - 1 < count && x >= 0 && x < count && route_space_m1[y - 1][x] != 'b' && route_space_m1[y - 1][x] != '\0' && checkback(flag, x, y - 1, route_space_m1, x, y, count, sx, sy)) {//checking for north
								flag = 1 - flag;
								dir_flag = 'n';
								v3[0].push_back(x);
								v3[1].push_back(y - 1);
							}
							else if (y >= 0 && y < count && x + 1 >= 0 && x + 1 < count && route_space_m1[y][x + 1] != 'b' && route_space_m1[y][x + 1] != '\0' && checkback(flag, x + 1, y, route_space_m1, x, y, count, sx, sy)) {//check for east
								flag = 1 - flag;
								dir_flag = 'e';
								v3[0].push_back(x + 1);
								v3[1].push_back(y);
							}
							else if (y >= 0 && y < count && x - 1 >= 0 && x - 1 < count && route_space_m1[y][x - 1] != 'b' && route_space_m1[y][x - 1] != '\0' && checkback(flag, x - 1, y, route_space_m1, x, y, count, sx, sy)) {//checking for west
								flag = 1 - flag;
								dir_flag = 'w';
								v3[0].push_back(x - 1);
								v3[1].push_back(y);
							}
							else {
								layer = 1;
								v4[0].push_back(x);
								v4[1].push_back(y);
                               					v5[0].push_back(x);
							    	v5[1].push_back(y);
							}
						}
					}
				}
				else if (layer == 1) {//search metal 2
					int x = v4[0][v4[0].size() - 1];
					cx = x;
					int y = v4[1][v4[1].size() - 1];
					cy = y;
					
					if (route_space_m1[y][x] != '\0'){
					//"in layer2"<<"\n";
					if (x == tx && y == ty) {//metal2 back trace starts from the terminal
						if (y - 1 >= 0 && y - 1 < count && x >= 0 && x < count && route_space_m2[y - 1][x] != 'b' && route_space_m2[y - 1][x] != '\0' && checkback(flag, x, y - 1, route_space_m2, x, y, count, sx, sy)) {//checking for north
							flag = 1 - flag;
							dir_flag = 'n';
							v4[0].push_back(x);
							v4[1].push_back(y - 1);
						}
						else if (y >= 0 && y < count && x - 1 >= 0 && x - 1 < count && route_space_m2[y][x - 1] != 'b' && route_space_m2[y][x - 1] != '\0' && checkback(flag, x - 1, y, route_space_m2, x, y, count, sx, sy)) {//checking for west
							flag = 1 - flag;
							dir_flag = 'w';
							v4[0].push_back(x - 1);
							v4[1].push_back(y);
						}
						else if (y >= 0 && y < count && x + 1 >= 0 && x + 1 < count && route_space_m2[y][x + 1] != 'b' && route_space_m2[y][x + 1] != '\0' && checkback(flag, x + 1, y, route_space_m2, x, y, count, sx, sy)) {//check for east
							flag = 1 - flag;
							dir_flag = 'e';
							v4[0].push_back(x + 1);
							v4[1].push_back(y);
						}
						else if (y + 1 >= 0 && y + 1 < count && x >= 0 && x < count && route_space_m2[y + 1][x] != 'b' && route_space_m2[y + 1][x] != '\0' && checkback(flag, x, y + 1, route_space_m2, x, y, count, sx, sy)) {// check for south
							flag = 1 - flag;
							dir_flag = 's';
							v4[0].push_back(x);
							v4[1].push_back(y + 1);
						}
						else {
							layer = 0;
							v3[0].push_back(x);
							v3[1].push_back(y);
                            				v5[0].push_back(x);
							v5[1].push_back(y);      
						}
					}
					//// start backtrace in layer m2 
					else if (x != tx || y != ty) {
						if (dir_flag == 'w') {
							if (y >= 0 && y < count && x - 1 >= 0 && x - 1 < count && route_space_m2[y][x - 1] != 'b' && route_space_m2[y][x - 1] != '\0' && checkback(flag, x - 1, y, route_space_m2, x, y, count, sx, sy)) {//checking for west
								if (y >= 0 && y < count && x - 1 >= 0 && x - 1 < count && route_space_m1[y][x - 1] != 'b' && route_space_m1[y][x - 1] != '\0' && checkback(flag, x - 1, y, route_space_m1, x, y, count, sx, sy)) {//checking for west
                                                        							layer=0;
														flag = 1 - flag;
                                                        							v3[0].push_back(x-1);
														v3[1].push_back(y);
                                                      								v5[0].push_back(x-1);
														v5[1].push_back(y);
                                
                                                       // break;
								}else{
								flag = 1 - flag;
								dir_flag = 'w';
								v4[0].push_back(x - 1);
								v4[1].push_back(y);
								}
							}
                                         
							else if (y - 1 >= 0 && y - 1 < count && x >= 0 && x < count && route_space_m2[y - 1][x] != 'b' && route_space_m2[y - 1][x] != '\0' && checkback(flag, x, y - 1, route_space_m2, x, y, count, sx, sy)) {//checking for north
								if (y - 1 >= 0 && y - 1 < count && x >= 0 && x < count && route_space_m1[y - 1][x] != 'b' && route_space_m1[y - 1][x] != '\0' && checkback(flag, x, y - 1, route_space_m1, x, y, count, sx, sy)) {//checking for north
									layer=0;
									flag = 1 - flag;
                                    					v3[0].push_back(x);
							        	v3[1].push_back(y-1);
									v5[0].push_back(x);
							        	v5[1].push_back(y-1);                   
								}else{
									flag = 1 - flag;
									dir_flag = 'n';
									v4[0].push_back(x);
									v4[1].push_back(y - 1);
								}
							}
							else if (y + 1 >= 0 && y + 1 < count && x >= 0 && x < count && route_space_m2[y + 1][x] != 'b' && route_space_m2[y + 1][x] != '\0' && checkback(flag, x, y + 1, route_space_m2, x, y, count, sx, sy)) {// check for south
								if (y + 1 >= 0 && y + 1 < count && x >= 0 && x < count && route_space_m1[y + 1][x] != 'b' && route_space_m1[y + 1][x] != '\0' && checkback(flag, x, y + 1, route_space_m1, x, y, count, sx, sy)) {// check for south
									layer=0;
									flag = 1 - flag;
                                    					v3[0].push_back(x);
									v3[1].push_back(y+1);
                                    					v5[0].push_back(x);
									v5[1].push_back(y+1);
                                                        //break;
								}
								else{
								flag = 1 - flag;
								dir_flag = 's';
								v4[0].push_back(x);
								v4[1].push_back(y + 1);
								}
							}
							else {
								layer = 0;
								v3[0].push_back(x);
								v3[1].push_back(y);
								v5[0].push_back(x);
								v5[1].push_back(y);
							}

						}
						else  if (dir_flag == 'e') {

							if (y >= 0 && y < count && x + 1 > 0 && x + 1 < count && route_space_m2[y][x + 1] != 'b' && route_space_m2[y][x + 1] != '\0' && checkback(flag, x + 1, y, route_space_m2, x, y, count, sx, sy)) {//check for east
								if (y >= 0 && y < count && x + 1 > 0 && x + 1 < count && route_space_m1[y][x + 1] != 'b' && route_space_m1[y][x + 1] != '\0' && checkback(flag, x + 1, y, route_space_m1, x, y, count, sx, sy)) {//check for east
								layer=0;
								flag = 1 - flag;
                                				v3[0].push_back(x+1);
								v3[1].push_back(y);
                               	 				v5[0].push_back(x+1);
								v5[1].push_back(y);           
								}
								else{
								flag = 1 - flag;
								dir_flag = 'e';
								v4[0].push_back(x + 1);
								v4[1].push_back(y);
								}
							}
							else if (y - 1 >= 0 && y - 1 < count && x >= 0 && x < count && route_space_m2[y - 1][x] != 'b' && route_space_m2[y - 1][x] != '\0' && checkback(flag, x, y - 1, route_space_m2, x, y, count, sx, sy)) {//checking for north
								if (y - 1 >= 0 && y - 1 < count && x >= 0 && x < count && route_space_m1[y - 1][x] != 'b' && route_space_m1[y - 1][x] != '\0' && checkback(flag, x, y - 1, route_space_m1, x, y, count, sx, sy)) {//checking for north
								layer=0;
								flag = 1 - flag;
                               	 				v3[0].push_back(x);
								v3[1].push_back(y-1);
                                				v5[0].push_back(x);
								v5[1].push_back(y-1);                          
								}
								else{
								flag = 1 - flag;
								dir_flag = 'n';
								v4[0].push_back(x);
								v4[1].push_back(y - 1);
								}								
							}
							else if (y + 1 >= 0 && y + 1 < count && x >= 0 && x < count && route_space_m2[y + 1][x] != 'b' && route_space_m2[y + 1][x] != '\0' && checkback(flag, x, y + 1, route_space_m2, x, y, count, sx, sy)) {// check for south
								if (y + 1 >= 0 && y + 1 < count && x >= 0 && x < count && route_space_m1[y + 1][x] != 'b' && route_space_m1[y + 1][x] != '\0' && checkback(flag, x, y + 1, route_space_m1, x, y, count, sx, sy)) {// check for south
								layer=0;
								flag = 1 - flag;
                                				v3[0].push_back(x);
								v3[1].push_back(y+1);
                                				v5[0].push_back(x);
								v5[1].push_back(y+1);                          
								}
								else{
								flag = 1 - flag;
								dir_flag = 's';
								v4[0].push_back(x);
								v4[1].push_back(y + 1);
								}
							}							
							else {
								layer = 0;
								v3[0].push_back(x);
								v3[1].push_back(y);
								v5[0].push_back(x);
								v5[1].push_back(y);
							}
						}
						else  if (dir_flag == 's') {
							//flag = 1 - flag;
							if (y + 1 > 0 && y + 1 < count && x >= 0 && x < count && route_space_m2[y + 1][x] != 'b' && route_space_m2[y + 1][x] != '\0' && checkback(flag, x, y + 1, route_space_m2, x, y, count, sx, sy)) {// check for south
								if (y + 1 >= 0 && y + 1 < count && x >= 0 && x < count && route_space_m1[y + 1][x] != 'b' && route_space_m1[y + 1][x] != '\0' && checkback(flag, x, y + 1, route_space_m1, x, y, count, sx, sy)) {// check for south
									layer=0;
									flag = 1 - flag;
                                    					v3[0].push_back(x);
									v3[1].push_back(y+1);
                                    					v5[0].push_back(x);
									v5[1].push_back(y + 1);
								}
								else{
								flag = 1 - flag;
								dir_flag = 's';
								v4[0].push_back(x);
								v4[1].push_back(y + 1);
								}
							}
							else if (y >= 0 && y < count && x + 1 >= 0 && x + 1 < count && route_space_m2[y][x + 1] != 'b' && route_space_m2[y][x + 1] != '\0' && checkback(flag, x + 1, y, route_space_m2, x, y, count, sx, sy)) {//check for east
								if (y >= 0 && y < count && x + 1 > 0 && x + 1 < count && route_space_m1[y][x + 1] != 'b' && route_space_m1[y][x + 1] != '\0' && checkback(flag, x + 1, y, route_space_m1, x, y, count, sx, sy)) {//check for east
								layer=0;
								flag = 1 - flag;
                                				v3[0].push_back(x+1);
								v3[1].push_back(y);
                                				v5[0].push_back(x+1);
								v5[1].push_back(y);
								}	
								else{
								flag = 1 - flag;
								dir_flag = 'e';
								v4[0].push_back(x + 1);
								v4[1].push_back(y);
								}
							}
							else if (y >= 0 && y < count && x - 1 >= 0 && x - 1 < count && route_space_m2[y][x - 1] != 'b' && route_space_m2[y][x - 1] != '\0' && checkback(flag, x - 1, y, route_space_m2, x, y, count, sx, sy)) {//checking for west
								if (y >= 0 && y < count && x - 1 >= 0 && x - 1 < count && route_space_m1[y][x - 1] != 'b' && route_space_m1[y][x - 1] != '\0' && checkback(flag, x - 1, y, route_space_m1, x, y, count, sx, sy)) {//checking for west
                                				layer=0;
								flag = 1 - flag;
                                				v3[0].push_back(x-1);
								v3[1].push_back(y);
                                				v5[0].push_back(x-1);
								v5[1].push_back(y);                            
								}
								else{
								flag = 1 - flag;
								dir_flag = 'w';
								v4[0].push_back(x - 1);
								v4[1].push_back(y);
								}
							}					
							else {
								layer = 0;
								v3[0].push_back(x);
								v3[1].push_back(y);
								v5[0].push_back(x);
								v5[1].push_back(y);
							}
						}

						else  if (dir_flag == 'n') {
							if (y - 1 >= 0 && y - 1 < count && x >= 0 && x < count && route_space_m2[y - 1][x] != 'b' && route_space_m2[y - 1][x] != '\0' && checkback(flag, x, y - 1, route_space_m2, x, y, count, sx, sy)) {//checking for north
								if (y - 1 >= 0 && y - 1 < count && x >= 0 && x < count && route_space_m1[y - 1][x] != 'b' && route_space_m1[y - 1][x] != '\0' && checkback(flag, x, y - 1, route_space_m1, x, y, count, sx, sy)) {//checking for north
								layer=0;
								flag = 1 - flag;
                                				v3[0].push_back(x);
								v3[1].push_back(y-1);
                                				v5[0].push_back(x);
								v5[1].push_back(y-1);				
								}
								else{
								flag = 1 - flag;
								dir_flag = 'n';
								v4[0].push_back(x);
								v4[1].push_back(y - 1);
								}
							}

							else if (y >= 0 && y < count && x + 1 >= 0 && x + 1 < count && route_space_m2[y][x + 1] != 'b' && route_space_m2[y][x + 1] != '\0' && checkback(flag, x + 1, y, route_space_m2, x, y, count, sx, sy)) {//check for east
								if (y >= 0 && y < count && x + 1 > 0 && x + 1 < count && route_space_m1[y][x + 1] != 'b' && route_space_m1[y][x + 1] != '\0' && checkback(flag, x + 1, y, route_space_m1, x, y, count, sx, sy)) {//check for east
								layer=0;
								flag = 1 - flag;
                                				v3[0].push_back(x+1);
								v3[1].push_back(y);
                                				v5[0].push_back(x+1);
								v5[1].push_back(y);
                                                       
								}
								else{
								flag = 1 - flag;
								dir_flag = 'e';
								v4[0].push_back(x + 1);
								v4[1].push_back(y);
								}
							}
							else if (y >= 0 && y < count && x - 1 >= 0 && x - 1 < count && route_space_m2[y][x - 1] != 'b' && route_space_m2[y][x - 1] != '\0' && checkback(flag, x - 1, y, route_space_m2, x, y, count, sx, sy)) {//checking for west
								if (y >= 0 && y < count && x - 1 >= 0 && x - 1 < count && route_space_m1[y][x - 1] != 'b' && route_space_m1[y][x - 1] != '\0' && checkback(flag, x - 1, y, route_space_m1, x, y, count, sx, sy)) {//checking for west
                                				layer=0;
								flag = 1 - flag;
                                				v3[0].push_back(x-1);
								v3[1].push_back(y);
                                				v5[0].push_back(x-1);
								v5[1].push_back(y);                                                       
								}
								else{
								flag = 1 - flag;
								dir_flag = 'w';
								v4[0].push_back(x - 1);
								v4[1].push_back(y);
								}
								}
							else {
								layer = 0;
								v3[0].push_back(x);
								v3[1].push_back(y);
								v5[0].push_back(x);
								v5[1].push_back(y);
							}
						}
					}
				}
		else{
			layer = 0;
			}
		}

	}
			///"end of Backtracing"<<"\n";
                        bool f=0;
			for (unsigned int i = 1; i < v3[0].size(); i++) {
				unsigned int vx = v3[0][i];
				unsigned int vy = v3[1][i];
				a3[0].push_back(vx);
				a3[1].push_back(vy);
				myfile<<"<< metal1 >>"<<endl;
      				myfile<<"rect "<<vx<<" "<<vy<<" "<<vx+1<<" "<<vy+1<<endl;
                                if(i==4){
                                f=1;
                                }
                                if(f==1){
                                f=0;
				myfile<<"<< labels >>"<<endl;
                                myfile<<"rlabel metal1 "<<vx<<" "<<vy<<" "<<vx+1<<" "<<vy+1<<" 0 net"<<nets1+1<<endl;
			}}
			for (unsigned int i = 1; i < v4[0].size(); i++) {

				unsigned int vx = v4[0][i];
				unsigned int vy = v4[1][i];
				a4[0].push_back(vx);
				a4[1].push_back(vy);
				myfile<<"<< metal2 >>"<<endl;				
				myfile<<"rect "<<vx<<" "<<vy<<" "<<vx+1<<" "<<vy+1<<endl;
                                if(i==4){
                                f=1;
                                }
                                if(i==v4[0].size()/2){
				myfile<<"<< labels >>"<<endl;
				myfile<<"rlabel metal2 "<<vx<<" "<<vy<<" "<<vx+1<<" "<<vy+1<<" 0 net"<<nets1+1<<endl;
			}}
		
		nets1++;
		}
		else{
		//std::cout << "\n" << "unrouted net = " << nets1<<"\n";
		unrouted++;
		alpha++;
		nets1++;
		}
		count = (s*(6 + channel_size) + channel_size);	
	}
    
for (unsigned int i = 0; i < v5[0].size(); i++) {
		unsigned int vx = v5[0][i];
		unsigned int vy = v5[1][i];
		myfile<<"<< m2contact >>"<<endl;
      	        myfile<<"rect "<<vx<<" "<<vy<<" "<<vx+1<<" "<<vy+1<<endl;
		} 
auto finish = std::chrono::high_resolution_clock::now();
std::chrono::duration<double>elapsed =finish-start;
//std::cout<<"Elapsed time in seconds:"<<elapsed.count()<<"s\n";
//std::cout<<"Elapsed time in minutes:"<<elapsed.count()/60<<"m\n";
}

