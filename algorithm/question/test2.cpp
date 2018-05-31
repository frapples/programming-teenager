#include <list>
#include <iostream>
using namespace std;


int cal(list<int> arr) {
    if (arr.size() <= 1) {
        return 0;
    }


    list<int>::iterator end = arr.end();
    end--;

    if (arr.size() == 2) {
        return *(arr.begin()) == *end ? 0 : 1;
    }


    if (*(arr.begin()) == *(end)) {
        arr.erase(arr.begin());
        arr.erase(end);
        return cal(arr);
    }


    list<int>::iterator first = arr.begin();
    list<int>::iterator second = first;
    ++second;

    list<int>::iterator end1 = end;
    --end1;

    if (*first + *second <= *end) {
        *second = *first + *second;
        arr.erase(first);
        return cal(arr) + 1;
    } else if (*end + *end1 <= *first) {
        *end = *end + *end1;
        arr.erase(end1);
        return cal(arr) + 1;
    } else {
        if (arr.size() == 3) {
            return 2;
        }

        *second = *first + *second;
        arr.erase(first);
        *end = *end + *end1;
        arr.erase(end1);
        return cal(arr) + 2;
    }
}

int main(void)
{
    int n;
    cin >> n;
    list<int> nums;
    for (int i = 0; i < n; i++) {
        int num;
        cin >> num;
        nums.push_back(num);
    }

    cout << cal(nums) << endl;
    return 0;
}
