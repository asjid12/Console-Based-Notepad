#include <iostream>
#include <string>
#include<fstream>
#include<Windows.h>
using namespace std;

class QuadNode {
public:
    char data;
    int X;
    int Y;
    QuadNode* TOP;
    QuadNode* BOTTOM;
    QuadNode* LEFT;
    QuadNode* RIGHT;

    QuadNode(int X = 0, int Y = 0, char data = '\0') {
        TOP = NULL;
        BOTTOM = NULL;
        LEFT = NULL;
        RIGHT = NULL;
        this->data = data;
        this->X = X;
        this->Y = Y;
    }
};

class NotePad {
public:
    int size;
    QuadNode* root;
    QuadNode* current;

    NotePad() {
        root = NULL;
        current = NULL;
    }

    void addText() {
        string S;
        cout << "Enter the string you want to add (use '+n' to start a new line): ";
        getline(cin, S);

        if (root == NULL) {
            QuadNode* newNode = new QuadNode(0, 0, S[0]);
            root = newNode;
            current = newNode;

            for (int i = 1; i < S.length(); i++) {
                QuadNode* newNode = new QuadNode(current->X, current->Y, S[i]);
                if (S[i] == '+' && i + 1 < S.length() && S[i + 1] == 'n')
                {
                    QuadNode* temp = root;
                    while (temp->BOTTOM != NULL)
                        temp = temp->BOTTOM;

                    i += 2;
                    newNode->data = S[i];

                    newNode->Y = temp->Y + 1;
                    temp->BOTTOM = newNode;
                    current = newNode;
                }
                else {
                    newNode->X = current->X + 1;
                    current->RIGHT = newNode;
                    newNode->LEFT = current;
                    current = newNode;
                }
            }
        }
        else {
            for (int i = 0; i < S.length(); i++) {
                QuadNode* newNode = new QuadNode(current->X, current->Y, S[i]);
                if (S[i] == '+' && i + 1 < S.length() && S[i + 1] == 'n') {
                    QuadNode* temp = root;
                    while (temp->BOTTOM != NULL)
                        temp = temp->BOTTOM;

                    newNode->Y = temp->Y + 1;
                    temp->BOTTOM = newNode;
                    current = newNode;
                }
                else {
                    newNode->X = current->X + 1;
                    current->RIGHT = newNode;
                    newNode->LEFT = current;
                    current = newNode;
                }
            }
        }
    }

    void deleteText()
    {
        string text;
        cout << "Enter the text you want to delete: ";
        getline(cin, text);

        QuadNode* temp = root;
        while (temp != NULL) {
            QuadNode* curr = temp;
            while (curr != NULL) {
                if (curr->data == text[0]) {
                    if (matchText(curr, text)) {
                        removeNodes(curr, text.length());
                        return;  // Return after deleting the first occurrence
                    }
                }
                curr = curr->RIGHT;
            }
            temp = temp->BOTTOM;
        }
        cout << "Text not found." << endl;
    }

    bool matchText(QuadNode* startNode, const string& text)
    {
        QuadNode* curr = startNode;
        int count = 0;

        while (curr != NULL && count < text.length())
        {
            if (curr->data != text[count])
            {
                return false;
            }
            curr = curr->RIGHT;
            count++;
        }

        return count == text.length();
    }

    void removeNodes(QuadNode* startNode, int length) {
        QuadNode* curr = startNode;
        QuadNode* rowStart = curr->TOP;

        for (int i = 0; i < length; i++) {
            if (curr->LEFT != NULL) {
                curr->LEFT->RIGHT = curr->RIGHT;
            }
            if (curr->RIGHT != NULL) {
                curr->RIGHT->LEFT = curr->LEFT;
            }

            if (curr->TOP != NULL) {
                curr->TOP->BOTTOM = curr->BOTTOM;
                curr->TOP = NULL;
            }
            if (curr->BOTTOM != NULL) {
                curr->BOTTOM->TOP = curr->TOP;
                curr->BOTTOM = NULL;
            }

            QuadNode* temp = curr;
            curr = curr->RIGHT;
            delete temp;
        }

         Update the rowStart after deletion
        if (rowStart != NULL) {
            while (rowStart->LEFT != NULL) {
                rowStart = rowStart->LEFT;
            }
            current = rowStart;
        }
    }

    void searchText()
    {
        string text;
        cout << "Enter the text you want to search: ";
        getline(cin, text);

        QuadNode* temp = root;
        int count = 1;
        bool found = false;

        while (temp != NULL) {
            QuadNode* curr = temp;
            while (curr != NULL) {
                if (curr->data == text[0]) {
                    if (matchText(curr, text)) {
                        cout << "Match found at position (" << curr->X << ", " << curr->Y << ")" << endl;
                        found = true;
                    }
                }
                curr = curr->RIGHT;
            }
            temp = temp->BOTTOM;
            count++;
        }

        if (!found) {
            cout << "Text not found." << endl;
        }
    }

    string copyText()
    {
        int startX, startY, endX, endY;
        cout << "Enter starting X: ";
        cin >> startX;
        cout << "Enter starting Y: ";
        cin >> startY;
        cout << "Enter ending X: ";
        cin >> endX;
        cout << "Enter ending Y: ";
        cin >> endY;
        string copiedText;

        QuadNode* temp = root;
        int countY = 0;

         Move to the start Y position
        while (temp != NULL && countY < startY) {
            temp = temp->BOTTOM;
            countY++;
        }

        if (temp == NULL) {
            return copiedText; // Return empty string if startY is out of bounds
        }

        QuadNode* curr = temp;
        int countX = 0;

         Move to the start X position
        while (curr != NULL && countX < startX) {
            curr = curr->RIGHT;
            countX++;
        }

        while (curr != NULL && countY <= endY) {
            if (countY == startY) {
                copiedText += curr->data;
            }
            else if (countY > startY && countY < endY) {
                copiedText += "\n";
                copiedText += curr->data;
            }
            else if (countY == endY) {
                copiedText += curr->data;
                break;
            }

            curr = curr->RIGHT;
            if (curr == NULL) {
                temp = temp->BOTTOM;
                countY++;
                if (temp != NULL) {
                    curr = temp;
                    countX = 0;
                }
            }
            else {
                countX++;
            }
        }

        return copiedText;
    }

    void pasteText()
    {
        int startX;
        int startY;
        cout << "Enter x: ";
        cin >> startX;
        cout << "Enter y: ";
        cin >> startY;

        string copiedText;
        cout << "Enter copied text: ";
        cin >> copiedText;

        QuadNode* temp = root;
        int countY = 0;

         Move to the start Y position
        while (temp != NULL && countY < startY) {
            temp = temp->BOTTOM;
            countY++;
        }

        if (temp == NULL) {
            cout << "Invalid Y position." << endl;
            return; // Return if startY is out of bounds
        }

        QuadNode* curr = temp;
        int countX = 0;

         Move to the start X position
        while (curr != NULL && countX < startX) {
            curr = curr->RIGHT;
            countX++;
        }

        for (char ch : copiedText)   // For every character in copied string
        {
            if (ch == '\n') {
                 Add new line
                QuadNode* newNode = new QuadNode(curr->X, curr->Y + 1, '+');
                newNode->BOTTOM = curr->BOTTOM;
                curr->BOTTOM = newNode;
                newNode->TOP = curr;
                curr = newNode;
            }
            else {
                 Add character
                QuadNode* newNode = new QuadNode(curr->X + 1, curr->Y, ch);
                newNode->LEFT = curr;
                curr->RIGHT = newNode;
                curr = newNode;
            }
        }
    }

    void findAndReplace()
    {
        string wordToFind;
        string replacementWord;
        cout << "Enter the word to find: ";
        cin >> wordToFind;
        cout << "Enter replacement word: ";
        cin >> replacementWord;

        QuadNode* temp = root;

        while (temp != NULL) {
            QuadNode* curr = temp;
            while (curr != NULL) {
                if (curr->data == wordToFind[0]) {
                    if (matchText(curr, wordToFind)) {
                        replaceText(curr, wordToFind.length(), replacementWord);
                    }
                }
                curr = curr->RIGHT;
            }
            temp = temp->BOTTOM;
        }
    }

    void replaceText(QuadNode* startNode, int length, const string& replacementWord) {
        QuadNode* curr = startNode;
        int count = 0;

        while (curr != NULL && count < length) {
            curr->data = replacementWord[count];
            curr = curr->RIGHT;
            count++;
        }
    }

    void saveToFile()
    {
        string filename = "output.txt";
        ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            cout << "Error opening the file." << endl;
            return;
        }

        QuadNode* temp = root;
        while (temp != NULL) {
            QuadNode* curr = temp;
            while (curr != NULL) {
                outputFile << curr->data;
                curr = curr->RIGHT;
            }
            outputFile << endl;
            temp = temp->BOTTOM;
        }

        outputFile.close();
        cout << "Notepad saved to " << filename << " successfully." << endl;
    }

    bool isAlphabetic(char ch)
    {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
    }

    int countTotalWords()
    {
        int wordCount = 0;
        bool insideWord = false;

        QuadNode* temp = root;
        while (temp != NULL) {
            QuadNode* curr = temp;
            while (curr != NULL) {
                if (isAlphabetic(curr->data)) {
                    if (!insideWord) {
                        insideWord = true;
                        wordCount++;
                    }
                }
                else {
                    insideWord = false;
                }
                curr = curr->RIGHT;
            }
            temp = temp->BOTTOM;
        }

        return wordCount;
    }



    void print() {
        QuadNode* temp = root;
        while (temp != NULL) {
            QuadNode* curr = temp;
            while (curr != NULL) {
                cout << curr->data;
                curr = curr->RIGHT;
            }
            cout << endl;
            temp = temp->BOTTOM;
        }
    }
};

int main()
{
    /*cout << "Welcome to NOTEPAD\n";
    Sleep(1000);
    system("cls");
    int choice;
    NotePad notepad;

    MENU:
    cout << "Choose the operation you want to perform: ";
    cout << "1. Add text\n";
    cout << "2. Delete text\n";
    cout << "3. Search a word\n";
    cout << "4. Copy text\n";
    cout << "5. Paste Text\n";
    cout << "6. Find and replace a word\n";
    cout << "7. Count total words\n";
    cout << "8. Save text to file\n";
    cout << "9. EXIT\n\n";
    cout << "Enter choice: ";
    cin >> choice;

    system("cls");

    switch (choice)
    {
    case 1:
        notepad.addText();
        break;
    case 2:
        notepad.deleteText();
        break;
    case 3:
        notepad.searchText();
        break;
    case 4:
        notepad.copyText();
        break;
    case 5:
        notepad.pasteText();
        break;
    case 6:
        notepad.findAndReplace();
        break;
    case 7:
        notepad.countTotalWords();
        break;
    case 8:
        notepad.saveToFile();
        break;
    case 9:
        goto EXIT;
    default:
        break;
    }
    Sleep(5000);
    system("cls");
    goto MENU;
EXIT:
    return 0;*/
}
