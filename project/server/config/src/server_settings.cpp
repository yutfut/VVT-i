#include "server_settings.h"

const std::vector<std::string> ServerSettings::valid_properties = {
    "listen", "servername", "fs_root", "database"
};

const std::vector<std::string> ServerSettings::database_valid_properties = {
    "user", "password", "host", "port", "dbname"
};

int ServerSettings::get_number_of_property(const std::string& property)
{
    int begin = 0;
    while (isspace(property[begin]))
    {
        ++begin;
    }
    int property_length = (property[property.length() - 1] == ':') ? property.length() - begin - 1 : property.length() - begin;

    for (auto iter = this->valid_properties.begin(); iter != this->valid_properties.end(); ++iter)
    {
        if (property.substr(begin, property_length) == *iter)
        {
            return iter - this->valid_properties.begin();
        }
    }

    return -1;
}

void ServerSettings::set_property(int number_of_property, std::string value)
{
    int begin = 0;
    while (isspace(value[begin]))
    {
        ++begin;
    }
    int value_length = (value[value.length() - 1] == ';') ? value.length() - begin - 1 : value.length() - begin;
    switch (number_of_property)
    {
    case LISTEN_NUMBER:
    {
        try
        {
            this->port = stoi(value.substr(begin, value_length));
        }
        catch (std::exception& e)
        {
            throw "Listen port can be only integer";
        }
        if (this->port <= 0)
        {
            throw "Listen port can be only greater than zero";
        }
        break;
    }
    case SERVERNAME_NUMBER:
        this->servername = value.substr(begin, value_length);
        break;
    case FS_ROOT:
        this->fs_root = value.substr(begin, value_length);
        break;
    }
}

int ServerSettings::get_number_of_database_property(const std::string& property)
{
    int begin = 0;
    while (isspace(property[begin]))
    {
        ++begin;
    }
    int property_length = (property[property.length() - 1] == ':') ? property.length() - begin - 1 : property.length() - begin;

    for (auto iter = this->database_valid_properties.begin(); iter != this->database_valid_properties.end(); ++iter)
    {
        if (property.substr(begin, property_length) == *iter)
        {
            return iter - this->database_valid_properties.begin();
        }
    }

    return -1;
}

void ServerSettings::set_database_property(int number_of_property, std::string value)
{
    int begin = 0;
    while (isspace(value[begin]))
    {
        ++begin;
    }
    int value_length = (value[value.length() - 1] == ';') ? value.length() - begin - 1 : value.length() - begin;
    switch (number_of_property)
    {
    case USER_NUMBER:
        this->database.user = value.substr(begin, value_length);
        break;
    case PASSWORD_NUMBER:
        this->database.password = value.substr(begin, value_length);
        break;
    case HOST_NUMBER:
        this->database.host = value.substr(begin, value_length);
        break;
    case PORT_NUMBER:
    {
        try
        {
            this->database.port = stoi(value.substr(begin, value_length));
        }
        catch (std::exception& e)
        {
            throw "Listen port can be only integer";
        }
        if (this->database.port <= 0)
        {
            throw "Listen port can be only greater than zero";
        }
        break;
    }
    case DBNAME_NUMBER:
        this->database.dbname = value.substr(begin, value_length);
        break;
    }
}

int ServerSettings::get_port()
{
    return this->port;
}

std::string ServerSettings::get_servername()
{
    return this->servername;
}

std::string ServerSettings::get_fs_root()
{
    return this->fs_root;
}

database_t ServerSettings::get_database()
{
    return this->database;;
}
