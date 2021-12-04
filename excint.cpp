#include <exception>
#include <iostream>
#include <limits>

class IntegerOverflow: public std::exception {
public:
    const char* what() { return "Value overfull!"; }
};

class Integer {
private:
    int value;
public:
    Integer(): Integer(0) { }
    Integer(int value): value(value) { }

    Integer(Integer const &src): Integer(src.value) { }
    Integer& operator=(Integer const &src) { this->value=src.value; return *this; }
    Integer(Integer &&src): Integer(src.value) { src.value=0; }
    Integer& operator=(Integer &&src) { this->value=src.value; src.value=0; return *this; }
    ~Integer() { std::cout<<"Ouch!\n"; }

    Integer& operator++() {
        if(this->value==INT_MAX) throw IntegerOverflow();
        else { ++(this->value); return *this; }
    }

    Integer operator++(int) {
        if(this->value==INT_MAX) throw IntegerOverflow();
        else {
            Integer tmp = *this;
            ++(this->value);
            return tmp;
        }
    }

    Integer& operator--() {
        if(this->value==INT_MIN) throw IntegerOverflow();
        else { --(this->value); return *this; }
    }

    Integer operator--(int) {
        if(this->value==INT_MIN) throw IntegerOverflow();
        else { Integer tmp = *this; --(this->value); return tmp; }
    }

    Integer& operator+=(Integer &src) {
        long long thVl = this->value;
        long long sVl = src.value;
        long long Vl = sVl + thVl;
        std::cout<<thVl<<" "<<sVl<<" "<<Vl<<"\n";
        if(Vl<INT_MIN||Vl>INT_MAX) throw IntegerOverflow();
        else { (this->value)+=(src.value); return *this; }
    }

    Integer& operator-=(Integer &src) {
        long long thVl = this->value;
        long long sVl = src.value;
        long long Vl = sVl - thVl;
        std::cout<<thVl<<" "<<sVl<<" "<<Vl<<"\n";
        if(Vl<INT_MIN||Vl>INT_MAX) throw IntegerOverflow();
        else { (this->value)-=(src.value); return *this; }
    }

    Integer& operator*=(Integer &src) {
        long long thVl = this->value;
        long long sVl = src.value;
        long long Vl = sVl * thVl;
        std::cout<<thVl<<sVl<<Vl<<"\n";
        if(Vl<INT_MIN||Vl>INT_MAX) throw IntegerOverflow();
        else { (this->value)*=(src.value); return *this; }
    }

    Integer& operator+(Integer &src) { *this+=src; return *this; }
    Integer& operator-(Integer &src) { *this-=src; return *this; }
    Integer& operator*(Integer &src) { *this*=src; return *this; }

    Integer& operator/(Integer &src) { this->value = (this->value)/(src.value); return *this; }
    Integer& operator%(Integer &src) { this->value = (this->value)%(src.value); return *this; }

    friend bool operator<(Integer &src) { return this->value<src.value; }
    friend bool operator<=(Integer &src) { return this->value<=src.value; }
    friend bool operator>=(Integer &src) { return this->value>=src.value; }
    friend bool operator>(Integer &src) { return this->value>src.value; }
    friend bool operator==(Integer &src) { return this->value==src.value; }

    friend std::ostream& operator<<(std::ostream &out, const Integer& num) { out<<num.value<<"\n"; return out; }

    friend std::istream& operator>>(std::istream &in, Integer& num) { in>>num.value; return in; }
};

int main() {
    try {
        Integer A, B;
        std::cin>>A>>B;
        std::cout<<A+B<<" "<<A*B;
    } catch(IntegerOverflow &err) {
        std::cout<<err.what();
    } catch(...) {
        std::cout<<"Unknown error!\n";
    }
    return 0;
}
