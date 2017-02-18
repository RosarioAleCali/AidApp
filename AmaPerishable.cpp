#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "AmaPerishable.h"

using namespace std;

namespace sict
{
    AmaPerishable::AmaPerishable()  : AmaProduct('P')
    {

    }

    const Date& AmaPerishable::expiry() const
    {
        return expiry_;
    }

    void AmaPerishable::expiry(const Date& value)
    {
        expiry_ = value;
    }

    std::fstream& AmaPerishable::store(std::fstream& file, bool addNewLine) const
    {
        AmaProduct::store(file, false);
        file << ",";
        file << expiry_;

        if(addNewLine)
            file << endl;

        return file;
    }

    std::fstream& AmaPerishable::load(std::fstream& file)
    {
        AmaProduct::load(file);
        expiry_.read(file);
        file.ignore();

        return file;
    }

    std::ostream& AmaPerishable::write(std::ostream& os, bool linear) const
    {
        AmaProduct::write(os, linear);
        if(err_.isClear() && !isEmpty())
        {
            if(linear)
                cout << expiry_;
            else
                cout << endl << "Expiry date: " << expiry_;
        }

        return os;
    }

    std::istream& AmaPerishable::read(std::istream& istr)
    {
        AmaProduct::read(istr);
        if(err_.isClear())
        {
            cout << "Expiry date (YYYY/MM/DD): ";

            Date temp;
            istr >> temp;

            if(temp.errCode() != 0)
            {
                if(temp.errCode() == 1)
                    err_.message("Invalid Date Entry");
                else if(temp.errCode() == 2)
                    err_.message("Invalid Year in Date Entry");
                else if(temp.errCode() == 3)
                    err_.message("Invalid Month in Date Entry");
                else
                    err_.message("Invalid Day in Date Entry");

            istr.setstate(ios::failbit);
            }
            else
            {
                expiry_ = temp;
            }
        }
        return istr;
    }
}
