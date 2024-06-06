#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <conio.h>
#include <cmath>
#include <cctype>

using namespace std;
const int MAX_ITEM = 100;

struct MenuItem
{
    string name;
    float price;
    string category;
    MenuItem(string n, float p, string c) : name(n), price(p), category(c) {}
};

struct Node
{
    MenuItem data;
    Node *next;
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
    vector<Node *> nodeList;

    // Store all nodes in a vector
    while (temp)
    {
        nodeList.push_back(temp);
        temp = temp->next;
    }

    // Sort the vector based on the character at the current position
    sort(nodeList.begin(), nodeList.end(), [pos](Node *a, Node *b)
         {
        if (pos >= a->data.name.length() && pos >= b->data.name.length()) {
            return false;
        } else if (pos >= a->data.name.length()) {
            return true;
        } else if (pos >= b->data.name.length()) {
            return false;
        } else {
            return a->data.name[pos] < b->data.name[pos];
        } });

    // Rebuild the linked list from the sorted vector
    Node *newHead = nullptr;
    Node *tail = nullptr;
    for (Node *node : nodeList)
    {
        node->next = nullptr;
        if (newHead == nullptr)
        {
            newHead = node;
        }
        else
        {
            tail->next = node;
        }
        tail = node;
    }

    head = newHead;
}

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

void countSortDescending(Node*& head, int pos) {
    Node* temp = head;
    vector<Node*> nodeList;

    // Store all nodes in a vector
    while (temp) {
        nodeList.push_back(temp);
        temp = temp->next;
    }

    // Sort the vector based on the character at the current position in descending order
    sort(nodeList.begin(), nodeList.end(), [pos](Node* a, Node* b) {
        if (pos >= a->data.name.length() && pos >= b->data.name.length()) {
            return false;
        }
        else if (pos >= a->data.name.length()) {
            return false;
        }
        else if (pos >= b->data.name.length()) {
            return true;
        }
        else {
            return a->data.name[pos] > b->data.name[pos];
        }
    });

    // Rebuild the linked list from the sorted vector
    Node* newHead = nullptr;
    Node* tail = nullptr;
    for (Node* node : nodeList) {
        node->next = nullptr;
        if (newHead == nullptr) {
            newHead = node;
        }
        else {
            tail->next = node;
        }
        tail = node;
    }

    head = newHead;
}

void radixSortDescending(Node*& head) {
    int maxLen = 0;
    Node* temp = head;
    while (temp) {
        maxLen = max(maxLen, static_cast<int>(temp->data.name.length()));
        temp = temp->next;
    }

    for (int pos = maxLen - 1; pos >= 0; pos--) {
        countSortDescending(head, pos);
    }
}

//bucket sort
void bucketSort(Node *&head)
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
    vector<vector<Node *>> buckets(numBuckets);

    // Distribute the menu items into the buckets based on their price
    temp = head;
    while (temp)
    {
        int bucketIndex = static_cast<int>(floor(temp->data.price / maxPrice * (numBuckets - 1)));
        buckets[bucketIndex].push_back(temp);
        temp = temp->next;
    }

    // Sort each bucket individually
    for (int i = 0; i < numBuckets; i++)
    {
        sort(buckets[i].begin(), buckets[i].end(), [](Node *a, Node *b) {
            return a->data.price < b->data.price;
        });
    }

    // Rebuild the linked list from the sorted buckets
    Node *newHead = nullptr;
    Node *tail = nullptr;
    for (int i = 0; i < numBuckets; i++)
    {
        for (Node *node : buckets[i])
        {
            if (!newHead)
            {
                newHead = node;
                tail = node;
            }
            else
            {
                tail->next = node;
                tail = node;
            }
        }
    }

    // Set the tail's next pointer to nullptr to mark the end of the list
    tail->next = nullptr;

    // Update the head pointer
    head = newHead;

    cout << "Menu sorted using bucket sort." << endl;
}

void printMenu(Node *head)
{
    if (head == nullptr)
    {
        cout << "The menu is empty." << endl;
        return;
    }

    cout << "Menu Items:" << endl;
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

// Ternary Search
int ternarySearch(int l, int r, string key, vector<MenuItem> &items)
{
    if (r >= l)
    {
        int mid1 = l + (r - l) / 3;
        int mid2 = r - (r - l) / 3;

        if (items[mid1].name == key)
        {
            return mid1;
        }
        if (items[mid2].name == key)
        {
            return mid2;
        }

        if (key < items[mid1].name)
        {
            return ternarySearch(l, mid1 - 1, key, items);
        }
        else if (key > items[mid2].name)
        {
            return ternarySearch(mid2 + 1, r, key, items);
        }
        else
        {
            return ternarySearch(mid1 + 1, mid2 - 1, key, items);
        }
    }

    return -1;
}

// Utility function to convert a string to lowercase
string toLowerCase(const string &str)
{
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// jump search
vector<int> jumpSearch(const vector<MenuItem> &items, const string &key)
{
    vector<int> results;
    int n = items.size();
    int step = sqrt(n);
    int prev = 0;
    string lowerKey = toLowerCase(key);

    while (prev < n)
    {
        // Check within the current block
        for (int i = prev; i < min(step, n); i++)
        {
            if (toLowerCase(items[i].name).find(lowerKey) != string::npos)
            {
                results.push_back(i);
            }
        }
        prev = step;
        step += sqrt(n);
    }

    return results;
}

void searchMenu(Node *head, const string &itemName, int searchType)
{
    vector<MenuItem> items;
    Node *temp = head;

    while (temp)
    {
        items.push_back(temp->data);
        temp = temp->next;
    }

    sort(items.begin(), items.end(), [](MenuItem a, MenuItem b)
         { return a.name < b.name; });

    vector<int> indices;
    if (searchType == 1)
    {
        int index = ternarySearch(0, items.size() - 1, itemName, items);
        if (index != -1)
        {
            indices.push_back(index);
        }
    }
    else if (searchType == 2)
    {
        indices = jumpSearch(items, itemName);
    }

    if (!indices.empty())
    {
        cout << "Items found: " << endl;
        for (int index : indices)
        {
            cout << items[index].name << " - RM" << fixed << setprecision(2) << items[index].price << " (" << items[index].category << ")" << endl;
        }
    }
    else
    {
        cout << "Item not found in the menu." << endl;
    }
}

void goBackToMenu(Node *&head);

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
            vector<Node *> nodeList;
            temp = head;
            while (temp)
            {
                nodeList.push_back(temp);
                temp = temp->next;
            }
            sort(nodeList.begin(), nodeList.end(), [](Node *a, Node *b)
                 { return a->data.price > b->data.price; });
            for (Node *node : nodeList)
            {
                cout << itemNumber << ") " << node->data.name << " RM" << fixed << setprecision(2) << node->data.price << endl;
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
    cout << "4. Delete" << endl;
    cout << "5. EXIT" << endl;
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
        system("cls");
        fflush(stdin);
        cout << "----------------------------------------" << endl;
        cout << "          Entering new item         " << endl;
        cout << "----------------------------------------" << endl;
        fflush(stdin);
        cout << "Enter the name of the new item : ";
        getline(cin, itemName);

        cout << "Enter the price of the new item : RM ";
        cin >> itemPrice;

        cout << "Enter the category of the new item : ";
        cin.ignore();
        getline(cin, itemCategory);

        // Open the menu file for appending
        menuFile.open("Menu.txt", ios::in | ios::out | ios::app);
        if (menuFile.is_open())
        {
            // Write the new item to the file
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
    } // end of else if 1
    else if (displayChoice == 2)
    {
        system("cls");
        printMenu(head);
        goBackToMenu(head);
    }
    break;

    case 2:

        int sortChoice;
        cout << "Sorted Menu Options:" << endl;
        cout << "1. Sort alphabetically" << endl;
        cout << "2. Sort alphabetically (Z-A)" << endl;
        cout << "3. Sort by price (lowest to highest)" << endl;
        cout << "4. Search" << endl;
        cout << "Enter your choice: ";
        cin >> sortChoice;

        if (sortChoice == 1)
        {
            system("cls");
            radixSort(head);
            printMenu(head);
            goBackToMenu(head);
        }
        else if (sortChoice == 2) { 
        system("cls");
        radixSortDescending(head); 
        printMenu(head);
        goBackToMenu(head);
    }
        else if (sortChoice == 4)
        {
            int searchType;
            system("cls");
            cout << "Search Options:" << endl;
            cout << "1. Ternary Search" << endl;
            cout << "2. Jump Search" << endl;
            cout << "Enter your choice: ";
            cin >> searchType;

            system("cls");
            cout << "Enter the name of the item to search: ";
            cin.ignore();
            getline(cin, itemName);
            searchMenu(head, itemName, searchType);
            goBackToMenu(head);
        }
        break;

 case 3:

        fflush(stdin);
        cout << "Entering new item";
        cout << "Enter the name of the new item : ";
        getline(cin, itemName);

        cout << "Enter the price of the new item : RM ";
        cin >> itemPrice;

        cout << "Enter the category of the new item : ";
        cin.ignore();
        getline(cin, itemCategory);

        menuFile.open("Menu.txt", ios::in | ios::out | ios::app);
        if (menuFile.is_open())
        {
            menuFile << itemName << " " << fixed << setprecision(2) << itemPrice << " " << itemCategory << endl;
            cout << "Item added successfully.\n\n";
            menuFile.close();

            menuFile.open("Menu.txt", ios::in);
            if (menuFile.is_open())
            {
                vector<string> itemNames;
                vector<float> itemPrices;
                vector<string> itemCategories;
                string name, category;
                float price;

                while (menuFile >> name >> price >> category)
                {
                    itemNames.push_back(name);
                    itemPrices.push_back(price);
                    itemCategories.push_back(category);
                }

                cout << "Updated Menu" << endl;
                for (int i = 0; i < itemNames.size(); i++)
                {
                    cout << i + 1 << ") " << setw(30) << left << itemNames[i] << " RM" << fixed << setprecision(2) << itemPrices[i] << " (" << itemCategories[i] << ")" << endl;
                }
            }

            menuFile.close();
            goBackToMenu(head);
        }
        else
        {
            cout << "Error opening Menu.txt for writing.\n";
        }
        break;

    case 4:
        fflush(stdin);
        cout << "Enter the name of the item to delete: ";
        getline(cin, itemToDelete);
        deleteMenuItem(head, itemToDelete);
        goBackToMenu(head); // This line is executed even if the item is not found
        break;

    case 5:
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
    cout << "\nEnter 1 to go back to the previous menu: ";
    int backChoice;
    cin >> backChoice;
    if (backChoice == 1)
    {
        showMenuOptions(head);
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
