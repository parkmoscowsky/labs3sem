#include <bits/stdc++.h>
#include <functional>
#include <iostream>

template <typename T>
struct SkipNode {
    T value;
    SkipNode **backwardNodes,**forwardNodes;

    explicit SkipNode(T val, int size) {
        value=val;
        backwardNodes=new SkipNode*[size];
        forwardNodes=new SkipNode*[size];
        for(int pos=0;pos<size;++pos) backwardNodes[pos]=forwardNodes[pos]=nullptr;
    }

    ~SkipNode() {
        delete next();
        delete[] backwardNodes;
        delete[] forwardNodes;
    }

    static void connect(SkipNode *a,SkipNode *b,int lvl) {
        if(b!=nullptr) b->backwardNodes[lvl]=a;
        if(a!=nullptr) a->forwardNodes[lvl]=b;
    }

    static void multiconnect(SkipNode *a,SkipNode *b,SkipNode *c,int lvl) {
        connect(a,b,lvl);
        connect(b,c,lvl);
    }

    SkipNode *next(int lvl=0) const { return forwardNodes[lvl]; }

    SkipNode *prev(int lvl=0) const { return backwardNodes[lvl]; }
};

template <typename T>
class SkipList {
    class SkipListIterator: public std::iterator<std::bidirectional_iterator_tag, SkipList<T>> {
        SkipNode<T> *NodePtr;
    public:
        explicit SkipListIterator(SkipNode<T> *Ptr) { NodePtr=Ptr; }

        SkipNode<T> *operator~() { return NodePtr; }

        const T &operator*() const { return NodePtr->value; }

        T &operator*() { return NodePtr->value; }

        SkipListIterator &operator++() { NodePtr=NodePtr->next(); return *this; }

        SkipListIterator &operator--() { NodePtr=NodePtr->prev(); return *this; }

        bool operator==(const SkipListIterator &iter) const { return NodePtr==iter.NodePtr; }

        bool operator!=(const SkipListIterator &iter) const { return NodePtr!=iter.NodePtr; }
    };

    std::function<int(T, T)> compare;
    int listSize = 0;
    int maxLevel, currLevel;
    double probability;
    SkipNode<T> *head, *NIL;
public:
    explicit SkipList(int maxLvl=6,double prob=0.25,std::function<int(T,T)> cmp=[](T a, T b){ return a-b; }) {
        compare=cmp;
        maxLevel=maxLvl;
        probability=prob;
        currLevel=1;
        head=NIL=new SkipNode<T>(-1,maxLevel);
    }

    void copy(const SkipList<T> &src) {
        compare=src.compare;
        maxLevel=src.maxLevel;
        currLevel=src.currLevel;
        listSize=src.listSize;
        NIL=src.NIL;
        probability=src.probability;

        head=new SkipNode<T>(-1,maxLevel);
        auto **rm=new SkipNode<T> *[maxLevel];
        auto **rmSrc=new SkipNode<T> *[maxLevel];

        for(int pos=0;pos<maxLevel;++pos) { rm[pos]=head; rmSrc[pos]=src.head; }

        SkipNode<T> *tmp=head;
        SkipNode<T> *tmpSrc=src.head;

        while(tmpSrc!=nullptr) {
            for(int pos=0;pos<maxLevel;++pos) {
                if(rmSrc[pos]->forwardNodes[pos]==tmpSrc) {
                    SkipNode<T>::connect(rm[pos],tmp,pos);
                    rmSrc[pos]=tmpSrc;
                    rm[pos]=tmp;
                }
            }

            tmpSrc=tmpSrc->forwardNodes[0];
            if(tmpSrc) tmp->forwardNodes[0]=new SkipNode<T>(tmpSrc->value,maxLevel);
            tmp=tmp->forwardNodes[0];
        }

        delete[] rm;
        delete[] rmSrc;
    }

    SkipList& move(SkipList<T> &&src) {
        maxLevel=src.maxLevel;
        probability=src.probability;
        currLevel=src.currLevel;
        src.currLevel=0;
        head=src.head;
        NIL=src.NIL;
        listSize=src.listSize;
        src.listSize=0;
        src.NIL=src.head=nullptr;
        compare=src.compare;

        return *this;
    }

    SkipList(const SkipList<T> &src) { copy(src); }

    SkipList(SkipList<T> &&src) { move(); }

    SkipList &operator=(SkipList<T> &&src) {
        if(&src!=this) { delete[] head; move(); }

        return *this;
    }

    SkipList<T> &operator=(const SkipList<T> &src) {
        if(this==&src) return *this;
        delete head;
        copy(src);

        return *this;
    }

    ~SkipList() { delete head; }

    bool probTest() const { return (((float)rand())/RAND_MAX)<probability; }

    int size() const { return listSize; }

    bool empty() const { return !listSize; }

    SkipList& clear() {
        delete head;
        NIL=head=new SkipNode<T>(-1,maxLevel);
        currLevel=1;
        listSize=0;

        return *this;
    }

    SkipList& erase(SkipListIterator iter) {
        SkipNode<T> *Iter = ~iter;
        for(int pos=0;pos<maxLevel;++pos) {
            SkipNode<T>::connect(Iter->prev(pos),Iter->next(pos),pos);
            Iter->forwardNodes[pos]=nullptr;
        }

        delete Iter;

        return *this;
    }

    SkipList& erase(SkipListIterator from,SkipListIterator to) {
        SkipListIterator To=to;
        ++To;
        while(from!=To) {
            SkipListIterator tmp=from;
            ++tmp;
            erase(from);
            from=tmp;
        }

        return *this;
    }

    SkipList& erase(std::pair<SkipListIterator,SkipListIterator> range) { erase(range.first,range.second); return *this; }

    SkipListIterator begin() { return SkipListIterator(head->next()); }

    SkipListIterator end() { return SkipListIterator(nullptr); }

    SkipList& insert(T val) {
        SkipNode<T> **rm=new SkipNode<T> *[maxLevel];
        for(int pos=0;pos<maxLevel;++pos) { rm[pos]=head; }

        SkipNode<T> *Ptr=head, *nPtr;
        for (int pos=currLevel-1;pos>=0;--pos) {
            nPtr=Ptr->next(pos);
            while(nPtr!=nullptr&&compare(nPtr->value,val)<0) { Ptr=nPtr; nPtr=Ptr->next(pos); }
            rm[pos]=Ptr;
        }

        SkipNode<T> *newNode=new SkipNode<T>(val,maxLevel);
        SkipNode<T>::multiconnect(rm[0],newNode,rm[0]->next(0),0);
        for(int pos=1;(pos<maxLevel)&&probTest();++pos) {
            SkipNode<T>::multiconnect(rm[pos],newNode,rm[pos]->next(pos),pos);
            if(pos+1>currLevel) currLevel=pos+1;
        }

        ++listSize;
        if(rm[0]==NIL) NIL=Ptr;

        delete[] rm;

        return *this;
    }

    SkipListIterator find(T val) {
        SkipNode<T> *Ptr=head,*nPtr;

        for(int pos=currLevel-1;pos>=0;--pos) {
            nPtr=Ptr->next(pos);
            while(nPtr!=nullptr) {
                if(compare(nPtr->value,val)<0) { Ptr=nPtr; nPtr=Ptr->next(pos);
                } else if(compare(nPtr->value,val)==0) {
                    return SkipListIterator(nPtr);
                } else break;
            }
        }

        return end();
    }

    int count(T val) {
        SkipListIterator leftIter=find(val);
        if(leftIter==end()) return 0;
        int counter=1;
        SkipListIterator rightIter=leftIter,tmpIter=rightIter;
        while(*++tmpIter==*rightIter) { rightIter=tmpIter; ++counter; }
        tmpIter=leftIter;
        while(*--tmpIter==*leftIter) { leftIter=tmpIter; ++counter; }

        return counter;
    }

    SkipListIterator lower_bound(T val) {
        SkipNode<T> *Ptr=head,*nPtr;
        for(int pos=currLevel-1;pos>=0;--pos) {
            nPtr=Ptr->forwardNodes(pos);

            while(nPtr!=nullptr) {
                if(compare(nPtr->value,val)<0) { Ptr=nPtr; nPtr=Ptr->forwardNodes(pos);
                } else break;
            }
        }

        return SkipListIterator(nPtr);
    }

    SkipListIterator upper_bound(T val) {
        SkipNode<T> *Ptr=head,*nPtr;
        for(int pos=currLevel-1;pos>=0;--pos) {
            nPtr=Ptr->next(pos);

            while(nPtr!=nullptr) {
                if(compare(nPtr->value,val)<=0) { Ptr=nPtr; nPtr=Ptr->next(pos);
                } else break;
            }
        }

        return SkipListIterator(nPtr);
    }

    std::pair<SkipListIterator,SkipListIterator> equal_range(T val) {
        SkipListIterator leftIter=find(val);
        if(leftIter==end()) std::make_pair(leftIter,leftIter);
        SkipListIterator rightIter=leftIter,tmpIter=rightIter;

        while(*++tmpIter==*rightIter) rightIter=tmpIter;
        tmpIter=leftIter;

        while(*--tmpIter==*leftIter) leftIter=tmpIter;

        return std::make_pair(leftIter,rightIter);
    }

    friend std::istream& operator>>(std::istream &in,SkipList &l) {
        T val;
        in>>val;
        l.insert(val);

        return in;
    }

    friend std::ostream& operator<<(std::ostream &out,SkipList &l) {
        for(int pos=l.maxLevel-1;pos>=0;--pos) {
            out<<"Level No."<<pos<<":\n";
            SkipNode<T> *P=l.head->next(pos);
            while(P) { out<<P->value<<" "; P=P->next(pos); }

            out<<"\n";
        }

        out<<"\n";

        return out;
    }
};


int main() {
    SkipList<int> skiplist(3,0.29,[](int a,int b){ return a-b; });

    skiplist.insert(1);
    skiplist.insert(6);
    skiplist.insert(6);

    for(int pos=0;pos!=10;++pos) std::cin>>skiplist;

    std::cout<<skiplist;
    std::cout<<skiplist.count(6)<<" elements are equal to 6"<<"\n";
    skiplist.erase(skiplist.equal_range(1));
    std::cout<<skiplist;
    skiplist.erase(skiplist.equal_range(6));
    std::cout<<skiplist;

    std::cout<<"\nElements from 10:\n";
    for(auto iter=skiplist.upper_bound(10);iter!=skiplist.end();++iter) std::cout<<*iter<<" ";
    std::cout<<"\nIterating from begin to end:\n";
    for(auto iter:skiplist) std::cout<<iter<<" ";

    return 0;
}
