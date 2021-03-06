#**SUPERMARKET VENDE++**

## SPECIFICATIONS 
 * For testing purposes, the Bottom 10 reccomendation system contains a variable name N_COMMON, present
 	in the Bottom_10.h file, by default it is 10, but it can be changed to another number in order to **test the Bottom_10 functionality without doing a bottom 10**, example below:

>**N_COMMON = 10  ---> Bottom 10**.

>**N_COMMON =  7  ----> Bottom 7**.
 
>**N_COMMON =  3  ----> Bottom 3**.

 This is useful since it allows us to test the program without needing to create big amounts of data, so **change its number as needed**.

 * The Bottom 10 advertisement can **recommend multiple products** if that is the case.

 * In the beginning of the program, if you insert a name of a file Windows cannot open, **the program will close**, if you insert the name of a file Windows can open, it will check if it is the file requested and if not it asks the user to **reinput the file name**.

 * There cannot exist transactions which do not have any product bought.

 * 2 Clients **cannot have the same name**.

 * In the Bottom 10 reccomendation system, we did not use maps or multimaps, and instead used a 
 	vector of pairs, so that we could order it by value and not by key, which makes it easier to
 	find the Bottom 10, since they will be the first 10 costumers.

 * Everytime the program asks the user to input a reference the a certain client, it is always done by **referencing their name** and never the number for efficiency reasons.

 * Everytime information is asked to the user it always as a way to cancel, usually by inputting CTRL+Z.
 
 * We know that the user will probably not do this, but if he types, anywhere in the program, a string followed by CTRL+Z ("kdjjsja^Z", for example), he has to press enter 2 times, so the program can assume that was an invalid input. We couldn´t modified this, and we just want to inform you that we know that our program does this. 
 
 * All the classes in this program follow the **singleton design pattern**, meaning only one instance of the class can exist at all times.

 * All the information changed during runtime, like removing client or adding a transaction, is **lost if the program is closed**, unless closed by going to the Main Menu and selecting the exit option.

## CLASSES

> **SUPERMARKET**

>>Its a class that contains the classes below and is responsible for the Menus.


>> **CLIENT** Class
>>> Handles all of the client information, client information is stored in a **set<Client_t>**, sorted by Alphabetic order.

>> **TRANS** Class
>>> Handles all of the transactions information, this information is stored in a **set<Trans_t>**, sorted by the date of the transaction, earlier date is the first transaction.

>> **PRODUCT** Class
>>>Handles all the product information, its name and its price, this information is stored in a **map<string,double>**, therefore sorted by Alphabetic product order.

>> **BOTTOM_10** Class
>>> Responsible for the Bottom 10 publicity, its data member CtoT, is also used in the regular Advertisement. This `vector<pair <unsigned int , vector <string > > >` , holds a single vector<string> for each client, which represents all the products the client bought in the store.

>> **VISUALIZE** Class
>>> Handles the common visualizing options in classes, such as number visualization and name visualization, it has **no data members.**


>> **INPUT_ASKER** Class
>>>Handles the majority of the input of the programs, it always asks the client name and never the client ID, **no data members.**

