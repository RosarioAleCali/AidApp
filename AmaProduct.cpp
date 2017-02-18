#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "AmaProduct.h"

using namespace std;

namespace sict
{
    AmaProduct::AmaProduct(char tag)
    {
        fileTag_ = tag;
    }

    const char* AmaProduct::unit() const
    {
        return unit_;
    }

    void AmaProduct::unit(const char* value)
    {
        strncpy(unit_, value, 10);
        unit_[10] = '\0';
    }

    std::fstream& AmaProduct::store(std::fstream& file, bool addNewLine) const
    {
        file << fileTag_ << "," << sku() << "," << name() << "," << price() << "," << taxed() << "," << quantity() << ","
             << unit_ << "," << qtyNeeded();

        if(addNewLine)
            file << endl;

        return file;
    }

    std::fstream& AmaProduct::load(std::fstream& file)
    {
        char buf_char[1501] = "";
        double buf_dbl;
        int buf_int;

        file.getline(buf_char,1500,',');
        sku(buf_char);
        file.getline(buf_char,1500,',');
        name(buf_char);
        file >> buf_dbl;
        file.ignore();
        price(buf_dbl);
        file >> buf_int;
        file.ignore();
        taxed(bool(buf_int));
        file >> buf_int;
        file.ignore();
        quantity(buf_int);
        file.getline(buf_char,1500,',');
        unit(buf_char);
        file >> buf_int;
        qtyNeeded(buf_int);
        file.ignore();

        return file;
    }

    std::ostream& AmaProduct::write(std::ostream& os, bool linear) const
    {
        if(!err_.isClear())
        {
            os << err_;

            return os;
        }
        else
        {
            if(linear)
            {
                os.width(MAX_SKU_LEN);
                os << left << sku() << "|";
                os.width(20);
                os << left << name() << "|";
                os.width(7);
                os << right << fixed << setprecision(2) << cost() << "|";
                os.width(4);
                os << quantity() << "|";
                os.width(10);
                os << left << unit() << "|";
                os.width(4);
                os << right << qtyNeeded() << "|";
            }
            else
            {
                os << "Sku: " << sku() << endl;
                os << "Name: " << name() << endl;
                os << "Price: " << price() << endl;
                if(taxed())
                    os << "Price after tax: " << cost() << endl;
                else
                    os << "Price after tax: N/A" << endl;
                os << "Quantity On Hand: " << quantity() << " " << unit() << endl;
                os << "Quantity Needed: " << qtyNeeded();
            }
            return os;
        }
    }

    std::istream& AmaProduct::read(std::istream& istr)
    {
        char sku_t[MAX_SKU_LEN + 1];
        char name_t[75];
        char unit_t[11];
        char tax_t;
        double price_t;
        int qty_t;
        int qty_n_t;

        if(!istr.fail()) err_.clear();
        cout << "Sku: ";
        istr.getline(sku_t, MAX_SKU_LEN, '\n');
        cout << "Name: ";
        istr >> name_t;
        cout << "Unit: ";
        istr >> unit_t;
        cout << "Taxed? (y/n): ";
        istr >> tax_t;
        if (tax_t == 'N' || tax_t == 'n' || tax_t == 'Y' || tax_t== 'y')
        {
            istr.ignore(1000, '\n');
            cout << "Price: ";
            istr >> price_t;
        }
        else
        {
            istr.setstate(ios::failbit);
            err_.message("Only (Y)es or (N)o are acceptable");
        }

        if (err_.isClear() && istr.fail())
        {
            err_.message("Invalid Price Entry");
        }
        if (err_.isClear())
        {
            cout << "Quantity On hand: ";
            istr >> qty_t;
        }

        if (err_.isClear() && istr.fail())
        {
            err_.message("Invalid Quantity Entry");
        }
        if (err_.isClear())
        {
            cout << "Quantity Needed: ";
            istr >> qty_n_t;
        }

        if (err_.isClear() && istr.fail())
        {
            err_.message("Invalid Quantity Needed Entry");
        }

        if (!istr.fail())
        {
            sku(sku_t);
            name(name_t);
            unit(unit_t);
            taxed(tax_t == 'y' || tax_t == 'Y');
            price(price_t);
            quantity (qty_t);
            qtyNeeded (qty_n_t);
        }
        return istr;
    }

}
