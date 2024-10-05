#ifndef FINALS_DATA_H
#define FINALS_DATA_H

#include <iostream>
#include <fstream>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <iomanip>
#include <unordered_set>
#include <unordered_map>
#include <limits>

using namespace std;

class Video;
class CustomerRent;

//base class Customer
class Customer {
public:
    int Customer_ID;
    string Name;
    string Address;

    Customer(int id, string name, string address)
        : Customer_ID(id), Name(name), Address(address) {}

    static void addNewCustomer(queue<Customer*>& customers, map<int, int>& customerIndexById, int& nextCustomerId);
    static void showCustomerDetails(const queue<Customer*>& customers, const map<int, int>& customerIndexById);
    static void printAllCustomers(const queue<Customer*>& customers);
    virtual void rentVideo(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById) = 0;
    virtual void returnVideo(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById) = 0;
    virtual void printRentedVideos() const = 0;
    virtual void displayDetails() const = 0;
};

//child class CustomerRent, inheriting from Customer
class CustomerRent : public Customer {
public:
    stack<int> Rented_Videos;

    CustomerRent(int id, string name, string address)
        : Customer(id, name, address) {}

    void rentVideo(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById) override;
    void returnVideo(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById) override;
    void printRentedVideos() const override;
    void displayDetails() const override;
    void displayDetails(const list<Video>& videos) const;

    static void listRentedVideosByCustomer(const queue<CustomerRent*>& customers, const map<int, int>& customerIndexById, const list<Video>& videos);
    static void loadCustomersFromFile(queue<CustomerRent*>& customers, map<int, int>& customerIndexById, int& nextCustomerId);
    static void loadRentedVideosFromFile(queue<CustomerRent*>& customers, map<int, int>& customerIndexById);
    bool isVideoRented(int videoId) const;
    static void saveCustomersToFile(const queue<CustomerRent*>& customers);
    static void saveRentedVideosToFile(const queue<CustomerRent*>& customers);
    void displayRentedVideos() const;
};

// video class definition
class Video {
public:
    int Video_ID;
    string Movie_Title;
    string Genre;
    string Production;
    int Number_of_Copies;

    Video(int id, string title, string genre, string production, int copies)
        : Video_ID(id), Movie_Title(title), Genre(genre), Production(production), Number_of_Copies(copies) {}

    static void addNewVideo(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById, int& nextVideoId);
    static void rentVideo(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById, queue<CustomerRent*>& customers, map<int, int>& customerIndexById);
    static void returnVideo(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById, queue<CustomerRent*>& customers, map<int, int>& customerIndexById);
    static void showVideoDetails(const list<Video>& videos, const map<int, list<Video>::iterator>& videoIndexById);
    static void displayAllVideos(const list<Video>& videos);
    static void checkVideoAvailability(const list<Video>& videos, const map<int, list<Video>::iterator>& videoIndexById);
    static void loadVideosFromFile(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById, int& nextVideoId);
    static void saveVideosToFile(const list<Video>& videos);
    void displayDetails() const;
};

#endif // FINALS_DATA_H