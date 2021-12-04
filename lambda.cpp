#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

template<typename T>
T palindrome(T container) {
    std::vector<std::string> tmp;
    std::copy_if(container.begin(), container.end(), std::back_inserter(tmp), [](std::string s) {
        bool flag = true;
        int n = s.size();
        for(int i=0; i<n/2&&flag;++i) flag = flag && (s[i]==s[n-1-i]);
        return flag;
    });
    T new_container(tmp.begin(),tmp.end());
    return new_container;
}

int main() {
    std::set<std::string> s0;
    int counter;
    std::cin>>counter;
    for(int i=0;i!=counter;++i) {
        std::string tmp;
        std::cin>>tmp;
        s0.insert(tmp);
    }
    std::set<std::string> s1 = palindrome(s0);
    for(auto iter=s1.begin();iter!=s1.end();++iter) std::cout<<*iter<<" ";
}
