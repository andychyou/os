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

#define arr_size 10

struct bitmap_arr{
    struct bitmap* bitmap_;
    char bitmap_name[10];
};

struct hash_arr{
    struct hash* hash_;
    char hash_name[10];
};

struct list_arr{
    struct list* list_;
    char list_name[10];
};

struct bitmap_arr bitmap_arr_head[arr_size];
struct hash_arr hash_arr_head[arr_size];
struct list_arr list_arr_head[arr_size];

int bitmap_arr_s = -1;
int bitmap_arr_e = -1;
int hash_arr_s = -1;
int hash_arr_e = -1;
int list_arr_s = -1;
int list_arr_e = -1;

int create(char* buffer);
int dumpdata(char* buffer);
int delete(char* buffer);
int find_bitmap(char* buffer);
int find_hash(char* buffer);
int find_list(char* buffer);

FILE *fp;

//********* 제출전 할 것
//1. fp대신 stdin로 fscanf 바꿀 것

int main(int argc, char* argv[]){
    
    fp = fopen(argv[1], "r");

    char buffer[20];

    for(int i = 0 ; i < arr_size; i++){
        bitmap_arr_head[i].bitmap_ = NULL;
    }
    for(int i = 0 ; i < arr_size; i++){
        hash_arr_head[i].hash_ = NULL;
    }
    for(int i = 0 ; i < arr_size; i++){
        list_arr_head[i].list_ = NULL;
    }

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
            int arr_i = find_bitmap(buffer);
            fscanf(fp,"%zu", &idx);
            bitmap_mark(bitmap_arr_head[arr_i].bitmap_, idx);
        }

        else if(strcmp(buffer, "bitmap_all") == 0){
            size_t start, cnt;
            fscanf(fp, "%s", buffer);
            int arr_i = find_bitmap(buffer);
            fscanf(fp, "%zu %zu", &start, &cnt);
            printf("%s\n", bitmap_all(bitmap_arr_head[arr_i].bitmap_, start, cnt)?"true":"false");
        }
        
        else if(strcmp(buffer, "bitmap_any") == 0){
            size_t start, cnt;
            fscanf(fp, "%s", buffer);
            int arr_i = find_bitmap(buffer);
            fscanf(fp, "%zu %zu", &start, &cnt);
            printf("%s\n", bitmap_any(bitmap_arr_head[arr_i].bitmap_, start, cnt)?"true":"false");
        }

        else if(strcmp(buffer, "bitmap_contains") == 0){
            size_t start, cnt;
            bool contain;
            fscanf(fp, "%s", buffer);
            int arr_i = find_bitmap(buffer);
            fscanf(fp, "%zu %zu", &start, &cnt);
            fscanf(fp, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            printf("%s\n", bitmap_contains(bitmap_arr_head[arr_i].bitmap_, start, cnt, contain)?"true":"false");
        }

        else if(strcmp(buffer, "bitmap_count") == 0){
            size_t start, cnt;
            bool contain;
            fscanf(fp, "%s", buffer);
            int arr_i = find_bitmap(buffer);
            fscanf(fp, "%zu %zu", &start, &cnt);
            fscanf(fp, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            printf("%zu\n", bitmap_count(bitmap_arr_head[arr_i].bitmap_, start, cnt, contain));

        }

        else if(strcmp(buffer, "bitmap_expand") == 0 ){
            size_t size;
            fscanf(fp, "%s %zu", buffer, &size);
            int arr_i = find_bitmap(buffer);
            if(arr_i != -1)
                bitmap_arr_head[arr_i].bitmap_ = bitmap_expand(bitmap_arr_head[arr_i].bitmap_, size);
        }

        else if(strcmp(buffer, "bitmap_flip") == 0 ){
            size_t idx;
            fscanf(fp, "%s %zu", buffer, &idx);
            int arr_i = find_bitmap(buffer);
            bitmap_flip (bitmap_arr_head[arr_i].bitmap_, idx);
        }

        else if(strcmp(buffer, "bitmap_none") == 0 ){
            size_t start, cnt;
            fscanf(fp, "%s %zu %zu", buffer, &start, &cnt);
            int arr_i = find_bitmap(buffer);
            bitmap_none (bitmap_arr_head[arr_i].bitmap_,  start,  cnt);

        }

        else if(strcmp(buffer, "bitmap_reset") == 0 ){
            size_t idx;
            fscanf(fp, "%s %zu", buffer, &idx);
            int arr_i = find_bitmap(buffer);  
            bitmap_reset (bitmap_arr_head[arr_i].bitmap_,  idx);
        }

        else if(strcmp(buffer, "bitmap_scan_and_flip") == 0 ){
            size_t start, cnt;
            bool contain;

            fscanf(fp, "%s %zu %zu", buffer, &start, &cnt);
            int arr_i = find_bitmap(buffer);  
            fscanf(fp, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            bitmap_scan_and_flip (bitmap_arr_head[arr_i].bitmap_,  start,  cnt, contain);
        }

        else if(strcmp(buffer, "bitmap_scan") == 0 ){
            size_t start, cnt;
            bool contain;

            fscanf(fp, "%s %zu %zu", buffer, &start, &cnt);
            int arr_i = find_bitmap(buffer);  
            fscanf(fp, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            bitmap_scan (bitmap_arr_head[arr_i].bitmap_,  start,  cnt, contain);
        }

        else if(strcmp(buffer, "bitmap_set_all") == 0 ){
            bool contain;

            fscanf(fp, "%s", buffer);
            int arr_i = find_bitmap(buffer);  
            fscanf(fp, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            bitmap_set_all (bitmap_arr_head[arr_i].bitmap_, contain); 
        }

        else if(strcmp(buffer, "bitmap_set_multiple") == 0 ){
            size_t start, cnt;
            bool contain;

            fscanf(fp, "%s %zu %zu", buffer, &start, &cnt);
            int arr_i = find_bitmap(buffer);  
            fscanf(fp, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            bitmap_set_multiple (bitmap_arr_head[arr_i].bitmap_,  start, cnt, contain) ;
        }

        else if(strcmp(buffer, "bitmap_set") == 0 ){
            size_t idx;
            bool contain;

            fscanf(fp, "%s %zu", buffer, &idx);
            int arr_i = find_bitmap(buffer);  
            fscanf(fp, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            bitmap_set (bitmap_arr_head[arr_i].bitmap_, idx,  contain); 

        }

        else if(strcmp(buffer, "bitmap_test") == 0){
            size_t idx;
            fscanf(fp, "%s %zu", buffer, &idx);
            int arr_i = find_bitmap(buffer);  
            printf("%s\n", bitmap_test(bitmap_arr_head[arr_i].bitmap_, idx)?"true":"false");
        }

        else if(strcmp(buffer, "bitmap_size") == 0 ){
            size_t size;

            fscanf(fp, "%s %zu", buffer, &size);
            int arr_i = find_bitmap(buffer);  
            
            printf("%zu\n", (size_t) bitmap_size (bitmap_arr_head[arr_i].bitmap_)); 
        }

        else if(strcmp(buffer, "bitmap_dump") == 0){
            fscanf(fp, "%s", buffer);
            int arr_i = find_bitmap(buffer);  
            bitmap_dump(bitmap_arr_head[arr_i].bitmap_);
        }
        else if(strcmp(buffer, "delete") == 0){
            fscanf(fp, "%s", buffer);
            delete(buffer);
        }

        //*************************
        //2. hash

        else if(strcmp(buffer, "hash_insert") == 0){
            // size_t ele;
            // fscanf(fp,"%s %zu", buffer, &ele);
            // struct hash* target_hash = find_hash(buffer);
            // hash_insert(target_hash, ele);
        }

        //*************************
        //3. list

        else if(strcmp(buffer, "list_push_front") == 0 ){
            int i;
            fscanf(fp, "%s %d", buffer, &i);
            struct list* target_list = find_list(buffer);
            struct list_elem *e = list_begin(target_list);
            struct list_item *temp = list_entry(e, struct list_item, elem);
            temp->data = i;
            list_push_front (target_list, e);

        }

        else if(strcmp(buffer, "list_push_back") == 0 ){
            int i;
            fscanf(fp, "%s %d", buffer, &i);
            struct list* target_list = find_list(buffer);
            struct list_elem *e = list_begin(target_list);
            struct list_item *temp = list_entry(e, struct list_item, elem);
            temp->data = i;
            list_push_back (target_list, e);

        }

        else if(strcmp(buffer, "list_pop_front") == 0 ){
            int i;
            fscanf(fp, "%s %d", buffer, &i);
            struct list* target_list = find_list(buffer);
            struct list_elem *e = list_begin(target_list);
            struct list_item *temp = list_entry(e, struct list_item, elem);
            temp->data = i;
            list_pop_front (target_list);

        }

        else if(strcmp(buffer, "list_pop_back") == 0 ){
            int i;
            fscanf(fp, "%s %d", buffer, &i);
            struct list* target_list = find_list(buffer);
            struct list_elem *e = list_begin(target_list);
            struct list_item *temp = list_entry(e, struct list_item, elem);
            temp->data = i;
            list_pop_back (target_list);

        }

        else if(strcmp(buffer, "list_front") == 0 ){
            fscanf(fp, "%s", buffer);
            struct list* target_list = find_list(buffer);
            struct list_elem *e = list_front(target_list);
            struct list_item *temp = list_entry(e, struct list_item, elem);
            printf("%d\n", temp->data);
        }

        else if(strcmp(buffer, "list_back") == 0 ){
            fscanf(fp, "%s", buffer);
            struct list* target_list = find_list(buffer);
            struct list_elem *e = list_back(target_list);
            struct list_item *temp = list_entry(e, struct list_item, elem);
            printf("%d\n", temp->data);
        }
    }
    
    return 0;
}

int create(char* buffer){
    if(strcmp(buffer,"bitmap") == 0){//create bitmap
        size_t bitsize;
        fscanf(fp, "%s", buffer);//bitmap name
        fscanf(fp, "%zu", &bitsize);//bitsize
        bool create = true;
        int arr_i;
        if(find_bitmap(buffer) != -1){
            for(arr_i = 0; arr_i < arr_size; arr_i++){
                if(bitmap_arr_head[arr_i].bitmap_ == NULL){
                    break;
                }
            }
            bitmap_create(bitmap_arr_head[arr_i].bitmap_);
            strcpy(bitmap_arr_head[arr_i].bitmap_name, buffer);
            return 0;

        }
        
    }
    else if(strcmp(buffer, "hashtable") == 0 ){
        
    }
    else if(strcmp(buffer, "list") == 0 ){
        fscanf(fp, "%s", buffer);//list name
        bool create = true;
        int arr_i;
        
        if(find_list(buffer) != -1){
            for(arr_i = 0; arr_i < arr_size; arr_i++){
                if(list_arr_head[arr_i].list_ == NULL){
                    break;
                }
            }
            list_init(list_arr_head[arr_i].list_);
            strcpy(list_arr_head[arr_i].list_name, buffer);
            return 0;

        }
        
       
    else{
        printf("could not create\n");
        return -1;
    }

}

int dumpdata(char* buffer){//find bitmap, hash, list with named buffer. search bitmap, hash, list in order
    int arr_i = find_bitmap(buffer);
    if(arr_i != -1){
        size_t size = bitmap_size(bitmap_arr_head[arr_i].bitmap_);
        for(size_t i = 0; i < size; i++){
            printf("%zu", (size_t) bitmap_test(bitmap_arr_head[arr_i].bitmap_, i));
        }
        printf("\n");
        return 0;
    }

    //find hash
    // if(target_hash != NULL){
    //     struct hash_iterator i;
    //     hash_first (&i, target_hash);
    //     while (hash_next (&i))
    //         {
    //         struct hash *f = hash_entry (hash_cur (&i), struct hash_elem, list_elem);
            
    //         }
    //     return 0;
    //}

    //find list
    arr_i = find_list(buffer);
    if(arr_i != -1){
        struct list_elem* e;
        for (e = list_begin (list_arr_head[arr_i].list_); e != list_end (list_arr_head[arr_i].list_); e = list_remove (e))
        {
            struct list_item *temp = list_entry(e, struct list_item, elem);
            printf("%d ", temp->data);
        }
        printf("\n");
        return 0;
    }
    return -1; //can't find
}



int delete(char* buffer){//find bitmap, hash, list with named buffer. search bitmap, hash, list in order
    bool bitmap_found = false;
    int i;
    for(i = 0; i < arr_size; i++){
        if(strcmp(bitmap_arr_head[i].bitmap_name, buffer) == 0){
            bitmap_found = true;
            break;
        }
    }

    if(bitmap_found){
        bitmap_destroy(bitmap_arr_head[i].bitmap_);
        bitmap_arr_head[i].bitmap_ = NULL;
        
        return 0;
    }
        

  
    //find hash

    //find list

    bool list_found = false;
    int i;
    for(i = 0; i < arr_size; i++){
        if(strcmp(list_arr_head[i].list_name, buffer) == 0){
            list_found = true;
            break;
        }
    }

    if(list_found){
        int arr_i = find_list(buffer);
        // struct list_elem *e = list_begin(list_arr_head[arr_i].list_);
        // struct list_item *temp = list_entry(e, struct list_item, elem);

        // list_remove(list_arr_head[i].list_);
        free(list_arr_head[arr_i].list_);
        list_arr_head[arr_i].list_ = NULL;
        
        return 0;
    }

    
    return -1; //can't find
}

int find_bitmap(char* buffer){
    bool found = false;
    int i;
    for(i = 0;  i < arr_size; i++){
        if(strcmp(bitmap_arr_head[i].bitmap_name, buffer) == 0){
            found = true;
            break;
        }
    }
    if(found){
        return i;
    }
    else{
        return -1;
    }
   
}

int find_hash(char* buffer){
    bool found = false;
    int i;
    for(i = 0;  i < arr_size; i++){
        if(strcmp(hash_arr_head[i].hash_name, buffer) == 0){
            found = true;
            break;
        }
    }
    if(found){
        return i;
    }
    else{
        return -1;
    }
}


int find_list(char* buffer){
    bool found = false;
    int i;
    for(i = 0;  i < arr_size; i++){
        if(strcmp(list_arr_head[i].list_name, buffer) == 0){
            found = true;
            break;
        }
    }
    if(found){
        return i;
    }
    else{
        return -1;
    }
    
}