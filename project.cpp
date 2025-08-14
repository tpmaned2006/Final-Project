#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Transaction structure to hold each record
struct Transaction {
    int id;
    string date;     // Format: YYYY-MM-DD
    string category; // e.g., Food, Travel, Shopping
    string description;
    double amount;
};

class AnalyticsSummary {
private:
    std::vector<Transaction> transactions;
public:
    AnalyticsSummary(const std::vector<Transaction> &txns) : transactions(txns) {}

    void generateSummary() {
        double totalIncome = 0, totalExpense = 0;
        for (const auto &txn : transactions) {
            if (txn.amount >= 0) totalIncome += txn.amount;
            else totalExpense += txn.amount;
        }

        std::cout << "\n=== Analytics Summary ===\n";
        std::cout << "Total Income : " << totalIncome << "\n";
        std::cout << "Total Expense: " << totalExpense << "\n";
        std::cout << "Net Balance  : " << (totalIncome + totalExpense) << "\n";
        std::cout << "=========================\n";
    }
};

// Ledger class to manage all transactions
class Ledger {
private:
    vector<Transaction> transactions;
    int nextId;
    string filename;

public:
    Ledger(string file) {
        filename = file;
        nextId = 1;
        loadFromFile();
    }

    void addTransaction(string date, string category, string description, double amount) {
        Transaction t;
        t.id = nextId++;
        t.date = date;
        t.category = category;
        t.description = description;
        t.amount = amount;
        transactions.push_back(t);
        saveToFile();
        cout << "✅ Transaction added successfully!\n";
    }

    void viewTransactions() {
        if (transactions.empty()) {
            cout << "📂 No transactions found.\n";
            return;
        }

        cout << left << setw(5) << "ID" << setw(12) << "Date" << setw(15) << "Category"
             << setw(25) << "Description" << setw(10) << "Amount" << endl;
        cout << "---------------------------------------------------------------------\n";

        for (auto &t : transactions) {
            cout << left << setw(5) << t.id
                 << setw(12) << t.date
                 << setw(15) << t.category
                 << setw(25) << t.description
                 << setw(10) << fixed << setprecision(2) << t.amount
                 << endl;
        }
    }

    void searchTransaction(string keyword) {
        bool found = false;
        for (auto &t : transactions) {
            if (t.category == keyword || t.date == keyword || t.description.find(keyword) != string::npos) {
                if (!found) {
                    cout << "🔍 Matching transactions:\n";
                    cout << left << setw(5) << "ID" << setw(12) << "Date" << setw(15) << "Category"
                         << setw(25) << "Description" << setw(10) << "Amount" << endl;
                    cout << "---------------------------------------------------------------------\n";
		}
                cout << left << setw(5) << t.id
                     << setw(12) << t.date
                     << setw(15) << t.category
                     << setw(25) << t.description
                     << setw(10) << fixed << setprecision(2) << t.amount
                     << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "❌ No matching transactions found.\n";
        }
    }

    void deleteTransaction(int id) {
        for (auto it = transactions.begin(); it != transactions.end(); ++it) {
            if (it->id == id) {
                transactions.erase(it);
                saveToFile();
                cout << "🗑️ Transaction deleted successfully.\n";
                return;
            }
        }
        cout << "❌ Transaction with ID " << id << " not found.\n";
    }

    void showAnalyticsSummary();

    void showAdvancedAnalytics();
    void Ledger::showAdvancedAnalytics() {
    // Example: Monthly total, highest expense category, etc.
    map<string, double> categoryTotals;
    double totalSpent = 0;
    double highestExpense = 0;
    string highestCategory;

    for (auto &transaction : transactions) {
        categoryTotals[transaction.category] += transaction.amount;
        totalSpent += transaction.amount;

        if (categoryTotals[transaction.category] > highestExpense) {
            highestExpense = categoryTotals[transaction.category];
            highestCategory = transaction.category;
        }
    }

    cout << "\n===== Advanced Analytics =====\n";
    cout << "Total spent: " << totalSpent << "\n";
    cout << "Highest spending category: " << highestCategory 
         << " (" << highestExpense << ")\n";
    cout << "Spending by category:\n";
    for (auto &entry : categoryTotals) {
        cout << " - " << entry.first << ": " << entry.second << "\n";
    }
    cout << "=============================\n";
}

private:
    void saveToFile() {
        ofstream out(filename);
        if (!out) {
            cerr << "❌ Error saving file.\n";
            return;
        }
        for (auto &t : transactions) {
            out << t.id << "," << t.date << "," << t.category << ","
                << t.description << "," << t.amount << "\n";
        }
    }

    void loadFromFile() {
        ifstream in(filename);
        if (!in) {
            return; // No file yet
        }
        Transaction t;
        char comma;
        while (in >> t.id >> comma &&
               getline(in, t.date, ',') &&
               getline(in, t.category, ',') &&
               getline(in, t.description, ',') &&
               (in >> t.amount)) {
            transactions.push_back(t);
            if (t.id >= nextId) {
                nextId = t.id + 1;
            }
        }
    }
};

// Utility to clear input stream
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Menu display
void showMenu() {
    cout << "\n===== LedgerLite Menu =====\n";
    cout << "1. Add Transaction\n";
    cout << "2. View All Transactions\n";
    cout << "3. Search Transaction\n";
    cout << "4. Delete Transaction\n";
    cout << "5. Analytics Summary\n";
    cout << "6. Show Advanced Analytics\n";
    cout << "7. Exit\n";
    cout << "===========================\n";
    cout << "Enter choice: ";
}

int main() {
    Ledger ledger("transactions.csv");
    int choice;

    do {
        showMenu();
        cin >> choice;
        clearInput();

        if (choice == 1) {
            string date, category, description;
            double amount;
            cout << "Enter date (YYYY-MM-DD): ";
            getline(cin, date);
            cout << "Enter category: ";
            getline(cin, category);
            cout << "Enter description: ";
            getline(cin, description);
            cout << "Enter amount: ";
            cin >> amount;
            clearInput();
            ledger.addTransaction(date, category, description, amount);
        }
        else if (choice == 2) {
            ledger.viewTransactions();
        }
        else if (choice == 3) {
            string keyword;
            cout << "Enter date/category/keyword to search: ";
            getline(cin, keyword);
            ledger.searchTransaction(keyword);
        }
        else if (choice == 4) {
            int id;
            cout << "Enter transaction ID to delete: ";
            cin >> id;
            clearInput();
            ledger.deleteTransaction(id);
        }
        else if (choice == 5) {
            ledger.showAnalyticsSummary();
        }
        else if (choice == 6) {
            ledger.showAdvancedAnalytics(); 
        }
        else {
	    cout << "Invalid choice. Try again.\n";

    } while (choice != 7);

    return 0;
}

