#include<iostream>
#include<string>
#include<vector>

class BigInteger;

BigInteger operator + (const BigInteger& first_term, const BigInteger& sec_term);
BigInteger operator - (const BigInteger& minuend, const BigInteger& subtrahend);
BigInteger operator * (const BigInteger& first_mult, const BigInteger& sec_mult);
BigInteger operator / (const BigInteger& dividend, const BigInteger& divider);
BigInteger operator % (const BigInteger& dividend, const BigInteger& divider);

bool operator < (const BigInteger& a, const BigInteger& b);
bool operator == (const BigInteger& a, const BigInteger& b);
bool operator > (const BigInteger& a, const BigInteger& b);
bool operator != (const BigInteger& a, const BigInteger& b);
bool operator <= (const BigInteger& a, const BigInteger& b);
bool operator >= (const BigInteger& a, const BigInteger& b);

std::istream& operator >> (std::istream& read, BigInteger& our_bigint);
std::ostream& operator << (std::ostream& write, const BigInteger& our_bigint);

class BigInteger {
    std::vector<int> BigNumber;
    bool is_positive;
public:

    const BigInteger NumberAbs() const {
        BigInteger ans = *this;
        ans.is_positive = true;
        return ans;
    }

    BigInteger(const BigInteger& a) :
        BigNumber(a.BigNumber),
        is_positive(a.is_positive) {
    }
    BigInteger(const int& a) {
        if (a < 0) {
            is_positive = false;
            int b = a*-1;
            while (b > 0) {
                BigNumber.push_back(b % 10);
                b /= 10;
            }

        }
        else {
            is_positive = true;
            if (a == 0) {
                BigNumber.push_back(0);
            }
            else {
                int used = a;
                while (used > 0) {
                    BigNumber.push_back(used % 10);
                    used /= 10;
                }
            }
        }
    }
    BigInteger() :
        is_positive(true) {
    };
    BigInteger(const std::string& str_form) {
        BigNumber.clear();
        if (str_form[0] == '-') {
            is_positive = false;
        } else {
            is_positive = true;
        }
        for (size_t i = 0; i < str_form.size(); ++i) {
            if (str_form[str_form.size() - 1 - i] != '-')
                BigNumber.push_back(str_form[str_form.size() - 1 - i] - '0');
        }
    }

    BigInteger& operator ++ () {
        *this += 1;
        return *this;
    }
    BigInteger& operator -- () {
        *this -= 1;
        return *this;
    }
    const BigInteger operator ++ (int) {
        BigInteger new_this = *this;
        *this += 1;
        return new_this;
    }
    const BigInteger operator -- (int) {
        BigInteger new_this = *this;
        *this -= 1;
        return new_this;
    }
    const BigInteger operator - () const {
        BigInteger rev;
        if (BigNumber[BigNumber.size() - 1] != 0) {
            rev.is_positive = !is_positive;
        }
        rev.BigNumber = BigNumber;
        return rev;
    }


    BigInteger& operator = (const BigInteger& a) {
        is_positive = a.is_positive;
        BigNumber = a.BigNumber;
        return *this;
    }
    BigInteger& operator += (const BigInteger& a) {
        if (is_positive == a.is_positive) {
            for (size_t i = 0; i < BigNumber.size(); ++i) {
                if (i < a.BigNumber.size()) {
                    BigNumber[i] += a.BigNumber[i];
                }
            }
            if (a.BigNumber.size() > BigNumber.size()) {
                for (int i = a.BigNumber.size() - BigNumber.size(); i > 0; --i) {
                    BigNumber.push_back(a.BigNumber[BigNumber.size()]);
                }
            }
            BigNumber.push_back(0);
            for (size_t i = 0; i < BigNumber.size() - 1; ++i) {
                BigNumber[i + 1] += BigNumber[i] / 10;
                BigNumber[i] %= 10;
            }
            if (BigNumber[BigNumber.size() - 1] == 0) {
                BigNumber.pop_back();
            }
        }
        else {
            is_positive = !is_positive;
            *this -= a;
            is_positive = !is_positive;
        }
        return *this;
    }
    BigInteger& operator -= (const BigInteger& a) {
        if (is_positive == a.is_positive) {
            if (NumberAbs() >= a.NumberAbs()) {
                for (size_t i = 0; i < a.BigNumber.size(); ++i) {
                    BigNumber[i] -= a.BigNumber[i];
                }
                for (size_t i = 0; i < BigNumber.size(); ++i) {
                    if (BigNumber[i] < 0) {
                        BigNumber[i] = 10 + BigNumber[i];
                        --BigNumber[i + 1];
                    }
                }
                while (BigNumber[BigNumber.size() - 1] == 0 && BigNumber.size() > 1) {
                    BigNumber.pop_back();
                }
            } else {
                  is_positive = !is_positive;
                  for (size_t i = 0; i < BigNumber.size(); i++) {
                      BigNumber[i] = a.BigNumber[i] - BigNumber[i];
                  }
                  for (size_t i = BigNumber.size(); i < a.BigNumber.size(); ++i) {
                      BigNumber.push_back(a.BigNumber[i]);
                  }
                  for (size_t i = 0; i < BigNumber.size(); ++i) {
                      if (BigNumber[i] < 0) {
                          BigNumber[i] = 10 + BigNumber[i];
                          --BigNumber[i + 1];
                      }
                  }
                  while (BigNumber[BigNumber.size() - 1] == 0 && BigNumber.size() > 1) {
                      BigNumber.pop_back();
                  }
            }
        } else {
              is_positive = !is_positive;
              *this += a;
              is_positive = !is_positive;
        }
        return *this;
    }
    BigInteger& operator *= (const BigInteger& a) {
        is_positive = !(a.is_positive ^ is_positive);
        std::vector<int> mul(BigNumber.size() + a.BigNumber.size());
        for (size_t i = 0; i < BigNumber.size(); ++i) {
            for (size_t j = 0; j < a.BigNumber.size(); j++) {
                mul[i + j] += BigNumber[i] * a.BigNumber[j];
            }
        }
        size_t i = 0;
        while (i < mul.size()) {
            if (mul[i] >= 10) {
                if (i == mul.size() - 1) {
                    mul.push_back(0);
                }
                mul[i + 1] += mul[i] / 10;
                mul[i] %= 10;
            }
            i++;
        }
        while (mul.size() > 1 && mul.back() == 0) {
            mul.pop_back();
        }
        BigNumber = std::move(mul);
        return *this;
    }
    BigInteger& operator /= (const BigInteger& a) {
        if (a == 0) {
            *this = 0;
            return *this;
        }
        bool sym = !(a.is_positive ^ is_positive);
        is_positive = true;

        if (a > *this) {
            *this = 0;
            return *this;
        }

        std::vector<int> rez;

        BigInteger used = a;
        used.is_positive = true;
        size_t used_size = used.BigNumber.size();
        int our_size = BigNumber.size();
        for (size_t i = 0; i < our_size - used_size; ++i) {
            used *= 10;
        }
        for (size_t i = 0; i < our_size; i++) {
            if (used.BigNumber.size() >= used_size) {
                rez.push_back(0);
                while (*this >= used) {
                    *this -= used;
                    rez.back()++;
                }
                for (size_t j = 0; j < used.BigNumber.size() - 1; ++j) {
                    used.BigNumber[j] = used.BigNumber[j + 1];
                }
                used.BigNumber.pop_back();
            }
        }
        is_positive = sym;
        std::vector<int> rev_rez;
        for (size_t i = 0; i < rez.size(); ++i) {
            rev_rez.push_back(rez[rez.size() - 1 - i]);
        }
        if (rev_rez[rev_rez.size() - 1] == 0) {
            rev_rez.pop_back();
        }
        BigNumber = std::move(rev_rez);
        return *this;
    }
    BigInteger& operator %= (const BigInteger& a) {
        *this -= ((*this / a) * a);
        return *this;
    }

    bool smaler(const BigInteger& b) const {
        if (BigNumber[BigNumber.size() - 1] == 0 && b.BigNumber[b.BigNumber.size() - 1] == 0) {
            return false;
        }
        if (is_positive && !b.is_positive) {
            return false;
        }
        if (!is_positive && b.is_positive) {
            return true;
        }
        if (is_positive && b.is_positive) {
            if (BigNumber.size() > b.BigNumber.size()) {
                return false;
            }
            if (BigNumber.size() < b.BigNumber.size()) {
                return true;
            }
            if (BigNumber.size() == b.BigNumber.size()) {
                for (int i = BigNumber.size() - 1; i >= 0; --i) {
                    if (BigNumber[i] < b.BigNumber[i]) {
                        return true;
                    }
                    if (BigNumber[i] > b.BigNumber[i]) {
                        return false;
                    }
                }
                return false;
            }
        }
        if ((!is_positive && !b.is_positive)) {
            if (BigNumber.size() > b.BigNumber.size()) {
                return true;
            }
            if (BigNumber.size() < b.BigNumber.size()) {
                return false;
            }
            if (BigNumber.size() == b.BigNumber.size()) {
                for (int i = BigNumber.size() - 1; i >= 0; --i) {
                    if (BigNumber[i] < b.BigNumber[i]) {
                        return false;
                    }
                    if (BigNumber[i] > b.BigNumber[i]) {
                        return true;
                    }
                }
                return false;
            }
        }
        return true;
    }

    const std::string toString() const {
        std::string str_form;
        if (!is_positive && *this != 0) {
            str_form.push_back('-');
        }
        for (size_t i = 0; i < BigNumber.size(); ++i) {
            str_form.push_back(BigNumber[BigNumber.size() - 1 - i] + '0');
        }
        return str_form;
    }

    explicit operator bool() const {
        if(BigNumber[BigNumber.size() - 1]) {
            return true;
        }
        return false;
    }
};

bool operator < (const BigInteger& a, const BigInteger& b) {
    return a.smaler(b);
}
bool operator == (const BigInteger& a, const BigInteger& b) {
    return (!(a < b) && !(b < a));
}
bool operator > (const BigInteger& a, const BigInteger& b) {
    return b < a;
}
bool operator != (const BigInteger& a, const BigInteger& b) {
    return !(a == b);
}
bool operator <= (const BigInteger& a, const BigInteger& b) {
    return !(a > b);
}
bool operator >= (const BigInteger& a, const BigInteger& b) {
    return !(a < b);
}

BigInteger operator + (const BigInteger& first_term, const BigInteger& sec_term) {
    BigInteger sum = first_term;
    sum += sec_term;
    return sum;
}
BigInteger operator - (const BigInteger& minuend, const BigInteger& subtrahend) {
    BigInteger dif = minuend;
    dif -= subtrahend;
    return dif;
}
BigInteger operator * (const BigInteger& first_mult, const BigInteger& sec_mult) {
    BigInteger prod = first_mult;
    prod *= sec_mult;
    return prod;
}
BigInteger operator / (const BigInteger& dividend, const BigInteger& divider) {
    if (dividend == 0) {
        return 0;
    }
    BigInteger rel = dividend;
    rel /= divider;
    return rel;
}
BigInteger operator % (const BigInteger& dividend, const BigInteger& divider) {
    BigInteger rem = dividend;
    rem %= divider;
    return rem;
}

std::istream& operator >> (std::istream& read, BigInteger& our_bigint) {
    std::string str_form;
    read >> str_form;
    our_bigint = BigInteger(str_form);
    return read;
}

std::ostream& operator << (std::ostream& write, const BigInteger& our_bigint) {
    write << our_bigint.toString();
    return write;
}