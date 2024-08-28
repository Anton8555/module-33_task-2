#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <exception>
#include <assert.h>

// global program setup
const int
    numberSectors = 9,  // the number of sectors of the conditional pond
    numberFish = 1,  // the number of fish
    numberBoots = 3;  // the number of boots

// types of objects in sectors
enum typeObject {
    NONE,  // The object is not defined
    FISH,  // the fish
    BOOT   // the boot
};

// game data
std::vector<typeObject> field;  // body of water
int count;  // attempt counter

// the class of exclusion of caught fish
class Fish_Exception: public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "A fish has been caught.";
    }
};

// the caught boot exclusion class
class Boot_Exception: public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "The boot is caught.";
    }
};

// exception class of nothing caught
class Nothing_Exception: public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Nothing is caught.";
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 @brief Filling empty sectors with objects.
 @param inObject the type of object to fill in.
 @param inNumberObject the amount that needs to be filled in.
*/
void filling_with_objects(const typeObject inObject, const int inNumberObject)
{
    // list of empty sector indexes
    std::vector<int> list_is_empty;

    // collecting information about empty sectors
    list_is_empty.clear();
    for(int i = 0; i < numberSectors; i++) {
        // if the sector is empty, then remember it
        if(field[i] == NONE) {
            list_is_empty.push_back(i);
        }
    }

    // filling empty sectors with objects
    for(int i=0; i<inNumberObject; i++) {
        // calculating a random index of an empty sector
        int ind = std::rand() % list_is_empty.size();
        // filling in the sector
        field[list_is_empty[ind]] = inObject;
        // deleting information about an empty sector, since it is no longer empty
        list_is_empty.erase(list_is_empty.begin() + ind);
    }
}

/*!
 @brief The subroutine for displaying an array in an easy-to-read form.
*/
void field_print()
{
    std::cout << "Field: [";
    for(int i=0; i<numberSectors; i++) {
        switch(field[i]) {
            case NONE: std::cout << "NONE "; break;
            case FISH: std::cout << "FISH "; break;
            case BOOT: std::cout << "BOOT "; break;
        }
    }
    std::cout << "]\n";
}

/*!
 @brief Initialization of the game.
*/
void game_init()
{
    // starting a random number generator
    std::srand(time(nullptr));

    // clearing sectors
    field.resize(numberSectors);
    for(int i=0; i<numberSectors; i++) {
        field[i] = NONE;
    }

    // filling sectors with objects
    filling_with_objects(FISH, numberFish);
    filling_with_objects(BOOT, numberBoots);

    // reset the attempt counter
    count = 0;
}

/*!
 @brief The fishing rod casting routine.
*/
void game_step(const int inIndex)
{
    switch(field[inIndex]) {
        case NONE: throw Nothing_Exception(); break;
        case FISH: throw Fish_Exception(); break;
        case BOOT: throw Boot_Exception(); break;
        default: throw std::runtime_error("An error occurred during the execution of the program.");
    }
}

int main()
{
    // checking global constants for correctness
    assert((numberBoots + numberFish) <= numberSectors);

    // implementation of the game
    game_init();
    //field_print(); // for the test
    while(true) {
        int ind;

        // entering the sector index
        std::cout << "Enter the sector index: ";
        std::cin >> ind;

        // trying to catch a fish
        try
        {
            // counting attempts
            count++;
            // casting a fishing rod
            game_step(ind);
        }
        catch(const Nothing_Exception & e)  // We got into an empty sector
        {
            std::cout << e.what() << '\n';
        }
        catch(const Fish_Exception & e)  // Caught a fish
        {
            std::cout << e.what() << '\n';
            break;
        }
        catch(const Boot_Exception & e)  // Caught a boot
        {
            std::cout << e.what() << '\n';
            count = -1;
            break;
        }
        catch(...)
        {
            std::cout << "An unidentified error." << '\n';
            return 0;
        }
    }

    // the output of the game result
    std::cout << "Count = " << count << std::endl << std::endl;

    // output of the program shutdown message
    std::cout
        << "----------------------------------------------------------\n"
        << "End of program.\n";
    return 0;
}
