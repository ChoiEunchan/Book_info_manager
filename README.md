# B+ tree, Heap & Selection Tree Implementation
### There may be errors when running in a Windows or other environment so LINUX environment is recommended.
### (Work Environment : Ubuntu 18.04)

- This project is implementation of B+tree, Selection tree and Heap.\
Book data is consists of several things, book name(`name`), classification code(`code`), `author`, publish year(`year`).\
All datas must be saved in data file, `loan_book.txt`.\
Data condition is followings.

    ```
    1. All datas are seperated with \t(tab).
    2. Name is unduplicated and under or equal to 50 letters.
    3. The code is consisted from 000 to 600. Amount of borrowable book is different for each codes.
    ```
- Specific amount can check with following table.
        <table>
            <tr>
                <th> Code </td>
                <th> Amount of borrowable book </td>
            </tr>
            <tr>
                <td> 000 </td>
                <td> 3 </td>
            </tr>
            <tr>
                <td> 100 </td>
                <td> 3 </td>
            </tr>
            <tr>
                <td> 200 </td>
                <td> 3 </td>
            </tr>
            <tr>
                <td> 300 </td>
                <td> 4 </td>
            </tr>
            <tr>
                <td> 400 </td>
                <td> 4 </td>
            </tr>
            <tr>
                <td> 500 </td>
                <td> 2 </td>
            </tr>
            <tr>
                <td> 600 </td>
                <td> 2 </td>
            </tr>
            <tr>
                <td> 700 </td>
                <td> 2 </td>
            </tr>
        </table>
- Data structures, B+ tree, selection tree and heap implementation details are following.<br/>
    <h4>1. B+ Tree</h4>
        - Degree of tree is 3.<br/>
        - Book information will be saved when reading datas in here.<br/>
        - Make new node if new book is entered.<br/>
        - Increase borrowing amount of books if existing book is entered.<br/>
        - It is based on the book name and is not case sensitive.<br/>
    <h4>2. Selection Tree (Min winner Tree)</h4>
        - Number of runs is equal to number of codes.<br/>
        - Unborrowable books will be saved in corresponding runs.<br/>
        &emsp;The book must be deleted from B+ tree.<br/>
        - Selection tree is also rearraanged if heap is rearranged.<br/>
    <h4>3. Heap</h4>
        - Left child will be created first if parent node has no child.<br/>
        - When rearrange heap, reconstructing is over if every parent node's book name is less or equal to child's.<br/>
        - Selection tree is also rearraanged if heap is rearranged.<br/><br/>


- User info manager is worked some commands, `LOAD`, `ADD`, `QPOP`, `SEARCH`, `PRINT`, `DELETE` and `EXIT`.\
Each commands are explained in following table.

    <table>
        <tr>
            <th colspan='4'> <h2>Command Workout</h2></td>
        </tr>
        <tr>
            <th> Command </td>
            <th> Work </td>
            <th> Error code </td>
            <th> Example </td>
        </tr>
        <tr>
            <td> LOAD </td>
            <td> 1. Load data from text file.<br/>
                2. Save member information to B+ tree.<br/>
            </td>
            <td> 100 </td>
            <td> LOAD </td>
        </tr>
        <tr>
            <td> ADD </td>
            <td> Add book data into B+ tree. </td>
            <td> 200 </td>
            <td> ADD    harry potter    300 J.K.Rowling 1990<br/>
                (Empty space is \t)
            </td>
        </tr>
        <tr>
            <td> SEARCH_BP </td>
            <td> 1. Search the book information.<br/>
                &emsp;If command option is one, find the book corresponding to entered.<br/>
                &emsp;If command option is two, print the range search result.
            </td>
            <td> 300 </td>
            <td> SEARCH_BP harry<br/>
                SEARCH_BP a z
            </td>
        </tr>
        <tr>
            <td> PRINT_BP </td>
            <td> Prints member information corresponding to the entered name. </td>
            <td> 400 </td>
            <td> PRINT_BP </td>
        </tr>
        <tr>
            <td> PRINT_ST </td>
            <td> Print every book info in selection tree corresponding to entered code.<br/>
                Printing order is ascending order based on book name.</td>
            <td> 500 </td>
            <td> PRINT_ST 300 </td>
        </tr>
        <tr>
            <td> DELETE </td>
            <td> Delete root node of selection tree and its book infromation.<br/>
                Rearrange if heap condition is unsatisfied after deletion.
            </td>
            <td> 600 </td>
            <td> DELETE </td>
        </tr>
        <tr>
            <td> EXIT </td>
            <td> Free the program from memory and terminate the program. </td>
            <td> 700 </td>
            <td> EXIT </td>
        </tr>
    </table>

    <table>
        <tr>
            <th colspan='2'> <h2>Error condition</h2></td>
        </tr>
        <tr>
            <th> Command </td>
            <th> Situation that error's occured </td>
        </tr>
        <tr>
            <td> LOAD </td>
            <td> 1. Data file does not exist.<br/>
                2. Any member information is in "B+ tree".<br/>
            </td>
        </tr>
        <tr>
            <td> ADD </td>
            <td> Data format is not fit. </td>
        </tr>
        <tr>
            <td> SEARCH_BP </td>
            <td> 1. Number of entered option is none or more than two<br/>
                2. There's no data in B+ tree
            </td>
        </tr>
        <tr>
            <td> PRINT_BP </td>
            <td> There's no data in B+ tree </td>
        </tr>
        <tr>
            <td> PRINT_ST </td>
            <td> 1. There's no data in Selection tree.<br/>
                2. There's no Heap corresponding to code.<br/>
                3. Wrong code is entered.
            </td>
        </tr>
        <tr>
            <td> DELETE </td>
            <td> There's no data in Selection tree. </td>
        </tr>
    </table>

- After run this program, log  file `log.txt` is remained and there are result of each command.
- Run following codes when you run this program. Current directory must be the root directory of this project.
    ``` bash
    make
    ./run
    ```
    
Reference: https://github.com/DSLDataStorage/DS_Project_2_2023_2