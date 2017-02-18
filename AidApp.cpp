#include <cstring>
#include <new>
#include <iomanip>
#include <string>
#include "AidApp.h"

using namespace std;

namespace sict
{
    /*Default Costructor: Copies the filename, sets noOfProducts to zero and sets the products to nullptr.
    Then it loads the records calling the loadRecs() function*/
	AidApp::AidApp(const char* filename)
	{
		strncpy(filename_, filename, 255);
		filename_[255] = '\0';
		for (int i = 0; i < MAX_NO_RECS; i++)
		{
			product_[i] = nullptr;
		}
		noOfProducts_ = 0;
		loadRecs();
	}

    //Display a pause message. Only Enter will close the message
	void AidApp::pause() const
	{
		cout << "Press Enter to continue..." << endl;
		cin.ignore(1000, '\n');
	}

    //display the menu,validate input and return menu selection or -1(if invalid)
	int AidApp::menu()
	{
		int sel;
		cout << "Disaster Aid Supply Management Program" << endl;
		cout << "1- List products" << endl;
		cout << "2- Display product" << endl;
		cout << "3- Add non-perishable product" << endl;
		cout << "4- Add perishable product" << endl;
		cout << "5- Add to quantity of purchased products" << endl;
		cout << "0- Exit program" << endl;
		cout << "> ";
		cin >> sel;
		cout << endl;
		if (!cin.fail() && (sel >= 0 && sel <= 5))
		{
			cin.ignore(2000, '\n');
			return sel;
		}
		else
		{
			cin.clear();
			cin.ignore(2000, '\n');
			return -1;
		}
	}

    //Load records from a file and creates product objects
	void AidApp::loadRecs()
	{
		int readIndex = 0;

		datafile_.open(filename_, ios::in);
		if (datafile_.fail())
		{
			datafile_.clear();
			datafile_.close();
			datafile_.open(filename_, ios::out);
			datafile_.close();
		}
		else
		{
			while (!datafile_.fail())
			{
				delete product_[readIndex];

				char read_c;

				datafile_ >> read_c;

				if (read_c == 'P')
					product_[readIndex] = new AmaPerishable;
				if (read_c == 'N')
					product_[readIndex] = new AmaProduct;
				if (read_c == 'P' || read_c == 'N')
				{
					datafile_.ignore();
					product_[readIndex]->load(datafile_);
					readIndex++;
				}
			}
			noOfProducts_ = readIndex - 1;
			datafile_.close();
		}
	}

    // Save all the records to a file
	void AidApp::saveRecs()
	{
		datafile_.open(filename_, ios::out);
		for (int i = 0; i < noOfProducts_; i++)
		{
			product_[i]->store(datafile_);
		}
		datafile_.close();
	}

    //List all the product objects in a nice output
	void AidApp::listProducts()const
	{
		double tot = 0.00;

		cout << " Row | SKU    | Product Name       | Cost  | QTY| Unit     |Need| Expiry   " << endl;
		cout << "-----|--------|--------------------|-------|----|----------|----|----------" << endl;
		for (int i = 0; i < noOfProducts_; i++)
		{
			cout << right << setw(4) << i+1 << " | " << *product_[i] << endl;
			tot += *product_[i];
			if (i == 9)
				pause();
		}
		cout << "---------------------------------------------------------------------------" << endl;
		cout << "Total cost of support: $" << fixed << setprecision(2) << tot << endl << endl;
	}

	// Receives a sku and searches throu all the products returnig its index or -1 if it can't find the product
	int AidApp::SearchProducts(const char* sku)const
	{
		for (int i = 0; i < noOfProducts_; i++)
		{
			if (*product_[i] == sku)
				return i;
		}
		return -1;
	}

    // Searches for a product and then it adds a quantity based on the quantity needed
	void AidApp::addQty(const char* sku)
	{
		int item_ = SearchProducts(sku);

		if (item_ == -1)
			cout << "Not Found!" << endl;
		else
		{
			int value;
			product_[item_]->write(cout, false);
			cout << endl << endl << "Please enter the number of purchased items: ";
			cin >> value;
			if (cin.fail())
				cout << "Invalid quantity value" << endl;
			else
			{
				if (value <= (product_[item_]->qtyNeeded() - product_[item_]->quantity()))
					*product_[item_] += value;
				else
				{
					int extra = value - (product_[item_]->qtyNeeded() - product_[item_]->quantity());
					value = (product_[item_]->qtyNeeded() - product_[item_]->quantity());
					*product_[item_] += value;
					cout << "Too many items; only " << value << " is needed, please return the extra " << extra << " items." << endl;
				}
				saveRecs();
				cout << endl << "Updated!" << endl << endl;
			}
			cin.clear();
			cin.ignore(2000, '\n');
		}
	}

	// Creates a new product and saves it into the product array and then on file.
	// The product is either perishable or not based on the value received
	void AidApp::addProduct(bool isPerishable)
	{
	    if (isPerishable)
		{
			product_[noOfProducts_] = new AmaPerishable;
		}
		else
		{
			product_[noOfProducts_] = new AmaProduct;
		}
		product_[noOfProducts_]->read(cin);
		if (!cin.fail())
		{
			noOfProducts_++;
			saveRecs();
			cout << endl << "Product added" << endl << endl;
		}
		else
		{
			cout << *product_[noOfProducts_] << endl << endl;
			cin.clear();
			cin.ignore(2000, '\n');
		}
    }

	// Based on the menu input it calls the functions required and passes the values needed
	int AidApp::run()
	{
		int option;
		int stop = 1;
		char s_item[MAX_SKU_LEN + 1];
		int found;

		while (stop != 0)
		{
			option = menu();
			switch (option)
			{
			case 1:
				listProducts();
				pause();
				break;
			case 2:
				cout << "Please enter the SKU: ";
				cin.getline(s_item, MAX_SKU_LEN);
				cout << endl;
				found = SearchProducts(s_item);
				if (found != -1)
                {
                    product_[found]->write(cout, false);
                    cout << endl << endl;
                }
				else
                {
                    cout << "Not found!" << endl;
                    cin.clear();
                    //cin.ignore(1000, '\n');
                }
                pause();
				break;
			case 3:
				addProduct(false);
 				loadRecs();
				break;
			case 4:
				addProduct(true);
				loadRecs();
				break;
			case 5:
				cout << "Please enter the SKU: ";
				cin.getline(s_item, MAX_SKU_LEN);
				cout << endl;
				addQty(s_item);
				break;
			case 0:
				cout << "Goodbye!!" << endl;
				stop = 0;
				break;
			default:
				cout << "===Invalid Selection, try again.===" << endl;
				pause();
			}
		}
		return 0;
	}
}
