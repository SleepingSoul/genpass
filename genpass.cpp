#include <charconv>
#include <iostream>
#include <string>
#include <random>


std::string generatePassword(int length, const std::string_view characterSet)
{
    // Create a random device object
    std::random_device randDevice;

    // Create a pseudo-random number generator object
    std::mt19937 generator(randDevice());

    // Create a uniform distribution object for integers in the range [0, chars.size() - 1]
    std::uniform_int_distribution<> distribution(0, characterSet.size() - 1);

    std::string password;

    std::generate_n(std::back_inserter(password), length, [&] { return characterSet[distribution(generator)]; });

    return password;
}

int main(int argc, char** argv)
{
    constexpr const std::string_view AllChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    constexpr const std::string_view NoSpecialChars{ AllChars.cbegin(), std::prev(AllChars.cend(), 10) };

    bool useAllCharacters = true;
    int length = 30;

    const auto handleArgs = [&](std::initializer_list<std::string_view> args)
    {
        if (std::find(args.begin(), args.end(), "-ns") != args.end())
        {
            useAllCharacters = false;
        }

        if (std::find(args.begin(), args.end(), "-h") != args.end())
        {
            std::cout << "You can pass length as an argument: genpass 10 -> will generate 10 characters password.\n"
                "You can also pass '-ns' to use only letters.";
            std::exit(0);
        }
        
        for (const auto& arg : args)
        {
            int argLength = 0;
            if (std::from_chars(std::to_address(arg.begin()), std::to_address(arg.end()), argLength); argLength > 0)
            {
                length = argLength;
                break;
            }
        }
    };

    switch (argc)
    {
        case 1:
            break; // no need to handle no args
        case 2:
            handleArgs({ std::string_view(argv[1]) });
            break;
        case 3:
            handleArgs({ std::string_view(argv[1]), std::string_view(argv[2]) });
            break;
        default:
            std::cout << "Too much arguments. Use '-h' for help.";
            return 0;
    }

    std::cout << "Generating password of " << length << " characters:\n" << generatePassword(length, useAllCharacters ? AllChars : NoSpecialChars);
    return 0;
}
