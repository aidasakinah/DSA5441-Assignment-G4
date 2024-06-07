#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <cmath>
#include <cctype>

using namespace std;
const int MAX_ITEM = 100;

struct MenuItem
{
    string name;
    float price;
    string category;
    MenuItem() : name(""), price(0.0), category("") {}
    MenuItem(string n, float p, string c) : name(n), price(p), category(c) {}
};

struct Node
{
    MenuItem data;
    Node *next;

    Node() : next(nullptr) {}
    Node(MenuItem item) : data(item), next(nullptr) {}
};

void insertMenuItem(Node *&head, MenuItem item)
{
    Node *newNode = new Node(item);
    if (!head)
    {
        head = newNode;
        return;
    }
    Node *curr = head;
    while (curr->next)
    {
        curr = curr->next;
    }
    curr->next = newNode;
}

void deleteMenuItem(Node *&head, const string &name)
{
    if (!head)
    {
        cout << "Menu is empty. Nothing to delete." << endl;
        return;
    }

    Node *curr = head;
    Node *prev = nullptr;

    // Handle the case where the head node needs to be deleted
    if (curr->data.name == name)
    {
        head = curr->next;
        delete curr;
        cout << "Item \"" << name << "\" deleted successfully." << endl;
        return;
    }

    // Traverse the linked list to find the node to delete
    while (curr != nullptr && curr->data.name != name)
    {
        prev = curr;
        curr = curr->next;
    }

    // If the item is not found
    if (curr == nullptr)
    {
        cout << "Item \"" << name << "\" not found in the menu." << endl;
        return;
    }

    // Delete the node
    prev->next = curr->next;
    delete curr;
    cout << "Item \"" << name << "\" deleted successfully." << endl;
}

void countSort(Node *&head, int pos)
{
    Node *temp = head;
    Node *nodeList[MAX_ITEM];
    int count = 0;

    // Store all nodes in an array
    while (temp)
    {
        nodeList[count++] = temp;
        temp = temp->next;
    }

    // Sort the array based on the character at the current position
    sort(nodeList, nodeList + count, [pos](Node *a, Node *b)
         {
        if (pos >= a->data.name.length() && pos >= b->data.name.length()) {
            return false;
        } else if (pos >= a->data.name.length()) {
            return false;
        } else if (pos >= b->data.name.length()) {
            return true;
        } else {
            return a->data.name[pos] < b->data.name[pos];
        } });

    // Rebuild the linked list from the sorted array
    Node *newHead = nullptr;
    Node *tail = nullptr;
    for (int i = 0; i < count; i++)
    {
        nodeList[i]->next = nullptr;
        if (newHead == nullptr)
        {
            newHead = nodeList[i];
        }
        else
        {
            tail->next = nodeList[i];
        }
        tail = nodeList[i];
    }

    head = newHead;
}

// alphabetically ascending
void radixSort(Node *&head)
{
    int maxLen = 0;
    Node *temp = head;
    while (temp)
    {
        maxLen = max(maxLen, static_cast<int>(temp->data.name.length()));
        temp = temp->next;
    }

    for (int pos = maxLen - 1; pos >= 0; pos--)
    {
        countSort(head, pos);
    }
}

void countSortDescending(Node *&head, int pos)
{
    Node *temp = head;
    Node *nodeList[MAX_ITEM];
    int count = 0;

    // Store all nodes in an array
    while (temp)
    {
        nodeList[count++] = temp;
        temp = temp->next;
    }

    // Sort the array based on the character at the current position in descending order
    sort(nodeList, nodeList + count, [pos](Node *a, Node *b)
         {
        if (pos >= a->data.name.length() && pos >= b->data.name.length()) {
            return false;
        }
        else if (pos >= a->data.name.length()) {
            return true;
        }
        else if (pos >= b->data.name.length()) {
            return false;
        }
        else {
            return a->data.name[pos] > b->data.name[pos];
        } });

    // Rebuild the linked list from the sorted array
    Node *newHead = nullptr;
    Node *tail = nullptr;
    for (int i = 0; i < count; i++)
    {
        nodeList[i]->next = nullptr;
        if (newHead == nullptr)
        {
            newHead = nodeList[i];
        }
        else
        {
            tail->next = nodeList[i];
        }
        tail = nodeList[i];
    }

    head = newHead;
}

// aplhabetic descending
void radixSortDescending(Node *&head)
{
    int maxLen = 0;
    Node *temp = head;
    while (temp)
    {
        maxLen = max(maxLen, static_cast<int>(temp->data.name.length()));
        temp = temp->next;
    }

    for (int pos = maxLen - 1; pos >= 0; pos--)
    {
        countSortDescending(head, pos);
    }
}

// Custom sorting algorithm: Find minimum and swap
void customSort(Node *&head)
{
    if (!head)
        return;

    Node *current = head;
    while (current)
    {
        Node *minNode = current;
        Node *temp = current->next;
        while (temp)
        {
            if (temp->data.price < minNode->data.price)
            {
                minNode = temp;
            }
            temp = temp->next;
        }
        if (minNode != current)
        {
            swap(current->data, minNode->data);
        }
        current = current->next;
    }
}

// Bucket sort in ascending order
void bucketSortAscending(Node *&head)
{
    if (!head)
    {
        cout << "The menu is empty. Nothing to sort." << endl;
        return;
    }

    // Find the maximum price in the menu
    float maxPrice = head->data.price;
    Node *temp = head->next;
    while (temp)
    {
        maxPrice = max(maxPrice, temp->data.price);
        temp = temp->next;
    }

    // Create buckets based on the maximum price
    int numBuckets = static_cast<int>(ceil(sqrt(maxPrice)));
    Node *buckets[numBuckets] = {nullptr};

    // Distribute the menu items into the buckets based on their price
    temp = head;
    while (temp)
    {
        int bucketIndex = static_cast<int>(floor(temp->data.price / maxPrice * (numBuckets - 1)));
        // Add item to the front of the bucket's linked list
        Node *newNode = new Node(temp->data);
        newNode->next = buckets[bucketIndex];
        buckets[bucketIndex] = newNode;
        temp = temp->next;
    }

    // Sort each bucket individually using custom sort
    for (int i = 0; i < numBuckets; ++i)
    {
        customSort(buckets[i]);
    }

    // Rebuild the linked list from the sorted buckets
    Node *newHead = nullptr;
    Node *tail = nullptr;
    for (int i = 0; i < numBuckets; ++i)
    {
        Node *current = buckets[i];
        while (current)
        {
            if (!newHead)
            {
                newHead = current;
                tail = current;
            }
            else
            {
                tail->next = current;
                tail = current;
            }
            current = current->next;
        }
    }

    // Set the tail's next pointer to nullptr to mark the end of the list
    if (tail)
        tail->next = nullptr;

    // Update the head pointer
    head = newHead;
}

// Custom sorting algorithm: Find maximum and swap
void customSortDescending(Node *&head)
{
    if (!head)
        return;

    Node *current = head;
    while (current)
    {
        Node *maxNode = current;
        Node *temp = current->next;
        while (temp)
        {
            if (temp->data.price > maxNode->data.price)
                maxNode = temp;
            temp = temp->next;
        }
        if (maxNode != current)
            swap(current->data, maxNode->data);
        current = current->next;
    }
}

// Bucket sort in descending order
void bucketSortDescending(Node *&head)
{
    if (!head)
    {
        cout << "The menu is empty. Nothing to sort." << endl;
        return;
    }

    // Find the maximum price in the menu
    float maxPrice = head->data.price;
    Node *temp = head->next;
    while (temp)
    {
        maxPrice = max(maxPrice, temp->data.price);
        temp = temp->next;
    }

    // Create array of linked lists for buckets
    int numBuckets = static_cast<int>(ceil(sqrt(maxPrice)));
    Node *buckets[numBuckets] = {nullptr};

    // Distribute the menu items into the buckets based on their price
    temp = head;
    while (temp)
    {
        int bucketIndex = static_cast<int>(floor(temp->data.price / maxPrice * (numBuckets - 1)));
        // Add item to the front of the bucket's linked list
        Node *newNode = new Node;
        newNode->data = temp->data;
        newNode->next = buckets[bucketIndex];
        buckets[bucketIndex] = newNode;
        temp = temp->next;
    }

    // Sort each bucket individually using custom sort in descending order
    for (int i = 0; i < numBuckets; ++i)
    {
        customSortDescending(buckets[i]);
    }

    // Rebuild the linked list from the sorted buckets in descending order
    Node *newHead = nullptr;
    Node *tail = nullptr;
    for (int i = numBuckets - 1; i >= 0; --i)
    {
        Node *current = buckets[i];
        while (current)
        {
            if (!newHead)
            {
                newHead = current;
                tail = current;
            }
            else
            {
                tail->next = current;
                tail = current;
            }
            current = current->next;
        }
    }

    // Set the tail's next pointer to nullptr to mark the end of the list
    tail->next = nullptr;

    // Update the head pointer
    head = newHead;
}

// to print menu from txt file
void printMenu(Node *head)
{
    if (head == nullptr)
    {
        cout << "The menu is empty." << endl;
        return;
    }

    cout << string(65, '-') << endl;
    cout << setw(30) << "Menu" << endl;
    cout << string(65, '-') << endl;
    cout << left << setw(30) << "Name" << setw(15) << "Price (RM)" << setw(20) << "Category" << endl;
    cout << string(65, '-') << endl;

    Node *temp = head;
    int itemNumber = 1;
    while (temp != nullptr)
    {
        cout << left << setw(30) << temp->data.name << setw(15) << fixed << setprecision(2) << temp->data.price << setw(20) << temp->data.category << endl;
        temp = temp->next;
        itemNumber++;
    }
}

// Utility function to convert a string to lowercase
string toLowerCase(const string &str)
{
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Ternary Search
void ternarySearch(Node *head, const string &key)
{
    if (!head)
        return;

    // Convert key to lowercase
    string lowerKey = toLowerCase(key);

    // Display unsorted search results
    cout << "Unsorted Search Results:" << endl;
    bool foundUnsorted = false;
    Node *temp = head;
    while (temp)
    {
        string itemName = toLowerCase(temp->data.name);
        if (itemName.find(lowerKey) != string::npos)
        {
            cout << temp->data.name << " - RM" << fixed << setprecision(2) << temp->data.price << " (" << temp->data.category << ")" << endl;
            foundUnsorted = true;
        }
        temp = temp->next;
    }

    if (!foundUnsorted)
    {
        cout << "Item not found in the menu." << endl;
        return;
    }

    // Sort the linked list in ascending order using bucket sort
    bucketSortAscending(head);

    // Display sorted search results in ascending order
    cout << "\nSorted Search Results by Price (Ascending):" << endl;
    bool foundSortedAscending = false;
    temp = head;
    while (temp)
    {
        string itemName = toLowerCase(temp->data.name);
        if (itemName.find(lowerKey) != string::npos)
        {
            cout << temp->data.name << " - RM" << fixed << setprecision(2) << temp->data.price << " (" << temp->data.category << ")" << endl;
            foundSortedAscending = true;
        }
        temp = temp->next;
    }

    if (!foundSortedAscending)
    {
        cout << "Item not found in the menu." << endl;
        return;
    }

    // Sort the linked list in descending order using bucket sort
    bucketSortDescending(head);

    // Display sorted search results in descending order
    cout << "\nSorted Search Results by Price (Descending):" << endl;
    bool foundSortedDescending = false;
    temp = head;
    while (temp)
    {
        string itemName = toLowerCase(temp->data.name);
        if (itemName.find(lowerKey) != string::npos)
        {
            cout << temp->data.name << " - RM" << fixed << setprecision(2) << temp->data.price << " (" << temp->data.category << ")" << endl;
            foundSortedDescending = true;
        }
        temp = temp->next;
    }

    if (!foundSortedDescending)
    {
        cout << "Item not found in the menu." << endl;
    }
}

// Jump Search
void jumpSearch(Node *head, const string &key)
{
    if (!head)
        return;

    // Convert key to lowercase
    string lowerKey = toLowerCase(key);

    // Create a dummy array to hold the nodes
    Node *dummy[MAX_ITEM];
    int count = 0;

    Node *temp = head;
    while (temp)
    {
        dummy[count++] = temp;
        temp = temp->next;
    }

    // Display unsorted search results
    cout << "Unsorted Search Results:" << endl;
    bool found = false;
    for (int i = 0; i < count; ++i)
    {
        string itemName = toLowerCase(dummy[i]->data.name);
        if (itemName.find(lowerKey) != string::npos)
        {
            cout << dummy[i]->data.name << " - RM" << fixed << setprecision(2) << dummy[i]->data.price << " (" << dummy[i]->data.category << ")" << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "Item not found in the menu." << endl;
        return;
    }

    // Sort the linked list using radix sort in ascending order
    radixSort(head);

    // Display sorted search results in ascending order
    cout << "\nSorted Search Results by Name (Ascending):" << endl;
    temp = head;
    found = false;
    while (temp)
    {
        string itemName = toLowerCase(temp->data.name);
        if (itemName.find(lowerKey) != string::npos)
        {
            cout << temp->data.name << " - RM" << fixed << setprecision(2) << temp->data.price << " (" << temp->data.category << ")" << endl;
            found = true;
        }
        temp = temp->next;
    }

    if (!found)
    {
        cout << "Item not found in the menu." << endl;
    }

    // Sort the linked list using radix sort in descending order
    radixSortDescending(head);

    // Display sorted search results in descending order
    cout << "\nSorted Search Results by Name (Descending):" << endl;
    temp = head;
    found = false;
    while (temp)
    {
        string itemName = toLowerCase(temp->data.name);
        if (itemName.find(lowerKey) != string::npos)
        {
            cout << temp->data.name << " - RM" << fixed << setprecision(2) << temp->data.price << " (" << temp->data.category << ")" << endl;
            found = true;
        }
        temp = temp->next;
    }

    if (!found)
    {
        cout << "Item not found in the menu." << endl;
    }
}

void searchResults(Node *head, const string &itemName, int searchType)
{
    if (searchType == 1)
    {
        cout << "----------------------------------------" << endl;
        cout << "         Ternary Search Results         " << endl;
        cout << "----------------------------------------" << endl;
        ternarySearch(head, itemName);
    }
    else if (searchType == 2)
    {
        cout << "----------------------------------------" << endl;
        cout << "           Jump Search Results          " << endl;
        cout << "----------------------------------------" << endl;
        jumpSearch(head, itemName);
    }
}

void goBackToMenu(Node *&head);
void showMenuOptions(Node *&head);

void addMenuItem(Node *&head)
{
    string itemName;

    system("cls");
    fflush(stdin);
    cout << "----------------------------------------" << endl;
    cout << "\tEntering new item" << endl;
    cout << "----------------------------------------" << endl;

    cout << "Enter the name of the new item (no spaces) : ";
    getline(cin, itemName);

    float itemPrice;
    cout << "Enter the price of the new item : RM ";
    cin >> itemPrice;

    string itemCategory;
    cout << "Enter the category of the new item (FOOD/BEVERAGE): ";
    cin.ignore();
    getline(cin, itemCategory);
    transform(itemCategory.begin(), itemCategory.end(), itemCategory.begin(), ::toupper);

    fstream menuFile("Menu.txt", ios::in | ios::out | ios::app);
    if (menuFile.is_open())
    {
        menuFile << itemName << " " << fixed << setprecision(2) << itemPrice << " " << itemCategory << endl;
        cout << "Item added successfully.\n\n";
        menuFile.close();

        // Update the linked list with the new item
        insertMenuItem(head, MenuItem(itemName, itemPrice, itemCategory));

        // Display the updated menu
        printMenu(head);
        goBackToMenu(head);
    }
    else
    {
        cout << "Error opening Menu.txt for writing.\n";
    }
}

// menu sorting algorithm
// finished
void algorithmSortMenu(Node *&head)
{
    int sortalgo, radixChoice, bucketChoice;
    system("cls");
    cout << "----------------------------------------" << endl;
    cout << "            Sorting Algorithms          " << endl;
    cout << "----------------------------------------" << endl;
    cout << "1. Radix Sort" << endl;
    cout << "2. Bucket Sort" << endl;
    cout << "3. View unsorted data" << endl;
    cout << "4. Return to homepage" << endl;
    cout << "\nEnter your choice: ";
    cin >> sortalgo;

    if (sortalgo == 1)
    {
        system("cls");
        cout << "----------------------------------------" << endl;
        cout << "               Radix Sort               " << endl;
        cout << "----------------------------------------" << endl;
        cout << "1. Alphabetically (A-Z)" << endl;
        cout << "2. Alphabetically (Z-A)" << endl;
        cout << "3. Return to previous page" << endl;
        cout << "\nEnter your choice: ";
        cin >> radixChoice;

        if (radixChoice == 1)
        {
            system("cls");
            radixSort(head);
            printMenu(head);
            goBackToMenu(head);
            algorithmSortMenu(head);
        }
        else if (radixChoice == 2)
        {
            system("cls");
            radixSortDescending(head);
            printMenu(head);
            goBackToMenu(head);
        }
        else if (radixChoice == 3)
        {
            algorithmSortMenu(head);
        }
    }
    else if (sortalgo == 2)
    {
        system("cls");
        cout << "----------------------------------------" << endl;
        cout << "               Bucket Sort              " << endl;
        cout << "----------------------------------------" << endl;
        cout << "1. Price Range (Lowest-Highest)" << endl;
        cout << "2. Price Range (Highest to Lowest)" << endl;
        cout << "3. Return to previous page" << endl;
        cout << "\nEnter your choice: ";
        cin >> bucketChoice;

        if (bucketChoice == 1)
        {
            system("cls");
            bucketSortAscending(head);
            printMenu(head);
            goBackToMenu(head);
        }
        else if (bucketChoice == 2)
        {
            system("cls");
            bucketSortDescending(head);
            printMenu(head);
            goBackToMenu(head);
        }
        else if (bucketChoice == 3)
        {
            algorithmSortMenu(head);
        }
    }
    else if (sortalgo == 3)
    {
        system("cls");
        printMenu(head);
        goBackToMenu(head);
    }
    else if (sortalgo == 4)
    {
        showMenuOptions(head);
    }
    else
    {
        cout << "Invalid choice. Please select again." << endl;
        algorithmSortMenu(head);
    }
}

// menu search algorithm
void algorithmSearchMenu(Node *&head)
{
    int searchType;
    system("cls");
    cout << "----------------------------------------" << endl;
    cout << "            Search Algorithms           " << endl;
    cout << "----------------------------------------" << endl;
    cout << "1. Ternary Search" << endl;
    cout << "2. Jump Search" << endl;
    cout << "3. Return to homepage" << endl;
    cout << "\nEnter your choice: ";
    cin >> searchType;

    if (searchType == 1)
    {
        system("cls");
        cout << "Enter the name of the item to search: ";
        string itemName;
        cin.ignore();
        getline(cin, itemName);
        searchResults(head, itemName, searchType);
        goBackToMenu(head);
    }
    else if (searchType == 2)
    {
        system("cls");
        cout << "Enter the name of the item to search: ";
        string itemName;
        cin.ignore();
        getline(cin, itemName);
        searchResults(head, itemName, searchType);
        goBackToMenu(head);
    }
    else if (searchType == 3)
    {
        showMenuOptions(head);
    }
    else
    {
        cout << "Invalid choice. Please select again." << endl;
        algorithmSearchMenu(head);
    }
}
class Restaurant
{
private:
    float price, itemPrice;
    string itemName;

public:
    void readFile(Node *&head, int displayChoice)
    {
        string menuFileName = "Menu.txt";
        ifstream menuFile(menuFileName.c_str(), ios::in);
        if (!menuFile)
        {
            cout << "File does not exist";
            exit(1);
        }

        system("cls");
        cout << "Food and Beverage" << endl;

        string name, category;
        float price;

        while (menuFile >> name >> price >> category)
        {
            insertMenuItem(head, MenuItem(name, price, category));
        }

        if (displayChoice == 3)
        {
            radixSort(head);
        }

        // Print the sorted menu
        Node *temp = head;
        int itemNumber = 1;
        if (displayChoice == 1)
        {
            // Sort from lowest price to highest price
            while (temp)
            {
                cout << itemNumber << ") " << temp->data.name << " RM" << fixed << setprecision(2) << temp->data.price << endl;
                temp = temp->next;
                itemNumber++;
            }
        }
        else if (displayChoice == 2)
        {
            // Sort from highest price to lowest price
            Node *nodeList[MAX_ITEM];
            int count = 0;
            temp = head;
            while (temp)
            {
                nodeList[count++] = temp;
                temp = temp->next;
            }
            sort(nodeList, nodeList + count, [](Node *a, Node *b)
                 { return a->data.price > b->data.price; });
            for (int i = 0; i < count; i++)
            {
                cout << itemNumber << ") " << nodeList[i]->data.name << " RM" << fixed << setprecision(2) << nodeList[i]->data.price << endl;
                itemNumber++;
            }
        }
        else if (displayChoice == 3)
        {
            // Sort alphabetically
            radixSort(head);
            // Print the sorted menu
            Node *temp = head;
            int itemNumber = 1;
            while (temp)
            {
                cout << itemNumber << ") " << temp->data.name << " RM" << fixed << setprecision(2) << temp->data.price << " (" << temp->data.category << ")" << endl;
                temp = temp->next;
                itemNumber++;
            }
        }
        menuFile.close();
    }
};

void showMenuOptions(Node *&head)
{
    int choice;
    string itemName, line, itemCategory, itemToDelete;
    float itemPrice;
    fstream menuFile;
    system("cls");
    cout << "\n---------------------------------------------------------" << endl;
    cout << "\tWelcome to Restaurant Fusion Fare Delight!" << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << "1. Unsorted Menu" << endl;
    cout << "2. Sorted Menu" << endl;
    cout << "3. Add Menu" << endl;
    cout << "4. EXIT" << endl;
    cout << "\nEnter your choice :";
    cin >> choice;
    cin.ignore();
    cout << endl;

    Node *temp = nullptr;
    int i = 1;

    switch (choice)
    {
    case 1:
        int displayChoice;
        system("cls");
        cout << "----------------------------------------" << endl;
        cout << "          Unsorted Menu Options         " << endl;
        cout << "----------------------------------------" << endl;
        cout << "1. Add Item" << endl;
        cout << "2. View Menu" << endl;
        cout << "\nEnter your choice : ";
        cin >> displayChoice;

        if (displayChoice == 1)
        {
            addMenuItem(head); // Call the addMenuItem function
        }
        else if (displayChoice == 2)
        {
            system("cls");
            printMenu(head);
            goBackToMenu(head);
        }
        break;

    case 2:

        int sortMenuChoice, sortalgo, radixChoice;

        system("cls");
        cout << "----------------------------------------" << endl;
        cout << "           Sorted Menu Options          " << endl;
        cout << "----------------------------------------" << endl;
        cout << "1. Sort Data" << endl;
        cout << "2. Search for Data" << endl;
        cout << "3. Return to homepage" << endl;
        cout << "\nEnter your choice: ";
        cin >> sortMenuChoice;

        if (sortMenuChoice == 1) // this is where sorted data is
        {
            algorithmSortMenu(head);
        }
        else if (sortMenuChoice == 2)
        {
            algorithmSearchMenu(head);
        }
        break;

    case 3:
        addMenuItem(head);
        break;

    case 4:
        cout << "Thank you for visiting Restaurant Fusion Fare Delights." << endl;
        exit(0);
        break;

    default:
        system("cls");
        cout << "Invalid choice. Please select again." << endl;
        showMenuOptions(head);
        break;
    } // end of switch
} // end function

void goBackToMenu(Node *&head)
{
    cout << "\nEnter 1 to go back to the homepage, 2 to Sorting Menu or 3 to Search Menu: ";
    int backChoice;
    cin >> backChoice;
    if (backChoice == 1)
    {
        showMenuOptions(head);
    }
    else if (backChoice == 2)
    {
        algorithmSortMenu(head);
    }
    else if (backChoice == 3)
    {
        algorithmSearchMenu(head);
    }
    else
    {
        cout << "Invalid choice. Please select again." << endl;
        goBackToMenu(head);
    }
}

int main()
{
    Node *head = nullptr;
    Restaurant R;

    // Read menu data from file and populate linked list
    R.readFile(head, 0); // Pass 0 as displayChoice to skip sorting

    showMenuOptions(head);

    // Clean up dynamically allocated memory
    Node *temp = head;
    while (temp != nullptr)
    {
        Node *next = temp->next;
        delete temp;
        temp = next;
    }

    return 0;
}
