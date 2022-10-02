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

struct bitmap_arr* bitmap_arr_head = NULL;
struct hash_arr* hash_arr_head = NULL;
struct list_arr* list_arr_head = NULL;
int bitmap_arr_s = -1;
int bitmap_arr_e = -1;
int hash_arr_s = -1;
int hash_arr_e = -1;
int list_arr_s = -1;
int list_arr_e = -1;

int create(char* buffer);
int dumpdata(char* buffer);
int delete(char* buffer);
struct bitmap* find_bitmap(char* buffer);

FILE *fp;

int main(int argc, char* argv[]){
    
    fp = fopen(argv[1], "r");

    char buffer[20];

    while(fscanf(fp, "%s", buffer) != EOF){
        if(strcmp(buffer, "quit") == 0 || strcmp(buffer, "quit\n") == 0){
            break;
        }

        else if(strcmp(buffer,"create") == 0){//create bitmap, hash, list
            fscanf(fp, "%s", buffer);//create buffer type data structure
            create(buffer);
        }

        else if(strcmp(buffer, "dumpdata") == 0){//dumpdata bitmap, hash, list
            fscanf(fp, "%s", buffer);
            dumpdata(buffer);
        }

        else if(strcmp(buffer, "bitmap_mark") == 0){//dumpdata bitmap, hash, list
            size_t idx;
            fscanf(fp, "%s", buffer);
            struct bitmap* target_bitmap = find_bitmap(buffer);
            fscanf(fp,"%zu", &idx);
            bitmap_mark(target_bitmap, idx);
        }

        else if(strcmp(buffer, "bitmap_all") == 0){
            size_t start, cnt;
            fscanf(fp, "%s", buffer);
            struct bitmap* target_bitmap = find_bitmap(buffer);
            fscanf(fp, "%zu %zu", &start, &cnt);
            printf("%s\n", bitmap_all(target_bitmap, start, cnt)?"true":"false");
        }
        
        else if(strcmp(buffer, "bitmap_any") == 0){
            size_t start, cnt;
            fscanf(fp, "%s", buffer);
            struct bitmap* target_bitmap = find_bitmap(buffer);
            fscanf(fp, "%zu %zu", &start, &cnt);
            printf("%s\n", bitmap_any(target_bitmap, start, cnt)?"true":"false");
        }

        else if(strcmp(buffer, "bitmap_contains") == 0){
            size_t start, cnt;
            bool contain;
            fscanf(fp, "%s", buffer);
            struct bitmap* target_bitmap = find_bitmap(buffer);
            fscanf(fp, "%zu %zu", &start, &cnt);
            fscanf(fp, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            printf("%s\n", bitmap_contains(target_bitmap, start, cnt, contain)?"true":"false");
        }

        else if(strcmp(buffer, "bitmap_count") == 0){
            size_t start, cnt;
            bool contain;
            fscanf(fp, "%s", buffer);
            struct bitmap* target_bitmap = find_bitmap(buffer);
            fscanf(fp, "%zu %zu", &start, &cnt);
            fscanf(fp, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            printf("%zu\n", bitmap_count(target_bitmap, start, cnt, contain));

        }

        else if(strcmp(buffer, "bitmap_dump") == 0){
            fscanf(fp, "%s", buffer);
            struct bitmap* target_bitmap = find_bitmap(buffer);
            bitmap_dump(target_bitmap);
        }
        else if(strcmp(buffer, "delete") == 0){
            fscanf(fp, "%s", buffer);
            delete(buffer);
        }
    }
    
    return 0;
}

int create(char* buffer){
    if(strcmp(buffer,"bitmap") == 0){//create bitmap
        size_t bitsize;
        fscanf(fp, "%s", buffer);//bitmap name
        fscanf(fp, "%zu", &bitsize);//bitsize
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
        fscanf(fp, "%s", buffer);//hashtable name
    }
    else if(strcmp(buffer, "list") == 0 ){
        fscanf(fp, "%s", buffer);//list name
    }    
    else{
        printf("could not create\n");
        return -1;
    }

    return 0;
}

// int dumpdata(char* buffer){
//     struct bitmap_arr* bitmap_arr_temp = bitmap_arr_head;
//     struct hash_arr* hash_arr_temp = hash_arr_head;
//     struct list_arr* list_arr_temp = list_arr_head;
//     bool bitmap_found = false;

//     while(bitmap_arr_temp != NULL){//find bitmap named buffer
//         if(strcmp(bitmap_arr_temp->bitmap_name, buffer) == 0){
//             bitmap_found = true;
//             break;
//         }
//         bitmap_arr_temp = bitmap_arr_temp->next;
//     }

//     if(bitmap_found){

//         bitmap_dump (bitmap_arr_temp->bitmap_);
//         printf("bitmap cnt: %zu", bitmap_size(bitmap_arr_temp->bitmap_));
//         printf("dumped bitmap");
//     }

//     return 0;
// }

int dumpdata(char* buffer){//find bitmap, hash, list with named buffer. search bitmap, hash, list in order
    struct bitmap* target_bitmap = find_bitmap(buffer);
    if(target_bitmap != NULL){
        size_t size = bitmap_size(target_bitmap);
        for(size_t i = 0; i < size; i++){
            printf("%zu", (size_t) bitmap_test(target_bitmap, i));
        }
        printf("\n");
        return 0;
    }

    //find hash

    //find list

    return -1; //can't find
}


int delete(char* buffer){//find bitmap, hash, list with named buffer. search bitmap, hash, list in order
    struct bitmap* target_bitmap = find_bitmap(buffer);
    if(target_bitmap != NULL){

        bitmap_destroy(target_bitmap);
        return 0;
    }

    //find hash

    //find list

    return -1; //can't find
}

struct bitmap* find_bitmap(char* buffer){
    struct bitmap_arr* bitmap_arr_temp = bitmap_arr_head;
    bool bitmap_found = false;

    while(bitmap_arr_temp != NULL){//find bitmap named buffer
        if(strcmp(bitmap_arr_temp->bitmap_name, buffer) == 0){
            bitmap_found = true;
            break;
        }
        bitmap_arr_temp = bitmap_arr_temp->next;
    }

    if(bitmap_found)
        return bitmap_arr_temp->bitmap_;
    else
        return NULL;
}