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

    // Sort the linked list using radix sort
    radixSort(head);

    // Display sorted search results
    cout << "\nSorted Search Results by Name:" << endl;
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