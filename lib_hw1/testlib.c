#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "bitmap.h"
#include "debug.h"
#include "hash.h"
#include "hex_dump.h"
#include "limits.h"
#include "list.h"
#include "round.h"

struct bitmap_arr{
    struct bitmap* bitmap_;
    char bitmap_name[10];
    struct bitmap_arr* next;
};

struct hash_arr{
    struct hastable* hastable_;
    char hash_name[10];
    struct hashtable_arr* next;
};

struct list_arr{
    struct list* list_;
    char list_name[10];
    struct list_arr* next;
};

int main(int argc, char* argv[]){
    
    // char string[50];
    // fgets(string, 50, stdin);
    // char *token = strtok(string, " ");

    // while(strcmp(token, "quit\n") != 0 || strcmp(token, "quit") != 0){
    //     int commandCnt = 0;
    //     while(token != NULL){
    //         printf("%s\n", token);


    //         commandCnt++;
    //         token = strtok(NULL, " ");
    //     }
    // }

    struct bitmap_arr* bitmap_arr_head = NULL;
    struct hash_arr* hash_arr_head = NULL;
    struct list_arr* list_arr_head = NULL;
    int bitmap_arr_s = -1;
    int bitmap_arr_e = -1;
    int hash_arr_s = -1;
    int hash_arr_e = -1;
    int list_arr_s = -1;
    int list_arr_e = -1;



    char buffer[20];
    while(scanf("%s", buffer) != EOF){
        if(strcmp(buffer, "quit") == 0 || strcmp(buffer, "quit\n") == 0){
            break;
        }

        if(strcmp(buffer,"create") == 0){//create bitmap, hash, list
            scanf("%s", buffer);//create buffer type data structure
            if(strcmp(buffer,"bitmap") == 0){//create bitmap
                int bitsize;
                scanf("%s", buffer);//bitmap name
                scanf("%d", &bitsize);//bitsize
                struct bitmap_arr* curr = bitmap_arr_head;
                struct bitmap_arr* before = bitmap_arr_head;
                bool create = true;

                while(curr != NULL){
                    if(strcmp(curr->bitmap_name, buffer) == 0) {
                        create = false;
                        break;
                    }
                    before = curr;
                    curr = curr->next;
                }

                if(create){
                    struct bitmap_arr* new_bitmap = (struct bitmap_arr*)(malloc(sizeof(struct bitmap_arr)));
                    new_bitmap->bitmap_ = bitmap_create(bitsize);
                    strcpy(new_bitmap->bitmap_name, buffer);

                    if(before == NULL){//new_bitmap is the first bitmap
                        new_bitmap->next = NULL;
                        bitmap_arr_head = new_bitmap;
                        bitmap_arr_s = bitmap_arr_e = 0;
                    }

                    else{
                        new_bitmap->next = curr;
                        before->next = new_bitmap;                      
                        bitmap_arr_e++;
                            
                    }
                    
                }
                
            }
            else if(strcmp(buffer, "hashtable") == 0 ){
                scanf("%s", buffer);//hashtable name
            }
            else if(strcmp(buffer, "list") == 0 ){
                scanf("%s", buffer);//list name
            }
        }
    }
    
    return 0;
}