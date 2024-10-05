#include "finals-data.h"

// system("cls")
void clearScreen() {
    system("cls");
}

// display details of video
void Video::displayDetails() const {
    cout << "Video ID             : " << Video_ID << "\n"
         << "Movie Title          : " << Movie_Title << "\n"
         << "Genre                : " << Genre << "\n"
         << "Production           : " << Production << "\n"
         << "Number of Copies     : " << Number_of_Copies << "\n"
         << "Availability         : " << (Number_of_Copies > 0 ? "Available" : "Not Available") << "\n";
}

// Video ADT function implementations
// [1] add new video
void Video::addNewVideo(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById, int& nextVideoId) {
    clearScreen();

    cout << "=============================================\n";
    cout << setw(33) << "LeStack's Video Store\n";
    cout << setw(31) << "[1] ADD NEW VIDEO\n";
    cout << "=============================================\n";

    string title, genre, production;
    int copies;

    cout << "Enter movie title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter genre: ";
    getline(cin, genre);
    cout << "Enter production: ";
    getline(cin, production);

    bool validInput = false;
    while (!validInput) {
        cout << "Enter number of copies: ";
        cin >> copies;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n\n";
        } else if (copies < 0) {
            cout << "Invalid input. Please enter a non-negative number.\n\n";
        } else {
            validInput = true;
        }
    }

    Video newVideo(nextVideoId++, title, genre, production, copies);
    videos.push_back(newVideo);
    videoIndexById[newVideo.Video_ID] = prev(videos.end());

    cout << "Video added successfully.\n";
    cout << "\nPress any key to return to the Main Menu.\n";
    cin.ignore();
    cin.get();
    clearScreen();
}

// [2] rent a video
void Video::rentVideo(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById, queue<CustomerRent*>& customers, map<int, int>& customerIndexById) {
    clearScreen();

    int customerId;

    cout << "=============================================\n";
    cout << setw(33) << "LeStack's Video Store\n";
    cout << setw(30) << "[2] RENT A VIDEO\n";
    cout << "=============================================\n";

    bool validInput = false;
    while (!validInput) {
        cout << "Enter customer ID: ";
        cin >> customerId;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n\n";
        } else {
            validInput = true;
        }
    }

    if (customerIndexById.find(customerId) == customerIndexById.end()) {
        cout << "Customer not found.\n";
        cout << "\nPress any key to return to the Main Menu.\n";
        cin.ignore();
        cin.get();
        clearScreen();
        return;
    }

    CustomerRent* customer = customers.front();
    while (customer->Customer_ID != customerId) {
        customers.push(customers.front());
        customers.pop();
        customer = customers.front();
    }

    cout << "\nCustomer ID:\t\t" << customer->Customer_ID << "\n";
    cout << "Name:\t\t\t" << customer->Name << "\n";
    cout << "Address:\t\t" << customer->Address << "\n";

    char rentMore = 'Y';
    while (rentMore == 'Y') {
        int videoId;

        cout << "\nVideos to Rent:\n";
        bool validInput = false;
        while (!validInput) {
            cout << "Enter video ID: ";
            cin >> videoId;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n\n";
            } else {
                validInput = true;
            }
        }

        if (videoIndexById.find(videoId) == videoIndexById.end()) {
            cout << "Video not found.\n";
        } else {
            auto videoIter = videoIndexById[videoId];
            Video& video = *videoIter;

            if (video.Number_of_Copies <= 0) {
                cout << "No copies available.\n";
            } else {
                cout << "\nVideo ID:\t\t" << video.Video_ID << "\n";
                cout << "Movie Title:\t\t" << video.Movie_Title << "\n";
                cout << "Number of Copies:\t" << video.Number_of_Copies - 1 << "\n";

                video.Number_of_Copies--;
                customer->Rented_Videos.push(videoId);

                saveVideosToFile(videos);
                customer->saveRentedVideosToFile(customers);
            }
        }
        if (rentMore != 'Y') {
            break;
        }

        bool yn = false;
        while (!yn){
            cout << "\nDo you want to rent another video? (Y/N): ";
            cin >> rentMore;
            rentMore = toupper(rentMore);
            if (rentMore == 'Y' || rentMore == 'N'){
                yn = true;
            } else {
                cout << "Invalid input. Please enter Y or N.\n";
            }
        }
    }

    cout << "\nRenting Video Completed. Press any key to return to the Main Menu.\n";
    cin.ignore();
    cin.get();
    clearScreen();
}

// [3] return a video
void Video::returnVideo(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById, queue<CustomerRent*>& customers, map<int, int>& customerIndexById) {
    clearScreen();

    int customerId;

    cout << "=============================================\n";
    cout << setw(33) << "LeStack's Video Store\n";
    cout << setw(30) << "[3] RETURN A VIDEO\n";
    cout << "=============================================\n";

    bool validInput = false;
    while (!validInput) {
        cout << "Enter customer ID: ";
        cin >> customerId;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n\n";
        } else {
            validInput = true;
        }
    }

    if (customerIndexById.find(customerId) == customerIndexById.end()) {
        cout << "Customer not found.\n";
        cout << "\nPress any key to return to the Main Menu.\n";
        cin.ignore();
        cin.get();
        clearScreen();
        return;
    }

    int customerIndex = customerIndexById[customerId];
    CustomerRent* customer = nullptr;
    queue<CustomerRent*> tempQueue = customers;

    for (int i = 0; !tempQueue.empty(); ++i) {
        CustomerRent* currentCustomer = tempQueue.front();
        tempQueue.pop();
        if (i == customerIndex) {
            customer = currentCustomer;
            break;
        }
        tempQueue.push(currentCustomer);
    }

    if (!customer) {
        cout << "Customer not found.\n";
        return;
    }

    customer->displayDetails(videos);

    int videoId;
    bool boolVidID = false;
    while (!boolVidID) {
        cout << "Enter video ID to return: ";
        cin >> videoId;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n\n";
        } else {
            boolVidID = true;
        }
    }

    stack<int> tempStack = customer->Rented_Videos;
    bool found = false;
    while (!tempStack.empty()) {
        if (tempStack.top() == videoId) {
            found = true;
            break;
        }
        tempStack.pop();
    }

    if (!found) {
        cout << "This video was not rented by the customer.\n";
        cout << "\nPress any key to return to the Main Menu.\n";
        cin.ignore();
        cin.get();
        clearScreen();
        return;
    }

    stack<int> newStack;
    bool videoReturned = false;
    while (!customer->Rented_Videos.empty()) {
        int currentVideoId = customer->Rented_Videos.top();
        customer->Rented_Videos.pop();
        if (currentVideoId == videoId && !videoReturned) {
            videoReturned = true;
            if (videoIndexById.find(videoId) != videoIndexById.end()) {
                videoIndexById[videoId]->Number_of_Copies++;
            } else {
                cout << "Video ID not found in inventory.\n";
            }
        } else {
            newStack.push(currentVideoId);
        }
    }
    while (!newStack.empty()) {
        customer->Rented_Videos.push(newStack.top());
        newStack.pop();
    }

    if (videoReturned) {
        saveVideosToFile(videos);
        customer->saveRentedVideosToFile(customers);
        cout << "Video returned successfully.\n";
    } else {
        cout << "Failed to return the video.\n";
    }
    cout << "\nPress any key to return to the Main Menu.\n";
    cin.ignore();
    cin.get();
    clearScreen();
}

// [4] show video details
void Video::showVideoDetails(const list<Video>& videos, const map<int, list<Video>::iterator>& videoIndexById) {
    clearScreen();

    int videoId;

    cout << "=============================================\n";
    cout << setw(33) << "LeStack's Video Store\n";
    cout << setw(34) << "[4] SHOW VIDEO DETAILS\n";
    cout << "=============================================\n";

    bool validInput = false;
    while (!validInput) {
        cout << "Enter video ID: ";
        cin >> videoId;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n\n";
        } else {
            validInput = true;
        }
    }

    auto it = videoIndexById.find(videoId);
    if (it != videoIndexById.end()) {
        it->second->displayDetails();
        cout << "\nPress any key to return to the Main Menu.\n";
        cin.ignore();
        cin.get();
        clearScreen();
    } else {
        cout << "Video not found.\n";
        cout << "\nPress any key to return to the Main Menu.\n";
        cin.ignore();
        cin.get();
        clearScreen();
    }
}

// [5] display all videos
void Video::displayAllVideos(const list<Video>& videos) {
    clearScreen();

    cout << "=============================================\n";
    cout << setw(33) << "LeStack's Video Store\n";
    cout << setw(34) << "[5] DISPLAY ALL VIDEOS\n";
    cout << "=============================================\n";
    cout << "\n";
    cout << "| " << setw(8) << "Video ID" << " | "
         << setw(40) << "Movie Title" << " | "
         << setw(10) << "Genre" << " | "
         << setw(15) << "Production" << " | "
         << setw(7) << "Copies" << " |\n";
    cout << "|----------|------------------------------------------|------------|-----------------|---------|\n";
    
    for (list<Video>::const_iterator it = videos.begin(); it != videos.end(); ++it) {
        cout << "| " << setw(8) << it->Video_ID << " | "
             << setw(40) << it->Movie_Title << " | "
             << setw(10) << it->Genre << " | "
             << setw(15) << it->Production << " | "
             << setw(7) << it->Number_of_Copies << " |\n";
    }

    cout << "\nPress any key to return to the Main Menu.\n";
    cin.ignore();
    cin.get();
    clearScreen();
}

// [6] check video availability
void Video::checkVideoAvailability(const list<Video>& videos, const map<int, list<Video>::iterator>& videoIndexById) {
    clearScreen();

    int videoId;

    cout << "=============================================\n";
    cout << setw(33) << "LeStack's Video Store\n";
    cout << setw(36) << "[6] CHECK VIDEO AVAILABILITY\n";
    cout << "=============================================\n";

    bool validInput = false;
    while (!validInput) {
        cout << "Enter video ID: ";
        cin >> videoId;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n\n";
        } else {
            validInput = true;
        }
    }

    auto it = videoIndexById.find(videoId);
    if (it != videoIndexById.end()) {
        const Video& video = *(it->second);
        cout << "Video ID             : " << "\t" << video.Video_ID << "\n"
             << "Movie Title          : " << "\t" << video.Movie_Title << "\n"
             << "Genre                : " << "\t" << video.Genre << "\n"
             << "Production           : " << "\t" << video.Production << "\n"
             << "Number of Copies     : " << "\t" << video.Number_of_Copies << "\n"
             << "Availability         : " << "\t" << (video.Number_of_Copies > 0 ? "Available" : "Not Available") << "\n";
             cout << "\nPress any key to return to the Main Menu.\n";
            cin.ignore();
            cin.get();
            clearScreen();
    } else {
        cout << "Video not found.\n";
        cout << "\nPress any key to return to the Main Menu.\n";
        cin.ignore();
        cin.get();
        clearScreen();
    }
}

// load vid from file (file manipulation)
void Video::loadVideosFromFile(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById, int& nextVideoId) {
    ifstream file("videos.txt");
    if (!file) {
        cout << "Failed to open videos.txt\n";
        return;
    }

    int id, copies;
    string title, genre, production;
    while (file >> id >> copies) {
        file.ignore();
        getline(file, title);
        getline(file, genre);
        getline(file, production);
        Video video(id, title, genre, production, copies);
        videos.push_back(video);
        videoIndexById[id] = prev(videos.end());
        nextVideoId = max(nextVideoId, id + 1);
    }
    file.close();
}

// save vid to file (file manipulation)
void Video::saveVideosToFile(const list<Video>& videos) {
    ofstream file("videos.txt");
    if (!file) {
        cout << "Failed to open videos.txt\n";
        return;
    }

    for (const auto& video : videos) {
        file << video.Video_ID << "\n" << video.Number_of_Copies << "\n" << video.Movie_Title << "\n" << video.Genre << "\n" << video.Production << "\n";
    }
    file.close();
}

// CustomerRent ADT function implementations
void CustomerRent::displayDetails() const {
    cout << "Customer ID: " << Customer_ID << "\n"
         << "Name       : " << Name << "\n"
         << "Address    : " << Address << "\n";
}

void CustomerRent::displayDetails(const list<Video>& videos) const {
    clearScreen();

    cout << "Customer ID:\t\t" << Customer_ID << "\n"
         << "Name:\t\t\t" << Name << "\n"
         << "Address:\t\t" << Address << "\n";

    if (!Rented_Videos.empty()) {
        cout << "\nList of Videos Rented:\n\n";
        cout << setw(10) << "Copies" << " | " << setw(10) << "Video ID" << " | " << setw(10) << "Customer ID" << " | " << setw(40) << "Movie Title\n";
        cout << "-----------|------------|-------------|------------------------------------------\n";

        map<int, pair<int, int>> rentedCounts;

        ifstream rentedFile("rented_videos.txt");
        if (rentedFile.is_open()) {
            int copyNumber, videoID, customerID;
            while (rentedFile >> customerID >> videoID >> copyNumber) {
                if (customerID == Customer_ID) {
                    if (rentedCounts.find(videoID) != rentedCounts.end() && rentedCounts[videoID].second == customerID) {
                        rentedCounts[videoID].first += copyNumber;
                    } else {
                        rentedCounts[videoID] = make_pair(copyNumber, customerID);
                    }
                }
            }
            rentedFile.close();
        } else {
            cerr << "Unable to open rented_videos.txt\n";
            return;
        }

        for (const auto& pair : rentedCounts) {
            int rentedVideoId = pair.first;
            int count = pair.second.first; 
            int customerId = pair.second.second;

            for (const auto& video : videos) {
                if (video.Video_ID == rentedVideoId) {
                    cout << setw(10) << count << " | " << setw(10) << video.Video_ID << " | " << setw(11) << customerId << " | " << setw(40) << video.Movie_Title << "\n";
                    break;
                }
            }
        }

        cout << "\nPress <Enter> to continue...\n";
        cin.ignore();
        cin.get();
    } else {
        cout << "\nNo videos currently rented.\n";
        cout << "\nPress <Enter> to continue...\n";
        cin.ignore();
        cin.get();
    }
}

// customer rented videos
void CustomerRent::displayRentedVideos() const {
    cout << "Rented Videos:\n";
    
    stack<int> temp = Rented_Videos;

    while (!temp.empty()) {
        cout << temp.top() << "\n";
        temp.pop();
    }
}

// child ADT rent video
void CustomerRent::rentVideo(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById) {
    int videoId;

    cout << "Enter video ID to rent: ";
    cin >> videoId;

    auto it = videoIndexById.find(videoId);
    if (it != videoIndexById.end() && it->second->Number_of_Copies > 0) {
        Rented_Videos.push(videoId);
        it->second->Number_of_Copies--;
        cout << "Video rented successfully.\n";
    } else {
        cout << "Video not available.\n";
    }
}

// child ADT return video
void CustomerRent::returnVideo(list<Video>& videos, map<int, list<Video>::iterator>& videoIndexById) {
    if (Rented_Videos.empty()) {
        cout << "No videos to return.\n";
        return;
    }

    int videoId = Rented_Videos.top();
    Rented_Videos.pop();

    auto it = videoIndexById.find(videoId);
    if (it != videoIndexById.end()) {
        it->second->Number_of_Copies++;
        cout << "Video returned successfully.\n";
    } else {
        cout << "Video ID " << videoId << " not found in the collection.\n";
    }
}

// child ADT print rented videos
void CustomerRent::printRentedVideos() const {
    stack<int> temp = Rented_Videos;

    cout << "Rented Videos:\n";

    while (!temp.empty()) {
        cout << temp.top() << "\n";
        temp.pop();
    }
}

// [7.4] list of videos rented by a customer
void CustomerRent::listRentedVideosByCustomer(const queue<CustomerRent*>& customers, const map<int, int>& customerIndexById, const list<Video>& videos) {
    clearScreen();

    cout << "=============================================\n";
    cout << setw(33) << "LeStack's Video Store\n";
    cout << setw(39) << "[7.4] VIDEOS RENTED BY CUSTOMER\n";
    cout << "=============================================\n";

    int customerId;
    bool validInput = false;

    while (!validInput) {
        cout << "Enter customer ID: ";
        cin >> customerId;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n\n";
        } else {
            validInput = true;
        }
    }

    auto it = customerIndexById.find(customerId);
    if (it == customerIndexById.end()) {
        cout << "Customer not found.\n\n";
        cout << "Press any key to return to the Main Menu.\n";
        cin.ignore();
        cin.get();
        clearScreen();
        return;
    }

    int index = it->second;
    queue<CustomerRent*> tempQueue = customers;
    CustomerRent* targetCustomer = nullptr;

    for (int i = 0; !tempQueue.empty(); ++i) {
        CustomerRent* currentCustomer = tempQueue.front();
        tempQueue.pop();
        if (i == index) {
            targetCustomer = currentCustomer;
            break;
        }
        tempQueue.push(currentCustomer);
    }

    if (targetCustomer) {
        targetCustomer->displayDetails(videos);
        clearScreen();
    } else {
        cout << "Customer not found.\n";
        cout << "Press any key to return to the Main Menu.\n\n";
        cin.ignore();
        cin.get();
        clearScreen();
    }
}

// load customers from file (file manipulation)
void CustomerRent::loadCustomersFromFile(queue<CustomerRent*>& customers, map<int, int>& customerIndexById, int& nextCustomerId) {
    ifstream file("customers.txt");
    if (!file) {
        cout << "Failed to open customers.txt\n";
        return;
    }

    int id;
    string name, address;
    while (file >> id) {
        file.ignore();
        getline(file, name);
        getline(file, address);
        CustomerRent* customer = new CustomerRent(id, name, address);
        customers.push(customer);
        customerIndexById[id] = customers.size() - 1;
        nextCustomerId = max(nextCustomerId, id + 1);
    }
    file.close();

    loadRentedVideosFromFile(customers, customerIndexById);
}

//load rented videos from file (file manipulation)
void CustomerRent::loadRentedVideosFromFile(queue<CustomerRent*>& customers, map<int, int>& customerIndexById) {
    ifstream file("rented_videos.txt");
    if (!file) {
        cerr << "Failed to open rented_videos.txt\n";
        return;
    }

    map<int, map<int, int>> rentalData;
    int customerId, videoId, count;
    while (file >> customerId >> videoId >> count) {
        rentalData[customerId][videoId] += count;
    }
    file.close();

    for (auto& customerPair : rentalData) {
        int customerId = customerPair.first;
        if (customerIndexById.find(customerId) != customerIndexById.end()) {
            CustomerRent* customer = customers.front();
            while (customer && customer->Customer_ID != customerId) {
                customers.push(customers.front());
                customers.pop();
                customer = customers.front();
            }
            if (customer) {
                while (!customer->Rented_Videos.empty()) {
                    customer->Rented_Videos.pop();
                }
                for (auto& videoPair : customerPair.second) {
                    int videoId = videoPair.first;
                    int count = videoPair.second;
                    for (int i = 0; i < count; ++i) {
                        customer->Rented_Videos.push(videoId);
                    }
                }
            }
        }
    }
}

// check if video is rented
bool CustomerRent::isVideoRented(int videoId) const {
    stack<int> temp = Rented_Videos;
    while (!temp.empty()) {
        if (temp.top() == videoId) {
            return true;
        }
        temp.pop();
    }
    return false;
}

// save customer to file (file manipulation)
void CustomerRent::saveCustomersToFile(const queue<CustomerRent*>& customers) {
    ofstream file("customers.txt");
    if (!file) {
        cout << "Failed to open customers.txt\n";
        return;
    }

    queue<CustomerRent*> temp = customers;
    while (!temp.empty()) {
        const CustomerRent* customer = temp.front();
        file << customer->Customer_ID << "\n" << customer->Name << "\n" << customer->Address << "\n";
        temp.pop();
    }
    file.close();
}

// save rented videos to file (file manipulation)
void CustomerRent::saveRentedVideosToFile(const queue<CustomerRent*>& customers) {
    map<int, map<int, int>> rentalData;

    queue<CustomerRent*> temp = customers;
    while (!temp.empty()) {
        const CustomerRent* customer = temp.front();
        stack<int> rentedVideos = customer->Rented_Videos;

        while (!rentedVideos.empty()) {
            int videoId = rentedVideos.top();
            rentalData[customer->Customer_ID][videoId]++;
            rentedVideos.pop();
        }

        temp.pop();
    }

    ofstream file("rented_videos.txt", ios::trunc);
    if (!file) {
        cerr << "Failed to open rented_videos.txt\n";
        return;
    }

    for (const auto& customerPair : rentalData) {
        int customerId = customerPair.first;
        for (const auto& videoPair : customerPair.second) {
            int videoId = videoPair.first;
            int count = videoPair.second;
            file << customerId << " " << videoId << " " << count << "\n";
        }
    }

    file.close();
}

// Customer ADT function implementations
// [7.1] add new customer
void Customer::addNewCustomer(queue<Customer*>& customers, map<int, int>& customerIndexById, int& nextCustomerId) {
    clearScreen();

    cout << "=============================================\n";
    cout << setw(33) << "LeStack's Video Store\n";
    cout << setw(34) << "[7.1] ADD NEW CUSTOMER\n";
    cout << "=============================================\n";

    string name, address;

    cout << "Enter customer name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter customer address: ";
    getline(cin, address);

    CustomerRent* newCustomer = new CustomerRent(nextCustomerId++, name, address);
    customers.push(newCustomer);
    customerIndexById[newCustomer->Customer_ID] = customers.size() - 1;

    cout << "Customer added successfully.\n\n";
    cout << "Press any key to return to the Main Menu.\n";
    cin.get();
    clearScreen();
}

// [7.2] show customer details
void Customer::showCustomerDetails(const queue<Customer*>& customers, const map<int, int>& customerIndexById) {
    clearScreen();

    cout << "=============================================\n";
    cout << setw(33) << "LeStack's Video Store\n";
    cout << setw(36) << "[7.2] SHOW CUSTOMER DETAILS\n";
    cout << "=============================================\n";

    int customerId;
    bool validInput = false;

    while (!validInput) {
        cout << "Enter customer ID: ";
        cin >> customerId;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n\n";
        } else {
            validInput = true;
        }
    }

    auto it = customerIndexById.find(customerId);
    if (it == customerIndexById.end()) {
        cout << "Customer not found.\n\n";
        cout << "Press any key to return to the Main Menu.\n";
        cin.ignore();
        cin.get();
        clearScreen();
        return;
    }

    int index = it->second;
    queue<Customer*> tempQueue = customers;
    Customer* targetCustomer = nullptr;

    for (int i = 0; !tempQueue.empty(); ++i) {
        Customer* currentCustomer = tempQueue.front();
        tempQueue.pop();
        if (i == index) {
            targetCustomer = currentCustomer;
            break;
        }
    }

    if (targetCustomer) {
        CustomerRent* rentCustomer = dynamic_cast<CustomerRent*>(targetCustomer);
        if (rentCustomer) {
            cout << "\n";
            rentCustomer->displayDetails();
            cout << "\nPress any key to return to the Main Menu.\n";
            cin.ignore();
            cin.get();
            clearScreen();
        } else {
            cout << "Customer details not available.\n";
            cout << "Press any key to return to the Main Menu.\n";
            cin.ignore();
            cin.get();
            clearScreen();
        }
    } else {
        cout << "Customer not found.\n\n";
        cout << "Press any key to return to the Main Menu.\n";
        cin.ignore();
        cin.get();
        clearScreen();
    }
}

// [7.3] display all customers
void Customer::printAllCustomers(const queue<Customer*>& customers) {
    clearScreen();

    cout << "=============================================\n";
    cout << setw(33) << "LeStack's Video Store\n";
    cout << setw(36) << "[7.3] DISPLAY ALL CUSTOMERS\n";
    cout << "=============================================\n";

    queue<Customer*> temp = customers;

    while (!temp.empty()) {
        temp.front()->displayDetails();
        temp.pop();
        cout << "---------------------------------------------\n";

    }

    cout << "Press any key to return to the Main Menu.\n";
    cin.ignore();
    cin.get();
    clearScreen();
}