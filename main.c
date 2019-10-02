#include <stdio.h>
#include "list.h"
#include "Display.h"

int main() {
//    int arr[] = {2,2,2,2};
//    LedSignResult res1 = initBoard("2342",4,32,arr,"0,1,U;2,1,L;");
//    if(res1 != LED_SIGN_SUCCESS){
//        printf("res1 ERROR!!\n");
//    }
//    LedSignResult res2 = addSubBoard(5,35,35,30,5);
//    if(res2 != LED_SIGN_OUT_OF_BOARD_COARDINATES){
//        printf("res2 ERROR!!\n");
//    }
//    LedSignResult res3 = addSubBoard(5,34,35,28,5);
//    if(res3 != LED_SIGN_SUCCESS){
//        printf("res3 ERROR!!\n");
//    }
//    LedSignResult res4 = addSubBoard(5,35,120,20,10);
//    if(res4 != LED_SIGN_OUT_OF_BOARD_COARDINATES){
//        printf("res4 ERROR!!\n");
//    }
//    LedSignResult res5 = addSubBoard(6,61,39,1,1);
//    if(res5 != LED_SIGN_ANOTHER_DISPLAY_LOCATED_THERE){
//        printf("res5 ERROR!!\n");
//    }
//    LedSignResult res6 = addSubBoard(6,62,40,1,1);
//    if(res6 != LED_SIGN_SUCCESS){
//        printf("res6 ERROR!!\n");
//    }
//
//    LedSignResult res7 = cleanSubBoard(3);
//    if(res7 != LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID){
//        printf("res7 ERROR!!\n");
//    }
//    LedSignResult res8 = cleanSubBoard(6);
//    if(res8 != LED_SIGN_SUCCESS){
//        printf("res8 ERROR!!\n");
//    }
//    LedSignResult res9 = cleanSubBoard(6);
//    if(res9 != LED_SIGN_SUCCESS){
//        printf("res9 ERROR!!\n");
//    }
//    LedSignResult res10 = deleteSubBoard(2);
//    if(res10 != LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID){
//        printf("res10 ERROR!!\n");
//    }
//    LedSignResult res11 = deleteSubBoard(6);
//    if(res11 != LED_SIGN_SUCCESS){
//        printf("res11 ERROR!!\n");
//    }
//    LedSignResult res12 = deleteSubBoard(6);
//    if(res12 != LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID){
//        printf("res12 ERROR!!\n");
//    }
//    LedSignResult res13 = addText(2,34,36,28,5,15,20,25,false,10,"my name is Samah");
//    if(res13 != LED_SIGN_NO_DISPLAY_CAN_CONTAIN_THAT_TEXT){
//        printf("res13 ERROR!!\n");
//    }
//    LedSignResult res14 = addText(2,34,35,28,5,15,20,25,false,10,"my name is Samah");
//    if(res14 != LED_SIGN_SUCCESS){
//        printf("res14 ERROR!!\n");
//    }
//    LedSignResult res15 = addText(7,34,35,10,3,15,20,25,false,10,"my name is Samah");
//    if(res15 != LED_SIGN_ANOTHER_OBJECT_LOCATED_THERE){
//        printf("res15 ERROR!!\n");
//    }
//    LedSignResult res16 = addText(2,34,35,1,1,15,20,25,false,10,"my name is Samah");
//    if(res16 != LED_SIGN_OBJECT_ID_ALREADY_EXIST){
//        printf("res16 ERROR!!\n");
//    }
//    LedSignResult res17 = cleanSubBoard(5);
//    if(res17 != LED_SIGN_SUCCESS){
//        printf("res17 ERROR!!\n");
//    }
//    LedSignResult res18 = addText(2,34,35,24,5,15,20,25,false,10,"my name is Samah");
//    if(res18 != LED_SIGN_SUCCESS){
//        printf("res18 ERROR!!\n");
//    }
//    LedSignResult res19 = updateText(6, 2, "my name is Lobabaa");
//    if(res19 != LED_SIGN_NO_DISPLAY_WITH_THE_GIVEN_ID){
//        printf("res19 ERROR!!\n");
//    }
//    LedSignResult res20 = updateText(5, 3, "my name is Lobabaa");
//    if(res20 != LED_SIGN_NO_TEXT_WITH_THE_GIVEN_ID){
//        printf("res20 ERROR!!\n");
//    }
//    LedSignResult res21 = updateText(5, 2, "my name is Lobabaa");
//    if(res21 != LED_SIGN_SUCCESS){
//        printf("res21 ERROR!!\n");
//    }
//    LedSignResult res28 = addImageToStock(5,3,2,"1,2,3;4,5,6;7,8,9;10,11,12;13,14,15;16,17,18;");
//    if(res28 != LED_SIGN_SUCCESS){
//        printf("res28 ERROR!!\n");
//    }
//    LedSignResult res22 = addPicture(9,58,35,false,25,95,19,5);
//    if(res22 != LED_SIGN_SUCCESS){
//        printf("res22 ERROR!!\n");
//    }
//    LedSignResult res23 = addPicture(11,57,35,false,25,95,19,5);
//    if(res23 != LED_SIGN_ANOTHER_OBJECT_LOCATED_THERE){
//        printf("res23 ERROR!!\n");
//    }
//    LedSignResult res24 = updatePicture(5,11,55,25,100,5);
//    if(res24 != LED_SIGN_NO_PICTURE_WITH_THE_GIVEN_ID){
//        printf("res24 ERROR!!\n");
//    }
//    LedSignResult res25 = updatePicture(5,9,55,25,100,5);
//    if(res25 != LED_SIGN_SUCCESS){
//        printf("res25 ERROR!!\n");
//    }
//    LedSignResult res26 = deleteObject(5,9);
//    if(res26 != LED_SIGN_SUCCESS){
//        printf("res26 ERROR!!\n");
//    }
//    LedSignResult res27 = deleteObject(5,2);
//    if(res27 != LED_SIGN_SUCCESS){
//        printf("res27 ERROR!!\n");
//    }
//    destroyBoard();
    return 0;
}