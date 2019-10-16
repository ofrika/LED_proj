#include <stdio.h>
#include <string>

#define BUFFSIZE 8192

int parseInit(char* buff)
{
	printf("Hello operator!\nWould you like to enter initialization settings? (Answer y/n): ");
	if (scanf_s("%s", buff, BUFFSIZE) != -1)
	{
		if (strcmp(buff, "n") == 0)
		{
			printf("\n***************************************************\n");
			printf("Initializing board with current default settings:\n");
			printf("-------- #Ports = 4 ------------------------------\n");
			printf("-------- #Matrices in each port = 8,8,8,8 --------\n");
			printf("-------- All matrices are directed up ------------\n");
			// LedSignResult res = initBoard(4, arr, "");
			int res = 0;
			if (res == 0) return 0;
			else return 1;
		}
		if (strcmp(buff, "y") == 0)
		{
			int numPorts, i;
			char* directions;
			printf("Please enter number of ports: ");
			scanf_s("%s", buff, BUFFSIZE);
			numPorts = atoi(buff);
			int *ports = (int*)malloc(sizeof(int)*numPorts);
			for (i = 0; i < numPorts; i++)
			{
				printf("Please enter number of matrices in port #%d: ", i+1);
				scanf_s("%s", buff, BUFFSIZE);
				ports[i] = atoi(buff);
			}
			printf("Please enter indeces of rotated matrices: (Answer {x1,y1,dir1;x2,y2,dir2;...}): ");
			scanf_s("%s", buff, BUFFSIZE);
			printf("\n***************************************************\n");
			printf("Initializing board with current received settings:\n");
			printf("-------- #Ports = %d ------------------------------\n", numPorts);
			printf("-------- #Matrices in each port = ");
			for (i = 0; i < numPorts; i++)
			{
				if (i != 0) printf(",");
				printf("%d", ports[i]);
			}
			printf(" --------\n");
			if (strcmp(buff, "{}") == 0)
				printf("-------- All matrices are directed up ------------\n");
			else
				printf("-------- Directions: %s ------------\n", buff);
			// LedSignResult res = initBoard(numPorts, ports, buff);
			int res = 0;
			if (res == 0) return 0;
			else return 1;
		}
		else return 1;
	}
}

int translateCommand(char* com)
{
	if (strcmp(com, "Init") == 0)
		return 0;
	if (strcmp(com, "Exit") == 0)
		return 1;
	if (strcmp(com, "Add_sub_board") == 0)
		return 2;
	if (strcmp(com, "Clear_sub_board") == 0)
		return 3;
	if (strcmp(com, "Delete_sub_board") == 0)
		return 4;
	if (strcmp(com, "Add_text_object") == 0)
		return 5;
	if (strcmp(com, "Add_picture_object") == 0)
		return 6;
	if (strcmp(com, "Update_text_object") == 0)
		return 7;
	if (strcmp(com, "Update_picture_object") == 0)
		return 8;
	if (strcmp(com, "Add_picture_to_database") == 0)
		return 9;
	if (strcmp(com, "Delete_object") == 0)
		return 10;
	if (strcmp(com, "Get_board_status") == 0)
		return 11;

	return -1;
}

int main(int argc, char *argv[])
{
	char buff[BUFFSIZE];
	int isDone = 0;

	if (parseInit(buff) == 0)
	{
		printf("Init succeeded!\n***************************************************\n\n");
		do {
			printf("What would you like to do next? ");
			scanf_s("%s", buff, BUFFSIZE);
			int com = translateCommand(buff);
			switch (com)
			{
				case 0: // Initialize board
				{
					if (parseInit(buff) == 0)
						printf("Init succeeded!\n***************************************************\n\n");
					break;
				}
				case 1: // Destroy board
				{
					isDone = 1;
					// LedSignResult res = destroyBoard();
					int res = 0;
					if (res == 0)
					{
						printf("\n***************************************************\n");
						printf("Board destruction succeeded!\n");
						printf("***************************************************\n\n");
					}
					else
					{
						printf("Board destruction failed! Please restart the machine.\n");
						return 1;
					}
					break;
				}
				case 2: // Add sub board
				{
					printf("Please enter sub board ID: ");
					scanf_s("%s", buff, BUFFSIZE);
					int id = atoi(buff);
					printf("Please enter location x: ");
					scanf_s("%s", buff, BUFFSIZE);
					int x = atoi(buff);
					printf("Please enter location y: ");
					scanf_s("%s", buff, BUFFSIZE);
					int y = atoi(buff);
					printf("Please enter width: ");
					scanf_s("%s", buff, BUFFSIZE);
					int len_x = atoi(buff);
					printf("Please enter length: ");
					scanf_s("%s", buff, BUFFSIZE);
					int len_y = atoi(buff);
					printf("\n***************************************************\n");
					printf("Adding sub-board with the following parameters:\n");
					printf("-------- ID = %d -------------------------------\n", id);
					printf("-------- Start location = %d, %d --------------\n", x, y);
					printf("-------- End location = %d, %d ----------------\n", x+len_x, y+len_y);
					// LedSignResult res = addSubBoard(id, x, y, len_x, len_y);
					int res = 0;
					if (res == 0) printf("Adding sub-board succeeded!\n");
					else printf("Adding sub-board failed!");
					printf("***************************************************\n\n");
					break;
				}
				case 3: // Clear sub board
				{
					printf("Please enter sub board ID: ");
					scanf_s("%s", buff, BUFFSIZE);
					int id = atoi(buff);
					// LedSignResult res = cleanSubBoard(id);
					int res = 0;
					printf("\n***************************************************\n");
					printf("Clearing sub-board #%d\n", id);
					if (res == 0) printf("Board clearing succeeded!\n");
					else printf("Board clearing failed!\n");
					printf("***************************************************\n\n");
					break;
				}
				case 4: // Delete sub board
				{
					printf("Please enter sub board ID: ");
					scanf_s("%s", buff, BUFFSIZE);
					int id = atoi(buff);
					// LedSignResult res = deleteSubBoard(id);
					int res = 0;
					printf("\n***************************************************\n");
					printf("Deleting sub-board #%d\n", id);
					if (res == 0) printf("Board deleting succeeded!\n");
					else printf("Board deleting failed!\n");
					printf("***************************************************\n\n");
					break;
				}
				case 5: // Add text object
				{
					printf("Please enter text object ID: ");
					scanf_s("%s", buff, BUFFSIZE);
					int id = atoi(buff);
					printf("Please enter sub board ID (Where object is located): ");
					scanf_s("%s", buff, BUFFSIZE);
					int id_board = atoi(buff);
					printf("Please enter location x: ");
					scanf_s("%s", buff, BUFFSIZE);
					int x = atoi(buff);
					printf("Please enter location y: ");
					scanf_s("%s", buff, BUFFSIZE);
					int y = atoi(buff);
					printf("Please enter width: ");
					scanf_s("%s", buff, BUFFSIZE);
					int len_x = atoi(buff);
					printf("Please enter length: ");
					scanf_s("%s", buff, BUFFSIZE);
					int len_y = atoi(buff);
					// Check if text object is in sub-board dimensions
					// if (exceeds dimensions)
					//		printf("Oops! text object exceeds sub-board dimensions.\nAction canceled");
					printf("Please enter text color - R: ");
					scanf_s("%s", buff, BUFFSIZE);
					unsigned char r = atoi(buff);
					printf("Please enter text color - G: ");
					scanf_s("%s", buff, BUFFSIZE);
					unsigned char g = atoi(buff);
					printf("Please enter text color - B: ");
					scanf_s("%s", buff, BUFFSIZE);
					unsigned char b = atoi(buff);
					printf("Should the text be scrollable? (Answer y/n): ");
					scanf_s("%s", buff, BUFFSIZE);
					int scrollable;
					if (strcmp(buff, "y") == 0)
						scrollable = 1;
					else
						scrollable = 0;
					printf("\n***************************************************\n");
					printf("Adding text object with the following parameters:\n");
					printf("-------- ID = %d -----------------------------------\n", id);
					if (scrollable) printf("-------- Text is scrollable ------------------------\n");
					else printf("-------- Text is not scrollable --------------------\n");
					printf("-------- ID of containing sub-board = %d -----------\n", id_board);
					printf("-------- Start location = %d, %d -------------------\n", x, y);
					printf("-------- End location = %d, %d ---------------------\n", x + len_x, y + len_y);
					printf("-------- RGB = %d,%d,%d ---------------------------\n", r,g,b);
					// LedSignResult res = addText(id, id_board, x, y, len_x, len_y);
					int res = 0;
					if (res == 0) printf("Adding text object succeeded!\n");
					else printf("Adding text object failed!\n");
					printf("***************************************************\n\n");
					break;
				}
				case 6: // Add picture object
				{
					printf("Please enter picture object ID: ");
					scanf_s("%s", buff, BUFFSIZE);
					int id = atoi(buff);
					printf("Please enter sub board ID (Where object is located): ");
					scanf_s("%s", buff, BUFFSIZE);
					int id_board = atoi(buff);
					printf("Please enter location x: ");
					scanf_s("%s", buff, BUFFSIZE);
					int x = atoi(buff);
					printf("Please enter location y: ");
					scanf_s("%s", buff, BUFFSIZE);
					int y = atoi(buff);
					printf("Please enter width: ");
					scanf_s("%s", buff, BUFFSIZE);
					int len_x = atoi(buff);
					printf("Please enter length: ");
					scanf_s("%s", buff, BUFFSIZE);
					int len_y = atoi(buff);
					// Check if picture object is in sub-board dimensions
					// if (exceeds dimensions)
					//		printf("Oops! text object exceeds sub-board dimensions.\nAction canceled.\n");
					printf("Do you want to select color for the picture? (e.g. for arrows) Answer y/n: ");
					scanf_s("%s", buff, BUFFSIZE);
					int withColor;
					if (strcmp(buff, "y") == 0)
						withColor = 1;
					else
						withColor = 0;
					unsigned char r, g, b;
					if (withColor)
					{
						printf("Please enter text color - R: ");
						scanf_s("%s", buff, BUFFSIZE);
						r = atoi(buff);
						printf("Please enter text color - G: ");
						scanf_s("%s", buff, BUFFSIZE);
						g = atoi(buff);
						printf("Please enter text color - B: ");
						scanf_s("%s", buff, BUFFSIZE);
						b = atoi(buff);
					}
					printf("\n***************************************************\n");
					printf("Adding picture object with the following parameters:\n");
					printf("-------- ID = %d -----------------------------------\n", id);
					printf("-------- ID of containing sub-board = %d -----------\n", id_board);
					printf("-------- Start location = %d, %d -------------------\n", x, y);
					printf("-------- End location = %d, %d ---------------------\n", x + len_x, y + len_y);
					if (withColor) printf("-------- RGB = %d,%d,%d ---------------------------\n", r, g, b);
					// LedSignResult res = addPicture(id, id_board,x, y, len_x, len_y);
					int res = 0;
					if (res == 0) printf("Adding picture object succeeded!\n");
					else printf("Adding picture object failed!\n");
					printf("***************************************************\n\n");
					break;
				}
				case 7: // Update text object
				{
					printf("Please enter sub board ID (Where object is located): ");
					scanf_s("%s", buff, BUFFSIZE);
					int id_board = atoi(buff);
					printf("Please enter text object ID: ");
					scanf_s("%s", buff, BUFFSIZE);
					int id = atoi(buff);
					printf("Please enter text string: ");
					scanf_s("%[^\n]", buff);
					printf("\n***************************************************\n");
					printf("Printing text with the following parameters:\n");
					printf("-------- ID of text object = %d --------------------\n", id);
					printf("-------- ID of containing sub-board = %d -----------\n", id_board);
					printf("-------- String value: %s ----------------\n", buff);
					// LedSignResult res = updateText(id_board, id, buff);
					int res = 0;
					if (res == 0) printf("Printing text succeeded!\n");
					else printf("Printing text failed!");
					printf("***************************************************\n\n");
					break;
				}
				case 8: // Update picture object
				{
					printf("Please enter sub board ID (Where object is located): ");
					scanf_s("%s", buff, BUFFSIZE);
					int id_board = atoi(buff);
					printf("Please enter picture object ID: ");
					scanf_s("%s", buff, BUFFSIZE);
					int id = atoi(buff);
					printf("Please enter index of picture in database: ");
					scanf_s("%s", buff, BUFFSIZE);
					int index = atoi(buff);
					printf("\n***************************************************\n");
					printf("Printing picture with the following parameters:\n");
					printf("-------- ID of picture object = %d ------------------\n", id);
					printf("-------- ID of containing sub-board = %d ------------\n", id_board);
					printf("-------- Index = %d --------------------------------\n", index);
					// LedSignResult res = updatePicture(id_board, id, index);
					int res = 0;
					if (res == 0) printf("Printing picture succeeded!\n");
					else printf("Printing picture failed!");
					printf("***************************************************\n\n");
					break;
				}
				case 9: // Add picture to database
				{
					printf("Please enter width: ");
					scanf_s("%s", buff, BUFFSIZE);
					int width = atoi(buff);
					printf("Please enter length: ");
					scanf_s("%s", buff, BUFFSIZE);
					int length = atoi(buff);
					printf("Please enter index of picture for database: ");
					scanf_s("%s", buff, BUFFSIZE);
					int index = atoi(buff);
					printf("Please enter rgb data: (Write {r1,g1,b1;r2,g2,b2;...})");
					scanf_s("%s", buff, BUFFSIZE);
					printf("\n***************************************************\n");
					printf("Adding picture with the following parameters to database:\n");
					printf("-------- Dimensions = %dx%d ------------------------\n", width,length);
					printf("-------- Index = %d --------------------------------\n", index);
					// LedSignResult res = updatePicture(id_board, id, index);
					int res = 0;
					if (res == 0) printf("Adding picture to database succeeded!\n");
					else printf("Adding picture to database failed!");
					printf("***************************************************\n\n");
					break;
				}
				case 10: // Delete object
				{
					printf("Please enter sub board ID (Where object is located): ");
					scanf_s("%s", buff, BUFFSIZE);
					int id_board = atoi(buff);
					printf("Please enter object ID: ");
					scanf_s("%s", buff, BUFFSIZE);
					int id = atoi(buff);
					// LedSignResult res = deleteObject(id_board, id);
					int res = 0;
					printf("\n***************************************************\n");
					printf("Deleting object #%d in sub-board #%d\n", id, id_board);
					if (res == 0) printf("Object deleting succeeded!\n");
					else printf("Object deleting failed!\n");
					printf("***************************************************\n\n");
					break;
				}
				case 11: // Get status
				{
					// getStatus();
					break;
				}
				default:
					printf("Invalid command!\n");
					break;
			}
			
				
		} while (isDone == 0);
	}
	else
	{
		printf("Init failed! Please restart program.\n");
		return 1;
	}

	return 0;
}