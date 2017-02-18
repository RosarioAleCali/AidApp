#include <iomanip>
#include <iostream>
#include "Date.h"
#include "general.h"


namespace sict
{
    int Date::value()const
    {
        return year_ * 372 + mon_ * 31 + day_;
    }

    void Date::errCode(int errorCode)
    {
        readErrorCode_ = errorCode;
    }

    Date::Date()
    {
        year_ = 0;
        mon_ = 0;
        day_ = 0;
        readErrorCode_ = NO_ERROR;
    }

    Date::Date(int year, int month, int day)
    {
        year_ = year;
        mon_ = month;
        day_ = day;
        readErrorCode_ = NO_ERROR;
    }

    bool Date::operator==(const Date& D) const
    {
        return this->value() == D.value();
    }

    bool Date::operator!=(const Date& D) const
    {
        return !(this->value() == D.value());
    }

    bool Date::operator<=(const Date& D) const
    {
        return this->value() <= D.value();
    }

    bool Date::operator>=(const Date& D) const
    {
        return this->value() >! D.value();
    }

    bool Date::operator<(const Date& D) const
    {
        return this->value() < D.value();
    }

    bool Date::operator>(const Date& D) const
    {
        return !(this->value() < D.value());
    }

    int Date::errCode()const
    {
        return readErrorCode_;
    }

    bool Date::bad()const
    {
        return readErrorCode_ != 0;
    }

    std::istream& Date::read(std::istream& istr)
    {
        readErrorCode_ = NO_ERROR;

        istr >> year_;
        istr.ignore();
        istr >> mon_;
        istr.ignore();
        istr >> day_;

        if(istr.fail())
            readErrorCode_ = CIN_FAILED;
        else
        {
            if(!(year_ >= MIN_YEAR && year_ <= MAX_YEAR))
            {
                readErrorCode_ = YEAR_ERROR;
            }
            else if (!(mon_ >= 1 && mon_ <= 12))
            {
                readErrorCode_ = MON_ERROR;
            }
            else
            {
                if(!(day_ >= 1 && day_ <= mdays()))
                    readErrorCode_ = DAY_ERROR;
            }
        }

        return istr;
    }

    std::ostream& Date::write(std::ostream& ostr) const
    {
        ostr << std::setfill('0') << year_ << "/" << std::setw(2) << mon_ << "/" << std::setw(2) << day_ << std::setfill(' ');

        return ostr;
    }

    std::ostream& operator<<(std::ostream& os, const Date& D)
    {
        D.write(os);

        return os;
    }

    std::istream& operator>>(std::istream& is, Date& D)
    {
        D.read(is);

        return is;
    }


    int Date::mdays()const
    {
        int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1 };
        int mon = mon_ >= 1 && mon_ <= 12 ? mon_ : 13;
        mon--;
        return days[mon] + int((mon == 1)*((year_ % 4 == 0) && (year_ % 100 != 0)) || (year_ % 400 == 0));
    }
}
