#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define MAX_TITLE 100
#define MAX_ARTIST 100
#define MAX_SONGS 1000

typedef enum {Title, Artist, SongID, Random} InfoType;

/* AVL song node, keyed on SongID */
typedef struct SONG_node_tag {
    int SongID;
    char Title[MAX_TITLE];
    char Artist[MAX_ARTIST];
    int rdm;
    int height;
    struct SONG_node_tag* left;
    struct SONG_node_tag* right;
} song_node;

/* AVL history node, keyed on timestamp */
typedef struct SONG_node_play_history_tag {
    int SongID;
    char Title[MAX_TITLE];
    char Artist[MAX_ARTIST];
    int timestamp;
    int height;
    struct SONG_node_play_history_tag* left;
    struct SONG_node_play_history_tag* right;
} song_history_node;

/* AVL helpers for playlist */
int song_height(song_node* n);
int song_balance(song_node* n);
song_node* song_rotateRight(song_node* y);
song_node* song_rotateLeft(song_node* x);
song_node* song_avl_fix(song_node* n);
song_node* song_avl_insert(song_node* root, song_node* newNode);
song_node* song_avl_min(song_node* root);
song_node* song_avl_delete(song_node* root, int SongID);

/* AVL helpers for history */
int song_hist_height(song_history_node* n);
int song_hist_balance(song_history_node* n);
song_history_node* hist_rotateRight(song_history_node* y);
song_history_node* hist_rotateLeft(song_history_node* x);
song_history_node* hist_avl_fix(song_history_node* n);
song_history_node* hist_avl_insert(song_history_node* root, song_history_node* newNode);

/* node creation and memory */
song_node* createNode();
song_node* clone_song_node(song_node* node);
song_node* free_playlist(song_node* root);
song_history_node* free_play_history(song_history_node* root);

/* core playlist operations */
song_node* Add_song(song_node* root);
song_node* Delete_song(int SongID, song_node* root);
song_node* Search_Song(song_node* root, InfoType type);
song_node* Search_by_ID(song_node* root, int id);

/* flatten tree to array */
int tree_to_array(song_node* root, song_node** arr, int idx);

/* merge sort and its helpers */
bool check_First_less_than_Second(song_node* first,song_node* second,InfoType type);
void MergeSort_array(song_node* arr[],int left,int right,InfoType type);
void Merge(song_node* arr[],int left,int mid,int right,InfoType type);


/* playback and display */
void Display_playlist(song_node* root, InfoType type);
void display_song_node(song_node* node);
void Play_Songs(song_node* root);
void Repeat_Song_Playlist(song_node* root);
void Shuffle_and_Play(song_node* root);

/* history operations */
void Add_to_history(song_node* song);
void Display_Play_History();
void inorder_history(song_history_node* root);
void reverse_inorder_history(song_history_node* root);

/* range search */
void Range_Search(song_node* root, int s1, int s2);

/* set operations */
song_node* Union_of_Playlists(song_node* root1, song_node* root2);
song_node* Intersection_of_Playlists(song_node* root1, song_node* root2);
song_node* Difference_of_Playlists(song_node* root1, song_node* root2);
song_node* Symmetric_Difference_of_Playlists(song_node* root1, song_node* root2);

#endif