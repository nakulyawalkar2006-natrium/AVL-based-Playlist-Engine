# 🎵 Playlist Management System

A feature-rich **Playlist Management System** developed in **C**, using **AVL Trees** to efficiently manage songs and playback history. The project supports playlist creation, searching, sorting, playback, play history, shuffle mode, and playlist set operations while maintaining logarithmic time complexity for core operations.

---

## 📌 Features

- 🎵 Add and delete songs
- 🔍 Search songs by:
  - Song ID
  - Title
  - Artist
- 📂 Display playlist sorted by:
  - Song ID
  - Title
  - Artist
- ▶️ Sequential music playback
- 🔀 Shuffle and play
- 🔁 Repeat playlist
- 🕒 Play history tracking
- 📊 Range search using Song IDs
- ➕ Playlist Union
- ✖ Playlist Intersection
- ➖ Playlist Difference
- 🔄 Symmetric Difference of playlists
- ⚡ AVL Tree implementation for fast operations

---

# 🏗️ Data Structures Used

- AVL Tree
- Arrays
- Merge Sort
- Binary Search Tree Traversals

---

# 📂 Project Structure

```
.
├── playlist.c          # Complete implementation
├── playlist.h          # Header file and declarations
├── README.md
```

---

# 🚀 Major Functionalities

## 1. Playlist Management

- Add new songs
- Delete existing songs
- Prevent duplicate Song IDs
- Automatic AVL balancing after insertion and deletion

---

## 2. Search

Supports searching by:

- Song ID (O(log n))
- Song Title
- Artist Name

---

## 3. Display Playlist

Display songs sorted by:

- Song ID
- Title
- Artist

Merge Sort is used for Title and Artist sorting.

---

## 4. Playback

Supports:

- Next song
- Previous song
- Exit playback

Every played song is automatically stored in the play history.

---

## 5. Shuffle Mode

Assigns random values to every song and displays them in random order while preserving the original playlist.

---

## 6. Repeat Playlist

Continuously loops through the playlist until interrupted by the user.

---

## 7. Play History

Maintains a separate AVL Tree indexed by timestamps.

History can be viewed in:

- Chronological order
- Reverse chronological order

---

## 8. Range Search

Displays all songs whose Song IDs lie within a specified range.

Time Complexity:

```
O(log n + k)
```

where **k** is the number of matching songs.

---

## 9. Playlist Set Operations

Supports operations between two playlists:

- Union
- Intersection
- Difference
- Symmetric Difference

---

# ⚙️ Algorithms Used

| Operation | Algorithm |
|----------|------------|
| Insert | AVL Tree |
| Delete | AVL Tree |
| Search by ID | AVL Tree |
| Playlist Sorting | Merge Sort |
| Shuffle | Random + Merge Sort |
| History Storage | AVL Tree |
| Range Search | BST Traversal |

---

# 📊 Time Complexity

| Operation | Complexity |
|------------|------------|
| Add Song | O(log n) |
| Delete Song | O(log n) |
| Search by Song ID | O(log n) |
| Search by Title | O(n) |
| Search by Artist | O(n) |
| Display by Song ID | O(n) |
| Display by Title | O(n log n) |
| Display by Artist | O(n log n) |
| Shuffle Playlist | O(n log n) |
| Range Search | O(log n + k) |
| Union | O(n log n) |
| Intersection | O(n log n) |
| Difference | O(n log n) |

---

# 💻 Menu Options

```
1. Add Song
2. Delete Song
3. Display Playlist
4. Search Song
5. Play Songs
6. Shuffle & Play
7. Repeat Playlist
8. Display Play History
9. Set Operations
10. Range Search
11. Exit
```

---

# 🛠️ Technologies Used

- C Programming
- AVL Trees
- Merge Sort
- Recursion
- Dynamic Memory Allocation
- Windows Console Functions (`conio.h`, `windows.h`)

---

# ▶️ Compilation

Using GCC (MinGW):

```bash
gcc playlist.c -o playlist.exe
```

Run:

```bash
playlist.exe
```

---

# 📖 Sample Workflow

1. Add songs to the playlist.
2. Display songs sorted by Title, Artist, or Song ID.
3. Search for a song.
4. Play songs sequentially or shuffle them.
5. View play history.
6. Perform set operations between playlists.
7. Search songs within a Song ID range.

---

# 🎯 Learning Outcomes

This project demonstrates practical implementation of:

- AVL Trees
- Self-balancing Binary Search Trees
- Merge Sort
- Searching Algorithms
- Dynamic Memory Management
- Playlist Management Systems
- Tree Traversals
- Set Operations on Trees
- Modular Programming in C

---

# 📜 License

This project is intended for educational and academic purposes.

---

# 👨‍💻 Author

**Nakul Yawalkar**

If you found this project helpful, consider giving it a ⭐ on GitHub!
