#include "finals-data.h"

int main() {
    queue<CustomerRent*> customers;
    map<int, int> customerIndexById;
    int nextCustomerId = 1;

    list<Video> videos;
    map<int, list<Video>::iterator> videoIndexById;
    int nextVideoId = 1;

    CustomerRent::loadCustomersFromFile(customers, customerIndexById, nextCustomerId);
    Video::loadVideosFromFile(videos, videoIndexById, nextVideoId);
    CustomerRent::loadRentedVideosFromFile(customers, customerIndexById);

    int choice;
    do {
        cout << "=============================================\n";
        cout << setw(32) << "LeStack's Video Store\n";
        cout << "=============================================\n";
        cout << "1. New Video\n";
        cout << "2. Rent a Video\n";
        cout << "3. Return a Video\n";
        cout << "4. Show Video Details\n";
        cout << "5. Display all Videos\n";
        cout << "6. Check Video Availability\n";
        cout << "7. Customer Maintenance\n";
        cout << "   - Add New Customer\n";
        cout << "   - Show Customer Details\n";
        cout << "   - Display all customers\n";       
        cout << "   - List of Videos Rented by a Customer\n";
        cout << "8. Exit Program\n\n";
        
        bool validInput = false;
        while (!validInput){
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n\n";
        } else if (choice < 1 || choice > 8) {
            cout << "Invalid choice. Please enter a number from 1 to 8.\n\n";
        } else {
            validInput = true;
            }
        }

        switch (choice) {
            case 1:
                Video::addNewVideo(videos, videoIndexById, nextVideoId);
                break;
            case 2:
                Video::rentVideo(videos, videoIndexById, customers, customerIndexById);
                break;
            case 3:
                Video::returnVideo(videos, videoIndexById, customers, customerIndexById);
                break;
            case 4:
                Video::showVideoDetails(videos, videoIndexById);
                break;
            case 5:
                Video::displayAllVideos(videos);
                break;
            case 6:
                Video::checkVideoAvailability(videos, videoIndexById);
                break;
            case 7: {
                int subChoice;
                system("cls");
                cout << "=============================================\n";
                cout << setw(33) << "LeStack's Video Store\n";
                cout << setw(37) << "[7] CUSTOMER MAINTENANCE MENU\n";
                cout << "=============================================\n";
                cout << "1. Add New Customer\n";
                cout << "2. Show Customer Details\n";
                cout << "3. Display all customers\n";       
                cout << "4. List of Videos Rented by a Customer\n\n";

                bool choiceValid = false;
                while (!choiceValid){
                cout << "Enter your choice: ";
                cin >> subChoice;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number.\n\n";
                } else if (subChoice < 1 || subChoice > 4) {
                    cout << "Invalid choice. Please enter a number from 1 to 4.\n\n";
                } else {
                    choiceValid = true;
                    }
                }
                
                switch (subChoice) {
                    case 1:
                        Customer::addNewCustomer(reinterpret_cast<queue<Customer*>&>(customers), customerIndexById, nextCustomerId);
                        break;
                    case 2:
                        cout << "\n[7.2] SHOW CUSTOMER DETAILS\n----------------------------\n";
                        Customer::showCustomerDetails(reinterpret_cast<queue<Customer*>&>(customers), customerIndexById);
                        break;
                    case 3:
                    	cout << "\n[7.3] SHOW ALL CUSTOMER DETAILS\n----------------------------\n";
                        Customer::printAllCustomers(reinterpret_cast<queue<Customer*>&>(customers));
                    	break;
                    case 4:
                        cout << "\n[7.4] LIST ALL VIDEOS RENTED BY A CUSTOMER\n------------------------------------------\n";
                        CustomerRent::listRentedVideosByCustomer(customers, customerIndexById, videos);
                        break;
                    default:
                        cout << "Invalid choice. Press <Enter> to try again.\n";
                        cin.ignore();
                        cin.get();
                        system("cls");
                        break;
                }
                break;
            }
            case 8:
                cout << "Exiting program.\n";
                Video::saveVideosToFile(videos);
                CustomerRent::saveCustomersToFile(customers);
                CustomerRent::saveRentedVideosToFile(customers);
                break;
            default:
                cout << "Invalid choice. Press <Enter> to try again.\n";
                cin.ignore();
                cin.get();
                system("cls");
                break;
        }
    } while (choice != 8);
    
    return 0;
}