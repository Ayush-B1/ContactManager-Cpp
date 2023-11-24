#include <iostream>
#include <map>
#include<vector>
#include <string>

const int TABLE_SIZE = 100;

using namespace std;

// Define a Contact structure
struct Contact {
string name;
long long int phone_number;
string category;


Contact() {
   name = "";
   phone_number = 0;
   category = "";
}

Contact(const  string& n, long long int num, const string& temp) {
   name = n;
   phone_number = num;
   category = temp;
}

bool operator<(const Contact& other) const {
   return name < other.name;
}

bool operator==(const Contact& other) const {
   return phone_number == other.phone_number;
}
};

// Binary Search Tree for Sorting Contacts by Name
class ContactTree {
private:
  struct TreeNode {
      Contact contact;
      TreeNode* left;
      TreeNode* right;
      TreeNode(const Contact& c) : contact(c), left(nullptr), right(nullptr) {}
  };
  TreeNode* root;

public:
  ContactTree() : root(nullptr) {}

  void Insert(const Contact& contact) {
      root = InsertRec(root, contact);
  }

  void Delete(const Contact& contact) {
      root = DeleteRec(root, contact);
  }

  void InOrderTraversal(map<string, Contact>& result) {
      InOrderTraversalRec(root, result);
  }

  void SearchByCategory(const string& category, map<string, Contact>& result) {
      SearchByCategoryRec(root, category, result);
  }

private:
  TreeNode* InsertRec(TreeNode* node, const Contact& contact) {
      if (node == nullptr)
          return new TreeNode(contact);
      if (contact < node->contact)
          node->left = InsertRec(node->left, contact);
      else
          node->right = InsertRec(node->right, contact);
      return node;
  }
  TreeNode* DeleteRec(TreeNode* node, const Contact& contact) {
      if (node == nullptr)
          return node;
      if (contact < node->contact)
          node->left = DeleteRec(node->left, contact);
      else if (node->contact < contact)
          node->right = DeleteRec(node->right, contact);
      else {
          if (node->left == nullptr) {
              TreeNode* temp = node->right;
              delete node;
              return temp;
          } else if (node->right == nullptr) {
              TreeNode* temp = node->left;
              delete node;
              return temp;
          }
          TreeNode* temp = FindMin(node->right);
          node->contact = temp->contact;
          node->right = DeleteRec(node->right, temp->contact);
      }
      return node;
  }
  TreeNode* FindMin(TreeNode* node) {
      while (node->left != nullptr)
          node = node->left;
      return node;
  }
  void InOrderTraversalRec(TreeNode* node, map<string, Contact>& result) {
      if (node != nullptr) {
          InOrderTraversalRec(node->left, result);
          result[node->contact.name] = node->contact;
          InOrderTraversalRec(node->right, result);
      }
  }
  void SearchByCategoryRec(TreeNode* node, const string& category, map<string, Contact>& result) {
      if (node != nullptr) {
          SearchByCategoryRec(node->left, category, result);
          if (node->contact.category == category) {
              result[node->contact.name] = node->contact;
          }
          SearchByCategoryRec(node->right, category, result);
      }
  }
};

// Hash Table for Fast Phone Number Lookup
class PhoneNumberHashTable {
private:
  Contact data[TABLE_SIZE];
  // Simple hash function for demonstration purposes
  int hashFunction(long long int phoneNumber) {
      return phoneNumber % TABLE_SIZE;
  }

public:
  void Insert(const Contact& contact) {
      // Check for duplicate phone numbers
      int index = hashFunction(contact.phone_number);
      while (data[index].phone_number != 0) {
          if (data[index].phone_number == contact.phone_number) {
              cout << " Duplicate phone number. Warning!! Duplicate Phone number has been  added." << endl;
              return;
          }
          index = (index + 1) % TABLE_SIZE;  // Linear probing to handle collisions
      }
      data[index] = contact;
  }

  bool Find(long long int phoneNumber, Contact& result) {
      int index = hashFunction(phoneNumber);
      while (data[index].phone_number != 0) {
          if (data[index].phone_number == phoneNumber) {
              result = data[index];
              return true;
          }
          index = (index + 1) % TABLE_SIZE;  // Linear probing to handle collisions
      }
      return false;
  }

  void Remove(long long int phoneNumber) {
      int index = hashFunction(phoneNumber);
      while (data[index].phone_number != 0) {
          if (data[index].phone_number == phoneNumber) {
              data[index].phone_number = -1;  // Mark as deleted
              return;
          }
          index = (index + 1) % TABLE_SIZE;  // Linear probing to handle collisions
      }
  }
};

// Simple Graph for Managing Relationships
class RelationshipGraph {
private:
  map<Contact, map<Contact, bool> > relationships;
public:
  void AddRelationship(const Contact& contact1, const Contact& contact2) {
      relationships[contact1][contact2] = true;
      relationships[contact2][contact1] = true;
  }
  void RemoveRelationship(const Contact& contact1, const Contact& contact2) {
      relationships[contact1].erase(contact2);
      relationships[contact2].erase(contact1);
  }
  void DisplayRelationships(const Contact& contact) {
      cout << "Relationships for " << contact.name << ":" << endl;
      if (relationships.find(contact) != relationships.end()) {
          for (const auto& rel : relationships[contact]) {
              cout << "- " << rel.first.name << endl;
          }
      } else {
          cout << "No relationships found." << endl;
      }
  }
};
// Contact Book Application
class ContactBook {
private:
  ContactTree contactTree;
  PhoneNumberHashTable phoneNumberTable;
  RelationshipGraph relationshipGraph;
public:
void AddContact() {
  try {
      string name, category;
      long long int phoneNumber;
      // Validate Name
      while (true) {
          cin.ignore();
          cout << "Enter Name (type -1 to exit to menu): ";
          getline(cin, name);
          if (name == "-1") {
              cout << "Exiting to menu..." << endl;
              return;
          }
          bool validName = true;
          for (char ch : name) {
              if (!(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' && ch <= 'z')) {
                  validName = false;
                  break;
              }
          }
          if (!validName || name.empty()) {
              cerr << "Invalid input. Name must contain only alphabetic characters." << endl;
          } else {
              break;
          }
      }
      // Validate Phone Number
      while (true) {
          cout << "Enter Phone Number (type -1 to exit to menu): ";
          cin >> phoneNumber;
          if (phoneNumber == -1) {
              cout << "Exiting to menu..." << endl;
              return;
          }
          if (cin.fail() || phoneNumber <= 0) {
              cin.clear(); // clear input buffer to restore cin to a usable state
              while (cin.get() != '\n'); // discard invalid input
              cerr << "Invalid input. Please enter a positive number for the phone number." << endl;
          } else {
              break;
          }
      }
      // Validate Category
      while (true) {
          cout << "Enter Category (type -1 to exit to menu): ";
          cin.ignore(); // clear buffer
          getline(cin, category);
          if (category == "-1") {
              cout << "Exiting to menu..." << endl;
              return;
          }

          bool validCategory = true;
          for (char ch : category) {
              if (!(ch >= 'A' && ch <= 'Z') && !(ch >= 'a' and ch <= 'z')) {
                  validCategory = false;
                  break;
              }
          }

          if (!validCategory || category.empty()) {
              cerr << "Invalid input. Category must contain only alphabetic characters." << endl;
          } else {
              break;
          }
      }

      Contact newContact(name, phoneNumber, category);
      contactTree.Insert(newContact);
      phoneNumberTable.Insert(newContact);
      cout<<endl;
      cout << "Contact added successfully." << endl;
  } catch (const exception& e) {
      cerr << "Error: " << e.what() << endl;
  }
}

  void DeleteContact() {
      long long int phoneNumber;
      cout << "Enter Phone Number to Delete: ";
      cin >> phoneNumber;
      Contact contactToDelete;
      if (phoneNumberTable.Find(phoneNumber, contactToDelete)) {
          contactTree.Delete(contactToDelete);
          phoneNumberTable.Remove(phoneNumber);
          // Remove relationships involving this contact
          relationshipGraph.RemoveRelationship(contactToDelete, contactToDelete);
          cout << "Contact deleted successfully." << endl;
      } else {
          cout << "Contact not found." << endl;
      }
  }

  void SearchContact() {
      int choice;
      cout << "Search By:" << endl;
      cout << "1. Name" << endl;
      cout << "2. Phone Number" << endl;
      cout << "3. Category" << endl;
      cout << "Enter choice: ";
      cin >> choice;
      if (choice == 1) {
          string name;
          cout << "Enter Name to Search: ";
          cin >> name;
          map<string, Contact> searchResults;
          SearchByName(name, searchResults);
          DisplaySearchResults(searchResults);
      } else if (choice == 2) {
          long long int phoneNumber;
          cout << "Enter Phone Number to Search: ";
          cin >> phoneNumber;
          Contact searchResult;
          if (phoneNumberTable.Find(phoneNumber, searchResult)) {
              cout << "Search Result:" << endl;
              DisplayContact(searchResult);
          } else {
              cout << "Contact not found." << endl;
          }
      } else if (choice == 3) {
          string category;
          cout << "Enter Category to Search: ";
          cin >> category;
          map<string, Contact> searchResults;
          contactTree.SearchByCategory(category, searchResults);
          DisplaySearchResults(searchResults);
      } else {
          cout << "Invalid choice." << endl;
      }
  }

  void EditContact() {
      long long int phoneNumber;
      cout << "Enter Phone Number to Edit: ";
      cin >> phoneNumber;
      Contact contactToEdit;
      if (phoneNumberTable.Find(phoneNumber, contactToEdit)) {
          cout << "Editing Contact:" << endl;
          DisplayContact(contactToEdit);
          string newName;
          cout << "Enter New Name: ";
          cin >> newName;
          long long int newPhoneNumber;
          cout << "Enter New Phone Number: ";
          cin >> newPhoneNumber;
          string newCategory;
          cout << "Enter New Category: ";
          cin >> newCategory;
          Contact editedContact(newName, newPhoneNumber, newCategory);
          contactTree.Delete(contactToEdit);
          phoneNumberTable.Remove(phoneNumber);
          contactTree.Insert(editedContact);
          phoneNumberTable.Insert(editedContact);
          cout << "Contact edited successfully." << endl;
      } else {
          cout << "Contact not found." << endl;
      }
  }

  void DisplayAllContacts() {
      map<string, Contact> contacts;
      contactTree.InOrderTraversal(contacts);

      if (contacts.empty()) {
          cout << "No contacts found." << endl;
      } else {
          cout << "All Contacts:" << endl;
          for (const auto& entry : contacts) {
              DisplayContact(entry.second);
          }
      }
  }

  void DisplayContact(const Contact& contact) {
      cout << "Name: " << contact.name << endl;
      cout << "Phone Number: " << contact.phone_number << endl;
      cout << "Category: " << contact.category << endl;
      cout << endl;
  }

  void DisplaySearchResults(const map<string, Contact>& results) {
      if (results.empty()) {
          cout << "No contacts found." << endl;
      } else {
          cout << "Search Results:" << endl;
          for (const auto& entry : results) {
              DisplayContact(entry.second);
          }
      }
  }

  void SearchByName(const string& name, map<string, Contact>& results) {
      map<string, Contact> allContacts;
      contactTree.InOrderTraversal(allContacts);




      for (const auto& entry : allContacts) {
          if (entry.second.name.find(name) != string::npos) {
              results[entry.second.name] = entry.second;
          }
      }
  }

  void AddRelationship() {
      string name1, name2;
      cout << "Enter Name 1: ";
      cin >> name1;
      cout << "Enter Name 2: ";
      cin >> name2;
      Contact contact1, contact2;
      if (FindContactByName(name1, contact1) && FindContactByName(name2, contact2)) {
          relationshipGraph.AddRelationship(contact1, contact2);
          cout << "Relationship added successfully." << endl;
      } else {
          cout << "One or both contacts not found." << endl;
      }
  }

  void RemoveRelationship() {
      string name1, name2;
      cout << "Enter Name 1: ";
      cin >> name1;
      cout << "Enter Name 2: ";
      cin >> name2;
      Contact contact1, contact2;
      if (FindContactByName(name1, contact1) && FindContactByName(name2, contact2)) {
          relationshipGraph.RemoveRelationship(contact1, contact2);
          cout << "Relationship removed successfully." << endl;
      } else {
          cout << "One or both contacts not found." << endl;
      }
  }

  void DisplayRelationships() {
      string name;
      cout << "Enter Name to Display Relationships: ";
      cin >> name;
      Contact contact;
      if (FindContactByName(name, contact)) {
          relationshipGraph.DisplayRelationships(contact);
      } else {
          cout << "Contact not found." << endl;
      }
  }

  bool FindContactByName(const string& name, Contact& result) {
      map<string, Contact> all;
              map<string, Contact> allContacts;
      contactTree.InOrderTraversal(allContacts);

      for (const auto& entry : allContacts) {
          if (entry.second.name == name) {
              result = entry.second;
              return true;
          }
      }
      return false;
  }
};

// Main function
int main() {
   ContactBook contactBook;


   while (true) {
       cout<<endl; 
       cout << "Contact Book Menu:" << endl;
       cout<<endl;
       cout << "1. Add Contact" << endl;
       cout<<endl;
       cout << "2. Delete Contact" << endl;
       cout<<endl;
       cout << "3. Search Contact" << endl;
       cout<<endl;
       cout << "4. Edit Contact" << endl;
       cout<<endl;
       cout << "5. Display All Contacts" << endl;
       cout<<endl;
       cout << "6. Add Relationship" << endl;
       cout<<endl;
       cout << "7. Remove Relationship" << endl;
       cout<<endl;
       cout << "8. Display Relationships" << endl;
       cout<<endl;
       cout << "9. Exit" << endl;
       cout<<endl;
       cout << "Enter your choice: ";


       int choice;
       cin >> choice;

       cout<<endl; 

       if (cin.fail()) {
           cin.clear(); // Clear the error flag
           cin.ignore(); // Discard invalid input
           cerr << "Invalid input. Please enter a number." << endl;
           continue;
       }


  
       switch (choice) {
           case 1:
               contactBook.AddContact();
               break;
           case 2:
               contactBook.DeleteContact();
               break;
           case 3:
               contactBook.SearchContact();
               break;
           case 4:
               contactBook.EditContact();
               break;
           case 5:
               contactBook.DisplayAllContacts();
               break;
           case 6:
               contactBook.AddRelationship();
               break;
           case 7:
               contactBook.RemoveRelationship();
               break;
           case 8:
               contactBook.DisplayRelationships();
               break;
           case 9:
               exit(0);
           default:
               cout << "Invalid choice. Try again." << endl;
       }
   }
   return 0;
}