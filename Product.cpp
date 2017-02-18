#include <cstring>
#include "Product.h"
#include "general.h"

using namespace std;

namespace sict
{
    Product::Product()
    {
        sku_[0] = '\0';
        name_ = nullptr;
        price_ = 0;
        taxed_ = true;
        quantity_ = 0;
        qtyNeeded_ = 0;
    }

    Product::Product(const char* upc, const char* nm, bool tax, double cost, int need)
    {
        if(upc != nullptr && nm != nullptr && cost >= 0 && need >= 0)
        {
            // Re-using setter functions
            sku (upc);
            name (nm);
            taxed (tax);
            quantity_ = 0;
            price (cost);
            qtyNeeded (need);
        }
    }

    Product::Product(const Product& source)
    {
        name_ = nullptr;
        *this = source;
    }

    Product& Product::operator=(const Product& source)
    {
        if(this != &source)
        {
            // Re-using setter functions
            sku(source.sku_);
			quantity(source.quantity_);
			price(source.price_);
			taxed(source.taxed_);
			qtyNeeded(source.qtyNeeded_);

            delete [] name_; // Delete previously allocated memory <-- Problem was here!
            name(source.name_); // Call copy constructor
        }
        return *this;
    }

    Product::~Product()
    {
        delete [] name_;
    }

    void Product::sku (const char* upc)
    {
        strncpy(sku_, upc, MAX_SKU_LEN);
        sku_[MAX_SKU_LEN] = '\0';
    }

    void Product::price (double cost)
    {
        price_ = cost;
    }

    void Product::name (const char* nm)
    {
        name_ = new char[strlen(nm) + 1];
        strcpy(name_, nm);
    }

    void Product::taxed (bool decision)
    {
        taxed_ = decision;
    }

    void Product::quantity (int qnt)
    {
        quantity_ = qnt;
    }

    void Product::qtyNeeded (int need)
    {
        qtyNeeded_ = need;
    }

    const char* Product::sku() const
    {
        return sku_;
    }

    double Product::price() const
    {
        return price_;
    }

    const char* Product::name() const
    {
        return name_;
    }

    bool Product::taxed() const
    {
        return taxed_;
    }

    int Product::quantity() const
    {
        return quantity_;
    }

    int Product::qtyNeeded() const
    {
        return qtyNeeded_;
    }

    double Product::cost() const
    {
        if(taxed())
        {
            return price_ * (TAX +1);
        }
        else
            return price_;
    }

    bool Product::isEmpty() const
    {
        return sku_[0] == '\0' && name_ == nullptr && quantity_ == 0 && qtyNeeded_ == 0 && price_ == 0;
    }

    bool Product::operator==(const char* source)const
    {
        return strcmp(sku_, source)==0;
    }

    int Product::operator+=(int source)
    {
        quantity_ += source;

        return quantity_;
    }

    int Product::operator-=(int source)
    {
        quantity_ -= source;

        return quantity_;
    }

    double operator+=(double& value, const Product& obj)
    {
        value += (obj.quantity() * obj.cost());

        return value;
    }

    std::ostream& operator<<(std::ostream& ostr, const Product& obj)
    {
        obj.write(ostr, true);

        return ostr;
    }

    std::istream& operator>>(std::istream& istr, Product& obj)
    {
        obj.read(istr);

        return istr;
    }
}
