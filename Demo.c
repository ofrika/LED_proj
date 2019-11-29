// ------------ This is initialization setup ------------

Init(default)
// For welcome message
Add_sub_board(1,0,0,255,31)
// For parking areas
Add_sub_board(2,0,64,82,63);
Add_sub_board(3,120,64,82,63);
// In case something went wrong
Clear_sub_board(1);
Clear_sub_board(2);
Clear_sub_board(3);
// For welcome message
Add_text_area(1,1,32,0,192,32,255,100,180,1);
// For left parking headline
Add_text_area(2,1,0,64,80,8,0,20,220,0);
Add_text_area(2,2,20,77,16,8,0,20,220,0);
Add_picture_area(2,3,50,77,20,20,1,255,255,255);
// For right parking headline
Add_text_area(3,1,120,64,80,8,255,255,0,0);
Add_text_area(3,2,140,77,16,8,255,255,0,0);
Add_picture_area(3,3,170,77,20,20,1,255,255,255);
// For welcome message
Insert_text(1,1,{72,37,51,33,57,53,37,96,66,57,96,36,36,91},14);
// For left parking headline
Insert_text(2,1,{58,29,63,49,96,54,57,63,67,43},10);
Insert_text(2,2,{82,81},2);
Insert_picture (2,3,2);
// For right parking headline
Insert_text(2,1,{58,29,63,49,96,64,57,69,67,43},10);
Insert_text(2,2,{83,49},2);
Insert_picture (3,3,3);



// ------------ This is the demo itself: type these in command window ------------

// Shows identifying invalid commands
// fhreerkhg

// Shows clearing sub-board
// Write: Clear_sub_board(2)

// Shows the board still exists but there are no areas inside
// Write: Get_status()

// Shows live adding of a new picture
// Write: Add_picture_area(2,25,0,64,50,50,1,255,255,255)

// Shows the board exists and there is a new picture area inside with ID=25
// Write: Get_status()

// Shows inserting a smiley face to the picture area
// Write: Insert_picture (2,1,5)

// Shows flipping board
// Write: Flip_down()


