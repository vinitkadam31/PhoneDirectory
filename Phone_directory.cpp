#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<list>
#include<algorithm>
#include <iterator>
#include<exception>
#include<regex>

using namespace std;

class PhoneDirectory
{
    public:
    int number;
    string firstName, lastName;
    string phoneNumber;
    string phoneNumberWithoutFormat;
};

vector<PhoneDirectory> phoneDirectoryList;
bool searchResultFound;

vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

string getPhoneNumerWIthoutFormat(string phone)
{
    phone.erase(std::remove(phone.begin(), phone.end(), '('), phone.end());
    phone.erase(std::remove(phone.begin(), phone.end(), ')'), phone.end());
    phone.erase(std::remove(phone.begin(), phone.end(), '-'), phone.end());
    return phone.c_str();
}

void showDirectory();
PhoneDirectory GetNewDirectory();
PhoneDirectory getSearchParameters();
void insertEntry(PhoneDirectory record);
void instructions();
void merge_sort(int low , int high);
void sortDirectory();
void quickSort(int low, int high);
void randomizedQuickSort(int low, int high);
void binarySearch(PhoneDirectory dirNew, int low, int high, int org_high);
void deleteEntry(PhoneDirectory record);
bool CheckIfSameName(PhoneDirectory record1, PhoneDirectory record2);
bool CheckIfSamePhone(PhoneDirectory record1, PhoneDirectory record2);

int main(int argc, char* argv[])
{
    ifstream in(argv[1]);

    if(!in) {
        cout << "Cannot open input file.\n";
        return 1;
    }
    bool isEmptyFile = true;
    try
    {
        char str[255];
        while(in)
        {
            in.getline(str, 255);
            if(in)
            {
                isEmptyFile = false;
                vector<string> v = split(str, ',');
                PhoneDirectory pd;
                pd.phoneNumber = v[1];
                size_t temp = pd.phoneNumber.find_first_not_of(" \t");
                pd.phoneNumber.erase(0,temp);

                pd.phoneNumberWithoutFormat = getPhoneNumerWIthoutFormat(v[1]);
                vector<string> name = split(v[0], ' ');
                pd.number = atoi(name[0].c_str());
                pd.firstName = name[1];
                pd.lastName = name[2];
                phoneDirectoryList.push_back(pd);
            }
        }
        in.close();
    }
    catch(exception ex)
    {
        cout<<"Wrong file format"<<endl;
        return 1;
    }
    if(isEmptyFile)
    {
        cout<<"File is empty";
        return 1;
    }

    sortDirectory();
    int input = 0;
    while(input!=5)
    {
        instructions();
        cin>>input;
        switch(input)
        {
            case 1:
                insertEntry(GetNewDirectory());
                sortDirectory();
                break;
            case 2:
                searchResultFound = false;
                binarySearch(getSearchParameters(), 0, phoneDirectoryList.size(), phoneDirectoryList.size());
                if(!searchResultFound)
                    cout<<"Name not found"<<endl;
                break;

            case 3:
                deleteEntry(getSearchParameters());
                break;

            case 4:
                showDirectory();
                break;

            default:
                break;
        }
    }
    cout<<endl;
    return 0;
}

void instructions()
{
    cout<<endl;
    cout<<"Please provide options:"<<endl;
    cout<<"Insert"<<"\t"<<"(press 1)"<<endl;
    cout<<"Search"<<"\t"<<"(press 2)"<<endl;
    cout<<"Delete"<<"\t"<<"(press 3)"<<endl;
    cout<<"Show Directory"<<"\t"<<"(press 4)"<<endl;
    cout<<"Exit (press 5)"<<endl;
}

PhoneDirectory GetNewDirectory()
{
    string firstName, lastName, phone;
    cout<<"Please enter first name"<<endl;
    cin>>firstName;
    cout<<"Please enter last name"<<endl;
    cin>>lastName;
    cout<<"Please enter 10 digit phone number with format (xxx)xxx-xxxx"<<endl;
    cin>>phone;

    PhoneDirectory newRecord;
    newRecord.firstName = firstName;
    newRecord.lastName = lastName;
    newRecord.phoneNumber = phone;
    newRecord.phoneNumberWithoutFormat = phone;

    return newRecord;
}

PhoneDirectory getSearchParameters()
{
    string firstName, lastName, phone;
    cout<<"Please enter first name"<<endl;
    cin>>firstName;
    cout<<"Please enter last name"<<endl;
    cin>>lastName;

    PhoneDirectory newRecord;
    newRecord.firstName = firstName;
    newRecord.lastName = lastName;
    return newRecord;
}

 void showDirectory()
{
    for(int i=0;i<(int)phoneDirectoryList.size();i++)
    {
        cout <<i+1<<" "<<phoneDirectoryList[i].firstName << " " << phoneDirectoryList[i].lastName<<", "<<phoneDirectoryList[i].phoneNumber<<endl;
    }
    cout<<endl;
}

bool checkIfExistingEntry(PhoneDirectory record)
{
    for(int i=0;i<(int)phoneDirectoryList.size();i++)
    {
        if(CheckIfSameName(phoneDirectoryList[i], record) && CheckIfSamePhone(phoneDirectoryList[i], record))
            return true;
    }
    return false;
}

void insertEntry(PhoneDirectory record)
{
    if(checkIfExistingEntry(record))
    {
        cout<<"Existing entry"<<endl;
    }
    else
    {
        phoneDirectoryList.push_back(record);
    }

}


void deleteEntry(PhoneDirectory record)
{
    int deletedRecordEntry = 0;
    bool deletedRecordFound =false;
    for(int i= phoneDirectoryList.size()-1;i>=0;i--)
    {
        if(CheckIfSameName(phoneDirectoryList[i], record))
        {
            deletedRecordEntry = i;
            deletedRecordFound = true;
            break;
        }
    }

    if(deletedRecordFound)
    {
        for(int i=deletedRecordEntry ;i< (int)phoneDirectoryList.size()-1;i++)
        {
            phoneDirectoryList[i] = phoneDirectoryList[i+1];
        }
        phoneDirectoryList.pop_back();
        cout<<"Record deleted successfully"<<endl;
    }
    else
    {
        cout<<"Record does not exists"<<endl;
    }
}

void sortDirectory()
{
    merge_sort(0, phoneDirectoryList.size()-1);
    quickSort(0, phoneDirectoryList.size()-1);
}

//start merge sort

string convertToUpper(string input)
{
    int i=0;
    while(input[i]!='\0')
    {
        input[i]=toupper(input[i]);
        i++;
    }
	return input;
}

bool compareNames(PhoneDirectory record1, PhoneDirectory record2)
{
    if(convertToUpper(record1.lastName)< convertToUpper(record2.lastName))
        return true;
    else if(convertToUpper(record1.lastName)> convertToUpper(record2.lastName))
        return false;
    else
    {
        if(convertToUpper(record1.firstName)< convertToUpper(record2.firstName))
            return true;
        else if(convertToUpper(record1.firstName)> convertToUpper(record2.firstName))
            return false;
        else
        {
            return true;
        }
    }
}

void merge(int low, int mid, int high)
{
    int j = mid+1;
    int inc = low;

    PhoneDirectory newDir[high-low+1];
    int k=0;

    int i = low;
    while(i <= high)
    {
        if(inc > mid)
            newDir[k++] = phoneDirectoryList[j++] ;

        else if (j > high)
            newDir[k++] = phoneDirectoryList[inc++];

        else if(compareNames(phoneDirectoryList[inc],phoneDirectoryList[j]))
            newDir[k++] = phoneDirectoryList[inc++];

       else
            newDir[k++] = phoneDirectoryList[j++];

        i++;
     }
    for (int i=0 ; i< k ;i ++)
    {
        phoneDirectoryList[ low++ ] = newDir[ i ] ;
    }

}

void merge_sort(int low , int high)
{
    if(low < high)
    {
        int mid = (low + high )/2;
        merge_sort (low , mid );
        merge_sort (mid+1 , high );

        merge(low , mid , high );
   }
}

//end merge sort

//start randomized Quick sort

bool CheckIfSameName(PhoneDirectory record1, PhoneDirectory record2)
{
    if(convertToUpper(record1.lastName) == convertToUpper(record2.lastName) && convertToUpper(record1.firstName) == convertToUpper(record2.firstName))
        return true;

    return false;
}

bool CheckIfSamePhone(PhoneDirectory record1, PhoneDirectory record2)
{
    if(record1.phoneNumber == record2.phoneNumber)
        return true;

    return false;
}

void quickSort(int low, int high)
{
    int lowForQuick = 0, highForQuick =0;
    bool foundSameName = false;
    for(int i=0;i<=high; i++)
    {
        if(i==(high) && foundSameName == true)
        {
            highForQuick = i;
            //cout<<"lowForQuick: "<<lowForQuick<<" "<<"highForQuick: "<<highForQuick<<endl;
            randomizedQuickSort(lowForQuick, highForQuick);
            lowForQuick=0;
            highForQuick=0;
            foundSameName = false;
            break;
        }
        if(CheckIfSameName(phoneDirectoryList[i], phoneDirectoryList[i+1]))
        {
            if(foundSameName ==  false)
                lowForQuick = i;
            foundSameName = true;
        }
        else if(foundSameName)
        {
            highForQuick = i;
            //cout<<"lowForQuick: "<<lowForQuick<<" "<<"highForQuick: "<<highForQuick<<endl;
            randomizedQuickSort(lowForQuick, highForQuick);
            lowForQuick=0;
            highForQuick=0;
            foundSameName = false;
        }
    }
}

bool comparePhone(PhoneDirectory record1, PhoneDirectory record2)
{
    if(record1.phoneNumber<= record2.phoneNumber)
        return true;
    else if(record1.phoneNumber> record2.phoneNumber)
        return false;

    return false;
}

void swap(int index1, int index2)
{
    PhoneDirectory temp = phoneDirectoryList[index1];
    phoneDirectoryList[index1] =  phoneDirectoryList[index2];
    phoneDirectoryList[index2] = temp;
}

int partition(int low, int high)
{
    PhoneDirectory pivot = phoneDirectoryList[high];

    int i=low-1;

    for(int j=low;j<high;j++)
    {
        if(comparePhone(phoneDirectoryList[j],pivot))
        {
            i++;
            swap(i, j);
        }
    }
    swap(i+1, high);
    return (i+1);
}

int randomizedPartition(int low, int high)
{
    int pivot1 = rand()%((high - low) + 1) + low; ;// get random number between low and high
    //cout<<"Random number between "<<low<<" and "<<high<<" is:"<<pivot<<endl;
    swap(pivot1, high);
    //return partition(low, high);

    PhoneDirectory pivot = phoneDirectoryList[high];

    int i=low-1;

    for(int j=low;j<high;j++)
    {
        if(comparePhone(phoneDirectoryList[j],pivot))
        {
            i++;
            swap(i, j);
        }
    }
    swap(i+1, high);
    return (i+1);
}

void randomizedQuickSort(int low, int high)
{
    if(low<high)
    {
        int pivot = randomizedPartition(low, high);
        randomizedQuickSort(low, pivot-1);
        randomizedQuickSort(pivot+1, high);
    }
}

//end randomized Quick sort

//start binary search

void FindAllSameElementsNearby(int mid, int org_high)
{
    for(int i=0;i<=mid;i++)
    {
        if(CheckIfSameName(phoneDirectoryList[i],phoneDirectoryList[mid]))
        {
            cout<<i+1<<" "<<phoneDirectoryList[i].firstName << " " << phoneDirectoryList[i].lastName<<", "<<phoneDirectoryList[i].phoneNumber<<endl;
        }
    }

    for(int i=mid+1;i<org_high;i++)
    {
        if(CheckIfSameName(phoneDirectoryList[i],phoneDirectoryList[mid]))
        {
            cout<<i+1<<" "<<phoneDirectoryList[i].firstName << " " << phoneDirectoryList[i].lastName<<", "<<phoneDirectoryList[i].phoneNumber<<endl;
        }
        else
        {
            break;
        }
    }
}

void binarySearch(PhoneDirectory dirNew, int low, int high, int org_high)
{
    if(low<=high)
    {
        int mid = (low+high)/2;
        if(CheckIfSameName(dirNew,phoneDirectoryList[mid]))
        {
            cout<<"Record found"<<endl;
            searchResultFound = true;
            FindAllSameElementsNearby(mid, org_high);
        }
        else if(compareNames(dirNew, phoneDirectoryList[mid]))
            binarySearch(dirNew, low, mid-1, org_high);
        else
            binarySearch(dirNew, mid+1, high, org_high);
    }
}

//end binary search
