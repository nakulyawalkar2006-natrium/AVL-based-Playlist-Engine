#include "playlist.h"

/* global state */
song_history_node* play_history_root=NULL;
static int history_timestamp=0;

/* AVL helpers for playlist */

int song_height(song_node* node)
{
    int height=0;
    if(node!=NULL) height=node->height;
    return height;
}

int song_balance(song_node* node)
{
    int balance=0;
    if(node!=NULL)  balance=song_height(node->left)-song_height(node->right);
    return balance;
}

int max2(int a, int b)
{
    int max=b;
    if (a>b) max=a;
    return max;
}

song_node* song_rotateRight(song_node* node)
{
    song_node* temp=node->left;
    song_node* temp2=temp->right;
    temp->right=node;
    node->left=temp2;
    node->height=1+max2(song_height(node->left), song_height(node->right));
    temp->height=1+max2(song_height(temp->left), song_height(temp->right));
    return temp;
}

song_node* song_rotateLeft(song_node* node)
{
    song_node* temp=node->right;
    song_node* temp2=temp->left;
    temp->left=node;
    node->right=temp2;
    node->height=1+max2(song_height(node->left), song_height(node->right));
    temp->height=1+max2(song_height(temp->left), song_height(temp->right));
    return temp;
}

// rebalance after insert or delete 
song_node* song_avl_fix(song_node* node)
{
    node->height=1+max2(song_height(node->left),song_height(node->right));
    int bal=song_balance(node);

    // left-left 
    if(bal>1 && song_balance(node->left)>=0)  node=song_rotateRight(node);

    // left-right
    else if(bal>1 && song_balance(node->left)<0)
    {
        node->left=song_rotateLeft(node->left);
        node=song_rotateRight(node);
    }

    // right-right 
    else if(bal<-1 && song_balance(node->right)<=0)  node=song_rotateLeft(node);

    // right-left
    else if(bal<-1 && song_balance(node->right)>0)
    {
        node->right=song_rotateRight(node->right);
        node=song_rotateLeft(node);
    }

    return node;
}

song_node* song_avl_insert(song_node* root,song_node* newNode)
{
    if(root==NULL)  root=newNode;
    else if(newNode->SongID<root->SongID)  root->left=song_avl_insert(root->left,newNode);
    else if(newNode->SongID>root->SongID)  root->right=song_avl_insert(root->right,newNode);
    else
    {
        //  duplicate SongID, discard 
        printf("Song with ID %d already exists. Song not added.\n", newNode->SongID);
        free(newNode);
    }
    if(root!=newNode)  root=song_avl_fix(root);

    return root;
}

song_node* song_avl_min(song_node* root)
{
    while(root->left!=NULL)  root=root->left;

    return root;
}

song_node* song_avl_delete(song_node* root, int SongID)
{
    if(root==NULL)
    {
        printf("Song with ID %d not found.\n", SongID);
    }
    else if(SongID<root->SongID)
    {
        root->left=song_avl_delete(root->left, SongID);
        root=song_avl_fix(root);
    }
    else if(SongID>root->SongID)
    {
        root->right=song_avl_delete(root->right, SongID);
        root=song_avl_fix(root);
    }
    else    //  node found 
    {
        if(root->left==NULL||root->right==NULL)
        {
            song_node* child;
            if(root->left!=NULL)  child=root->left;
            else child=root->right;

            printf("Song with ID %d deleted successfully.\n", SongID);
            free(root);
            root=child;
        }
        else    //  two children case, replace with in-order successor 
        {
            song_node* INsucc=song_avl_min(root->right);
            root->SongID=INsucc->SongID;
            strcpy(root->Title, INsucc->Title);
            strcpy(root->Artist, INsucc->Artist);
            root->rdm=INsucc->rdm;
            printf("Song with ID %d deleted successfully.\n", SongID);
            root->right=song_avl_delete(root->right, INsucc->SongID);
            root=song_avl_fix(root);
        }
    }
    return root;
}

/* AVL helpers for history */

int song_hist_height(song_history_node* n)
{
    int height=0;
    if(n!=NULL)  height=n->height;

    return height;
}

int song_hist_balance(song_history_node* n)
{
    int balance=0;
    if(n!=NULL)  balance=song_hist_height(n->left)-song_hist_height(n->right);

    return balance;
}

song_history_node* hist_rotateRight(song_history_node* node)
{
    song_history_node* temp=node->left;
    song_history_node* temp2=temp->right;
    temp->right=node;
    node->left=temp2;
    node->height=1+max2(song_hist_height(node->left), song_hist_height(node->right));
    temp->height=1+max2(song_hist_height(temp->left), song_hist_height(temp->right));
    return temp;
}

song_history_node* hist_rotateLeft(song_history_node* node)
{
    song_history_node* temp=node->right;
    song_history_node* temp2=temp->left;
    temp->left=node;
    node->right=temp2;
    node->height=1+max2(song_hist_height(node->left), song_hist_height(node->right));
    temp->height=1+max2(song_hist_height(temp->left), song_hist_height(temp->right));
    return temp;
}

song_history_node* hist_avl_fix(song_history_node* n)
{
    n->height=1+max2(song_hist_height(n->left), song_hist_height(n->right));
    int bal=song_hist_balance(n);
    // left-left 
    if(bal>1&&song_hist_balance(n->left)>=0)   n=hist_rotateRight(n);

    // left-right 
    else if(bal>1&&song_hist_balance(n->left)<0)
    {
        n->left=hist_rotateLeft(n->left);
        n=hist_rotateRight(n);
    }

    // right-right 
    else if(bal<-1&&song_hist_balance(n->right)<=0)  n=hist_rotateLeft(n);

    // right-left
    else if(bal<-1&&song_hist_balance(n->right)>0)
    {
        n->right=hist_rotateRight(n->right);
        n=hist_rotateLeft(n);
    }
    return n;
}

song_history_node* hist_avl_insert(song_history_node* root,song_history_node* newNode)
{
    if(root==NULL)  root=newNode;
    else if(newNode->timestamp<root->timestamp)  root->left=hist_avl_insert(root->left,newNode);
    else  root->right=hist_avl_insert(root->right,newNode);
    if(root!=newNode)  root=hist_avl_fix(root);

    return root;
}


/////////////////// HELPER FUNCTIONS ///////////////////

/* node creation and memory */

song_node* createNode()
{
    song_node* NewSong=(song_node*)malloc(sizeof(song_node));
    printf("Enter Song ID: ");
    scanf("%d", &NewSong->SongID);
    if(NewSong->SongID<0)
    {
        printf("Invalid Song ID. Song ID should be a non-negative integer.\n");
        free(NewSong);
        NewSong=NULL;
    }
    else
    {
        printf("Enter Song Title: ");
        scanf(" %[^\n]", NewSong->Title);
        printf("Enter Artist Name: ");
        scanf(" %[^\n]", NewSong->Artist);
        NewSong->left=NULL;
        NewSong->right=NULL;
        NewSong->height=1;
        NewSong->rdm=-1;
    }
    return NewSong;
}

song_node* clone_song_node(song_node* node)
{
    song_node* new_node=(song_node*)malloc(sizeof(song_node));
    new_node->SongID=node->SongID;
    strcpy(new_node->Title, node->Title);
    strcpy(new_node->Artist, node->Artist);
    new_node->left=NULL;
    new_node->right=NULL;
    new_node->height=1;
    new_node->rdm=node->rdm;
    return new_node;
}

song_node* free_playlist(song_node* root)
{
    if(root!=NULL)
    {
        free_playlist(root->left);
        free_playlist(root->right);
        free(root);
        root=NULL;
    }
    return root;
}

song_history_node* free_play_history(song_history_node* root)
{
    if(root!=NULL)
    {
        free_play_history(root->left);
        free_play_history(root->right);
        free(root);
        root=NULL;
    }
    return root;
}

/* flatten tree to array,inorder gives sorted by SongID */

int tree_to_array(song_node* root,song_node* arr[],int idx)
{
    if(root!=NULL)
    {
        idx=tree_to_array(root->left,arr,idx);
        arr[idx++]=root;
        idx=tree_to_array(root->right,arr,idx);
    }
    return idx;
}

void display_song_node(song_node* node)
{
    if(node!=NULL)
    {
        printf("ID: %d, Title: %s, Artist: %s\n", node->SongID, node->Title, node->Artist);
    }    
    else
    {
        printf("Song not found.\n");
    }    
}


/* merge sort and its helpers */

bool check_First_less_than_Second(song_node* first,song_node* second,InfoType type)
{
    bool result=false;
    if(type==SongID)
    {
        if(first->SongID<=second->SongID)
        {
            result=true;
        }
    }
    else if(type==Title)
    {
        if(strcmp(first->Title,second->Title)<=0)
        {
            result=true;
        }
    }
    else if(type==Artist)
    {
        if(strcmp(first->Artist,second->Artist)<=0)
        {
            result=true;
        }
    }
    else if(type==Random)
    {
        if(first->rdm<=second->rdm)
        {
            result=true;
        }
    }
    return result;
}



void MergeSort_array(song_node* arr[],int left,int right,InfoType type)
{
    if(left<right)
    {
        int mid=left+(right-left)/2;
        MergeSort_array(arr,left,mid,type);
        MergeSort_array(arr,mid+1,right,type);
        Merge(arr,left,mid,right,type);
    }
}


void Merge(song_node* arr[],int left,int mid,int right,InfoType type)
{
    int n1=mid-left+1;
    int n2=right-mid;
    song_node* Left[n1];
    song_node* Right[n2];
    for(int i=0; i<n1; i++)
    {
        Left[i]=arr[left+i];
    }
    for(int j=0; j<n2; j++)
    {
        Right[j]=arr[mid+1+j];
    }
    int i=0,j=0,k=left;
    while(i<n1 && j<n2)
    {
        if(check_First_less_than_Second(Left[i],Right[j],type))
        {
            arr[k++]=Left[i++];
        }
        else
        {
            arr[k++]=Right[j++];
        }
    }
    while(i<n1)
    {
        arr[k++]=Left[i++];
    }
    while(j<n2)
    {
        arr[k++]=Right[j++];
    }
}




/////////////////////////// MAIN FUNCTIONS ///////////////////////////

/* core playlist operations */

song_node* Add_song(song_node* root)
{
    song_node* NewSong=createNode();
    if(NewSong!=NULL)  root=song_avl_insert(root, NewSong);

    return root;
}

song_node* Delete_song(int SongID, song_node* root)
{
    if(root==NULL)  printf("Playlist is empty.\n");
    else   root=song_avl_delete(root, SongID);
    return root;
}


song_node* Search_by_ID(song_node* root, int id)      //O(log n) BST search by SongID
{
    if(root!=NULL && id!=root->SongID)
    {
        if(id<root->SongID)   root=Search_by_ID(root->left, id);
        else  root=Search_by_ID(root->right, id);
    }

    return root;
}

song_node* Search_Song(song_node* root, InfoType type)
{
    song_node* ret=NULL;
    if(root==NULL)
    {
        printf("Playlist is empty.\n");
    }
    else if(type==SongID)
    {
        int id;
        printf("Enter Song ID to search: ");
        scanf("%d", &id);
        song_node* found=Search_by_ID(root, id);
        if(found!=NULL)  ret=clone_song_node(found);

    }
    else if(type==Title)
    {
        char title[MAX_TITLE];
        printf("Enter Song Title to search: ");
        scanf(" %[^\n]", title);
        song_node* arr[MAX_SONGS];
        int n=tree_to_array(root,arr,0);  // n=tree_size(root)
        int i=0;
        while(i<n && ret==NULL)
        {
            if(strcmp(arr[i]->Title, title)==0) 
            {
                ret=clone_song_node(arr[i]);
            }    
            i++;
        }
    }
    else
    {
        char artist[MAX_ARTIST];
        printf("Enter Artist Name to search: ");
        scanf(" %[^\n]", artist);
        song_node* arr[MAX_SONGS];
        int n=tree_to_array(root, arr, 0);
        int i=0;
        while(i<n && ret==NULL)
        {
            if(strcmp(arr[i]->Artist, artist)==0)
            {
                ret=clone_song_node(arr[i]);
            }    
            i++;
        }
    }
    return ret;
}

void Display_playlist(song_node* root, InfoType type)
{
    if(root==NULL)
    {
        printf("Playlist is empty.\n");
    }
    else
    {
        song_node* arr[MAX_SONGS];
        int n=tree_to_array(root, arr, 0);
        if(type==SongID)     // inorder traversal already gives sorted order 
        {
            printf("Playlist sorted by Song ID:\n");
            
        }
        else if(type==Title)
        {
            printf("Playlist sorted by Title:\n");
            MergeSort_array(arr, 0, n-1, Title);
        }
        else if(type==Artist)
        {
            printf("Playlist sorted by Artist, then Title:\n");
            MergeSort_array(arr, 0, n-1, Artist);
        }
        else if(type==Random)
        {
            printf("Playlist shuffled:\n");
            MergeSort_array(arr, 0, n-1, Random);
        }
        for(int i=0; i<n; i++)
        {
            printf("ID: %d, Title: %s, Artist: %s\n", arr[i]->SongID,arr[i]->Title,arr[i]->Artist);
        }    
    }
}

void Play_Songs(song_node* root)
{
    if(root==NULL)
    {
        printf("Playlist is empty.\n");
    }
    else
    {
        song_node* arr[MAX_SONGS];
        int n=tree_to_array(root,arr,0);
        int i=0;
        bool done=false;
        while(i>=0 && i<n && !done)
        {
            printf("Playing ID: %d, Title: %s, Artist: %s\n", arr[i]->SongID, arr[i]->Title, arr[i]->Artist);
            Add_to_history(arr[i]);
            printf("Do you want to play Next(0), Prev(1), Exit(-1): ");
            int choice;
            scanf("%d", &choice);
            printf("\n");
            if(choice==0)
                i++;
            else if(choice==1)
                i--;
            else if(choice==-1)
                done=true;
            else
            {
                printf("Invalid choice. Playing next song.\n");
                i++;
            }
        }
    }
}


void Shuffle_and_Play(song_node* root)
{
    if(root==NULL)
    {
        printf("Playlist is empty.\n");
    }
    else
    {
        song_node* arr[MAX_SONGS];
        int n=tree_to_array(root,arr,0);
        for(int i=0; i<n; i++)
        {
            arr[i]->rdm=rand()%10000;
        }    
        MergeSort_array(arr, 0, n-1, Random);   
        int i=0;
        bool done=false;
        while(i>=0 && i<n && !done)
        {
            printf("Playing ID: %d, Title: %s, Artist: %s\n", arr[i]->SongID, arr[i]->Title, arr[i]->Artist);
            Add_to_history(arr[i]);
            printf("Do you want to play Next(0), Prev(1), Exit(-1): ");
            int choice;
            scanf("%d", &choice);
            printf("\n");
            if(choice==0)
                i++;
            else if(choice==1)
                i--;
            else if(choice==-1)
                done=true;
            else
            {
                printf("Invalid choice. Playing next song.\n");
                i++;
            }
        }
        for(int j=0; j<n; j++)
        {
            arr[j]->rdm=-1;
        }    
    }
}

void Repeat_Song_Playlist(song_node* root)
{
    if(root==NULL)
    {
        printf("Playlist is empty.\n");
    }
    else
    {
        song_node* arr[MAX_SONGS];
        int n=tree_to_array(root, arr, 0);
        bool repeat=true;
        int i=0;
        printf("Repeating playlist songs. Press any key to stop...\n");
        while(repeat)
        {
            if(_kbhit())
            {
                getch();
                repeat=false;
            }
            printf("Playing ID: %d, Title: %s, Artist: %s\n", arr[i]->SongID, arr[i]->Title, arr[i]->Artist);
            Add_to_history(arr[i]);
            i=(i+1)%n;
            Sleep(1000);
        }
    }
}

/////////////////// history operations 


void inorder_history(song_history_node* root)  // in-order is chronological, lowest timestamp first 
{
    if(root!=NULL)
    {
        inorder_history(root->left);
        printf("ID: %d, Title: %s, Artist: %s\n", root->SongID, root->Title, root->Artist);
        inorder_history(root->right);
    }
}

void reverse_inorder_history(song_history_node* root)  // reverse in-order is reverse chronological, highest timestamp first
{
    if(root!=NULL)
    {
        reverse_inorder_history(root->right);
        printf("ID: %d, Title: %s, Artist: %s\n", root->SongID, root->Title, root->Artist);
        reverse_inorder_history(root->left);
    }
}

void Add_to_history(song_node* song)
{
    song_history_node* node=(song_history_node*)malloc(sizeof(song_history_node));
    node->SongID=song->SongID;
    strcpy(node->Title, song->Title);
    strcpy(node->Artist, song->Artist);
    node->timestamp=history_timestamp++;
    node->left=NULL;
    node->right=NULL;
    node->height=1;
    play_history_root=hist_avl_insert(play_history_root, node);
}

void Display_Play_History()
{
    if(play_history_root==NULL)
    {
        printf("No songs played yet.\n");
    }
    else
    {
        int choice;
        printf("\n1. Chronological order\n2. Reverse chronological order\nEnter your choice: ");
        scanf("%d", &choice);
        if(choice==1)
        {
            printf("Play History (Chronological Order):\n");
            inorder_history(play_history_root);
        }
        else if(choice==2)
        {
            printf("Play History (Reverse Chronological Order):\n");
            reverse_inorder_history(play_history_root);
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }
}

void Range_Search(song_node* root,int s1,int s2)  // O(log n + k)
{
    if(root!=NULL)
    {
        if(root->SongID>s1)
        {
            Range_Search(root->left,s1,s2);
        }    
        if(root->SongID>=s1 && root->SongID<=s2)
        {
            printf("ID: %d, Title: %s, Artist: %s\n", root->SongID, root->Title, root->Artist);
        }    
        if(root->SongID<s2)
        {
            Range_Search(root->right,s1,s2);
        }    
    }
}

/* set operations */

song_node* Union_of_Playlists(song_node* root1, song_node* root2)
{
    song_node* result=NULL;
    song_node* arr1[MAX_SONGS];
    song_node* arr2[MAX_SONGS];
    int n1=tree_to_array(root1, arr1, 0);
    int n2=tree_to_array(root2, arr2, 0);
    for(int i=0; i<n1; i++)
    {
        result=song_avl_insert(result, clone_song_node(arr1[i]));
    }
    for(int i=0; i<n2; i++)
    {
        if(Search_by_ID(result, arr2[i]->SongID)==NULL)
        {
            result=song_avl_insert(result, clone_song_node(arr2[i]));
        }    
    }
    return result;
}

song_node* Intersection_of_Playlists(song_node* root1, song_node* root2)
{
    song_node* result=NULL;
    song_node* arr1[MAX_SONGS];
    int n1=tree_to_array(root1, arr1, 0);
    for(int i=0; i<n1; i++)
    {
        if(Search_by_ID(root2, arr1[i]->SongID)!=NULL)
        {
            result=song_avl_insert(result, clone_song_node(arr1[i]));
        }
    }
    return result;
}

song_node* Difference_of_Playlists(song_node* root1, song_node* root2)
{
    song_node* result=NULL;
    song_node* arr1[MAX_SONGS];
    int n1=tree_to_array(root1, arr1, 0);
    for(int i=0; i<n1; i++)
    {
        if(Search_by_ID(root2, arr1[i]->SongID)==NULL)
        {
            result=song_avl_insert(result, clone_song_node(arr1[i]));
        }
    }
    return result;
}

song_node* Symmetric_Difference_of_Playlists(song_node* root1, song_node* root2)
{
    song_node* result=NULL;
    song_node* arr1[MAX_SONGS];
    song_node* arr2[MAX_SONGS];
    int n1=tree_to_array(root1, arr1, 0);
    int n2=tree_to_array(root2, arr2, 0);
    for(int i=0; i<n1; i++)
    {
        if(Search_by_ID(root2, arr1[i]->SongID)==NULL)
        {
            result=song_avl_insert(result, clone_song_node(arr1[i]));
        }    
    }
    for(int i=0; i<n2; i++)
    {
        if(Search_by_ID(root1, arr2[i]->SongID)==NULL)
        {
            result=song_avl_insert(result, clone_song_node(arr2[i]));
        }    
    }
    return result;
}

int main()
{
    srand((unsigned)time(0));
    song_node* playlist=NULL;
    int choice;
    do
    {
        printf("\nMenu:\n");
        printf("1. Add Song\n");
        printf("2. Delete Song\n");
        printf("3. Display Playlist\n");
        printf("4. Search Song\n");
        printf("5. Play Songs\n");
        printf("6. Shuffle & Play\n");
        printf("7. Repeat Playlist\n");
        printf("8. Display Play History\n");
        printf("9. Set Operations\n");
        printf("10. Range Search\n");
        printf("11. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice)
        {
            case 1:
                playlist=Add_song(playlist);
                break;
            case 2:
            {
                int id;
                printf("Enter Song ID to delete: ");
                scanf("%d", &id);
                playlist=Delete_song(id, playlist);
                break;
            }
            case 3:
            {
                int sort_choice;
                printf("Sort by:\n1. Title\n2. Artist (then Title)\n3. Song ID\nEnter your choice: ");
                scanf("%d", &sort_choice);
                if(sort_choice==1)
                    Display_playlist(playlist, Title);
                else if(sort_choice==2)
                    Display_playlist(playlist, Artist);
                else if(sort_choice==3)
                    Display_playlist(playlist, SongID);
                else
                    printf("Invalid choice.\n");
                break;
            }
            case 4:
            {
                int search_choice;
                printf("Search by:\n1. Song ID\n2. Title\n3. Artist\nEnter your choice: ");
                scanf("%d", &search_choice);
                song_node* found=NULL;
                if(search_choice==1)
                    found=Search_Song(playlist, SongID);
                else if(search_choice==2)
                    found=Search_Song(playlist, Title);
                else if(search_choice==3)
                    found=Search_Song(playlist, Artist);
                else
                    printf("Invalid choice.\n");
                if(found!=NULL||search_choice>=1&&search_choice<=3)
                {
                    display_song_node(found);
                    free(found);
                }
                break;
            }
            case 5:
                Play_Songs(playlist);
                break;
            case 6:
                Shuffle_and_Play(playlist);
                break;
            case 7:
                Repeat_Song_Playlist(playlist);
                break;
            case 8:
                Display_Play_History();
                break;
            case 9:
            {
                printf("Enter number of songs in the second playlist: ");
                int n;
                scanf("%d", &n);
                song_node* playlist2=NULL;
                for(int i=0; i<n; i++)
                {
                    printf("Enter details for song %d:\n", i+1);
                    playlist2=Add_song(playlist2);
                }
                bool flag=true;
                while(flag)
                {
                    printf("\nSet Operations:\n1. Union\n2. Intersection\n3. Difference (Playlist 1 - Playlist 2)\n4. Symmetric Difference\n5. Exit\nEnter your choice: ");
                    int set_choice;
                    scanf("%d", &set_choice);
                    song_node* result=NULL;
                    switch(set_choice)
                    {
                        case 1:
                            result=Union_of_Playlists(playlist, playlist2);
                            printf("\nUnion of Playlists:\n");
                            Display_playlist(result, SongID);
                            break;
                        case 2:
                            result=Intersection_of_Playlists(playlist, playlist2);
                            printf("\nIntersection of Playlists:\n");
                            Display_playlist(result, SongID);
                            break;
                        case 3:
                            result=Difference_of_Playlists(playlist, playlist2);
                            printf("\nDifference (Playlist 1 - Playlist 2):\n");
                            Display_playlist(result, SongID);
                            break;
                        case 4:
                            result=Symmetric_Difference_of_Playlists(playlist, playlist2);
                            printf("\nSymmetric Difference:\n");
                            Display_playlist(result, SongID);
                            break;
                        case 5:
                            flag=false;
                            break;
                        default:
                            printf("Invalid choice.\n");
                    }
                    result=free_playlist(result);
                }
                playlist2=free_playlist(playlist2);
                break;
            }
            case 10:
            {
                int s1, s2;
                printf("Enter start SongID (s1): ");
                scanf("%d", &s1);
                printf("Enter end SongID (s2): ");
                scanf("%d", &s2);
                if(s1>s2)
                    printf("s1 must be <= s2.\n");
                else
                {
                    printf("Songs with SongID in [%d, %d]:\n", s1, s2);
                    Range_Search(playlist, s1, s2);
                }
                break;
            }
            case 11:
                play_history_root=free_play_history(play_history_root);
                playlist=free_playlist(playlist);
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while(choice!=11);
    return 0;
}