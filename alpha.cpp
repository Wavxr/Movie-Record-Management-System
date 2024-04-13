#include <iostream>
#include <fstream>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <cstdlib> // For system("pause") and system("cls")
#include <windows.h> // for creating a console window
using namespace std;

HWND WINAPI GetConsoleWindowNT(void)  // Function to get the console window
{
	// Defining a function pointer for GetConsoleWindow
    typedef HWND WINAPI(*GetConsoleWindowT)(void);
    GetConsoleWindowT GetConsoleWindow;
    // Getting the handle of KERNEL32.DLL
    HMODULE hk32Lib = GetModuleHandle(TEXT("KERNEL32.DLL"));
    // Getting the address of GetConsoleWindow
    GetConsoleWindow = (GetConsoleWindowT)GetProcAddress(hk32Lib
    ,TEXT("GetConsoleWindow"));
	// Checking if GetConsoleWindow function exists
    if(GetConsoleWindow == NULL){
        return NULL;
    }
    // Returning the console window handle
    return GetConsoleWindow();
}

class Video {
private:
    int videoID;
    string movieTitle;
    string genre;
    string production;
    string imageFilename;

public:
	int numCopies;
	
    Video(int id, const string& title, const string& genre, const string& production, int copies, const string& image)
        : videoID(id), movieTitle(title), genre(genre), production(production), numCopies(copies), imageFilename(image) {}

    int getVideoID() const {
        return videoID;
    }

    string getMovieTitle() const {
        return movieTitle;
    }

    string getGenre() const {
        return genre;
    }

    string getProduction() const {
        return production;
    }

    int getNumCopies() const {
        return numCopies;
    }

    string getImageFilename() const {
        return imageFilename;
    }

    void rentCopy() {
        if (numCopies > 0) {
            numCopies--;
        }
    }

    void returnCopy() {
        numCopies++;
    }
};

class Customer {
private:
    int customerID;
    string name;
    string address;

public:
    Customer(int id, const string& name, const string& address)
        : customerID(id), name(name), address(address) {}

    int getCustomerID() const {
        return customerID;
    }

    string getName() const {
        return name;
    }

    string getAddress() const {
        return address;
    }
};

class VideoStore {
private:
    list<Video> videos;
    list<Customer> customers;
    unordered_map<int, list<int>> rentedVideos;

    int generateVideoID() {
    	static int videoID = 1;
    	return videos.size() + videoID;
	}

	int generateCustomerID() {
    	static int customerID = 1;
    	return customers.size() + customerID;
	}	


    void loadVideosFromFile(const string& filename) {
    ifstream file(filename);
        if (!file.is_open()){
    		 fopen("videos.txt","wb+"); // this line tells the program to create a file if there is no existing file to update
    		
    		if(!file.is_open()){
    			cout << "Cannot Open File"; 
			}
		}

        int id;
        string title;
        string genre;
        string production;
        int copies;
        string image;

        while (file >> id >> title >> genre >> production >> copies >> image) {
            Video video(id, title, genre, production, copies, image);
            videos.push_back(video);
        }

        file.close();
    }

    void loadCustomersFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()){
    		 fopen("customers.txt","wb+"); // this line tells the program to create a file if there is no existing file to update
    		
    		if(!file.is_open()){
    			cout << "Cannot Open File"; 
			}
		}

        int id;
        string name;
        string address;

        while (file >> id >> name >> address) {
            Customer customer(id, name, address);
            customers.push_back(customer);
        }

        file.close();
    }

    void loadRentedVideosFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()){
    		 fopen("rented_videos.txt","wb+"); // this line tells the program to create a file if there is no existing file to update
    		
    		if(!file.is_open()){
    			cout << "Cannot Open File"; 
			}
		}

        int customerID;
        int videoID;

        while (file >> customerID >> videoID) {
            rentedVideos[customerID].push_back(videoID);
        }

        file.close();
    }

    void saveVideosToFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening " << filename << endl;
            return;
        }

        for (const Video& video : videos) {
            file << video.getVideoID() << " "
                 << video.getMovieTitle() << " "
                 << video.getGenre() << " "
                 << video.getProduction() << " "
                 << video.getNumCopies() << " "
                 << video.getImageFilename() << endl;
        }

        file.close();
    }

    void saveCustomersToFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening " << filename << endl;
            return;
        }

        for (const Customer& customer : customers) {
            file << customer.getCustomerID() << " "
                 << customer.getName() << " "
                 << customer.getAddress() << endl;
        }

        file.close();
    }

    void saveRentedVideosToFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening " << filename << endl;
            return;
        }

        for (const auto& entry : rentedVideos) {
            int customerID = entry.first;
            const list<int>& rentedVideoIDs = entry.second;
            for (int videoID : rentedVideoIDs) {
                file << customerID << " " << videoID << endl;
            }
        }

        file.close();
    }

public:
    VideoStore() {
        loadVideosFromFile("videos.txt");
        loadCustomersFromFile("customers.txt");
        loadRentedVideosFromFile("rented_videos.txt");
    }

    ~VideoStore() {
        saveVideosToFile("videos.txt");
        saveCustomersToFile("customers.txt");
        saveRentedVideosToFile("rented_videos.txt");
    }
    
    const Customer* getCustomerByID(int customerID) const {
        auto it = find_if(customers.begin(), customers.end(), [&](const Customer& customer) {
            return customer.getCustomerID() == customerID;
        });

        if (it != customers.end()) {
            return &(*it);
        }

        return nullptr;
    }

    const list<Video>& getAllVideos() const {
        return videos;
    }

    const Video* getVideoByID(int videoID) {
        auto it = find_if(videos.begin(), videos.end(), [&](const Video& video) {
            return video.getVideoID() == videoID;
        });

        if (it != videos.end()) {
            return &(*it);
        }

        return nullptr;
    }


    void insertVideo(const string& title, const string& genre, const string& production, int copies, const string& image) {
    	int videoID = generateVideoID();
    	Video video(videoID, title, genre, production, copies, image);
    	videos.push_back(video);
    	cout << "Video added successfully. Video ID: " << videoID << endl;
    	
    	// Save the updated list of videos to the data file
    	saveVideosToFile("videos.txt");
	}


    void rentVideo(int customerID, int videoID) {
        auto it = find_if(videos.begin(), videos.end(), [&](const Video& video) {
            return video.getVideoID() == videoID;
        });

        if (it != videos.end()) {
            Video& video = *it;
            if (video.getNumCopies() > 0) {
                rentedVideos[customerID].push_back(videoID);
                video.numCopies--;
                cout << "Video rented successfully." << endl;
            } else {
                cout << "No copies available for rental." << endl;
            }
        } else {
            cout << "Video not found." << endl;
        }
        
        saveVideosToFile("videos.txt");
    	saveRentedVideosToFile("rented_videos.txt");
    }

    void returnVideo(int customerID, int videoID) {
        auto it = rentedVideos.find(customerID);
        if (it != rentedVideos.end()) {
            list<int>& rentedVideoIDs = it->second;
            auto rentedVideoIt = find(rentedVideoIDs.begin(), rentedVideoIDs.end(), videoID);
            if (rentedVideoIt != rentedVideoIDs.end()) {
                rentedVideoIDs.erase(rentedVideoIt);

                auto videoIt = find_if(videos.begin(), videos.end(), [&](const Video& video) {
                    return video.getVideoID() == videoID;
                });

                if (videoIt != videos.end()) {
                    Video& video = *videoIt;
                    video.numCopies++;
                    cout << "Video returned successfully." << endl;
                    return;
                }
            }
        }
        cout << "Video not found in the rental list." << endl;
        
        // Save the updated lists to the data files
   		saveVideosToFile("videos.txt");
    	saveRentedVideosToFile("rented_videos.txt");
    }

    void showVideoDetails(int videoID) {
        auto it = find_if(videos.begin(), videos.end(), [&](const Video& video) {
            return video.getVideoID() == videoID;
        });

        if (it != videos.end()) {
            const Video& video = *it;
            cout << "Video ID: " << video.getVideoID() << endl;
            cout << "Title: " << video.getMovieTitle() << endl;
            cout << "Genre: " << video.getGenre() << endl;
            cout << "Production: " << video.getProduction() << endl;
            cout << "Number of Copies: " << video.getNumCopies() << endl;
            cout << "Image Filename: " << video.getImageFilename() << endl;
        } else {
            cout << "Video not found." << endl;
        }
    }

    void displayAllVideos() {
        for (const Video& video : videos) {
            cout << "Video ID: " << video.getVideoID() << endl;
            cout << "Title: " << video.getMovieTitle() << endl;
            cout << "Genre: " << video.getGenre() << endl;
            cout << "Production: " << video.getProduction() << endl;
            cout << "Number of Copies: " << video.getNumCopies() << endl;
            cout << "Image Filename: " << video.getImageFilename() << endl;
            cout << "=============================" << endl;
        }
    }

    void addCustomer(const string& name, const string& address) {
        int customerID = generateCustomerID();
        Customer customer(customerID, name, address);
        customers.push_back(customer);
        cout << "Customer added successfully. Customer ID: " << customerID << endl;
    }

    void showCustomerDetails(int customerID) {
        auto it = find_if(customers.begin(), customers.end(), [&](const Customer& customer) {
            return customer.getCustomerID() == customerID;
        });

        if (it != customers.end()) {
            const Customer& customer = *it;
            cout << "Customer ID: " << customer.getCustomerID() << endl;
            cout << "Name: " << customer.getName() << endl;
            cout << "Address: " << customer.getAddress() << endl;
        } else {
            cout << "Customer not found." << endl;
        }
    }

    void printListOfCustomers() {
        for (const Customer& customer : customers) {
            cout << "Customer ID: " << customer.getCustomerID() << endl;
            cout << "Name: " << customer.getName() << endl;
            cout << "Address: " << customer.getAddress() << endl;
            cout << "=============================" << endl;
        }
    }

    void printVideosRentedByCustomer(int customerID) {
        auto it = rentedVideos.find(customerID);
        if (it != rentedVideos.end()) {
            const list<int>& rentedVideoIDs = it->second;
            cout << "Videos rented by Customer ID " << customerID << ":" << endl;
            for (int videoID : rentedVideoIDs) {
                showVideoDetails(videoID);
                cout << "-----------------------------" << endl;
            }
        } else {
            cout << "Customer not found or no videos rented." << endl;
        }
    }
};


void loadingScreen(){
		
		char intro[15]="WELCOME TO";
		char intro2[15]="MOVIE RECORD";
		char intro3[30]="MANAGEMENT SYSTEM";
		cout<<"\n\n\n\n\n\n\n\n\n\n\t\t\t      ";
		
		for(int ctr=0; ctr<strlen(intro);ctr++){ 

			cout<<intro[ctr]<<" "; 
			Sleep(80);
		}
		cout<<"\n\n\n\t\t\t     ";
		for(int ctr2=0; ctr2<strlen(intro2) ;ctr2++){

			cout<<intro2[ctr2]<<" ";
			Sleep(80);
		}
		cout<<"\n\n\n\t\t       ";
	
		for(int ctr3=0; ctr3<strlen(intro3) ;ctr3++){
		
			cout<<intro3[ctr3]<<" ";
			Sleep(80);
		}
}			
	//This function displays a loading bar that fills up as the program loads.	
void loadingbar(){
		for (int i=15;i<=100;i+=5){ 
			system("cls");
			cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t";
			cout<<"  Loading "<<i<<"%";
			cout<<"\n\n\n\n\n\n\t\t";
			char x=176; //an ascii character
			for (int j=0; j<i;j+=2){ //display a block of code that get longer as the loading % is closer to 100
				cout<<x;
			}
			Sleep(100);

		}
	}
	
void exitScreen(){
		// This function displays a message to the user upon exiting the program. It includes a decorative design using ASCII characters from the Windows.h library.
		//for design purposes, l1-li2 is an ascii characters in windows.h
		char l1 = 201;
		char l2 = 187;
		char l3 = 200;
		char l4 = 188;
		char li1 = 205;
		char li2 = 186;	
		
		cout<<"\n\n\n\n\n\n\n\n\n\n\t\t  Y O U   H A V E   E X I T E D   T H E   A P P.\n\n\n";
			cout<<"\t\t  "<<l1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<l2<<"\n";
			cout<<"\t\t  "<<li2<<"          GROUP USING NAMESPACE STD;       "<<li2<<endl;
			cout<<"\t\t  "<<li2<<"                   Members                 "<<li2<<endl;
			cout<<"\t\t  "<<li2<<"-------------------------------------------"<<li2<<endl;
			cout<<"\t\t  "<<li2<<"           John Waver O. Aguilar           "<<li2<<endl;
			cout<<"\t\t  "<<li2<<"           Franz Christian Brecio          "<<li2<<endl;
			cout<<"\t\t  "<<li2<<"           Justin Hendrick Cabral          "<<li2<<endl;
			cout<<"\t\t  "<<li2<<"             Gerardo Razon III             "<<li2<<endl;
			cout<<"\t\t  "<<l3<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<li1<<l4;
			cout<<"\n\n\n\n\n\n\n\n";
	}
	
void rentVideoOption(VideoStore& videoStore);
void rentAnotherVideoOption(VideoStore& videoStore);

int main() {
    VideoStore videoStore;

    HWND hWnd = GetConsoleWindowNT();
    MoveWindow(hWnd, 350, 50, 680, 600, TRUE); // Sizing the window for a cleaner look

    system("title MOVIE RECORD MANAGEMENT SYSTEM");

    system("Color 03");
    loadingbar();
	system("cls");
    loadingScreen();
    Sleep(300);

    int choice;
    int customerID;
    int videoID;
    string title;
    string genre;
    string production;
    int copies;
    string image;
    string name;
    string address;

    while(1) {
    	VideoStore videoStore;
		int choice;
		
		system("cls");
		system("Color 03");
		cout<<"\n\n\n\n";
    	
        cout<<"        WELCOME TO GROUP NAMESPACE STD's VIDEO RECORD MANAGEMENT SYSTEM        ";
		cout<<"\n\n--------------------------------------------------------------------------------";
        cout<<"\n\n\t\t\t\t   MAIN MENU\n\n";
		Sleep(300);
        cout<<"                             >  [1] - ADD NEW VIDEO\n";
        cout<<endl;
        Sleep(300);
        cout<<"                             >  [2] - RENT A VIDEO\n";
        cout<<endl;
        Sleep(300);
        cout<<"                             >  [3] - RETURN A VIDEO\n";
		cout<<endl;
		Sleep(300);
		cout<<"                             >  [4] - SHOW VIDEO DETAILS\n";
		cout<<endl;
		Sleep(300);
		cout<<"                             >  [5] - DISPLAY VIDEO DETAILS\n";
		cout<<endl;
		Sleep(300);
		cout<<"                             >  [6] - CHECK VIDEO AVAILABILITY\n";
		cout<<endl;
		Sleep(300);
		cout<<"                             >  [7] - CUSTOMER MAINTENTANCE\n";
		cout<<endl;
		Sleep(300);
		cout<<"                             >  [8] - EXIT\n";
		cout<<endl;
		Sleep(300);
		cout<<"\n\n--------------------------------------------------------------------------------";
        cout<<"\n\t\t\tEnter your choice: "; 
        cin>>choice; //let the user pick in main menu

        switch (choice) {
            case 1: {
            	system("cls");
                cout << "\n\n\t\tEnter title: ";
                cin.ignore();
                getline(cin, title);
                cout << "\n\n\t\tEnter genre: ";
                getline(cin, genre);
                cout << "\n\n\t\tEnter production: ";
                getline(cin, production);
                cout << "\n\n\t\tEnter number of copies: ";
                cin >> copies;
                cout << "\n\n\t\tEnter image filename: ";
                cin.ignore();
                getline(cin, image);
                videoStore.insertVideo(title, genre, production, copies, image);
                system("pause");
                break;
            }

			case 2: {
				system("cls");
    			cout << "Enter customer ID: ";
    			cin >> customerID;

    			// Display customer name and address
    			videoStore.showCustomerDetails(customerID);

    			char choice;
    			do {
    				system("cls");
       				// Display all videos
        			videoStore.displayAllVideos();
        			// Rent a video
        			cout << "Enter video ID: ";
        			cin >> videoID;
        			videoStore.rentVideo(customerID, videoID);
        			// Ask if the user wants to rent another video
        			cout << "Rent another video? (Y/N): ";
        			cin >> choice;
    			} while (choice == 'Y' || choice == 'y');

    			break;
			}
 				
            case 3: {
            	system("cls");
                cout << "\n\n\t\tEnter customer ID: ";
                cin >> customerID;
                // Display customer name and address
    			videoStore.showCustomerDetails(customerID);
    			// Display rented videos by the customer
    			videoStore.printVideosRentedByCustomer(customerID);

                cout << "\n\n\t\tEnter video ID to Return: ";
                cin >> videoID;
                videoStore.returnVideo(customerID, videoID);
                system("pause");
                break;
            }

            case 4: {
            	system("cls");
                cout << "\n\n\t\tEnter video ID: ";
                cin >> videoID;
                videoStore.showVideoDetails(videoID);
                system("pause");
                break;
            }

            case 5: {
            	system("cls");
                videoStore.displayAllVideos();
                system("pause");
                break;
            }

            case 6: {
    system("cls");
    cout << "\n\n\t\tEnter video ID: ";
    cin >> videoID;

    const Video* video = videoStore.getVideoByID(videoID);
    if (video) {
        cout << "\n\n\t\tVideo Details:\n";
        videoStore.showVideoDetails(videoID);

        if (video->getNumCopies() > 0) {
            cout << "\n\n\t\tThis movie is available for rent." << endl;
        } else {
            cout << "\n\n\t\tThis movie is currently unavailable for rent." << endl;
        }
    } else {
        cout << "\n\n\t\tVideo not found." << endl;
    }

    system("pause");
    break;
}


            case 7: {
            	system("cls");
                int customerChoice;
                cout<<"\n\n--------------------------------------------------------------------------------";
        		cout<<"\n\n\t\t\t\t   CUSTOMER MAINTENANCE\n\n";
				Sleep(300);
        		cout<<"                             >  [1] - ADD NEW CUSTOMER\n";
        		cout<<endl;
        		Sleep(300);
        		cout<<"                             >  [2] - SHOW CUSTOMER DETAILS\n";
        		cout<<endl;
        		Sleep(300);
        		cout<<"                             >  [3] - LIST OF VIDEOS RENTED BY A CUSTOMER\n";
				cout<<"\n\n--------------------------------------------------------------------------------";
        		cout<<"\n\t\t\tEnter your choice: "; 
                cin >> customerChoice;

                switch (customerChoice) {
                    case 1: {
                    	system("cls");
                        cout << "\n\n\t\tEnter name: ";
                        cin.ignore();
                        getline(cin, name);
                        cout << "\n\n\t\tEnter address: ";
                        getline(cin, address);
                        videoStore.addCustomer(name, address);
                        system("pause");
                        break;
                    }

                    case 2: {
                    	system("cls");
                        cout << "\n\n\t\tEnter customer ID: ";
                        cin >> customerID;
                        videoStore.showCustomerDetails(customerID);
                        system("pause");
                        break;
                    }

                    case 3: {
    					system("cls");
    					cout << "\n\n\t\tEnter customer ID: ";
    					cin >> customerID;

    					const Customer* customer = videoStore.getCustomerByID(customerID);
    						if (customer) {
        					cout << "\n\n\t\tCustomer Details:\n";
        					videoStore.showCustomerDetails(customerID);

        					cout << "\nList of Videos Rented by \"" << customer->getName() << "\":" << endl;
        					videoStore.printVideosRentedByCustomer(customerID);
    						} else {
        						cout << "\n\n\t\tCustomer not found." << endl;
   							}

    					system("pause");
    					break;
					}


                    default:
                        cout<<"\n\n\t\t\tInvalid Input! try again\n\n";
        				cout<<"\t\t\t";
        				system("pause");
						continue;
                }

                break;
            }

            case 8: {
            	system("cls");
        		Sleep(200);
        		exitScreen();
				exit(0);
            }

            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        	}
		}
    return 0;

}
