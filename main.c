#include <stdio.h>
#include "mixedList.h"

int main() {
    MixedList ml = mixedListCreate();
    RGB color = createRGB(1,10,30);
    TextObject t1 = createTextObject(1,1,1,2,2,color,true,16,"my name is Samah");
    TextObject t2 = createTextObject(2,1,4,2,2,color,true,16,"my name is Samah");
    TextObject t3 = createTextObject(3,3,3,2,2,color,true,16,"my name is Samah");
    PicObject t4 = createPicObject(3,3,3,2,2,color,EXISTING,"up");
    if(mixedListInsert(ml,t2,Text) != MIXED_LIST_SUCCESS){
        printf("error1!");
    }
    if(mixedListInsert(ml,t1,Text) != MIXED_LIST_SUCCESS){
        printf("error2!");
    }
    for(int i = mixedListGetFirstID(ml); i != -1; i = mixedListGetNextID(ml)){
        printf("id = %d\n", i);
    }
    return 0;
}