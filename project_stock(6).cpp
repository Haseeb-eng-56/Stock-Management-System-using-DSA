#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <map>
#include <limits>
#include <cctype>
#include <queue>
#include <stack>
#include<list>
using namespace std;

// ==================== DATA STRUCTURES ====================
struct Item {
    int id;
    string name;
    int quantity;
    double price;
    bool present;
    string addedDate;
    int salesCount;
    double totalSalesValue;
    string lastSoldDate;
    string expiryDate;
    string supplier;
    double discountPercentage;
    
    Item() : id(0), name(""), quantity(0), price(0.0), present(false), 
             addedDate(""), salesCount(0), totalSalesValue(0.0), lastSoldDate(""),
             expiryDate(""), supplier(""), discountPercentage(0.0) {}
};

struct Category {
    string name;
    vector<Item> slots;
    int itemCount;
    double categoryValue;
    double totalSales;
    
    Category() : itemCount(0), categoryValue(0.0), totalSales(0.0) {}
    Category(const string& n, const vector<string>& allowedNames) : 
             name(n), itemCount(0), categoryValue(0.0), totalSales(0.0) {
        slots.resize(allowedNames.size());
        for (int i = 0; i < (int)allowedNames.size(); ++i) {
            slots[i].id = i + 1;
            slots[i].name = allowedNames[i];
            slots[i].quantity = 0;
            slots[i].price = 0.0;
            slots[i].present = false;
            slots[i].addedDate = "";
            slots[i].salesCount = 0;
            slots[i].totalSalesValue = 0.0;
            slots[i].lastSoldDate = "";
            slots[i].expiryDate = "";
            slots[i].supplier = "";
            slots[i].discountPercentage = 0.0;
        }
    }
};

struct SaleRecord {
    string itemName;
    string category;
    int quantitySold;
    double totalPrice;
    string saleDate;
    string customerName;
    string paymentMethod;
    double discountAmount;
    string invoiceNumber;
};

struct Customer {
    string name;
    string phone;
    string email;
    string address;
    double totalSpent;
    int purchaseCount;
    string joinDate;
    string lastPurchaseDate;
    string customerType;
    double loyaltyPoints;
    double personalDiscount;
    
    Customer() : totalSpent(0.0), purchaseCount(0), loyaltyPoints(0.0), personalDiscount(0.0) {}
    Customer(string n, string p, string e, string a) : 
             name(n), phone(p), email(e), address(a), totalSpent(0.0), 
             purchaseCount(0), joinDate(""), lastPurchaseDate(""), 
             customerType("Regular"), loyaltyPoints(0.0), personalDiscount(0.0) {}
};

struct SystemConfig {
    string businessName;
    string businessAddress;
    string businessPhone;
    string taxNumber;
    double taxRate;
    int lowStockThreshold;
    double bulkDiscount1;
    int bulkQuantity1;
    double bulkDiscount2;
    int bulkQuantity2;
    double bulkDiscount3;
    int bulkQuantity3;
    string currency;
    bool enableSeasonalDiscount;
    double seasonalDiscountRate;
    string discountStartDate;
    string discountEndDate;
    
    SystemConfig() : taxRate(0.17), lowStockThreshold(5),
                     bulkDiscount1(0.05), bulkQuantity1(10),
                     bulkDiscount2(0.10), bulkQuantity2(20),
                     bulkDiscount3(0.15), bulkQuantity3(50),
                     currency("Rs."), enableSeasonalDiscount(false),
                     seasonalDiscountRate(0.10) {
        businessName = " Sheikh's Super Mart";
        businessAddress = "Shahbaz Khan Road, Kasur";
        businessPhone = "0313-4111530";
        taxNumber = "TRN-123456789";
        discountStartDate = "2024-01-01";
        discountEndDate = "2024-12-31";
    }
};

// ==================== ALLOWED ITEMS ====================
vector<string> allowedSoaps = {"Lux", "Palmolive", "Tibet", "Savannah", "Dettol", "Safeguard", "Lifebuoy", "Capri", "Pears", "Santoor"};
vector<string> allowedBiscuits = {"Oreo", "Prince", "Lemon", "Rio", "Sooper", "Peanut Pick", "Candi", "Party", "Cafe", "Nan Khatai"};
vector<string> allowedMeat = {"Beef", "Mutton", "Chicken", "Fish", "Lamb", "Crabs", "Duck", "Drumsticks", "Wings", "Nuggets"};
vector<string> allowedStationery = {"Pen", "Pencil", "Eraser", "Sharpener", "Notebook", "Register", "Marker", "Highlighter", "Glue Stick", "Stapler"};
vector<string> allowedElectronics = {"Mobile", "Charger", "Headphones", "Laptop", "Keyboard", "Mouse", "Power Bank", "SSD", "Router", "USB Cable"};
vector<string> allowedTeaCoffee = {"Green Tea", "Black Tea", "Olong Tea", "Cold Coffee", "Nescafe", "Espresso", "Tea Bags", "Herbal Tea", "Latte", "Ground Coffee"};
vector<string> allowedOils = {"Olive Oil", "Sunflower Oil", "Canola Oil", "Mustard Oil", "Coconut Oil", "Vegetable Oil", "Sesame Oil", "Palm Oil", "Groundnut Oil", "Soybean Oil"};
vector<string> allowedSnacks = {"Chips", "Nuts", "Popcorn", "Samosa", "Pakora", "Crackers", "Pretzels", "Energy Bar", "Mixture", "Cookies"};

// ==================== ADVANCED DATA STRUCTURES ====================

// BST Node for Top Selling Items
struct BSTNode {
    string itemName;
    string category;
    int salesCount;
    double totalRevenue;
    BSTNode* left;
    BSTNode* right;
    
    BSTNode(string name, string cat, int sales, double revenue) 
        : itemName(name), category(cat), salesCount(sales), totalRevenue(revenue), 
          left(nullptr), right(nullptr) {}
};

// BST for Top Selling Items
class SalesBST {
private:
    BSTNode* root;
    
    BSTNode* insertBySales(BSTNode* node, const string& name, const string& cat, int sales, double revenue) {
        if (!node) return new BSTNode(name, cat, sales, revenue);
        
        if (sales > node->salesCount) {
            node->left = insertBySales(node->left, name, cat, sales, revenue);
        } else {
            node->right = insertBySales(node->right, name, cat, sales, revenue);
        }
        return node;
    }
    
    BSTNode* insertByRevenue(BSTNode* node, const string& name, const string& cat, int sales, double revenue) {
        if (!node) return new BSTNode(name, cat, sales, revenue);
        
        if (revenue > node->totalRevenue) {
            node->left = insertByRevenue(node->left, name, cat, sales, revenue);
        } else {
            node->right = insertByRevenue(node->right, name, cat, sales, revenue);
        }
        return node;
    }
    
    void inOrderTraversal(BSTNode* node, vector<pair<string, pair<int, double>>>& result, int limit) {
        if (!node) return;
        
        inOrderTraversal(node->left, result, limit);
        if (result.size() < limit) {
            result.push_back({node->itemName + "|" + node->category, 
                            {node->salesCount, node->totalRevenue}});
        }
        inOrderTraversal(node->right, result, limit);
    }
    
    void clearTree(BSTNode* node) {
        if (!node) return;
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
    
public:
    SalesBST() : root(nullptr) {}
    
    ~SalesBST() {
        clearTree(root);
    }
    
    void buildTreeBySales(const vector<pair<string, pair<int, double>>>& items) {
        clearTree(root);
        root = nullptr;
        for (const auto& item : items) {
            size_t pos = item.first.find('|');
            string itemName = item.first.substr(0, pos);
            string catName = item.first.substr(pos + 1);
            root = insertBySales(root, itemName, catName, item.second.first, item.second.second);
        }
    }
    
    void buildTreeByRevenue(const vector<pair<string, pair<int, double>>>& items) {
        clearTree(root);
        root = nullptr;
        for (const auto& item : items) {
            size_t pos = item.first.find('|');
            string itemName = item.first.substr(0, pos);
            string catName = item.first.substr(pos + 1);
            root = insertByRevenue(root, itemName, catName, item.second.first, item.second.second);
        }
    }
    
    vector<pair<string, pair<int, double>>> getTopItems(int limit, bool byRevenue = false) {
        vector<pair<string, pair<int, double>>> result;
        inOrderTraversal(root, result, limit);
        return result;
    }
};

// Graph for Category Relationships
class CategoryGraph {
private:
    map<string, vector<string>> adjList;
    
public:
    void addCategory(const string& category) {
        if (adjList.find(category) == adjList.end()) {
            adjList[category] = vector<string>();
        }
    }
    
    void addRelationship(const string& cat1, const string& cat2) {
        adjList[cat1].push_back(cat2);
        adjList[cat2].push_back(cat1);
    }
    
    vector<string> getRelatedCategories(const string& category) {
        if (adjList.find(category) != adjList.end()) {
            return adjList[category];
        }
        return vector<string>();
    }
    
    void printGraph() {
        for (const auto& pair : adjList) {
            cout << "Category: " << pair.first << " -> Related to: ";
            for (const string& related : pair.second) {
                cout << related << " ";
            }
            cout << endl;
        }
    }
};

// Heap Node for Low Stock Items
struct HeapNode {
    string itemName;
    string category;
    int quantity;
    int threshold;
    
    HeapNode(string name, string cat, int qty, int thr) 
        : itemName(name), category(cat), quantity(qty), threshold(thr) {}
    
    bool operator<(const HeapNode& other) const {
        // Priority: items with lower quantity (more critical) have higher priority
        double ratio1 = (double)quantity / threshold;
        double ratio2 = (double)other.quantity / other.threshold;
        return ratio1 > ratio2; // Min-heap based on ratio
    }
};

// Priority Queue (Heap) for Low Stock Items
class LowStockPriorityQueue {
private:
    vector<HeapNode> heap;
    
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] < heap[parent]) {
                swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }
    
    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;
            
            if (left < size && heap[left] < heap[smallest]) {
                smallest = left;
            }
            if (right < size && heap[right] < heap[smallest]) {
                smallest = right;
            }
            if (smallest != index) {
                swap(heap[index], heap[smallest]);
                index = smallest;
            } else {
                break;
            }
        }
    }
    
public:
    void push(const HeapNode& node) {
        heap.push_back(node);
        heapifyUp(heap.size() - 1);
    }
    
    HeapNode pop() {
        if (heap.empty()) {
            return HeapNode("", "", 0, 0);
        }
        
        HeapNode top = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
        return top;
    }
    
    bool isEmpty() const {
        return heap.empty();
    }
    
    int size() const {
        return heap.size();
    }
    
    vector<HeapNode> getAllItems() {
        vector<HeapNode> result = heap;
        sort(result.begin(), result.end());
        return result;
    }
};

// Stack for Undo Operations
class UndoStack {
private:
    stack<pair<string, Item>> undoStack; // operation type and item state
    
public:
    void pushOperation(const string& operation, const Item& item) {
        undoStack.push({operation, item});
    }
    
    pair<string, Item> popOperation() {
        if (undoStack.empty()) {
            return {"", Item()};
        }
        pair<string, Item> top = undoStack.top();
        undoStack.pop();
        return top;
    }
    
    bool isEmpty() const {
        return undoStack.empty();
    }
    
    int size() const {
        return undoStack.size();
    }
};

// Queue for Sales Processing
class SalesQueue {
private:
    queue<SaleRecord> salesQueue;
    
public:
    void enqueue(const SaleRecord& record) {
        salesQueue.push(record);
    }
    
    SaleRecord dequeue() {
        if (salesQueue.empty()) {
            return SaleRecord();
        }
        SaleRecord front = salesQueue.front();
        salesQueue.pop();
        return front;
    }
    
    bool isEmpty() const {
        return salesQueue.empty();
    }
    
    int size() const {
        return salesQueue.size();
    }
};

// Linked List for Transaction History
struct TransactionNode {
    SaleRecord data;
    TransactionNode* next;
    TransactionNode* prev;
    
    TransactionNode(const SaleRecord& record) : data(record), next(nullptr), prev(nullptr) {}
};

class TransactionLinkedList {
private:
    TransactionNode* head;
    TransactionNode* tail;
    int size;
    
public:
    TransactionLinkedList() : head(nullptr), tail(nullptr), size(0) {}
    
    ~TransactionLinkedList() {
        TransactionNode* current = head;
        while (current) {
            TransactionNode* next = current->next;
            delete current;
            current = next;
        }
    }
    
    void addTransaction(const SaleRecord& record) {
        TransactionNode* newNode = new TransactionNode(record);
        
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }
    
    vector<SaleRecord> getRecentTransactions(int count) {
        vector<SaleRecord> result;
        TransactionNode* current = tail;
        int collected = 0;
        
        while (current && collected < count) {
            result.push_back(current->data);
            current = current->prev;
            collected++;
        }
        
        reverse(result.begin(), result.end());
        return result;
    }
    
    int getSize() const {
        return size;
    }
    
    void displayAll() {
        TransactionNode* current = head;
        int count = 0;
        while (current && count < 10) {
            cout << "Transaction " << ++count << ": " 
                 << current->data.itemName << " - " 
                 << current->data.totalPrice << endl;
            current = current->next;
        }
    }
};

struct InventorySystem {
    Category categories[8];
    vector<SaleRecord> sales;
    vector<Customer> customers;
    SystemConfig config;
    double totalRevenue;
    double totalExpenses;
    double totalProfit;
    int totalItems;
    int totalCustomers;
    
    // Advanced Data Structures
    SalesBST salesBST;
    CategoryGraph categoryGraph;
    LowStockPriorityQueue lowStockQueue;
    UndoStack undoStack;
    SalesQueue pendingSalesQueue;
    TransactionLinkedList transactionHistory;
    
    InventorySystem() : totalRevenue(0.0), totalExpenses(0.0), totalProfit(0.0),
                        totalItems(0), totalCustomers(0) {}
};

// ==================== UTILITY FUNCTIONS ====================
string toLower(const string& s) {
    string out = s;
    transform(out.begin(), out.end(), out.begin(), [](unsigned char c){ return tolower(c); });
    return out;
}

string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-" 
       << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
       << setw(2) << setfill('0') << ltm->tm_mday;
    return ss.str();
}

string getCurrentTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << setw(2) << setfill('0') << ltm->tm_hour << ":"
       << setw(2) << setfill('0') << ltm->tm_min << ":"
       << setw(2) << setfill('0') << ltm->tm_sec;
    return ss.str();
}

string generateInvoiceNumber() {
    static int counter = 1000;
    stringstream ss;
    ss << "INV-" << getCurrentDate() << "-" << counter++;
    return ss.str();
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printSeparator(int len = 60, char ch = '=') {
    cout << string(len, ch) << endl;
}

void printHeader(const string& title) {
    cout << "\n";
    printSeparator(60, '=');
    cout << "                 " << title << "\n";
    printSeparator(60, '=');
    cout << endl;
}

void printMenu(InventorySystem &sys) {
    printSeparator(70, '=');
    cout << "          " << sys.config.businessName << " - INVENTORY MANAGEMENT SYSTEM\n";
    printSeparator(70, '=');
    cout << "  1. Add New Item                 8. Sales Analytics\n";
    cout << "  2. Update Item                  9. Top Selling Items (using BST)\n";
    cout << "  3. Delete Item                  10. Customer Management\n";
    cout << "  4. Sell Item                    11. Advanced Discount Management\n";
    cout << "  5. Display Inventory            12. Business Intelligence\n";
    cout << "  6. Low Stock Items (using Heap) 13. System Settings\n";
    cout << "  7. Advanced Search              14. Backup & Restore\n";
    cout << " 15. View Category Graph          16. Undo Last Operation\n";
    cout << " 17. View Transaction History     18. Process Pending Sales\n";
    cout << "  0. Exit System\n";
    printSeparator(70, '=');
    cout << "\nEnter choice (0-18): ";
}

int findIndexByName(const Category& cat, const string& name) {
    string lname = toLower(name);
    for (int i = 0; i < (int)cat.slots.size(); ++i) {
        if (toLower(cat.slots[i].name) == lname) return i;
    }
    return -1;
}

void initInventory(InventorySystem &sys) {
    sys.categories[0] = Category("Soaps", allowedSoaps);
    sys.categories[1] = Category("Biscuits", allowedBiscuits);
    sys.categories[2] = Category("Meat", allowedMeat);
    sys.categories[3] = Category("Stationery", allowedStationery);
    sys.categories[4] = Category("Electronics", allowedElectronics);
    sys.categories[5] = Category("Tea & Coffee", allowedTeaCoffee);
    sys.categories[6] = Category("Oils", allowedOils);
    sys.categories[7] = Category("Snacks", allowedSnacks);
    
    // Initialize Category Graph relationships
    sys.categoryGraph.addCategory("Soaps");
    sys.categoryGraph.addCategory("Biscuits");
    sys.categoryGraph.addCategory("Meat");
    sys.categoryGraph.addCategory("Stationery");
    sys.categoryGraph.addCategory("Electronics");
    sys.categoryGraph.addCategory("Tea & Coffee");
    sys.categoryGraph.addCategory("Oils");
    sys.categoryGraph.addCategory("Snacks");
    
    // Add relationships based on business logic
    sys.categoryGraph.addRelationship("Biscuits", "Snacks");
    sys.categoryGraph.addRelationship("Tea & Coffee", "Biscuits");
    sys.categoryGraph.addRelationship("Soaps", "Oils");
    sys.categoryGraph.addRelationship("Stationery", "Electronics");
}

// ==================== INVENTORY FUNCTIONS ====================
void addItem(InventorySystem &sys) {
    printHeader("ADD NEW ITEM");
    
    cout << "Select Category:\n";
    for (int i = 0; i < 8; ++i) {
        cout << "  " << i+1 << ". " << sys.categories[i].name 
             << " (" << sys.categories[i].itemCount << " items)\n";
    }
    
    cout << "\nEnter category number (1-8): ";
    int catNum;
    cin >> catNum;
    
    if (catNum < 1 || catNum > 8) {
        cout << "ERROR: Invalid category!\n";
        clearInputBuffer();
        return;
    }
    
    Category &cat = sys.categories[catNum-1];
    clearInputBuffer();
    
    cout << "\nAvailable items in " << cat.name << ":\n";
    for (int i = 0; i < 10; ++i) {
        string status = cat.slots[i].present ? "Added" : "Available";
        cout << "  " << i+1 << ". " << cat.slots[i].name << " - " << status << "\n";
    }
    
    // ✅ **MAP کا استعمال: Suggested Items based on Category Graph**
    cout << "\n **SUGGESTED ITEMS (Using Category Graph):**\n";
    map<string, int> suggestedItems;  // ✅ MAP استعمال ہوا
    
    // Get related categories
    vector<string> relatedCats = sys.categoryGraph.getRelatedCategories(cat.name);
    for (const string& relatedCat : relatedCats) {
        for (int i = 0; i < 8; ++i) {
            if (sys.categories[i].name == relatedCat) {
                for (const Item& item : sys.categories[i].slots) {
                    if (item.present && item.salesCount > 5) { // Popular items
                        suggestedItems[item.name + " (" + relatedCat + ")"] = item.salesCount;
                    }
                }
            }
        }
    }
    
    if (!suggestedItems.empty()) {
        cout << "Popular items in related categories:\n";
        int count = 0;
        for (const auto& pair : suggestedItems) {
            if (count < 3) {
                cout << "  • " << pair.first << " (Sold: " << pair.second << " times)\n";
                count++;
            }
        }
        cout << endl;
    }
    
    cout << "\nEnter item name: ";
    string itemName;
    getline(cin, itemName);
    
    int idx = findIndexByName(cat, itemName);
    if (idx == -1) {
        cout << "ERROR: Item not allowed in this category!\n";
        return;
    }
    
    if (cat.slots[idx].present) {
        cout << "WARNING: Item already exists! Use update option.\n";
        return;
    }
    
    cout << "Enter quantity: ";
    int qty;
    while (!(cin >> qty) || qty < 0) {
        cout << "ERROR: Invalid quantity! Enter again: ";
        cin.clear();
        clearInputBuffer();
    }
    
    cout << "Enter price per unit: " << sys.config.currency;
    double price;
    while (!(cin >> price) || price < 0) {
        cout << "ERROR: Invalid price! Enter again: " << sys.config.currency;
        cin.clear();
        clearInputBuffer();
    }
    
    clearInputBuffer();
    cout << "Enter expiry date (YYYY-MM-DD) or press Enter to skip: ";
    string expiry;
    getline(cin, expiry);
    
    cout << "Enter supplier name or press Enter to skip: ";
    string supplier;
    getline(cin, supplier);
    
    cout << "Enter item discount percentage (0-100): ";
    double discount;
    cin >> discount;
    clearInputBuffer();
    
    // Save current state for undo
    sys.undoStack.pushOperation("ADD", cat.slots[idx]);
    
    cat.slots[idx].quantity = qty;
    cat.slots[idx].price = price;
    cat.slots[idx].present = true;
    cat.slots[idx].addedDate = getCurrentDate();
    cat.slots[idx].expiryDate = expiry;
    cat.slots[idx].supplier = supplier;
    cat.slots[idx].discountPercentage = (discount / 100);
    cat.itemCount++;
    cat.categoryValue += qty * price;
    sys.totalItems++;
    
    cout << "\nSUCCESS: Item added successfully!\n";
    cout << "  Name: " << cat.slots[idx].name << "\n";
    cout << "  Quantity: " << qty << "\n";
    cout << "  Price: " << sys.config.currency << price << "\n";
    cout << "  Discount: " << discount << "%\n";
    cout << "  Total Value: " << sys.config.currency << (qty * price) << "\n";
    cout << "  Added Date: " << cat.slots[idx].addedDate << "\n";
    if (!expiry.empty()) cout << "  Expiry Date: " << expiry << "\n";
    if (!supplier.empty()) cout << "  Supplier: " << supplier << "\n";
}

void updateItem(InventorySystem &sys) {
    printHeader("ADVANCED UPDATE ITEM");
    
    // ✅ **MAP کا استعمال: Category Statistics**
    map<string, pair<int, double>> categoryStats;  // ✅ MAP استعمال ہوا
    for (int i = 0; i < 8; ++i) {
        categoryStats[sys.categories[i].name] = {
            sys.categories[i].itemCount, 
            sys.categories[i].categoryValue
        };
    }
    
    cout << "Select category:\n";
    for (int i = 0; i < 8; ++i) {
        if (sys.categories[i].itemCount > 0) {
            cout << "  " << i+1 << ". " << sys.categories[i].name 
                 << " [" << sys.categories[i].itemCount << " items, "
                 << sys.config.currency << sys.categories[i].categoryValue << "]\n";
        }
    }
    
    cout << "\nEnter category number: ";
    int catNum;
    cin >> catNum;
    
    if (catNum < 1 || catNum > 8 || sys.categories[catNum-1].itemCount == 0) {
        cout << "ERROR: Invalid category or no items!\n";
        clearInputBuffer();
        return;
    }
    
    Category &cat = sys.categories[catNum-1];
    clearInputBuffer();
    
    cout << "Enter item name to update: ";
    string itemName;
    getline(cin, itemName);
    
    int idx = findIndexByName(cat, itemName);
    if (idx == -1 || !cat.slots[idx].present) {
        cout << "ERROR: Item not found!\n";
        return;
    }
    
    Item &item = cat.slots[idx];
    
    // ✅ **BST کا استعمال: Compare with Top Selling Items**
    cout << "\n **COMPARISON WITH TOP SELLERS (Using BST):**\n";
    
    vector<pair<string, pair<int, double>>> itemSales;
    for (int i = 0; i < 8; ++i) {
        const Category &category = sys.categories[i];
        for (const Item &it : category.slots) {
            if (it.salesCount > 0) {
                itemSales.push_back({it.name + "|" + category.name, 
                                   {it.salesCount, it.totalSalesValue}});
            }
        }
    }
    
    if (!itemSales.empty()) {
        sys.salesBST.buildTreeBySales(itemSales);
        auto topItems = sys.salesBST.getTopItems(5);
        
        bool foundInTop = false;
        for (const auto& topItem : topItems) {
            size_t pos = topItem.first.find('|');
            string topItemName = topItem.first.substr(0, pos);
            if (topItemName == item.name) {
                foundInTop = true;
                cout << " This item is in TOP " << topItems.size() << " best sellers!\n";
                cout << "   Sales Rank: " << (&topItem - &topItems[0] + 1) << "\n";
                cout << "   Total Sales: " << topItem.second.first << " units\n";
                cout << "   Total Revenue: " << sys.config.currency << topItem.second.second << "\n";
                break;
            }
        }
        
        if (!foundInTop && item.salesCount > 0) {
            cout << " This item has sold " << item.salesCount << " units\n";
            cout << " Total Revenue: " << sys.config.currency << item.totalSalesValue << "\n";
            
            // Find rank
            int rank = 1;
            for (const auto& topItem : topItems) {
                if (item.salesCount > topItem.second.first) break;
                rank++;
            }
            cout << " Rank compared to top sellers: " << rank << "\n";
        }
        cout << endl;
    }
    
    // Save current state for undo
    sys.undoStack.pushOperation("UPDATE", item);
    
    cout << "\nCurrent details of '" << item.name << "':\n";
    cout << "  1. Quantity: " << item.quantity << "\n";
    cout << "  2. Price: " << sys.config.currency << item.price << "\n";
    cout << "  3. Item Discount: " << (item.discountPercentage * 100) << "%\n";
    cout << "  4. Expiry Date: " << (item.expiryDate.empty() ? "Not set" : item.expiryDate) << "\n";
    cout << "  5. Supplier: " << (item.supplier.empty() ? "Not set" : item.supplier) << "\n";
    cout << "  6. Sales Count: " << item.salesCount << "\n";
    cout << "  7. Update All Fields\n";
    
    cout << "\nSelect field to update (1-7): ";
    int fieldChoice;
    cin >> fieldChoice;
    
    double oldValue = item.quantity * item.price;
    
    switch(fieldChoice) {
        case 1:
            cout << "Enter new quantity: ";
            cin >> item.quantity;
            break;
        case 2:
            cout << "Enter new price: " << sys.config.currency;
            cin >> item.price;
            break;
        case 3:
            cout << "Enter item discount percentage (0-100): ";
            cin >> item.discountPercentage;
            item.discountPercentage /= 100;
            cout << "Item discount set to " << (item.discountPercentage * 100) << "%\n";
            break;
        case 4:
            clearInputBuffer();
            cout << "Enter expiry date (YYYY-MM-DD): ";
            getline(cin, item.expiryDate);
            break;
        case 5:
            clearInputBuffer();
            cout << "Enter supplier name: ";
            getline(cin, item.supplier);
            break;
        case 6:
            cout << "Enter new sales count: ";
            cin >> item.salesCount;
            break;
        case 7:
            cout << "\n--- Update All Fields ---\n";
            cout << "Enter new quantity: ";
            cin >> item.quantity;
            cout << "Enter new price: " << sys.config.currency;
            cin >> item.price;
            cout << "Enter item discount percentage (0-100): ";
            double disc;
            cin >> disc;
            item.discountPercentage = disc / 100;
            clearInputBuffer();
            cout << "Enter expiry date (YYYY-MM-DD): ";
            getline(cin, item.expiryDate);
            cout << "Enter supplier name: ";
            getline(cin, item.supplier);
            break;
        default:
            cout << "Invalid choice!\n";
            clearInputBuffer();
            return;
    }
    
    // Update category value
    double newValue = item.quantity * item.price;
    cat.categoryValue += (newValue - oldValue);
    
    // Check for expiry
    if (!item.expiryDate.empty() && item.expiryDate < getCurrentDate()) {
        cout << "\nWARNING: Item has expired! (" << item.expiryDate << ")\n";
        cout << "Do you want to remove expired item? (y/n): ";
        char choice;
        cin >> choice;
        if (tolower(choice) == 'y') {
            item.present = false;
            item.quantity = 0;
            cat.itemCount--;
            sys.totalItems--;
            cout << "Expired item removed from inventory.\n";
        }
    }
    
    cout << "\nSUCCESS: Item updated successfully!\n";
    cout << "  New Value: " << sys.config.currency << newValue << "\n";
    clearInputBuffer();
}

void deleteItem(InventorySystem &sys) {
    printHeader("DELETE ITEM");
    
    cout << "Select category:\n";
    for (int i = 0; i < 8; ++i) {
        if (sys.categories[i].itemCount > 0) {
            cout << "  " << i+1 << ". " << sys.categories[i].name << "\n";
        }
    }
    
    cout << "\nEnter category number: ";
    int catNum;
    cin >> catNum;
    
    if (catNum < 1 || catNum > 8) {
        cout << "ERROR: Invalid category!\n";
        clearInputBuffer();
        return;
    }
    
    Category &cat = sys.categories[catNum-1];
    clearInputBuffer();
    
    cout << "Enter item name to delete: ";
    string itemName;
    getline(cin, itemName);
    
    int idx = findIndexByName(cat, itemName);
    if (idx == -1 || !cat.slots[idx].present) {
        cout << "ERROR: Item not found!\n";
        return;
    }
    
    Item &item = cat.slots[idx];
    
    // Save current state for undo
    sys.undoStack.pushOperation("DELETE", item);
    
    cout << "\nWARNING: Deleting '" << item.name << "'\n";
    cout << "  Quantity: " << item.quantity << " units\n";
    cout << "  Value: " << sys.config.currency << (item.quantity * item.price) << "\n";
    if (!item.expiryDate.empty()) cout << "  Expiry Date: " << item.expiryDate << "\n";
    if (!item.supplier.empty()) cout << "  Supplier: " << item.supplier << "\n";
    
    cout << "\nAre you sure? (y/n): ";
    char confirm;
    cin >> confirm;
    clearInputBuffer();
    
    if (tolower(confirm) == 'y') {
        cat.categoryValue -= (item.quantity * item.price);
        item.present = false;
        item.quantity = 0;
        item.price = 0.0;
        item.addedDate = "";
        item.expiryDate = "";
        item.supplier = "";
        item.discountPercentage = 0.0;
        item.salesCount = 0;
        item.totalSalesValue = 0.0;
        cat.itemCount--;
        sys.totalItems--;
        cout << "SUCCESS: Item deleted successfully!\n";
    } else {
        cout << "Deletion cancelled.\n";
        sys.undoStack.popOperation(); // Remove the undo record
    }
}

void sellItem(InventorySystem &sys) {
    printHeader("SELL ITEM");
    
    cout << "Select category:\n";
    for (int i = 0; i < 8; ++i) {
        cout << "  " << i+1 << ". " << sys.categories[i].name << "\n";
    }
    
    cout << "\nEnter category number: ";
    int catNum;
    cin >> catNum;
    
    if (catNum < 1 || catNum > 8) {
        cout << "ERROR: Invalid category!\n";
        clearInputBuffer();
        return;
    }
    
    Category &cat = sys.categories[catNum-1];
    clearInputBuffer();
    
    cout << "Enter item name: ";
    string itemName;
    getline(cin, itemName);
    
    int idx = findIndexByName(cat, itemName);
    if (idx == -1 || !cat.slots[idx].present) {
        cout << "ERROR: Item not available!\n";
        return;
    }
    
    Item &item = cat.slots[idx];
    
    // Check expiry date
    if (!item.expiryDate.empty() && item.expiryDate < getCurrentDate()) {
        cout << "WARNING: This item has expired on " << item.expiryDate << "!\n";
        cout << "Do you still want to sell it? (y/n): ";
        char choice;
        cin >> choice;
        clearInputBuffer();
        if (tolower(choice) != 'y') {
            cout << "Sale cancelled.\n";
            return;
        }
    }
    
    // ✅ **GRAPH کا استعمال: Cross-selling Suggestions**
    cout << "\n **CROSS-SELLING SUGGESTIONS (Using Category Graph):**\n";
    vector<string> relatedCategories = sys.categoryGraph.getRelatedCategories(cat.name);
    
    if (!relatedCategories.empty()) {
        cout << "Customers who buy " << cat.name << " also buy from:\n";
        for (const string& relatedCat : relatedCategories) {
            // Find the category
            for (int i = 0; i < 8; ++i) {
                if (sys.categories[i].name == relatedCat && sys.categories[i].itemCount > 0) {
                    cout << "  • " << relatedCat << ": ";
                    // Show top 2 items from related category
                    int count = 0;
                    for (const Item& relatedItem : sys.categories[i].slots) {
                        if (relatedItem.present && relatedItem.quantity > 0 && count < 2) {
                            cout << relatedItem.name << " (" << sys.config.currency 
                                 << relatedItem.price << ")";
                            if (count == 0) cout << ", ";
                            count++;
                        }
                    }
                    cout << endl;
                    break;
                }
            }
        }
        cout << endl;
    }
    
    cout << "\nItem Details:\n";
    cout << "  Name: " << item.name << "\n";
    cout << "  Available: " << item.quantity << " units\n";
    cout << "  Price: " << sys.config.currency << item.price << " per unit\n";
    if (item.discountPercentage > 0) {
        cout << "  Item Discount: " << (item.discountPercentage*100) << "%\n";
    }
    if (!item.expiryDate.empty()) {
        cout << "  Expiry Date: " << item.expiryDate << "\n";
    }
    
    cout << "\nEnter customer name (or 'Walk-in'): ";
    string customerName;
    getline(cin, customerName);
    
    // Find customer if exists
    Customer* currentCustomer = nullptr;
    for (auto &cust : sys.customers) {
        if (toLower(cust.name) == toLower(customerName)) {
            currentCustomer = &cust;
            break;
        }
    }
    
    if (currentCustomer) {
        cout << "Welcome back, " << currentCustomer->name << "!\n";
        cout << "Customer Type: " << currentCustomer->customerType << "\n";
        if (currentCustomer->personalDiscount > 0) {
            cout << "Personal Discount: " << (currentCustomer->personalDiscount*100) << "%\n";
        }
    }
    
    cout << "Enter quantity to sell: ";
    int sellQty;
    while (!(cin >> sellQty) || sellQty <= 0) {
        cout << "ERROR: Invalid quantity! Enter again: ";
        cin.clear();
        clearInputBuffer();
    }
    
    if (sellQty > item.quantity) {
        cout << "ERROR: Not enough stock available!\n";
        clearInputBuffer();
        return;
    }
    
    // Calculate base total
    double total = sellQty * item.price;
    double totalDiscount = 0.0;
    string discountDetails = "";
    
    // Apply item-specific discount
    if (item.discountPercentage > 0) {
        double itemDiscount = total * item.discountPercentage;
        total -= itemDiscount;
        totalDiscount += itemDiscount;
        discountDetails += "Item Discount (" + to_string(item.discountPercentage*100) + "%): -" + 
                          sys.config.currency + to_string(itemDiscount) + "\n";
    }
    
    // Apply bulk discount
    double bulkDiscount = 0.0;
    if (sellQty >= sys.config.bulkQuantity3) {
        bulkDiscount = total * sys.config.bulkDiscount3;
        discountDetails += "Bulk Discount (" + to_string(sys.config.bulkDiscount3*100) + "%): -" + 
                          sys.config.currency + to_string(bulkDiscount) + "\n";
    } else if (sellQty >= sys.config.bulkQuantity2) {
        bulkDiscount = total * sys.config.bulkDiscount2;
        discountDetails += "Bulk Discount (" + to_string(sys.config.bulkDiscount2*100) + "%): -" + 
                          sys.config.currency + to_string(bulkDiscount) + "\n";
    } else if (sellQty >= sys.config.bulkQuantity1) {
        bulkDiscount = total * sys.config.bulkDiscount1;
        discountDetails += "Bulk Discount (" + to_string(sys.config.bulkDiscount1*100) + "%): -" + 
                          sys.config.currency + to_string(bulkDiscount) + "\n";
    }
    
    total -= bulkDiscount;
    totalDiscount += bulkDiscount;
    
    // Apply seasonal discount
    if (sys.config.enableSeasonalDiscount && 
        getCurrentDate() >= sys.config.discountStartDate && 
        getCurrentDate() <= sys.config.discountEndDate) {
        double seasonalDiscount = total * sys.config.seasonalDiscountRate;
        total -= seasonalDiscount;
        totalDiscount += seasonalDiscount;
        discountDetails += "Seasonal Discount (" + to_string(sys.config.seasonalDiscountRate*100) + "%): -" + 
                          sys.config.currency + to_string(seasonalDiscount) + "\n";
    }
    
    // Apply customer personal discount
    double customerDiscount = 0.0;
    if (currentCustomer && currentCustomer->personalDiscount > 0) {
        customerDiscount = total * currentCustomer->personalDiscount;
        total -= customerDiscount;
        totalDiscount += customerDiscount;
        discountDetails += "Personal Discount (" + to_string(currentCustomer->personalDiscount*100) + "%): -" + 
                          sys.config.currency + to_string(customerDiscount) + "\n";
    }
    
    // Apply loyalty discount based on customer type
    double loyaltyDiscount = 0.0;
    if (currentCustomer) {
        if (currentCustomer->customerType == "Platinum") {
            loyaltyDiscount = total * 0.15;
            discountDetails += "Platinum Discount (15%): -" + sys.config.currency + to_string(loyaltyDiscount) + "\n";
        } else if (currentCustomer->customerType == "Gold") {
            loyaltyDiscount = total * 0.10;
            discountDetails += "Gold Discount (10%): -" + sys.config.currency + to_string(loyaltyDiscount) + "\n";
        } else if (currentCustomer->customerType == "Silver") {
            loyaltyDiscount = total * 0.05;
            discountDetails += "Silver Discount (5%): -" + sys.config.currency + to_string(loyaltyDiscount) + "\n";
        }
        total -= loyaltyDiscount;
        totalDiscount += loyaltyDiscount;
    }
    
    double tax = total * sys.config.taxRate;
    double finalTotal = total + tax;
    
    // Create sale record
    SaleRecord record;
    record.itemName = item.name;
    record.category = cat.name;
    record.quantitySold = sellQty;
    record.totalPrice = finalTotal;
    record.saleDate = getCurrentDate() + " " + getCurrentTime();
    record.customerName = customerName;
    record.paymentMethod = "Cash";
    record.discountAmount = totalDiscount;
    record.invoiceNumber = generateInvoiceNumber();
    
    // Add to sales queue for processing
    sys.pendingSalesQueue.enqueue(record);
    
    // Add to transaction history (Linked List)
    sys.transactionHistory.addTransaction(record);
    
    // Update inventory
    item.quantity -= sellQty;
    item.salesCount += sellQty;
    item.totalSalesValue += (sellQty * item.price);
    item.lastSoldDate = getCurrentDate() + " " + getCurrentTime();
    cat.categoryValue -= (sellQty * item.price);
    cat.totalSales += (sellQty * item.price);
    
    // Update system totals
    sys.sales.push_back(record);
    sys.totalRevenue += finalTotal;
    sys.totalProfit = sys.totalRevenue - sys.totalExpenses;
    
    // Update or create customer
    if (customerName != "Walk-in") {
        if (!currentCustomer) {
            cout << "Enter customer phone: ";
            string phone;
            clearInputBuffer();
            getline(cin, phone);
            
            Customer newCust(customerName, phone, "", "");
            newCust.totalSpent = finalTotal;
            newCust.purchaseCount = 1;
            newCust.joinDate = getCurrentDate();
            newCust.lastPurchaseDate = getCurrentDate();
            newCust.loyaltyPoints = finalTotal / 100;
            sys.customers.push_back(newCust);
            sys.totalCustomers++;
        } else {
            currentCustomer->totalSpent += finalTotal;
            currentCustomer->purchaseCount++;
            currentCustomer->lastPurchaseDate = getCurrentDate();
            currentCustomer->loyaltyPoints += finalTotal / 100;
            
            // Upgrade customer type
            if (currentCustomer->totalSpent > 50000) currentCustomer->customerType = "Platinum";
            else if (currentCustomer->totalSpent > 20000) currentCustomer->customerType = "Gold";
            else if (currentCustomer->totalSpent > 5000) currentCustomer->customerType = "Silver";
        }
    }
    
    if (item.quantity == 0) {
        item.present = false;
        cat.itemCount--;
        sys.totalItems--;
    }
    
    // Print enhanced receipt
    cout << "\n";
    printSeparator(50, '=');
    cout << "              " << sys.config.businessName << " - SALES RECEIPT\n";
    printSeparator(50, '=');
    cout << "Invoice #: " << record.invoiceNumber << "\n";
    cout << left << setw(20) << "Item:" << item.name << "\n";
    cout << left << setw(20) << "Category:" << cat.name << "\n";
    cout << left << setw(20) << "Quantity:" << sellQty << " units\n";
    cout << left << setw(20) << "Unit Price:" << sys.config.currency << item.price << "\n";
    cout << left << setw(20) << "Subtotal:" << sys.config.currency << (sellQty * item.price) << "\n";
    
    if (totalDiscount > 0) {
        cout << "\nDISCOUNTS APPLIED:\n";
        cout << discountDetails;
        cout << left << setw(20) << "Total Discount:" << "-" << sys.config.currency << totalDiscount << "\n";
    }
    
    cout << left << setw(20) << "Tax (" << (sys.config.taxRate*100) << "%):" << sys.config.currency << tax << "\n";
    printSeparator(50, '-');
    cout << left << setw(20) << "TOTAL:" << sys.config.currency << finalTotal << "\n";
    printSeparator(50, '=');
    cout << left << setw(20) << "Customer:" << customerName << "\n";
    if (currentCustomer) {
        cout << left << setw(20) << "Customer Type:" << currentCustomer->customerType << "\n";
        cout << left << setw(20) << "Loyalty Points:" << currentCustomer->loyaltyPoints << "\n";
    }
    cout << left << setw(20) << "Date/Time:" << record.saleDate << "\n";
    cout << left << setw(20) << "Business:" << sys.config.businessName << "\n";
    cout << left << setw(20) << "Address:" << sys.config.businessAddress << "\n";
    cout << left << setw(20) << "Phone:" << sys.config.businessPhone << "\n";
    printSeparator(50, '=');
    
    cout << "\nSUCCESS: Sale completed successfully!\n";
    clearInputBuffer();
}

void displayInventory(InventorySystem &sys) {
    printHeader("INVENTORY DISPLAY");
    
    int choice;
    cout << "1. View All Items\n";
    cout << "2. View by Category\n";
    cout << "3. View Statistics\n";
    cout << "4. View Sales History\n";
    cout << "5. View Expensive Items (Price > 1000)\n";
    cout << "6. View Items with Discounts\n";
    cout << "7. View Items Near Expiry\n";
    cout << "Enter choice: ";
    cin >> choice;
    clearInputBuffer();
    
    if (choice == 1) {
        double totalValue = 0;
        int totalCount = 0;
        
        cout << "\n" << sys.config.businessName << " - Inventory Report\n";
        printSeparator(80, '=');
        
        for (int i = 0; i < 8; ++i) {
            const Category &cat = sys.categories[i];
            if (cat.itemCount > 0) {
                cout << "\n[" << cat.name << "] (" << cat.itemCount << " items)\n";
                printSeparator(80, '-');
                cout << left << setw(20) << "Item" << setw(10) << "Qty" 
                     << setw(15) << "Price" << setw(15) << "Discount" 
                     << setw(15) << "Value" << setw(10) << "Status\n";
                printSeparator(80, '-');
                
                for (const Item &item : cat.slots) {
                    if (item.present) {
                        double value = item.quantity * item.price;
                        totalValue += value;
                        totalCount++;
                        
                        string status;
                        if (item.quantity == 0) status = "OUT OF STOCK";
                        else if (item.quantity < 3) status = "CRITICAL";
                        else if (item.quantity < 5) status = "LOW";
                        else if (item.quantity < 10) status = "OK";
                        else status = "GOOD";
                        
                        string discountStr = (item.discountPercentage > 0) ? 
                                           to_string(item.discountPercentage*100) + "%" : "0%";
                        
                        cout << left << setw(20) << item.name 
                             << setw(10) << item.quantity 
                             << setw(15) << sys.config.currency + to_string(item.price)
                             << setw(15) << discountStr
                             << setw(15) << sys.config.currency + to_string(value)
                             << setw(10) << status << "\n";
                    }
                }
            }
        }
        
        if (totalCount == 0) {
            cout << "WARNING: Inventory is empty! Add items first.\n";
        } else {
            cout << "\nSUMMARY:\n";
            cout << "Total Items: " << totalCount << "\n";
            cout << "Total Value: " << sys.config.currency << totalValue << "\n";
            cout << "Total Revenue: " << sys.config.currency << sys.totalRevenue << "\n";
            cout << "Total Profit: " << sys.config.currency << sys.totalProfit << "\n";
        }
        
    } else if (choice == 2) {
        cout << "\nSelect category:\n";
        for (int i = 0; i < 8; ++i) {
            cout << i+1 << ". " << sys.categories[i].name << "\n";
        }
        
        int catNum;
        cout << "Enter category number: ";
        cin >> catNum;
        clearInputBuffer();
        
        if (catNum >= 1 && catNum <= 8) {
            const Category &cat = sys.categories[catNum-1];
            if (cat.itemCount > 0) {
                cout << "\n" << sys.config.businessName << " - " << cat.name << " Items\n";
                cout << left << setw(20) << "Item" << setw(15) << "Quantity" 
                     << setw(15) << "Price" << setw(15) << "Discount" << setw(15) << "Sales" 
                     << setw(15) << "Expiry Date\n";
                printSeparator(95, '-');
                for (const Item &item : cat.slots) {
                    if (item.present) {
                        string discountStr = (item.discountPercentage > 0) ? 
                                           to_string(item.discountPercentage*100) + "%" : "0%";
                        string expiry = item.expiryDate.empty() ? "N/A" : item.expiryDate;
                        
                        cout << left << setw(20) << item.name 
                             << setw(15) << item.quantity 
                             << setw(15) << sys.config.currency + to_string(item.price)
                             << setw(15) << discountStr
                             << setw(15) << item.salesCount
                             << setw(15) << expiry << "\n";
                    }
                }
            } else {
                cout << "ERROR: No items in this category!\n";
            }
        }
    } else if (choice == 3) {
        double totalValue = 0;
        int totalCount = 0;
        int lowStock = 0;
        int outOfStock = 0;
        int discountedItems = 0;
        int expiredItems = 0;
        
        string currentDate = getCurrentDate();
        for (int i = 0; i < 8; ++i) {
            totalValue += sys.categories[i].categoryValue;
            totalCount += sys.categories[i].itemCount;
            
            for (const Item &item : sys.categories[i].slots) {
                if (item.present) {
                    if (item.quantity == 0) outOfStock++;
                    else if (item.quantity < sys.config.lowStockThreshold) lowStock++;
                    
                    if (item.discountPercentage > 0) discountedItems++;
                    
                    if (!item.expiryDate.empty() && item.expiryDate < currentDate) {
                        expiredItems++;
                    }
                }
            }
        }
        
        cout << "\n" << sys.config.businessName << " - INVENTORY STATISTICS:\n";
        printSeparator(50, '-');
        cout << left << setw(30) << "Total Items:" << totalCount << "\n";
        cout << left << setw(30) << "Inventory Value:" << sys.config.currency << totalValue << "\n";
        cout << left << setw(30) << "Total Revenue:" << sys.config.currency << sys.totalRevenue << "\n";
        cout << left << setw(30) << "Total Profit:" << sys.config.currency << sys.totalProfit << "\n";
        cout << left << setw(30) << "Low Stock Items:" << lowStock << "\n";
        cout << left << setw(30) << "Out of Stock Items:" << outOfStock << "\n";
        cout << left << setw(30) << "Discounted Items:" << discountedItems << "\n";
        cout << left << setw(30) << "Expired Items:" << expiredItems << "\n";
        cout << left << setw(30) << "Total Customers:" << sys.customers.size() << "\n";
        cout << left << setw(30) << "Total Sales:" << sys.sales.size() << "\n";
        
    } else if (choice == 4) {
        if (sys.sales.empty()) {
            cout << "WARNING: No sales recorded yet!\n";
        } else {
            cout << "\n" << sys.config.businessName << " - Recent Sales (Last 10):\n";
            cout << left << setw(15) << "Date" << setw(20) << "Item" 
                 << setw(10) << "Qty" << setw(15) << "Amount" 
                 << setw(20) << "Customer" << setw(15) << "Invoice\n";
            printSeparator(95, '-');
            
            int limit = min(10, (int)sys.sales.size());
            for (int i = 0; i < limit; i++) {
                const SaleRecord &sale = sys.sales[sys.sales.size()-1-i];
                cout << left << setw(15) << sale.saleDate.substr(0, 10)
                     << setw(20) << sale.itemName
                     << setw(10) << sale.quantitySold
                     << setw(15) << sys.config.currency + to_string(sale.totalPrice)
                     << setw(20) << sale.customerName
                     << setw(15) << sale.invoiceNumber.substr(0, 15) << "\n";
            }
        }
    } else if (choice == 5) {
        cout << "\n" << sys.config.businessName << " - Expensive Items (Price > 1000):\n";
        bool found = false;
        for (int i = 0; i < 8; ++i) {
            const Category &cat = sys.categories[i];
            for (const Item &item : cat.slots) {
                if (item.present && item.price > 1000) {
                    found = true;
                    cout << cat.name << " - " << item.name 
                         << " | Price: " << sys.config.currency << item.price
                         << " | Qty: " << item.quantity 
                         << " | Discount: " << (item.discountPercentage*100) << "%"
                         << " | Value: " << sys.config.currency << (item.quantity * item.price) << "\n";
                }
            }
        }
        if (!found) cout << "No expensive items found.\n";
    } else if (choice == 6) {
        cout << "\n" << sys.config.businessName << " - Items with Discounts:\n";
        cout << left << setw(20) << "Item" << setw(15) << "Category" 
             << setw(15) << "Discount" << setw(15) << "Price" << setw(20) << "Discounted Price\n";
        printSeparator(85, '-');
        
        int discountCount = 0;
        for (int i = 0; i < 8; ++i) {
            const Category &cat = sys.categories[i];
            for (const Item &item : cat.slots) {
                if (item.present && item.discountPercentage > 0) {
                    discountCount++;
                    double discountedPrice = item.price * (1 - item.discountPercentage);
                    cout << left << setw(20) << item.name
                         << setw(15) << cat.name
                         << setw(15) << to_string(item.discountPercentage*100) + "%"
                         << setw(15) << sys.config.currency + to_string(item.price)
                         << setw(20) << sys.config.currency + to_string(discountedPrice) << "\n";
                }
            }
        }
        if (discountCount == 0) cout << "No discounted items found.\n";
    } else if (choice == 7) {
        cout << "\n" << sys.config.businessName << " - Items Near Expiry (within 30 days):\n";
        string currentDate = getCurrentDate();
        bool found = false;
        
        for (int i = 0; i < 8; ++i) {
            const Category &cat = sys.categories[i];
            for (const Item &item : cat.slots) {
                if (item.present && !item.expiryDate.empty()) {
                    // Simple check for items expiring soon
                    if (item.expiryDate >= currentDate && item.expiryDate <= "2024-12-31") {
                        found = true;
                        cout << cat.name << " - " << item.name 
                             << " | Expiry: " << item.expiryDate
                             << " | Qty: " << item.quantity 
                             << " | Price: " << sys.config.currency << item.price
                             << " | Supplier: " << (item.supplier.empty() ? "N/A" : item.supplier) << "\n";
                    }
                }
            }
        }
        if (!found) cout << "No items near expiry found.\n";
    }
}

void viewLowStock(InventorySystem &sys) {
    printHeader("LOW STOCK ITEMS (using HEAP) - Quantity < " + to_string(sys.config.lowStockThreshold));
    
    // Clear and rebuild the priority queue
    sys.lowStockQueue = LowStockPriorityQueue();
    
    for (int i = 0; i < 8; ++i) {
        const Category &cat = sys.categories[i];
        for (const Item &item : cat.slots) {
            if (item.present && item.quantity < sys.config.lowStockThreshold) {
                HeapNode node(item.name, cat.name, item.quantity, sys.config.lowStockThreshold);
                sys.lowStockQueue.push(node);
            }
        }
    }
    
    if (sys.lowStockQueue.isEmpty()) {
        cout << "SUCCESS: No low stock items found!\n";
        return;
    }
    
    cout << sys.config.businessName << " - Low Stock Report (Priority Order):\n";
    cout << left << setw(5) << "Prio" << setw(15) << "Category" << setw(20) << "Item" 
         << setw(10) << "Qty" << setw(10) << "Threshold" << setw(15) << "Urgency\n";
    printSeparator(80, '-');
    
    vector<HeapNode> allItems = sys.lowStockQueue.getAllItems();
    int priority = 1;
    for (const HeapNode &node : allItems) {
        double ratio = (double)node.quantity / node.threshold;
        string urgency;
        
        if (node.quantity == 0) urgency = "CRITICAL";
        else if (ratio <= 0.2) urgency = "VERY HIGH";
        else if (ratio <= 0.4) urgency = "HIGH";
        else if (ratio <= 0.6) urgency = "MEDIUM";
        else urgency = "LOW";
        
        cout << left << setw(5) << priority++
             << setw(15) << node.category
             << setw(20) << node.itemName
             << setw(10) << node.quantity
             << setw(10) << node.threshold
             << setw(15) << urgency << "\n";
    }
    
    cout << "\nTotal critical items: " << sys.lowStockQueue.size() << "\n";
}

void advancedSearch(InventorySystem &sys) {
    printHeader("ADVANCED SEARCH");
    
    int searchType;
    cout << "Search Options:\n";
    cout << "1. Search by Item Name\n";
    cout << "2. Search by Price Range\n";
    cout << "3. Search by Quantity Range\n";
    cout << "4. Search by Sales Count\n";
    cout << "5. Search Items Never Sold\n";
    cout << "6. Search Items Added in Last 30 Days\n";
    cout << "7. Search by Multiple Criteria\n";
    cout << "8. Search Items by Supplier\n";
    cout << "9. Search Items by Expiry Date\n";
    cout << "\nEnter choice (1-9): ";
    cin >> searchType;
    clearInputBuffer();
    
    if (searchType == 1) {
        cout << "Enter item name (or part of name): ";
        string searchName;
        getline(cin, searchName);
        
        cout << "\n" << sys.config.businessName << " - Search Results for '" << searchName << "':\n";
        bool found = false;
        for (int i = 0; i < 8; ++i) {
            const Category &cat = sys.categories[i];
            for (const Item &item : cat.slots) {
                if (item.present && toLower(item.name).find(toLower(searchName)) != string::npos) {
                    found = true;
                    string status;
                    if (item.quantity == 0) status = "OUT OF STOCK";
                    else if (item.quantity < 3) status = "CRITICAL";
                    else if (item.quantity < 5) status = "LOW";
                    else if (item.quantity < 10) status = "OK";
                    else status = "GOOD";
                    
                    cout << "[" << cat.name << "] " << item.name 
                         << " | Stock: " << item.quantity 
                         << " | Price: " << sys.config.currency << item.price 
                         << " | Discount: " << (item.discountPercentage*100) << "%"
                         << " | Status: " << status 
                         << " | Sales: " << item.salesCount << " units\n";
                }
            }
        }
        
        if (!found) cout << "ERROR: No items found.\n";
        
    } else if (searchType == 2) {
        double minPrice, maxPrice;
        cout << "Enter minimum price: " << sys.config.currency;
        cin >> minPrice;
        cout << "Enter maximum price: " << sys.config.currency;
        cin >> maxPrice;
        clearInputBuffer();
        
        cout << "\n" << sys.config.businessName << " - Items in Price Range " << sys.config.currency << minPrice 
             << " - " << sys.config.currency << maxPrice << ":\n";
        bool found = false;
        for (int i = 0; i < 8; ++i) {
            const Category &cat = sys.categories[i];
            for (const Item &item : cat.slots) {
                if (item.present && item.price >= minPrice && item.price <= maxPrice) {
                    found = true;
                    cout << cat.name << " - " << item.name 
                         << " | Price: " << sys.config.currency << item.price
                         << " | Stock: " << item.quantity 
                         << " | Discount: " << (item.discountPercentage*100) << "%"
                         << " | Value: " << sys.config.currency << (item.quantity * item.price) << "\n";
                }
            }
        }
        if (!found) cout << "ERROR: No items found in this price range.\n";
        
    } else if (searchType == 3) {
        int minQty, maxQty;
        cout << "Enter minimum quantity: ";
        cin >> minQty;
        cout << "Enter maximum quantity: ";
        cin >> maxQty;
        clearInputBuffer();
        
        cout << "\n" << sys.config.businessName << " - Items in Quantity Range " << minQty << " - " << maxQty << ":\n";
        bool found = false;
        for (int i = 0; i < 8; ++i) {
            const Category &cat = sys.categories[i];
            for (const Item &item : cat.slots) {
                if (item.present && item.quantity >= minQty && item.quantity <= maxQty) {
                    found = true;
                    string status;
                    if (item.quantity == 0) status = "OUT OF STOCK";
                    else if (item.quantity < 3) status = "CRITICAL";
                    else if (item.quantity < 5) status = "LOW";
                    else if (item.quantity < 10) status = "OK";
                    else status = "GOOD";
                    
                    cout << cat.name << " - " << item.name 
                         << " | Qty: " << item.quantity 
                         << " | Price: " << sys.config.currency << item.price
                         << " | Discount: " << (item.discountPercentage*100) << "%"
                         << " | Status: " << status << "\n";
                }
            }
        }
        if (!found) cout << "ERROR: No items found in this quantity range.\n";
        
    } else if (searchType == 4) {
        int minSales;
        cout << "Enter minimum sales count: ";
        cin >> minSales;
        clearInputBuffer();
        
        cout << "\n" << sys.config.businessName << " - Items with " << minSales << "+ sales:\n";
        bool found = false;
        for (int i = 0; i < 8; ++i) {
            const Category &cat = sys.categories[i];
            for (const Item &item : cat.slots) {
                if (item.present && item.salesCount >= minSales) {
                    found = true;
                    cout << cat.name << " - " << item.name 
                         << " | Sales: " << item.salesCount << " units"
                         << " | Revenue: " << sys.config.currency << item.totalSalesValue
                         << " | Stock: " << item.quantity 
                         << " | Discount: " << (item.discountPercentage*100) << "%" << "\n";
                }
            }
        }
        if (!found) cout << "ERROR: No items found with that many sales.\n";
        
    } else if (searchType == 5) {
        cout << "\n" << sys.config.businessName << " - Items Never Sold:\n";
        bool found = false;
        for (int i = 0; i < 8; ++i) {
            const Category &cat = sys.categories[i];
            for (const Item &item : cat.slots) {
                if (item.present && item.salesCount == 0) {
                    found = true;
                    cout << cat.name << " - " << item.name 
                         << " | Stock: " << item.quantity 
                         << " | Price: " << sys.config.currency << item.price
                         << " | Discount: " << (item.discountPercentage*100) << "%"
                         << " | Added: " << item.addedDate << "\n";
                }
            }
        }
        if (!found) cout << "SUCCESS: All items have been sold at least once!\n";
        
    } else if (searchType == 6) {
        cout << "\n" << sys.config.businessName << " - Items Added in Last 30 Days:\n";
        bool found = false;
        string currentDate = getCurrentDate();
        for (int i = 0; i < 8; ++i) {
            const Category &cat = sys.categories[i];
            for (const Item &item : cat.slots) {
                if (item.present && !item.addedDate.empty()) {
                    // Simple date check (assuming format YYYY-MM-DD)
                    if (item.addedDate >= "2024-01-01") { // Simplified check
                        found = true;
                        cout << cat.name << " - " << item.name 
                             << " | Added: " << item.addedDate
                             << " | Stock: " << item.quantity 
                             << " | Price: " << sys.config.currency << item.price 
                             << " | Discount: " << (item.discountPercentage*100) << "%" << "\n";
                    }
                }
            }
        }
        if (!found) cout << "ERROR: No recently added items found.\n";
        
    } else if (searchType == 7) {
        // Multiple criteria search
        cout << "\n--- " << sys.config.businessName << " - MULTIPLE CRITERIA SEARCH ---\n";
        
        double minPrice = 0, maxPrice = 999999;
        int minQty = 0, maxQty = 999999;
        int minSales = 0;
        double minDiscount = 0, maxDiscount = 100;
        
        cout << "Price Range (leave 0 for no limit):\n";
        cout << "Min Price: " << sys.config.currency;
        cin >> minPrice;
        cout << "Max Price: " << sys.config.currency;
        cin >> maxPrice;
        
        cout << "\nQuantity Range (leave 0 for no limit):\n";
        cout << "Min Quantity: ";
        cin >> minQty;
        cout << "Max Quantity: ";
        cin >> maxQty;
        
        cout << "\nMinimum Sales Count: ";
        cin >> minSales;
        
        cout << "\nDiscount Range (0-100%):\n";
        cout << "Min Discount %: ";
        cin >> minDiscount;
        cout << "Max Discount %: ";
        cin >> maxDiscount;
        clearInputBuffer();
        
        minDiscount /= 100;
        maxDiscount /= 100;
        
        cout << "\nItems matching all criteria:\n";
        bool found = false;
        for (int i = 0; i < 8; ++i) {
            const Category &cat = sys.categories[i];
            for (const Item &item : cat.slots) {
                if (item.present && 
                    item.price >= minPrice && item.price <= maxPrice &&
                    item.quantity >= minQty && item.quantity <= maxQty &&
                    item.salesCount >= minSales &&
                    item.discountPercentage >= minDiscount && item.discountPercentage <= maxDiscount) {
                    found = true;
                    cout << cat.name << " - " << item.name 
                         << " | Price: " << sys.config.currency << item.price
                         << " | Qty: " << item.quantity 
                         << " | Sales: " << item.salesCount
                         << " | Discount: " << (item.discountPercentage*100) << "%"
                         << " | Value: " << sys.config.currency << (item.quantity * item.price) << "\n";
                }
            }
        }
        if (!found) cout << "ERROR: No items match all criteria.\n";
        
    } else if (searchType == 8) {
        cout << "Enter supplier name (or part of name): ";
        string supplierName;
        getline(cin, supplierName);
        
        cout << "\n" << sys.config.businessName << " - Items from Supplier '" << supplierName << "':\n";
        bool found = false;
        for (int i = 0; i < 8; ++i) {
            const Category &cat = sys.categories[i];
            for (const Item &item : cat.slots) {
                if (item.present && !item.supplier.empty() && 
                    toLower(item.supplier).find(toLower(supplierName)) != string::npos) {
                    found = true;
                    cout << cat.name << " - " << item.name 
                         << " | Supplier: " << item.supplier
                         << " | Stock: " << item.quantity 
                         << " | Price: " << sys.config.currency << item.price
                         << " | Expiry: " << (item.expiryDate.empty() ? "N/A" : item.expiryDate) << "\n";
                }
            }
        }
        if (!found) cout << "ERROR: No items found from this supplier.\n";
        
    } else if (searchType == 9) {
        cout << "Enter expiry date (YYYY-MM-DD) or 'expired' for expired items: ";
        string expiryInput;
        getline(cin, expiryInput);
        
        cout << "\n" << sys.config.businessName << " - Items ";
        if (toLower(expiryInput) == "expired") {
            cout << "that have expired:\n";
        } else {
            cout << "expiring on " << expiryInput << ":\n";
        }
        
        bool found = false;
        string currentDate = getCurrentDate();
        for (int i = 0; i < 8; ++i) {
            const Category &cat = sys.categories[i];
            for (const Item &item : cat.slots) {
                if (item.present && !item.expiryDate.empty()) {
                    bool match = false;
                    if (toLower(expiryInput) == "expired") {
                        match = (item.expiryDate < currentDate);
                    } else {
                        match = (item.expiryDate == expiryInput);
                    }
                    
                    if (match) {
                        found = true;
                        string status = (item.expiryDate < currentDate) ? "EXPIRED" : "ACTIVE";
                        cout << cat.name << " - " << item.name 
                             << " | Expiry: " << item.expiryDate
                             << " | Status: " << status
                             << " | Stock: " << item.quantity 
                             << " | Price: " << sys.config.currency << item.price
                             << " | Supplier: " << (item.supplier.empty() ? "N/A" : item.supplier) << "\n";
                    }
                }
            }
        }
        if (!found) cout << "ERROR: No items found with this expiry criteria.\n";
    }
}

void salesAnalytics(InventorySystem &sys) {
    printHeader("SALES ANALYTICS");
    
    if (sys.sales.empty()) {
        cout << "WARNING: No sales recorded yet!\n";
        return;
    }
    
    cout << sys.config.businessName << " - SALES DASHBOARD\n";
    cout << "Total Sales: " << sys.sales.size() << "\n";
    cout << "Total Revenue: " << sys.config.currency << sys.totalRevenue << "\n";
    cout << "Average per Sale: " << sys.config.currency << (sys.totalRevenue / sys.sales.size()) << "\n";
    cout << "Total Customers: " << sys.customers.size() << "\n";
    cout << "Total Profit: " << sys.config.currency << sys.totalProfit << "\n";
    cout << "Total Discount Given: " << sys.config.currency << [&sys]() {
        double totalDiscount = 0;
        for (const auto &sale : sys.sales) {
            totalDiscount += sale.discountAmount;
        }
        return totalDiscount;
    }() << "\n";
    
    map<string, double> categorySales;
    map<string, int> categoryQty;
    
    for (const SaleRecord &sale : sys.sales) {
        categorySales[sale.category] += sale.totalPrice;
        categoryQty[sale.category] += sale.quantitySold;
    }
    
    cout << "\nSALES BY CATEGORY:\n";
    for (const auto &pair : categorySales) {
        cout << "  " << pair.first << ": " << categoryQty[pair.first] 
             << " items | " << sys.config.currency << pair.second << "\n";
    }
    
    // Daily sales trend
    cout << "\nDAILY SALES TREND (Last 7 days):\n";
    map<string, double> dailySales;
    for (const SaleRecord &sale : sys.sales) {
        string date = sale.saleDate.substr(0, 10);
        dailySales[date] += sale.totalPrice;
    }
    
    vector<pair<string, double>> last7Sales;
    for (auto it = dailySales.rbegin(); it != dailySales.rend() && last7Sales.size() < 7; ++it) {
        last7Sales.push_back(*it);
    }
    
    if (!last7Sales.empty()) {
        for (const auto &day : last7Sales) {
            cout << "  " << day.first << ": " << sys.config.currency << day.second << "\n";
        }
    }
}

void topSellingItems(InventorySystem &sys) {
    printHeader("TOP SELLING ITEMS (using BST)");
    
    vector<pair<string, pair<int, double>>> itemSales;
    
    for (int i = 0; i < 8; ++i) {
        const Category &cat = sys.categories[i];
        for (const Item &item : cat.slots) {
            if (item.salesCount > 0) {
                itemSales.push_back({item.name + "|" + cat.name, 
                                   {item.salesCount, item.totalSalesValue}});
            }
        }
    }
    
    if (itemSales.empty()) {
        cout << "WARNING: No sales data!\n";
        return;
    }
    
    // Build BST sorted by sales count
    sys.salesBST.buildTreeBySales(itemSales);
    auto topBySales = sys.salesBST.getTopItems(10, false);
    
    cout << sys.config.businessName << " - TOP 10 BEST SELLERS (BST Sorted):\n";
    cout << left << setw(5) << "Rank" << setw(20) << "Item" 
         << setw(15) << "Category" << setw(15) << "Sold" << setw(15) << "Revenue\n";
    printSeparator(70, '-');
    
    for (size_t i = 0; i < topBySales.size(); i++) {
        size_t pos = topBySales[i].first.find('|');
        string itemName = topBySales[i].first.substr(0, pos);
        string catName = topBySales[i].first.substr(pos + 1);
        
        cout << left << setw(5) << i+1 
             << setw(20) << itemName
             << setw(15) << catName
             << setw(15) << topBySales[i].second.first
             << setw(15) << sys.config.currency + to_string(topBySales[i].second.second) << "\n";
    }
    
    // Build BST sorted by revenue
    sys.salesBST.buildTreeByRevenue(itemSales);
    auto topByRevenue = sys.salesBST.getTopItems(10, true);
    
    cout << "\n" << sys.config.businessName << " - TOP 10 BY REVENUE (BST Sorted):\n";
    cout << left << setw(5) << "Rank" << setw(20) << "Item" 
         << setw(15) << "Category" << setw(15) << "Revenue" << setw(15) << "Sold\n";
    printSeparator(70, '-');
    
    for (size_t i = 0; i < topByRevenue.size(); i++) {
        size_t pos = topByRevenue[i].first.find('|');
        string itemName = topByRevenue[i].first.substr(0, pos);
        string catName = topByRevenue[i].first.substr(pos + 1);
        
        cout << left << setw(5) << i+1 
             << setw(20) << itemName
             << setw(15) << catName
             << setw(15) << sys.config.currency + to_string(topByRevenue[i].second.second)
             << setw(15) << topByRevenue[i].second.first << "\n";
    }
}

void customerManagement(InventorySystem &sys) {
    printHeader("CUSTOMER MANAGEMENT");
    
    int choice;
    cout << "1. View All Customers\n";
    cout << "2. Search Customer by Name\n";
    cout << "3. View Top Customers by Spending\n";
    cout << "4. Add New Customer\n";
    cout << "5. Update Customer Information\n";
    cout << "Enter choice: ";
    cin >> choice;
    clearInputBuffer();
    
    if (choice == 1) {
        if (sys.customers.empty()) {
            cout << "WARNING: No customers yet!\n";
            return;
        }
        
        cout << sys.config.businessName << " - CUSTOMER DATABASE (" << sys.customers.size() << " customers)\n";
        cout << left << setw(20) << "Name" << setw(15) << "Phone" 
             << setw(15) << "Type" << setw(10) << "Purchases" << setw(15) << "Total Spent" 
             << setw(15) << "Personal Disc\n";
        printSeparator(90, '-');
        
        for (const Customer &cust : sys.customers) {
            cout << left << setw(20) << cust.name
                 << setw(15) << cust.phone
                 << setw(15) << cust.customerType
                 << setw(10) << cust.purchaseCount
                 << setw(15) << sys.config.currency + to_string(cust.totalSpent)
                 << setw(15) << to_string(cust.personalDiscount*100) + "%" << "\n";
        }
        
    } else if (choice == 2) {
        cout << "Enter customer name to search: ";
        string searchName;
        getline(cin, searchName);
        
        bool found = false;
        for (const Customer &cust : sys.customers) {
            if (toLower(cust.name).find(toLower(searchName)) != string::npos) {
                found = true;
                cout << "\nCustomer Details:\n";
                cout << "  Name: " << cust.name << "\n";
                cout << "  Phone: " << cust.phone << "\n";
                cout << "  Type: " << cust.customerType << "\n";
                cout << "  Total Spent: " << sys.config.currency << cust.totalSpent << "\n";
                cout << "  Purchase Count: " << cust.purchaseCount << "\n";
                cout << "  Personal Discount: " << (cust.personalDiscount*100) << "%\n";
                cout << "  Loyalty Points: " << cust.loyaltyPoints << "\n";
                cout << "  Join Date: " << cust.joinDate << "\n";
                cout << "  Last Purchase: " << cust.lastPurchaseDate << "\n";
                cout << "  Address: " << cust.address << "\n";
                cout << "  Email: " << cust.email << "\n";
            }
        }
        if (!found) cout << "ERROR: Customer not found!\n";
        
    } else if (choice == 3) {
        if (sys.customers.empty()) {
            cout << "WARNING: No customers yet!\n";
            return;
        }
        
        // Sort customers by total spent
        vector<Customer> sortedCustomers = sys.customers;
        sort(sortedCustomers.begin(), sortedCustomers.end(),
             [](const Customer &a, const Customer &b) { return a.totalSpent > b.totalSpent; });
        
        cout << sys.config.businessName << " - TOP 10 CUSTOMERS BY SPENDING:\n";
        cout << left << setw(5) << "Rank" << setw(20) << "Name" << setw(15) << "Phone"
             << setw(15) << "Type" << setw(15) << "Total Spent" << setw(10) << "Purchases\n";
        printSeparator(80, '-');
        
        int limit = min(10, (int)sortedCustomers.size());
        for (int i = 0; i < limit; i++) {
            cout << left << setw(5) << i+1
                 << setw(20) << sortedCustomers[i].name
                 << setw(15) << sortedCustomers[i].phone
                 << setw(15) << sortedCustomers[i].customerType
                 << setw(15) << sys.config.currency + to_string(sortedCustomers[i].totalSpent)
                 << setw(10) << sortedCustomers[i].purchaseCount << "\n";
        }
        
    } else if (choice == 4) {
        cout << "ADD NEW CUSTOMER\n";
        
        string name, phone, email, address;
        cout << "Enter customer name: ";
        getline(cin, name);
        
        cout << "Enter phone number: ";
        getline(cin, phone);
        
        cout << "Enter email (or press Enter to skip): ";
        getline(cin, email);
        
        cout << "Enter address (or press Enter to skip): ";
        getline(cin, address);
        
        Customer newCust(name, phone, email, address);
        newCust.joinDate = getCurrentDate();
        sys.customers.push_back(newCust);
        sys.totalCustomers++;
        
        cout << "SUCCESS: Customer added successfully!\n";
        
    } else if (choice == 5) {
        cout << "UPDATE CUSTOMER INFORMATION\n";
        cout << "Enter customer name: ";
        string custName;
        getline(cin, custName);
        
        Customer* targetCust = nullptr;
        for (auto &cust : sys.customers) {
            if (toLower(cust.name) == toLower(custName)) {
                targetCust = &cust;
                break;
            }
        }
        
        if (!targetCust) {
            cout << "ERROR: Customer not found!\n";
            return;
        }
        
        cout << "\nCurrent Information for " << targetCust->name << ":\n";
        cout << "1. Phone: " << targetCust->phone << "\n";
        cout << "2. Email: " << targetCust->email << "\n";
        cout << "3. Address: " << targetCust->address << "\n";
        cout << "4. Personal Discount: " << (targetCust->personalDiscount*100) << "%\n";
        
        cout << "\nSelect field to update (1-4): ";
        int field;
        cin >> field;
        clearInputBuffer();
        
        switch(field) {
            case 1:
                cout << "Enter new phone number: ";
                getline(cin, targetCust->phone);
                break;
            case 2:
                cout << "Enter new email: ";
                getline(cin, targetCust->email);
                break;
            case 3:
                cout << "Enter new address: ";
                getline(cin, targetCust->address);
                break;
            case 4:
                cout << "Enter new personal discount percentage (0-50): ";
                double discount;
                cin >> discount;
                targetCust->personalDiscount = discount / 100;
                clearInputBuffer();
                break;
            default:
                cout << "Invalid choice!\n";
                return;
        }
        
        cout << "SUCCESS: Customer information updated!\n";
    }
}

void advancedDiscountManagement(InventorySystem &sys) {
    printHeader("ADVANCED DISCOUNT MANAGEMENT");
    
    int choice;
    cout << "1. View All Discount Policies\n";
    cout << "2. Set Item-Specific Discount\n";
    cout << "3. Set Seasonal Discount\n";
    cout << "4. Set Customer Personal Discount\n";
    cout << "5. Apply Flash Sale\n";
    cout << "6. View Discounted Items\n";
    cout << "7. Clear All Item Discounts\n";
    cout << "\nEnter choice (1-7): ";
    cin >> choice;
    clearInputBuffer();
    
    if (choice == 1) {
        cout << "\n" << sys.config.businessName << " - ALL DISCOUNT POLICIES\n";
        printSeparator(60, '-');
        
        cout << "BULK PURCHASE DISCOUNTS:\n";
        cout << "  - " << sys.config.bulkQuantity1 << "+ items: " << (sys.config.bulkDiscount1*100) << "% discount\n";
        cout << "  - " << sys.config.bulkQuantity2 << "+ items: " << (sys.config.bulkDiscount2*100) << "% discount\n";
        cout << "  - " << sys.config.bulkQuantity3 << "+ items: " << (sys.config.bulkDiscount3*100) << "% discount\n";
        
        cout << "\nSEASONAL DISCOUNT:\n";
        if (sys.config.enableSeasonalDiscount) {
            cout << "  Status: ACTIVE (" << (sys.config.seasonalDiscountRate*100) << "%)\n";
            cout << "  Period: " << sys.config.discountStartDate << " to " << sys.config.discountEndDate << "\n";
        } else {
            cout << "  Status: INACTIVE\n";
        }
        
        cout << "\nLOYALTY DISCOUNTS:\n";
        cout << "  - Silver (Rs.5,000+): 5% discount\n";
        cout << "  - Gold (Rs.20,000+): 10% discount\n";
        cout << "  - Platinum (Rs.50,000+): 15% discount\n";
        
        cout << "\nITEM-SPECIFIC DISCOUNTS:\n";
        int discountedItems = 0;
        for (int i = 0; i < 8; ++i) {
            for (const Item &item : sys.categories[i].slots) {
                if (item.present && item.discountPercentage > 0) {
                    discountedItems++;
                    cout << "  - " << item.name << " (" << sys.categories[i].name << "): " 
                         << (item.discountPercentage*100) << "% off\n";
                }
            }
        }
        if (discountedItems == 0) cout << "  No item-specific discounts\n";
        
    } else if (choice == 2) {
        cout << "\nSET ITEM-SPECIFIC DISCOUNT\n";
        cout << "Enter category name: ";
        string catName;
        getline(cin, catName);
        
        Category* targetCat = nullptr;
        for (int i = 0; i < 8; ++i) {
            if (toLower(sys.categories[i].name) == toLower(catName)) {
                targetCat = &sys.categories[i];
                break;
            }
        }
        
        if (!targetCat || targetCat->itemCount == 0) {
            cout << "ERROR: Category not found or empty!\n";
            return;
        }
        
        cout << "Enter item name: ";
        string itemName;
        getline(cin, itemName);
        
        int idx = findIndexByName(*targetCat, itemName);
        if (idx == -1 || !targetCat->slots[idx].present) {
            cout << "ERROR: Item not found!\n";
            return;
        }
        
        Item &item = targetCat->slots[idx];
        cout << "Current discount: " << (item.discountPercentage*100) << "%\n";
        cout << "Enter new discount percentage (0-100): ";
        double discount;
        cin >> discount;
        clearInputBuffer();
        
        if (discount < 0 || discount > 100) {
            cout << "ERROR: Discount must be between 0-100%\n";
            return;
        }
        
        item.discountPercentage = discount / 100;
        cout << "SUCCESS: Discount for " << item.name << " set to " << discount << "%\n";
        
    } else if (choice == 3) {
        cout << "\nSET SEASONAL DISCOUNT\n";
        cout << "Current status: " << (sys.config.enableSeasonalDiscount ? "ACTIVE" : "INACTIVE") << "\n";
        cout << "Current rate: " << (sys.config.seasonalDiscountRate*100) << "%\n";
        
        cout << "\n1. Enable/Disable Seasonal Discount\n";
        cout << "2. Change Discount Rate\n";
        cout << "3. Set Discount Period\n";
        cout << "Enter choice: ";
        int subChoice;
        cin >> subChoice;
        clearInputBuffer();
        
        if (subChoice == 1) {
            sys.config.enableSeasonalDiscount = !sys.config.enableSeasonalDiscount;
            cout << "Seasonal discount is now " << (sys.config.enableSeasonalDiscount ? "ENABLED" : "DISABLED") << "\n";
        } else if (subChoice == 2) {
            cout << "Enter new seasonal discount percentage: ";
            double newRate;
            cin >> newRate;
            sys.config.seasonalDiscountRate = newRate / 100;
            cout << "Seasonal discount rate set to " << newRate << "%\n";
            clearInputBuffer();
        } else if (subChoice == 3) {
            cout << "Current period: " << sys.config.discountStartDate << " to " << sys.config.discountEndDate << "\n";
            cout << "Enter new start date (YYYY-MM-DD): ";
            getline(cin, sys.config.discountStartDate);
            cout << "Enter new end date (YYYY-MM-DD): ";
            getline(cin, sys.config.discountEndDate);
            cout << "Discount period set from " << sys.config.discountStartDate 
                 << " to " << sys.config.discountEndDate << "\n";
        }
        
    } else if (choice == 4) {
        cout << "\nSET CUSTOMER PERSONAL DISCOUNT\n";
        if (sys.customers.empty()) {
            cout << "WARNING: No customers found!\n";
            return;
        }
        
        cout << "Enter customer name: ";
        string custName;
        getline(cin, custName);
        
        Customer* targetCust = nullptr;
        for (auto &cust : sys.customers) {
            if (toLower(cust.name) == toLower(custName)) {
                targetCust = &cust;
                break;
            }
        }
        
        if (!targetCust) {
            cout << "ERROR: Customer not found!\n";
            return;
        }
        
        cout << "Customer: " << targetCust->name << "\n";
        cout << "Current discount: " << (targetCust->personalDiscount*100) << "%\n";
        cout << "Total spent: " << sys.config.currency << targetCust->totalSpent << "\n";
        cout << "Customer type: " << targetCust->customerType << "\n";
        
        cout << "\nEnter personal discount percentage (0-50): ";
        double discount;
        cin >> discount;
        clearInputBuffer();
        
        if (discount < 0 || discount > 50) {
            cout << "ERROR: Personal discount must be between 0-50%\n";
            return;
        }
        
        targetCust->personalDiscount = discount / 100;
        cout << "SUCCESS: Personal discount for " << targetCust->name 
             << " set to " << discount << "%\n";
        
    } else if (choice == 5) {
        cout << "\nAPPLY FLASH SALE\n";
        cout << "Flash sale applies discount to ALL items in a category!\n";
        
        cout << "Select category for flash sale:\n";
        for (int i = 0; i < 8; ++i) {
            cout << i+1 << ". " << sys.categories[i].name << "\n";
        }
        
        int catNum;
        cout << "Enter category number: ";
        cin >> catNum;
        
        if (catNum < 1 || catNum > 8) {
            cout << "ERROR: Invalid category!\n";
            clearInputBuffer();
            return;
        }
        
        Category &cat = sys.categories[catNum-1];
        if (cat.itemCount == 0) {
            cout << "ERROR: Category is empty!\n";
            clearInputBuffer();
            return;
        }
        
        cout << "Enter flash sale discount percentage: ";
        double discount;
        cin >> discount;
        clearInputBuffer();
        
        if (discount < 0 || discount > 100) {
            cout << "ERROR: Invalid discount percentage!\n";
            return;
        }
        
        discount /= 100;
        
        // Apply discount to all items in category
        int affectedItems = 0;
        for (auto &item : cat.slots) {
            if (item.present) {
                item.discountPercentage = discount;
                affectedItems++;
            }
        }
        
        cout << "SUCCESS: Flash sale applied to " << affectedItems 
             << " items in " << cat.name << " category!\n";
        cout << "All items now have " << (discount*100) << "% discount\n";
        
    } else if (choice == 6) {
        cout << "\nVIEW ALL DISCOUNTED ITEMS\n";
        cout << left << setw(20) << "Item" << setw(15) << "Category" 
             << setw(15) << "Discount" << setw(15) << "Price" << setw(20) << "Discounted Price\n";
        printSeparator(85, '-');
        
        int discountedCount = 0;
        for (int i = 0; i < 8; ++i) {
            for (const Item &item : sys.categories[i].slots) {
                if (item.present && item.discountPercentage > 0) {
                    discountedCount++;
                    double discountedPrice = item.price * (1 - item.discountPercentage);
                    cout << left << setw(20) << item.name
                         << setw(15) << sys.categories[i].name
                         << setw(15) << to_string(item.discountPercentage*100) + "%"
                         << setw(15) << sys.config.currency + to_string(item.price)
                         << setw(20) << sys.config.currency + to_string(discountedPrice) << "\n";
                }
            }
        }
        
        if (discountedCount == 0) {
            cout << "No discounted items found.\n";
        } else {
            cout << "\nTotal discounted items: " << discountedCount << "\n";
        }
    } else if (choice == 7) {
        cout << "\nCLEAR ALL ITEM DISCOUNTS\n";
        cout << "WARNING: This will remove all item-specific discounts!\n";
        cout << "Are you sure? (y/n): ";
        char confirm;
        cin >> confirm;
        clearInputBuffer();
        
        if (tolower(confirm) == 'y') {
            int clearedCount = 0;
            for (int i = 0; i < 8; ++i) {
                for (auto &item : sys.categories[i].slots) {
                    if (item.present && item.discountPercentage > 0) {
                        item.discountPercentage = 0.0;
                        clearedCount++;
                    }
                }
            }
            cout << "SUCCESS: Cleared discounts from " << clearedCount << " items.\n";
        } else {
            cout << "Operation cancelled.\n";
        }
    }
}

void businessIntelligence(InventorySystem &sys) {
    printHeader("BUSINESS INTELLIGENCE");
    
    double totalValue = 0;
    int totalItems = 0;
    int expiredItems = 0;
    int discountedItems = 0;
    
    string currentDate = getCurrentDate();
    for (int i = 0; i < 8; ++i) {
        totalValue += sys.categories[i].categoryValue;
        totalItems += sys.categories[i].itemCount;
        
        for (const Item &item : sys.categories[i].slots) {
            if (item.present) {
                if (item.discountPercentage > 0) discountedItems++;
                if (!item.expiryDate.empty() && item.expiryDate < currentDate) expiredItems++;
            }
        }
    }
    
    cout << sys.config.businessName << " - BUSINESS OVERVIEW:\n";
    cout << "  Total Revenue: " << sys.config.currency << sys.totalRevenue << "\n";
    cout << "  Total Expenses: " << sys.config.currency << sys.totalExpenses << "\n";
    cout << "  Net Profit: " << sys.config.currency << sys.totalProfit << "\n";
    cout << "  Inventory Value: " << sys.config.currency << totalValue << "\n";
    cout << "  Items in Stock: " << totalItems << "\n";
    cout << "  Discounted Items: " << discountedItems << "\n";
    cout << "  Expired Items: " << expiredItems << "\n";
    cout << "  Total Customers: " << sys.customers.size() << "\n";
    
    double profitMargin = (sys.totalRevenue > 0) ? (sys.totalProfit / sys.totalRevenue * 100) : 0;
    cout << "  Profit Margin: " << fixed << setprecision(2) << profitMargin << "%\n";
    
    // Calculate total discount given
    double totalDiscountGiven = 0;
    for (const auto &sale : sys.sales) {
        totalDiscountGiven += sale.discountAmount;
    }
    cout << "  Total Discount Given: " << sys.config.currency << totalDiscountGiven << "\n";
    
    cout << "\nCATEGORY PERFORMANCE:\n";
    for (int i = 0; i < 8; ++i) {
        const Category &cat = sys.categories[i];
        double percentage = (totalValue > 0) ? (cat.categoryValue / totalValue * 100) : 0;
        cout << "  " << cat.name << ": " << sys.config.currency << cat.categoryValue 
             << " (" << fixed << setprecision(1) << percentage << "%) "
             << "[" << cat.itemCount << " items, Sales: " << sys.config.currency << cat.totalSales << "]\n";
    }
    
    // Customer segmentation
    cout << "\nCUSTOMER SEGMENTATION:\n";
    int regular = 0, silver = 0, gold = 0, platinum = 0;
    for (const auto &cust : sys.customers) {
        if (cust.customerType == "Regular") regular++;
        else if (cust.customerType == "Silver") silver++;
        else if (cust.customerType == "Gold") gold++;
        else if (cust.customerType == "Platinum") platinum++;
    }
    
    cout << "  Regular: " << regular << " customers\n";
    cout << "  Silver: " << silver << " customers\n";
    cout << "  Gold: " << gold << " customers\n";
    cout << "  Platinum: " << platinum << " customers\n";
}

void systemSettings(InventorySystem &sys) {
    printHeader("SYSTEM SETTINGS");
    
    int choice;
    cout << "1. View Current Settings\n";
    cout << "2. Edit Business Information\n";
    cout << "3. Edit Discount Settings\n";
    cout << "4. Edit Tax Settings\n";
    cout << "5. Edit Stock Settings\n";
    cout << "6. Edit Seasonal Discount Settings\n";
    cout << "Enter choice: ";
    cin >> choice;
    clearInputBuffer();
    
    if (choice == 1) {
        cout << "\nCURRENT SYSTEM SETTINGS:\n";
        cout << "  Business Name: " << sys.config.businessName << "\n";
        cout << "  Address: " << sys.config.businessAddress << "\n";
        cout << "  Phone: " << sys.config.businessPhone << "\n";
        cout << "  Tax Number: " << sys.config.taxNumber << "\n";
        cout << "  Tax Rate: " << (sys.config.taxRate * 100) << "%\n";
        cout << "  Low Stock Threshold: " << sys.config.lowStockThreshold << " units\n";
        cout << "  Currency: " << sys.config.currency << "\n";
        cout << "\n  Seasonal Discount: " << (sys.config.enableSeasonalDiscount ? "ENABLED" : "DISABLED") << "\n";
        if (sys.config.enableSeasonalDiscount) {
            cout << "    Rate: " << (sys.config.seasonalDiscountRate * 100) << "%\n";
            cout << "    Period: " << sys.config.discountStartDate << " to " << sys.config.discountEndDate << "\n";
        }
        cout << "\n  Bulk Discount Settings:\n";
        cout << "    - " << sys.config.bulkQuantity1 << "+ items: " << (sys.config.bulkDiscount1*100) << "%\n";
        cout << "    - " << sys.config.bulkQuantity2 << "+ items: " << (sys.config.bulkDiscount2*100) << "%\n";
        cout << "    - " << sys.config.bulkQuantity3 << "+ items: " << (sys.config.bulkDiscount3*100) << "%\n";
        
    } else if (choice == 2) {
        cout << "\nEDIT BUSINESS INFORMATION:\n";
        cout << "Current Business Name: " << sys.config.businessName << "\n";
        cout << "Enter new Business Name: ";
        getline(cin, sys.config.businessName);
        
        cout << "Current Address: " << sys.config.businessAddress << "\n";
        cout << "Enter new Address: ";
        getline(cin, sys.config.businessAddress);
        
        cout << "Current Phone: " << sys.config.businessPhone << "\n";
        cout << "Enter new Phone: ";
        getline(cin, sys.config.businessPhone);
        
        cout << "Current Tax Number: " << sys.config.taxNumber << "\n";
        cout << "Enter new Tax Number: ";
        getline(cin, sys.config.taxNumber);
        
        cout << "SUCCESS: Business information updated!\n";
        
    } else if (choice == 3) {
        cout << "\nEDIT BULK DISCOUNT SETTINGS:\n";
        
        cout << "Current: " << sys.config.bulkQuantity1 << "+ items = " << (sys.config.bulkDiscount1*100) << "%\n";
        cout << "Enter new quantity for first tier: ";
        cin >> sys.config.bulkQuantity1;
        cout << "Enter new discount percentage for first tier: ";
        double disc1;
        cin >> disc1;
        sys.config.bulkDiscount1 = disc1 / 100;
        
        cout << "\nCurrent: " << sys.config.bulkQuantity2 << "+ items = " << (sys.config.bulkDiscount2*100) << "%\n";
        cout << "Enter new quantity for second tier: ";
        cin >> sys.config.bulkQuantity2;
        cout << "Enter new discount percentage for second tier: ";
        double disc2;
        cin >> disc2;
        sys.config.bulkDiscount2 = disc2 / 100;
        
        cout << "\nCurrent: " << sys.config.bulkQuantity3 << "+ items = " << (sys.config.bulkDiscount3*100) << "%\n";
        cout << "Enter new quantity for third tier: ";
        cin >> sys.config.bulkQuantity3;
        cout << "Enter new discount percentage for third tier: ";
        double disc3;
        cin >> disc3;
        sys.config.bulkDiscount3 = disc3 / 100;
        
        clearInputBuffer();
        cout << "SUCCESS: Bulk discount settings updated!\n";
        
    } else if (choice == 4) {
        cout << "\nEDIT TAX SETTINGS:\n";
        cout << "Current Tax Rate: " << (sys.config.taxRate * 100) << "%\n";
        cout << "Enter new tax rate (percentage): ";
        double newRate;
        cin >> newRate;
        sys.config.taxRate = newRate / 100;
        clearInputBuffer();
        cout << "SUCCESS: Tax rate updated to " << newRate << "%!\n";
        
    } else if (choice == 5) {
        cout << "\nEDIT STOCK SETTINGS:\n";
        cout << "Current Low Stock Threshold: " << sys.config.lowStockThreshold << " units\n";
        cout << "Enter new low stock threshold: ";
        cin >> sys.config.lowStockThreshold;
        
        cout << "Current Currency Symbol: " << sys.config.currency << "\n";
        cout << "Enter new currency symbol: ";
        clearInputBuffer();
        getline(cin, sys.config.currency);
        
        cout << "SUCCESS: Stock settings updated!\n";
        
    } else if (choice == 6) {
        cout << "\nEDIT SEASONAL DISCOUNT SETTINGS:\n";
        cout << "Current status: " << (sys.config.enableSeasonalDiscount ? "ENABLED" : "DISABLED") << "\n";
        cout << "1. Enable/Disable Seasonal Discount\n";
        cout << "2. Set Discount Rate\n";
        cout << "3. Set Discount Period\n";
        cout << "Enter choice: ";
        int subChoice;
        cin >> subChoice;
        clearInputBuffer();
        
        if (subChoice == 1) {
            sys.config.enableSeasonalDiscount = !sys.config.enableSeasonalDiscount;
            cout << "Seasonal discount is now " << (sys.config.enableSeasonalDiscount ? "ENABLED" : "DISABLED") << "\n";
        } else if (subChoice == 2) {
            cout << "Current rate: " << (sys.config.seasonalDiscountRate * 100) << "%\n";
            cout << "Enter new seasonal discount percentage: ";
            double newRate;
            cin >> newRate;
            sys.config.seasonalDiscountRate = newRate / 100;
            clearInputBuffer();
            cout << "Seasonal discount rate updated to " << newRate << "%\n";
        } else if (subChoice == 3) {
            cout << "Current period: " << sys.config.discountStartDate << " to " << sys.config.discountEndDate << "\n";
            cout << "Enter new start date (YYYY-MM-DD): ";
            getline(cin, sys.config.discountStartDate);
            cout << "Enter new end date (YYYY-MM-DD): ";
            getline(cin, sys.config.discountEndDate);
            cout << "Seasonal discount period updated.\n";
        }
    }
}

void backupRestore(InventorySystem &sys) {
    printHeader("BACKUP & RESTORE");
    
    int choice;
    cout << "1. Create Backup\n";
    cout << "2. View Backup Information\n";
    cout << "Enter choice: ";
    cin >> choice;
    clearInputBuffer();
    
    if (choice == 1) {
        string filename = "super_mart_inventory_backup_" + getCurrentDate() + "_" + getCurrentTime() + ".txt";
        replace(filename.begin(), filename.end(), ':', '-');
        
        ofstream file(filename);
        
        if (file) {
            file << "=== SUPER MART INVENTORY BACKUP ===\n";
            file << "Backup Date: " << getCurrentDate() << " | Time: " << getCurrentTime() << "\n";
            file << "Business: " << sys.config.businessName << "\n";
            file << "Address: " << sys.config.businessAddress << "\n";
            file << "Phone: " << sys.config.businessPhone << "\n\n";
            
            file << "INVENTORY DATA:\n";
            file << "Total Items: " << sys.totalItems << "\n";
            file << "Total Inventory Value: " << sys.config.currency << [&sys]() {
                double total = 0;
                for (int i = 0; i < 8; ++i) {
                    total += sys.categories[i].categoryValue;
                }
                return total;
            }() << "\n\n";
            
            for (int i = 0; i < 8; ++i) {
                const Category &cat = sys.categories[i];
                if (cat.itemCount > 0) {
                    file << "\nCategory: " << cat.name << " (" << cat.itemCount << " items)\n";
                    file << string(40, '-') << "\n";
                    for (const Item &item : cat.slots) {
                        if (item.present) {
                            file << "  " << item.name << " | Qty: " << item.quantity 
                                 << " | Price: " << sys.config.currency << item.price 
                                 << " | Discount: " << (item.discountPercentage*100) << "%"
                                 << " | Value: " << sys.config.currency << (item.quantity * item.price);
                            if (!item.expiryDate.empty()) file << " | Expiry: " << item.expiryDate;
                            if (!item.supplier.empty()) file << " | Supplier: " << item.supplier;
                            file << "\n";
                        }
                    }
                }
            }
            
            file << "\nFINANCIAL DATA:\n";
            file << "Total Revenue: " << sys.config.currency << sys.totalRevenue << "\n";
            file << "Total Profit: " << sys.config.currency << sys.totalProfit << "\n";
            file << "Total Customers: " << sys.customers.size() << "\n";
            file << "Total Sales: " << sys.sales.size() << "\n";
            
            file << "\nSYSTEM SETTINGS:\n";
            file << "Tax Rate: " << (sys.config.taxRate * 100) << "%\n";
            file << "Low Stock Threshold: " << sys.config.lowStockThreshold << " units\n";
            file << "Currency: " << sys.config.currency << "\n";
            file << "Bulk Discounts: " << sys.config.bulkQuantity1 << "+=" << (sys.config.bulkDiscount1*100) 
                 << "%, " << sys.config.bulkQuantity2 << "+=" << (sys.config.bulkDiscount2*100) 
                 << "%, " << sys.config.bulkQuantity3 << "+=" << (sys.config.bulkDiscount3*100) << "%\n";
            file << "Seasonal Discount: " << (sys.config.enableSeasonalDiscount ? "ACTIVE" : "INACTIVE");
            if (sys.config.enableSeasonalDiscount) {
                file << " (" << (sys.config.seasonalDiscountRate*100) << "%, " 
                     << sys.config.discountStartDate << " to " << sys.config.discountEndDate << ")";
            }
            file << "\n";
            
            file.close();
            cout << "SUCCESS: Backup created: " << filename << "\n";
        } else {
            cout << "ERROR: Failed to create backup file!\n";
        }
    } else if (choice == 2) {
        cout << "\nBACKUP INFORMATION:\n";
        cout << "Backups are saved as text files with timestamp.\n";
        cout << "Filename format: super_mart_inventory_backup_YYYY-MM-DD_HH-MM-SS.txt\n";
        cout << "Backup location: Current directory where program is running.\n";
        cout << "\nBackup includes:\n";
        cout << "1. All inventory items with details\n";
        cout << "2. Financial data (revenue, profit)\n";
        cout << "3. Customer count and sales count\n";
        cout << "4. System settings\n";
        cout << "5. Business information\n";
    }
}

// ==================== ADVANCED DATA STRUCTURE FUNCTIONS ====================

void viewCategoryGraph(InventorySystem &sys) {
    printHeader("CATEGORY RELATIONSHIP GRAPH");
    
    cout << "Category Relationships in " << sys.config.businessName << ":\n\n";
    sys.categoryGraph.printGraph();
    
    cout << "\nRELATED CATEGORY ANALYSIS:\n";
    for (int i = 0; i < 8; ++i) {
        string catName = sys.categories[i].name;
        vector<string> related = sys.categoryGraph.getRelatedCategories(catName);
        if (!related.empty()) {
            cout << "\n" << catName << " is related to: ";
            for (const string& rel : related) {
                cout << rel << " ";
            }
            cout << "\n   Cross-selling opportunities exist!\n";
        }
    }
}

void undoLastOperation(InventorySystem &sys) {
    printHeader("UNDO LAST OPERATION");
    
    if (sys.undoStack.isEmpty()) {
        cout << "No operations to undo!\n";
        return;
    }
    
    auto lastOp = sys.undoStack.popOperation();
    string operation = lastOp.first;
    Item oldItem = lastOp.second;
    
    cout << "Last operation: " << operation << "\n";
    cout << "Item: " << oldItem.name << "\n";
    cout << "Old quantity: " << oldItem.quantity << "\n";
    cout << "Old price: " << sys.config.currency << oldItem.price << "\n";
    
    cout << "\nDo you want to undo this operation? (y/n): ";
    char choice;
    cin >> choice;
    clearInputBuffer();
    
    if (tolower(choice) == 'y') {
        // Find the item in the system
        bool found = false;
        for (int i = 0; i < 8 && !found; ++i) {
            for (auto &item : sys.categories[i].slots) {
                if (item.name == oldItem.name) {
                    if (operation == "DELETE") {
                        item = oldItem;
                        sys.categories[i].itemCount++;
                        sys.totalItems++;
                        sys.categories[i].categoryValue += (item.quantity * item.price);
                    } else if (operation == "ADD") {
                        item.present = false;
                        item.quantity = 0;
                        item.price = 0.0;
                        sys.categories[i].itemCount--;
                        sys.totalItems--;
                        sys.categories[i].categoryValue -= (oldItem.quantity * oldItem.price);
                    } else if (operation == "UPDATE") {
                        double oldValue = item.quantity * item.price;
                        double newValue = oldItem.quantity * oldItem.price;
                        sys.categories[i].categoryValue += (newValue - oldValue);
                        item = oldItem;
                    }
                    found = true;
                    break;
                }
            }
        }
        
        if (found) {
            cout << "SUCCESS: Operation undone!\n";
        } else {
            cout << "ERROR: Item not found in system!\n";
        }
    } else {
        cout << "Undo cancelled.\n";
    }
}

void viewTransactionHistory(InventorySystem &sys) {
    printHeader("TRANSACTION HISTORY (Linked List)");
    
    int transactionCount = sys.transactionHistory.getSize();
    if (transactionCount == 0) {
        cout << "No transactions recorded yet!\n";
        return;
    }
    
    cout << "Total Transactions: " << transactionCount << "\n\n";
    sys.transactionHistory.displayAll();
    
    cout << "\nRecent transactions from Linked List:\n";
    vector<SaleRecord> recent = sys.transactionHistory.getRecentTransactions(5);
    if (!recent.empty()) {
        cout << left << setw(20) << "Item" << setw(15) << "Qty" 
             << setw(15) << "Amount" << setw(20) << "Customer\n";
        printSeparator(70, '-');
        
        for (const auto &transaction : recent) {
            cout << left << setw(20) << transaction.itemName
                 << setw(15) << transaction.quantitySold
                 << setw(15) << sys.config.currency + to_string(transaction.totalPrice)
                 << setw(20) << transaction.customerName << "\n";
        }
    }
}

void processPendingSales(InventorySystem &sys) {
    printHeader("PROCESS PENDING SALES (Queue)");
    
    if (sys.pendingSalesQueue.isEmpty()) {
        cout << "No pending sales in the queue!\n";
        return;
    }
    
    cout << "Pending sales in queue: " << sys.pendingSalesQueue.size() << "\n\n";
    
    int count = 0;
    while (!sys.pendingSalesQueue.isEmpty() && count < 5) {
        SaleRecord sale = sys.pendingSalesQueue.dequeue();
        cout << "Processing sale #" << ++count << ":\n";
        cout << "  Invoice: " << sale.invoiceNumber << "\n";
        cout << "  Item: " << sale.itemName << "\n";
        cout << "  Customer: " << sale.customerName << "\n";
        cout << "  Amount: " << sys.config.currency << sale.totalPrice << "\n";
        cout << "  Date: " << sale.saleDate << "\n";
        cout << string(40, '-') << "\n";
    }
    
    cout << "\nProcessed " << count << " sales from the queue.\n";
    cout << "Remaining in queue: " << sys.pendingSalesQueue.size() << "\n";
}

// ==================== MAIN FUNCTION ====================
int main() {
    InventorySystem sys;
    initInventory(sys);
    
    // DYNAMIC STARTUP HEADER
    cout << "\n";
    printSeparator(70, '=');
    cout << "      " << sys.config.businessName << " - INVENTORY MANAGEMENT SYSTEM\n";
    printSeparator(70, '=');
    cout << "Business: " << sys.config.businessName << "\n";
    cout << "Address: " << sys.config.businessAddress << "\n";
    cout << "Phone: " << sys.config.businessPhone << "\n";
    printSeparator(70, '=');
    
    int choice;
    do {
        printMenu(sys);
        
        if (!(cin >> choice)) {
            cin.clear();
            clearInputBuffer();
            cout << "ERROR: Invalid input! Enter a number 0-18.\n";
            continue;
        }
        
        switch(choice) {
            case 1: addItem(sys); break;
            case 2: updateItem(sys); break;
            case 3: deleteItem(sys); break;
            case 4: sellItem(sys); break;
            case 5: displayInventory(sys); break;
            case 6: viewLowStock(sys); break;
            case 7: advancedSearch(sys); break;
            case 8: salesAnalytics(sys); break;
            case 9: topSellingItems(sys); break;
            case 10: customerManagement(sys); break;
            case 11: advancedDiscountManagement(sys); break;
            case 12: businessIntelligence(sys); break;
            case 13: systemSettings(sys); break;
            case 14: backupRestore(sys); break;
            case 15: viewCategoryGraph(sys); break;
            case 16: undoLastOperation(sys); break;
            case 17: viewTransactionHistory(sys); break;
            case 18: processPendingSales(sys); break;
            case 0: 
                cout << "\n";
                printSeparator(50, '=');
                cout << "          " << sys.config.businessName << " - THANK YOU!\n";
                printSeparator(50, '=');
                cout << "\nFINAL STATISTICS:\n";
                cout << "  Business: " << sys.config.businessName << "\n";
                cout << "  Address: " << sys.config.businessAddress << "\n";
                cout << "  Phone: " << sys.config.businessPhone << "\n";
                cout << "  Total Items: " << sys.totalItems << "\n";
                cout << "  Total Revenue: " << sys.config.currency << sys.totalRevenue << "\n";
                cout << "  Total Customers: " << sys.customers.size() << "\n";
                cout << "  Total Sales: " << sys.sales.size() << "\n";
                cout << "\nGoodbye!\n";
                break;
            default: 
                cout << "ERROR: Invalid choice! Enter 0-18.\n";
                break;
        }
        
        if (choice != 0) {
            cout << "\nPress Enter to continue...";
            clearInputBuffer();
            cin.get();
        }
        
    } while (choice != 0);
    
    return 0;
}