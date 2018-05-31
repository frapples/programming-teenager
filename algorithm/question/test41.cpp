#include<vector>
#include<iostream>
#include<algorithm>

int main(int argc, char *argv[])
{
    std::vector<int> nums;
    while (std::cin.peek() != '\n') {
        int n;
        std:: cin >> n;
        nums.push_back(n);
    }

    int k;
    std::cin >> k;

    std::sort(nums.begin(), nums.end());
    printf("%d\n", nums[nums.size() - k]);

    return 0;
}
