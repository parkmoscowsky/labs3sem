#include<iostream>
#include<set>
#include<random>

class Predicate {
public:
    virtual bool operator()(int prob) const = 0;
};

class DiscretePredicate: public Predicate {
private:
    int const state;
public:
    DiscretePredicate(int state): state(state) { }

    bool operator()(int prob) const {
        return state == prob;
    }
};

class SegmentPredicate: public Predicate {
private:
    int const bint, eint;
public:
    SegmentPredicate(): bint(0), eint(-1) { }
    SegmentPredicate(int bint, int eint): bint(bint), eint(eint) { }

    bool operator()(int prob) const {
        return prob>=bint && prob<=eint;
    }
};

class SetPredicate: public Predicate {
private:
    std::set<int> const states;
public:
    SetPredicate(): states() { }
    SetPredicate(std::set<int> const &src): states(src) { }

    bool operator()(int prob) const {
        return states.count(prob)>0;
    }
};

class IntersectionPredicate: public Predicate {
private:
    const Predicate &pred1, &pred2;
public:
    IntersectionPredicate(Predicate const &pred1, Predicate const &pred2): pred1(pred1), pred2(pred2) { }

    bool operator()(int prob) const {
        return pred1(prob)&&pred2(prob);
    }
};

class UnionPredicate: public Predicate {
private:
    const Predicate &pred1, &pred2;
public:
    UnionPredicate(Predicate const &pred1, Predicate const &pred2): pred1(pred1), pred2(pred2) { }

    bool operator()(int prob) const {
        return pred1(prob)||pred2(prob);
    }
};

class SegmentPlusSetPredicate: public Predicate {
private:
    int const bint, eint;
    std::set<int> const states;
public:
    SegmentPlusSetPredicate(int bint, int eint, std::set<int> const &src): bint(bint), eint(eint), states(src) { }

    bool operator()(int prob) const {
        SegmentPredicate s(bint, eint);
        SetPredicate ss(states);
        UnionPredicate u(s,ss);
        return u(prob);
    }
};

class SegmentMinusSetPredicate: public Predicate {
private:
    int const bint, eint;
    std::set<int> const states;
public:
    SegmentMinusSetPredicate(int bint, int eint, std::set<int> const &src): bint(bint), eint(eint), states(src) { }

    bool operator()(int prob) const {
        SegmentPredicate s(bint, eint);
        SetPredicate ss(states);
        IntersectionPredicate i(s,ss);
        return i(prob);
    }
};

class SegmentPlusMinusSetPredicate: public Predicate {
private:
    int const bint, eint;
    std::set<int> const plus_states, minus_states;
public:
    SegmentPlusMinusSetPredicate(int bint, int eint, std::set<int> const &plus_src, std::set<int> const &minus_src): bint(bint), eint(eint), plus_states(plus_src), minus_states(minus_src) { }

    bool operator()(int prob) const {
        SetPredicate s(plus_states);
        SegmentMinusSetPredicate ss(bint, eint, minus_states);
        UnionPredicate u(s,ss);
        return u(prob);
    }
};

class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count): seed(seed), test_min(test_min), test_max(test_max), test_count(test_count) { }

    float operator()(Predicate const &predicate) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution <int>dstr(test_min,test_max);
        unsigned good = 0;
        for (unsigned cnt=0; cnt!=test_count; ++cnt)
            if (predicate(dstr(rng))) ++good;

        return static_cast<float>(good)/static_cast<float>(test_count);
    }
};

int main(int argc, const char* argv[]) {
    DiscretePredicate d(1);
    SegmentPredicate s(0,10);
    SetPredicate ss({1,3,5,7,23,48,57,60,90,99});
    ProbabilityTest pt(10,0,100,100000);
    std::cout<<pt(d)<<std::endl;
    std::cout<<pt(s)<<std::endl;
    std::cout<<pt(ss)<<std::endl;
    return 0;
}
