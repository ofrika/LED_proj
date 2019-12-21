#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "Parser.h"

#define MAX_LEN 10

int get_next_numerical_input(char* orig_comm, int start_from, int len, int* end)
{
	char *e_start;
	char *e_end;
	char *comm;
	int i;
	if (start_from == 0)
	{
		comm = malloc(sizeof(char)*(len));
		for (i = 0; i < len; i++)
			comm[i] = orig_comm[i];
		e_start = strchr(comm, '(');
	}
	else
	{
		comm = malloc(sizeof(char)*(len - start_from));
		for (i = 0; i < len - start_from; i++)
			comm[i] = orig_comm[i + start_from];
		e_start = strchr(comm, ',');
	}
	int index_start = (int)(e_start - comm) + 1;
	char *substr = malloc(sizeof(char)*(len - index_start));
	for (i = 0; i < len - index_start; i++)
		substr[i] = comm[i + index_start];
	e_end = strchr(substr, ',');
	int index_end = (int)(e_end - substr) + index_start;
	char *final = malloc(sizeof(char)*(index_end - index_start));
	for (i = 0; i < index_end - index_start; i++)
		final[i] = comm[i + index_start];
	int out = atoi(substr);
	free(substr);
	free(final);
	(*end) = start_from + index_end - 1;
	return  out;
}

int get_array_input(char* orig_comm, int start_from, int* end, int* array_out, int full_len)
{
	char *e_start;
	char *e_end;
	char *comm;
	int i = 0, j = 0, pos = 0, count_cells = 0, count_len = 0;
	char buff[MAX_LEN];
	for (j = 0; j < MAX_LEN; j++)
		buff[j] = '\0';
	for (j = 0; j < full_len; j++)
		array_out[j] = -1;
	while (orig_comm[start_from + i] != '\0') {
		if (orig_comm[start_from + i] == '{')
			pos = i;
		if (orig_comm[start_from + i] == '}') {
			(*end) = start_from + i + 1;
			break;
		}
		i++;
	}
	for (i = start_from + pos + 1; i < (*end); i++) {
		if ((orig_comm[i] != ',') && (orig_comm[i] != '}')) {
			buff[count_len] = orig_comm[i];
			count_len++;
		}
		else {
			count_len = 0;
			sscanf(buff, "%d", &array_out[count_cells]);
			count_cells++;
			for (j = 0; j < MAX_LEN; j++)
				buff[j] = '\0';
		}
	}
	return count_cells;
}

void check_error(LedSignResult res, char* buff_out){
	if(res != LED_SIGN_SUCCESS){
		switch(res){
			case LED_SIGN_OUT_OF_MEMORY:
				xil_printf("ERROR: LED_SIGN_OUT_OF_MEMORY\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_OUT_OF_MEMORY\n\n");
				break;
			case LED_SIGN_ILLEGAL_ARGUMENTS:
				xil_printf("ERROR: LED_SIGN_ILLEGAL_ARGUMENTS\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_ILLEGAL_ARGUMENTS\n\n");
				break;
			case LED_SIGN_ERROR_WHILE_PARSING_DIRECTIONS:
				xil_printf("ERROR: LED_SIGN_ERROR_WHILE_PARSING_DIRECTIONS\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_ERROR_WHILE_PARSING_DIRECTIONS\n\n");
				break;
			case LED_SIGN_IMAGE_DOESNT_EXIST_IN_STOCK:
				xil_printf("ERROR: LED_SIGN_IMAGE_DOESNT_EXIST_IN_STOCK\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_IMAGE_DOESNT_EXIST_IN_STOCK\n\n");
				break;
			case LED_SIGN_OUT_OF_BOARD_COARDINATES:
				xil_printf("ERROR: LED_SIGN_OUT_OF_BOARD_COARDINATES\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_OUT_OF_BOARD_COARDINATES\n\n");
				break;
			case LED_SIGN_DISPLAY_CAN_NOT_CONTAIN_THAT_TEXT:
				xil_printf("ERROR: LED_SIGN_DISPLAY_CAN_NOT_CONTAIN_THAT_TEXT\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_DISPLAY_CAN_NOT_CONTAIN_THAT_TEXT\n\n");
				break;
			case LED_SIGN_DISPLAY_CAN_NOT_CONTAIN_THAT_PICTURE:
				xil_printf("ERROR: LED_SIGN_DISPLAY_CAN_NOT_CONTAIN_THAT_PICTURE\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_DISPLAY_CAN_NOT_CONTAIN_THAT_PICTURE\n\n");
				break;
			case LED_SIGN_DISPLAY_ID_ALREADY_EXIST:
				xil_printf("ERROR: LED_SIGN_DISPLAY_ID_ALREADY_EXIST\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_DISPLAY_ID_ALREADY_EXIST\n\n");
				break;
			case LED_SIGN_OBJECT_ID_ALREADY_EXIST:
				xil_printf("ERROR: LED_SIGN_OBJECT_ID_ALREADY_EXIST\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_OBJECT_ID_ALREADY_EXIST\n\n");
				break;
			case LED_SIGN_ANOTHER_DISPLAY_LOCATED_THERE:
				xil_printf("ERROR: LED_SIGN_ANOTHER_DISPLAY_LOCATED_THERE\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_ANOTHER_DISPLAY_LOCATED_THERE\n\n");
				break;
			case LED_SIGN_ANOTHER_OBJECT_LOCATED_THERE:
				xil_printf("ERROR: LED_SIGN_ANOTHER_OBJECT_LOCATED_THERE\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_ANOTHER_OBJECT_LOCATED_THERE\n\n");
				break;
			case LED_SIGN_OBJECT_WITH_THE_GIVEN_ID:
				xil_printf("ERROR: LED_SIGN_OBJECT_WITH_THE_GIVEN_ID\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_OBJECT_WITH_THE_GIVEN_ID\n\n");
				break;
			case LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID:
				xil_printf("ERROR: LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID\n\n");
				break;
			case LED_SIGN_NO_PICTURE_WITH_THE_GIVEN_ID:
				xil_printf("ERROR: LED_SIGN_NO_PICTURE_WITH_THE_GIVEN_ID\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_NO_PICTURE_WITH_THE_GIVEN_ID\n\n");
				break;
			case LED_SIGN_NO_TEXT_WITH_THE_GIVEN_ID:
				xil_printf("ERROR: LED_SIGN_NO_TEXT_WITH_THE_GIVEN_ID\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_NO_TEXT_WITH_THE_GIVEN_ID\n\n");
				break;
			case LED_SIGN_PICTURE_DIMENSIONS_ARE_TOO_MUCH_SMALL:
				xil_printf("ERROR: LED_SIGN_PICTURE_DIMENSIONS_ARE_TOO_MUCH_SMALL\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: LED_SIGN_PICTURE_DIMENSIONS_ARE_TOO_MUCH_SMALL\n\n");
				break;
			default:
				xil_printf("ERROR: Unkown error\n\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"ERROR: Unkown error\n\n");
		}
	}
}

int translateCommand(char* com)
{
	if (strstr(com, "Init") != NULL)
		return 0;
	if (strstr(com, "Exit") != NULL)
		return 1;
	if (strstr(com, "Add_sub_board") != NULL)
		return 2;
	if (strstr(com, "Clear_sub_board") != NULL)
		return 3;
	if (strstr(com, "Delete_sub_board") != NULL)
		return 4;
	if (strstr(com, "Add_text_area") != NULL)
		return 5;
	if (strstr(com, "Add_picture_area") != NULL)
		return 6;
	if (strstr(com, "Insert_text") != NULL)
		return 7;
	if (strstr(com, "Insert_picture") != NULL)
		return 8;
	if (strstr(com, "Add_picture_to_db") != NULL)
		return 9;
	if (strstr(com, "Delete_area") != NULL)
		return 10;
	if (strstr(com, "Get_status") != NULL)
		return 11;
	if (strstr(com, "Change_text_color") != NULL)
		return 12;
	if (strstr(com, "Change_picture_color") != NULL)
		return 13;
	if (strstr(com, "Flip_down") != NULL)
		return 14;
	if (strstr(com, "Flip_right") != NULL)
		return 15;
	if (strstr(com, "Draw_frame") != NULL)
		return 16;
	if (strstr(com, "Test_running_pixel") != NULL)
		return 17;
	return -1;
}

int parseMessage(char* input, char* buff_out)
{
	int com = translateCommand(input);
	int len = 0, end = 0, k = 0;
	strcpy(buff_out, "");
	while (input[len] != '\0')
		len++;
	xil_printf("\n***************************************************\n");
	switch (com)
	{
		case 0: // Initialize board
		{
			if (strstr(input, "default") != NULL)
			{
				xil_printf("Initializing board with current default settings:\n");
				xil_printf("-------- #Ports = 4 ------------------------------\n");
				xil_printf("-------- #Matrices in each port = 8,8,8,8 --------\n");
				xil_printf("-------- All matrices are directed right ----------\n");
				int arr[] = { 8,8,8,8 };
				LedSignResult res = initBoard(4, arr, "");
				if (res != LED_SIGN_SUCCESS)
				{
					xil_printf("Init ERROR! Terminating program. Please restart machine\n");
					check_error(res, buff_out);
					strcat(buff_out,"Init failed! Terminating program.");
					destroyBoard();
					return 1;
				}
				else{
					xil_printf("Init succeeded!\n***************************************************\n\n");
					strcpy(buff_out,"SUCCESS");
				}
			}
			else
			{
				int num_ports = get_next_numerical_input(input, end, len, &end);
				int* tmp = malloc(sizeof(int)*(strlen(input)));
				int port_arr_size = get_array_input(input, end, &end, tmp, strlen(input));
				int* clean_port_arr = malloc(sizeof(int)*port_arr_size);
				for (k = 0; k < port_arr_size; k++)
					clean_port_arr[k] = tmp[k];
				char* dir = malloc(sizeof(char)*((strlen(input) - end - 3)));
				for (k = 0; k < (strlen(input) - end - 4); k++)
					dir[k] = input[end + 2 + k];
				dir[k] = '\0';
				xil_printf("Initializing board with current received settings:\n");
				xil_printf("-------- #Ports = %d ------------------------------\n", num_ports);
				xil_printf("-------- #Matrices in each port = ");
				for (k = 0; k < num_ports; k++)
				{
					if (k != 0) xil_printf(",");
					xil_printf("%d", clean_port_arr[k]);
				}
				xil_printf(" --------\n");
				xil_printf("-------- Rotated matrices = %s", dir);
				if (strcmp(dir, "") == 0)
					xil_printf("None");
				xil_printf(" --------\n");
				LedSignResult res = initBoard(num_ports, clean_port_arr, dir);
				free(dir);
				free(tmp);
				if (res != LED_SIGN_SUCCESS)
				{
					xil_printf("Init ERROR! Please restart machine\n");
					check_error(res, buff_out);
					destroyBoard();
					return 1;
				}
				else{
					xil_printf("Init succeeded!\n***************************************************\n\n");
					strcpy(buff_out,"SUCCESS");
				}
			}
			break;
		}
		case 1: // Destroy board
		{
			destroyBoard();
			xil_printf("Exit succeeded! Terminating program. You can turn off machine now.\n");
			xil_printf("***************************************************\n\n");
			strcpy(buff_out,"SUCCESS. Terminating program.");
			return 1;
			break;
		}
		case 2: // Add sub board
		{
			int id = get_next_numerical_input(input, end, len, &end);
			int x = get_next_numerical_input(input, end, len, &end);
			int y = get_next_numerical_input(input, end, len, &end);
			int len_x = get_next_numerical_input(input, end, len, &end);
			int len_y = get_next_numerical_input(input, end, len, &end);
			xil_printf("Adding sub-board with the following parameters:\n");
			xil_printf("-------- ID = %d -------------------------------\n", id);
			xil_printf("-------- Start location = %d, %d --------------\n", x, y);
			xil_printf("-------- End location = %d, %d ----------------\n", x + len_x, y + len_y);
			LedSignResult res = addSubBoard(id, x, y, len_x, len_y);
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Adding sub-board failed!");
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Adding sub-board succeeded!\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
			break;
		}
		case 3: // Clear sub board
		{
			int id = get_next_numerical_input(input, end, len, &end);
			LedSignResult res = cleanSubBoard(id);
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Clearing sub-board #%d failed!", id);
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Clearing sub-board #%d succeeded!\n", id);
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
			break;
		}
		case 4: // Delete sub board
		{
			int id = get_next_numerical_input(input, end, len, &end);
			LedSignResult res = deleteSubBoard(id);
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Deleting sub-board #%d failed!", id);
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Deleting sub-board #%d succeeded!\n", id);
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
			break;
		}
		case 5: // Add text object (area)
		{
			int id_board = get_next_numerical_input(input, end, len, &end);
			int id = get_next_numerical_input(input, end, len, &end);
			int x = get_next_numerical_input(input, end, len, &end);
			int y = get_next_numerical_input(input, end, len, &end);
			int len_x = get_next_numerical_input(input, end, len, &end);
			int len_y = get_next_numerical_input(input, end, len, &end);
			unsigned char r = (unsigned char)get_next_numerical_input(input, end, len, &end);
			unsigned char g = (unsigned char)get_next_numerical_input(input, end, len, &end);
			unsigned char b = (unsigned char)get_next_numerical_input(input, end, len, &end);
			int scrollable_in = get_next_numerical_input(input, end, len, &end);
			bool scrollable;
			if (scrollable_in)
				scrollable = 1;
			else
				scrollable = 0;
			xil_printf("Adding text area with the following parameters:\n");
			xil_printf("-------- ID = %d -----------------------------------\n", id);
			if (scrollable) xil_printf("-------- Text is scrollable ------------------------\n");
			else xil_printf("-------- Text is not scrollable --------------------\n");
			xil_printf("-------- ID of containing sub-board = %d -----------\n", id_board);
			xil_printf("-------- Start location = %d, %d -------------------\n", x, y);
			xil_printf("-------- End location = %d, %d ---------------------\n", x + len_x, y + len_y);
			xil_printf("-------- RGB = %d,%d,%d ---------------------------\n", r, g, b);
			LedSignResult res = createTextArea(id_board, id, x, y, len_x, len_y, r, g, b, scrollable);
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Adding text area failed!");
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Adding text area succeeded!\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
			break;
		}
		case 6: // Add picture object (area)
		{
			int id_board = get_next_numerical_input(input, end, len, &end);
			int id = get_next_numerical_input(input, end, len, &end);
			int x = get_next_numerical_input(input, end, len, &end);
			int y = get_next_numerical_input(input, end, len, &end);
			int len_x = get_next_numerical_input(input, end, len, &end);
			int len_y = get_next_numerical_input(input, end, len, &end);
			xil_printf("Adding picture area with the following parameters:\n");
			xil_printf("-------- ID = %d -----------------------------------\n", id);
			xil_printf("-------- ID of containing sub-board = %d -----------\n", id_board);
			xil_printf("-------- Start location = %d, %d -------------------\n", x, y);
			xil_printf("-------- End location = %d, %d ---------------------\n", x + len_x, y + len_y);
			LedSignResult res = createPictureArea(id_board, id, x, y, len_x, len_y);
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Adding picture area failed!");
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Adding picture area succeeded!\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
			break;
		}
		case 7: // Print (insert/update) text
		{
			int id_board = get_next_numerical_input(input, end, len, &end);
			int id = get_next_numerical_input(input, end, len, &end);
			int* tmp = malloc(sizeof(int)*(strlen(input)));
			int arr_size = get_array_input(input, end, &end, tmp, strlen(input));
			int* clean_arr = malloc(sizeof(int)*arr_size);
			for (k = 0; k < arr_size; k++)
				clean_arr[k] = tmp[k];
			free(tmp);
			xil_printf("Printint text with the following parameters:\n");
			xil_printf("-------- ID = %d ------------------------------------\n", id);
			xil_printf("-------- ID of containing sub-board = %d -----------\n", id_board);
			xil_printf("-------- Text string = ");
			for (k = 0; k < arr_size; k++)
				if (k != arr_size - 1)
					xil_printf("%d,", clean_arr[k]);
				else
					xil_printf("%d ", clean_arr[k]);
			xil_printf("------------------\n");
			LedSignResult res = updateText(id_board, id, clean_arr, arr_size);
			free(clean_arr);
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Printing text failed!");
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Printing text succeeded! Check out the board :)\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
			break;
		}
		case 8: // Print picture object
		{
			int id_board = get_next_numerical_input(input, end, len, &end);
			int id = get_next_numerical_input(input, end, len, &end);
			int index = get_next_numerical_input(input, end, len, &end);
			xil_printf("Printing picture with the following parameters:\n");
			xil_printf("-------- ID = %d -----------------------------------\n", id);
			xil_printf("-------- ID of containing sub-board = %d -----------\n", id_board);
			xil_printf("-------- Index = %d --------------------------------\n", index);
			LedSignResult res = updatePicture(id_board, id, index);
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Printing picture object failed!");
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Printing picture object succeeded! Check out the board :)\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
			break;
		}
		case 9: // Add picture to database
		{
			int index = get_next_numerical_input(input, end, len, &end);
			int width = get_next_numerical_input(input, end, len, &end);
			int length = get_next_numerical_input(input, end, len, &end);
			// Get R data
			int* tmp = malloc(sizeof(int)*(strlen(input)));
			int arr_size_r = get_array_input(input, end, &end, tmp, strlen(input));
			byte* clean_arr_r = malloc(sizeof(int)*arr_size_r);
			for (k = 0; k < arr_size_r; k++)
				clean_arr_r[k] = (byte)tmp[k];
			// Get G data
			int arr_size_g = get_array_input(input, end, &end, tmp, strlen(input));
			byte* clean_arr_g = malloc(sizeof(int)*arr_size_g);
			for (k = 0; k < arr_size_g; k++)
				clean_arr_g[k] = (byte)tmp[k];
			// Get B data
			int arr_size_b = get_array_input(input, end, &end, tmp, strlen(input));
			byte* clean_arr_b = malloc(sizeof(int)*arr_size_b);
			for (k = 0; k < arr_size_b; k++)
				clean_arr_b[k] = (byte)tmp[k];
			xil_printf("Adding image to database with the following parameters:\n");
			xil_printf("-------- Index = %d ---------------------------------\n", index);
			xil_printf("-------- Dimensions = %dx%d --------------------------\n", width, length);
			xil_printf("-------- R data = {");
			for (k = 0; k < arr_size_r; k++)
				if (k != arr_size_r - 1)
					xil_printf("%d,", clean_arr_r[k]);
				else
					xil_printf("%d", clean_arr_r[k]);
			xil_printf("} ------------------\n-------- G data = {");
			for (k = 0; k < arr_size_g; k++)
				if (k != arr_size_g - 1)
					xil_printf("%d,", clean_arr_g[k]);
				else
					xil_printf("%d", clean_arr_g[k]);
			xil_printf("} ------------------\n-------- B data = {");
			for (k = 0; k < arr_size_b; k++)
				if (k != arr_size_b - 1)
					xil_printf("%d,", clean_arr_b[k]);
				else
					xil_printf("%d", clean_arr_b[k]);
			xil_printf("} ------------------\n");
			LedSignResult res = addImageToDB(index, length, width, clean_arr_r, clean_arr_g, clean_arr_b);
			free(tmp);
			free(clean_arr_r);
			free(clean_arr_g);
			free(clean_arr_b);
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Adding image to database failed!");
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Adding image to database succeeded!\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
			break;
		}
		case 10: // Delete object (area)
		{
			int id_board = get_next_numerical_input(input, end, len, &end);
			int id = get_next_numerical_input(input, end, len, &end);
			LedSignResult res = deleteArea(id_board, id);
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Deleting area #%d failed!", id);
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Deleting area #%d succeeded!\n", id);
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
			break;
		}
		case 11: // Get status
		{
			getStatus(buff_out);
			break;
		}
		case 12: // Change text color
		{
			int id_board = get_next_numerical_input(input, end, len, &end);
			int id = get_next_numerical_input(input, end, len, &end);
			unsigned char r = (unsigned char)get_next_numerical_input(input, end, len, &end);
			unsigned char g = (unsigned char)get_next_numerical_input(input, end, len, &end);
			unsigned char b = (unsigned char)get_next_numerical_input(input, end, len, &end);
			xil_printf("Changing text color with the following parameters:\n");
			xil_printf("-------- ID = %d -----------------------------------\n", id);
			xil_printf("-------- ID of containing sub-board = %d -----------\n", id_board);
			xil_printf("-------- RGB = %d,%d,%d ---------------------------\n", r, g, b);
			LedSignResult res = updateTextColor(id_board, id, r, g, b);
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Changing text color failed!");
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Changing text color succeeded!\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
			break;
		}
		case 13: // Change picture color
		{
			int id_board = get_next_numerical_input(input, end, len, &end);
			int id = get_next_numerical_input(input, end, len, &end);
			unsigned char r = (unsigned char)get_next_numerical_input(input, end, len, &end);
			unsigned char g = (unsigned char)get_next_numerical_input(input, end, len, &end);
			unsigned char b = (unsigned char)get_next_numerical_input(input, end, len, &end);
			xil_printf("Changing picture color with the following parameters:\n");
			xil_printf("-------- ID = %d -----------------------------------\n", id);
			xil_printf("-------- ID of containing sub-board = %d -----------\n", id_board);
			xil_printf("-------- RGB = %d,%d,%d ---------------------------\n", r, g, b);
			LedSignResult res = updatePictureColor(id_board, id, r, g, b);
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Changing picture color failed!");
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Changing picture color succeeded!\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
			break;
		}
		case 14: // Flip down
		{
			LedSignResult res = FlipDown();
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Flipping down failed!");
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Flipping down succeeded! Check out board :)\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
		}
		case 15: // Flip right
		{
			LedSignResult res = FlipRight();
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Flipping right failed!");
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Flipping right succeeded! Check out board :)\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
			break;
		}
		case 16: // Draw frame
		{
			LedSignResult res = drawFrame();
			if (res != LED_SIGN_SUCCESS){
				xil_printf("Drawing frame failed!");
				check_error(res, buff_out);
			}
			else
			{
				xil_printf("Drawing frame succeeded! Check out the board :)\n");
				xil_printf("***************************************************\n\n");
				strcpy(buff_out,"SUCCESS");
			}
			break;
		}
		case 17: // Test running pixel
		{
			byte r = (byte)get_next_numerical_input(input, end, len, &end);
			byte g = (byte)get_next_numerical_input(input, end, len, &end);
			byte b = (byte)get_next_numerical_input(input, end, len, &end);
			TestRunningPixel(r,g,b);
			xil_printf("Testing running pixel succeeded! Check out the board :)\n");
			xil_printf("***************************************************\n\n");
			strcpy(buff_out,"SUCCESS");
			break;
		}
		default:
		{
			xil_printf("Invalid command!\n");
			xil_printf("***************************************************\n\n");
			strcpy(buff_out,"ERROR: INVALID COMMAND");
			break;
		}
	}
	return 0;
}

int getTerminatorOut()
{
	return getTerminator();
}

void setTerminatorOut(int val)
{
	setTerminator(val);
}
