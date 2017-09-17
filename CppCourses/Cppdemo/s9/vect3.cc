// vect3.cpp -- using STL functions
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

class Review{
	public:
    string title;
    int rating;
	bool operator<(const Review& rhs) const;
};

bool worseThan(const Review & r1, const Review & r2);
bool FillReview(Review & rr);
void ShowReview(const Review & rr);

int main()
{
    vector<Review> books;
    Review temp;
    while (FillReview(temp))
        books.push_back(temp);
    cout << "Thank you. You entered the following "
         << books.size() << " ratings:\n"
          << "Rating\tBook\n";
    for_each(books.begin(), books.end(), ShowReview);
    
    sort(books.begin(), books.end());
    cout << "Sorted by title:\nRating\tBook\n";
    for_each(books.begin(), books.end(), ShowReview);
    
    sort(books.begin(), books.end(), worseThan);
    cout << "Sorted by rating:\nRating\tBook\n";
    for_each(books.begin(), books.end(), ShowReview);

    random_shuffle(books.begin(), books.end());
    cout << "After shuffling:\nRating\tBook\n";
    for_each(books.begin(), books.end(), ShowReview);

    return 0;
}

bool Review::operator<(const Review & r) const
{
    if (title < r.title)
        return true;
    else if (title == r.title && rating < r.rating)
        return true;
    else
        return false;
}

bool worseThan(const Review & r1, const Review & r2)
{
    if (r1.rating < r2.rating)
        return true;
    else
        return false;
}

bool FillReview(Review & rr)
{
    cout << "Enter book title (quit to quit): ";
    getline(cin,rr.title);
    if (rr.title == "quit")
        return false;
    cout << "Enter book rating: ";
    cin >> rr.rating;
    if (!cin)
        return false;
    cin.get();
    return true;
}

void ShowReview(const Review & rr)
{
    cout << rr.rating << "\t" << rr.title << endl;
}

